
#include "StdAfx.h"
#include "GameControl.h"
#include "RGame.h"
#include "RBonus.h"
#include "../Draw/Particles.h"
#include "REmitterRockets.h"

CREmitterRockets::CREmitterRockets()
{
	m_pGameCtrl = CGameControl::GetInstance();

	// tworzenie tablicy rakiet
	SEmitterRocket sEmitterRockets;
	sEmitterRockets.cPos = CVector3();
	sEmitterRockets.sShot.bDraw = GL_FALSE;
	sEmitterRockets.sShot.fSpeed = 750.0f;//150.0f; //900.0f i predkosc statku wroga 260.0f - za szybko
	sEmitterRockets.sShot.iTexture = -1;//m_pGameCtrl->m_iTexParticle01;//czasteczka
	sEmitterRockets.sShot.i3DObjIndex = -1;
	sEmitterRockets.sShot.eWeaponType = EWeaponEmitterRocket;
	sEmitterRockets.sShot.uiGroupIndex = 0;
	sEmitterRockets.sShot.iModeFly = 0;
	for( GLint i = 0; i < MAX_EMITTER_ROCKETS_ARRAY; ++i ) {
		sEmitterRockets.iIndexOfEmitter = m_pGameCtrl->GetParticles()->CreateEmitter(
				//        z       y      x
				CVector3( 0.0f, 0.0f, 0.0f ),
				m_pGameCtrl->m_iTexParticle01, //textura
				CVector3( 0.0f, 0.0f, 0.0f ),
				CVector3( 0.0f, 0.0f, 0.0f ),
				1.0f, //scale
				2.0f, //life
				GL_TRUE, //czasteczka po wygasnieciu moze ozywac
				1.0f, //r
				1.0f, //g
				0.1f, //b
				-200.0f, //speedX
				0.0f, //speedY
				0.0f, //speedZ
				25, //ilosc czastek
				100.0f,	//fMoveSpeed
				200		//iRandSpeedRange1
			);
		sEmitterRockets.iIndexOfEmitter2 = m_pGameCtrl->GetParticles()->CreateEmitter(
				//        z       y      x
				CVector3( 0.0f, 0.0f, 0.0f ),
				m_pGameCtrl->m_iTexParticle01, //textura
				CVector3( 0.0f, 0.0f, 0.0f ),
				CVector3( 0.0f, 0.0f, 0.0f ),
				1.0f, //scale
				2.0f, //life
				GL_TRUE, //czasteczka po wygasnieciu moze ozywac
				1.0f, //r
				0.1f, //g
				0.1f, //b
				-200.0f, //speedX
				0.0f, //speedY
				0.0f, //speedZ
				25, //ilosc czastek
				100.0f,	//fMoveSpeed
				200		//iRandSpeedRange1
			);
		m_aEmitterRockets.push_back( sEmitterRockets );
	}
}

CREmitterRockets::~CREmitterRockets()
{
	m_aEmitterRockets.clear();
}

GLvoid CREmitterRockets::SetForStart()
{
	for( GLint i = 0; i < GetEmitterRocketsSize(); ++i )
		m_aEmitterRockets[ i ].sShot.bDraw = GL_FALSE;
}

GLvoid CREmitterRockets::AddPlayerShot_EmitterRocket()
{
	//oblicz czestotliwosc wystrzalu zaleznie od gwiazdki dla uzbrojenia
	// czestotliwosc strzelania 2 razy mniejsza od lasera
	// sila rakiety 2 razy wieksza od lasera
	unsigned long ulMS = static_cast<unsigned long>( m_pGameCtrl->GetRGame()->GetPlayer()->GetStarsArmament() );
	switch( ulMS) {
		case 0:
			ulMS = 450 - m_pGameCtrl->GetRGame()->GetRBonus()->GetEmitterRocketsRateShoot();
			break;
		case 1:
			ulMS = 385 - m_pGameCtrl->GetRGame()->GetRBonus()->GetEmitterRocketsRateShoot();
			break;
		case 2:
			ulMS = 320 - m_pGameCtrl->GetRGame()->GetRBonus()->GetEmitterRocketsRateShoot();
			break;
		case 3:
			ulMS = 255 - m_pGameCtrl->GetRGame()->GetRBonus()->GetEmitterRocketsRateShoot();
			break;
	}
	if( !m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( ulMS, 2 ) )
		return;

	m_pGameCtrl->GetRGame()->GetRBonus()->ReduceAmmoEmitterRockets( 1 );

	GLint iHowMuch = m_pGameCtrl->GetRGame()->GetRBonus()->GetEmitterRocketsNumberShots(); //ile wystrzelic na raz

	for( GLint i = 0; i < GetEmitterRocketsSize(); ++i ) {
		if( !m_aEmitterRockets[ i ].sShot.bDraw ) {
			m_aEmitterRockets[ i ].fCosActualAngle = 0.0f;
			m_aEmitterRockets[ i ].bCosSign = ( GLboolean )rand() % 2;
			m_aEmitterRockets[ i ].sShot.bDraw = GL_TRUE;
			m_aEmitterRockets[ i ].cPos = m_pGameCtrl->GetRGame()->GetPlayer()->GetPosition();
			m_aEmitterRockets[ i ].cPos.z -= m_pGameCtrl->GetRGame()->GetPlayer()->GetCollisionA();
			m_aEmitterRockets[ i ].sShot.iArmament = ( m_pGameCtrl->GetRGame()->GetPlayer()->GetArmamentActual() + m_pGameCtrl->GetRGame()->GetRBonus()->GetEmitterRocketsPowerShoot() );
			m_aEmitterRockets[ i ].sShot.i3DObjIndex = -1;
			m_aEmitterRockets[ i ].sShot.iModeFly = 0;
			switch( m_pGameCtrl->GetRGame()->GetRBonus()->GetEmitterRocketsNumberShots() ) {
				case 2:
					if( iHowMuch == 2 )
						m_aEmitterRockets[ i ].cPos.x += 5.0f;
					else if( iHowMuch == 1 )
						m_aEmitterRockets[ i ].cPos.x -= 5.0f;
					break;
				case 3:
					if( iHowMuch == 2 )
						m_aEmitterRockets[ i ].sShot.iModeFly = 1;
					else if( iHowMuch == 1 )
						m_aEmitterRockets[ i ].sShot.iModeFly = -1;
					else
						m_aEmitterRockets[ i ].sShot.iModeFly = 0;
					break;
				default: //1
					m_aEmitterRockets[ i ].sShot.iModeFly = 0;
					break;
			}
			if( --iHowMuch == 0 )
				break;
		}
	}

	if( CWindowData::GetInstance()->SettingFile().bSound )
		COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundHomingRocket );
}

