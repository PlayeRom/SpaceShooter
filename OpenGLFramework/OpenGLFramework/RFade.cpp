
#include "StdAfx.h"
#include "GameControl.h"
#include "RFade.h"

CRFade::CRFade()
{
	m_bDraw = GL_FALSE;
}

CRFade::~CRFade()
{
}

GLvoid CRFade::DoDrawFullFade()
{
	m_fFade = 0.0f;
	m_bDraw = GL_TRUE;
	m_bChangeLevel = GL_FALSE;
	m_bFullFade = GL_TRUE;
}

GLvoid CRFade::DoDrawHalfFade()
{
	m_fFade = 1.1f; // wiecej niz 1.0 aby przytrzymac dluzej czarny ekran
	m_bDraw = GL_TRUE;
	m_bChangeLevel = GL_FALSE;
	m_bFullFade = GL_FALSE;
}

GLvoid CRFade::Draw()
{
	if( !m_bDraw )
		return;

	glLoadIdentity();
	CGameControl::GetInstance()->Enable2D();
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_TEXTURE_2D );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	glColor4f( 0, 0, 0, m_fFade ); // gdy m_fFade = 1.0 to mamy czarny ekran

	CGameControl::GetInstance()->DrawQuadOnAllScreen();
	glDisable( GL_BLEND );

	//wroc do narmalnego koloru
	glColor4ub( 255, 255, 255, 255 );

	if( m_bFullFade )
		CalculateFullFade();
	else
		CalculateHalfFade();
}

GLvoid CRFade::CalculateFullFade()
{
	if( m_fFade < 1.0f && !m_bChangeLevel ) {
		// etap 1: przyciemniamy ekran
		m_fFade += CGameControl::GetInstance()->GetSpeedCtrl()->GetMultiplier();
	}
	else {
		// etap 2: rozjaœniamy ekran
		m_bChangeLevel = GL_TRUE;
		m_fFade -= CGameControl::GetInstance()->GetSpeedCtrl()->GetMultiplier();
		if( m_fFade < 0.0f ) {
			m_fFade = 0.0f;
			m_bDraw = GL_FALSE;
		}
	}
}

GLvoid CRFade::CalculateHalfFade()
{
	m_fFade -= CGameControl::GetInstance()->GetSpeedCtrl()->GetMultiplier();
	if( m_fFade < 0.0f ) {
		m_fFade = 0.0f;
		m_bDraw = GL_FALSE;
	}
}