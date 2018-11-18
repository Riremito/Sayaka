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
	EDITBOX_TEST2,
	LISTVIEW_TEST,
	EDITBOX_VIEWTEST,
	COMBOBOX_TEST
};

void ListView(SimpleWindow *sw) {
	std::string output;
	std::string item;
	SimpleListView *slv = sw->ListView(LISTVIEW_TEST);

	for (int i = 0; i < slv->headercount; i++) {
		if (!slv->GetItem(i, item)) {
			return;
		}
		if (output.length()) {
			output += ", ";
		}
		output += item;
	}

	sw->SetText(EDITBOX_VIEWTEST, output.c_str());
}

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
		SimpleListView *slv = sw->ListView(LISTVIEW_TEST);
		slv->Clear();
	}
	else {
		std::string output;
		SimpleListView *slv = sw->ListView(LISTVIEW_TEST);
		slv->AddItem("test");
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

void ComboBox(SimpleWindow *sw) {
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

	SimpleListView *slv = sw->ListView(LISTVIEW_TEST, 400, 3);
	slv->AddHeader("項目1", 100);
	slv->AddHeader("項目2", 100);
	slv->AddHeader("項目3", 100);
	slv->AddItem("test1");
	slv->AddItem("test2");
	slv->AddItem("test3");
	slv->AddItem("test4");
	slv->AddItem("test5");
	slv->AddItem("test6");
	slv->AddItem("test7");
	slv->AddItem("test8");
	slv->AddItem("test9");
	sw->SetFunction(LISTVIEW_TEST, ListView);
	sw->EditBox(EDITBOX_VIEWTEST, 400, 300, NULL, 400 - 6);

	sw->ComboBox(COMBOBOX_TEST, 3, 300, 100, 20);
	sw->SetFunction(COMBOBOX_TEST, ComboBox);
	sw->ComboBoxAddSel(COMBOBOX_TEST, "A");
	sw->ComboBoxAddSel(COMBOBOX_TEST, "B");
	sw->ComboBoxAddSel(COMBOBOX_TEST, "C");
	sw->ComboBoxSetSel(COMBOBOX_TEST, 1);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE/* hPrevInstance*/, LPSTR/* lpCmdLine*/, int/* nCmdShow*/) {

	SimpleWindow sw(hInstance, "TestSimpleWindow", 800, 600, CreateControls);

	return 0;
}