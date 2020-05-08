
#include "StdAfx.h"
#include "WindowData.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "../Draw/3DObjManager.h"
#include "../Draw/VertexArrays.h"
#include "../Draw/Fog.h"
#include "Cursor.h"
#include "ShipsManager.h"
#include "RGame.h"
#include "RMultiplayerMenu.h"
#include "RShipsBrowse.h"

const GLfloat MAX_DISTANCE_AUTO_ZOOM = -1000.0f;
const GLfloat DISTANCE_ZOOM	= -70.0f;

CRShipsBrowse::CRShipsBrowse()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_pFog = new CFog();

	m_pSDLFontTitle = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 50 );
	m_pSDLFontMenu = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 23 );
	m_pSDLFontParam = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 1 ), 16 );

	SetMouseRegion();

	m_fRotX = 20.0f;
	m_fRotY = 0.0f;
	m_fRotZ = 0.0f;
	m_sAutoZoom = 1;
	m_fConstZoom = DISTANCE_ZOOM; //domyslna pozycja statku po Z
	m_fZoom = MAX_DISTANCE_AUTO_ZOOM;
	m_bHoldMouse = GL_FALSE;

	SetMode( EBrowsePlayerShips );

	m_iActualPrintDescLine = 0;
	m_iActualPrintCharInLine = 0;

	m_iTextPosYAvailable = static_cast< GLint >( 7.68f * 50.0f );
	m_iTextPosYTitle = static_cast< GLint >( 7.68f * 13.0f );
}

CRShipsBrowse::~CRShipsBrowse()
{
	delete m_pSDLFontTitle;
	delete m_pSDLFontMenu;
	delete m_pSDLFontParam;
	delete m_pFog;
}

GLvoid CRShipsBrowse::SetFog()
{
	m_pFog->SetColor( 0.0f, 0.0f, 0.0f, 1.0f );
	m_pFog->SetFogMode( GL_EXP );
	m_pFog->SetStartEnd( 800.0f, 1200.0f );
	m_pFog->SetDensity( 0.0015f );
}

GLvoid CRShipsBrowse::SetFogDensity( GLboolean bShipAvailable )
{
	if( bShipAvailable )
		m_pFog->SetDensity( 0.0015f );
	else //jezeli statek niedostepny:
		m_pFog->SetDensity( 1.0f );
}

GLvoid CRShipsBrowse::SetMouseRegion()
{
	GLfloat fPercent = 1.0f;
	for( GLint i = 0; i < MAX_REGIONS_SHIP_BROWSE - 1; ++i ) {
		SetRegionX( m_rRegions[ i ].left, fPercent );
		SetRegionX( m_rRegions_Font[ i ].left, fPercent, GL_TRUE );
		fPercent += 23.75f; //dlugosc przycisku
		SetRegionX( m_rRegions[ i ].right, fPercent );
		SetRegionX( m_rRegions_Font[ i ].right, fPercent, GL_TRUE );
		fPercent += 1.0f; //odstep

		SetRegionY( m_rRegions[ i ].top, 92 );
		SetRegionY( m_rRegions[ i ].bottom, 99 );

		SetRegionY( m_rRegions_Font[ i ].top, 92, GL_TRUE );
		SetRegionY( m_rRegions_Font[ i ].bottom, 99, GL_TRUE );
	}

	//region "Opis"
	GLint i = MAX_REGIONS_SHIP_BROWSE - 1;
	SetRegionX( m_rRegions[ i ].left, 75.25f );
	SetRegionX( m_rRegions_Font[ i ].left, 75.25f, GL_TRUE );
	SetRegionX( m_rRegions[ i ].right, 99.0f );
	SetRegionX( m_rRegions_Font[ i ].right, 99.0f, GL_TRUE );
	SetRegionY( m_rRegions[ i ].top, 15 );
	SetRegionY( m_rRegions[ i ].bottom, 22 );
	SetRegionY( m_rRegions_Font[ i ].top, 15, GL_TRUE );
	SetRegionY( m_rRegions_Font[ i ].bottom, 22, GL_TRUE );

	ResetDescVariable();

	//ustawienie regionow dla textu opisujacego statek
	fPercent = 26.0f;
	for( GLint i = 0; i < MAX_REGIONS_DESCRIPTION; ++i ) {
		SetRegionY( m_rRegionsDesc_Font[ i ].y, fPercent, GL_TRUE );
		fPercent += 3.5f;

		SetRegionX( m_rRegionsDesc_Font[ i ].x, 42, GL_TRUE );
	}

	//ustawienie regionow gwiazdek
	//[ 0 ][ 1 ][ 2 ]
	//[ 3 ][ 4 ][ 5 ]
	GLfloat fResolutionRelation = GLfloat( CWindowData::GetInstance()->SettingFile().iWidth ) / GLfloat( CWindowData::GetInstance()->SettingFile().iHeight );
	GLfloat fSizeA = 1.5f; //dlugosc boku
	GLfloat fSizeB = fSizeA * fResolutionRelation; //wysokosc boku
	GLfloat fPercentLeft = 13.0f;
	GLfloat fPercentTop = 20.0f;
	for( GLint i = 0; i < 12; ++i ) {
		SetRegionX( m_rRegionsStars[ i ].left, fPercentLeft );
		SetRegionX( m_rRegionsStars[ i ].right, fPercentLeft + fSizeA );

		SetRegionX( m_rRegionsStarsFont[ i ].left, fPercentLeft, GL_TRUE );
		SetRegionX( m_rRegionsStarsFont[ i ].right, fPercentLeft + fSizeA, GL_TRUE );
		fPercentLeft += fSizeA;// + 0.5f;
		
		SetRegionY( m_rRegionsStars[ i ].top, fPercentTop );
		SetRegionY( m_rRegionsStars[ i ].bottom, fPercentTop + fSizeB );

		SetRegionY( m_rRegionsStarsFont[ i ].top, fPercentTop, GL_TRUE );
		SetRegionY( m_rRegionsStarsFont[ i ].bottom, fPercentTop + fSizeB, GL_TRUE );

		if( i == 2 || i == 5 || i == 8 ) {
			fPercentLeft = 13.0f;
			fPercentTop += ( fSizeB + ( 0.4f * fResolutionRelation ) );
		}
	}
}

