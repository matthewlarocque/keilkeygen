/**
 * @file dlgx.c
 * 
 */
#include "dlgx.h"

#include "event.h"

static HINSTANCE m_hInstance;

static BOOL DlgxOnCtlColor(void) {
	return (BOOL)(LRESULT)CreateSolidBrush(RGB(0, 44, 73));
}

static void DlgxOnCommand(HWND m_hWnd,
	UINT nID) {
	switch (nID) {
	case IDC_CLOSE:
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
		break;
	case IDC_COPY:
		OnCopyClick(m_hWnd);
		break;
	case IDC_OBTAIN:
		OnObtainClick(m_hWnd);
		break;
	default: break;
	}
}

static void DlgxOnDrawItem(HWND m_hWnd,
	UINT nID,
	LPARAM lParam) {

	LPDRAWITEMSTRUCT lpdis = (DRAWITEMSTRUCT*)lParam;

	if (nID == IDC_CLOSE) {

		SIZE size;

		GetTextExtentPoint32(lpdis->hDC, "X", 1, &size);

		SetTextColor(lpdis->hDC, RGB(255, 255, 255));

		SetBkColor(lpdis->hDC, RGB(0, 44, 73));

		ExtTextOut(lpdis->hDC,

			((lpdis->rcItem.right - lpdis->rcItem.left) - size.cx) / 2,

			((lpdis->rcItem.bottom - lpdis->rcItem.top) - size.cy) / 2,

			ETO_OPAQUE | ETO_CLIPPED, &lpdis->rcItem, "X", 1, NULL);

		UINT edge = EDGE_RAISED;
		
		if (lpdis->itemState & ODS_SELECTED) {
			edge = EDGE_SUNKEN;
		}

		DrawEdge(lpdis->hDC, &lpdis->rcItem, edge, BF_RECT);
	}
}

static void DlgxOnInitDialog(HWND m_hWnd) {
	
	HICON hIcon = LoadIconA(m_hInstance, MAKEINTRESOURCE(IDI_KEYGEN));

	SendMessageA(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessageA(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	ShowWindow(CreateWindowEx(
		0,
		"BUTTON",
		NULL,
		WS_CHILD | BS_OWNERDRAW,
		424,
		1,
		20,
		20,
		m_hWnd,
		(HMENU)IDC_CLOSE,
		(HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE),
		NULL
		), SW_SHOW);

	OnTargetInit(m_hWnd);
	
	OnLicTypeInit(m_hWnd);
}

static void DlgxOnLBtnDown(HWND m_hWnd) {
	
	POINT point;

	GetCursorPos(&point);

	ScreenToClient(m_hWnd, &point);
	
	SendMessage(m_hWnd,
		WM_NCLBUTTONDOWN,
		HTCAPTION,
		MAKELPARAM(point.x, point.y)
		);
}

static void DlgxOnPaint(HWND m_hWnd) {
	
	PAINTSTRUCT ps;

	HDC hdc = BeginPaint(m_hWnd, &ps);

	HBITMAP hBmp = LoadBitmap(
		m_hInstance,
		MAKEINTRESOURCE(IDB_KEYGEN)
		);

	FillRect(hdc, &ps.rcPaint,
		CreatePatternBrush(hBmp));

	EndPaint(m_hWnd, &ps);
}

HINSTANCE DlgxGetInstance(void) {
	return m_hInstance;
}

void DlgxSetInstance(HINSTANCE hInst) {
	m_hInstance = hInst;
}

INT_PTR CALLBACK DlgxWindowProc(
	_In_  HWND m_hWnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	) {

	switch (uMsg) {
	case WM_CLOSE:
		EndDialog(m_hWnd, 0);
		break;
	case WM_COMMAND:
		DlgxOnCommand(
			m_hWnd,
			LOWORD(wParam));
		break;
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORBTN:
		return DlgxOnCtlColor();
	case WM_DRAWITEM:
		DlgxOnDrawItem(
			m_hWnd,
			LOWORD(wParam),
			lParam);
		break;
	case WM_INITDIALOG:
		DlgxOnInitDialog(m_hWnd);
		break;
	case WM_LBUTTONDOWN:
		DlgxOnLBtnDown(m_hWnd);
		break;
	case WM_PAINT:
		DlgxOnPaint(m_hWnd);
		break;
		/*case WM_SHOWWINDOW:
		PlaySound(
			MAKEINTRESOURCE(IDR_KEYGEN),
			NULL,
			SND_LOOP | SND_ASYNC | SND_RESOURCE
			);
		break;*/
	default: break;
	}

	return FALSE;
}
