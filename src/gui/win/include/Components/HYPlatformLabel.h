/*	A static label object for Win32.	Sergei L. Kosakovsky Pond, May 2000-December 2002.*/#ifndef _HYPLABEL_#define _HYPLABEL_#include "HYBaseGUI.h"#include "Windows.h"#include "hy_strings.h"//__________________________________________________________________class _HYPlatformLabel{public:	_HYPlatformLabel(void);	// flags, title, visibility	virtual	~_HYPlatformLabel(void);	virtual void		_SetBackColor  	 (_HYColor&);	virtual void		_SetForeColor  	 (_HYColor&);	virtual	void		_SetDimensions   (_HYRect,_HYRect);	virtual	void		_SetVisibleSize  (_HYRect);	virtual	void		_SetFont		 (_HYFont&);	virtual	void		_SetText		 (void);	virtual	void		_Paint (Ptr p);	virtual	void		_Update(Ptr p);	HBRUSH		 backFill;	COLORREF	 fc;	RECT		 labelRect;	HFONT		 pLabelFont;	HPEN		 labelPen;};#endif