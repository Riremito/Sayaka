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
	STATICTEXT_TEST,
	EDITBOX_TEST2
};

void Button(SimpleWindow *sw) {
	if (sw->CheckBoxStatus(CHECKBOX_TEST)) {
		std::string edit1, edit2;
		sw->GetText(EDITBOX_TEST, edit1);
		sw->GetText(EDITBOX_TEST2, edit2);

		if (edit2.length()) {
			edit2 += "\r\n";
		}

		edit2 += edit1;

		sw->SetText(EDITBOX_TEST2, edit2.c_str());
	}
}

void CheckBox(SimpleWindow *sw) {
	if (sw->CheckBoxStatus(CHECKBOX_TEST)) {
		sw->ReadOnly(EDITBOX_TEST, true);
	}
	else {
		sw->ReadOnly(EDITBOX_TEST, false);
	}
}

void CreateControls(SimpleWindow *sw) {
	sw->EditBox(EDITBOX_TEST, 3, 3, NULL, 400 - 6, 10);
	sw->CheckBox(CHECKBOX_TEST, 250, 133, "確定");
	sw->SetFunction(CHECKBOX_TEST, CheckBox);
	sw->Button(BUTTON_TEST, 330, 131, "表示");
	sw->SetFunction(BUTTON_TEST, Button);
	sw->SetText(EDITBOX_TEST, "テキスト");
	sw->EditBox(EDITBOX_TEST2, 3, 160, NULL, 400 - 6, 10);
	sw->ReadOnly(EDITBOX_TEST2, true);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	SimpleWindow sw(hInstance, "TestSimpleWindow", 400, 300, CreateControls);

	return 0;
}