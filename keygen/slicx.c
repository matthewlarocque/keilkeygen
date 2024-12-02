#include <stdlib.h>
#include <string.h>

/**
 * @file slicx.c
 *
 */
#include "slicx.h"

#include "crypt.h"
#include "rand.h"
#include "rdx36.h"

/**
 * @function lic_rand
 * 
 * initialize lic, and fill flags and random
 * values for lic
 *
 * @parameter
 *    lic : LIC string (in/out)
 *   tick : the tick value is used to generate
 *          random numbers
 *
 * @return : none;
 */
static void lic_rand(lic_t lic, uint32_t tick) {
	
	uint8_t val;

	int i;

	random3(tick);

	for (i = 0; i < 35; i++) {

		if ((i ==  5) || (i == 11) ||
			(i == 17) || (i == 23) ||
			(i == 29)) {
			lic[i] = '-'; continue;
		}
		
		lic[i] = '0';
	}

	lic[i] = '\0';

	/* fill flag value for lic */
	lic[14] = '1';
	lic[24] = '1';
	lic[31] = '1';
	lic[34] = '1';

	/* fill random values for lic */
	for (i = 7; i < 11 ;i++) {
		
		val = random3(0) % 36;
		
		lic[i] = rdx36tochar(val);
	}
}

/**
 * @function lic_type
 * 
 * fill license type value for lic
 *
 * @parameter
 *   lic : LIC string (in/out)
 *    id : license type id
 *
 * @return : none;
 */
static void lic_type(lic_t lic, lic_type_id id) {
	
	lic[4] = 'C'; /* permanent, fewer restrictions */

	switch (id) {
	case MDK_CORTEX_M0_M0PLUS: lic[0] = '1'; break;
	case MDK_CORTEX_M0_M0PLUS_128K: lic[0] = '2'; break;
	case MDK_CORTEX_M0_M0PLUS_256K: lic[0] = '3'; break;
	case A_MACRO_ASSEMBLER_KIT: lic[0] = 'A'; break;
	case C_COMPILER_ASSEMBLER_KIT: lic[0] = 'C'; break;
	case DK_DEVELOPERS_KIT: lic[0] = 'D'; break;
	case DB_DEBUGER: lic[0] = 'G'; break;
	case HTX_HITEX_EXTENSIONS: lic[0] = 'H'; break;
	case IFX_INFINEON_EXTENSIONS: lic[0] = 'I'; break;
	case MDK_STANDARD_CORTEX_M_ONLY: lic[0] = 'J'; break;
	case MDK_PLUS:
	case PK_PROF_DEVELOPERS_KIT: lic[0] = 'K'; break;
	case MDK_64K: lic[0] = 'N'; break;
	case MDK_BASIC_256K: lic[0] = 'Q'; break;
	case RL:
	case ARTX_REAL_TIME_OS: lic[0] = 'R'; break;
	case MDK_PROFESSIONAL_CORTEX_M_ONLY: lic[0] = 'S'; break;
	default: lic[0] = 'T'; break;
	}

	lic[13] = '0'; /* any manufacturer allowed */
}

/**
 * @function lic_dev
 * 
 * fill target device value for lic
 *
 * @parameter
 *   lic : LIC string (in/out)
 *    id : target device id
 *
 * @return : none;
 */
static void lic_dev(lic_t lic, lic_dev_id id) {

	switch (id) {
	case DEVICE_C51: lic[1] = '1'; break;
	case DEVICE_C251: lic[1] = '2'; break;
	case DEVICE_C166: lic[1] = '6'; break;
	default: lic[1] = 'A'; break;
	}
}

/**
 * @function lic_support
 * 
 * fill support end date for lic
 *
 * @parameter
 *   lic : LIC string (in/out)
 *
 * @return : none;
 */
static void lic_support(lic_t lic) {
	// U: June, A: August, D: December
	lic[2] = 'D'; /*  December */
	lic[3] = 'Z'; /* 2032: coff('Z') + 1998*/
}

