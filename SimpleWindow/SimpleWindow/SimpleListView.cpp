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


	hwnd = CreateWindowExA(NULL, "SysListView32", NULL, LVS_REPORT | WS_CHILD /*| LVS_NOSORTHEADER*/ | WS_BORDER | LVS_SINGLESEL | LVS_SHOWSELALWAYS, X, Y, iWidth, iHeight, hWndParent, NULL, NULL, NULL);

	//1行選択
	DWORD dwExStyle = ListView_GetExtendedListViewStyle(hwnd) | LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(hwnd, dwExStyle);
	//グリッド線
	dwExStyle = ListView_GetExtendedListViewStyle(hwnd) | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(hwnd, dwExStyle);

	HFONT hFont = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, "MS Gothic");
	SendMessageA(hwnd, WM_SETFONT, (WPARAM)hFont, NULL);

	ShowWindow(hwnd, SW_SHOW);
}

SimpleListView::~SimpleListView() {
	if (next) {
		delete next;
	}
}

SimpleListView* SimpleListView::Find(int iID) {
	if (id == iID) {
		return this;
	}

	if (next) {
		return next->Find(iID);
	}

	return NULL;
}
/*
void SimpleListView::Notify(HWND hWnd, SimpleWindow *sw) {
	if (hwnd && hwnd == hWnd) {
		if (notify) {
			notify(sw);
		}
		return;
	}

	if (next) {
		return next->Notify(hWnd, sw);
	}
}
*/
void SimpleListView::AddHeader(const char *cText, int iWidth) {
	LVCOLUMNA lvc;

	ZeroMemory(&lvc, sizeof(lvc));

	lvc.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
	lvc.fmt = LVCFMT_LEFT /*| LVCFMT_FIXED_WIDTH*/;
	lvc.pszText = (char *)cText;
	lvc.cx = iWidth;
	lvc.iSubItem = headercount;

	ListView_InsertColumn(hwnd, headercount, &lvc);
	headercount++;
}


void SimpleListView::AddItem(const char *cText) {
	LVITEMA item;

	item.mask = LVIF_TEXT;
	item.iItem = itemcount / headercount;
	item.iSubItem = itemcount % headercount;
	item.pszText = (char *)cText;

	if (itemcount % headercount == 0) {
		ListView_InsertItem(hwnd, &item);
	}
	else {
		ListView_SetItem(hwnd, &item);
	}

	ListView_EnsureVisible(hwnd, itemcount / headercount, true);

	itemcount++;
}

bool SimpleListView::GetItem(int index, std::string &output) {
	int line = SendMessageA(hwnd, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	if (line == -1) {
		output.clear();
		return false;
	}

	char buf[0x1000];

	LVITEMA item;

	item.iSubItem = index;
	item.cchTextMax = sizeof(buf);
	item.pszText = buf;

	SendMessageA(hwnd, LVM_GETITEMTEXT, line, (LPARAM)&item);

	output = buf;

	return true;
}