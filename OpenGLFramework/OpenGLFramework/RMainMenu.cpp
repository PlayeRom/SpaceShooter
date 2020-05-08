// RMainMenu.cpp: implementation of the CRMainMenu class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "WindowData.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "Cursor.h"
#include "RShipsBrowse.h"
#include "ROptions.h"
#if _USE_MULTIPLYER_
	#include "RMultiplayerMenu.h"
#endif
#include "RCredits.h"
#include "RMainMenu.h"

#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRMainMenu::CRMainMenu()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_pSDLFontTitle = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 3 ), 70 );
	m_pSDLFontMenu = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 23 );
	m_pSDLFontVersion = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 12 );

	SetMouseRegion();

	SetStartMenuAnimY();
}

CRMainMenu::~CRMainMenu()
{
	delete m_pSDLFontTitle;
	delete m_pSDLFontMenu;
	delete m_pSDLFontVersion;
}

GLvoid CRMainMenu::SetMouseRegion()
{
	GLint iPercent = 21;
	for( GLint i = 0; i < MAX_REGIONS; ++i ) {
		if( i == MAX_REGIONS - 1 ) //dla przycisku "Exit"
			iPercent = 84;
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

GLvoid CRMainMenu::SetStartMenuAnimY( GLboolean bSet )
{
	if( bSet ) {
		m_fMenuAnimY = -static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iHeight );
		m_fMuliplierMenuAnim = 768.0f / static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iHeight );
		m_fMenuAnimYFont = m_fMenuAnimY * m_fMuliplierMenuAnim;
	}
	else {
		m_fMenuAnimY = 0.0f;
		m_fMenuAnimYFont = 0.0f;
	}
}

GLvoid CRMainMenu::RestartObjects()
{
	SetMouseRegion();
}

GLboolean CRMainMenu::KeyDown( GLuint uiKey )
{
	if( m_fMenuAnimY < 0.0f ) {
		m_fMenuAnimY = 0.0f;
		return GL_TRUE;
	}

	switch( uiKey ) {
		case VK_ESCAPE:
			m_pGameCtrl->Exit();
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CRMainMenu::MouseLButtonDown( GLint iX, GLint iY )
{
	if( m_fMenuAnimY < 0.0f ) {
		m_fMenuAnimY = 0.0f;
		return GL_TRUE;
	}

	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegions, MAX_REGIONS, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //new game
			m_pGameCtrl->SetEMainGameMode( EDifficultyLevel );
			return GL_TRUE;
		case 1: //enemy ships browser
			m_pGameCtrl->GetRShipsBrowse()->SetStartVariables();
			m_pGameCtrl->GetRShipsBrowse()->SetMode( EBrowseEnemyShips );
			m_pGameCtrl->SetEMainGameMode( EShipsBrowser );
			return GL_TRUE;
		case 2: //options
			m_pGameCtrl->GetROptions()->SetOldIndexLanguage( CWindowData::GetInstance()->SettingFile().iIndexLanguage );
			m_pGameCtrl->SetEMainGameMode( EOptions );
			return GL_TRUE;
		case 3: //highscore
			m_pGameCtrl->SetEMainGameMode( EHighscore );
			return GL_TRUE;
#if _USE_MULTIPLYER_
		case 4: //multiplayer
			m_pGameCtrl->GetRMultiplayerMenu()->SetStartVariables();
			m_pGameCtrl->SetEMainGameMode( EMultiplayerMenu );
			//m_pGameCtrl->GetRMessageBox()->DoMessageBox( _T("Not available yet."), EMsgOK, NULL, NULL );
			return GL_TRUE;
		case 5: //credits
			m_pGameCtrl->GetRCredits()->SetStartVariables();
			m_pGameCtrl->SetEMainGameMode( ECredits );
			return GL_TRUE;
		case 6: //exit
			m_pGameCtrl->Exit();
			return GL_TRUE;
#else
		case 4: //credits
			m_pGameCtrl->GetRCredits()->SetStartVariables();
			m_pGameCtrl->SetEMainGameMode( ECredits );
			return GL_TRUE;
		case 5: //exit
			m_pGameCtrl->Exit();
			return GL_TRUE;
#endif
	}

	return GL_FALSE;
}

GLvoid CRMainMenu::Draw()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), GAME_NAME, GL_TRUE );
	m_pSDLFontVersion->DrawTextFormat( 2, 768, GL_FALSE, RGB( 255, 255, 255 ), _T("v. %d.%d.%d"), MAJOR_VERSION, MINOR_VERSION, RELEASE_VERSION );

	DrawButtonts();
}

GLvoid CRMainMenu::DrawButtonts()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	if( m_fMenuAnimYFont == 0.0f ) //rysujemy przyciski juz bez animacji
		DrawRegions( m_rRegions, MAX_REGIONS );
	else 
	{
		// animuj
		m_fMenuAnimY += ( 400.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( m_fMenuAnimY > 0.0f )
			m_fMenuAnimY = 0.0f;

		m_fMenuAnimYFont = m_fMenuAnimY * m_fMuliplierMenuAnim;

		//rysujemy przyciski z animacja
		RECT rTempRegions[ MAX_REGIONS ];
		::memcpy_s( rTempRegions, sizeof( rTempRegions ), m_rRegions, sizeof( m_rRegions ) );
		for( GLint i = 0; i < MAX_REGIONS; ++i ) {
			rTempRegions[ i ].bottom -= static_cast< GLint >( m_fMenuAnimY );
			rTempRegions[ i ].top -= static_cast< GLint >( m_fMenuAnimY );
		}

		DrawRegions( rTempRegions, MAX_REGIONS );
	}

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 0; i < MAX_REGIONS; ++i ) {
		if( CheckPtCursorInRect( m_pGameCtrl->GetPointCursor(), m_rRegions[ i ] ) && m_fMenuAnimY == 0.0f ) {
			bPtCursorInRect = GL_TRUE;
			m_iWhereIsMouse = i;
			DrawRegionIfMouse( m_rRegions[ i ] );
		}
		else {
			bPtCursorInRect = GL_FALSE;
			if( m_iWhereIsMouse == i )
				m_iWhereIsMouseOld = -1; //nigdzie
		}

		RECT rcFont = m_rRegions_Font[ i ];
		rcFont.bottom -= static_cast< GLint >( m_fMenuAnimYFont );
		rcFont.top -= static_cast< GLint >( m_fMenuAnimYFont );

		if( i == MAX_REGIONS - 1 ) //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
		else
			crColor = bPtCursorInRect ? RGB( 50, 255, 50 ) : RGB( 128, 128, 128 );

#if _USE_MULTIPLYER_
		m_pSDLFontMenu->DrawText( rcFont, crColor, LNG_LINE( i ) );
#else
		int iLng = i;
		if( i >= 4 )
			++iLng;
		m_pSDLFontMenu->DrawText( rcFont, crColor, LNG_LINE( iLng ) );
#endif
	}
}