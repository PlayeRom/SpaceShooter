#pragma once

#include "RMenuBase.h"

class CSDLFont;
class CGameControl;

#define MAX_REGIONS_OPTIONS_CTRL 12

class CROptionsControl : public CRMenuBase
{
public:
	CROptionsControl();
	~CROptionsControl();

	//ustawienie regionow menu wzgledem rozdzielczosci
	GLvoid SetMouseRegion();
	GLvoid Draw();
	GLvoid DrawButtonts();
	GLvoid RestartObjects();

	GLboolean MouseLButtonDown(GLint iX, GLint iY);
	GLboolean KeyDown(GLuint uiKey);

private:
	CGameControl *m_pGameCtrl;
	CSDLFont	*m_pSDLFontTitle;
	CSDLFont	*m_pSDLFontMenu;
	CSDLFont	*m_pSDLFontMenuSmall;

	TCHAR m_cButtonName[ 256 ];

	RECT m_rRegions[MAX_REGIONS_OPTIONS_CTRL];
	RECT m_rRegions_Font[MAX_REGIONS_OPTIONS_CTRL];

	GLvoid Exit();
	LPCTSTR	GetButtonName( COLORREF &crColor, GLint i );
};