/**
 * @function lic_sum1
 * 
 * calculate and fill sum1 for lic
 *
 * @parameter
 *   lic : LIC string (in/out)
 *
 * @return : none;
 */
static void lic_sum1(lic_t lic) {
	
	uint32_t r, t;

	t = strtol(&lic[12], 0, 36);

	r = 0;

	if (t) {
		r = strtol(&lic[0], 0, 36);

		t *= 7;
		r /= t;
		r += 0x41c64e6d;
		r %= 36;
	}

	lic[12] = rdx36tochar(r);
}

/**
 * @function lic_sum2
 * 
 * calculate and fill sum2 for lic
 *
 * @parameter
 *   lic : LIC string (in/out)
 *
 * @return : none;
 */
static void lic_sum2(lic_t lic) {

	uint32_t r = 0;

	r += lic[0] ^ lic[1];

	r += lic[1] ^ lic[12];

	r += 4 * lic[0];

	lic[15] = rdx36tochar(r % 36);
}

/**
 * @function lic_sum3
 * 
 * calculate and fill sum3 for lic
 *
 * @parameter
 *   lic : LIC string (in/out)
 *
 * @return : none;
 */
static void lic_sum3(lic_t lic) {
	
	uint32_t r, t;

	int i, s;

	uint8_t m_tb[] = {
		0x02, 0x03, 0x04, 0x05, 0x07, 0x05, 0x0d, 0x03,
		0x0b, 0x00
	};

	r = chartordx36(lic[0]);

	for (i = 1; i < 17; i++) {

		if ((i > 4) && (i < 12)) {
			continue;
		}

		if (i > 4) {
			s = i - 8;
		}
		else {
			s = i - 1;
		}

		t = m_tb[s] * r;

		t ^= chartordx36(lic[i]);

		r += t & 0x3f;
	}

	r = rdx36tochar(r % 36);

	r = chartordx36(r);

	r += 7 * chartordx36(lic[10]);

	r += 3 * chartordx36(lic[9]);

	r += 5 * chartordx36(lic[8]);

	r += 11 * chartordx36(lic[7]);

	lic[6] = rdx36tochar(r % 36);
}

/**
 * @function lic_mcid
 * 
 * fill computer id value for lic
 *
 * @parameter
 *   lic : LIC string (in/out)
 *   cid : CID string (in/out)
 *
 * @return : none;
 */
static void lic_mcid(lic_t lic, cid_t cid) {

	decrypt_cid(cid);

	lic[18] = cid[1];
	lic[19] = cid[2];
	lic[20] = cid[3];
	lic[21] = cid[4];
	lic[22] = cid[6];
}

/**
 * @function lic_regist
 * 
 * fill registry related values for lic
 *
 * @parameter
 *   lic : LIC string (in/out)
 *
 * @return : none;
 */
static void lic_regist(lic_t lic) {

	if (lic[4] == 'C') {
		/* When the conditions are met, the
		following values are required */
		lic[16] = 'Z';

		lic[25] = '0';
		lic[26] = '0';
		lic[27] = '0';
		lic[28] = '0';
	}
}

/**
 * @function lic_make
 * 
 * make a lic and encrypt the lic
 *
 * @parameter
 *       lic : LIC string (in/out)
 *       cid : CID string (in/out)
 *    dev_id : target device id
 *   type_id : license type id
 *      tick : the tick value is used to generate
 *             random numbers
 *
 * @return : 0 means no errors;
 */
int lic_make(lic_t lic, cid_t cid, lic_dev_id dev_id, lic_type_id type_id, uint32_t tick) {
	
	if (!lic) {
		return -1;
	}
	
	if (mcid_check(cid)) {
		return -2; 
	}

	/* Do not change the order in which functions are
	called unless you know what you are doing. */
	lic_rand(lic, tick);

	lic_type(lic, type_id);

	lic_regist(lic);

	lic_dev(lic, dev_id);

	lic_support(lic);

	lic_sum1(lic);
	lic_sum2(lic);
	lic_sum3(lic);

	lic_mcid(lic, cid);

	encrypt_lic(lic);

	return 0;
}
