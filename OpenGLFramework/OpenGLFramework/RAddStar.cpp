
#include "StdAfx.h"
#include "GameControl.h"
#include "WindowData.h"
#include "RGame.h"
#include "RPlayer.h"
#include "ShipsManager.h"
#include "RDifficultyLevel.h"
#include "../Fonts/SDLFont.h"
#include "Cursor.h"
#include "RAddStar.h"

CRAddStar::CRAddStar()
{
	m_pGameCtrl = CGameControl::GetInstance();
	m_pSDLTitle = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 40 );
	m_pSDLFontText = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 20 );
	m_pSDLFontParam = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 1 ), 20 );

	SetRegions();

	//dla napisow parametrow
	m_iCountPixelOffsetParamX = static_cast< GLint >( 10.24f * 15.0f );
	m_iCountPixelOffsetParamY = static_cast< GLint >( 7.68f * 0.5f );

	m_iCounterAddStar = 0;
	m_bDraw = GL_FALSE;
	m_bWasCalledDraw = GL_FALSE;
}

CRAddStar::~CRAddStar()
{
	delete m_pSDLTitle;
	delete m_pSDLFontText;
	delete m_pSDLFontParam;
}

GLvoid CRAddStar::SetRegions()
{
	//ustaw ramke na caly ekran
	m_rRegionBackground[ 0 ].left = 0;
	m_rRegionBackground[ 0 ].right = CWindowData::GetInstance()->SettingFile().iWidth;
	m_rRegionBackground[ 0 ].top = 0;
	m_rRegionBackground[ 0 ].bottom = CWindowData::GetInstance()->SettingFile().iHeight;
	//ustawiamy tlo - ramke pod okno
	SetRegionX( m_rRegionBackground[ 1 ].left, 10 );
	SetRegionX( m_rRegionBackground[ 1 ].right, 90 );
	SetRegionY( m_rRegionBackground[ 1 ].top, 20 );
	SetRegionY( m_rRegionBackground[ 1 ].bottom, 80 );

	//ustawienie regionow gwiazdek
	//[ 0 ][ 1 ][ 2 ]
	//[ 3 ][ 4 ][ 5 ]
	GLfloat fResolutionRelation = GLfloat( CWindowData::GetInstance()->SettingFile().iWidth ) / GLfloat( CWindowData::GetInstance()->SettingFile().iHeight );
	GLfloat fSizeA = 3.0f;//1.5f; //dlugosc boku
	GLfloat fSizeB = fSizeA * fResolutionRelation; //wysokosc boku
	GLfloat fPercentLeft = 50.0f;
	GLfloat fPercentTop = 40.0f;
	for( GLint i = 0; i < 12; ++i ) {
		SetRegionX( m_rRegionsStars[ i ].left, fPercentLeft );
		SetRegionX( m_rRegionsStars[ i ].right, fPercentLeft + fSizeA );

		SetRegionX( m_rRegionsStarsFont[ i ].left, fPercentLeft, GL_TRUE );
		SetRegionX( m_rRegionsStarsFont[ i ].right, fPercentLeft + fSizeA, GL_TRUE );
		fPercentLeft += fSizeA + 0.5f;
		
		SetRegionY( m_rRegionsStars[ i ].top, fPercentTop );
		SetRegionY( m_rRegionsStars[ i ].bottom, fPercentTop + fSizeB );

		SetRegionY( m_rRegionsStarsFont[ i ].top, fPercentTop, GL_TRUE );
		SetRegionY( m_rRegionsStarsFont[ i ].bottom, fPercentTop + fSizeB, GL_TRUE );

		if( i == 2 || i == 5 || i == 8 ) {
			fPercentLeft = 50.0f;
			fPercentTop += ( fSizeB + ( 0.4f * fResolutionRelation ) );
		}
	}

	//przypisanie regionow dla [+] z regionow gwiazdek
	GLint j = 0;
	for( GLint i = 2; i < 12; i += 3 ) {
		m_rRegionsButton[ j ].bottom = m_rRegionsStars[ i ].bottom;
		m_rRegionsButton[ j ].top = m_rRegionsStars[ i ].top;
		SetRegionX( m_rRegionsButton[ j ].left, fPercentLeft );
		SetRegionX( m_rRegionsButton[ j ].right, fPercentLeft + fSizeA );

		m_rRegionsFontButton[ j ].bottom = m_rRegionsStarsFont[ i ].bottom;
		m_rRegionsFontButton[ j ].top = m_rRegionsStarsFont[ i ].top;
		SetRegionX( m_rRegionsFontButton[ j ].left, fPercentLeft, GL_TRUE );
		SetRegionX( m_rRegionsFontButton[ j ].right, fPercentLeft + fSizeA, GL_TRUE );

		++j;
	}

	//ustawienie regionu dla przycisku [OK]
	SetRegionX( m_rRegionsButton[ 4 ].left, 40 );
	SetRegionX( m_rRegionsButton[ 4 ].right, 60 );
	SetRegionY( m_rRegionsButton[ 4 ].top, 74 );
	SetRegionY( m_rRegionsButton[ 4 ].bottom, 79 );

	SetRegionX( m_rRegionsFontButton[ 4 ].left, 40, GL_TRUE );
	SetRegionX( m_rRegionsFontButton[ 4 ].right, 60, GL_TRUE );
	SetRegionY( m_rRegionsFontButton[ 4 ].top, 74, GL_TRUE );
	SetRegionY( m_rRegionsFontButton[ 4 ].bottom, 79, GL_TRUE );
}

