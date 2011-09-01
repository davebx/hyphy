/*	Button component for Win32.	Sergei L. Kosakovsky Pond, May 2000 - December 2002*/#include "errorfns.h"#include "HYButton.h"#include "HYUtils.h"#include "HYEventTypes.h"#include "HYGraphicPane.h"#include "HYPlatformWindow.h"//___________________________________________________________________HYPlatformButton::_HYPlatformButton 	(void){	backFill = CreateSolidBrush (RGB (255,255,255));	checkPointer (backFill);	buttonWindow 	= nil;	buttonRect		= (RECT) {		0,0,100,100	};	pLabelFont 		= nil;	fc				= 0;}//___________________________________________________________________HYPlatformButton::~_HYPlatformButton	(void){	DeleteObject (backFill);	if (pLabelFont) {		DeleteObject (pLabelFont);	}}//__________________________________________________________________void	_HYPlatformButton::_SetBackColor (_HYColor& c){	DeleteObject (backFill);	backFill = CreateSolidBrush (RGB(c.R,c.G,c.B));	checkPointer (backFill);}//__________________________________________________________________void		_HYPlatformButton::_SetFont (_HYFont& f){	if (pLabelFont) {		DeleteObject(pLabelFont);	}	pLabelFont = CreateFont (f.size,0,0,0,(f.style&HY_FONT_BOLD)?FW_BOLD:FW_NORMAL,f.style&HY_FONT_ITALIC,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,							 CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,f.face.sData);	if (buttonWindow) {		SendMessage (buttonWindow, WM_SETFONT, (WPARAM)pLabelFont, 1);	}}//__________________________________________________________________void		_HYPlatformButton::_Update (Ptr p){	_Paint (p);}//__________________________________________________________________void		_HYPlatformButton::_SetDimensions (_HYRect r, _HYRect rel){	_HYButton* theParent = (_HYButton *) this;	theParent->_HYPlatformComponent::_SetDimensions (r,rel);	_SetVisibleSize (rel);}//__________________________________________________________________void		_HYPlatformButton::_EnableButton (bool e){	if (buttonWindow) {		EnableWindow (buttonWindow, e);	}}//__________________________________________________________________void		_HYPlatformButton::_SetButtonKind (unsigned char k){	if (buttonWindow) {		bool	     onOff = (k==HY_BUTTON_OK);		SendMessage (buttonWindow,BM_SETSTYLE, onOff?BS_DEFPUSHBUTTON:BS_PUSHBUTTON, 1);	}}//__________________________________________________________________void		_HYPlatformButton::_SetVisibleSize (_HYRect rel){	_HYButton * theParent = (_HYButton*) this;	buttonRect.left=rel.left;	buttonRect.top = rel.top;	_HYRect s = theParent->_SuggestDimensions();	buttonRect.right =  buttonRect.left+s.right;	buttonRect.bottom = buttonRect.top+s.bottom;	AlignRectangle (rel, buttonRect, theParent->GetAlignFlags());	if (buttonWindow) {		SetWindowPos(buttonWindow,nil, buttonRect.left,buttonRect.top,buttonRect.right-buttonRect.left+1,buttonRect.bottom-buttonRect.top+1,SWP_NOZORDER);	}}//__________________________________________________________________void		_HYPlatformButton::_Paint (Ptr p){	_HYButton * theParent = (_HYButton*) this;	RECT		notButton,				clientArea = HYRect2Rect (theParent->rel);	SubtractRect (&notButton, &clientArea, & buttonRect);	if (!(theParent->settings.width&HY_COMPONENT_TRANSP_BG)) {		FillRect     ((HDC)((_HYRect*)p)->width, &notButton, backFill);	}	if (buttonWindow) {		UpdateWindow (buttonWindow);	}	(*theParent)._HYPlatformComponent::_Paint(p);}//___________________________________________________________________HYRect	_HYButton::_SuggestDimensions (void){	_HYRect res = {10,100,10,100,HY_COMPONENT_NO_SCROLL};	res.top = res.bottom = buttonFont.size+8;	res.left= res.right  = GetVisibleStringWidth (buttonText,buttonFont) + 15;	return res;}//__________________________________________________________________void		_HYButton::_Activate (void){	if (!activationFlag)		if (buttonWindow&&isEnabled) {			EnableWindow (buttonWindow,true);		}	_HYPlatformComponent::_Activate();}//__________________________________________________________________void		_HYButton::_Deactivate (void){	if (activationFlag)		if (buttonWindow&&isEnabled) {			EnableWindow (buttonWindow,false);		}	_HYPlatformComponent::_Deactivate();}//__________________________________________________________________void		_HYPlatformButton::_SetText (void){	_HYButton *parent = (_HYButton*)this;	if (buttonWindow) {		SetWindowText (buttonWindow,parent->buttonText.sData);	} else {		buttonWindow = CreateWindow ("BUTTON",parent->buttonText.sData,WS_VISIBLE|WS_CHILD|((parent->buttonKind==HY_BUTTON_OK)?BS_DEFPUSHBUTTON:BS_PUSHBUTTON),									 buttonRect.left,buttonRect.right,buttonRect.right-buttonRect.left+1,buttonRect.bottom-buttonRect.top+1									 ,((_HYButton*)this)->parentWindow,NULL,GetModuleHandle (NULL), NULL);		checkPointer   (buttonWindow);		if (pLabelFont) {			SendMessage (buttonWindow, WM_SETFONT, (WPARAM)pLabelFont, 1);		}	}}//__________________________________________________________________bool _HYButton::_ProcessOSEvent (Ptr vEvent){	_HYWindowsUIMessage * theEvent = (_HYWindowsUIMessage *)vEvent;	switch (theEvent->iMsg) {	case WM_COMMAND: {		WORD	   wP = HIWORD(theEvent->wParam);		if ((wP==BN_CLICKED)||(wP==BN_DOUBLECLICKED)) {			HWND	cbh = (HWND)theEvent->lParam;			if ((cbh == buttonWindow)&&isEnabled) {				if (messageRecipient) {					messageRecipient->ProcessEvent (generateButtonPushEvent (GetID(),0));				}				return true;			}		}		break;	}	case WM_CTLCOLORBTN: {		if ((HWND)theEvent->lParam == buttonWindow) {			theEvent->res = (LRESULT)backFill;			return true;		}		break;	}	case WM_CHAR: {		bool	good	= false;		int 	keyCode = theEvent->wParam;		if (buttonKind == HY_BUTTON_OK) {			good = (keyCode==VK_RETURN);		} else if (buttonKind == HY_BUTTON_CANCEL) {			good = (keyCode== VK_ESCAPE);		}		if (good) {			SendMessage (buttonWindow, BM_SETSTATE, true, 0);			if (messageRecipient) {				messageRecipient->ProcessEvent (generateButtonPushEvent (GetID(),0));			}		}		if (good) {			return true;		}		break;	}	}	return _HYPlatformComponent::_ProcessOSEvent (vEvent);}// EOF