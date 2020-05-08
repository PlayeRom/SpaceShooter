
#include "StdAfx.h"
#include "WindowData.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "../Sound/MasterVolume.h"
#include "ROptionsSound.h"

CROptionsSound::CROptionsSound()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_pSDLFontTitle = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 50 );
	m_pSDLFontMenu = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 23 );

	SetMouseRegion();

	COpenALManager::GetInstance()->SetSoundGain( CWindowData::GetInstance()->SettingFile().iVolumeSound );
	COpenALManager::GetInstance()->SetMusicGain( CWindowData::GetInstance()->SettingFile().iVolumeMusic );

	m_aIndices = new CVector2[ 4 ];
}

CROptionsSound::~CROptionsSound()
{
	delete m_pSDLFontTitle;
	delete m_pSDLFontMenu;
	delete [] m_aIndices;
}

GLvoid CROptionsSound::SetMouseRegion()
{
	GLint iPercent = 30;
	for( GLint i = 0; i < MAX_REGIONS_OPTIONS_SD; ++i ) {
		if( i == MAX_REGIONS_OPTIONS_SD - 3 || i == MAX_REGIONS_OPTIONS_SD - 2
			|| i == MAX_REGIONS_OPTIONS_SD - 5 || i == MAX_REGIONS_OPTIONS_SD - 6 )
			continue;
		if( i == MAX_REGIONS_OPTIONS_SD - 1 )
			iPercent = 84; //dla przycisku "wroc"
		//ustawienie dla czterech regionów:
		//1. Sound on/off
		//2. Music on/off
		//3. 'suwak' - ale z innym left i right		MAX_REGIONS_OPTIONS_SD - 7
		//4. [-]									MAX_REGIONS_OPTIONS_SD - 6
		//5. [+]									MAX_REGIONS_OPTIONS_SD - 5
		//6. 'suwak' - ale z innym left i right		MAX_REGIONS_OPTIONS_SD - 4
		//7. [-]									MAX_REGIONS_OPTIONS_SD - 3
		//8. [+]									MAX_REGIONS_OPTIONS_SD - 2
		//9. Wroc									MAX_REGIONS_OPTIONS_SD - 1
		SetRegionX( m_rRegions[ i ].left, 25 );
		SetRegionX( m_rRegions[ i ].right, 75 );

		SetRegionX( m_rRegions_Font[ i ].left, 25, GL_TRUE );
		SetRegionX( m_rRegions_Font[ i ].right, 75, GL_TRUE );

		SetRegionY( m_rRegions[ i ].top, iPercent );
		SetRegionY( m_rRegions_Font[ i ].top, iPercent, GL_TRUE );
		iPercent += 7;
		SetRegionY( m_rRegions[ i ].bottom, iPercent );
		SetRegionY( m_rRegions_Font[ i ].bottom, iPercent, GL_TRUE );
		iPercent += 2; //normalnie

		if( i == MAX_REGIONS_OPTIONS_SD - 4 || i == MAX_REGIONS_OPTIONS_SD - 7 ) {
			//suwak, modyfikacja left i right
			SetRegionX( m_rRegions[ i ].left, 25+8 );
			SetRegionX( m_rRegions[ i ].right, 75-8 );

			SetRegionX( m_rRegions_Font[ i ].left, 25+8, GL_TRUE );
			SetRegionX( m_rRegions_Font[ i ].right, 75-8, GL_TRUE );
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//regiony dla przycisku [-], po lewej stronie suwaka
	GLint i = MAX_REGIONS_OPTIONS_SD - 6; //dla [-]
	GLint j = MAX_REGIONS_OPTIONS_SD - 7; //region do rysowania suwaka
	SetRegionX( m_rRegions[ i ].left, 25 );
	SetRegionX( m_rRegions[ i ].right, 25+7 );

	SetRegionX( m_rRegions_Font[ i ].left, 25, GL_TRUE );
	SetRegionX( m_rRegions_Font[ i ].right, 25+7, GL_TRUE );

	m_rRegions[ i ].top = m_rRegions[ j ].top;
	m_rRegions_Font[ i ].top = m_rRegions_Font[ j ].top;

	m_rRegions[ i ].bottom = m_rRegions[ j ].bottom;
	m_rRegions_Font[ i ].bottom = m_rRegions_Font[ j ].bottom;

	///////////////////////////////////////////////////////////////////////////
	//regiony dla przycisku [+], po prawej stronie suwaka
	i = MAX_REGIONS_OPTIONS_SD - 5; //dla [+]
	SetRegionX( m_rRegions[ i ].left, 75-7 );
	SetRegionX( m_rRegions[ i ].right, 75 );

	SetRegionX( m_rRegions_Font[ i ].left, 75-7, GL_TRUE );
	SetRegionX( m_rRegions_Font[ i ].right, 75, GL_TRUE );

	m_rRegions[ i ].top = m_rRegions[ j ].top;
	m_rRegions_Font[ i ].top = m_rRegions_Font[ j ].top;

	m_rRegions[ i ].bottom = m_rRegions[ j ].bottom;
	m_rRegions_Font[ i ].bottom = m_rRegions_Font[ j ].bottom;

	///////////////////////// druga para [+][-] ////////////////
	///////////////////////// dla muzyki ///////////////////////

	///////////////////////////////////////////////////////////////////////////
	//regiony dla przycisku [-], po lewej stronie suwaka
	i = MAX_REGIONS_OPTIONS_SD - 3; //dla [-]
	j = MAX_REGIONS_OPTIONS_SD - 4; //region do rysowania suwaka
	SetRegionX( m_rRegions[ i ].left, 25 );
	SetRegionX( m_rRegions[ i ].right, 25+7 );

	SetRegionX( m_rRegions_Font[ i ].left, 25, GL_TRUE );
	SetRegionX( m_rRegions_Font[ i ].right, 25+7, GL_TRUE );

	m_rRegions[ i ].top = m_rRegions[ j ].top;
	m_rRegions_Font[ i ].top = m_rRegions_Font[ j ].top;

	m_rRegions[ i ].bottom = m_rRegions[ j ].bottom;
	m_rRegions_Font[ i ].bottom = m_rRegions_Font[ j ].bottom;

	///////////////////////////////////////////////////////////////////////////
	//regiony dla przycisku [+], po prawej stronie suwaka
	i = MAX_REGIONS_OPTIONS_SD - 2; //dla [+]
	SetRegionX( m_rRegions[ i ].left, 75-7 );
	SetRegionX( m_rRegions[ i ].right, 75 );

	SetRegionX( m_rRegions_Font[ i ].left, 75-7, GL_TRUE );
	SetRegionX( m_rRegions_Font[ i ].right, 75, GL_TRUE );

	m_rRegions[ i ].top = m_rRegions[ j ].top;
	m_rRegions_Font[ i ].top = m_rRegions_Font[ j ].top;

	m_rRegions[ i ].bottom = m_rRegions[ j ].bottom;
	m_rRegions_Font[ i ].bottom = m_rRegions_Font[ j ].bottom;
}

GLvoid CROptionsSound::RestartObjects()
{
	SetMouseRegion();
}

GLboolean CROptionsSound::KeyDown( GLuint uiKey )
{
	switch( uiKey ) {
		case VK_ESCAPE:
			Exit();
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CROptionsSound::MouseLButtonDown( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegions, MAX_REGIONS_OPTIONS_SD, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //sound on/off
			CWindowData::GetInstance()->SettingFile().bSound = !CWindowData::GetInstance()->SettingFile().bSound;
			return GL_TRUE;
		case 1: //music on/off
			CWindowData::GetInstance()->SettingFile().bMusic = !CWindowData::GetInstance()->SettingFile().bMusic;
#if _USE_SOUND_OPENAL_ && _USE_OGG_
			if( !CWindowData::GetInstance()->SettingFile().bMusic )
				COpenALManager::GetInstance()->Stop( m_pGameCtrl->m_aMusicMenu[ m_pGameCtrl->m_iActualMusicMenu ] );
			else
				COpenALManager::GetInstance()->Play( m_pGameCtrl->m_aMusicMenu[ m_pGameCtrl->m_iActualMusicMenu ] );
#endif
			return GL_TRUE;
		case 2://napis 'volume of sound:'
			//ustaw na domyslne
			CWindowData::GetInstance()->SettingFile().iVolumeSound = DEFAULT_SOUND_VOLUME;
			COpenALManager::GetInstance()->SetSoundGain( CWindowData::GetInstance()->SettingFile().iVolumeSound );
			return GL_TRUE;
		case 3: //[-]
			CWindowData::GetInstance()->SettingFile().iVolumeSound -= 10;
			if( CWindowData::GetInstance()->SettingFile().iVolumeSound < 0 )
				CWindowData::GetInstance()->SettingFile().iVolumeSound = 0;
			COpenALManager::GetInstance()->SetSoundGain( CWindowData::GetInstance()->SettingFile().iVolumeSound );
			return GL_TRUE;
		case 4://[+]
			CWindowData::GetInstance()->SettingFile().iVolumeSound += 10;
			if( CWindowData::GetInstance()->SettingFile().iVolumeSound > 100 )
				CWindowData::GetInstance()->SettingFile().iVolumeSound = 100;
			COpenALManager::GetInstance()->SetSoundGain( CWindowData::GetInstance()->SettingFile().iVolumeSound );
			return GL_TRUE;
		/////////////////////////////////////////////
		case 5://napis 'volume of music:'
			//ustaw na domyslne
			CWindowData::GetInstance()->SettingFile().iVolumeMusic = DEFAULT_MUSIC_VOLUME;
			COpenALManager::GetInstance()->SetMusicGain( CWindowData::GetInstance()->SettingFile().iVolumeMusic );
			return GL_TRUE;
		case 6: //[-]
			CWindowData::GetInstance()->SettingFile().iVolumeMusic -= 10;
			if( CWindowData::GetInstance()->SettingFile().iVolumeMusic < 0 )
				CWindowData::GetInstance()->SettingFile().iVolumeMusic = 0;
			COpenALManager::GetInstance()->SetMusicGain( CWindowData::GetInstance()->SettingFile().iVolumeMusic );
			return GL_TRUE;
		case 7://[+]
			CWindowData::GetInstance()->SettingFile().iVolumeMusic += 10;
			if( CWindowData::GetInstance()->SettingFile().iVolumeMusic > 100 )
				CWindowData::GetInstance()->SettingFile().iVolumeMusic = 100;
			COpenALManager::GetInstance()->SetMusicGain( CWindowData::GetInstance()->SettingFile().iVolumeMusic );
			return GL_TRUE;
		case 8: //wroc
			Exit();
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CROptionsSound::Exit()
{
	//zapisz opcje
	CWindowData::GetInstance()->SaveSettingFile();
	m_pGameCtrl->SetEMainGameMode( EOptions );
}

GLvoid CROptionsSound::Draw()
{
	glLoadIdentity();

	glDisable( GL_LIGHTING );

	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), LNG_LINE( 81 ), GL_TRUE );

	DrawButtonts();
}

GLvoid CROptionsSound::DrawSlider( GLint iIndexOfSlider )
{
	//rysujemy suwak
	glColor4ub( 255, 255, 255, 255 );
	GLint iPixelsSeparate = 5;
	GLint i = iIndexOfSlider; //region suwaka 

	glEnableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	m_aIndices[ 0 ] = CVector2( GLfloat( m_rRegions[ i ].left + iPixelsSeparate ), GLfloat( m_rRegions[ i ].bottom - 8 ) );
	m_aIndices[ 1 ] = CVector2( GLfloat( m_rRegions[ i ].right - iPixelsSeparate ), GLfloat( m_rRegions[ i ].bottom - 8 ) );
	m_aIndices[ 2 ] = CVector2( GLfloat( m_rRegions[ i ].right - iPixelsSeparate ), GLfloat( m_rRegions[ i ].top + 8 ) );

	glVertexPointer( 2, GL_FLOAT, 0, m_aIndices );
	glDrawArrays( GL_LINE_LOOP, 0, 3 );

	//dlugosc suwaka
	GLint iLengthSlider = m_rRegions[ i ].right - m_rRegions[ i ].left - ( iPixelsSeparate * 2 );
	//aktualna pozycja suwaka
	GLint iPercent = 0;
	if( iIndexOfSlider == MAX_REGIONS_OPTIONS_SD - 7 )
		iPercent = COpenALManager::GetInstance()->GetPercentSoundGain();
	else
		iPercent = COpenALManager::GetInstance()->GetPercentMusicGain();
	GLint iSliderPosition = ( iLengthSlider * iPercent ) / 100;
	iSliderPosition += m_rRegions[ i ].left + iPixelsSeparate;

	glColor4ub( 16, 255, 16, 255 );

	m_aIndices[ 0 ] = CVector2( GLfloat( iSliderPosition + 2 ), GLfloat( m_rRegions[ i ].bottom - iPixelsSeparate ) );
	m_aIndices[ 1 ] = CVector2( GLfloat( iSliderPosition + 2 ), GLfloat( m_rRegions[ i ].top + iPixelsSeparate ) );
	m_aIndices[ 2 ] = CVector2( GLfloat( iSliderPosition - 2 ), GLfloat( m_rRegions[ i ].bottom - iPixelsSeparate ) );
	m_aIndices[ 3 ] = CVector2( GLfloat( iSliderPosition - 2 ), GLfloat( m_rRegions[ i ].top + iPixelsSeparate ) );

	glVertexPointer( 2, GL_FLOAT, 0, m_aIndices );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}

GLvoid CROptionsSound::DrawButtonts()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	//rysujemy przyciski
	DrawRegions( m_rRegions, MAX_REGIONS_OPTIONS_SD );

	DrawSlider( MAX_REGIONS_OPTIONS_SD - 7 ); // suwak dla dzwieku
	DrawSlider( MAX_REGIONS_OPTIONS_SD - 4 ); // suwak dla muzyki

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 0; i < MAX_REGIONS_OPTIONS_SD; ++i ) {
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

		if( i == MAX_REGIONS_OPTIONS_SD - 1 ) { //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
			m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 16 ) );
		}
		else {
			crColor = bPtCursorInRect ? RGB( 50, 255, 50 ) : RGB( 128, 128, 128 );

			switch( i ) {
				case 0://sound
					m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 82 ),
						CWindowData::GetInstance()->SettingFile().bSound ? LNG_LINE( 10 ) : LNG_LINE( 11 ) );
					break;
				case 1://music
					m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 83 ),
						CWindowData::GetInstance()->SettingFile().bMusic ? LNG_LINE( 10 ) : LNG_LINE( 11 ) );
					break;
				case 2://volume od sound:
					crColor = RGB( 128, 128, 128 );
					m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 84 ), CWindowData::GetInstance()->SettingFile().iVolumeSound );
					break;
				case 3://[-]
					m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 85 ) );
					break;
				case 4://[+]
					m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 86 ) );
					break;
				////////////////////////////
				case 5://volume of music:
					crColor = RGB( 128, 128, 128 );
					m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 87 ), CWindowData::GetInstance()->SettingFile().iVolumeMusic );
					break;
				case 6://[-]
					m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 85 ) );
					break;
				case 7://[+]
					m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 86 ) );
					break;
			}
		}
	}
}