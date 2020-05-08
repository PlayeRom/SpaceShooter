#include "StdAfx.h"
#include "../Draw/3DObjManager.h"
#include "../Draw/StencilShadow.h"
#include "WindowData.h"
#include "GameControl.h"
#include "RGame.h"
#include "RShockWaveEffect.h"
#include "RBonus.h"
#include "RRemoteRocket.h"

CRRemoteRocket::CRRemoteRocket()
{
	m_pGameCtrl = CGameControl::GetInstance();
}

CRRemoteRocket::~CRRemoteRocket()
{
}

GLvoid CRRemoteRocket::ServiceEnterKey()
{
	if( m_bBlockRemoteRocket ) //blokada po kolizji zdalnej rakiery z wrogiem
		return;

	if( m_pGameCtrl->GetRGame()->GetRBonus()->GetRemoteRocketsNumber() > 0 && !m_bIsDraw ) {
		//wystrzel rakiete zdalna
		m_bIsDraw = GL_TRUE;
		m_cPosition = m_pGameCtrl->GetRGame()->GetPlayer()->GetPosition();
		m_cPosition.z -= m_pGameCtrl->GetRGame()->GetPlayer()->GetCollisionA();
		m_fSpeed = 400.0f;
		m_pGameCtrl->GetRGame()->GetRBonus()->ReduceRemoteRocketsNumber();

		if( CWindowData::GetInstance()->SettingFile().bSound )
			COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundRemoteRocket );
	}
	else if( m_bIsDraw ) {
		//rakietka leci, wiec detonuj
		DetonationRemoteRocket();

		if( CWindowData::GetInstance()->SettingFile().bSound )
			COpenALManager::GetInstance()->Stop( m_pGameCtrl->m_iSoundRemoteRocket );
	}
}

//dla blokowania klawisza enter gdy rakieta zdalna zderzyla sie z wrogiem, TODO: 32
GLvoid CRRemoteRocket::BlockEnterKey()
{
	if( m_bBlockRemoteRocket ) {
		if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 1000, 13 ) )
			m_bBlockRemoteRocket = GL_FALSE; //odblokuj eneter
	}
	else
		m_pGameCtrl->GetSpeedCtrl()->UpdateAnimationTime( 13 );
}

GLvoid CRRemoteRocket::DrawFlyRemoteRocket()
{
	if( !m_bIsDraw )
		return;

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
	glEnable( GL_LIGHT2 );
	glDisable( GL_BLEND );
	m_pGameCtrl->Disable2D();

	CVector4 cLightPosShadow;
	if( CWindowData::GetInstance()->SettingFile().bShadows ) {
		glLoadIdentity();
		glRotatef( -180.0f, 0.0f, 1.0f, 0.0f );
		glTranslatef( -m_cPosition.x, -m_cPosition.y, -m_cPosition.z );
		m_pGameCtrl->GetRGame()->SetCamera();

		cLightPosShadow = m_pGameCtrl->GetStencilShadow()->GetLightPos( m_pGameCtrl->GetLighting()->GetPosition( GL_LIGHT2 ) );
	}

	glLoadIdentity();
	m_pGameCtrl->GetRGame()->SetCamera();
	glTranslatef( m_cPosition.x, m_cPosition.y, m_cPosition.z );
	glRotatef( 180.0f, 0.0f, 1.0f, 0.0f );

	m_pGameCtrl->Get3DObjManager()->Draw3DObject_Lists( REMOTE_ROCKET_3DOBJ_INDEX, GL_FALSE );

	if( CWindowData::GetInstance()->SettingFile().bShadows )
		m_pGameCtrl->GetStencilShadow()->DrawShadowForObj( REMOTE_ROCKET_3DOBJ_INDEX, cLightPosShadow );

	m_cPosition.z -= ( m_fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	if( m_cPosition.z < MAX_GAME_DISTANCE )
		m_bIsDraw = GL_FALSE;

	glEnable( GL_LIGHT0 );
	glDisable( GL_LIGHT2 );

	if( m_bIsDraw ) {
		if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 10, 10 ) )
			m_pGameCtrl->GetRGame()->GetUnivBillboard()->AddSmokeRemoteRocket( m_cPosition );

		//sprawdz czy rakieta nie zderzyla sie z jakims wrogim statkiem
		SShot sShot;
		sShot.bDraw = GL_TRUE;
		sShot.iArmament = REMOTE_ROCKET_POWER;
		sShot.i3DObjIndex = -1;
		sShot.eWeaponType = EWeaponRemoteRocket;
		m_pGameCtrl->GetRGame()->CheckCollisionEnemyShot( sShot, m_cPosition );
	}
}

