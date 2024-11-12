/**
 * @file event.h
 *
 * Macro definitions and function declarations provided to users
 */
#ifndef __EVENT_H
#define __EVENT_H

#include "dlgx.h"

#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************
 *  Miscellaneous Functions
 */
void OnTargetInit(HWND m_hWnd);
void OnLicTypeInit(HWND m_hWnd);
void OnCopyClick(HWND m_hWnd);
void OnObtainClick(HWND m_hWnd);

#ifdef __cplusplus
}
#endif

#endif    /* __EVENT_H */