GLvoid CRAddStar::RestartObjects()
{
	SetRegions();
}

GLvoid CRAddStar::SetScoreThresholds()
{
	//dla dodawania gwiazdek, progi punktowe
	m_aScoreThresholds[ 0 ] = 3000 * m_pGameCtrl->GetRDifficultyLevel()->GetDifficultyLevel();
	for( GLint i = 1; i < MAX_ADD_STAR; ++i )
		m_aScoreThresholds[ i ] = ( ( m_aScoreThresholds[ i - 1 ] * 2 ) + ( m_aScoreThresholds[ i - 1 ] / 2 ) );
	m_iCounterAddStar = 0;
}

GLvoid CRAddStar::SetStarsNumberAndDoDraw( GLint iStarsShield, GLint iStarsArmour, GLint iStarsArmament, GLint iStarsManoeuvring )
{
	m_iStarsShield = iStarsShield;
	m_iStarsArmour = iStarsArmour;
	m_iStarsArmament = iStarsArmament;
	m_iStarsManoeuvring = iStarsManoeuvring;
	if( m_iStarsShield == 3 && m_iStarsArmour == 3 && m_iStarsArmament == 3 && m_iStarsManoeuvring == 3 )
		m_bAddedStar = GL_TRUE;
	else
		m_bAddedStar = GL_FALSE;

	m_bDraw = GL_TRUE;
	m_bWasCalledDraw = GL_TRUE;
}

GLvoid CRAddStar::NoDraw()
{
	m_bDraw = GL_FALSE;
	++m_iCounterAddStar;
}

GLvoid CRAddStar::Draw()
{
	glLoadIdentity();

	//rysujemy przyciski
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );

	DrawRegions( m_rRegionBackground, 2 );	// regiony na caly ekran, oraz okno
	DrawRegionBorder( m_rRegionBackground[ 1 ] );//rysujemy ramkê - niby okna

	DrawStarsParam();
	DrawButtonts();

	m_pSDLTitle->DrawText( 0, static_cast< GLint >( 7.68f * 30.0f ), RGB( 100, 100, 255 ), LNG_LINE( 60 ), GL_TRUE );
}

GLvoid CRAddStar::DrawStarsParam()
{
	//rysujemy gwiazdki
	GLint iPosX = m_rRegionsStarsFont[ 0 ].left - m_iCountPixelOffsetParamX;
	m_pSDLFontParam->DrawText( iPosX, m_rRegionsStarsFont[ 0 ].bottom + m_iCountPixelOffsetParamY, RGB( 255, 255, 255 ), LNG_LINE( 27 ) );
	GLboolean bFill = GL_FALSE;
	for( GLint i = 2; i >= 0; --i ) {
		if( m_iStarsShield - 1 == i ) 
			bFill = GL_TRUE;
		DrawStar( i, bFill );
	}
	//pancerz
	m_pSDLFontParam->DrawText( iPosX, m_rRegionsStarsFont[ 3 ].bottom + m_iCountPixelOffsetParamY, RGB( 255, 255, 255 ), LNG_LINE( 28 ) );
	bFill = GL_FALSE;
	for( GLint i = 5; i >= 3; --i ) {
		if( m_iStarsArmour - 1 == i - 3 ) 
			bFill = GL_TRUE;
		DrawStar( i, bFill );
	}
	//uzbrojenie
	m_pSDLFontParam->DrawText( iPosX, m_rRegionsStarsFont[ 6 ].bottom + m_iCountPixelOffsetParamY, RGB( 255, 255, 255 ), LNG_LINE( 29 ) );
	bFill = GL_FALSE;
	for( GLint i = 8; i >= 6; --i ) {
		if( m_iStarsArmament - 1 == i - 6 ) 
			bFill = GL_TRUE;
		DrawStar( i, bFill );
	}
	//zwrotnosc
	m_pSDLFontParam->DrawText( iPosX, m_rRegionsStarsFont[ 9 ].bottom + m_iCountPixelOffsetParamY, RGB( 255, 255, 255 ), LNG_LINE( 30 ) );
	bFill = GL_FALSE;
	for( GLint i = 11; i >= 9; --i ) {
		if( m_iStarsManoeuvring - 1 == i - 9 ) 
			bFill = GL_TRUE;
		DrawStar( i, bFill );
	}

	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
}

