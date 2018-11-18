/*
	Created by Sayaka
*/
#ifndef __SIMPLEWINDOW_H__
#define __SIMPLEWINDOW_H__

#include<Windows.h>
#include<Richedit.h>
#include<CommCtrl.h>
#pragma comment(lib, "comctl32.lib")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include<string>

class SimpleWindow;
class SimpleListView;
class FunctionTable;

class SimpleWindow {
	/*
		メインウィンドウ
	*/
private:
	HINSTANCE hInstanceSW;
	HWND hWndSW;
	void (*CreateControls)(SimpleWindow *sw);

	static LRESULT CALLBACK SimpleWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	void AdjustWindow(HWND hWnd, int nWidth, int nHeight);

public:
	SimpleWindow(HINSTANCE hInstance, const char *cTitle = "SimpleWindow", int iWidth = 800, int iHeight = 600, void(*vCreateControls)(SimpleWindow *sw) = NULL);
	~SimpleWindow();
	HWND GetHWND();

	/*
		コントロール
	*/
private:
	HFONT hFontSW;
	HCURSOR hDefaultCursor;
	FunctionTable* functiontable;

	void InitControls();
	void FixCursor(HWND hWnd, UINT Msg);
	void SetFont(int iID);
	int GetWidth(const char *cText);
	int GetHeight(const char *cText);
	FunctionTable* FindFunction(int iID);

public:
	void Button(int iID, int X, int Y, const char *cText, int iWidth = 0, int iHeight = 0);
	void CheckBox(int iID, int X, int Y, const char *cText);
	void EditBox(int iID, int X, int Y, const char *cText, int iWidth = 0, int iLine = 0, bool bNumber = false);
	void StaticText(int iID, int X, int Y, const char *cText, int iWidth = 0, int iLine = 0);
	void ComboBox(int iID, int X, int Y, int iWidth, int iHeight);

	LRESULT ReadOnly(int iID, bool bReadOnly = true);
	BOOL SetText(int iID, const char *cText);
	void GetText(int iID, std::string &output);
	void SetFunction(int iID, void (*vFunction)(SimpleWindow *sw));
	bool CheckBoxStatus(int iID);
	void ComboBoxAddSel(int iID, const char *cText);
	void ComboBoxSetSel(int iID, int index);

	/*
		リストコントロール
	*/
private:
	SimpleListView *listview;
	SimpleListView* FindListView(HWND hWnd);
public:
	SimpleListView* ListView(int iID, int X, int Y, int iWidth = 394, int iHeight = 294);
	SimpleListView* ListView(int iID);
};


/*
	関数テーブル
*/
class FunctionTable {
public:
	int id;
	void (*function)(SimpleWindow *sw);
	FunctionTable* next;

	FunctionTable(int iID, void(*vFunction)(SimpleWindow *sw));
	~FunctionTable();
};


/*
	リストコントロール
*/
class SimpleListView {
private:
	int itemcount;
	/*
	int GetItemCount();
	*/

public:
	int id;
	HWND hwnd;
	int headercount;
	SimpleListView *next;
	void (*notify)(SimpleWindow *sw);

	SimpleListView(HWND hWndParent, int iID, int X, int Y, int iWidth, int iHeight);
	~SimpleListView();

	void AddHeader(const char *cText, int iWidth);
	void AddItem(const char *cText);
	bool GetItem(int index, std::string &output);
	void SetFunction(void (*vFunction)(SimpleWindow *sw));
	void Clear();
};


#endif