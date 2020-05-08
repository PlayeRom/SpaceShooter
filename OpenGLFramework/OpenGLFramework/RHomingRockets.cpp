
#include "StdAfx.h"
#include "GameControl.h"
#include "RGame.h"
#include "RBonus.h"
#include "../Draw/3DObjManager.h"
#include "RHomingRockets.h"

CRHomingRockets::CRHomingRockets()
{
	m_pGameCtrl = CGameControl::GetInstance();

	// tworzenie tablicy rakiet
	SHomingRockets sHomingRockets;
	sHomingRockets.sShot.bDraw = GL_FALSE;
	sHomingRockets.sShot.fSpeed = 600.0f;//150.0f; //900.0f i predkosc statku wroga 260.0f - za szybko
	sHomingRockets.sShot.iTexture = -1;//m_pGameCtrl->m_iTexParticle01;//czasteczka
	sHomingRockets.sShot.i3DObjIndex = -1;
	sHomingRockets.sShot.eWeaponType = EWeaponHomingRocket;
	sHomingRockets.sShot.uiGroupIndex = 0;
	sHomingRockets.sShot.iModeFly = 0;
	for( GLint i = 0; i < MAX_HOMING_ROCKETS_ARRAY; ++i ) {
		sHomingRockets.iAnimIndex = m_pGameCtrl->GetSpeedCtrl()->CreateAnimationControl();
		m_aHomingRockets.push_back( sHomingRockets );
	}

	m_aIndices = new CVector3[ 2 ];
}

CRHomingRockets::~CRHomingRockets()
{
	m_aHomingRockets.clear();
	delete [] m_aIndices;
}

GLvoid CRHomingRockets::SetForStart()
{
	for( GLint i = 0; i < GetHomingRocketsSize(); ++i )
		m_aHomingRockets[ i ].sShot.bDraw = GL_FALSE;
}

GLvoid CRHomingRockets::AddPlayerShot_HomingRocket()
{
	//oblicz czestotliwosc wystrzalu zaleznie od gwiazdki dla uzbrojenia
	// czestotliwosc strzelania 2 razy mniejsza od lasera
	// sila rakiety 2 razy wieksza od lasera
	unsigned long ulMS = static_cast<unsigned long>( m_pGameCtrl->GetRGame()->GetPlayer()->GetStarsArmament() );
	switch( ulMS) {
		case 0:
			ulMS = 900 - m_pGameCtrl->GetRGame()->GetRBonus()->GetHomingRocketsRateShoot();
			break;
		case 1:
			ulMS = 770 - m_pGameCtrl->GetRGame()->GetRBonus()->GetHomingRocketsRateShoot();
			break;
		case 2:
			ulMS = 640 - m_pGameCtrl->GetRGame()->GetRBonus()->GetHomingRocketsRateShoot();
			break;
		case 3:
			ulMS = 510 - m_pGameCtrl->GetRGame()->GetRBonus()->GetHomingRocketsRateShoot();
			break;
	}
	if( !m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( ulMS, 2 ) )
		return;

	m_pGameCtrl->GetRGame()->GetRBonus()->ReduceAmmoHomingRockets( 1 );

	GLint iHowMuch = m_pGameCtrl->GetRGame()->GetRBonus()->GetHomingRocketsNumberShots(); //ile wystrzelic na raz

	for( GLint i = 0; i < GetHomingRocketsSize(); ++i ) {
		if( !m_aHomingRockets[ i ].sShot.bDraw ) {
			m_aHomingRockets[ i ].sShot.bDraw = GL_TRUE;
			m_aHomingRockets[ i ].cPos = m_pGameCtrl->GetRGame()->GetPlayer()->GetPosition();
			m_aHomingRockets[ i ].cPos.z -= m_pGameCtrl->GetRGame()->GetPlayer()->GetCollisionA();
			m_aHomingRockets[ i ].fStartPosX = 0.0f;
			m_aHomingRockets[ i ].iSmokeCounter = 0;
			m_aHomingRockets[ i ].sShot.iArmament = ( m_pGameCtrl->GetRGame()->GetPlayer()->GetArmamentActual() + m_pGameCtrl->GetRGame()->GetRBonus()->GetHomingRocketsPowerShoot() ) * 2;
			m_aHomingRockets[ i ].sShot.i3DObjIndex = -1;
			switch( m_pGameCtrl->GetRGame()->GetRBonus()->GetHomingRocketsNumberShots() ) {
				case 2:
					if( iHowMuch == 2 )
						m_aHomingRockets[ i ].sShot.iModeFly = 1;
					else if( iHowMuch == 1 )
						m_aHomingRockets[ i ].sShot.iModeFly = -1;
					break;
				case 3:
					if( iHowMuch == 2 )
						m_aHomingRockets[ i ].sShot.iModeFly = 1;
					else if( iHowMuch == 1 )
						m_aHomingRockets[ i ].sShot.iModeFly = -1;
					else
						m_aHomingRockets[ i ].sShot.iModeFly = 0;
					break;
				default: //1
					m_aHomingRockets[ i ].sShot.iModeFly = 0;
					break;
			}
			if( --iHowMuch == 0 )
				break;
		}
	}

	if( CWindowData::GetInstance()->SettingFile().bSound )
		COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundHomingRocket );
}