GLvoid CRShipsBrowse::ResetDescVariable()
{
	m_rRegionDescription = m_rRegions[ MAX_REGIONS_SHIP_BROWSE - 1 ];
	m_iDescRollActual = 0;
	m_bDescRoll = GL_FALSE;
}

GLvoid CRShipsBrowse::RestartObjects()
{
	ResetVariables();
	SetMouseRegion();
	SetFog();
}

GLvoid CRShipsBrowse::ResetVariables()
{
	m_fRotX = 20.0f;
	m_fRotZ = 0.0f;
	if( m_sAutoZoom == 0 )
		m_sAutoZoom = -1;
}

GLvoid CRShipsBrowse::SetStartVariables()
{
	m_fRotX = 20.0f;
	m_fRotZ = 0.0f;
	m_sAutoZoom = 1;
	m_fZoom = MAX_DISTANCE_AUTO_ZOOM;
	m_iShipsManagerIndex = -1;
	m_iNextOrPrev = 0;
	SetFog();
	ResetDescVariable();
	m_bPrintDescForThisShip = GL_TRUE;
}

GLvoid CRShipsBrowse::SetEndVariables()
{
}

GLvoid CRShipsBrowse::SetMode( EModeShipBrowse eMode )
{
	m_eActualMode = eMode;

	//znajdz pierwszy statek
	m_iShipsManagerIndex = GetNextIndexShip( -1 );
	SetShipInformation();
}

GLint CRShipsBrowse::GetNextIndexShip( GLint iIndex )
{
	GLboolean bPlayerShip = GL_TRUE;
	if( m_eActualMode == EBrowseEnemyShips )
		bPlayerShip = GL_FALSE;

	return m_pGameCtrl->GetShipsManager()->GetNextIndex( iIndex, bPlayerShip );
}

GLint CRShipsBrowse::GetPreviousIndexShip( GLint iIndex )
{
	GLboolean bPlayerShip = GL_TRUE;
	if( m_eActualMode == EBrowseEnemyShips )
		bPlayerShip = GL_FALSE;

	return m_pGameCtrl->GetShipsManager()->GetPreviousIndex( iIndex, bPlayerShip );
}

