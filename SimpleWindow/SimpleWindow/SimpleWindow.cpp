/*
	Created by Sayaka
*/
/*
#include<Richedit.h>
#include<CommCtrl.h>
#pragma comment(lib, "comctl32.lib")
*/
#include"SimpleWindow.h"

SimpleWindow::SimpleWindow(HINSTANCE hInstance, const char *cTitle, int iWidth, int iHeight, void (*vCreateControls)(SimpleWindow *sw)) {
	WNDCLASSEXA wcx;
	std::string str;

	InitCommonControls();

	hInstanceSW = hInstance;
	CreateControls = vCreateControls;
	CommandControls = NULL;
	NotifyControls = NULL;

	ZeroMemory(&wcx, sizeof(wcx));

	wcx.cbSize = sizeof(wcx);
	wcx.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcx.hInstance = hInstance;
	wcx.lpfnWndProc = SimpleWindowProc;

	str = cTitle;
	str += "Class";

	wcx.lpszClassName = str.c_str();
	wcx.style = CS_HREDRAW | CS_VREDRAW;

	// Register Class
	if (!RegisterClassExA(&wcx)) {
		return;
	}

	HWND hWnd = CreateWindowExA(NULL, str.c_str(), cTitle, (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, this);

	if (hWnd) {
		// Change Window Size
		AdjustWindow(hWnd, iWidth, iHeight);

		MSG lpMsg;
		while (GetMessageA(&lpMsg, NULL, NULL, NULL) > 0) {
			TranslateMessage(&lpMsg);
			DispatchMessageA(&lpMsg);
		}

		// Close Window Manually
		SendMessageA(hWnd, WM_CLOSE, 0, 0);
	}

	// Unregister Class
	UnregisterClassA(str.c_str(), hInstance);
}

SimpleWindow::~SimpleWindow() {

}

LRESULT CALLBACK SimpleWindow::SimpleWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg) {
	case WM_CREATE:
	{
		// Set this Pointer
		SetWindowLongA(hWnd, GWL_USERDATA, (LONG)((CREATESTRUCTA *)lParam)->lpCreateParams);
		// Get this Pointer
		SimpleWindow *sw = (SimpleWindow *)GetWindowLongA(hWnd, GWL_USERDATA);
		// Use it for Creating Controls
		sw->hWndSW = hWnd;

		if (sw->CreateControls) {
			// Create Controls
			sw->InitControls();
			sw->CreateControls(sw);
		}
		break;
	}

	case WM_COMMAND:
	{
		SimpleWindow *sw = (SimpleWindow *)GetWindowLongA(hWnd, GWL_USERDATA);
		sw->idtable.Find(sw, LOWORD(wParam));
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
	{
		SimpleWindow *sw = (SimpleWindow *)GetWindowLongA(hWnd, GWL_USERDATA);
		sw->FixCursor(hWnd, Msg);
		break;
	}
	}
	return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

void SimpleWindow::AdjustWindow(HWND hWnd, int nWidth, int nHeight) {
	RECT W_RECT, C_RECT;
	GetWindowRect(hWnd, &W_RECT);
	GetClientRect(hWnd, &C_RECT);
	SetWindowPos(hWnd, HWND_TOP, NULL, NULL, (nWidth + (W_RECT.right - W_RECT.left) - (C_RECT.right - C_RECT.left)), (nHeight + (W_RECT.bottom - W_RECT.top) - (C_RECT.bottom - C_RECT.top)), SWP_NOMOVE);
	ShowWindow(hWnd, SW_SHOW);
}


HWND SimpleWindow::GetHWND() {
	return hWndSW;
}