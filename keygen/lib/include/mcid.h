/**
 * @file mcid.h
 *
 * Macro definitions and function declarations provided to users
 */
#ifndef __MCID_H__
#define __MCID_H__

#include <stdint.h>

typedef char cid_t[12];

#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************
 *  Miscellaneous Functions
 */
int mcid_check(cid_t cid);

#ifdef __cplusplus
}
#endif

#endif    /* __MCID_H__ */
