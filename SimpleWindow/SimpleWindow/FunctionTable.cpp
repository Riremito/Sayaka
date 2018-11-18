#include"SimpleWindow.h"

FunctionTable::FunctionTable(int iID, void (*vFunction)(SimpleWindow *sw)) {
	id = iID;
	function = vFunction;
	next = NULL;
}

FunctionTable::~FunctionTable() {
	if (next) {
		delete next;
	}
}