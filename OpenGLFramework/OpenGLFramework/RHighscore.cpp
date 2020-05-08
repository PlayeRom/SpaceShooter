
#include "StdAfx.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "WindowData.h"
#include "RHighscore.h"

SItem g_sHighscoreItem[ 10 ];

GLint QSortCompareHighscoreItem( const GLvoid *arg1, const GLvoid *arg2 );
GLvoid MsgCleanUpYes( GLvoid );

CRHighscore::CRHighscore()
{
	m_pGameCtrl = CGameControl::GetInstance();
	m_pSDLFontTitle = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 50 );
	m_pSDLFontMenu = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 23 );
	m_pSDLFontItems = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 2 ), 20 );

	SetMouseRegion();
}

CRHighscore::~CRHighscore()
{
	delete m_pSDLFontTitle;
	delete m_pSDLFontMenu;
	delete m_pSDLFontItems;
}

GLvoid CRHighscore::SetMouseRegion()
{
	SetRegionX( m_rRegions[ 0 ].left, 25 );
	SetRegionX( m_rRegions[ 0 ].right, 75 );
	SetRegionY( m_rRegions[ 0 ].top, 23 );
	SetRegionY( m_rRegions[ 0 ].bottom, 82 );

	GLint iPercent = 14;
	for( GLint i = 1; i < MAX_REGIONS_HIGHSCORE; ++i ) {
		if( i == MAX_REGIONS_HIGHSCORE - 1 ) //dla wyjacia
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

	GLfloat fPercent = 27.5f;
	for( GLint i = 0; i < MAX_ITEMS_HIGHSCORE + 1; ++i ) {
		SetRegionX( m_pRegions_Name[ i ].x, 26, GL_TRUE );
		SetRegionY( m_pRegions_Name[ i ].y, fPercent, GL_TRUE );

		fPercent += 5.25f;
	}
}

GLvoid CRHighscore::RestartObjects()
{
	SetMouseRegion();
}

GLboolean CRHighscore::KeyDown( GLuint uiKey )
{
	switch( uiKey ) {
		case VK_ESCAPE:
			m_pGameCtrl->SetEMainGameMode( EMainMenu );
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CRHighscore::MouseLButtonDown( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, &m_rRegions[ 1 ], MAX_REGIONS_HIGHSCORE - 1, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //wyczysc
			m_pGameCtrl->GetRMessageBox()->DoMessageBox( LNG_LINE( 124 ), CRMessageBox::EMsgYesNo, MsgCleanUpYes, NULL );
			return GL_TRUE;
		case 1: //wroc
			m_pGameCtrl->SetEMainGameMode( EMainMenu );
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid MsgCleanUpYes( GLvoid )
{
	CWindowData::GetInstance()->CleanUpHighscore();
	CWindowData::GetInstance()->SaveSettingFile();
}

GLvoid CRHighscore::Draw()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	//tytul: Najlepsze wyniki
	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), LNG_LINE( 3 ), GL_TRUE );

	//rysujemy plansze z items-ami i przyciski
	DrawRegions( m_rRegions, MAX_REGIONS_HIGHSCORE );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 1; i < MAX_REGIONS_HIGHSCORE; ++i ) {
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

		if( i == MAX_REGIONS_HIGHSCORE - 1 ) { //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
			m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 16 ) );
		}
		else {
			crColor = bPtCursorInRect ? RGB( 50, 255, 50 ) : RGB( 128, 128, 128 );
			m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( 123 ) );
		}
	}

	//rysujemy pozycje - item

	for( GLint i = 0; i < MAX_ITEMS_HIGHSCORE + 1; ++i ) {
		if( i == 0 ) { //napisy "Player" "Level" "Score"
			m_pSDLFontItems->DrawTextFormat( m_pRegions_Name[ i ].x, m_pRegions_Name[ i ].y, GL_FALSE, RGB( 255, 255, 255 ), _T("    %-17s  %-7s %s"), LNG_LINE( 120 ), LNG_LINE( 121 ), LNG_LINE( 122 ) );
			continue;
		}
		else if( i == 1 ) //pierwsze miejsce
			crColor = RGB( 10, 255, 10 );
		else
			crColor = ( i % 2 == 0 ) ? RGB( 0, 128, 128 ) : RGB( 0, 0, 128 );
		
		if( i > 0 )
			m_pSDLFontItems->DrawTextFormat( m_pRegions_Name[ i ].x, m_pRegions_Name[ i ].y, GL_FALSE, crColor,
											 _T("%2d. %-17s  %2d      %d"),
											 i,
											 g_sHighscoreItem[ i - 1 ].cNickname,
											 g_sHighscoreItem[ i - 1 ].iLevel,
											 g_sHighscoreItem[ i - 1 ].uiScore );
	}
}

GLint QSortCompareHighscoreItem( const GLvoid *arg1, const GLvoid *arg2 )
{
	SItem *s1 = ( SItem* )arg1;
	SItem *s2 = ( SItem* )arg2;
	if( s1->uiScore < s2->uiScore ) return 1;
	if( s1->uiScore > s2->uiScore ) return -1;
	return 0;
}

GLvoid CRHighscore::SortItems()
{
	qsort( &g_sHighscoreItem[ 0 ], MAX_ITEMS_HIGHSCORE, sizeof( SItem ), QSortCompareHighscoreItem );
}