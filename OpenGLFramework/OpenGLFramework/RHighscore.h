#pragma once

#include "RMenuBase.h"

#define MAX_REGIONS_HIGHSCORE 3 //0 - tlo pod itemsy, 1 - wyczysc i  2 - wyjscie
#define MAX_ITEMS_HIGHSCORE 10

struct SItem
{
	TCHAR cNickname[MAX_HIGHSCORE_NICKNAME_LEN];
	GLuint uiScore;
	GLint iLevel;
};

extern SItem g_sHighscoreItem[MAX_ITEMS_HIGHSCORE];

class CRHighscore : public CRMenuBase
{
public:
	CRHighscore();
	~CRHighscore();

	GLvoid SetMouseRegion();
	GLvoid Draw();
	GLvoid RestartObjects();

	GLboolean MouseLButtonDown(GLint iX, GLint iY);
	GLboolean KeyDown(GLuint uiKey);

	GLvoid SortItems();

private:
	CGameControl *m_pGameCtrl;
	CSDLFont *m_pSDLFontTitle;
	CSDLFont *m_pSDLFontMenu;
	CSDLFont *m_pSDLFontItems;

	RECT m_rRegions[MAX_REGIONS_HIGHSCORE];
	RECT m_rRegions_Font[MAX_REGIONS_HIGHSCORE];

	//dla fontow listy z ksywami i punktami
	POINT m_pRegions_Name[MAX_ITEMS_HIGHSCORE + 1]; //dla listy z ksywami
};
