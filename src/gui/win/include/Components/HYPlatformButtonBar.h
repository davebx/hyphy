/*	At toolbar menu object for Win 32.	Sergei L. Kosakovsky Pond, May 2000 - December 2002.*/#ifndef _HYPBUTTONBAR_#define _HYPBUTTONBAR_#include "HYPlatformComponent.h"//__________________________________________________________________class _HYPlatformButtonBar{public:	_HYPlatformButtonBar(void);	virtual	~_HYPlatformButtonBar(void);	virtual void		_SetBackColor  	 (_HYColor&);	virtual	void		_SetDimensions   (_HYRect,_HYRect);	virtual	void		_SetVisibleSize  (_HYRect);	_HYRect		_GetButtonRect   (bool conv = false);	virtual	void		_Paint (Ptr p);	virtual	void		_Update(Ptr p);	void		_DisposeButtons (void);	void		_DisposeButton	(long);	void		_MarkButtonForUpdate(int);	void		_UnpushButton	(void);	int			_FindClickedButton (int,int);	HWND		 ttCtl;	HBRUSH		 backFill;	RECT		 buttonRect;	int			 pushed,				 lastMouseDown;};#endif