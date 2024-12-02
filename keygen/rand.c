/**
 * @file rand.c
 *
 */
#include "rand.h"

/**
 * @function random3
 * 
 * Park and Miller's MINSTD generator
 *
 * @parameter
 *   seed : random seed, ignored if 0
 *
 * @return : random number;
 */
uint32_t random3(uint32_t seed) {

	static uint64_t next;

	if (seed) {
		next = seed & 0x3FFFFFFF;
	}

	next = next ? next : 1;

	next = next * 48271;

	next = next % 2147483647;

	return (uint32_t)next;
}
