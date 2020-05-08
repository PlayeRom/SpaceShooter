#pragma once

#include "RMenuBase.h"

class CSDLFont;
class CGameControl;

GLvoid MsgExitGameYes(GLvoid);
GLvoid MsgExitGameNo(GLvoid);

#define MAX_REGIONS_GAME_MENU 4 // ale 0 to plansza na caly ekran

class CRGameMenu : public CRMenuBase
{
public:
	CRGameMenu();
	~CRGameMenu();

	//ustawienie regionow menu wzgledem rozdzielczosci
	GLvoid SetMouseRegion();
	GLvoid Draw();
	GLvoid DrawButtonts();
	GLvoid RestartObjects();

	GLboolean MouseLButtonDown( GLint iX, GLint iY );
	GLboolean KeyDown( GLuint uiKey );

	//GLboolean m_bChangeCursor;

	inline GLvoid DoDraw()		{ m_bDraw = GL_TRUE; };
	inline GLvoid NoDraw()		{ m_bDraw = GL_FALSE; };
	inline GLboolean IsDraw()	{ return m_bDraw; }

private:
	CGameControl	*m_pGameCtrl;
	CSDLFont		*m_pSDLFontMenu;

	RECT m_rRegions[ MAX_REGIONS_GAME_MENU ];
	RECT m_rRegions_Font[ MAX_REGIONS_GAME_MENU ];

	GLboolean m_bDraw; //rysuj sie
};
