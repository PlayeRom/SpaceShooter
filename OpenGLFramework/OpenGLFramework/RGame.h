#pragma once

#include "EnemyShip.h"
#include "RPlayer.h"
#include "GameControl.h"
#include "UniversalBillboard.h"
#include "RMenuBase.h"

class CRPlayer;
class CEnemyShip;
class CRBonus;
class CRHelp;
class CSDLFont;
class CRVisualEnemyHit;
class CRStarsEffect;
class CRWeaponSelect;
class CRGameMenu;
class CRAddStar;
class CRBoss;
class CRShockWaveEffect;
class CRRemoteRocket;
class CREnemyShotAnimCtrl;
class CRHomingRockets;
class CREmitterRockets;
class CRStats;
class CRFade;

//zmienna przez ktora dzelona jest pozycja eksplozji
//im wartosc jest wieksza tym dzwiek jest blizej sluchacza i jest gloscniejszy
#define DIVISION_EXPLO_POSITION_FOR_SOUND 100.0f //60.0f
#define MAX_INDICATOR_TEXT 8

enum ECameraMode
{
	ECameraModeNormal,
	ECameraModeCabin,		//widok sponad kabiny
	ECameraModeTail,		//blisko za ogonem tak, ze widac caly statek gracza z bliska
	//
	ECameraModeLastElement	//zawsze ostatni
};

class CRGame : public CRMenuBase
{
public:
	CRGame();
	~CRGame();

	GLvoid Initialization();
	GLvoid SetRegions();
	GLvoid RestartObjects();

	GLvoid SetStartPlayerVariables(GLint iIndexShip, GLboolean bMultiplayer = GL_FALSE);
	GLvoid SetEndPlayerVariables(EMainGameMode eSkipTo);

	GLboolean MouseLButtonDown(GLint iX, GLint iY);
	GLboolean MouseLButtonUp(GLint iX, GLint iY);
	GLboolean MouseRButtonDown(GLint iX, GLint iY);
	GLboolean MouseWheel(GLint iScrollLines);
	GLboolean KeyDown(GLuint uiKey);
	GLboolean KeyUp(GLuint uiKey);

	GLvoid Draw();

	GLboolean DrawExplosion(SUniversalBillboard &in_sUB);
#ifndef DRAW_PLAYER_SHIELD_3D
	GLboolean DrawPlayerShield2D(SUniversalBillboard &in_sUB);
#endif
	GLvoid DrawPlayerShots(SUniversalBillboard &in_sUB, GLint iIndex);
	GLvoid DrawEnemyShots(SUniversalBillboard &in_sUB, GLint iIndex);

	GLvoid CalculatePlayerDestroy(SShot &sShot);
	GLvoid CheckCollisionEnemyShot(SShot &sShot, CVector3 &cShotPos);
	GLvoid CalculateEnemyDestroy(SEnemyShip &sEnemyShip, SShot &sShot, CVector3 &cShotPos);

	inline CRPlayer*			GetPlayer()				{ return m_pPlayer; }
	inline CEnemyShip*			GetEnemyShip()			{ return m_pEnemyShip; }
	inline CRBonus*				GetRBonus()				{ return m_pRBonus; }
	inline CUniversalBillboard* GetUnivBillboard()		{ return m_pUnivBillboard; }
	inline CRHelp*				GetRHelp()				{ return m_pRHelp; }
	inline CRVisualEnemyHit*	GetVisualEnemyHit()		{ return m_pVisualEnemyHit; }
	inline CRStarsEffect*		GetStarsEffect()		{ return m_pStarsEffect; }
	inline CRWeaponSelect*		GetRWeaponSelect()		{ return m_pRWeaponSelect; }
	inline CRGameMenu*			GetRGameMenu()			{ return m_pRGameMenu; }
	inline CRAddStar*			GetRAddStar()			{ return m_pRAddStar; }
	inline CRBoss*				GetRBoss()				{ return m_pRBoss; }
	inline CRShockWaveEffect*	GetRShockWaveEffect()	{ return m_pRShockWaveEffect; }
	inline CRRemoteRocket*		GetRRemoteRocket()		{ return m_pRRemoteRocket; }
	inline CREnemyShotAnimCtrl*	GetREnemyShotAnimCtrl() { return m_pREnemyShotAnimCtrl; }
	inline CRHomingRockets*		GetRHomingRockets()		{ return m_pRHomingRockets; }
	inline CREmitterRockets*	GetREmitterRockets()	{ return m_pREmitterRockets; }
	inline CRStats*				GetRStats()				{ return m_pRStats; }
	inline CRFade*				GetRFade()				{ return m_pRFade; }

	GLvoid AddExplosion(const CVector3 &in_cPos, GLfloat fScale, GLboolean bForEnemy = GL_TRUE, GLboolean bMultikill = GL_TRUE);

	inline GLboolean IsPause(){ return m_bPause; }
	inline GLvoid SetPause(GLboolean bPause){ m_bPause = bPause; }

	GLvoid JumpToLevel(GLint iLevel);

