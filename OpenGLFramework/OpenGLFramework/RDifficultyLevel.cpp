
#include "StdAfx.h"
#include "WindowData.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "Cursor.h"
#include "RShipsBrowse.h"
#include "RDifficultyLevel.h"

CRDifficultyLevel::CRDifficultyLevel()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_pSDLFontTitle = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 50 );
	m_pSDLFontMenu = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 23 );

	SetMouseRegion();

	SetDifficultyLevel( ELevelNone );
}

CRDifficultyLevel::~CRDifficultyLevel()
{
	delete m_pSDLFontTitle;
	delete m_pSDLFontMenu;
}

GLvoid CRDifficultyLevel::SetMouseRegion()
{
	GLint iPercent = 30;
	for( GLint i = 0; i < MAX_REGIONS_DIFF_LEVEL; ++i ) {
		if( i == MAX_REGIONS_DIFF_LEVEL - 1 ) {
			//Wróc - na samym dole
			iPercent = 84;
		}
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

GLvoid CRDifficultyLevel::RestartObjects()
{
	SetMouseRegion();
}

GLboolean CRDifficultyLevel::KeyDown( GLuint uiKey )
{
	switch( uiKey ) {
		case VK_ESCAPE:
			m_pGameCtrl->SetEMainGameMode( EMainMenu );
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CRDifficultyLevel::MouseLButtonDown( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegions, MAX_REGIONS_DIFF_LEVEL, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //easy
			SetDifficultyLevel( ELevelEasy );
			m_pGameCtrl->GetRShipsBrowse()->SetStartVariables();
			m_pGameCtrl->GetRShipsBrowse()->SetMode( EBrowsePlayerShips );
			m_pGameCtrl->SetEMainGameMode( EShipsBrowser );
			return GL_TRUE;
		case 1: //normal
			SetDifficultyLevel( ELevelNormal );
			m_pGameCtrl->GetRShipsBrowse()->SetStartVariables();
			m_pGameCtrl->GetRShipsBrowse()->SetMode( EBrowsePlayerShips );
			m_pGameCtrl->SetEMainGameMode( EShipsBrowser );
			return GL_TRUE;
		case 2: //hard
			SetDifficultyLevel( ELevelHard );
			m_pGameCtrl->GetRShipsBrowse()->SetStartVariables();
			m_pGameCtrl->GetRShipsBrowse()->SetMode( EBrowsePlayerShips );
			m_pGameCtrl->SetEMainGameMode( EShipsBrowser );
			return GL_TRUE;
		case 3: //back
			SetDifficultyLevel( ELevelNone );
			m_pGameCtrl->SetEMainGameMode( EMainMenu );
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CRDifficultyLevel::Draw()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), LNG_LINE( 12 ), GL_TRUE );

	//rysujemy przyciski
	DrawRegions( m_rRegions, MAX_REGIONS_DIFF_LEVEL );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 0; i < MAX_REGIONS_DIFF_LEVEL; ++i ) {
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

		if( i == MAX_REGIONS_DIFF_LEVEL - 1 ) //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
		else 
			crColor = bPtCursorInRect ? RGB( 50, 255, 50 ) : RGB( 128, 128, 128 );
		
		m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( i + 13 ) );
	}
}