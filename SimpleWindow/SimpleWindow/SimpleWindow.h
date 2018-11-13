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

class SimpleWindow {
	/*
		メインウィンドウ
	*/
private:
	HINSTANCE hInstanceSW;
	HWND hWndSW;
	void(*CreateControls)(SimpleWindow *sw);

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

	int iID_test;
	void (*vFunction_test)(SimpleWindow *sw);

	void InitControls();
	void FixCursor(HWND hWnd, UINT Msg);
	void SetFont(int iID);
	int GetWidth(const char *cText);
	int GetHeight(const char *cText);

	class IDTable {
	private:
		int id;
		void (*ft)(SimpleWindow *sw);
		IDTable *next;

	public:
		IDTable();
		IDTable(int iID, void (*vFunction)(SimpleWindow *sw));
		~IDTable();
		void Add(int iID, void (*vFunction)(SimpleWindow *sw));
		SimpleWindow::IDTable* Find(SimpleWindow *sw, int iID);
	} idtable;
	
public:
	void Button(int iID, int X, int Y, const char *cText, int iWidth = 0, int iHeight = 0);
	void CheckBox(int iID, int X, int Y, const char *cText);
	void EditBox(int iID, int X, int Y, const char *cText, int iWidth = 0, int iLine = 0, bool bNumber = false);
	void StaticText(int iID, int X, int Y, const char *cText, int iWidth = 0, int iLine = 0);

	LRESULT ReadOnly(int iID, bool bReadOnly = true);
	BOOL SetText(int iID, const char *cText);
	void GetText(int iID, std::string &out);
	void SetFunction(int iID, void (*vFunction)(SimpleWindow *sw));
	bool CheckBoxStatus(int iID);
};

#endif