GLboolean CRShipsBrowse::KeyDown( GLuint uiKey )
{
	switch( uiKey ) {
		case VK_ESCAPE:
			m_aShipSchemeArray.clear();
			Back();
			return GL_TRUE;

		case VK_RETURN:
			m_aShipSchemeArray.clear();
			RunGame( m_iShipsManagerIndex );
			return GL_TRUE;

		case VK_LEFT:
			m_aShipSchemeArray.clear();
			SetPreviousShip();
			return GL_TRUE;

		case VK_RIGHT:
			m_aShipSchemeArray.clear();
			SetNextShip();
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CRShipsBrowse::MouseLButtonDown( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegions, MAX_REGIONS_SHIP_BROWSE, i ) ) {
		m_ptCursorPos.x = m_ptCursorOldPos.x = iX;
		m_ptCursorPos.y = m_ptCursorOldPos.y = iY;
		m_bHoldMouse = GL_TRUE;
		if( !CWindowData::GetInstance()->SettingFile().bFullScreen )
			::ClientToScreen( CWindowData::GetInstance()->GetHWND(), &m_ptCursorPos );
		m_pGameCtrl->GetCursor()->SetMyCursor( CCursor::ERotateObj );
		return GL_TRUE;
	}

	switch( i ) {
		case 0: //previous
			m_aShipSchemeArray.clear();
			ResetDescVariable();
			SetPreviousShip();
			return GL_TRUE;

		case 1: //next
			m_aShipSchemeArray.clear();
			ResetDescVariable();
			SetNextShip();
			return GL_TRUE;

		case 2: //play
			m_aShipSchemeArray.clear();
			RunGame( m_iShipsManagerIndex );
			return GL_TRUE;

		case 3: //back
			m_aShipSchemeArray.clear();
			Back();
			return GL_TRUE;

		case 4: //description
			if( m_pGameCtrl->GetShipsManager()->IsAvailableInBrowse( m_iShipsManagerIndex ) )
				m_bDescRoll = !m_bDescRoll;
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CRShipsBrowse::SetPreviousShip()
{
	ResetVariables();
	m_iNextOrPrev = -1;
	m_bPrintDescForThisShip = GL_TRUE;
}

GLvoid CRShipsBrowse::SetNextShip()
{
	ResetVariables();
	m_iNextOrPrev = 1;
	m_bPrintDescForThisShip = GL_TRUE;
}

GLvoid CRShipsBrowse::Back()
{
	SetEndVariables();
	switch( m_eActualMode ) {
		case EBrowseEnemyShips:
			m_pGameCtrl->SetEMainGameMode( EMainMenu );
			break;

		case EBrowsePlayerShips:
			m_pGameCtrl->SetEMainGameMode( EDifficultyLevel );
			break;

#if _USE_MULTIPLYER_
		case EBrowseMultiplayer:
			m_pGameCtrl->SetEMainGameMode( EMultiplayerMenu );
			break;
#endif
	}		
}

GLvoid CRShipsBrowse::RunGame( GLint iIndexShip, GLboolean bCheckCorrectness /*= GL_TRUE*/ )
{
	if( bCheckCorrectness ) {
		if( m_eActualMode == EBrowseEnemyShips || !m_pGameCtrl->GetShipsManager()->IsAvailableInBrowse( iIndexShip ) )
			return;
	}

	SetEndVariables();

	switch( m_eActualMode ) {
		case EBrowsePlayerShips:
			//POINT ptCursorPos;
			m_ptCursorPos.x = CWindowData::GetInstance()->SettingFile().iWidth / 2;
			m_ptCursorPos.y = CWindowData::GetInstance()->SettingFile().iHeight / 2;
			::SetCursorPos( m_ptCursorPos.x, m_ptCursorPos.y );
			m_pGameCtrl->GetCursor()->SetMyCursor( CCursor::EGame );

			m_pGameCtrl->GetRGame()->SetStartPlayerVariables( iIndexShip );
			m_pGameCtrl->SetEMainGameMode( ESingleGame );
			break;
#if _USE_MULTIPLYER_
		case EBrowseMultiplayer:
			m_pGameCtrl->GetRMultiplayerMenu()->SelectShipDone( iIndexShip );
			m_pGameCtrl->SetEMainGameMode( EMultiplayerMenu );
			break;
#endif
	}

	m_pGameCtrl->StartPlayMusicForGame();
}

GLboolean CRShipsBrowse::MouseLButtonUp( GLint /*iX*/, GLint /*iY*/ )
{
	m_bHoldMouse = GL_FALSE;
	m_pGameCtrl->GetCursor()->SetMyCursor( CCursor::EMenu );
	return GL_TRUE;
}

GLboolean CRShipsBrowse::MouseWheel( GLint iScrollLines )
{
	m_fZoom += ( static_cast< GLfloat >( iScrollLines ) * 5.0f );
	if( m_fZoom > -30.0f ) m_fZoom = -30.0f;
	if( m_fZoom < -200.0f ) m_fZoom = -200.0f;
	return GL_TRUE;
}

GLvoid CRShipsBrowse::CalcRotateShip()
{
	if( m_bHoldMouse ) {
		m_ptCursorOldPos.x = m_ptCursorPos.x;
		m_ptCursorOldPos.y = m_ptCursorPos.y;
		::GetCursorPos( &m_ptCursorPos );

		m_fRotY -= ( static_cast< GLfloat >( m_ptCursorOldPos.x - m_ptCursorPos.x ) * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() * 50.0f );
		m_fRotX -= ( static_cast< GLfloat >( m_ptCursorOldPos.y - m_ptCursorPos.y ) * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() * 50.0f );
	}
	else {
		m_fRotY += ( 10.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( m_fRotY > 360.0f )
			m_fRotY -= 360.0f;
	}
}

GLvoid CRShipsBrowse::Draw()
{
	CalcRotateShip();
	DrawShip();
	m_pGameCtrl->DrawSkyBox_List();
	/////////////////////////////////////////////////////////////////////////
	glLoadIdentity();
	glDisable( GL_LIGHT2 );
	glEnable( GL_LIGHT0 );
	glDisable( GL_LIGHTING );

	// wypisz teksty
	if( !m_pGameCtrl->GetShipsManager()->IsAvailableInBrowse( m_iShipsManagerIndex ) )
		m_pSDLFontMenu->DrawText( 0, m_iTextPosYAvailable, RGB( 255, 10, 10 ), LNG_LINE( 32 ), GL_TRUE );

	switch( m_eActualMode ) {
		case EBrowseEnemyShips:
			m_pSDLFontTitle->DrawText( 0, m_iTextPosYTitle, RGB( 100, 100, 255 ), LNG_LINE( 31 ), GL_TRUE );
			break;

		default:
			m_pSDLFontTitle->DrawText( 0, m_iTextPosYTitle, RGB( 100, 100, 255 ), LNG_LINE( 18 ), GL_TRUE );
			break;
	}

	if( m_sAutoZoom == 0 )
		DrawStatistics();

	DrawAll2D();
}

GLvoid CRShipsBrowse::DrawSchemeTop()
{
	glPushMatrix();

	ScaleElementsScheme();

	glRotatef( 90.0f, 0.0f, 1.0f, 0.0f );
	glRotatef( 180.0f, 1.0f, 0.0f, 0.0f );
	glRotatef( -90.0f, 0.0f, 0.0f, 1.0f );
	
	DrawElementsScheme();

	glPopMatrix();
}

GLvoid CRShipsBrowse::DrawSchemeFront()
{
	glPushMatrix();

	glTranslatef( static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iWidth ) / 2.25f,
				  static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iHeight ) / 2.75f,
				  0.0f );

	ScaleElementsScheme();

	glRotatef( 360.0f, 0.0f, 1.0f, 0.0f );
	glRotatef( 180.0f, 1.0f, 0.0f, 0.0f );
	
	DrawElementsScheme();

	glPopMatrix();
}

GLvoid CRShipsBrowse::DrawSchemeSide()
{
	glPushMatrix();

	glTranslatef( 0.0f,
				  static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iHeight ) / 2.75f,
				  0.0f );

	ScaleElementsScheme();

	glRotatef( 90.0f, 0.0f, 1.0f, 0.0f );
	glRotatef( 180.0f, 1.0f, 0.0f, 0.0f );

	DrawElementsScheme();

	glPopMatrix();
}

