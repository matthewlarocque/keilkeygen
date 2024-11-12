/**
 * @file dlgx.h
 *
 * Macro definitions and function declarations provided to users
 */
#ifndef __DLGX_H
#define __DLGX_H

#include <Windows.h>

#include "assert.h"

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#pragma comment(lib, "Winmm.lib")

#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************
 *  Miscellaneous Functions
 */
void DlgxSetInstance(HINSTANCE hInst);

HINSTANCE DlgxGetInstance(void);

INT_PTR CALLBACK DlgxWindowProc(
	_In_  HWND hWnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	);

#ifdef __cplusplus
}
#endif

#endif    /* __DLGX_H */
