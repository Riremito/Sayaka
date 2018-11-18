/*
	Created by Sayaka
*/
#include"SimpleWindow.h"

SimpleListView::SimpleListView(HWND hWndParent, int iID, int X, int Y, int iWidth, int iHeight) {
	id = iID;
	headercount = 0;
	itemcount = 0;
	next = NULL;
	notify = NULL;

	hwnd = CreateWindowExA(NULL, "SysListView32", NULL, LVS_REPORT | WS_CHILD | LVS_NOSORTHEADER | WS_BORDER | LVS_SINGLESEL | LVS_SHOWSELALWAYS, X, Y, iWidth, iHeight, hWndParent, NULL, NULL, NULL);

	//1行選択
	DWORD dwExStyle = SendMessageA(hwnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0) | LVS_EX_FULLROWSELECT;
	SendMessageA(hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwExStyle);
	//グリッド線
	dwExStyle = SendMessageA(hwnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0) | LVS_EX_GRIDLINES;
	SendMessageA(hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwExStyle);

	HFONT hFont = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, "MS Gothic");
	SendMessageA(hwnd, WM_SETFONT, (WPARAM)hFont, NULL);

	ShowWindow(hwnd, SW_SHOW);
}

SimpleListView::~SimpleListView() {
	if (next) {
		delete next;
	}
}

void SimpleListView::AddHeader(const char *cText, int iWidth) {
	LVCOLUMNA lvc;

	ZeroMemory(&lvc, sizeof(lvc));

	lvc.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
	lvc.fmt = LVCFMT_LEFT /*| LVCFMT_FIXED_WIDTH*/;
	lvc.pszText = (char *)cText;
	lvc.cx = iWidth;
	lvc.iSubItem = headercount;

	SendMessageA(hwnd, LVM_INSERTCOLUMN, (WPARAM)headercount, (LPARAM)&lvc);
	headercount++;
}

void SimpleListView::AddItem(const char *cText) {
	LVITEMA item;
	item.mask = LVIF_TEXT;
	item.iItem = itemcount / headercount;
	item.iSubItem = itemcount % headercount;
	item.pszText = (char *)cText;

	if (itemcount % headercount == 0) {
		SendMessageA(hwnd, LVM_INSERTITEM, 0, (LPARAM)&item);
	}
	else {
		SendMessageA(hwnd, LVM_SETITEM, 0, (LPARAM)&item);
	}

	SendMessageA(hwnd, LVM_ENSUREVISIBLE, (WPARAM)(itemcount / headercount), true);
	itemcount++;
}

bool SimpleListView::GetItem(int index, std::string &output) {
	int line = SendMessageA(hwnd, LVM_GETNEXTITEM, (WPARAM)-1, LVNI_SELECTED);

	if (line == -1) {
		output.clear();
		return false;
	}

	char buf[0x1000];// 後で直す

	LVITEMA item;

	item.iSubItem = index;
	item.cchTextMax = sizeof(buf);
	item.pszText = buf;

	SendMessageA(hwnd, LVM_GETITEMTEXT, line, (LPARAM)&item);

	output = buf;
	return true;
}

void SimpleListView::SetFunction(void (*vFunction)(SimpleWindow *sw)) {
	notify = vFunction;
}

void SimpleListView::Clear() {
	SendMessageA(hwnd, LVM_DELETEALLITEMS, 0, 0);
	itemcount = 0;
}

/*
int SimpleListView::GetItemCount() {
	return SendMessageA(hwnd, LVM_GETITEMCOUNT, 0, 0);
}
*/