GLvoid CREmitterRockets::DrawEmitterRocket()
{
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
	glEnable( GL_LIGHT2 );
	glDisable( GL_BLEND );
	m_pGameCtrl->Disable2D();

	for( GLint i = GetEmitterRocketsSize() - 1; i >= 0; --i ) {
		if( !m_aEmitterRockets[ i ].sShot.bDraw )
			continue;

		glLoadIdentity();
		m_pGameCtrl->GetRGame()->SetCamera();
		glTranslatef( m_aEmitterRockets[ i ].cPos.x, m_aEmitterRockets[ i ].cPos.y, m_aEmitterRockets[ i ].cPos.z );
		glRotatef( 180.0f, 0.0f, 1.0f, 0.0f );
		//glScalef( 0.5f, 1.0f, 0.5f );

		m_pGameCtrl->GetParticles()->DrawEmitter( m_aEmitterRockets[ i ].iIndexOfEmitter );
		m_pGameCtrl->GetParticles()->DrawEmitter( m_aEmitterRockets[ i ].iIndexOfEmitter2 );

		m_aEmitterRockets[ i ].fCosActualAngle += ( 2000.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );

		if( m_aEmitterRockets[ i ].bCosSign )
			m_aEmitterRockets[ i ].cPos.x += static_cast< GLfloat >( cos( DEG2RAD( m_aEmitterRockets[ i ].fCosActualAngle ) ) ) * ( m_aEmitterRockets[ i ].sShot.fSpeed / 2.0f ) * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier();
		else
			m_aEmitterRockets[ i ].cPos.x -= static_cast< GLfloat >( cos( DEG2RAD( m_aEmitterRockets[ i ].fCosActualAngle ) ) ) * ( m_aEmitterRockets[ i ].sShot.fSpeed / 2.0f ) * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier();

		m_aEmitterRockets[ i ].cPos.z -= ( m_aEmitterRockets[ i ].sShot.fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( m_aEmitterRockets[ i ].cPos.z < MAX_GAME_DISTANCE )
			m_aEmitterRockets[ i ].sShot.bDraw = GL_FALSE;

		GLfloat fDivision = static_cast< GLfloat >( m_aEmitterRockets[ i ].sShot.iModeFly );
		if( fDivision < 0 )
			fDivision = -fDivision;
		fDivision = 20.0f - ( fDivision * 1.25f );
		if( m_aEmitterRockets[ i ].sShot.iModeFly < 0 )
			m_aEmitterRockets[ i ].cPos.x -= ( ( m_aEmitterRockets[ i ].sShot.fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() ) / fDivision );
		else if( m_aEmitterRockets[ i ].sShot.iModeFly > 0 )
			m_aEmitterRockets[ i ].cPos.x += ( ( m_aEmitterRockets[ i ].sShot.fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() ) / fDivision );

		if( m_aEmitterRockets[ i ].sShot.bDraw ) {
			//sprawdz czy rakieta nie zderzyla sie z jakims wrogim statkiem
			m_pGameCtrl->GetRGame()->CheckCollisionEnemyShot( m_aEmitterRockets[ i ].sShot, m_aEmitterRockets[ i ].cPos );
		}
	}

	glEnable( GL_LIGHT0 );
	glDisable( GL_LIGHT2 );
}