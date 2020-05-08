#pragma once

#include "RMenuBase.h"

class CSDLFont;
class CGameControl;

#define MAX_REGIONS_OPTIONS_SD 9

class CROptionsSound : public CRMenuBase
{
public:
	CROptionsSound();
	~CROptionsSound();

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

	RECT m_rRegions[MAX_REGIONS_OPTIONS_SD];
	RECT m_rRegions_Font[MAX_REGIONS_OPTIONS_SD];

	GLvoid Exit();
	GLvoid DrawSlider(GLint iIndexOfSlider);

	CVector2* m_aIndices;
};
