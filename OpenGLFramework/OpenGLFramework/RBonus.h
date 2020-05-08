#pragma once

class CGameControl;
#include "RGame.h"

//dla broni Laser
#define LASER_RATE_SHOOT 20
#define LASER_POWER_SHOOT 5

//dla broni Chaingun
#define CHAINGUN_POWER_SHOOT 1
#define CHAINGUN_AMMO 400

//dla broni Impulse Laser
#define IMPULSE_LASER_POWER_SHOOT 1
#define IMPULSE_LASER_AMMO 30

//dla Emitter rockets
#define EMITTER_ROCKETS_RATE_SHOOT 30
#define EMITTER_ROCKETS_POWER_SHOOT 10
#define EMITTER_ROCKETS_AMMO 30

//dla Homing rockets
#define HOMING_ROCKETS_RATE_SHOOT 20
#define HOMING_ROCKETS_POWER_SHOOT 5
#define HOMING_ROCKETS_AMMO 20

#define BASE_VIEW_FINDER 1 //kwadracik celownika o 1
#define BASE_SLOW_ENEMIES_SECOND 5 //bazowe spowolnienie wroga na 5 sekind
#define BASE_SLOW_ENEMIES_DIVISION 2.0f //spowolnienie wroga ma byc dwukrotne
#define BASE_REMOTE_ROCKETS 1 //jedna znajdzka dodaje BASE_REMOTE_ROCKETS zdalnych rakiet

#define SCORE_FOR_TAKE_BONUS 100 //ilosc punktow za schwytanie znajdzki

class CRBonus
{
public:
	CRBonus();
	~CRBonus();

	//motody dodaj¹ce, wywo³ywane gdy gracz schwyta dan¹ znajdzke
	GLvoid UpgradeWeaponLaser();
	GLvoid UpgradeWeaponChaingun();
	GLvoid UpgradeWeaponImpulseLaser();
	GLvoid UpgradeWeaponHomingRockets();
	GLvoid UpgradeWeaponEmitterRockets();

	inline GLvoid AddNumberViewFinder() { 
		m_iNumberViewFinder += BASE_VIEW_FINDER;
		if(m_iNumberViewFinder > 22)
			m_iNumberViewFinder = 22;
	}
	inline GLvoid AddSlowEnemiesSeconds() { m_iSlowEnemiesSeconds += BASE_SLOW_ENEMIES_SECOND; }
	inline GLvoid AddRemoteRocketsNumber() { 
		if(m_iRemoteRocketsNumber < 999)
			m_iRemoteRocketsNumber += BASE_REMOTE_ROCKETS;
	}

	//gettery - pobieranie informacje o aktualnej liczbie
	inline GLuint GetLaserRateShoot() { return m_uiLaserRateShoot; }
	inline GLint GetLaserPowerShoot() { return m_iLaserPowerShoot; }
	inline GLint GetLaserNumberShots() { return m_iLaserNumberShots; }

	inline GLint GetChaingunPowerShoot() { return m_iChaingunPowerShoot; }
	inline GLint GetChaingunNumberShots() { return m_iChaingunNumberShots; }
	inline GLuint GetChaingunAmmo() { return m_uiChaingunAmmo; }

	inline GLint GetImpulseLaserPowerShoot() { return m_iImpulseLaserPowerShoot; }
	inline GLint GetImpulseLaserNumberShots() { return m_iImpulseLaserNumberShots; }
	inline GLuint GetImpulseLaserAmmo() { return m_uiImpulseLaserAmmo; }

	inline GLuint GetEmitterRocketsRateShoot() { return m_uiEmitterRocketsRateShoot; }
	inline GLint GetEmitterRocketsPowerShoot() { return m_iEmitterRocketsPowerShoot; }
	inline GLuint GetEmitterRocketsAmmo() { return m_uiEmitterRocketsAmmo; }
	inline GLint GetEmitterRocketsNumberShots() { return m_iEmitterRocketsNumberShots; }

	inline GLuint GetHomingRocketsRateShoot() { return m_uiHomingRocketsRateShoot; }
	inline GLint GetHomingRocketsPowerShoot() { return m_iHomingRocketsPowerShoot; }
	inline GLuint GetHomingRocketsAmmo() { return m_uiHomingRocketsAmmo; }
	inline GLint GetHomingRocketsNumberShots() { return m_iHomingRocketsNumberShots; }

	inline GLint GetNumberViewFinder() { return m_iNumberViewFinder; }
	inline GLint GetSlowEnemiesSeconds() { return m_iSlowEnemiesSeconds; }
	inline GLint GetRemoteRocketsNumber() { return m_iRemoteRocketsNumber; }

