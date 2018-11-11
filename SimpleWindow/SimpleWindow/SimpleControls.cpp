/*
	Created by Sayaka
*/
#include"SimpleWindow.h"

void SimpleWindow::InitControls() {
	hDefaultCursor = LoadCursorA(NULL, IDC_ARROW);
	hFontSW = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, "MS Gothic");
}

void SimpleWindow::FixCursor(HWND hWnd, UINT Msg) {
	if (GetFocus() == hWnd) {
		return;
	}

	switch (Msg) {
	case WM_MOUSEMOVE:
		SetCursor(hDefaultCursor);
		break;

	case WM_LBUTTONUP:
		SetFocus(hWnd);
		break;

	default:
		break;
	}
}

void SimpleWindow::SetFont(int iID) {
	SendDlgItemMessageA(hWndSW, iID, WM_SETFONT, (WPARAM)hFontSW, NULL);
}

int SimpleWindow::GetWidth(const char *cText) {
	int count = 0;

	for (int i = 0; cText[i]; i++) {
		if (cText[i] == '\r') {
			continue;
		}

		if (cText[i] == '\n') {
			int newline = GetWidth(&cText[i + 1]);

			if (newline > count * 6) {
				return newline;
			}

			break;
		}

		count++;
	}

	return count * 6;
}

int SimpleWindow::GetHeight(const char *cText) {
	int count = 1;

	for (int i = 0; cText[i]; i++) {
		if (cText[i] == '\n') {
			count++;
		}
	}

	return count * 12;
}

void SimpleWindow::Button(int iID, int X, int Y, const char *cText, int iWidth, int iHeight) {
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE;
	int width = iWidth ? iWidth : GetWidth(cText) + 12;
	int height = iHeight ? iHeight : GetHeight(cText) + 6;

	if (CreateWindowExA(NULL, "BUTTON", cText, dwStyle, X, Y, width, height, hWndSW, (HMENU)iID, hInstanceSW, NULL)) {
		SetFont(iID);
	}
}

void SimpleWindow::CheckBox(int iID, int X, int Y, const char *cText) {
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_MULTILINE;
	int width = GetWidth(cText) + 16;
	int height = GetHeight(cText) + 1;

	if (CreateWindowExA(NULL, "BUTTON", cText, dwStyle, X, Y, width, height, hWndSW, (HMENU)iID, hInstanceSW, NULL)) {
		SetFont(iID);
	}
}

void SimpleWindow::EditBox(int iID, int X, int Y, const char *cText, int iWidth, int iLine, bool bNumber) {
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER;
	int width = iWidth ? iWidth : GetWidth(cText) + 6;
	int height = iLine ? iLine * 6 + 4 : GetHeight(cText) + 4;

	if (height > 10) {
		dwStyle |= ES_MULTILINE | WS_VSCROLL;

		if (!iWidth) {
			width += 16;
		}
	}

	if (bNumber) {
		dwStyle |= ES_NUMBER;
	}

	if (CreateWindowExA(NULL, "EDIT", cText, dwStyle, X, Y, width, height, hWndSW, (HMENU)iID, hInstanceSW, NULL)) {
		SetFont(iID);
	}
}

void SimpleWindow::StaticText(int iID, int X, int Y, const char *cText, int iWidth, int iLine) {
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_LEFT;
	int width = iWidth ? iWidth : GetWidth(cText);
	int height = iLine ? iLine * 6 : GetHeight(cText);

	if (CreateWindowExA(NULL, "STATIC", cText, dwStyle, X, Y, width, height, hWndSW, (HMENU)iID, hInstanceSW, NULL)) {
		SetFont(iID);
	}
}

LRESULT SimpleWindow::ReadOnly(int iID, bool bReadOnly) {
	return SendDlgItemMessageA(hWndSW, iID, EM_SETREADONLY, bReadOnly, NULL);
}

BOOL SimpleWindow::SetText(int iID, const char *cText) {
	return SetDlgItemTextA(hWndSW, iID, cText);
}

void SimpleWindow::GetText(int iID, std::string &out) {
	DWORD dwLength = SendDlgItemMessageA(hWndSW, iID, WM_GETTEXTLENGTH, NULL, NULL);
	char *cText = new char[dwLength + 1];

	GetDlgItemTextA(hWndSW, iID, cText, dwLength + 1);

	out = cText;

	delete[] cText;
}