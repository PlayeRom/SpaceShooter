#pragma once

class CGameControl;

struct SExplosion
{
	GLint iActualAnimFrame; //0 to pierwsza, -1 to nie rysuj i usun
	GLint iTexture; //index tekstury z wybuchem
	GLfloat fScale;
	GLint iAnimCtrlIndex;
};

#ifndef DRAW_PLAYER_SHIELD_3D
struct SPlayerShield
{
	GLint iActualAnimFrame; //0 to pierwsza, -1 to nie rysuj
	GLint iTexture; //index tekstury z animacja oslony 2d
	GLfloat fScale;
	GLint iAnimCtrlIndex;
};
#endif

enum EBonusType
{
	EBonusWeaponLaser,
	EBonusWeaponChaingun,
	EBonusWeaponImpulseLaser,
	EBonusWeaponEmitterRockets,
	EBonusWeaponHomingRockets,
	EBonusShield,				//oslona
	EBonusViewFinder,			//celownik
	EBonusIndestructibility,	//niezniszczalnosc
	EBonusSlowdownEnemy,		//spowolnienie wroga
	EBonusRemoteRocket,			//zdalna rakieta
	EBonusLife,					//zycie, musi byc przed EBonusLastElement!
	//
	EBonusLastElement			//zawsze ostatni
};

enum EWeaponType
{
	EWeaponLaser,			//pociskowo laserowa
	EWeaponChaingun,		//szybka bron pociskowa - karabin maszynowy
	EWeaponImpulseLaser,	//wiazka lasera przez caly ekran
	EWeaponEmitterRocket,
	EWeaponHomingRocket,	//rakieta samonaprowadzajaca
	//
	EWeaponLastElement,
	//
	EWeaponRemoteRocket // RemoteRocket ostatni aby nie liczyl sie przy przelaczaniu broni
};

struct SShot
{
	GLboolean bDraw;
	//CVector3 cActualPos; //<- juz jest w SUniversalBillboard
	GLfloat fSpeed; //= 0.0 to pocisk jest zdalna rakieta
	GLint iTexture;
	GLint iArmament; //sila pocisku
	GLint iScaleImpulseLaser; //tylko dla broni impulse laser
	//////////// NEW ///////
	//Numer grupy pocisku jezli pociski sa grupowane - tak jak laser pociskowy
	//Jezeli pocisk samotny to uiGroupIndex = 0, liczniki grup to zmienne:
	//m_uiCounterPlayerShotGroup //dla pociskow gracza
	//m_uiCounterEnemyShotGroup //dla wrogich pociskow
	GLuint uiGroupIndex;
	////////////////////////
	GLint i3DObjIndex; //tylko dla pociskow gracza, indeks statku ktory myknal beczke i nie nalezy dla niego sprawdzac juz kolizji

	//< 0 - po skosie na lewo, czyli x--,
	//0 - na wprost,
	//> 0 - po skosie na prawo, czyli x++
	//kolejna zasada: im np. bardziej ujemna liczba tym wiekszy rozrzut
	GLint iModeFly;
	EWeaponType eWeaponType; //rodzaj broni z ktorej pocisk wystrzelono
};

enum EUniversalBillboardType
{
	EUBTypeNone,
	EUBTypeExplosion,
	EUBTypeSmokeRemoteRocket,
	EUBTypeSmokePlayerDamage,
	EUBTypeBonus,
	EUBTypePlayerShield,
	EUBTypePlayerShot,
	EUBTypeEnemyShot
};

//Ponizsza struktura, jest uniwersalna struktora roznych danych, potrzebnych do
//sortowania wedle glebi, aby elementy przezroczyste dobrze sie rysowaly
struct SUniversalBillboard
{
	CVector3 cPos;
	EUniversalBillboardType eType; //typ danych, 1 - SExplosion, 2 - SSmokeRemoteRocket, 3 - bonus, 4 - SPlayerShield, 5 - SShot
	union {
		SExplosion sExpl;
		GLboolean bIsDrawSmokeRR; //jezeli eType = EUBTypeSmokeRemoteRocket
		GLboolean bIsDrawSmokePD; //jezeli eType = EUBTypeSmokePlayerDamage
		EBonusType eBonusType; //jezeli eType = EUBTypeBonus
#ifndef DRAW_PLAYER_SHIELD_3D
		SPlayerShield sPlayerShield;
#endif
		SShot sShot; //EUBTypePlayerShot lub EUBTypeEnemyShot
	};
};

class CUniversalBillboard
{
public:
	CUniversalBillboard();
	~CUniversalBillboard();

	GLvoid SetForStart();

	inline GLvoid AddUniversalBillboard(const SUniversalBillboard &in_sUB){
		m_aUniversalBillboard.push_back( in_sUB );
	}
	inline GLvoid DeleteUniversalBillboard(GLint iIndex){
		m_aUniversalBillboard.erase( m_aUniversalBillboard.begin() + iIndex );
	}
	inline GLint GetUniversalBillboardSize(){
		return static_cast< GLint >( m_aUniversalBillboard.size() );
	}

	GLvoid SortUniversalBillboard();
	GLvoid DrawUniversalBillboard();
	GLboolean IsBonusesOrEnemyShots();
	GLint AddExplosion(const CVector3 &in_cPos, GLfloat fScale);
	inline GLint GetAnimCtrlIndexFromExplosion(GLint iIndex){
		return m_aUniversalBillboard[iIndex].sExpl.iAnimCtrlIndex;
	}

#ifndef DRAW_PLAYER_SHIELD_3D
	GLvoid CUniversalBillboard::SetDrawPlayerShield2D();
#endif
	GLvoid AddSmokeRemoteRocket(const CVector3 &in_cPos);
	GLvoid AddSmokePlayerDamage(const CVector3 &in_cPos);

	GLvoid SetNoDrawForShotsGroup(EUniversalBillboardType eType, GLuint uiGroupIndex);
	GLvoid SetManoeuvringForShotsGroup(EUniversalBillboardType eType, GLuint uiGroupIndex, GLint i3DObjIndex);
	GLvoid SetArmamentForShotsGroup(EUniversalBillboardType eType, GLuint uiGroupIndex, GLint iArmament);

private:
	CGameControl *m_pGameCtrl;
	std::vector<SUniversalBillboard> m_aUniversalBillboard;

	GLvoid DrawSmokeRemoteRocket(SUniversalBillboard &in_sUB);
	GLvoid DrawSmokePlayerDamage(SUniversalBillboard &in_sUB);
};