GLvoid CRShipsBrowse::DrawElementsScheme()
{
	CVertexArrays::GetInstance()->EnableArraysElem();
	glDrawElements( GL_TRIANGLES, m_aShipSchemeArray.size(), GL_UNSIGNED_SHORT, &m_aShipSchemeArray[ 0 ] );
	CVertexArrays::GetInstance()->DisableClientState();
}

GLvoid CRShipsBrowse::ScaleElementsScheme()
{
	GLfloat fScale = 8.0f;
	if( m_iShipsManagerIndex == INDEX_BIG_SHIP_BOSS_01 )
		fScale = 4.0f;
	//skalowanie wzgledem rozdzielczosci, baza 1024x768
	if( CWindowData::GetInstance()->SettingFile().iWidth < 1024 && CWindowData::GetInstance()->SettingFile().iHeight < 768 ) {
		//jezeli rozdzielczosc mniejsza od 1024x768 to zmniejsz
		glScalef( fScale / ( 1024.0f / static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iWidth ) ),
				  fScale / ( 768.0f  / static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iHeight ) ),
				  0.0f );
	}
	else if( CWindowData::GetInstance()->SettingFile().iWidth > 1024 && CWindowData::GetInstance()->SettingFile().iHeight > 768 ) {
		//jezeli rozdzielczosc wieksza od 1024x768 to zwieksz
		glScalef( ( static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iWidth ) / 1024.0f ) * fScale,
				  ( static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iHeight ) / 768.0f ) * fScale,
				  0.0f );
	}
	else //dokladnie 1024x768
		glScalef( fScale, fScale, 0.0f );
}