	//ekstra gerrery
	inline GLfloat GetSlowEnemiesDivision() { 
		if(m_iSlowEnemiesSeconds > 0)
			return BASE_SLOW_ENEMIES_DIVISION;
		return 1.0f;
	}
	GLuint GetAmmoFromActualWeapon(EWeaponType eWeaponType);
	GLboolean IsAvailableWeapon(EWeaponType eWeaponType);

	//zmniejszanie wartosci
	inline GLvoid ReduceAmmoChaingun(GLuint uiReduce){
		if(m_uiChaingunAmmo >= uiReduce)
			m_uiChaingunAmmo -= uiReduce;
		else m_uiChaingunAmmo = 0;
	}
	inline GLvoid ReduceAmmoImpulseLaser(GLuint uiReduce){
		if(m_uiImpulseLaserAmmo >= uiReduce)
			m_uiImpulseLaserAmmo -= uiReduce;
		else m_uiImpulseLaserAmmo = 0;
	}
	inline GLvoid ReduceAmmoEmitterRockets(GLuint uiReduce){
		if(m_uiEmitterRocketsAmmo >= uiReduce)
			m_uiEmitterRocketsAmmo -= uiReduce;
		else m_uiEmitterRocketsAmmo = 0;
	}
	inline GLvoid ReduceAmmoHomingRockets(GLuint uiReduce){
		if(m_uiHomingRocketsAmmo >= uiReduce)
			m_uiHomingRocketsAmmo -= uiReduce;
		else m_uiHomingRocketsAmmo = 0;
	}

	GLvoid ReduceWeaponLaser();
	GLvoid ReduceWeaponChaingun();
	GLvoid ReduceWeaponImpulseLaser();
	GLvoid ReduceWeaponEmitterRockets();
	GLvoid ReduceWeaponHomingRockets();

	inline GLvoid ReduceNumberViewFinder() {
		if(m_iNumberViewFinder >= BASE_VIEW_FINDER)
			m_iNumberViewFinder -= BASE_VIEW_FINDER;
	}
	inline GLvoid ReduceSlowEnemiesSeconds() { 
		if(m_iSlowEnemiesSeconds >= BASE_SLOW_ENEMIES_SECOND)
			m_iSlowEnemiesSeconds -= BASE_SLOW_ENEMIES_SECOND;
		else
			m_iSlowEnemiesSeconds = 0;
	}
	inline GLvoid ReduceRemoteRocketsNumber() {
		if(m_iRemoteRocketsNumber > 0)
			--m_iRemoteRocketsNumber;
	}

	inline GLvoid ResetCounterGeneratedBonuses() { m_iCounterGeneratedBonuses = 0; }

	GLvoid ResetAllValue();
	GLvoid CalculateSlowEnemiesSeconds();

	GLvoid GenerateBonus(const CVector3 &in_cPos);
	GLvoid DrawBonus(SUniversalBillboard &in_sUB, GLint iIndex);

private:
	CGameControl	*m_pGameCtrl;

	GLuint	m_uiLaserRateShoot;		//wartosc czestotliwosci strzelania w ms, o jaka zostanie zmniejszona czestotliwosc podstawowa
	GLint	m_iLaserPowerShoot;		//dodatkowa si³a dla ka¿dego pocisku
	GLint	m_iLaserNumberShots;	//ilosc pociskow wystrzeliwanych jednoczesnie

	GLint	m_iChaingunPowerShoot;
	GLuint	m_uiChaingunAmmo; //pojedynczych pociskow
	GLint	m_iChaingunNumberShots;

	GLint	m_iImpulseLaserPowerShoot;
	GLuint	m_uiImpulseLaserAmmo; //ilosc wiazek
	GLint	m_iImpulseLaserNumberShots;

	GLuint	m_uiEmitterRocketsRateShoot;
	GLint	m_iEmitterRocketsPowerShoot;
	GLuint	m_uiEmitterRocketsAmmo; //ilosc rakiet
	GLint	m_iEmitterRocketsNumberShots;

	GLuint	m_uiHomingRocketsRateShoot;
	GLint	m_iHomingRocketsPowerShoot;
	GLuint	m_uiHomingRocketsAmmo; //ilosc rakiet
	GLint	m_iHomingRocketsNumberShots;

	GLint m_iNumberViewFinder;	//dodatkowe kwadraty celownika
	GLint m_iSlowEnemiesSeconds;//ilosc sekund pozostalych spowolnienia wroga
	GLint m_iRemoteRocketsNumber;		//ilosc zebranych bombek jebutek

	GLboolean m_bWasForceWeaponLaserBonus;

	GLint m_iCounterGeneratedBonuses; //licznik ilosci wypuszczonych znajdziek na dany poziom, resetowane co poziom
	GLint m_iCounterDrawBonuses; //ilosc aktualnie rysowanych znajdziek na planszy
};
