#include "StdAfx.h"
#include "GameControl.h"
#include "ShipsManager.h"
#include "RPlayer.h"
#include "RDifficultyLevel.h"
#include "RBonus.h"

CRBonus::CRBonus()
{
	ResetAllValue();
	m_pGameCtrl = CGameControl::GetInstance();
}

CRBonus::~CRBonus()
{
}

GLvoid CRBonus::ResetAllValue()
{
	m_uiLaserRateShoot = 0;
	m_iLaserPowerShoot = 0;
	m_iLaserNumberShots = 1;
	m_iChaingunPowerShoot = 0;
	m_uiChaingunAmmo = 0;
	m_iChaingunNumberShots = 0;
	m_iImpulseLaserPowerShoot = 0;
	m_uiImpulseLaserAmmo = 0;
	m_iImpulseLaserNumberShots = 0;
	m_uiEmitterRocketsRateShoot = 0;
	m_iEmitterRocketsPowerShoot = 0;
	m_uiEmitterRocketsAmmo = 0;
	m_iEmitterRocketsNumberShots = 0;
	m_uiHomingRocketsRateShoot = 0;
	m_iHomingRocketsPowerShoot = 0;
	m_uiHomingRocketsAmmo = 0;
	m_iHomingRocketsNumberShots = 0;

	m_iNumberViewFinder = 0;
	m_iSlowEnemiesSeconds = 0;
	m_iRemoteRocketsNumber = 2;

	m_iCounterGeneratedBonuses = 0;
	m_bWasForceWeaponLaserBonus = GL_FALSE;
	m_iCounterDrawBonuses = 0;
}

GLvoid CRBonus::UpgradeWeaponLaser()
{
	m_uiLaserRateShoot += LASER_RATE_SHOOT;
	if( m_uiLaserRateShoot > 155 )
		m_uiLaserRateShoot = 155;

	m_iLaserPowerShoot += LASER_POWER_SHOOT; 
	if( m_iLaserPowerShoot > LASER_POWER_SHOOT * 10 )
		m_iLaserPowerShoot = LASER_POWER_SHOOT * 10;

	if( ++m_iLaserNumberShots > 7 )
		m_iLaserNumberShots = 7;
}

GLvoid CRBonus::UpgradeWeaponChaingun()
{
	m_iChaingunPowerShoot += CHAINGUN_POWER_SHOOT; 
	if( m_iChaingunPowerShoot > CHAINGUN_POWER_SHOOT * 100 )
		m_iChaingunPowerShoot = CHAINGUN_POWER_SHOOT * 100;

	m_uiChaingunAmmo += CHAINGUN_AMMO;

	if( ++m_iChaingunNumberShots > 4 )
		m_iChaingunNumberShots = 4;
}

GLvoid CRBonus::UpgradeWeaponImpulseLaser()
{
	m_iImpulseLaserPowerShoot += IMPULSE_LASER_POWER_SHOOT; 
	if( m_iImpulseLaserPowerShoot > IMPULSE_LASER_POWER_SHOOT * 50 )
		m_iImpulseLaserPowerShoot = IMPULSE_LASER_POWER_SHOOT * 50;

	m_uiImpulseLaserAmmo += IMPULSE_LASER_AMMO;

	if( ++m_iImpulseLaserNumberShots > 3 )
		m_iImpulseLaserNumberShots = 3;
}

GLvoid CRBonus::UpgradeWeaponEmitterRockets()
{
	m_uiEmitterRocketsRateShoot += EMITTER_ROCKETS_RATE_SHOOT;
	if( m_uiEmitterRocketsRateShoot > 155 )
		m_uiEmitterRocketsRateShoot = 155;

	m_iEmitterRocketsPowerShoot += EMITTER_ROCKETS_POWER_SHOOT; 
	if( m_iEmitterRocketsPowerShoot > EMITTER_ROCKETS_POWER_SHOOT * 10 )
		m_iEmitterRocketsPowerShoot = EMITTER_ROCKETS_POWER_SHOOT * 10;

	m_uiEmitterRocketsAmmo += EMITTER_ROCKETS_AMMO;

	if( ++m_iEmitterRocketsNumberShots > 3 )
		m_iEmitterRocketsNumberShots = 3;
}