GLvoid CRShipsBrowse::DrawShipScheme()
{
	if( m_sAutoZoom != 0 || !m_pGameCtrl->GetShipsManager()->IsAvailableInBrowse( m_iShipsManagerIndex ) )
		return;

	if( m_aShipSchemeArray.size() == 0 ) {
		GLint iSize = CVertexArrays::GetInstance()->GetSubIndicesSize( m_iShipsManagerIndex );
		for( GLint i = 0; i < iSize; ++i )
			m_aShipSchemeArray.push_back( static_cast< GLushort >( CVertexArrays::GetInstance()->GetIndexFromIndices( m_iShipsManagerIndex, i ) ) );
	}

	glColor3f( 0.75f, 0.75f, 0.75f );
	m_pGameCtrl->Enable2D();
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glLoadIdentity();
	glTranslatef( static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iWidth ) / 3.25f,
				  static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iHeight ) / 2.75f,
				  0.0f );

	DrawSchemeTop();
	DrawSchemeSide();
	DrawSchemeFront();

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	m_pGameCtrl->Disable2D();
}

GLvoid CRShipsBrowse::DrawShip()
{
	//rysujemy statek
	if( m_sAutoZoom == 1 ) {
		m_fZoom += ( 2000.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( m_fZoom > m_fConstZoom ) {
			m_fZoom = m_fConstZoom;
			m_sAutoZoom = 0;
		}
	}
	if( m_sAutoZoom == -1 ) {
		m_fZoom -= ( 2000.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( m_fZoom < MAX_DISTANCE_AUTO_ZOOM ) {
			m_fZoom = MAX_DISTANCE_AUTO_ZOOM;
			m_sAutoZoom = 1;
			//tu zmiana indeksu obiektu do rysowania
			if( m_iNextOrPrev > 0 ) //next
				m_iShipsManagerIndex = GetNextIndexShip( m_iShipsManagerIndex );
			else if( m_iNextOrPrev < 0 ) //previous
				m_iShipsManagerIndex = GetPreviousIndexShip( m_iShipsManagerIndex );

			SetShipInformation();
		}
	}

	if( m_rRegionDescription.left < m_rRegions[ MAX_REGIONS_SHIP_BROWSE - 1 ].left )
		DrawShipScheme();

	m_pGameCtrl->Disable2D();
	glEnable( GL_DEPTH_TEST );
	glColor3f( 1.0f, 1.0f, 1.0f );
	glEnable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
	glEnable( GL_LIGHT2 );
	glDisable( GL_BLEND );

	CVector4 cLightPosShadow;
	if( CWindowData::GetInstance()->SettingFile().bShadows ) {
		// przeksztalcenia odwrotne
		glLoadIdentity();
		glRotatef( -m_fRotZ, 0.0f, 0.0f, 1.0f );
		glRotatef( -m_fRotY, 0.0f, 1.0f, 0.0f );
		glRotatef( -m_fRotX, 1.0f, 0.0f, 0.0f );
		glTranslatef( 0.0f, 0.0f, -m_fZoom );

		cLightPosShadow = m_pGameCtrl->GetStencilShadow()->GetLightPos( m_pGameCtrl->GetLighting()->GetPosition( GL_LIGHT2 ) );
	}
	// przesuwamy obiekt juz normalnie
	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, m_fZoom );
	glRotatef( m_fRotX, 1.0f, 0.0f, 0.0f );
	glRotatef( m_fRotY, 0.0f, 1.0f, 0.0f );
	glRotatef( m_fRotZ, 0.0f, 0.0f, 1.0f );

	glEnable( GL_FOG );
	SetFogDensity( m_pGameCtrl->GetShipsManager()->IsAvailableInBrowse( m_iShipsManagerIndex ) );
	m_pGameCtrl->Get3DObjManager()->Draw3DObject_Lists( m_iShipsManagerIndex, GL_TRUE, GL_LIGHT1 );
	if( CWindowData::GetInstance()->SettingFile().bShadows )
		m_pGameCtrl->GetStencilShadow()->DrawShadowForObj( m_iShipsManagerIndex, cLightPosShadow );
	glDisable( GL_FOG );
}

GLvoid CRShipsBrowse::DrawStatistics()
{
	//rysuj statystyki
	GLint iPosY;// = 140;
	iPosY = ( m_rRegionsStarsFont[ 0 ].bottom - m_rRegionsStarsFont[ 3 ].bottom ) * 2;
	iPosY = m_rRegionsStarsFont[ 0 ].bottom + iPosY + 6;
	m_pSDLFontParam->DrawTextFormat( 10, iPosY, GL_FALSE, RGB( 255, 255, 255 ), LNG_LINE( 25 ), lpNameShip );
	iPosY = m_rRegionsStarsFont[ 0 ].bottom - m_rRegionsStarsFont[ 3 ].bottom;
	iPosY = m_rRegionsStarsFont[ 0 ].bottom + iPosY + 6;
	m_pSDLFontParam->DrawText( 10, iPosY, RGB( 255, 255, 255 ), LNG_LINE( 26 ) );

	for( GLint j = 0, iJMulti3 = 0; j < 4; ++j, iJMulti3 += 3 ) { // j = petla po 4 parametrach statku, iJMulti3 = j*3
		m_pSDLFontParam->DrawText( 30, m_rRegionsStarsFont[ iJMulti3 ].bottom + 6, RGB( 255, 255, 255 ), LNG_LINE( 27 + j ) );
		GLboolean bFill = GL_FALSE;
		for( GLint i = 2 + iJMulti3; i >= iJMulti3; --i ) {
			if( m_iStarsParameters[ j ] - 1 == i - iJMulti3 )
				bFill = GL_TRUE;
			DrawStar( i, bFill );
		}
	}
}

GLvoid CRShipsBrowse::DrawAll2D()
{
	///////////////////////////////////////////////////////////////////////////
	//rysujemy przyciski
	////////////////////////////////////////////////////////////////////////////
	m_pGameCtrl->Enable2D();
	
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_TEXTURE_2D );
	
	glColor4ub( 12, 18, 12, 200 );
	glEnable( GL_BLEND );

	///////// rysuj podklad pod description ///////////
	GLboolean bDescBoardDisplay = GL_FALSE; //czy podklad calkowicie rozwiniety
	if( !m_bDescRoll ) {//zwijaj opis
		if( m_rRegionDescription.left < m_rRegions[ MAX_REGIONS_SHIP_BROWSE - 1 ].left ) {
			m_iDescRollActual = static_cast< GLint >( 1000.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
			m_rRegionDescription.left += m_iDescRollActual + 1;
			m_rRegionDescription.bottom -= m_iDescRollActual + 1;
		}
		else
			m_rRegionDescription = m_rRegions[ MAX_REGIONS_SHIP_BROWSE - 1 ];
		
		m_iActualPrintDescLine = 0;
		m_iActualPrintCharInLine = 0;
	}
	else {//rozwijaj opis
		GLfloat fWidth = static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iWidth ) / 100.0f;
		if( m_rRegionDescription.left > static_cast<LONG>( fWidth * 40.0f ) ) {
			m_iDescRollActual = static_cast< GLint >( 1000.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
			m_rRegionDescription.left -= m_iDescRollActual + 1;
			m_rRegionDescription.bottom += m_iDescRollActual + 1;
		}
		else {
			//ustaw m_rRegionDescription na wartosc maksymalna
			m_rRegionDescription.left = static_cast<LONG>( fWidth * 40.0f );
			LONG lOffset = m_rRegions[ MAX_REGIONS_SHIP_BROWSE - 1 ].left - m_rRegionDescription.left;
			m_rRegionDescription.bottom = m_rRegions[ MAX_REGIONS_SHIP_BROWSE - 1 ].bottom + lOffset;
			bDescBoardDisplay = GL_TRUE;
			if( m_iActualPrintDescLine == 0 ) {
				m_iActualPrintDescLine = 1;
				m_iActualPrintCharInLine = 0;
			}
		}
	}
	if( m_rRegionDescription.left < m_rRegions[ MAX_REGIONS_SHIP_BROWSE - 1 ].left ) {
		glBegin( GL_TRIANGLE_STRIP );
			glVertex2i( m_rRegionDescription.right, m_rRegionDescription.bottom );
			glVertex2i( m_rRegionDescription.right, m_rRegionDescription.top );
			glVertex2i( m_rRegionDescription.left, m_rRegionDescription.bottom );
			glVertex2i( m_rRegionDescription.left, m_rRegionDescription.top );
		glEnd();
	}
	///////////////////////////////////////////////////

	for( GLint i = 0; i < MAX_REGIONS_SHIP_BROWSE; ++i ) {
		if( ( m_eActualMode == EBrowseEnemyShips && i == 2 )
			|| ( !m_pGameCtrl->GetShipsManager()->IsAvailableInBrowse( m_iShipsManagerIndex ) && i == 2 )
			|| ( !m_pGameCtrl->GetShipsManager()->IsAvailableInBrowse( m_iShipsManagerIndex ) && i == MAX_REGIONS_SHIP_BROWSE - 1 ) )
			continue; //nie rysuj "Graj" i/lub "opis"

		//jezli statek nie w pelni wyswetlony to rysuj tylko przycisk "wroc"
		if( m_sAutoZoom != 0 && i != MAX_REGIONS_SHIP_BROWSE - 2 )
			continue;

		//nie rysuj "opis" jezeli m_rRegionDescription jest wysuniety
		if( m_rRegionDescription.left < m_rRegions[ MAX_REGIONS_SHIP_BROWSE - 1 ].left && i == MAX_REGIONS_SHIP_BROWSE - 1 )
			continue;

		glBegin( GL_TRIANGLE_STRIP );
			glVertex2i( m_rRegions[ i ].right, m_rRegions[ i ].bottom );
			glVertex2i( m_rRegions[ i ].right, m_rRegions[ i ].top );
			glVertex2i( m_rRegions[ i ].left, m_rRegions[ i ].bottom );
			glVertex2i( m_rRegions[ i ].left, m_rRegions[ i ].top );
		glEnd();
	}
	glDisable( GL_BLEND );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 0; i < MAX_REGIONS_SHIP_BROWSE; ++i ) {
		if( ( m_eActualMode == EBrowseEnemyShips && i == 2 )
			|| ( !m_pGameCtrl->GetShipsManager()->IsAvailableInBrowse( m_iShipsManagerIndex ) && i == 2 )
			|| ( !m_pGameCtrl->GetShipsManager()->IsAvailableInBrowse( m_iShipsManagerIndex ) && i == MAX_REGIONS_SHIP_BROWSE - 1 ) )
			continue; //nie rysuj "Graj" i/lub "opis"

		//jezli statek nie w pelni wyswetlony to rysuj tylko przycisk "wroc"
		if( m_sAutoZoom != 0 && i != MAX_REGIONS_SHIP_BROWSE - 2 )
			continue;

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

		if( i == MAX_REGIONS_SHIP_BROWSE - 2 ) //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
		else {
			if( bPtCursorInRect )
				crColor = ( i == 2 ) ? RGB( 0, 255, 0 ) : RGB( 50, 255, 50 );
			else
				crColor = ( i == 2 ) ? RGB( 50, 128, 50 ) : RGB( 128, 128, 128 );
		}

		m_pSDLFontMenu->DrawText( m_rRegions_Font[ i ], crColor, LNG_LINE( i + 19 ) );
	}

	if( bDescBoardDisplay )
		PrintDescription();
}

