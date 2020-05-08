#pragma once

#include "RMenuBase.h"

class CSDLFont;
class CGameControl;

#define MAX_REGIONS_OPTIONS_GR 10

class CROptionsGraphics : public CRMenuBase
{
public:
	CROptionsGraphics();
	~CROptionsGraphics();

	//ustawienie regionow menu wzgledem rozdzielczosci
	GLvoid SetMouseRegion();
	GLvoid Draw();
	GLvoid DrawButtonts();
	GLvoid RestartObjects();

	GLboolean MouseLButtonDown(GLint iX, GLint iY);
	GLboolean KeyDown(GLuint uiKey);

	GLvoid SetStartGraphiscOptions();

	inline GLint GetSelectedResX() { return m_iSelectedResX; }
	inline GLint GetSelectedResY() { return m_iSelectedResY; }
	inline GLboolean GetSelectedFullScreen() { return m_bSelectedFullScreen; }

private:
	CGameControl *m_pGameCtrl;
	CSDLFont	*m_pSDLFontTitle;
	CSDLFont	*m_pSDLFontMenu;

	RECT m_rRegions[MAX_REGIONS_OPTIONS_GR];
	RECT m_rRegions_Font[MAX_REGIONS_OPTIONS_GR];

	GLboolean m_bNeedRestart;
	GLubyte m_ubOldBitsPerPixel;
	GLint m_iOldWidth;
	GLint m_iOldHeight;
	GLint m_iOldAntialiasing;
	GLboolean m_bOldFullScreen;

	//rozdzielczosci do wyboru
	std::vector<GLint> m_aWidth;
	std::vector<GLint> m_aHeight;
	GLint m_iMaxIndexRes;
	GLint m_iActualIndexRes;
	GLvoid SetScreenResToArray();
	GLint m_iSelectedResX; //to jest wyswietlane
	GLint m_iSelectedResY;
	GLboolean m_bSelectedFullScreen;
	GLint m_iBrightness; //jasnosc

	GLvoid Exit();

	CVector2* m_aIndices;
};
