
#include "StdAfx.h"
#include "WindowData.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "../Draw/Brightness.h"
#include "ROptionsGraphics.h"

GLvoid MsgRestartWindow( GLvoid );

CROptionsGraphics::CROptionsGraphics()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_pSDLFontTitle = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 50 );
	m_pSDLFontMenu = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 23 );

	SetMouseRegion();

	m_bNeedRestart = GL_FALSE;

	m_aIndices = new CVector2[ 4 ];

	//tworzymy tablice z rodzielczosciami, gdzie wstawiamy rozdzielczosc ekranu
	//tak aby nie mozna bylo ustawic rozdzielczosci wiekszej od ekranowej
	GLint iResX = CWindowData::GetInstance()->GetDesktopWidth();
	GLint iResY = CWindowData::GetInstance()->GetDesktopHeight();
	if( iResX <= 800 || iResY <= 600 ) {
		SetScreenResToArray();
		return;
	}
	m_aWidth.push_back( 800 );
	m_aHeight.push_back( 600 );
	if( iResX <= 1024 || iResY <= 768 ) {
		SetScreenResToArray();
		return;
	}
	m_aWidth.push_back( 1024 );
	m_aHeight.push_back( 768 );
	if( iResX <= 1152 || iResY <= 864 ) {
		SetScreenResToArray();
		return;
	}
	m_aWidth.push_back( 1152 );
	m_aHeight.push_back( 864 );
	if( iResX <= 1280 || iResY <= 960 ) {
		SetScreenResToArray();
		return;
	}
	m_aWidth.push_back( 1280 );
	m_aHeight.push_back( 960 );
	if( iResX <= 1600 || iResY <= 1200 ) {
		SetScreenResToArray();
		return;
	}
	m_aWidth.push_back( 1600 );
	m_aHeight.push_back( 1200 );
	if( iResX <= 1920 || iResY <= 1440 ) {
		SetScreenResToArray();
		return;
	}
	m_aWidth.push_back( 1920 );
	m_aHeight.push_back( 1440 );
	if( iResX <= 2048 || iResY <= 1536 ) {
		SetScreenResToArray();
		return;
	}
	m_aWidth.push_back( 2048 );
	m_aHeight.push_back( 1536 );
}

GLvoid CROptionsGraphics::SetScreenResToArray()
{
	m_aWidth.push_back( CWindowData::GetInstance()->GetDesktopWidth() );
	m_aHeight.push_back( CWindowData::GetInstance()->GetDesktopHeight() );
	m_iMaxIndexRes = static_cast< GLint >( m_aWidth.size() - 1 );
}

GLvoid CROptionsGraphics::SetStartGraphiscOptions()
{
	m_bNeedRestart = GL_FALSE;
	m_ubOldBitsPerPixel = CWindowData::GetInstance()->SettingFile().ubBitsPerPixel;
	m_iOldWidth = CWindowData::GetInstance()->SettingFile().iWidth;
	m_iOldHeight = CWindowData::GetInstance()->SettingFile().iHeight;
	m_iOldAntialiasing = CWindowData::GetInstance()->SettingFile().iAntialiasing;
	m_bOldFullScreen = CWindowData::GetInstance()->SettingFile().bFullScreen;
	m_bSelectedFullScreen = CWindowData::GetInstance()->SettingFile().bFullScreen;
	m_iBrightness = CWindowData::GetInstance()->SettingFile().iBrightness;

	m_iActualIndexRes = 0;
	for( GLint i = 0; i < static_cast< GLint >( m_aWidth.size() ); ++i ) {
		if( m_aWidth[ i ] == CWindowData::GetInstance()->SettingFile().iWidth
			&& m_aHeight[ i ] == CWindowData::GetInstance()->SettingFile().iHeight )
		{
			m_iActualIndexRes = i;
			break;
		}
	}
	m_iSelectedResX = m_aWidth[ m_iActualIndexRes ];
	m_iSelectedResY = m_aHeight[ m_iActualIndexRes ];
}

CROptionsGraphics::~CROptionsGraphics( void )
{
	delete m_pSDLFontTitle;
	delete m_pSDLFontMenu;
	m_aWidth.clear();
	m_aHeight.clear();
	delete [] m_aIndices;
}