GLvoid CRAddStar::DrawStar( GLint iIndex, GLboolean bFill )
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

GLvoid CRAddStar::DrawButtonts()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	//rysujemy przyciski
	m_pGameCtrl->Enable2D();

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_DEPTH_TEST );

	glColor4ub( 12, 18, 12, 200 );
	glEnable( GL_BLEND );
	GLint i;
	for( i = 0; i < MAX_REGIONS_ADD_STAR; ++i ) {
		//nie rysuj [+] jezli juz dodano
		if( m_bAddedStar && i < 4 )
			continue;
		if( m_iStarsShield == 3 && i == 0 )
			continue;
		if( m_iStarsArmour == 3 && i == 1 )
			continue;
		if( m_iStarsArmament == 3 && i == 2 )
			continue;
		if( m_iStarsManoeuvring == 3 && i == 3 )
			continue;
		//nie rysuj [ ok ] jezeli nie dodano gwiazdki
		if( !m_bAddedStar && i == MAX_REGIONS_ADD_STAR - 1 )
			continue;
		glBegin( GL_TRIANGLE_STRIP );
			glVertex2i( m_rRegionsButton[ i ].right, m_rRegionsButton[ i ].bottom );
			glVertex2i( m_rRegionsButton[ i ].right, m_rRegionsButton[ i ].top );
			glVertex2i( m_rRegionsButton[ i ].left, m_rRegionsButton[ i ].bottom );
			glVertex2i( m_rRegionsButton[ i ].left, m_rRegionsButton[ i ].top );
		glEnd();
	}
	glDisable( GL_BLEND );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( i = 0; i < MAX_REGIONS_ADD_STAR; ++i ) {
		//nie rysuj [+] jezli juz dodano
		if( m_bAddedStar && i < 4 )
			continue;
		if( m_iStarsShield == 3 && i == 0 )
			continue;
		if( m_iStarsArmour == 3 && i == 1 )
			continue;
		if( m_iStarsArmament == 3 && i == 2 )
			continue;
		if( m_iStarsManoeuvring == 3 && i == 3 )
			continue;
		//nie rysuj [ ok ] jezeli nie dodano gwiazdki
		if( !m_bAddedStar && i == MAX_REGIONS_ADD_STAR - 1 )
			continue;
		if( CheckPtCursorInRect( m_pGameCtrl->GetPointCursor(), m_rRegionsButton[ i ] ) ) {
			bPtCursorInRect = GL_TRUE;
			m_iWhereIsMouse = i;
			DrawRegionIfMouse( m_rRegionsButton[ i ] );
		}
		else {
			bPtCursorInRect = GL_FALSE;
			if( m_iWhereIsMouse == i )
				m_iWhereIsMouseOld = -1; //nigdzie
		}

		if( i == MAX_REGIONS_ADD_STAR - 1 ) { //wyjscie lekko na czerwono
			if( bPtCursorInRect )
				crColor = RGB( 0, 255, 0 );
			else
				crColor = RGB( 50, 255, 50 );
			m_pSDLFontText->DrawText( m_rRegionsFontButton[ i ], crColor, LNG_LINE( 8 ) );
		}
		else {
			if( bPtCursorInRect )
				crColor = RGB( 50, 255, 50 );
			else
				crColor = RGB( 128, 128, 128 );
			m_pSDLFontText->DrawText( m_rRegionsFontButton[ i ], crColor, _T("+") );
		}
	}
}