GLvoid CRShipsBrowse::PrintDescription()
{
	//rysuj tekst opisu statku
	GLint iIndexText = 0;
	GLint iIndexTextBase = 189;
	switch( m_iShipsManagerIndex ) {
		case 2:
			iIndexText = iIndexTextBase;		//needle
			break;
		case 3:
			iIndexText = iIndexTextBase + 13;	//h.a.l.a.
			break;
		case 5:
			iIndexText = iIndexTextBase + 26;	//unnamed
			break;
		case 6:
			iIndexText = iIndexTextBase + 39;	//razor
			break;
		default:
			return; //!!!!
	}

	if( !m_bPrintDescForThisShip )
		m_iActualPrintDescLine = MAX_REGIONS_DESCRIPTION;

	TCHAR cTempText[ 256 ];
	for( GLint i = 0; i < m_iActualPrintDescLine; ++i ) {
		if( _tcscmp( LNG_LINE( iIndexText + i ), _T("Reserve") ) == 0 ) {
			m_bPrintDescForThisShip = GL_FALSE;
			break;
		}
		if( !m_bPrintDescForThisShip ) //wszystko rysuj normalnie baz drukowania
			m_pSDLFontParam->DrawText( m_rRegionsDesc_Font[ i ].x, m_rRegionsDesc_Font[ i ].y, RGB( 255, 255, 255 ), LNG_LINE( iIndexText + i ) );
		else { //drukuj
			if( i == m_iActualPrintDescLine - 1 ) {
				GLint iLengthText = _tcslen( LNG_LINE( iIndexText + i ) );
				GLint j;
				for( j = 0; j < m_iActualPrintCharInLine && j < iLengthText; ++j )
					cTempText[ j ] = LNG_LINE( iIndexText + i )[ j ];
				cTempText[ j ] = 0;
				m_pSDLFontParam->DrawText( m_rRegionsDesc_Font[ i ].x, m_rRegionsDesc_Font[ i ].y, RGB( 255, 255, 255 ), cTempText );
				if( ++m_iActualPrintCharInLine == iLengthText ) {
					//drukuj nastepna linie
					if( ++m_iActualPrintDescLine > MAX_REGIONS_DESCRIPTION ) {
						m_iActualPrintDescLine = MAX_REGIONS_DESCRIPTION;
						m_bPrintDescForThisShip = GL_FALSE; //caly teks wydrukowany
					}
					else
						m_iActualPrintCharInLine = 0;
				}
			}
			else //rysuj normalnie bo juz wydrukowano
				m_pSDLFontParam->DrawText( m_rRegionsDesc_Font[ i ].x, m_rRegionsDesc_Font[ i ].y, RGB( 255, 255, 255 ), LNG_LINE( iIndexText + i ) );
		}
	}
}

