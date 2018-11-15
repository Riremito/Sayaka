/*
	SimpleWindow Library
	Created by Sayaka
*/
#include"SimpleWindow.h"

/*
	���C���E�B���h�E
*/

SimpleWindow::SimpleWindow(HINSTANCE hInstance, const char *cTitle, int iWidth, int iHeight, void (*vCreateControls)(SimpleWindow *sw)) {
	WNDCLASSEXA wcx;
	std::string str = cTitle;

	// �N���X��
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
		// �E�B���h�E�T�C�Y�ύX
		AdjustWindow(hWnd, iWidth, iHeight);

		MSG lpMsg;
		while (GetMessageA(&lpMsg, NULL, NULL, NULL) > 0) {
			TranslateMessage(&lpMsg);
			DispatchMessageA(&lpMsg);
		}

		// �蓮�ŕ���
		SendMessageA(hWnd, WM_CLOSE, 0, 0);
	}

	UnregisterClassA(str.c_str(), hInstance);
}


SimpleWindow::~SimpleWindow() {
	// �K�v�Ȃ�
}

LRESULT CALLBACK SimpleWindow::SimpleWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	SimpleWindow *sw;

	switch (Msg) {
		// �R���g���[������
	case WM_CREATE:
		sw = (SimpleWindow *)((CREATESTRUCTA *)lParam)->lpCreateParams;
		SetWindowLongA(hWnd, GWL_USERDATA, (LONG)sw);
		sw->hWndSW = hWnd;

		if (sw->CreateControls) {
			sw->InitControls();
			sw->CreateControls(sw);
		}

		break;
		// �R���g���[���Ɋ֘A�t�����֐����Ăяo��
	case WM_COMMAND:
		sw = (SimpleWindow *)GetWindowLongA(hWnd, GWL_USERDATA);
		sw->idtable.Find(sw, LOWORD(wParam));
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		// �J�[�\���o�O�Ή�
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
	�R���g���[��
*/

void SimpleWindow::InitControls() {

	hDefaultCursor = LoadCursorA(NULL, IDC_ARROW);
	hFontSW = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, "MS Gothic");

	InitCommonControls();
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
	int height = iLine ? iLine * 12 + 4 : GetHeight(cText) + 4;

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

void SimpleWindow::SetFunction(int iID, void(*vFunction)(SimpleWindow *sw)) {
	idtable.Add(iID, vFunction);
}

bool SimpleWindow::CheckBoxStatus(int iID) {
	return IsDlgButtonChecked(hWndSW, iID);
}

/*
	�R���g���[���e�[�u��
*/

SimpleWindow::IDTable::IDTable() {
	id = -1;
	ft = NULL;
	next = NULL;
}

SimpleWindow::IDTable::IDTable(int iID, void(*vFunction)(SimpleWindow *sw)) {
	id = iID;
	ft = vFunction;
	next = NULL;
}

SimpleWindow::IDTable::~IDTable() {
	if (next) {
		delete next;
	}
}

void SimpleWindow::IDTable::Add(int iID, void(*vFunction)(SimpleWindow *sw)) {
	if (ft == NULL && id == -1 && next == NULL) {
		id = iID;
		ft = vFunction;
	}
	else {
		if (next == NULL) {
			next = new IDTable(iID, vFunction);
		}
		else {
			return next->Add(iID, vFunction);
		}
	}
}

SimpleWindow::IDTable* SimpleWindow::IDTable::Find(SimpleWindow *sw, int iID) {
	if (id == iID) {
		ft(sw);
		return this;
	}
	if (next) {
		return next->Find(sw, iID);
	}
	return NULL;
}

void SimpleWindow::ListView(int iID, int X, int Y, int iWidth, int iHeight) {
	HWND hWndLV = CreateWindowExA(NULL, "SysListView32", NULL, LVS_REPORT | WS_CHILD /*| LVS_NOSORTHEADER*/ | WS_BORDER | LVS_SINGLESEL | LVS_SHOWSELALWAYS, X, Y, iWidth, iHeight, hWndSW, NULL, NULL, NULL);
	
	hwndtable.Add(iID, hWndLV);

	SendMessageA(hWndLV, WM_SETFONT, (WPARAM)hFontSW, NULL);

	//1�s�I��
	DWORD dwExStyle = ListView_GetExtendedListViewStyle(hWndLV) | LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(hWndLV, dwExStyle);

	//�O���b�h��
	dwExStyle = ListView_GetExtendedListViewStyle(hWndSW) | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(hWndLV, dwExStyle);
	ShowWindow(hWndLV, SW_SHOW);
}

void SimpleWindow::AddHeader(int iID, const char *cText, int iWidth) {
	LVCOLUMNA lvc;

	ZeroMemory(&lvc, sizeof(lvc));

	lvc.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
	lvc.fmt = LVCFMT_LEFT /*| LVCFMT_FIXED_WIDTH*/;
	lvc.pszText = (char *)cText;
	lvc.cx = iWidth;
	lvc.iSubItem = hwndtable.Find(iID)->headercount;

	HWND hWndLV = hwndtable.Find(iID)->hwnd;

	if (hWndLV) {
		ListView_InsertColumn(hWndLV, hwndtable.Find(iID)->headercount, &lvc);
		hwndtable.Header(iID);
	}
}


void SimpleWindow::AddItem(int iID, const char *cText) {
	LVITEMA item;
	if (hwndtable.Find(iID)->item && hwndtable.Find(iID)->item % hwndtable.Find(iID)->headercount == 0) {
		hwndtable.Find(iID)->line++;
	}


	item.mask = LVIF_TEXT;
	item.iItem = hwndtable.Find(iID)->line;
	item.iSubItem = hwndtable.Find(iID)->item % hwndtable.Find(iID)->headercount;
	item.pszText = (char *)cText;

	if (hwndtable.Find(iID)->item % hwndtable.Find(iID)->headercount == 0) {
		ListView_InsertItem(hwndtable.Find(iID)->hwnd, &item);
	}
	else {
		ListView_SetItem(hwndtable.Find(iID)->hwnd, &item);
	}

	ListView_EnsureVisible(hwndtable.Find(iID)->hwnd, hwndtable.Find(iID)->line, true);

	hwndtable.Find(iID)->item++;
}

/*
	���X�g�e�[�u��
*/
SimpleWindow::HWNDTable::HWNDTable() {
	id = -1;
	hwnd = NULL;
	next = NULL;
	headercount = 0;
	line = 0;
	item = 0;
}

SimpleWindow::HWNDTable::HWNDTable(int iID, HWND hWnd) {
	id = iID;
	hwnd = hWnd;
	next = NULL;
	headercount = 0;
}

SimpleWindow::HWNDTable::~HWNDTable() {
	if (next) {
		delete next;
	}
}

void SimpleWindow::HWNDTable::Add(int iID, HWND hWnd) {
	if (hwnd == NULL && id == -1 && next == NULL) {
		id = iID;
		hwnd = hWnd;
	}
	else {
		if (next == NULL) {
			next = new HWNDTable(iID, hWnd);
		}
		else {
			return next->Add(iID, hWnd);
		}
	}
}

SimpleWindow::HWNDTable* SimpleWindow::HWNDTable::Find(int iID) {
	if (id == iID) {
		return this;
	}

	if (next) {
		return next->Find(iID);
	}

	return NULL;
}

void SimpleWindow::HWNDTable::Header(int iID) {
	if (id == iID) {
		headercount++;
		return;
	}

	if (next) {
		return next->Header(iID);
	}
}


int SimpleWindow::HWNDTable::GetHC(int iID) {
	if (id == iID) {
		return headercount;
	}

	if (next) {
		return next->GetHC(iID);
	}

	return 0;
}