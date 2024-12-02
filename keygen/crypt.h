/**
 * @file crypt.h
 *
 * Macro definitions and function declarations provided to users
 */
#ifndef __CRYPT_H__
#define __CRYPT_H__

#include <stdint.h>

#include "slicx.h"

#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************
 *  Miscellaneous Functions
 */
void encrypt_cid(cid_t cid);
void decrypt_cid(cid_t cid);
void encrypt_lic(lic_t lic);
int decrypt_lic(lic_t lic);

#ifdef __cplusplus
}
#endif

#endif    /* __CRYPT_H__ */