GLvoid CRRemoteRocket::DetonationRemoteRocket( GLboolean bManualDetonation /*= GL_TRUE*/ )
{
	/*
	bManualDetonation = GL_TRUE gdy gracz detonowal recznie, GL_FALSE - gdy rakieta zderzyla sie z wrogiem
	*/
	if( !m_bIsDraw )
		return; //zabezspieczenie lekko na wyrost

	//jezeli m_bBlockRemoteRocket jest GL_TRUE to blokujemy klawisz enter na ktorki czas, TODO: 32 
	m_bBlockRemoteRocket = !bManualDetonation;

	m_pGameCtrl->GetRGame()->AddExplosion( m_cPosition, 40.0f );
	//fala uderzeniowa
	m_pGameCtrl->GetRGame()->GetRShockWaveEffect()->DoDrawShockWave( m_cPosition );

	CheckCollisionEnemyRemoteRocket( m_cPosition );
	m_bIsDraw = GL_FALSE;
}

GLvoid CRRemoteRocket::CheckCollisionEnemyRemoteRocket( const CVector3 &in_cPosExpl )
{
	//sprawdz czy dany statek znajduje sie w obrebie kola wybuchu
	for( GLint i = m_pGameCtrl->GetRGame()->GetEnemyShip()->GetEnemyShipSize() - 1; i >= 0; --i ) {
		if( !m_pGameCtrl->GetRGame()->GetEnemyShip()->GetEnemyShip( i ).bAlive
			|| !m_pGameCtrl->GetRGame()->GetEnemyShip()->GetEnemyShip( i ).bStart )
			continue;

		if( !IsCollisionEnemyRemoteRocket( in_cPosExpl, m_pGameCtrl->GetRGame()->GetEnemyShip()->GetEnemyShip( i ).cActualPos ) )
			continue;

		//kolizja - enemy eksploduje
		SShot sShot = { 0 };
		sShot.iArmament = REMOTE_ROCKET_POWER; //najwazniejsze - sila bomby jebutki
		sShot.eWeaponType = EWeaponRemoteRocket;
		CVector3 cVec = m_pGameCtrl->GetRGame()->GetEnemyShip()->GetEnemyShip( i ).cActualPos;
		m_pGameCtrl->GetRGame()->CalculateEnemyDestroy(
			m_pGameCtrl->GetRGame()->GetEnemyShip()->GetEnemyShip( i ), sShot, cVec );
	}

	//sprawdz czy statek garcza jest w zasiegu razenia
	if( IsCollisionEnemyRemoteRocket( in_cPosExpl, m_pGameCtrl->GetRGame()->GetPlayer()->GetPosition() ) ) {
		SShot sShot = { 0 };
		sShot.iArmament = REMOTE_ROCKET_POWER;//najwazniejsze - sila bomby jebutki
		sShot.eWeaponType = EWeaponRemoteRocket;
		m_pGameCtrl->GetRGame()->CalculatePlayerDestroy( sShot );
	}
}

GLboolean CRRemoteRocket::IsCollisionEnemyRemoteRocket( CVector3 in_cPosExpl, const CVector3 &in_cPosShip )
{
	//sprawdz czy dany statek znajduje sie w obrebie kola wybuchu
	if( in_cPosExpl.CalcDistanceXZ( in_cPosShip ) > 242.0f )
		return GL_FALSE; //statek poza zasiegiem

	return GL_TRUE;
}