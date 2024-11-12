/**
 * @file main.c
 * 
 */
#include "dlgx.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DlgxSetInstance(hInstance);

	return DialogBox(
		hInstance,
		MAKEINTRESOURCE(IDD_KEYGEN),
		NULL,
		DlgxWindowProc);
}
