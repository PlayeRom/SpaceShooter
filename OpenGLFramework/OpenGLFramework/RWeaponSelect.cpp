
#include "StdAfx.h"
#include "WindowData.h"
#include "GameControl.h"
#include "RGame.h"
#include "RWeaponSelect.h"

CRWeaponSelect::CRWeaponSelect()
{
	m_bDraw = GL_FALSE;
	m_iItem = 0;
	m_fFade = 1.0f;

	SetRegions();

	if( EWeaponLastElement != CGameControl::GetInstance()->GetTexWeaponSelectSize() )
		::MessageBox( NULL, _T("EWeaponLastElement != CGameControl::GetInstance()->GetTexWeaponSelectSize()!"), _T("CRWeaponSelect ERROR"), MB_OK|MB_ICONERROR );
}

CRWeaponSelect::~CRWeaponSelect()
{
}

GLvoid CRWeaponSelect::SetRegions()
{
	//ustaw regiony
	GLfloat fSizeA = 6.0f; //dlugosc boku
	GLfloat fResolutionRelation = static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iWidth ) / static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iHeight );
	GLfloat fSizeB = fSizeA * fResolutionRelation; //wysokosc boku
	GLfloat fPercent = 50.0f - fSizeA - ( fSizeA / 2.0f );
	for( GLint i = 0; i < 3; ++i ) {
		SetRegionX( m_rRegions[ i ].left, fPercent );
		fPercent += fSizeA;
		SetRegionX( m_rRegions[ i ].right, fPercent );
		fPercent += 0.5f;

		SetRegionY( m_rRegions[ i ].top, 0.5f );
		SetRegionY( m_rRegions[ i ].bottom, 0.5f + fSizeB );
	}
}

GLvoid CRWeaponSelect::RestartObjects()
{
	SetRegions();
}

GLvoid CRWeaponSelect::DoDraw( GLint iItem )
{
	m_iItem = iItem;
	m_bDraw = GL_TRUE;
	m_fFade = 1.0f;
	if( CWindowData::GetInstance()->SettingFile().bSound ) {
		//stopwszystkim
		for( GLint i = 0; i < CGameControl::GetInstance()->GetSoundWeaponSelectSize(); ++i )
			COpenALManager::GetInstance()->Stop( CGameControl::GetInstance()->m_aSoundWeaponSelect[ i ] );

		if( m_iItem >= 0 && m_iItem < CGameControl::GetInstance()->GetSoundWeaponSelectSize() )
			COpenALManager::GetInstance()->PlayNo3D( CGameControl::GetInstance()->m_aSoundWeaponSelect[ m_iItem ] );
	}
}

GLvoid CRWeaponSelect::Draw()
{
	if( !m_bDraw )
		return;

	glLoadIdentity();
	CGameControl::GetInstance()->Enable2D();
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	if( m_iItem == 0 ) { //skrajny lewy
		DrawQuad( 0, 0, GL_TRUE );
		DrawQuad( 1, 1 );
		DrawQuad( 2, 2 );
	}
	else if( m_iItem == EWeaponLastElement - 1 ) { //skrajny prawy
		DrawQuad( 0, EWeaponLastElement - 3 );
		DrawQuad( 1, EWeaponLastElement - 2 );
		DrawQuad( 2, EWeaponLastElement - 1, GL_TRUE );
	}
	else { //srodkowy jakis
		DrawQuad( 0, m_iItem - 1 );
		DrawQuad( 1, m_iItem, GL_TRUE );
		DrawQuad( 2, m_iItem + 1 );
	}

	m_fFade -= ( 0.5f * CGameControl::GetInstance()->GetSpeedCtrl()->GetMultiplier() );
	if( m_fFade <= 0.0f )
		m_bDraw = GL_FALSE;

	glDisable( GL_BLEND );
}

GLvoid CRWeaponSelect::DrawQuad( GLint iRectIndex, GLint iTexIndex, GLboolean bSelectedColor /*= GL_FALSE*/ )
{
	if( iRectIndex < 0 || iRectIndex >= 3 )
		return;

	GLint iSize = 0;
	if( bSelectedColor ) {
		glColor4f( 1.0f, 1.0f, 0.1f, m_fFade ); //kolor selected
		iSize = 4;// zaznaczona ikonka bedzie troche wieksza
	}
	else
		glColor4f( 1.0f, 1.0f, 1.0f, m_fFade ); //kolor nie selected

	CGameControl::GetInstance()->GetTexLoader()->SetTexture( CGameControl::GetInstance()->m_aTexWeaponSelect[ iTexIndex ] );

	glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2i( m_rRegions[ iRectIndex ].right + iSize, m_rRegions[ iRectIndex ].bottom + iSize );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2i( m_rRegions[ iRectIndex ].right + iSize, m_rRegions[ iRectIndex ].top - iSize );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2i( m_rRegions[ iRectIndex ].left - iSize, m_rRegions[ iRectIndex ].bottom + iSize );
		glTexCoord2f( 0.0f, 1.0f ); glVertex2i( m_rRegions[ iRectIndex ].left - iSize, m_rRegions[ iRectIndex ].top - iSize );
	glEnd();
}