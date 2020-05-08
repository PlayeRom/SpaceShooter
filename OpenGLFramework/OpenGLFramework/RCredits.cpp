
#include "StdAfx.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "WindowData.h"
#include "RCredits.h"

CRCredits::CRCredits()
{
	m_pGameCtrl = CGameControl::GetInstance();
	m_pSDLFontTitle = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 50 );
	m_pSDLFontMenu = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 23 );

	SetMouseRegion();

	m_iTextPosY_27 = static_cast< GLint >( 7.68f * 27.0f );
	m_iTextPosY_13 = static_cast< GLint >( 7.68f * 13.0f );
	SetStartVariables();
}

CRCredits::~CRCredits()
{
	delete m_pSDLFontTitle;
	delete m_pSDLFontMenu;
}

GLvoid CRCredits::SetMouseRegion()
{
	GLint iPercent = 84;
	for( GLint i = 0; i < MAX_REGIONS_CREDITS; ++i ) {
		iPercent = ( i == 1 ) ? 84 : 21;

		SetRegionX( m_rRegions[ i ].left, 25 );
		SetRegionX( m_rRegions[ i ].right, 75 );

		SetRegionX( m_rRegions_Font[ i ].left, 25, GL_TRUE );
		SetRegionX( m_rRegions_Font[ i ].right, 75, GL_TRUE );

		SetRegionY( m_rRegions[ i ].top, iPercent );
		SetRegionY( m_rRegions_Font[ i ].top, iPercent, GL_TRUE );
		
		iPercent += ( i == 1 ) ? 7 : 61;

		SetRegionY( m_rRegions[ i ].bottom, iPercent );
		SetRegionY( m_rRegions_Font[ i ].bottom, iPercent, GL_TRUE );
		iPercent += 2;
	}
}

GLvoid CRCredits::RestartObjects()
{
	SetMouseRegion();
}

GLboolean CRCredits::KeyDown( GLuint uiKey )
{
	switch( uiKey ) {
		case VK_ESCAPE:
			m_pGameCtrl->SetEMainGameMode( EMainMenu );
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CRCredits::MouseLButtonDown( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, &m_rRegions[ 1 ], MAX_REGIONS_CREDITS - 1, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //exit
			m_pGameCtrl->SetEMainGameMode( EMainMenu );
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CRCredits::SetStartVariables()
{
	m_iPosYAnim = -static_cast< GLint >( 7.68f * 60.0f );
}

GLvoid CRCredits::Draw()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	m_pSDLFontTitle->DrawText( 225, m_iTextPosY_13, RGB( 100, 100, 255 ), LNG_LINE( 33 ), GL_TRUE );

	//rysujemy przyciski
	DrawRegions( m_rRegions, 2 );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	GLint i = 1;
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

	crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );

	m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 16 ) );

	glScissor( m_rRegions[ 0 ].left, CWindowData::GetInstance()->SettingFile().iHeight - m_rRegions[ 0 ].bottom + 5,
			   m_rRegions[ 0 ].right - m_rRegions[ 0 ].left,
			   m_rRegions[ 0 ].bottom - m_rRegions[ 0 ].top - 10 );	// Define Scissor Region
	glEnable( GL_SCISSOR_TEST );

	GLint iLanIndex = CWindowData::GetInstance()->SettingFile().iIndexLanguage;
	if( iLanIndex < 0 || iLanIndex > 1 )
		iLanIndex = 0; // domyœlnie angielski
	const TCHAR tcHeaders[ 4 ][ 2 ][ 32 ] = { { _T("Programmer & textures"), _T("Programowanie i tekstury") },
											  { _T("3D models & textures"), _T("Modele 3D i tekstury") },
											  { _T("Concept art"), _T("Concept art") },
											  { _T("Music & sounds"), _T("Muzyka i dŸwiêki") } };

	GLint iPosY = m_iTextPosY_27 - m_iPosYAnim;
	m_pSDLFontMenu->DrawText( 100, iPosY, RGB( 50, 255, 50 ), tcHeaders[ 0 ][ iLanIndex ], GL_TRUE );
	iPosY += 30;
	m_pSDLFontMenu->DrawText( 150, iPosY, RGB( 255, 255, 255 ), _T("Roman \"PlayeRom\" Ludwicki"), GL_TRUE );
	iPosY += 60;
	m_pSDLFontMenu->DrawText( 100, iPosY, RGB( 50, 255, 50 ), tcHeaders[ 1 ][ iLanIndex ], GL_TRUE );
	iPosY += 30;
	m_pSDLFontMenu->DrawText( 150, iPosY, RGB( 255, 255, 255 ), _T("Marcin \"Chrz¹szczyk\" ¯uk"), GL_TRUE );
	iPosY += 60;
	m_pSDLFontMenu->DrawText( 100, iPosY, RGB( 50, 255, 50 ), tcHeaders[ 2 ][ iLanIndex ], GL_TRUE );
	iPosY += 30;
	m_pSDLFontMenu->DrawText( 150, iPosY, RGB( 255, 255, 255 ), _T("Tomasz \"Unimatrix\" Zdeb"), GL_TRUE );
	iPosY += 60;
	m_pSDLFontMenu->DrawText( 100, iPosY, RGB( 50, 255, 50 ), tcHeaders[ 3 ][ iLanIndex ], GL_TRUE );
	iPosY += 30;
	m_pSDLFontMenu->DrawText( 150, iPosY, RGB( 255, 255, 255 ), _T("Micha³ \"Mimper\" Witek"), GL_TRUE );

	glDisable( GL_SCISSOR_TEST );

	if( m_iPosYAnim < 0 ) {
		GLint iInterval = GLint( 100.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( iInterval <= 0 )
			iInterval = 1;
		m_iPosYAnim += iInterval;
		if( m_iPosYAnim > 0 )
			m_iPosYAnim = 0;
	}
}