#include "StdAfx.h"
#include "GameControl.h"
#include "RGame.h"
#include "../Draw/3DObjManager.h"
#include "RShockWaveEffect.h"

CRShockWaveEffect::CRShockWaveEffect()
{
	m_pGameCtrl = CGameControl::GetInstance();
}

CRShockWaveEffect::~CRShockWaveEffect()
{
}

GLvoid CRShockWaveEffect::SetForStart()
{
	m_fShockWaveScale = 1.0f;
	m_bDrawShockWave = GL_FALSE;
}

GLvoid CRShockWaveEffect::DoDrawShockWave( const CVector3 &cPos )
{
#ifdef SHOCKWAVE_3DOBJ
	DoDrawShockWave_3DObj( cPos );
#else
	DoDrawShockWave_Quadric( cPos );
#endif
}

GLvoid CRShockWaveEffect::DrawShockWave()
{
#ifdef SHOCKWAVE_3DOBJ
	DrawShockWave_3DObj();
#else
	DrawShockWave_Quadric();
#endif
}

#ifdef SHOCKWAVE_3DOBJ
GLvoid CRShockWaveEffect::DoDrawShockWave_3DObj( const CVector3 &cPos )
{
	m_fShockWaveScale = 0.1f;
	m_cPosShockWave = cPos;
	m_bDrawShockWave = GL_TRUE;
}

GLvoid CRShockWaveEffect::DrawShockWave_3DObj()
{
	if( !m_bDrawShockWave )
		return;

	glLoadIdentity();
	m_pGameCtrl->Disable2D();
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA,GL_ONE );
	GLfloat fMaxScale = 30.0f;
	GLfloat fAnimSpeed = fMaxScale * 3.0f;

	m_pGameCtrl->GetRGame()->SetCamera();

	GLfloat fBlend = m_fShockWaveScale / fMaxScale;
	glColor4f( 1.0f, 1.0f, 0.5f, 1.05f - fBlend );

	glTranslatef( m_cPosShockWave.x, m_cPosShockWave.y, m_cPosShockWave.z );
	switch( m_pGameCtrl->GetRGame()->GetCameraMode() ) {
		case ECameraModeNormal:
			break;
		default:
			glRotatef( 2.0f, 1.0f, 0.0f, 0.0f );
			break;
	}
	glScalef( m_fShockWaveScale, m_fShockWaveScale, m_fShockWaveScale );

	//obliczenie indekas obiektu fali uderzeniowej
	//0.1 to poczatkowa wartosc m_fShockWaveScale
	GLint iIndex = static_cast< GLint >( m_fShockWaveScale / ( ( fMaxScale - 0.1f ) / static_cast< GLfloat >( SHOCKWAVE_3DOBJ_COUNT ) ) );
	iIndex += SHOCKWAVE_3DOBJ_FIRST_INDEX;
	if( iIndex < SHOCKWAVE_3DOBJ_FIRST_INDEX )
		iIndex = SHOCKWAVE_3DOBJ_FIRST_INDEX;
	else if( iIndex >= SHOCKWAVE_3DOBJ_FIRST_INDEX + SHOCKWAVE_3DOBJ_COUNT )
		iIndex = SHOCKWAVE_3DOBJ_FIRST_INDEX + SHOCKWAVE_3DOBJ_COUNT - 1;

	m_pGameCtrl->Get3DObjManager()->Draw3DObject_Lists( iIndex, GL_FALSE );

	m_fShockWaveScale += ( fAnimSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	if( m_fShockWaveScale > fMaxScale )
		m_bDrawShockWave = GL_FALSE; //nie rysuj
}
#else
GLvoid CRShockWaveEffect::DoDrawShockWave_Quadric( const CVector3 &cPos )
{
	m_fShockWaveScale = 1.0f;
	m_cPosShockWave = cPos;
	m_bDrawShockWave = GL_TRUE;
}

GLvoid CRShockWaveEffect::DrawShockWave_Quadric()
{
	if( !m_bDrawShockWave )
		return;

	glLoadIdentity();
	m_pGameCtrl->Disable2D();
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA,GL_ONE );
	GLfloat fMaxScale = 250.0f;
	GLfloat fAnimSpeed = fMaxScale * 3.2f;

	m_pGameCtrl->GetRGame()->SetCamera();

	GLfloat fBlend = m_fShockWaveScale / fMaxScale;
	glColor4f( 1.0f, 1.0f, 0.7f, 1.05f - fBlend );

	glTranslatef( m_cPosShockWave.x, m_cPosShockWave.y, m_cPosShockWave.z );
	switch( GetCameraMode() ) {
		case ECameraModeNormal:
			glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
			break;
		default:
			glRotatef( -87.0f, 1.0f, 0.0f, 0.0f );
			break;
	}

	gluDisk( m_pGameCtrl->GetQuadric(), m_fShockWaveScale * 0.7f, m_fShockWaveScale * 1.0f, 32, 2 );

	m_fShockWaveScale += ( fAnimSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	if( m_fShockWaveScale > fMaxScale )
		m_bDrawShockWave = GL_FALSE; //nie rysuj
}
#endif