GLvoid CRBonus::UpgradeWeaponHomingRockets()
{
	m_uiHomingRocketsRateShoot += HOMING_ROCKETS_RATE_SHOOT;
	if( m_uiHomingRocketsRateShoot > 155 )
		m_uiHomingRocketsRateShoot = 155;

	m_iHomingRocketsPowerShoot += HOMING_ROCKETS_POWER_SHOOT; 
	if( m_iHomingRocketsPowerShoot > HOMING_ROCKETS_POWER_SHOOT * 10 )
		m_iHomingRocketsPowerShoot = HOMING_ROCKETS_POWER_SHOOT * 10;

	m_uiHomingRocketsAmmo += HOMING_ROCKETS_AMMO;

	if( ++m_iHomingRocketsNumberShots > 3 )
		m_iHomingRocketsNumberShots = 3;
}

////////////////////////// reduce ///////////////////////////////////////

GLvoid CRBonus::ReduceWeaponLaser()
{
	if( m_uiLaserRateShoot >= LASER_RATE_SHOOT )
		m_uiLaserRateShoot -= LASER_RATE_SHOOT;

	if( m_iLaserPowerShoot >= LASER_POWER_SHOOT )
		m_iLaserPowerShoot -= LASER_POWER_SHOOT;

	if( m_iLaserNumberShots > 1 )
		--m_iLaserNumberShots;
}

GLvoid CRBonus::ReduceWeaponChaingun()
{
	if( m_iChaingunPowerShoot >= CHAINGUN_POWER_SHOOT )
		m_iChaingunPowerShoot -= CHAINGUN_POWER_SHOOT;

	if( m_uiChaingunAmmo >= CHAINGUN_AMMO )
		m_uiChaingunAmmo -= CHAINGUN_AMMO;
	else m_uiChaingunAmmo = 0;

	if( m_iChaingunNumberShots > 1 )
		--m_iChaingunNumberShots;
}

GLvoid CRBonus::ReduceWeaponImpulseLaser()
{
	if( m_iImpulseLaserPowerShoot >= IMPULSE_LASER_POWER_SHOOT )
		m_iImpulseLaserPowerShoot -= IMPULSE_LASER_POWER_SHOOT;

	if( m_uiImpulseLaserAmmo >= IMPULSE_LASER_AMMO )
		m_uiImpulseLaserAmmo -= IMPULSE_LASER_AMMO;
	else m_uiImpulseLaserAmmo = 0;

	if( m_iImpulseLaserNumberShots > 1 )
		--m_iImpulseLaserNumberShots;
}

GLvoid CRBonus::ReduceWeaponEmitterRockets()
{
	if( m_uiEmitterRocketsRateShoot >= EMITTER_ROCKETS_RATE_SHOOT )
		m_uiEmitterRocketsRateShoot -= EMITTER_ROCKETS_RATE_SHOOT;

	if( m_iEmitterRocketsPowerShoot >= EMITTER_ROCKETS_POWER_SHOOT )
		m_iEmitterRocketsPowerShoot -= EMITTER_ROCKETS_POWER_SHOOT;

	if( m_uiEmitterRocketsAmmo >= EMITTER_ROCKETS_AMMO )
		m_uiEmitterRocketsAmmo -= EMITTER_ROCKETS_AMMO;
	else m_uiEmitterRocketsAmmo = 0;

	if( m_iEmitterRocketsNumberShots > 1 )
		--m_iEmitterRocketsNumberShots;
}

