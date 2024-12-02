/**
 * @file crypt.c
 * 
 * Used for CID and LIC encryption and decryption
 */
#include "crypt.h"

/* The start subscript of CID for encryption/decryption */
#define START_SC_CID     (0x07)
/* The start subscript of LIC for encryption/decryption */
#define START_SC_LIC     (0x0e)

/* The subscript of the LIC checksum */
#define CKSUM_LIC_HSC    (0x18)
#define CKSUM_LIC_LSC    (0x1f)

/* The CID string encryption/decryption subscript table */
static uint8_t cds_tb[] = {
	0x09, 0x1, 0x03, 0x02, 0x04, 0x06, 0x08, 0x0a,
	START_SC_CID
};

/* The LIC string encryption/decryption subscript table */
static uint8_t lcs_tb[] = {
	0x16, 0x15, 0x14, 0x13, 0x12, 0x10, 0x0f, 0x0d,
	0x0c, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x04, 0x03,
	0x02, 0x01, 0x00, 0x1f, 0x18, 0x1a, 0x1e, 0x20,
	0x19, 0x1b, 0x21, 0x1c, 0x22,
	START_SC_LIC
};

/**
 * @function coff
 * 
 * character offset conversion
 *
 * @parameter
 *     c : character or offset
 *
 * @return : character or offset;
 */
static char coff(char c) {
    if (c < 35) {
        c += (c >= 10) ? 55 : 48;
        if (c >= 79) ++c;
    } else {
        if (c == 79) return '0';
        if (c > 79) --c;
        c -= 48;
        if (c >= 10) c -= 7;
    }
    return c;
}

/**
 * @function encrypt_ch
 * 
 * encrypt a character, 'O' will be treated as '0'
 *
 * @parameter
 *     c : '0' to '9' or 'A' to 'Z'
 *   off : offset (in/out), [0, 34]
 *
 * @return : '0' to '9' or 'A' to 'Z';
 */
static char encrypt_ch(char c, uint8_t *off) {
    uint8_t _off = coff(c);
    _off = (_off + *off) % 35;
    if (_off < 32) _off ^= 26;
    *off = (_off + 3) % 35;
    return coff(_off);
}

/**
 * @function decrypt_ch
 * 
 * decrypt a character, 'O' will be treated as '0'
 *
 * @parameter
 *     c : '0' to '9' or 'A' to 'Z'
 *   off : offset (in/out), [0, 34]
 *
 * @return : '0' to '9' or 'A' to 'Z';
 */
static char decrypt_ch(char c, uint8_t *off) {
    uint8_t _off = coff(c);
    uint8_t t = _off;
    if (_off < 32) _off ^= 26;
    _off = (_off < *off) ? (_off + 35 - *off) : (_off - *off);
    *off = (t + 3) % 35;
    return coff(_off);
}

/**
 * @function encrypt_cid
 * 
 * encrypt cid ...
 *
 * @parameter
 *   cid : CID string (in/out)
 *
 * @return : none;
 */
void encrypt_cid(cid_t cid) {
    uint8_t off = coff(cid[START_SC_CID]);
    const uint8_t *p = cds_tb;
    while (*p != START_SC_CID) {
        cid[*p] = encrypt_ch(cid[*p], &off);
        p++;
    }
}

/**
 * @function decrypt_cid
 * 
 * decrypt cid ...
 *
 * @parameter
 *   cid : CID string (in/out)
 *
 * @return : none;
 */
void decrypt_cid(cid_t cid) {
    uint8_t off = coff(cid[START_SC_CID]);
    const uint8_t *p = cds_tb;
    while (*p != START_SC_CID) {
        cid[*p] = decrypt_ch(cid[*p], &off);
        p++;
    }
}

/**
 * @function start_lic
 * 
 * calculate lic start character
 *
 * @parameter
 *   lic : LIC string (in)
 *
 * @return : lic start character;
 */
static char start_lic(lic_t lic) {
	uint32_t t0, t1, t2;
	uint8_t off, *p;
	uint8_t s_tb[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x06, 0x07, 0x08,
		0x09, 0x0a, 0x0c, 0x0d, 0x0f, 0x10, 0x12, 0x13,
		0x14, 0x15, 0x16, 0xff,
	};
	t0 = 0;
	p = s_tb;
	while (*p != 0xff) {
		off = coff(lic[*p++]);
		t2 = 0;
		if (off & 0x01) { t2 |= 0x00000001; }
		if (off & 0x02) { t2 |= 0x00000008; }
		if (off & 0x04) { t2 |= 0x00000020; }
		if (off & 0x08) { t2 |= 0x00000100; }
		if (off & 0x10) { t2 |= 0x00000400; }
		if (off & 0x20) { t2 |= 0x00002000; }
		if (off & 0x40) { t2 |= 0x00010000; }
		if (off & 0x80) { t2 |= 0x00040000; }
		t1 = t0 & 0x80000000;
		t0 = t2 ^ (t0 << 1);
		if (t1) {
			t0 ^= 0x00400007;
		}
	}
	t1 = t0 >> 5;
	t1 = t0 | t1;
	t2 = t0 >> 10;
	t0 = t2 ^ t1;
	off = t0 % 35;
	return coff(off);
}

/**
 * @function cksum_lic
 * 
 * calculate lic checksum
 *
 * @parameter
 *   lic : LIC string (in)
 *
 * @return : lic checksum;
 */
static uint32_t cksum_lic(lic_t lic) {
    uint32_t sum = 0;
    for (int i = 0; i < 35; i++) {
        if (i == 5 || i == 11 || i == 17 || i == 23 || 
            i == 24 || i == 29 || i == 31) continue;
        sum += coff(lic[i]);
    }
    return sum % 1225;
}

/**
 * @function encrypt_lic
 * 
 * encrypt lic ...
 *
 * @parameter
 *   lic : LIC string (in/out)
 *
 * @return : none;
 */
void encrypt_lic(lic_t lic) {
    lic[START_SC_LIC] = start_lic(lic);
    uint32_t sum = cksum_lic(lic);
    lic[CKSUM_LIC_HSC] = coff(sum / 35);
    lic[CKSUM_LIC_LSC] = coff(sum % 35);

    uint8_t off = coff(lic[START_SC_LIC]);
    const uint8_t *p = lcs_tb;
    while (*p != START_SC_LIC) {
        lic[*p] = encrypt_ch(lic[*p], &off);
        p++;
    }
}

/**
 * @function decrypt_lic
 * 
 * decrypt lic ...
 *
 * @parameter
 *   lic : LIC string (in/out)
 *
 * @return : 0 means no errors;
 */
int decrypt_lic(lic_t lic) {
    uint8_t off = coff(lic[START_SC_LIC]);
    const uint8_t *p = lcs_tb;
    while (*p != START_SC_LIC) {
        lic[*p] = decrypt_ch(lic[*p], &off);
        p++;
    }

    uint32_t sum = coff(lic[CKSUM_LIC_HSC]) * 35 + coff(lic[CKSUM_LIC_LSC]);
    if (sum != cksum_lic(lic)) return -1;
    if (start_lic(lic) != lic[START_SC_LIC]) return -2;
    return 0;
}
