/**
 * @file rdx36.c
 *
 */
#include "rdx36.h"

/**
 * @function rdx36tochar
 * 
 * convert to character
 *
 * @parameter
 *   n : 0 ~ 35
 *
 * @return : '0' to '9' or 'A' to 'Z';
 */
char rdx36tochar(uint8_t n) {
	
	if (n >= 10) {
		n += 7;
	}

	n += 48;

	/* 'O' will be treated as '0' */
	if (n == 79) {
		n = 48;
	}

	return n;
}

/**
 * @function chartordx36
 * 
 * convert to number
 *
 * @parameter
 *   c : '0' to '9' or 'A' to 'Z'
 *
 * @return : 0 ~ 35;
 */
uint8_t chartordx36(char c) {
	
	/* 'O' will be treated as '0' */
	if (c == 79) {
		c = 48;
	}

	c -= 48;

	if (c >= 10) {
		c -= 7;
	}

	return c;
}
