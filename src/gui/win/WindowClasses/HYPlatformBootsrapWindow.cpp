/*	Win 32 rtions of the bootstrap window class	Sergei L. Kosakovsky Pond, Spring 2000 - January 2003.*/#include "HYParameterTable.h"//__________________________________________________________________bool 		_HYBootstrapWindow::_ProcessMenuSelection (long msel){	switch (msel) {	case HY_WINDOW_MENU_ID_FILE+1 : { // file menu		DoSave ();		return true;	}	case HY_WINDOW_MENU_ID_FILE+2 : { // print menu		_HYTable* t =  (_HYTable*)GetCellObject (2,0);		t->_PrintTable((_HYTable*)GetCellObject (1,0));		return true;	}	}	if (_HYTWindow::_ProcessMenuSelection(msel)) {		return true;	}	return false;}//EOF