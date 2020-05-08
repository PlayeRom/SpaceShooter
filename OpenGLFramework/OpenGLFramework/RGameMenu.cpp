#include "StdAfx.h"
#include "GameControl.h"
#include "RGame.h"
#include "Cursor.h"
#include "ROptions.h"
#include "../Fonts/SDLFont.h"
#include "RGameMenu.h"

CRGameMenu::CRGameMenu()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_pSDLFontMenu = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 23 );

	SetMouseRegion();

	m_bDraw = GL_FALSE;
}

CRGameMenu::~CRGameMenu()
{
	delete m_pSDLFontMenu;
}

GLvoid CRGameMenu::SetMouseRegion()
{
	m_rRegions[ 0 ].left = 0;
	m_rRegions[ 0 ].right = CWindowData::GetInstance()->SettingFile().iWidth;
	m_rRegions[ 0 ].top = 0;
	m_rRegions[ 0 ].bottom = CWindowData::GetInstance()->SettingFile().iHeight;

	GLint iPercent = 39;
	for( GLint i = 1; i < MAX_REGIONS_GAME_MENU; ++i ) {
		SetRegionX( m_rRegions[ i ].left, 25 );
		SetRegionX( m_rRegions[ i ].right, 75 );

		SetRegionX( m_rRegions_Font[ i ].left, 25, GL_TRUE );
		SetRegionX( m_rRegions_Font[ i ].right, 75, GL_TRUE );

		SetRegionY( m_rRegions[ i ].top, iPercent );
		SetRegionY( m_rRegions_Font[ i ].top, iPercent, GL_TRUE );
		iPercent += 7;
		SetRegionY( m_rRegions[ i ].bottom, iPercent );
		SetRegionY( m_rRegions_Font[ i ].bottom, iPercent, GL_TRUE );
		iPercent += 2;
	}
}

GLvoid CRGameMenu::RestartObjects()
{
	SetMouseRegion();
}

GLboolean CRGameMenu::KeyDown( GLuint uiKey )
{
	switch( uiKey ) {
		case VK_ESCAPE:
			m_bDraw = GL_FALSE;
			MsgExitGameNo();
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CRGameMenu::MouseLButtonDown( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, &m_rRegions[ 1 ], MAX_REGIONS_GAME_MENU - 1, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //continue
			m_bDraw = GL_FALSE;
			MsgExitGameNo();
			return GL_TRUE;
		case 1: //options
			m_pGameCtrl->GetSpeedCtrl()->SetPause( GL_FALSE );
			m_pGameCtrl->GetROptions()->ReturnToGame();
			m_pGameCtrl->SetEMainGameMode( EOptions );
			m_pGameCtrl->StartPlayMusicForMenu();
			return GL_TRUE;
		case 2: //exit
			m_pGameCtrl->GetRMessageBox()->DoMessageBox( LNG_LINE( 34 ), CRMessageBox::EMsgYesNo, MsgExitGameYes, NULL );
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CRGameMenu::Draw()
{
	if( !m_bDraw )
		return;

	glLoadIdentity();

	glDisable( GL_LIGHTING );

	DrawButtonts();
}

GLvoid CRGameMenu::DrawButtonts()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	//rysujemy przyciski
	DrawRegions( m_rRegions, MAX_REGIONS_GAME_MENU);

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 1; i < MAX_REGIONS_GAME_MENU; ++i ) {
		if( CheckPtCursorInRect( m_pGameCtrl->GetPointCursor(), m_rRegions[ i ] ) ) {
			bPtCursorInRect = GL_TRUE;
			m_iWhereIsMouse = i;
			DrawRegionIfMouse( m_rRegions[ i ] );
		}
		else {
			bPtCursorInRect = GL_FALSE;
			if( m_iWhereIsMouse == i )
				m_iWhereIsMouseOld = -1; //nigdzie
		}

		if( i == MAX_REGIONS_GAME_MENU - 1 ) { //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
			m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 6 ) );
		}
		else {
			crColor = bPtCursorInRect ? RGB( 50, 255, 50 ) : RGB( 128, 128, 128 );
			switch( i ) {
				case 1: //"Kontynuuj"
					m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 127 ) );
					break;
				case 2: //"Opcje"
					m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 2 ) );
					break;
			}
		}
	}
}

GLvoid MsgExitGameYes( GLvoid ) //odpowiedz na klikniecie na tak w msgboxie przy wychodzeniu z gry
{
	CGameControl::GetInstance()->GetSpeedCtrl()->SetPause( GL_FALSE );
	CGameControl::GetInstance()->GetRGame()->SetEndPlayerVariables( EMainMenu );
}

GLvoid MsgExitGameNo( GLvoid )
{
	CGameControl::GetInstance()->GetRGame()->SetPressKeyEsc( !CGameControl::GetInstance()->GetRGame()->GetPressKeyEsc() );
	if( CGameControl::GetInstance()->GetRGame()->GetPressKeyP()
		|| CGameControl::GetInstance()->GetRGame()->GetPressKeyF1() )
	{
		//nadal pauzuj
		CGameControl::GetInstance()->GetRGame()->SetPause( GL_TRUE );
		CGameControl::GetInstance()->GetSpeedCtrl()->SetPause( GL_TRUE );
	}
	else {
		CGameControl::GetInstance()->GetRGame()->SetPause( GL_FALSE );
		CGameControl::GetInstance()->GetSpeedCtrl()->SetPause( GL_FALSE );
	}

	CGameControl::GetInstance()->GetRGame()->GetPlayer()->SetCursorPosAfterPause();
	CGameControl::GetInstance()->GetCursor()->SetMyCursor( CCursor::EGame );
}