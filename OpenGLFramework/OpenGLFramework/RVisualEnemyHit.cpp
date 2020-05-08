#include "StdAfx.h"
#include "GameControl.h"
#include "../Draw/Particles.h"
#include "RGame.h"
#include "RVisualEnemyHit.h"

CRVisualEnemyHit::CRVisualEnemyHit()
{
	m_pGameCtrl = CGameControl::GetInstance();

	Create();
}

CRVisualEnemyHit::~CRVisualEnemyHit()
{
	Clear();
}

GLvoid CRVisualEnemyHit::Create()
{
	for( GLint i = 0; i < MAX_VISUAL_ENEMY_HIT; ++i ) {
		SEmitterEnemyHit sEmitterEnemyHit;
		sEmitterEnemyHit.bIsDraw = GL_FALSE;
		sEmitterEnemyHit.cPos = CVector3();
		sEmitterEnemyHit.iEmitterIndex = m_pGameCtrl->GetParticles()->CreateEmitter(
			CVector3(),
			m_pGameCtrl->m_iTexParticle01, //textura
			CVector3(),
			CVector3(),
			2.0f, //skala
			1.0f, //life
			GL_FALSE, //czasteczki nie moga ozywac po wygasnieciu
			1.0f, //r
			1.0f, //g
			0.2f, //b
			0.0f, //speed x
			0.0f, //speed y
			0.0f, //speed z
			70 ); //ilosc

		m_aEmitterEnemyHit.push_back( sEmitterEnemyHit );
	}
}

GLvoid CRVisualEnemyHit::Clear()
{
	m_aEmitterEnemyHit.clear();
}

//ustawiamy do rysowania - wywolujemy w momencie gdy jakis wrog oberwie
GLvoid CRVisualEnemyHit::SetToDraw( const CVector3 &in_cPos )
{
	//szukamy wolnego
	for( GLint i = 0; i < GetEmitterEnemyHitSize(); ++i ) {
		if( !m_aEmitterEnemyHit[ i ].bIsDraw ) {
			m_aEmitterEnemyHit[ i ].bIsDraw = GL_TRUE;
			m_aEmitterEnemyHit[ i ].cPos = in_cPos;
			break;
		}
	}
}

GLvoid CRVisualEnemyHit::Draw()
{
	glLoadIdentity();
	m_pGameCtrl->GetRGame()->SetCamera();

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	m_pGameCtrl->Disable2D();

	glColor4ub( 255, 255, 255, 255 );

	for( GLint i = 0; i < GetEmitterEnemyHitSize(); ++i ) {
		if( m_aEmitterEnemyHit[ i ].bIsDraw ) {
			if( m_pGameCtrl->GetParticles()->IsAliveParticleInEmitter( m_aEmitterEnemyHit[ i ].iEmitterIndex ) ) {
				//sa zywe czasteczki, wiec mozemy rysowac
				glPushMatrix();
				glTranslatef( m_aEmitterEnemyHit[ i ].cPos.x, m_aEmitterEnemyHit[ i ].cPos.y, m_aEmitterEnemyHit[ i ].cPos.z );
				m_pGameCtrl->GetParticles()->DrawEmitter( m_aEmitterEnemyHit[ i ].iEmitterIndex );
				glPopMatrix();
			}
			else {
				//wczytskie czasteczki wymarly, wiec je przywracamy do nastepnego wykorzystania
				m_aEmitterEnemyHit[ i ].bIsDraw = GL_FALSE;
				m_pGameCtrl->GetParticles()->SetParticleInEmitterAsLive( m_aEmitterEnemyHit[ i ].iEmitterIndex );
			}
		}
	}
}