GLvoid CRBonus::ReduceWeaponHomingRockets()
{
	if( m_uiHomingRocketsRateShoot >= HOMING_ROCKETS_RATE_SHOOT )
		m_uiHomingRocketsRateShoot -= HOMING_ROCKETS_RATE_SHOOT;

	if( m_iHomingRocketsPowerShoot >= HOMING_ROCKETS_POWER_SHOOT )
		m_iHomingRocketsPowerShoot -= HOMING_ROCKETS_POWER_SHOOT;

	if( m_uiHomingRocketsAmmo >= HOMING_ROCKETS_AMMO )
		m_uiHomingRocketsAmmo -= HOMING_ROCKETS_AMMO;
	else m_uiHomingRocketsAmmo = 0;

	if( m_iHomingRocketsNumberShots > 1 )
		--m_iHomingRocketsNumberShots;
}

/////////////////////////////////////////////////////////////////////////

GLvoid CRBonus::CalculateSlowEnemiesSeconds()
{
	if( m_iSlowEnemiesSeconds > 0 ) {
		if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 1000, 9 ) )
			--m_iSlowEnemiesSeconds;
	}
	else
		m_pGameCtrl->GetSpeedCtrl()->UpdateAnimationTime( 9 );
}

//funkcja wywolywana za kazdym razem jak zniszczony zostanie wrogi obiekt
GLvoid CRBonus::GenerateBonus( const CVector3 &in_cPos )
{
	//jezli bonus ma sie pojawic poza obszarem gracza to nie generuj bonusa
	if( in_cPos.x < PLAYER_ZONE_LEFT_X || in_cPos.x > PLAYER_ZONE_RIGHT_X )
		return;

	//jezeli rysowana juz sa conalmniej 2 bonusy to return
	if( m_iCounterDrawBonuses >= 2 )
		return;

	//jezeli przekroczony limit znajdziek na poziom to return
	GLint iLimit;
	switch( m_pGameCtrl->GetRDifficultyLevel()->GetDifficultyLevel() ) {
		case ELevelEasy:
			iLimit = 6;
			break;

		case ELevelHard:
			iLimit = 4;
			break;

		default: //ELevelNormal
			iLimit = 5;
			break;
	}
	if( m_iCounterGeneratedBonuses >= iLimit )
		return;

	//jezeli gracz gra Needle i ma bron podstawowa na poziomie 1 to zwieksz szanse znajdzki i daj mu bron
	//pod warunkiem ze bron znajdzki lasera nie jest juz na planszy
	GLboolean bForceWeaponLaserBonus = GL_FALSE;
	if( m_pGameCtrl->GetRGame()->GetPlayer()->Get3DObjectIndex() == NEEDLE_3DOBJ_INDEX 
		&& GetLaserNumberShots() == 1
		&& !m_bWasForceWeaponLaserBonus )
	{
		bForceWeaponLaserBonus = GL_TRUE;
	}

	//losujemy czy w ogole bedzie bonus
	GLint iBonusRand = 98; //2.0%
	GLint iPercent = 100;
	switch( m_pGameCtrl->GetRDifficultyLevel()->GetDifficultyLevel() ) {
		case ELevelEasy:
			iBonusRand = 94; //6%
			if( bForceWeaponLaserBonus )
				iBonusRand = 50; //50.0%
			break;
		case ELevelHard:
			iBonusRand = 98; //2%
			break;
		default: //ELevelNormal
			iBonusRand = 96; //4%
			break;
	}
	if( rand() % iPercent < iBonusRand )
		return;

	SUniversalBillboard sUniversal;
	sUniversal.eType = EUBTypeBonus;
	sUniversal.cPos = in_cPos;
	if( bForceWeaponLaserBonus ) {
		sUniversal.eBonusType = EBonusWeaponLaser;
		m_bWasForceWeaponLaserBonus = GL_TRUE;
	}
	else {
		if( m_pGameCtrl->GetRGame()->GetPlayer()->GetLives() == 3 )
			//generuj bonus ale bez zycia
			sUniversal.eBonusType = static_cast<EBonusType>( rand() % ( EBonusLastElement - 1 ) );
		else //generuj ze wszytskich bonusow
			sUniversal.eBonusType = static_cast<EBonusType>( rand() % EBonusLastElement );
	}
	m_pGameCtrl->GetRGame()->GetUnivBillboard()->AddUniversalBillboard( sUniversal );
	++m_iCounterGeneratedBonuses; //reset co nowy poziom
	++m_iCounterDrawBonuses;
}

