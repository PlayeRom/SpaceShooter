#pragma once

#include "RMenuBase.h"

class CGameControl;
class CSDLFont;

#define MAX_REGIONS_CREDITS 2

class CRCredits : public CRMenuBase
{
public:
	CRCredits();
	~CRCredits();

	GLvoid SetMouseRegion();
	GLvoid Draw();
	GLvoid RestartObjects();

	GLboolean MouseLButtonDown(GLint iX, GLint iY);
	GLboolean KeyDown(GLuint uiKey);
	GLvoid SetStartVariables();

private:
	CGameControl *m_pGameCtrl;
	CSDLFont *m_pSDLFontTitle;
	CSDLFont *m_pSDLFontMenu;

	RECT m_rRegions[MAX_REGIONS_CREDITS];	//0 - plansza, 1 - przycisk
	RECT m_rRegions_Font[MAX_REGIONS_CREDITS];

	GLint m_iTextPosY_27;
	GLint m_iTextPosY_13;
	GLint m_iPosYAnim;
};