GLvoid CROptionsGraphics::SetMouseRegion()
{
	GLint iPercent = 21;
	for( GLint i = 0; i < MAX_REGIONS_OPTIONS_GR; ++i ) {
		if( i == MAX_REGIONS_OPTIONS_GR - 3 || i == MAX_REGIONS_OPTIONS_GR - 2 )
			continue;
		if( i == MAX_REGIONS_OPTIONS_GR - 1 )//dla przycisku "wroc"
			iPercent = 84;
		//ustawienie dla siedmiu regionów:
		//1. Resolutions
		//2. Color depth
		//3. Antialiasing							MAX_REGIONS_OPTIONS_GR - 8
		//4. VSync									MAX_REGIONS_OPTIONS_GR - 7
		//5. Fullscreen								MAX_REGIONS_OPTIONS_GR - 6
		//6. Shadows								MAX_REGIONS_OPTIONS_GR - 5
		//7. 'suwak' - z innymi right i left		MAX_REGIONS_OPTIONS_GR - 4
		//8. [-]									MAX_REGIONS_OPTIONS_GR - 3
		//9. [+]									MAX_REGIONS_OPTIONS_GR - 2
		//10. Wroc									MAX_REGIONS_OPTIONS_GR - 1
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

		if( i == MAX_REGIONS_OPTIONS_GR - 4 ) {
			//suwak, modyfikacja left i right
			SetRegionX( m_rRegions[ i ].left, 25+8 );
			SetRegionX( m_rRegions[ i ].right, 75-8 );

			SetRegionX( m_rRegions_Font[ i ].left, 25+8, GL_TRUE );
			SetRegionX( m_rRegions_Font[ i ].right, 75-8, GL_TRUE );
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//regiony dla przycisku [-], po lewej stronie suwaka
	GLint i = MAX_REGIONS_OPTIONS_GR - 3; //dla [-]
	GLint j = MAX_REGIONS_OPTIONS_GR - 4; //region do rysowania suwaka
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
	i = MAX_REGIONS_OPTIONS_GR - 2; //dla [+]
	SetRegionX( m_rRegions[ i ].left, 75-7 );
	SetRegionX( m_rRegions[ i ].right, 75 );

	SetRegionX( m_rRegions_Font[ i ].left, 75-7, GL_TRUE );
	SetRegionX( m_rRegions_Font[ i ].right, 75, GL_TRUE );

	m_rRegions[ i ].top = m_rRegions[ j ].top;
	m_rRegions_Font[ i ].top = m_rRegions_Font[ j ].top;

	m_rRegions[ i ].bottom = m_rRegions[ j ].bottom;
	m_rRegions_Font[ i ].bottom = m_rRegions_Font[ j ].bottom;
}

GLvoid CROptionsGraphics::RestartObjects()
{
	SetMouseRegion();
}

GLboolean CROptionsGraphics::KeyDown( GLuint uiKey )
{
	switch( uiKey ) {
		case VK_ESCAPE:
			Exit();
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CROptionsGraphics::MouseLButtonDown( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegions, MAX_REGIONS_OPTIONS_GR, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //rozdzielczosc
			++m_iActualIndexRes;
			if( m_iActualIndexRes > m_iMaxIndexRes )
				m_iActualIndexRes = 0;

			m_iSelectedResX = m_aWidth[ m_iActualIndexRes ];
			m_iSelectedResY = m_aHeight[ m_iActualIndexRes ];
			return GL_TRUE;
		case 1: //color depth
			if( CWindowData::GetInstance()->SettingFile().ubBitsPerPixel == 16 )
				CWindowData::GetInstance()->SettingFile().ubBitsPerPixel = 32;
			else
				CWindowData::GetInstance()->SettingFile().ubBitsPerPixel = 16;
			return GL_TRUE;
		case 2: //antialiasing
			CWindowData::GetInstance()->SettingFile().iAntialiasing += 2;
			if( CWindowData::GetInstance()->SettingFile().iAntialiasing > 6 )
				CWindowData::GetInstance()->SettingFile().iAntialiasing = 0;
			return GL_TRUE;
		case 3: //vsync
			CWindowData::GetInstance()->SettingFile().bVSync = !CWindowData::GetInstance()->SettingFile().bVSync;
			m_pGameCtrl->SetVSync(); //dziala bez restartowania okna
			return GL_TRUE;
		case 4: //fullscreen
			m_bSelectedFullScreen = !m_bSelectedFullScreen;
			return GL_TRUE;
		case 5: //shadows
			CWindowData::GetInstance()->SettingFile().bShadows = !CWindowData::GetInstance()->SettingFile().bShadows;
			return GL_TRUE;
		case 6://suwak
			//ustaw na default
			CWindowData::GetInstance()->SettingFile().iBrightness = 0;
			CBrightness::GetInstance()->SetBrightness();
			return GL_TRUE;
		case 7: //[-]
			CWindowData::GetInstance()->SettingFile().iBrightness -= 8;
			if( CWindowData::GetInstance()->SettingFile().iBrightness < -255 )
				CWindowData::GetInstance()->SettingFile().iBrightness = -255;
			CBrightness::GetInstance()->SetBrightness( CWindowData::GetInstance()->SettingFile().iBrightness );
			return GL_TRUE;
		case 8: //[+]
			CWindowData::GetInstance()->SettingFile().iBrightness += 8;
			if( CWindowData::GetInstance()->SettingFile().iBrightness > 255 )
				CWindowData::GetInstance()->SettingFile().iBrightness = 255;
			CBrightness::GetInstance()->SetBrightness( CWindowData::GetInstance()->SettingFile().iBrightness );
			return GL_TRUE;
		case 9: //wroc
			Exit();
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CROptionsGraphics::Exit()
{
	//sprawdz czy restart:
	if( CWindowData::GetInstance()->SettingFile().ubBitsPerPixel != m_ubOldBitsPerPixel )
		m_bNeedRestart = GL_TRUE;

	if( m_iSelectedResX != m_iOldWidth || m_iSelectedResY != m_iOldHeight )
		m_bNeedRestart = GL_TRUE;

	if( m_iOldAntialiasing != CWindowData::GetInstance()->SettingFile().iAntialiasing ) {
		CWindowData::GetInstance()->SetAntialiasingSamples( CWindowData::GetInstance()->SettingFile().iAntialiasing );
		m_bNeedRestart = GL_TRUE;
	}

	if( m_bOldFullScreen != m_bSelectedFullScreen )
		m_bNeedRestart = GL_TRUE;

	if( m_bNeedRestart )
		m_pGameCtrl->GetRMessageBox()->DoMessageBox( LNG_LINE( 80 ), CRMessageBox::EMsgOK, MsgRestartWindow, MsgRestartWindow );
	else {
		CWindowData::GetInstance()->SaveSettingFile();
		m_pGameCtrl->SetEMainGameMode( EOptions );
	}
}

GLvoid MsgRestartWindow( GLvoid ) //odpowiedz na klikniecie na OK w msgboxie przy restartcie okna
{
	CWindowData::GetInstance()->SettingFile().iWidth = CGameControl::GetInstance()->GetROptionsGraphics()->GetSelectedResX();
	CWindowData::GetInstance()->SettingFile().iHeight = CGameControl::GetInstance()->GetROptionsGraphics()->GetSelectedResY();

	//co do bugu numer 12:
	//jezli aktualnie mamy fullscreen i przelaczylismy sie na okno
	//oraz mamy rozdzielczosc gry != rozdzielczosci ekranu to przeba przeprowadziæ
	//podwójny restart
	//1 restart - gdzie ustawiamy rodzielczosc gry na rozdzielczosc ekranu
	//2 restart - gdzie ustawiamy rozdzielczosc gry na ta aktualnie uzywana
	if( CWindowData::GetInstance()->SettingFile().bFullScreen
		&& !CGameControl::GetInstance()->GetROptionsGraphics()->GetSelectedFullScreen()
		&& CWindowData::GetInstance()->SettingFile().iWidth != CWindowData::GetInstance()->GetDesktopWidth()
		&& CWindowData::GetInstance()->SettingFile().iHeight != CWindowData::GetInstance()->GetDesktopHeight() )
	{
		//potrzebny podwójny restart
		CWindowData::GetInstance()->SetDoubleRestart( GL_TRUE );
		//zapamietanie na boku rozdzielczosci prawidlowej, jaka ma bys stosowana
		CWindowData::GetInstance()->SetResDoubleRestartX( CGameControl::GetInstance()->GetROptionsGraphics()->GetSelectedResX() );
		CWindowData::GetInstance()->SetResDoubleRestartY( CGameControl::GetInstance()->GetROptionsGraphics()->GetSelectedResY() );

		//dla pierwszego restartu ustawiamy rozdzielczos na ta z pulpitu i na fullscreen
		CWindowData::GetInstance()->SettingFile().iWidth = CWindowData::GetInstance()->GetDesktopWidth();
		CWindowData::GetInstance()->SettingFile().iHeight = CWindowData::GetInstance()->GetDesktopHeight();
		CWindowData::GetInstance()->SettingFile().bFullScreen = GL_TRUE;

		//dalej patrz plik CFramework - glowna petla programu
	}
	else
		CWindowData::GetInstance()->SettingFile().bFullScreen = CGameControl::GetInstance()->GetROptionsGraphics()->GetSelectedFullScreen();

	CWindowData::GetInstance()->SaveSettingFile();
	CGameControl::GetInstance()->SetEMainGameMode( EOptions );
	CWindowData::GetInstance()->RestartWindow();
}

GLvoid CROptionsGraphics::Draw()
{
	glLoadIdentity();

	glDisable( GL_LIGHTING );

	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), LNG_LINE( 71 ), GL_TRUE );

	DrawButtonts();
}

GLvoid CROptionsGraphics::DrawButtonts()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	//rysujemy przyciski
	DrawRegions( m_rRegions, MAX_REGIONS_OPTIONS_GR );

	//rysujemy suwak
	glColor4ub( 255, 255, 255, 255 );
	GLint iPixelsSeparate = 5;
	GLint i = MAX_REGIONS_OPTIONS_GR - 4; //region suwaka 

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
	GLint iPercent = CWindowData::GetInstance()->SettingFile().iBrightness; //-255 do 255
	iPercent = iPercent + 255; //od 0 do 510
	iPercent = ( 100 * iPercent ) / 510; //od 0 do 100 w %
	GLint iSliderPosition = ( iLengthSlider * iPercent ) / 100;
	iSliderPosition += m_rRegions[ i ].left + iPixelsSeparate;

	glColor4ub( 16, 255, 16, 255 );

	m_aIndices[ 0 ] = CVector2( GLfloat( iSliderPosition + 2 ), GLfloat( m_rRegions[ i ].bottom - iPixelsSeparate ) );
	m_aIndices[ 1 ] = CVector2( GLfloat( iSliderPosition + 2 ), GLfloat( m_rRegions[ i ].top + iPixelsSeparate ) );
	m_aIndices[ 2 ] = CVector2( GLfloat( iSliderPosition - 2 ), GLfloat( m_rRegions[ i ].bottom - iPixelsSeparate ) );
	m_aIndices[ 3 ] = CVector2( GLfloat( iSliderPosition - 2 ), GLfloat( m_rRegions[ i ].top + iPixelsSeparate ) );

	glVertexPointer( 2, GL_FLOAT, 0, m_aIndices );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( i = 0; i < MAX_REGIONS_OPTIONS_GR; ++i ) {
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

		if( i == MAX_REGIONS_OPTIONS_GR - 1 ) { //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
			m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 16 ) );
		}
		else {
			crColor = bPtCursorInRect ? RGB( 50, 255, 50 ) : RGB( 128, 128, 128 );

			switch( i ) {
				case 0://rozdzielczosc
					m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 72 ), m_iSelectedResX, m_iSelectedResY );
					break;
				case 1://glebia kolorow
					m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 73 ), CWindowData::GetInstance()->SettingFile().ubBitsPerPixel );
					break;
				case 2://antyaliasing
					if( CWindowData::GetInstance()->SettingFile().iAntialiasing == 0 )
						m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 75 ) );
					else
						m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 74 ), CWindowData::GetInstance()->SettingFile().iAntialiasing );
					break;
				case 3://vsync
					if( CWindowData::GetInstance()->SettingFile().bVSync )
						m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 76 ), LNG_LINE( 10 ) );
					else
						m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 76 ), LNG_LINE( 11 ) );
					break;
				case 4://fullscreen
					if( m_bSelectedFullScreen )
						m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 77 ), LNG_LINE( 10 ) );
					else
						m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 77 ), LNG_LINE( 11 ) );
					break;
				case 5://Shadows
					if( CWindowData::GetInstance()->SettingFile().bShadows )
						m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 79 ), LNG_LINE( 10 ) );
					else
						m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 79 ), LNG_LINE( 11 ) );
					break;
				case 6: //brightness
				{
					GLint iPercent = CWindowData::GetInstance()->SettingFile().iBrightness; //-255 do 255
					if( iPercent < 0 ) iPercent = -iPercent;
					iPercent = ( 100 * iPercent ) / 255; //od 0 do 100 w %
					if( CWindowData::GetInstance()->SettingFile().iBrightness < 0 )
						iPercent = -iPercent;
					crColor = RGB( 128, 128, 128 );
					m_pSDLFontMenu->DrawTextFormat( m_rRegions_Font[ i ], crColor, LNG_LINE( 78 ), iPercent );
					break;
				}
				case 7://[-]
					m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 85 ) );
					break;
				case 8://[+]
					m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 86 ) );
					break;
			}
		}
	}
}