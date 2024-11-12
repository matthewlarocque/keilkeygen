/**
 * @file mcid.c
 *
 */
#include "mcid.h"
#include <string.h>

/**
 * @function mcid_check
 * 
 * check cid format
 *
 * @parameter
 *   cid : CID string (in)
 *
 * @return : 0 means no errors;
 */
int mcid_check(cid_t cid) {
	
	int num;

	if (!cid) { return -1; }
	
	num = strlen(cid);

	if (num != 11) {
		return -2;
	}

	for (num = 0; num < 11; num++) {
		if (num == 5) {
			if (cid[num] != '-') {
				return -3;
			}
			continue;
		}

		if (cid[num] < '0') {
			return -3;
		}

		if (cid[num] > '9' && cid[num] < 'A') {
			return -3;
		}

		if (cid[num] >'Z') {
			return -3;
		}
	}

	return 0;
}
