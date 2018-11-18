/*
	Created by Sayaka
*/
#include<Windows.h>
#include<tlhelp32.h>
#include"SimpleWindow.h"
#pragma comment(lib, "SimpleWindow.lib")
#include<stdio.h>

enum ID {
	LISTVIEW,
	EDITBOX,
	BUTTON
};

void Process(SimpleWindow *sw) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	char buf[0x100];
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	sw->ListView(LISTVIEW)->Clear();

	if (hSnapshot != INVALID_HANDLE_VALUE) {
		if (Process32First(hSnapshot, &pe)) {
			sprintf_s(buf, "%04X", pe.th32ProcessID);
			sw->ListView(LISTVIEW)->AddItem(buf);
			sw->ListView(LISTVIEW)->AddItem(pe.szExeFile);

			while (Process32Next(hSnapshot, &pe)) {
				sprintf_s(buf, "%04X", pe.th32ProcessID);
				sw->ListView(LISTVIEW)->AddItem(buf);
				sw->ListView(LISTVIEW)->AddItem(pe.szExeFile);
			}
		}

		CloseHandle(hSnapshot);
	}
}

void ListView(SimpleWindow *sw) {
	std::string output;
	std::string item1, item2;
	SimpleListView *slv = sw->ListView(LISTVIEW);

	slv->GetItem(0, item1);
	slv->GetItem(1, item2);

	output = item1 + ", " + item2;

	sw->SetText(EDITBOX, output.c_str());
}

void Button(SimpleWindow *sw) {
	Process(sw);
}

void CreateControls(SimpleWindow *sw) {
	SimpleListView *slv = sw->ListView(LISTVIEW, 3, 3, 394, 264);

	slv->AddHeader("PID", 50);
	slv->AddHeader("Process", 310);
	slv->SetFunction(ListView);
	Process(sw);

	sw->EditBox(EDITBOX, 3, 275, NULL, 330);
	sw->Button(BUTTON, 350, 275, "XV");
	sw->SetFunction(BUTTON, Button);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {

	SimpleWindow sw(hInstance, "Process", 400, 300, CreateControls);

	return 0;
}