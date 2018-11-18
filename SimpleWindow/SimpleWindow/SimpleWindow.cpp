/*
	SimpleWindow Library
	Created by Sayaka
*/
#include"SimpleWindow.h"

/*
	メインウィンドウ
*/

SimpleWindow::SimpleWindow(HINSTANCE hInstance, const char *cTitle, int iWidth, int iHeight, void (*vCreateControls)(SimpleWindow *sw)) {
	WNDCLASSEXA wcx;
	std::string str = cTitle;

	// クラス名
	str += "Class";

	hInstanceSW = hInstance;
	CreateControls = vCreateControls;

	ZeroMemory(&wcx, sizeof(wcx));

	wcx.cbSize = sizeof(wcx);
	wcx.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcx.hInstance = hInstance;
	wcx.lpfnWndProc = SimpleWindowProc;
	wcx.lpszClassName = str.c_str();
	wcx.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassExA(&wcx)) {
		return;
	}

	HWND hWnd = CreateWindowExA(NULL, str.c_str(), cTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, this);

	if (hWnd) {
		// ウィンドウサイズ変更
		AdjustWindow(hWnd, iWidth, iHeight);

		MSG lpMsg;
		while (GetMessageA(&lpMsg, NULL, NULL, NULL) > 0) {
			TranslateMessage(&lpMsg);
			DispatchMessageA(&lpMsg);
		}

		// 手動で閉じる
		SendMessageA(hWnd, WM_CLOSE, 0, 0);
	}

	UnregisterClassA(str.c_str(), hInstance);
}

SimpleWindow::~SimpleWindow() {
	if (functiontable) {
		delete functiontable;
	}

	if (listview) {
		delete listview;
	}
}

LRESULT CALLBACK SimpleWindow::SimpleWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	SimpleWindow *sw;
	NMHDR *nh;

	switch (Msg) {
		// コントロール生成
	case WM_CREATE:
		sw = (SimpleWindow *)((CREATESTRUCTA *)lParam)->lpCreateParams;
		SetWindowLongA(hWnd, GWL_USERDATA, (LONG)sw);
		sw->hWndSW = hWnd;

		if (sw->CreateControls) {
			sw->InitControls();
			sw->CreateControls(sw);
		}

		break;
		// コントロールに関連付けた関数を呼び出す
	case WM_COMMAND:
		sw = (SimpleWindow *)GetWindowLongA(hWnd, GWL_USERDATA);
		if (sw->functiontable) {
			FunctionTable *ft = sw->FindFunction(LOWORD(wParam));
			if (ft) {
				if (ft->function) {
					ft->function(sw);
				}
			}
		}
		break;

	case WM_NOTIFY:
		nh = (NMHDR *)lParam;
		if (nh->code == NM_CLICK) {
			sw = (SimpleWindow *)GetWindowLongA(hWnd, GWL_USERDATA);
			if (sw->listview) {
				SimpleListView *slv = sw->FindListView(nh->hwndFrom);
				if (slv) {
					if (slv->notify) {
						slv->notify(sw);
					}
				}
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		// カーソルバグ対応
	default:
		sw = (SimpleWindow *)GetWindowLongA(hWnd, GWL_USERDATA);
		sw->FixCursor(hWnd, Msg);
		break;
	}

	return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

void SimpleWindow::AdjustWindow(HWND hWnd, int nWidth, int nHeight) {
	RECT W_RECT, C_RECT;

	GetWindowRect(hWnd, &W_RECT);
	GetClientRect(hWnd, &C_RECT);
	SetWindowPos(hWnd, HWND_TOP, NULL, NULL, nWidth + (W_RECT.right - W_RECT.left) - (C_RECT.right - C_RECT.left), nHeight + (W_RECT.bottom - W_RECT.top) - (C_RECT.bottom - C_RECT.top), SWP_NOMOVE);
	ShowWindow(hWnd, SW_SHOW);
}


HWND SimpleWindow::GetHWND() {
	return hWndSW;
}

/*
	コントロール
*/

void SimpleWindow::InitControls() {
	hDefaultCursor = LoadCursorA(NULL, IDC_ARROW);
	hFontSW = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, "MS Gothic");
	functiontable = NULL;
}

void SimpleWindow::FixCursor(HWND hWnd, UINT Msg) {
	/*if (GetFocus() == hWnd) {
		return;
	}
	*/
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
	if (cText) {
		for (int i = 0; cText[i]; i++) {
			if (cText[i] == '\n') {
				count++;
			}
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
	int height = iLine ? iLine * 12 + 4 : GetHeight(cText) + 4;

	if (height > 16) {
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

void SimpleWindow::GetText(int iID, std::string &output) {
	DWORD dwLength = SendDlgItemMessageA(hWndSW, iID, WM_GETTEXTLENGTH, NULL, NULL);
	char *cText = new char[dwLength + 1];

	GetDlgItemTextA(hWndSW, iID, cText, dwLength + 1);

	output = cText;

	delete[] cText;
}

void SimpleWindow::SetFunction(int iID, void(*vFunction)(SimpleWindow *sw)) {
	if (listview) {
		SimpleListView *slv = ListView(iID);
		if (slv) {
			return slv->SetFunction(vFunction);
		}
	}
	if (!functiontable) {
		functiontable = new FunctionTable(iID, vFunction);
	}
	else {
		FunctionTable *ft;
		for (ft = functiontable; ft->next; ft = ft->next);
		ft->next = new FunctionTable(iID, vFunction);
	}
}

bool SimpleWindow::CheckBoxStatus(int iID) {
	return IsDlgButtonChecked(hWndSW, iID);
}

FunctionTable* SimpleWindow::FindFunction(int iID) {
	FunctionTable *ft;

	for (ft = functiontable; ft && ft->id != iID; ft = ft->next);

	return ft;
}

/*
	ListView
*/
SimpleListView* SimpleWindow::FindListView(HWND hWnd) {
	SimpleListView *slv;

	for (slv = listview; slv && slv->hwnd != hWnd; slv = slv->next);

	return slv;
}

SimpleListView* SimpleWindow::ListView(int iID, int X, int Y, int iWidth, int iHeight) {
	if (!listview) {
		InitCommonControls();
		listview = new SimpleListView(hWndSW, iID, X, Y, iWidth, iHeight);
		return listview;
	}
	else {
		SimpleListView *slv;
		for (slv = listview; slv->next; slv = slv->next);
		return slv->next = new SimpleListView(hWndSW, iID, X, Y, iWidth, iHeight);
	}
}

SimpleListView* SimpleWindow::ListView(int iID) {
	SimpleListView *slv;

	for (slv = listview; slv && slv->id != iID; slv = slv->next);

	return slv;
}