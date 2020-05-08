#pragma once

class CGameControl;

#define MAX_REGION_BONUSES 11
#define KEYBOARD_NUMBER 11

class CRHelp : public CRMenuBase
{
public:
	CRHelp();
	~CRHelp();

	GLvoid SetRegions();
	GLvoid RestartObjects();
	GLvoid Draw();

private:
	CGameControl	*m_pGameCtrl;
	CSDLFont		*m_pFont;

	POINT m_ptBonuses[MAX_REGION_BONUSES]; //srodek rysunku bonusa
	GLint m_iFontMoveX;
	GLint m_iFontMoveY;
};