GLvoid CRShipsBrowse::DrawStar( GLint iIndex, GLboolean bFill )
{
	glLoadIdentity();
	m_pGameCtrl->Enable2D();
	glDisable( GL_LIGHTING );
	glColor3f( 1.0f, 1.0f, 1.0f );
	
	glDisable( GL_DEPTH_TEST );
	if( bFill )
		m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexStarFill );
	else
		m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexStarEmpty );

	glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2i( m_rRegionsStars[ iIndex ].right, m_rRegionsStars[ iIndex ].bottom );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2i( m_rRegionsStars[ iIndex ].right, m_rRegionsStars[ iIndex ].top );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2i( m_rRegionsStars[ iIndex ].left, m_rRegionsStars[ iIndex ].bottom );
		glTexCoord2f( 0.0f, 1.0f ); glVertex2i( m_rRegionsStars[ iIndex ].left, m_rRegionsStars[ iIndex ].top );
	glEnd();
}

GLvoid CRShipsBrowse::SetShipInformation()
{
	m_fConstZoom = ( m_iShipsManagerIndex == INDEX_BIG_SHIP_BOSS_01 ) ? DISTANCE_BIG_SHIP_BOSS : DISTANCE_ZOOM;

	lpNameShip = m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_iShipsManagerIndex ).cShipName;
	m_iStarsParameters[ 0 ] = m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_iShipsManagerIndex ).iStarsShield;
	m_iStarsParameters[ 1 ] = m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_iShipsManagerIndex ).iStarsArmour;
	m_iStarsParameters[ 2 ] = m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_iShipsManagerIndex ).iStarsArmament;
	m_iStarsParameters[ 3 ] = m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_iShipsManagerIndex ).iStarsManoeuvring;
}