	GLboolean CheckCollisionPlayerBonus(const CVector3 &in_cPos, EBonusType eBonusType, GLint iIndex);
	GLvoid CheckEngage();
	GLvoid DrawEngageSquare(const CVector3 &in_cPos, GLfloat fSquareScale = 8.0f);

	inline GLvoid SetDrawPlayerZone(GLboolean bDrawPlayerZone){ m_bDrawPlayerZone = bDrawPlayerZone; }
	inline GLboolean IsDrawPlayerZone()				{ return m_bDrawPlayerZone; }
	GLvoid SetCamera();
	inline ECameraMode GetCameraMode()				{ return m_eCameraMode; }
	inline GLboolean IsShowEnterNickname()			{ return m_bShowEnterNickname; }
	inline GLvoid SetPressKeyEsc(GLboolean bParam)	{ m_bPressKeyEsc = bParam; }
	inline GLboolean GetPressKeyEsc()				{ return m_bPressKeyEsc; }
	inline GLboolean GetPressKeyP()					{ return m_bPressKeyP; }
	inline GLboolean GetPressKeyF1()				{ return m_bPressKeyF1; }

	inline GLboolean GetYouCanPressAnyKey()			{ return m_bYouCanPressAnyKey; }

	inline CVector3& GetHudColor(GLint iIndex){
		if(iIndex < 0 || iIndex >= GetHudColorSize())
			return m_aHudColor[0];
		return m_aHudColor[iIndex];
	}

	inline GLboolean IsEndGame() { return (GetPlayer()->GetLives() == -1 || m_bPlayerWin); }

	GLvoid StartMultiplayerGame();

	GLvoid CalculateShieldIndicator();
	GLvoid CalculateArmourIndicator();

	GLvoid ShowCursor();

private:
	CGameControl		*m_pGameCtrl;
	CRPlayer			*m_pPlayer;
	CEnemyShip			*m_pEnemyShip;
	CRBonus				*m_pRBonus;
	CUniversalBillboard *m_pUnivBillboard;
	CRHelp				*m_pRHelp;
	CSDLFont			*m_pSDLFontPoints;
	CSDLFont			*m_pSDLFontIndic;
	CSDLFont			*m_pSDLFontLevel;
	CSDLFont			*m_pSDLFontDifficultLevel1;
	CSDLFont			*m_pSDLFontDifficultLevel2;
	CSDLFont			*m_pSDLFontUserName;
	CRVisualEnemyHit	*m_pVisualEnemyHit;
	CRStarsEffect		*m_pStarsEffect;
	CRWeaponSelect		*m_pRWeaponSelect;
	CRGameMenu			*m_pRGameMenu;
	CRAddStar			*m_pRAddStar;
	CRBoss				*m_pRBoss;
	CRShockWaveEffect	*m_pRShockWaveEffect;
	CRRemoteRocket		*m_pRRemoteRocket;
	CREnemyShotAnimCtrl	*m_pREnemyShotAnimCtrl;
	CRHomingRockets		*m_pRHomingRockets;
	CREmitterRockets	*m_pREmitterRockets;
	CRStats				*m_pRStats;
	CRFade				*m_pRFade;

	GLboolean m_bDrawPlayerZone;

	//regiony niezmienne
	RECT m_rRegions[2]; //0-shield, 1-armour
	//regiony dla 0-shield i 1-armour, ktorych wartosc top bedzie modyfikowalna,
	//w zaleznosci od procentowej ilosc oslon. To bedzie rysowane z tekstura
	RECT m_rRegionsIndicators[2];
	//regiont na pionowe napisy na paskach
	RECT m_rRegionText[2][MAX_INDICATOR_TEXT];
	GLfloat m_fShieldSignsNumber; //ile liter ma pionowy tekst "oslona"
	GLfloat m_fArmourSignsNumber; //ile liter ma pionowy tekst "pancerz"
	GLvoid CalculateNumberOfIndicatorText();
	//regiony dla zyc
	RECT m_rRegionsLives[3];
	//regiony dla remote bombs
	RECT m_rRegionsRemoteRockets;
	POINT m_ptRegionsRemoteRocketsFont;
	POINT m_rPointAmmo;
	RECT m_rRegionSelectedWeapon;
	//
	LONG m_rRegionsTextForEnterYourName[3];
	POINT m_ptLevelInfo; //region dla informacji o aktualnym poziomie gry

	GLboolean m_bMouseHoldL;
	GLboolean m_bSpaceKeyHold;

	GLvoid DrawTextReady();
	GLvoid DrawEnemy();
	GLvoid MoveEnemy(SEnemyShip &in_sEnemy, GLfloat fSpeed);
	GLvoid CalculateTiltOnWingForEnemy(SEnemyShip &in_sEnemy);
	GLvoid CalculateRotForInertiaObject(SEnemyShip &in_sEnemy);
	GLvoid CalculateForEnemyManoeuvring(SEnemyShip &in_sEnemy);

	GLint m_iLevel;
	GLint m_iLevelToDisplay;
	GLint m_iDrawLevelInfo; //rysuj napisy Level1, Ready itp;