GLboolean CRAddStar::MouseLButtonDown( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegionsButton, MAX_REGIONS_ADD_STAR, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //[+] oslona
			if( !m_bAddedStar && m_iStarsShield < 3 ) {
				++m_iStarsShield;
				m_bAddedStar = GL_TRUE;
			}
			return GL_TRUE;
		case 1: //[+] pancerz
			if( !m_bAddedStar && m_iStarsArmour < 3 ) {
				++m_iStarsArmour;
				m_bAddedStar = GL_TRUE;
			}
			return GL_TRUE;
		case 2: //[+] uzbrojenie
			if( !m_bAddedStar && m_iStarsArmament < 3 ) {
				++m_iStarsArmament;
				m_bAddedStar = GL_TRUE;
			}
			return GL_TRUE;
		case 3: //[+] zwrotnosc
			if( !m_bAddedStar && m_iStarsManoeuvring < 3 ) {
				++m_iStarsManoeuvring;
				m_bAddedStar = GL_TRUE;
			}
			return GL_TRUE;
		case 4: //[OK]
			if( m_bAddedStar )
				NoDrawAddStar();
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CRAddStar::KeyDown( GLuint /*uiKeyCode*/ )
{
	return GL_TRUE;
}

GLboolean CRAddStar::CanAddStar( GLuint uiScore )
{
	return ( !m_bWasCalledDraw 
			&& m_iCounterAddStar < MAX_ADD_STAR 
			&& uiScore > m_aScoreThresholds[ m_iCounterAddStar ] );
}

GLvoid CRAddStar::DoDrawAddStar()
{
	CRPlayer *pPlayer = m_pGameCtrl->GetRGame()->GetPlayer();

	SetStarsNumberAndDoDraw( pPlayer->GetStarsShield(),
							 pPlayer->GetStarsArmour(),
							 pPlayer->GetStarsArmament(),
							 pPlayer->GetStarsManoeuvring() );

	SetPause( GL_TRUE );
	m_pGameCtrl->GetRGame()->ShowCursor();

	if( CWindowData::GetInstance()->SettingFile().bSound )
		COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundFanfary );
}

GLvoid CRAddStar::NoDrawAddStar()
{
	CRPlayer *pPlayer = m_pGameCtrl->GetRGame()->GetPlayer();

	//ustawiamy gwiazdki
	pPlayer->SetStarsShield( GetStarsShield() );
	pPlayer->SetStarsArmour( GetStarsArmour() );
	pPlayer->SetStarsArmament( GetStarsArmament() );
	pPlayer->SetStarsManoeuvring( GetStarsManoeuvring() );

	GLint iNewShieldMax = m_pGameCtrl->GetShipsManager()->GetShieldPoints( pPlayer->Get3DObjectIndex(), pPlayer->GetStarsShield() );
	//Wez roznice i dodaj do actual
	GLint iDifference = iNewShieldMax - pPlayer->GetShieldMax();
	//ustaw nowy max
	pPlayer->SetShieldMax( iNewShieldMax );
	pPlayer->IncreaseShieldActual( iDifference );
	if( pPlayer->GetShieldActual() > pPlayer->GetShieldMax() )
		pPlayer->SetShieldActual( pPlayer->GetShieldMax() );
	//
	GLint iNewArmourMax = m_pGameCtrl->GetShipsManager()->GetArmourPoints( pPlayer->Get3DObjectIndex(), pPlayer->GetStarsArmour() );
	iDifference = iNewArmourMax - pPlayer->GetArmourMax();
	pPlayer->SetArmourMax( iNewArmourMax );
	pPlayer->IncreaseArmourActual( iDifference );
	if( pPlayer->GetArmourActual() > pPlayer->GetArmourMax() )
		pPlayer->SetArmourActual( pPlayer->GetArmourMax() );
	//
	GLint iNewArmamentBase = m_pGameCtrl->GetShipsManager()->GetArmamentPoints( pPlayer->Get3DObjectIndex(), pPlayer->GetStarsArmament() );
	iDifference = iNewArmamentBase - pPlayer->GetArmamentBase();
	pPlayer->SetArmamentBase( iNewArmamentBase );
	pPlayer->IncreaseArmamentActual( iDifference );
	//
	pPlayer->SetManoeuvring( m_pGameCtrl->GetShipsManager()->GetManoeuvringPercent( pPlayer->Get3DObjectIndex(), pPlayer->GetStarsManoeuvring() ) );

	m_pGameCtrl->GetRGame()->CalculateShieldIndicator();
	m_pGameCtrl->GetRGame()->CalculateArmourIndicator();

	SetPause( GL_FALSE );
	m_pGameCtrl->GetCursor()->SetMyCursor( CCursor::EGame );

	NoDraw();
}

GLvoid CRAddStar::SetPause( GLboolean bPause )
{
	m_pGameCtrl->GetRGame()->SetPause( bPause );
	m_pGameCtrl->GetSpeedCtrl()->SetPause( bPause );
	m_pGameCtrl->GetRGame()->GetPlayer()->SetCursorPosAfterPause();
}