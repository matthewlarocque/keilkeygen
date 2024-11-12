/**
 * @file event.c
 * 
 */
#include "event.h"

#include "slicx.h"

typedef struct {
	const char id;
	const char *v;
} list_local_t;

static const list_local_t targetList[] = {
	{ 0x01, "C51" },
	{ 0x02, "C251" },
	{ 0x03, "C166" },
	{ 0x04, "ARM" },
	{ 0x00, "" }
};

static const list_local_t lictypeList[] = {
	{ 0x01, "Cortex-M0/M0+" },
	{ 0x02, "Cortex-M0/M0+ 128K" },
	{ 0x03, "Cortex-M0/M0+ 256K" },
	{ 0x04, "Macro Assembler Kit" },
	{ 0x05, "Compiler/Assembler Kit" },
	{ 0x06, "Developers Kit" },
	{ 0x07, "Debugger" },
	{ 0x08, "Hitex Extensions" },
	{ 0x09, "Infineon Extensions" },
	{ 0x0a, "Essential/Standard Cortex-M only" },
	{ 0x0c, "Prof. Developers Kit/Plus" },
	{ 0x0d, "Limited (64K)" },
	{ 0x0e, "Basic (256K)/Community" },
	{ 0x10, "RL/Real-Time OS" },
	{ 0x11, "Professional Cortex-M only(DS5)" },
	{ 0x12, "Professional" },
	{ 0x00, "" }
};

static const char *ListGetValue(char id, 
	const list_local_t *list) {
	
	const list_local_t ** p;
	
	p = (list_local_t**)list;

	return p[id]->v;
}

static const char ListGetId(char *v, 
	const list_local_t *list) {
	
	while (list->id) {
		if (!strcmp(v, list->v)) {
			return list->id;
		}
		list++;
	}

	return 0;
}

static void DropListInit(HWND m_hWnd,
	UINT nID,
	const list_local_t *list
	) {

	HWND s_hWnd = GetDlgItem(m_hWnd, nID);

	while (list->id) {
		SendMessage(s_hWnd, CB_ADDSTRING, 0, (LPARAM)list->v);
		list++;
	}

	SendMessage(s_hWnd, CB_SETCURSEL, 0, 0);
}

static void GetCtlText(HWND m_hWnd,
	UINT nID,
	LPTSTR lpString, int nMaxCount) {

	HWND s_hWnd = GetDlgItem(m_hWnd, nID);

	GetWindowText(s_hWnd, lpString, nMaxCount);
}

static void SetCtlText(HWND m_hWnd,
	UINT nID,
	LPTSTR lpString) {

	HWND s_hWnd = GetDlgItem(m_hWnd, nID);

	SetWindowText(s_hWnd, lpString);
}

void OnTargetInit(HWND m_hWnd) {
	DropListInit(m_hWnd, IDC_TARGET, targetList);
}

void OnLicTypeInit(HWND m_hWnd) {
	DropListInit(m_hWnd, IDC_LICTYPE, lictypeList);
}

void OnCopyClick(HWND m_hWnd) {

	HGLOBAL hgMem;

	lic_t lic;

	char * hgBuf;
	
	HWND s_hWnd = GetDlgItem(m_hWnd, IDC_LICENSE);

	GetWindowText(s_hWnd, lic, sizeof(lic_t));

	if (!strlen(lic)){
		MessageBox(m_hWnd,
			"The license content is empty, please click\n"
			" the 'OBTAIN' button first!",
			"Warnning...", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenClipboard(m_hWnd)) {
		MessageBox(m_hWnd,
			"Failed to open clipboard!",
			"Warnning...", MB_OK | MB_ICONWARNING);
		return;
	}
	EmptyClipboard();

	hgMem = GlobalAlloc(GMEM_MOVEABLE, sizeof(lic_t));

	if (hgMem == NULL) {
		CloseClipboard();
		MessageBox(m_hWnd,
			"Global memory cannot be allocated!",
			"Warnning...", MB_OK | MB_ICONWARNING);
		return;
	}

	hgBuf = (char*)GlobalLock(hgMem);

	strcpy_s(hgBuf, sizeof(lic_t), lic);

	GlobalUnlock(hgMem);

	SetClipboardData(CF_TEXT, hgMem);

	CloseClipboard();

	MessageBox(m_hWnd, 
		"Happy copy success!", "Message", MB_OK);
}

void OnObtainClick(HWND m_hWnd) {

	char target[16], lictype[64];

	int err;

	lic_t lic;
	
	cid_t cid;

	lic_dev_id dev_id;

	lic_type_id type_id;

	SetCtlText(m_hWnd, IDC_LICENSE, "");

	GetCtlText(m_hWnd, IDC_CPUID, cid, sizeof(cid_t));
	GetCtlText(m_hWnd, IDC_TARGET, target, 16);
	GetCtlText(m_hWnd, IDC_LICTYPE, lictype, 64);

	if (!strlen(cid)) {
		MessageBox(m_hWnd,
			"Computer ID cannot be empty!",
			"Warnning...", MB_OK | MB_ICONWARNING);
		return;
	}

	dev_id = ListGetId(target, targetList);
	type_id = ListGetId(lictype, lictypeList);

	err = lic_make(lic, cid, dev_id, type_id, GetTickCount());

	switch (err) {
	case 0: SetCtlText(m_hWnd, IDC_LICENSE, lic);
		break;
	case -2:
		MessageBox(m_hWnd,
			"Please enter the correct Computer ID!",
			"Warnning...", MB_OK | MB_ICONWARNING);
		break;
	default:
		MessageBox(m_hWnd,
			"An unknown error occurred!",
			"Warnning...", MB_OK | MB_ICONWARNING);
		break;
	}
}