	GLint m_iTextPosY_05;
	GLint m_iTextPosY_10;
	GLint m_iTextPosY_15;
	GLint m_iTextPosY_50;
	GLint m_iTextPosX_22;

	GLint m_iNumberActualDrawEnemy;
	GLvoid CreateEnemyShips();
	GLvoid CreateEnemyShipsFinally();
	GLvoid SetVariablesForCreateEnemyShips();

	//ilosc punktow za dany statek
	//uwaga - w 3DObjManager statki wroga musza byc indeksowane od 0
	std::vector<GLuint> m_aPointsForShootDown;

	GLvoid SelectWeapon(EWeaponType eWeaponType);
	EWeaponType m_eWeaponType;
	GLvoid SetNextWeapon();
	GLvoid SetPreviousWeapon();
	GLvoid AddPlayerShot();
	GLvoid AddPlayerShot_Laser();
	GLvoid AddPlayerShot_Chaingun();
	GLvoid CounterPlayerShot_ImpulseLaser();
	GLvoid AddPlayerShot_ImpulseLaser();

	GLint m_iImpulseLaserCounterTime; //zlicza czas ile czekamy z trzymaniem fire, potem z tego obliczana jest sila
	GLboolean m_bImpulseLaserShot; //wystrzelono z impulse laser
	GLfloat m_fImpulseLaserDrawCounter;
	GLint m_iEmitterImpulseLaser;
	//
	GLvoid AddEnemyShot(SEnemyShip &sEnemyShip);
	GLvoid AddShot(SUniversalBillboard &sBillboarShot, GLfloat fOffset = 0.0f, GLint iModeFly = 0, GLint iExtras = 0);
	GLvoid AddShot(const SEnemyShip &sEnemyShip, SUniversalBillboard &sBillboarShot, GLfloat fOffset = 0.0f, GLint iModeFly = 0, GLint iExtras = 0);

	GLfloat CalculateScaleShot(GLfloat fArmament, GLfloat fMaxBorder = 2.0f);

	GLvoid CheckCollisionPlayerShot(SShot &sShot, CVector3 &cShotPos);
	GLvoid CheckCollisionPlayerEnemy(SEnemyShip &sEnemy, GLint iIndex);

	GLboolean m_bYouCanPressAnyKey; //mozna nacisnac jakikolwiek klawisz pod konic gry aby wpisac pseudonim
	GLboolean m_bShowEnterNickname;
	GLint m_iPositionOnHighscore;
	TCHAR m_cUserName[MAX_HIGHSCORE_NICKNAME_LEN];
	GLvoid SetShowEnterNickname();
	GLvoid ConfirmHighscore();

	GLvoid PlayerLoseLive();
	
	GLboolean m_bPause;

	GLint m_iIndexLevelManager;
	GLvoid CheckEndLevel();
	GLboolean m_bPlayerWin;

	GLvoid AddExplosionSound(const CVector3 &in_cPos);
	GLint m_iActualExplodeSound;

	//migajacy kursor dla Enter your name
	GLboolean m_bDrawCursor;

	//kolor celownika
	std::vector< CVector3 > m_aHudColor;
	inline GLint GetHudColorSize() { return static_cast< GLint >( m_aHudColor.size() ); }

	//multikill
	GLint m_iMultiKillCount;
	GLint m_iMultiKillDisplay;
	GLfloat m_fDrawMultiKill;
	GLvoid CalculateMultiKill();

	GLint m_iActualGroup; //aktualnie rysowana grupa statkow, aby zapewnic odpowiednia kolejnosc wylatywania statkow

	GLboolean m_bDrawHelp;
	GLvoid DoHelp();

	//dla kontoli pauzy
	GLboolean m_bPressKeyP;
	GLboolean m_bPressKeyEsc;
	GLboolean m_bPressKeyF1;

	//dla zmiany tla
	GLint m_iTexBkgIndex;

	//tymczasowe dla zmiany polozenia wysokosci ziemi
#ifdef GROUND_VARIABLES
	GLfloat m_fGroundPosY;
	GLboolean m_bGroundPosYSign;
	GLfloat m_fGroundMove;
#endif
	CVector4 m_cLightPosShadow;
	CVector4 m_cLight1Pos;

	ECameraMode m_eCameraMode;

	//liczniki pociskow grup
	GLuint m_uiCounterPlayerShotGroup;
	GLuint m_uiCounterEnemyShotGroup;

	GLint m_iExplosionDrawCounter; //licznik - ile eksplozji jest rysowanych, dla wlaczenia swiatla
	GLint m_iTotalStartedDrawEnemy; //licznik resetowany co poziom, zlicza aktualna ilosc wypuszczonych do lotu statkow

	GLvoid PlayNextMusic();
	GLvoid StopPlayMusicAndRenewAfterDelay();

	GLboolean CanDoFade();

	////////////////////////////////////////////////////////////////////
	//////////////////////////multiplayer///////////////////////////////

	GLboolean m_bIsMultiplayer;	// czy gra multiplayer
	GLboolean m_bBlockMultiplayerGame;	// blokowanie rozpoczecia gry - wyswietlania napisow "Ready", "Level"
};