/**
 * @file rand.h
 *
 * Macro definitions and function declarations provided to users
 */
#ifndef __RAND_H__
#define __RAND_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************
 *  Miscellaneous Functions
 */
uint32_t random3(uint32_t seed);

#ifdef __cplusplus
}
#endif

#endif    /* __RAND_H__ */
