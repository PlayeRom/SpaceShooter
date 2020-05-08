#include "StdAfx.h"
#include "GameControl.h"
#include "RGame.h"
#include "RBoss.h"

CRBoss::CRBoss()
{
	m_pGameCtrl = CGameControl::GetInstance();
}

CRBoss::~CRBoss()
{
}

GLvoid CRBoss::SetNewDestinationPointForBoss()
{
	//TODO: dla multiplayera m_cBossDestinationPoint musi byc ustalane przez tworce gry i wysylane

	GLfloat fRandX = GLfloat( rand() %GLint( BOSS_ZONE_RIGHT_X * 2.0f ) ) - BOSS_ZONE_RIGHT_X;
	GLfloat fDistanceZ = CVector3( 0.0f, 0.0f, BOSS_ZONE_MAX ).CalcDistanceZ( CVector3( 0.0f, 0.0f, BOSS_ZONE_MIN ) );
	GLfloat fRandZ = BOSS_ZONE_MAX + GLfloat( rand() % GLint( fDistanceZ ) );

	m_cBossDestinationPoint = CVector3( fRandX, 0.0f, fRandZ );
}

GLvoid CRBoss::DrawEnergyBossBars( const CVector3 &in_cPos,
								   GLint iActualPointsShield, GLint iMaxPointsShield,
								   GLint iActualPointsArmour, GLint iMaxPointsArmour )
{
	m_fEnergyBossBar_left = -14.0f;
	m_fEnergyBossBar_right = 14.0f;
	m_fEnergyBossBar_top = 2.0f;
	m_fEnergyBossBar_bottom = -2.0f;

	glPushMatrix();
	glLoadIdentity();
	m_pGameCtrl->GetRGame()->SetCamera();

	glTranslatef( in_cPos.x, in_cPos.y + 20.0f, in_cPos.z );

	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	// rysuj pasek oslony
	if( iActualPointsShield > 0 ) {
		glColor4ub( 0, 255, 255, 255 );
		DrawEnergyBossBar( iActualPointsShield, iMaxPointsShield );
	}

	// rysuj pasek pancerza

	glTranslatef( 0.0f, -4.0f, 0.0f );
	glColor4ub( 175, 128, 128, 255 );
	DrawEnergyBossBar( iActualPointsArmour, iMaxPointsArmour );

	glEnable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );

	glPopMatrix();
}

GLvoid CRBoss::DrawEnergyBossBar( GLint iActualPoints, GLint iMaxPoints )
{
	GLfloat fEntity = ( m_fEnergyBossBar_right - m_fEnergyBossBar_left ) / static_cast< GLfloat >( iMaxPoints );
	GLfloat fActualBarPos = m_fEnergyBossBar_left + ( fEntity * static_cast< GLfloat >( iActualPoints ) );
	if( fActualBarPos > m_fEnergyBossBar_right )
		fActualBarPos = m_fEnergyBossBar_right;
	if( fActualBarPos > m_fEnergyBossBar_left ) {
		glBegin( GL_TRIANGLE_STRIP );
			glVertex3f( fActualBarPos,         m_fEnergyBossBar_bottom, 0.0f );
			glVertex3f( fActualBarPos,         m_fEnergyBossBar_top,    0.0f );
			glVertex3f( m_fEnergyBossBar_left, m_fEnergyBossBar_bottom, 0.0f );
			glVertex3f( m_fEnergyBossBar_left, m_fEnergyBossBar_top,    0.0f );
		glEnd();
	}
}

GLvoid CRBoss::MoveEnemyBoss( SEnemyShip &in_sEnemy, GLfloat fSpeed )
{
	DrawEnergyBossBars( in_sEnemy.cActualPos, in_sEnemy.iShield, in_sEnemy.iMaxShield, in_sEnemy.iArmour, in_sEnemy.iMaxArmour );

	// lec do punktu docelowego
	GLfloat fSpeed2 = fSpeed;
	if( in_sEnemy.cActualPos.z >= BOSS_ZONE_MAX ) //jezeli jest w strefie BOSS_ZONE...
		fSpeed2 /= 2.0f;

	CVector3 cDiff = m_cBossDestinationPoint - in_sEnemy.cActualPos;
	cDiff.Normalize();
	if( cDiff.x < 0.0f )
		cDiff.x *= -1;
	if( cDiff.z < 0.0f )
		cDiff.z *= -1;

	if( in_sEnemy.cActualPos.x > m_cBossDestinationPoint.x ) {
		in_sEnemy.cActualPos.x -= ( cDiff.x * fSpeed2 * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( in_sEnemy.cActualPos.x < m_cBossDestinationPoint.x )
			in_sEnemy.cActualPos.x = m_cBossDestinationPoint.x;
	}
	else if( in_sEnemy.cActualPos.x < m_cBossDestinationPoint.x ) {
		in_sEnemy.cActualPos.x += ( cDiff.x * fSpeed2 * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( in_sEnemy.cActualPos.x > m_cBossDestinationPoint.x )
			in_sEnemy.cActualPos.x = m_cBossDestinationPoint.x;
	}

	if( in_sEnemy.cActualPos.z > m_cBossDestinationPoint.z ) {
		in_sEnemy.cActualPos.z -= ( cDiff.z * fSpeed2 * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( in_sEnemy.cActualPos.z < m_cBossDestinationPoint.z )
			in_sEnemy.cActualPos.z = m_cBossDestinationPoint.z;
	}
	else if( in_sEnemy.cActualPos.z < m_cBossDestinationPoint.z ) {
		in_sEnemy.cActualPos.z += ( cDiff.z * fSpeed2 * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( in_sEnemy.cActualPos.z > m_cBossDestinationPoint.z )
			in_sEnemy.cActualPos.z = m_cBossDestinationPoint.z;
	}

	//jezeli dotarl do punktu, ustal nowy punkt
	if( in_sEnemy.cActualPos == m_cBossDestinationPoint )
		SetNewDestinationPointForBoss();

	//rysuj linie od statku boss do jego pkt docelowego
	/*glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glColor3f( 0.0f, 1.0f, 0.0f );
	glPushMatrix();
	glLoadIdentity();
	SetCamera();
	glBegin( GL_LINES);
		glVertex3f( in_sEnemy.cActualPos.x,
			in_sEnemy.cActualPos.y,
			in_sEnemy.cActualPos.z );
		glVertex3f( m_cBossDestinationPoint.x,
			m_cBossDestinationPoint.y,
			m_cBossDestinationPoint.z );
	glEnd();
	glPopMatrix();
	glColor3f( 1.0f, 1.0f, 1.0f );
	glEnable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );*/
}