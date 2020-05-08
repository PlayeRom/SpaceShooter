
#include "StdAfx.h"
#include "WindowData.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "ROptionsGraphics.h"
#include "RGame.h"
#include "Joystick.h"
#include "ROptions.h"

CROptions::CROptions()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_pSDLFontTitle = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 50 );
	m_pSDLFontMenu = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 23 );

	SetMouseRegion();

	m_bReturnToGame = GL_FALSE;
}

CROptions::~CROptions()
{
	delete m_pSDLFontTitle;
	delete m_pSDLFontMenu;
}

GLvoid CROptions::SetMouseRegion()
{
	GLint iPercent = 30;
	for( GLint i = 0; i < MAX_REGIONS_OPTIONS; ++i ) {
		if( i == MAX_REGIONS_OPTIONS - 1 ) //dla przycisku "wroc"
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

GLvoid CROptions::RestartObjects()
{
	SetMouseRegion();
}

GLboolean CROptions::KeyDown( GLuint uiKey )
{
	switch( uiKey ) {
		case VK_ESCAPE:
			if( m_bReturnToGame )
				DoReturnToGame();
			else
				m_pGameCtrl->SetEMainGameMode( EMainMenu );
			m_bReturnToGame = GL_FALSE;
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CROptions::MouseLButtonDown( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegions, MAX_REGIONS_OPTIONS, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //graphic
			m_pGameCtrl->GetROptionsGraphics()->SetStartGraphiscOptions();
			m_pGameCtrl->SetEMainGameMode( EOptionsGraphics );
			return GL_TRUE;
		case 1: //music
			m_pGameCtrl->SetEMainGameMode( EOptionsSound );
			return GL_TRUE;
		case 2: //language
			++CWindowData::GetInstance()->SettingFile().iIndexLanguage;
			if( CWindowData::GetInstance()->SettingFile().iIndexLanguage >= CMultiLanguage::GetInstance()->GetLangFileNumber() ) {
				CWindowData::GetInstance()->SettingFile().iIndexLanguage = 0;
			}
			if( !CMultiLanguage::GetInstance()->LoadLanguage( CWindowData::GetInstance()->SettingFile().iIndexLanguage ) ) {
				//TODO: error
			}
			//zapisz opcje
			CWindowData::GetInstance()->SaveSettingFile();
			//wywolanie restaru dla CRGame aby ustawil nowe regiony dla pionowych napisow na wskaznikach oslony i pancerza
			m_pGameCtrl->GetRGame()->SetRegions();
			return GL_TRUE;
		case 3: // sterowanie
			// strawdz czy jest joystick
			if( CWindowData::GetInstance()->SettingFile().bUseJoy && !m_pGameCtrl->GetJoystick()->IsJoysick() )
				CWindowData::GetInstance()->SettingFile().bUseJoy = GL_FALSE;
			m_pGameCtrl->SetEMainGameMode( EOptionsControl );
			return GL_TRUE;
		case 4: //wroc
			if( m_bReturnToGame )
				DoReturnToGame();
			else
				m_pGameCtrl->SetEMainGameMode( EMainMenu );
			m_bReturnToGame = GL_FALSE;
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CROptions::DoReturnToGame()
{
	m_pGameCtrl->GetSpeedCtrl()->SetPause( GL_TRUE );
	m_pGameCtrl->SetEMainGameMode( ESingleGame );
	m_pGameCtrl->StartPlayMusicForGame();
}

GLvoid CROptions::Draw()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), LNG_LINE( 2 ), GL_TRUE );

	DrawButtonts();
}

GLvoid CROptions::DrawButtonts()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	//rysujemy przyciski
	DrawRegions( m_rRegions, MAX_REGIONS_OPTIONS );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 0; i < MAX_REGIONS_OPTIONS; ++i ) {
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

		if( i == MAX_REGIONS_OPTIONS - 1 ) { //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
			m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 16 ) );
		}
		else {
			crColor = bPtCursorInRect ? RGB( 50, 255, 50 ) : RGB( 128, 128, 128 );
			if( i == 2 )
				m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( i + 64 ),
												CMultiLanguage::GetInstance()->GetXMLLngName() );
			else
				m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( i + 64 ) );
		}
	}
}