GLvoid CRHomingRockets::DrawHomingRocket()
{
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
	glEnable( GL_LIGHT2 );
	glDisable( GL_BLEND );
	m_pGameCtrl->Disable2D();

	for( GLint i = GetHomingRocketsSize() - 1; i >= 0; --i ) {
		if( !m_aHomingRockets[ i ].sShot.bDraw )
			continue;

		CVector4 cLightPosShadow;
		if( CWindowData::GetInstance()->SettingFile().bShadows ) {
			glLoadIdentity();
			//glScalef(-0.5f, -1.0f, -0.5f );
			glRotatef(-180.0f, 0.0f, 1.0f, 0.0f );
			glTranslatef( -m_aHomingRockets[ i ].cPos.x, -m_aHomingRockets[ i ].cPos.y, -m_aHomingRockets[ i ].cPos.z );
			m_pGameCtrl->GetRGame()->SetCamera();

			cLightPosShadow = m_pGameCtrl->GetStencilShadow()->GetLightPos( m_pGameCtrl->GetLighting()->GetPosition( GL_LIGHT2 ) );
		}

		glLoadIdentity();
		m_pGameCtrl->GetRGame()->SetCamera();
		glTranslatef( m_aHomingRockets[ i ].cPos.x, m_aHomingRockets[ i ].cPos.y, m_aHomingRockets[ i ].cPos.z );
		glRotatef( 180.0f, 0.0f, 1.0f, 0.0f );
		//glScalef( 0.5f, 1.0f, 0.5f );

		m_pGameCtrl->Get3DObjManager()->Draw3DObject_Lists( REMOTE_ROCKET_3DOBJ_INDEX, GL_FALSE );

		if( CWindowData::GetInstance()->SettingFile().bShadows )
			m_pGameCtrl->GetStencilShadow()->DrawShadowForObj( REMOTE_ROCKET_3DOBJ_INDEX, cLightPosShadow );

		m_aHomingRockets[ i ].cPos.z -= ( m_aHomingRockets[ i ].sShot.fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( m_aHomingRockets[ i ].cPos.z < MAX_GAME_DISTANCE )
			m_aHomingRockets[ i ].sShot.bDraw = GL_FALSE;

		if( m_aHomingRockets[ i ].fStartPosX < 0.25f && m_aHomingRockets[ i ].sShot.iModeFly != 0 ) {
			if( m_aHomingRockets[ i ].sShot.iModeFly == -1 )
				m_aHomingRockets[ i ].cPos.x -= ( ( m_aHomingRockets[ i ].sShot.fSpeed / 2.0f ) * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
			else if( m_aHomingRockets[ i ].sShot.iModeFly == 1 )
				m_aHomingRockets[ i ].cPos.x += ( ( m_aHomingRockets[ i ].sShot.fSpeed / 2.0f ) * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
			m_aHomingRockets[ i ].fStartPosX += m_pGameCtrl->GetSpeedCtrl()->GetMultiplier();
		}

		if( m_aHomingRockets[ i ].sShot.bDraw ) {
			GLint iSmokeLimit = 75;
			switch( m_pGameCtrl->GetRGame()->GetRBonus()->GetHomingRocketsNumberShots() ) {
				case 2:
					iSmokeLimit = 40;
					break;
				case 3:
					iSmokeLimit = 25;
					break;
			}
			if( m_aHomingRockets[ i ].iSmokeCounter < iSmokeLimit && m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 10, m_aHomingRockets[ i ].iAnimIndex ) ) {
				m_pGameCtrl->GetRGame()->GetUnivBillboard()->AddSmokeRemoteRocket( m_aHomingRockets[ i ].cPos );
				++m_aHomingRockets[ i ].iSmokeCounter;
			}

			//sprawdz czy rakieta nie zderzyla sie z jakims wrogim statkiem
			m_pGameCtrl->GetRGame()->CheckCollisionEnemyShot( m_aHomingRockets[ i ].sShot, m_aHomingRockets[ i ].cPos );
//#ifdef HOMING_ROCKETS
			if( m_aHomingRockets[ i ].sShot.bDraw ) {
				//samonaprowadzaj
				//znajdz pierwszy, najblizszy wrogi statek, od zadanej odleglosci od rakiety
				GLfloat fDistActual = -MAX_GAME_DISTANCE;
				GLint iHomingIndex = -1; //indeks statku na ktorego bedziemy sie naprowadzac
				
				CEnemyShip *objEnemyShip = m_pGameCtrl->GetRGame()->GetEnemyShip();
				for( GLint j = objEnemyShip->GetEnemyShipSize() - 1; j >= 0; --j ) {
					//naprowadza sie takze na wraki
					if( !objEnemyShip->GetEnemyShip( j ).bAlive || !objEnemyShip->GetEnemyShip( j ).bStart )
						continue;

					if( objEnemyShip->GetEnemyShip( j ).cActualPos.z >= m_aHomingRockets[ i ].cPos.z )
						continue;

					//sprawdz czy sie angazowac
//					os Z
//					^
//					|  x
//					+-----E
//					|    /
//					|   /
//				   d|  /20 stopni
//					| /
//					|/
//					R------------> os X
//					
//					R - rakieta lecaca ku gorze
//					E - enemy
//					d - odleglosc po os Z od rakiety do statku		
//					x - dludos ktora wyliczamy, dla zalozenia ze tak mieczy statkiem a rakieta wynosi 45 stopni

					//odleglosc po osi Z, miedzy rakieta a statkiem, gdyby statek byl pod katem 0 stopni
					GLfloat d = objEnemyShip->GetEnemyShip( j ).cActualPos.CalcDistanceZ( m_aHomingRockets[ i ].cPos );
					//odleglosc po osi x, miedzy rakieta a statkiem, gdyby statek byl pod katem 20 stopni
					GLfloat fDistXAngle = d * tan( DEG2RAD( 20.0f ) );

					//odleglosc po osi x, miedzy rakieta a statkiem, gdyby statek byl pod katem 90 stopni
					GLfloat fDistXReal = objEnemyShip->GetEnemyShip( j ).cActualPos.CalcDistanceX( m_aHomingRockets[ i ].cPos );
					if( fDistXReal >= fDistXAngle )
						continue;

					//oblicz odleglosc aby alokowac sie na najblizszy cel
					GLfloat fDistance = objEnemyShip->GetEnemyShip( j ).cActualPos.CalcDistanceXZ( m_aHomingRockets[ i ].cPos );
					if( fDistance < fDistActual ) {
						fDistActual = fDistance;
						iHomingIndex = j;
					}
				}
				if( iHomingIndex >= 0 ) {
					//objEnemyShip->GetEnemyShip( iHomingIndex ).iEngageHomingRocket = i;
					if( m_pGameCtrl->GetRGame()->GetRBonus()->GetNumberViewFinder() >= 1 ) {
						glLoadIdentity();
						glDisable( GL_LIGHTING );
						glDisable( GL_TEXTURE_2D );
						m_pGameCtrl->GetRGame()->SetCamera();
						GLint iIndexColor = CWindowData::GetInstance()->SettingFile().iHudColor;
						glColor3f( m_pGameCtrl->GetRGame()->GetHudColor( iIndexColor ).x,
								   m_pGameCtrl->GetRGame()->GetHudColor( iIndexColor ).y,
								   m_pGameCtrl->GetRGame()->GetHudColor( iIndexColor ).z );
						m_pGameCtrl->GetRGame()->DrawEngageSquare( objEnemyShip->GetEnemyShip( iHomingIndex ).cActualPos );
					}
					//
					if( m_pGameCtrl->GetRGame()->GetRBonus()->GetNumberViewFinder() >= 4 ) {
						//dla co najmniej 5 zchwytanych znajdziek viewfindef rysuj linie laczaca rakiete ze statkiem
						m_aIndices[ 0 ] = m_aHomingRockets[ i ].cPos;
						m_aIndices[ 1 ] = objEnemyShip->GetEnemyShip( iHomingIndex ).cActualPos;

						glEnableClientState( GL_VERTEX_ARRAY );
						glDisableClientState( GL_NORMAL_ARRAY );
						glDisableClientState( GL_COLOR_ARRAY );
						glDisableClientState( GL_TEXTURE_COORD_ARRAY );

						glVertexPointer( 3, GL_FLOAT, 0, m_aIndices );
						glDrawArrays( GL_LINES, 0, 2 );
					}
					//
					glColor3f( 1.0f, 1.0f, 1.0f );
					glEnable( GL_LIGHTING );
					glEnable( GL_TEXTURE_2D );
					
					if( objEnemyShip->GetEnemyShip( iHomingIndex ).cActualPos.x < m_aHomingRockets[ i ].cPos.x ) {
						//przesun w lewo
						m_aHomingRockets[ i ].cPos.x -= ( ( m_aHomingRockets[ i ].sShot.fSpeed / 5.0f ) * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
						if( m_aHomingRockets[ i ].cPos.x < objEnemyShip->GetEnemyShip( iHomingIndex ).cActualPos.x )
							m_aHomingRockets[ i ].cPos.x = objEnemyShip->GetEnemyShip( iHomingIndex ).cActualPos.x;
					}
					else if( objEnemyShip->GetEnemyShip( iHomingIndex ).cActualPos.x > m_aHomingRockets[ i ].cPos.x ) {
						//przesun w prawo
						m_aHomingRockets[ i ].cPos.x += ( ( m_aHomingRockets[ i ].sShot.fSpeed / 5.0f ) * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
						if( m_aHomingRockets[ i ].cPos.x > objEnemyShip->GetEnemyShip( iHomingIndex ).cActualPos.x )
							m_aHomingRockets[ i ].cPos.x = objEnemyShip->GetEnemyShip( iHomingIndex ).cActualPos.x;
					}
				}
			}
//#endif
		}
	}

	glEnable( GL_LIGHT0 );
	glDisable( GL_LIGHT2 );
}