GLvoid CRBonus::DrawBonus( SUniversalBillboard &in_sUB, GLint iIndex )
{
	/*
	//rysujemy 1 bonus
	iIndex - indeks in_sUB
	*/

	glPushMatrix();
	glTranslatef( in_sUB.cPos.x, in_sUB.cPos.y, in_sUB.cPos.z );

	GLfloat fScale = 5.0f;
	//TODO: ustaw odpowiednia teksture wg typu znajdzki
	switch( in_sUB.eBonusType ) {
		case EBonusWeaponLaser:
		case EBonusWeaponChaingun:
		case EBonusWeaponImpulseLaser:
		case EBonusWeaponEmitterRockets:
		case EBonusWeaponHomingRockets:
			fScale = 7.5f;
			m_pGameCtrl->GetTexLoader()->SetTexture( m_pGameCtrl->m_aTexWeaponSelect[ in_sUB.eBonusType ] );
			break;

		case EBonusLife:
			fScale = 10.0f;
			m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_pGameCtrl->GetRGame()->GetPlayer()->Get3DObjectIndex() ).iTexLife );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); 
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			break;

		case EBonusShield:
			m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexBonusShield );
			break;

		case EBonusViewFinder:
			m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexBonusViewfinder );
			break;

		case EBonusIndestructibility:
			m_pGameCtrl->GetTexLoader()->SetTexture( 0 );
			break;

		case EBonusSlowdownEnemy:
			m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexBonusEnemySlowdown );
			break;

		case EBonusRemoteRocket:
			fScale = 10.0f;
			m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexBonusRemoteRocket );
			break;
	}

	m_pGameCtrl->GetBillboard()->DrawQuadBillboard( CVector3(), fScale );

	//przesuwamy bonus
	in_sUB.cPos.z += ( PLAYER_FLY_SPEED * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	if( in_sUB.cPos.z > MIN_GAME_DISTANCE ) {
		m_pGameCtrl->GetRGame()->GetUnivBillboard()->DeleteUniversalBillboard( iIndex );
		--m_iCounterDrawBonuses;
	}
	else {
		if( m_pGameCtrl->GetRGame()->CheckCollisionPlayerBonus( in_sUB.cPos, in_sUB.eBonusType, iIndex ) )
			--m_iCounterDrawBonuses;
	}
	
	glPopMatrix();
}

GLuint CRBonus::GetAmmoFromActualWeapon( EWeaponType eWeaponType )
{
	GLuint uiAmmo = 0;
	switch( eWeaponType ) {
		case EWeaponChaingun:
			uiAmmo = GetChaingunAmmo();
			break;

		case EWeaponImpulseLaser:
			uiAmmo = GetImpulseLaserAmmo();
			break;

		case EWeaponEmitterRocket:
			uiAmmo = GetEmitterRocketsAmmo();
			break;

		case EWeaponHomingRocket:
			uiAmmo = GetHomingRocketsAmmo();
			break;
	}

	//return 0 jezeli brak ammo lub ammo w nieskonczonosci
	return uiAmmo;
}

GLboolean CRBonus::IsAvailableWeapon( EWeaponType eWeaponType )
{
	GLboolean bResult = GL_TRUE;
	switch( eWeaponType ) {
		case EWeaponLaser:
			// pozostaw GL_TRUE
			break;

		case EWeaponChaingun:
			if( GetChaingunPowerShoot() == 0 )
				bResult = GL_FALSE;
			break;

		case EWeaponImpulseLaser:
			if( GetImpulseLaserPowerShoot() == 0 )
				bResult = GL_FALSE;
			break;

		case EWeaponEmitterRocket:
			if( GetEmitterRocketsPowerShoot() == 0 )
				bResult = GL_FALSE;
			break;

		case EWeaponHomingRocket:
			if( GetHomingRocketsPowerShoot() == 0 )
				bResult = GL_FALSE;
			break;
	}

	return bResult;
}