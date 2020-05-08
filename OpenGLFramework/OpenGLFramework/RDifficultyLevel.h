#pragma once

#include "RMenuBase.h"

class CSDLFont;
class CGameControl;

#define MAX_REGIONS_DIFF_LEVEL 4

enum EDiffLevel
{
	ELevelNone,
	ELevelEasy,
	ELevelNormal,
	ELevelHard
};

class CRDifficultyLevel : public CRMenuBase
{
public:
	CRDifficultyLevel();
	~CRDifficultyLevel();

	//ustawienie regionow menu wzgledem rozdzielczosci
	GLvoid SetMouseRegion();
	GLvoid Draw();
	GLvoid RestartObjects();

	GLboolean MouseLButtonDown(GLint iX, GLint iY);
	GLboolean KeyDown(GLuint uiKey);

	inline EDiffLevel GetDifficultyLevel() { return m_eDiffucultyLevel; }
	inline GLvoid SetDifficultyLevel(EDiffLevel eLevel){ m_eDiffucultyLevel = eLevel; }

private:
	CGameControl *m_pGameCtrl;
	CSDLFont	*m_pSDLFontTitle;
	CSDLFont	*m_pSDLFontMenu;

	RECT m_rRegions[MAX_REGIONS_DIFF_LEVEL];
	RECT m_rRegions_Font[MAX_REGIONS_DIFF_LEVEL];

	EDiffLevel m_eDiffucultyLevel; //poziom trudnosci wybrany przez gracza
};
