
#include "StdAfx.h"
#include "WindowData.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "../Sound/MasterVolume.h"
#include "Joystick.h"
#include "ROptionsControl.h"

CROptionsControl::CROptionsControl()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_pSDLFontTitle		= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 50 );
	m_pSDLFontMenu		= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 23 );
	m_pSDLFontMenuSmall = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 16 );

	SetMouseRegion();

	COpenALManager::GetInstance()->SetSoundGain( CWindowData::GetInstance()->SettingFile().iVolumeSound );
	COpenALManager::GetInstance()->SetMusicGain( CWindowData::GetInstance()->SettingFile().iVolumeMusic );
}

CROptionsControl::~CROptionsControl()
{
	delete m_pSDLFontTitle;
	delete m_pSDLFontMenu;
	delete m_pSDLFontMenuSmall;
}

GLvoid CROptionsControl::SetMouseRegion()
{
	GLfloat iPercent = 21.0f;
	for( GLint i = 0; i < MAX_REGIONS_OPTIONS_CTRL; ++i ) {
		if( i == MAX_REGIONS_OPTIONS_CTRL - 1 )
			iPercent = 84.0f; //dla przycisku "wroc"

		SetRegionX( m_rRegions[ i ].left, 25 );
		SetRegionX( m_rRegions[ i ].right, 75 );

		SetRegionX( m_rRegions_Font[ i ].left, 25, GL_TRUE );
		SetRegionX( m_rRegions_Font[ i ].right, 75, GL_TRUE );

		SetRegionY( m_rRegions[ i ].top, iPercent );
		SetRegionY( m_rRegions_Font[ i ].top, iPercent, GL_TRUE );

		iPercent += ( i == MAX_REGIONS_OPTIONS_CTRL - 1 || i == 0 ) ? 7.0f : 4.5f;

		SetRegionY( m_rRegions[ i ].bottom, iPercent );
		SetRegionY( m_rRegions_Font[ i ].bottom, iPercent, GL_TRUE );
		iPercent += ( i == MAX_REGIONS_OPTIONS_CTRL - 1 || i == 0 ) ? 2.0f /*normalnie*/ : 0.5f;
	}
}

GLvoid CROptionsControl::RestartObjects()
{
	SetMouseRegion();
}

GLboolean CROptionsControl::KeyDown( GLuint uiKey )
{
	switch( uiKey ) {
		case VK_ESCAPE:
			if( m_pGameCtrl->GetJoystick()->IsBindKeyMode() )
				m_pGameCtrl->GetJoystick()->SetBindKeyMode( GL_FALSE );
			else
				Exit();

			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CROptionsControl::MouseLButtonDown( GLint iX, GLint iY )
{
	if( m_pGameCtrl->GetJoystick()->IsBindKeyMode() )
		return GL_FALSE;

	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegions, MAX_REGIONS_OPTIONS_CTRL, i ) )
		return GL_FALSE;

	if( !CWindowData::GetInstance()->SettingFile().bUseJoy && i > 0 && i < MAX_REGIONS_OPTIONS_CTRL - 1 )
		return GL_FALSE;

	switch( i ) {
		case 0: // uzyj joy'a
			CWindowData::GetInstance()->SettingFile().bUseJoy = !CWindowData::GetInstance()->SettingFile().bUseJoy;
			if( CWindowData::GetInstance()->SettingFile().bUseJoy && !m_pGameCtrl->GetJoystick()->IsJoysick() ) {
				CWindowData::GetInstance()->SettingFile().bUseJoy = GL_FALSE;
				m_pGameCtrl->GetRMessageBox()->DoMessageBox( LNG_LINE( 183 ), CRMessageBox::EMsgOK, NULL, NULL );
			}
			if( CWindowData::GetInstance()->SettingFile().bUseJoy )
				m_pGameCtrl->GetJoystick()->Initialize();
			return GL_TRUE;
		case 1: // poruszanie oœ x-y
			return GL_TRUE;
		case 2: // starzal
		case 3: // Odpalenie / detonacja zdalnej rakiety
		case 4: // Zmiana broni
		case 5: // Unik
		case 6: // Kamera
		case 7: // Wybór koloru celownika
		case 8: // Puza
		case 9: // Pomoc / puza
		case 10: // Wyjœcie / puza
			m_pGameCtrl->GetJoystick()->SetBindKeyMode( GL_TRUE, i - 2 );
			return GL_TRUE;
		case 11: //wroc
			Exit();
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CROptionsControl::Exit()
{
	//zapisz opcje
	CWindowData::GetInstance()->SaveSettingFile();
	m_pGameCtrl->SetEMainGameMode( EOptions );
}

GLvoid CROptionsControl::Draw()
{
	glLoadIdentity();

	glDisable( GL_LIGHTING );

	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), LNG_LINE( 166 ), GL_TRUE );

	DrawButtonts();
}

