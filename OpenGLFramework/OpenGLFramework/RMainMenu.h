// RMainMenu.h: interface for the CRMainMenu class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "RMenuBase.h"

class CSDLFont;
class CGameControl;

#if _USE_MULTIPLYER_
	#define MAX_REGIONS 7
#else
	#define MAX_REGIONS 6
#endif

class CRMainMenu : public CRMenuBase
{
public:
	CRMainMenu();
	virtual ~CRMainMenu();

	//ustawienie regionow menu wzgledem rozdzielczosci
	GLvoid SetMouseRegion();
	GLvoid Draw();
	GLvoid DrawButtonts();
	GLvoid RestartObjects();

	GLvoid SetStartMenuAnimY(GLboolean bSet = GL_TRUE);

	GLboolean MouseLButtonDown(GLint iX, GLint iY);
	GLboolean KeyDown(GLuint uiKey);

private:
	CGameControl	*m_pGameCtrl;
	CSDLFont		*m_pSDLFontTitle;
	CSDLFont		*m_pSDLFontMenu;
	CSDLFont		*m_pSDLFontVersion;

	RECT m_rRegions[ MAX_REGIONS ];
	RECT m_rRegions_Font[ MAX_REGIONS ];

	GLfloat m_fMenuAnimY;
	GLfloat m_fMenuAnimYFont;
	GLfloat m_fMuliplierMenuAnim;
};