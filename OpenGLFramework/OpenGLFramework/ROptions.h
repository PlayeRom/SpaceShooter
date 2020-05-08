#pragma once

#include "RMenuBase.h"

class CSDLFont;
class CGameControl;

#define MAX_REGIONS_OPTIONS 5

class CROptions : public CRMenuBase
{
public:
	CROptions();
	~CROptions();

	//ustawienie regionow menu wzgledem rozdzielczosci
	GLvoid SetMouseRegion();
	GLvoid Draw();
	GLvoid DrawButtonts();
	GLvoid RestartObjects();

	GLboolean MouseLButtonDown(GLint iX, GLint iY);
	GLboolean KeyDown(GLuint uiKey);

	inline GLvoid SetOldIndexLanguage(GLint iIndexLanguage){
		m_iOldIndexLanguage = iIndexLanguage;
	}

	inline GLvoid ReturnToGame() { m_bReturnToGame = GL_TRUE; }

private:
	CGameControl *m_pGameCtrl;
	CSDLFont	*m_pSDLFontTitle;
	CSDLFont	*m_pSDLFontMenu;

	RECT m_rRegions[MAX_REGIONS_OPTIONS];
	RECT m_rRegions_Font[MAX_REGIONS_OPTIONS];

	GLint m_iOldIndexLanguage;
	GLboolean m_bReturnToGame;

	GLvoid DoReturnToGame();
};
