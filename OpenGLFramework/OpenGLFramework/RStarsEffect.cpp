#include "StdAfx.h"
#include "GameControl.h"
#include "RGame.h"
#include "RStarsEffect.h"

CRStarsEffect::CRStarsEffect()
{
	m_pGameCtrl = CGameControl::GetInstance();

	CreateStars();
}

CRStarsEffect::~CRStarsEffect()
{
	DeleteStars();
}

GLvoid CRStarsEffect::CreateStars()
{
	DeleteStars();
	SStar sStar;
	sStar.bStart = GL_FALSE;
	sStar.cPos = CVector3();
	sStar.fSpeed = 0.0f;
	for( GLint i = 0; i < 200; ++i ) {
		m_aStars.push_back( sStar );

		if( i < 100 ) {
			//aby byly rysowana na samym poczatku gry
			m_aStars[ i ].cPos.x = static_cast< GLfloat >( rand() % 1210 ) - 605.0f;
			m_aStars[ i ].cPos.y = 0.0f;
			m_aStars[ i ].cPos.z = -( static_cast< GLfloat >( rand() % 2251 ) - 250.0f );
			m_aStars[ i ].fSpeed = static_cast< GLfloat >( rand() % 140 ) + 140.0f;
			m_aStars[ i ].cColor.x = m_aStars[ i ].cColor.y = m_aStars[ i ].cColor.z = static_cast< GLfloat >( rand() % 40 + 20 ) / 100.0f;
			m_aStars[ i ].bStart = GL_TRUE;
		}
	}
}

GLvoid CRStarsEffect::DeleteStars()
{
	m_aStars.clear();
}

GLvoid CRStarsEffect::DrawStars()
{
	glLoadIdentity();
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );

	m_pGameCtrl->Disable2D();

	m_pGameCtrl->GetRGame()->SetCamera();

	GLint iStarsSize = static_cast< GLint >( m_aStars.size() );
	if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime(/*250*/100, 7 ) ) {
		//dodajemy nowa wolna gwiazdke na ekran
		for( GLint i = 0; i < iStarsSize; ++i ) {
			if( !m_aStars[ i ].bStart ) {
				m_aStars[ i ].cPos.x = static_cast< GLfloat >( rand() % 1210 ) - 605.0f;
				m_aStars[ i ].cPos.y = 0.0f;
				m_aStars[ i ].cPos.z = MAX_GAME_DISTANCE;
				m_aStars[ i ].fSpeed = static_cast< GLfloat >( rand() % 140 ) + 140.0f;
				m_aStars[ i ].cColor.x = m_aStars[ i ].cColor.y = m_aStars[ i ].cColor.z = static_cast< GLfloat >( rand() % 40 + 20 ) / 100.0f;
				m_aStars[ i ].bStart = GL_TRUE;
				break;
			}
		}
	}

	//rysujemy
	for( GLint i = 0; i < iStarsSize; ++i ) {
		if( m_aStars[ i ].bStart ) {
			m_aStars[ i ].cPos.z += ( m_aStars[ i ].fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
			
			if( m_aStars[ i ].cPos.z > 0.0f )
				glPointSize( 3.0f );
			else if( m_aStars[ i ].cPos.z > -400.0f )
				glPointSize( 2.0f );
			else
				glPointSize( 1.0f );

			glColor3f( m_aStars[ i ].cColor.x, m_aStars[ i ].cColor.y, m_aStars[ i ].cColor.z );
			glBegin( GL_POINTS );
				glVertex3f( m_aStars[ i ].cPos.x, m_aStars[ i ].cPos.y, m_aStars[ i ].cPos.z );
			glEnd();

			if( m_aStars[ i ].cPos.z > MIN_GAME_DISTANCE )
				m_aStars[ i ].bStart = GL_FALSE;
		}
	}
}