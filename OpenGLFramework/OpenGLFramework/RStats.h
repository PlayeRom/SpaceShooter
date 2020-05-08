#pragma once

#include "UniversalBillboard.h"
#include "RMenuBase.h"

class CSDLFont;

#define MAX_REGIONS_STATS 2
#define MAX_WEAPON_STATS EWeaponLastElement + 1

class CRStats : public CRMenuBase
{
public:
	CRStats();
	~CRStats();

	GLvoid ClearStats();
	GLvoid IncreaseShootDownObjects() { ++m_uiShootDownObjects; }
	GLvoid IncreaseMissedObjects() { ++m_uiMissedObjects; }
	GLvoid SetTheBestMultiKill(GLint iMultiKill) { 
		if(iMultiKill > m_iTheBestMultiKill )
			m_iTheBestMultiKill = iMultiKill;
	}
	GLvoid IncreaseUsedWeapon(EWeaponType eWeaponType) { 
		if(eWeaponType == EWeaponRemoteRocket)
			++m_uiUsedWeaponSec[MAX_WEAPON_STATS - 1];
		else
			++m_uiUsedWeaponSec[eWeaponType];
	}

	GLvoid SetMouseRegion();
	GLvoid DrawStats();
	GLvoid RestartObjects();

private:
	GLuint m_uiShootDownObjects;	// ilosc zestrzelonych obiektów
	GLuint m_uiMissedObjects;		// ilosc pominietych, nie zestrzelonych obiektów
	GLint m_iTheBestMultiKill;		// najwiekszy MultiKill
	GLuint m_uiUsedWeaponSec[MAX_WEAPON_STATS]; // tablica uzywania danej broni, ostatni to remote rockets

	RECT m_rRegions[MAX_REGIONS_STATS];
	CSDLFont *m_pSDLFont;
	CSDLFont *m_pSDLFontItems;
	EWeaponType GetWeaponType();
	GLint m_iPosX;
};
