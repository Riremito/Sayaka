/*
	Created by Sayaka
*/
#include<Windows.h>
#include "../Release/SimpleWindow.h"
#pragma comment(lib, "../Release/SimpleWindow.lib")

enum ID {
	BUTTON_TEST,
	CHECKBOX_TEST,
	EDITBOX_TEST,
	STATICTEXT_TEST
};

void ButtonTest(SimpleWindow *sw) {
	MessageBoxA(sw->GetHWND(), "ButtonTest", "ButtonControl", MB_OK);
}

void CheckBoxTest(SimpleWindow *sw) {
	MessageBoxA(sw->GetHWND(), "CheckBoxTest", "CheckBoxControl", MB_OK);
}

void CreateControls(SimpleWindow *sw) {
	sw->Button(BUTTON_TEST, 3, 3, "Button Multi Line\r\nSize Check Test");
	sw->SetFunction(BUTTON_TEST, ButtonTest);
	sw->CheckBox(CHECKBOX_TEST, 3, 100, "CheckBox Test\r\nMulti Line");
	sw->SetFunction(CHECKBOX_TEST, CheckBoxTest);
	sw->EditBox(EDITBOX_TEST, 3, 200, "Edit Box Test\r\nMulti Line");
	sw->StaticText(STATICTEXT_TEST, 200, 3, "Static Text\r\nMulti Line");

	sw->ReadOnly(EDITBOX_TEST);
	sw->SetText(EDITBOX_TEST, "Set!");
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	SimpleWindow sw(hInstance, "TestSimpleWindow", 400, 300, CreateControls);

	return 0;
}