/**
 * @file slicx.h
 *
 * Macro definitions and function declarations provided to users
 */
#ifndef __SLICX_H__
#define __SLICX_H__

#include <stdint.h>

#include "mcid.h"

typedef enum {
	DEVICE_C51 = 1,
	DEVICE_C251,
	DEVICE_C166,
	DEVICE_ARM
} lic_dev_id;

typedef enum {
	MDK_CORTEX_M0_M0PLUS = 1,
	MDK_CORTEX_M0_M0PLUS_128K,
	MDK_CORTEX_M0_M0PLUS_256K,
	A_MACRO_ASSEMBLER_KIT,
	C_COMPILER_ASSEMBLER_KIT,
	DK_DEVELOPERS_KIT,
	DB_DEBUGER,
	HTX_HITEX_EXTENSIONS,
	IFX_INFINEON_EXTENSIONS,
	MDK_STANDARD_CORTEX_M_ONLY,
	MDK_PLUS,
	PK_PROF_DEVELOPERS_KIT,
	MDK_64K,
	MDK_BASIC_256K,
	RL,
	ARTX_REAL_TIME_OS,
	MDK_PROFESSIONAL_CORTEX_M_ONLY,
	MDK_PROFESSIONAL
} lic_type_id;

typedef char lic_t[36];

#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************
 *  Miscellaneous Functions
 */
int lic_make(lic_t lic, cid_t cid, lic_dev_id dev_id, lic_type_id type_id, uint32_t tick);

#ifdef __cplusplus
}
#endif

#endif    /* __SLICX_H__ */
