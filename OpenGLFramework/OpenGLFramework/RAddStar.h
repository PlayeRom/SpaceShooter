#pragma once

#include "RMenuBase.h"

class CGameControl;
class CSDLFont;

#define MAX_REGIONS_ADD_STAR 5

//ile razy mozna dodac gwiazdke
#define MAX_ADD_STAR 12

class CRAddStar : public CRMenuBase
{
public:
	CRAddStar();
	~CRAddStar();

	GLvoid DoDrawAddStar();
	GLvoid NoDrawAddStar();

	GLvoid SetScoreThresholds();
	GLvoid SetStarsNumberAndDoDraw(GLint iStarsShield, GLint iStarsArmour, GLint iStarsArmament, GLint iStarsManoeuvring);
	GLvoid NoDraw();
	GLvoid Draw();
	GLvoid RestartObjects();

	GLboolean MouseLButtonDown(GLint iX, GLint iY);
	GLboolean KeyDown(GLuint uiKeyCode);

	inline GLint GetStarsShield() { return m_iStarsShield; }
	inline GLint GetStarsArmour() { return m_iStarsArmour; }
	inline GLint GetStarsArmament() { return m_iStarsArmament; }
	inline GLint GetStarsManoeuvring() { return m_iStarsManoeuvring; }

	inline GLvoid SetDraw(GLboolean bDraw) { m_bDraw = bDraw; }
	inline GLboolean IsDraw()		{ return m_bDraw; }

	inline GLvoid SetWasCalledDraw(GLboolean bDraw)	{ m_bWasCalledDraw = bDraw; }

	GLboolean CanAddStar(GLuint uiScore);

private:
	CGameControl	*m_pGameCtrl;
	CSDLFont		*m_pSDLTitle;
	CSDLFont		*m_pSDLFontText;
	CSDLFont		*m_pSDLFontParam;

	//regiony prostokatow
	RECT m_rRegionBackground[2]; // 0 - na caly ekrna, 1 - ramka pod okno
	//regiony dla przyciskow [+], piaty dla [OK]
	RECT m_rRegionsButton[MAX_REGIONS_ADD_STAR];
	RECT m_rRegionsFontButton[MAX_REGIONS_ADD_STAR];

	RECT m_rRegionsStars[12];
	RECT m_rRegionsStarsFont[12];

	GLvoid SetRegions();

	GLint m_iStarsShield;
	GLint m_iStarsArmour;
	GLint m_iStarsArmament;
	GLint m_iStarsManoeuvring;
	GLboolean m_bAddedStar;

	GLvoid DrawStarsParam();
	GLvoid DrawStar(GLint iIndex, GLboolean bFill);
	GLvoid DrawButtonts();

	GLint m_iCountPixelOffsetParamX;
	GLint m_iCountPixelOffsetParamY;

	GLboolean m_bDraw;
	GLboolean m_bWasCalledDraw;

	GLuint m_aScoreThresholds[MAX_ADD_STAR]; //MAX_ADD_STAR razy mozna dodac gwiazdke w czasie gry
	GLint m_iCounterAddStar; //zlicza ile razy dodano gwiazdke

	GLvoid SetPause(GLboolean bPause);
};