GLvoid CROptionsControl::DrawButtonts()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	//rysujemy przyciski
	if( !CWindowData::GetInstance()->SettingFile().bUseJoy ) {
		// rysuj przycisk pierwszy i ostatni
		DrawRegions( m_rRegions, 1 );
		DrawRegions( m_rRegions, MAX_REGIONS_OPTIONS_CTRL, MAX_REGIONS_OPTIONS_CTRL - 1 );
	}
	else
		DrawRegions( m_rRegions, MAX_REGIONS_OPTIONS_CTRL );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 0; i < MAX_REGIONS_OPTIONS_CTRL; ++i ) {
		if( !CWindowData::GetInstance()->SettingFile().bUseJoy && i > 0 && i < MAX_REGIONS_OPTIONS_CTRL - 1 )
			continue;

		if( i != 1 && CheckPtCursorInRect( m_pGameCtrl->GetPointCursor(), m_rRegions[ i ] ) ) {
			bPtCursorInRect = GL_TRUE;
			m_iWhereIsMouse = i;
			DrawRegionIfMouse( m_rRegions[ i ] );
		}
		else {
			bPtCursorInRect = GL_FALSE;
			if( m_iWhereIsMouse == i )
				m_iWhereIsMouseOld = -1; //nigdzie
		}

		if( i == MAX_REGIONS_OPTIONS_CTRL - 1 ) { //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
			m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 16 ) );
		}
		else {
			crColor = bPtCursorInRect ? RGB( 50, 255, 50 ) : RGB( 128, 128, 128 );

			switch( i ) {
				case 0: //uzyj joy'a
					m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 167 ),
						CWindowData::GetInstance()->SettingFile().bUseJoy ? LNG_LINE( 10 ) : LNG_LINE( 11 ) );
					break;
				case 1: //osi x i y
					m_pSDLFontMenuSmall->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 170 ) );
					break;
				case 2: //strzal
				case 3: //Odpalenie / detonacja zdalnej rakiety
				case 4: //Zmiana broni
				case 5: //Unik
				case 6: //Kamera
				case 7: //Wybór koloru celownika
				case 8: //Puza
				case 9: //Pomoc
				case 10: //wyjscie
					m_pSDLFontMenuSmall->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 171 + i - 2 ), GetButtonName( crColor, i ) );
					break;
			}
		}
	}
}

LPCTSTR	CROptionsControl::GetButtonName( COLORREF &crColor, GLint i )
{
	GLint iButtonNum = m_pGameCtrl->GetJoystick()->GetBindButtonNumFromKey( i - 2 );

	if( iButtonNum )
		::wsprintf( m_cButtonName, LNG_LINE( 184 ), iButtonNum );
	else
		::wsprintf( m_cButtonName, LNG_LINE( 185 ) );

	if( m_pGameCtrl->GetJoystick()->IsBindKeyMode() ) {
		if( i == m_pGameCtrl->GetJoystick()->GetIndexBindKey() + 2 )
			crColor = RGB( 255, 0, 0 );
	}

	return m_cButtonName;
}