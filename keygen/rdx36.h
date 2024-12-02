/**
 * @file rdx36.h
 *
 * Macro definitions and function declarations provided to users
 */
#ifndef __RDX36_H__
#define __RDX36_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************
 *  Miscellaneous Functions
 */
char rdx36tochar(uint8_t n);
uint8_t chartordx36(char c);

#ifdef __cplusplus
}
#endif

#endif    /* __RDX36_H__ */
