/*	Check box for Win32.	Sergei L. Kosakovsky Pond, May 2000-December 2002*/#include "HYLabel.h"#include "errorfns.h"#include "HYPlatformWindow.h"#include "commctrl.h"//__________________________________________________________________static LRESULT	CALLBACK checkBoxSubclassHandler (HWND WindowHand, UINT iMsg, WPARAM wParam, LPARAM lParam){	_HYCheckbox * theParent = (_HYCheckbox*)GetWindowLongPtr (WindowHand, GWLP_USERDATA);	/*switch (iMsg)	{		case WM_ERASEBKGND:		{			return 1;		}	}*/	return CallWindowProc ((WNDPROC)theParent->mainHandler, WindowHand, iMsg, wParam, lParam);}//___________________________________________________________________HYPlatformCheckbox::_HYPlatformCheckbox (bool isR){	buttonWindow = nil;	isRadio 	 = isR;	checkboxRect = (RECT) {		0,0,100,100	};}//___________________________________________________________________HYPlatformCheckbox::~_HYPlatformCheckbox (void){}//__________________________________________________________________void		_HYCheckbox::_Activate (void){	_HYPlatformComponent::_Activate();}//__________________________________________________________________void		_HYCheckbox::_Deactivate (void){	_HYPlatformComponent::_Deactivate();}//__________________________________________________________________bool _HYCheckbox::_ProcessOSEvent (Ptr vEvent){	_HYWindowsUIMessage * theEvent = (_HYWindowsUIMessage *)vEvent;	switch (theEvent->iMsg) {	case WM_COMMAND: {		WORD	   wP = HIWORD(theEvent->wParam);		if ((wP==BN_CLICKED)||(wP==BN_DOUBLECLICKED)) {			HWND	cbh = (HWND)theEvent->lParam;			if ((cbh == buttonWindow)&&isEnabled) {				//printf   ("Button Push\n");				SetState (!checkState, true);				return true;			}		}		break;	}	case WM_CTLCOLORBTN: {		if ((HWND)theEvent->lParam == buttonWindow) {			if (settings.width&HY_COMPONENT_TRANSP_BG) {				theEvent->res = (LRESULT)defBrush;			} else {				theEvent->res = (LRESULT)backFill;			}			return true;		}		break;	}	}	return _HYPlatformComponent::_ProcessOSEvent (vEvent);}//___________________________________________________________________HYRect		_HYCheckbox::_SuggestDimensions (void){	_HYRect res = _HYLabel::_SuggestDimensions();	res.left += checkSpacing*2 + 13;	res.right = res.left;	return res;}//__________________________________________________________________void		_HYPlatformCheckbox::_SetVisibleSize (_HYRect rel){	_HYCheckbox * theParent = (_HYCheckbox*) this;	theParent->labelRect.left= rel.left;	theParent->labelRect.top = rel.top;	_HYRect s = theParent->_SuggestDimensions();	theParent->labelRect.right =  theParent->labelRect.left+s.right;	theParent->labelRect.bottom = theParent->labelRect.top+s.bottom;	AlignRectangle (rel, theParent->labelRect, theParent->GetAlignFlags());	checkboxRect.left = theParent->labelRect.left + theParent->checkSpacing;	checkboxRect.bottom = theParent->labelRect.bottom;	checkboxRect.right = checkboxRect.left+13;	checkboxRect.top = checkboxRect.bottom-13;	theParent->labelRect.left += 13+2*theParent->checkSpacing;	if (buttonWindow) {		SetWindowPos (buttonWindow,nil,checkboxRect.left,checkboxRect.top,0,0,SWP_NOSIZE|SWP_NOZORDER);	} else {		buttonWindow = CreateWindow ("BUTTON","",WS_VISIBLE|WS_CHILD|(isRadio?BS_RADIOBUTTON:BS_CHECKBOX),checkboxRect.left,checkboxRect.right,checkboxRect.right-checkboxRect.left+1,checkboxRect.bottom-checkboxRect.top+1									 ,((_HYCheckbox*)this)->parentWindow,NULL,GetModuleHandle (NULL), NULL);		checkPointer   (buttonWindow);		_SetState (theParent->checkState);		SetWindowLongPtr (buttonWindow,GWLP_USERDATA, (LONG_PTR)theParent);		mainHandler	= SetWindowLongPtr (buttonWindow,GWLP_WNDPROC,(LONG_PTR)checkBoxSubclassHandler);		char	      buffer[256];		GetClassName (theParent->parentWindow, buffer, 255);		WNDCLASS	  classInfo;		GetClassInfo (ProgramInstance, buffer, &classInfo);		defBrush = classInfo.hbrBackground;		/*		SIZE		   idealSize;		Button_GetIdealSize			(buttonWindow, idealSize);		bool		   resize = false;		long		   diff = checkBox.Rect.right-checkBox.left+1-idealSize.x;		if (diff)		{			resize = true;			checkBoxRect.left  -=diff/2;			checkBoxRect.right -= (diff-diff/2);		}		diff = checkBox.Rect.bottom-checkBox.top+1-idealSize.y;		if (diff)		{			resize = true;			checkBoxRect.top    -= diff/2;			checkBoxRect.bottom -= (diff-diff/2);		}		if (resize)			SetWindowPos (buttonWindow,NULL,checkboxRect.left,checkboxRect.right,checkboxRect.right-checkboxRect.left+1,checkboxRect.bottom-checkboxRect.top+1,SWP_NOZORDER|SWP_NOMOVE);		*/	}}//__________________________________________________________________void		_HYPlatformCheckbox::_Enable (bool e){	if (buttonWindow) {		EnableWindow (buttonWindow, e);	}}//__________________________________________________________________void		_HYPlatformCheckbox::_Update (Ptr p){	_Paint (p);}//__________________________________________________________________void		_HYPlatformCheckbox::_Paint (Ptr p){	_HYCheckbox *theParent = (_HYCheckbox*) this;	if (buttonWindow) {		UpdateWindow (buttonWindow);	}	theParent->_HYPlatformLabel::_Paint(p);}//__________________________________________________________________void	_HYPlatformCheckbox::_SetState (bool v){	if (buttonWindow) {		SendMessage (buttonWindow, BM_SETCHECK, v?BST_CHECKED:BST_UNCHECKED,0);	}}//EOF