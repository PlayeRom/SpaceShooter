#pragma once

class CGameControl;
class CVector3;

#include "EnemyShip.h"

struct SEmitterPlayerDamage
{
	CVector3 cPos;
	GLint iEmitterIndex;
};

#define MAX_EMITTER_PLAYER_DAMAGE 3

class CRPlayer
{
public:
	CRPlayer();
	~CRPlayer();

	GLvoid DrawPlayer();
	GLvoid DrawPlayerShield3D();

	GLboolean MouseLButtonDown( GLint iX, GLint iY );
	GLboolean MouseRButtonDown( GLint iX, GLint iY );

	GLvoid SetCursorPosAfterPause();
	GLvoid RestartObjects();

	GLvoid DoManoeuvring();
	GLvoid RealizeManoeuvring( GLint bMode = 0 );
	GLvoid CalculateManoeuvring();

	inline GLvoid SetImmortality( GLint iImmortal = 30 )	{ m_iImmortality = iImmortal; }
	inline GLboolean IsImmortality()						{ return m_iImmortality > 0 ? GL_TRUE : GL_FALSE; }
	inline GLint GetImmortality()							{ return m_iImmortality; }

	GLvoid SetPlayerHit();

	inline GLvoid Set3DObjectIndex( GLint i3DObjIndex )		{ m_i3DObjectIndex = i3DObjIndex; }
	inline GLint Get3DObjectIndex()							{ return m_i3DObjectIndex; }

	inline GLvoid SetPosition( CVector3 cPos )				{ m_cPosition = cPos; }
	inline CVector3 GetPosition()							{ return m_cPosition; }

	inline GLvoid SetStarsShield( GLint iStars )			{ m_iStarsShield = iStars; }
	inline GLint GetStarsShield()							{ return m_iStarsShield; }

	inline GLvoid SetStarsArmour( GLint iStars )			{ m_iStarsArmour = iStars; }
	inline GLint GetStarsArmour()							{ return m_iStarsArmour; }

	inline GLvoid SetStarsArmament( GLint iStars )			{ m_iStarsArmament = iStars; }
	inline GLint GetStarsArmament()							{ return m_iStarsArmament; }

	inline GLvoid SetStarsManoeuvring( GLint iStars )		{ m_iStarsManoeuvring = iStars; }
	inline GLint GetStarsManoeuvring()						{ return m_iStarsManoeuvring; }

	inline GLvoid SetShieldMax( GLint iShieldMax )			{ m_iShieldMax = iShieldMax; }
	inline GLint GetShieldMax()								{ return m_iShieldMax; }
	inline GLvoid SetShieldActual( GLint iShieldActual )	{ m_iShieldActual = iShieldActual; }
	inline GLvoid IncreaseShieldActual( GLint iIncrease )	{ m_iShieldActual += iIncrease; }
	inline GLvoid DecreaseShieldActual( GLint iDecrease )	{ m_iShieldActual -= iDecrease; }
	inline GLint GetShieldActual()							{ return m_iShieldActual; }

	inline GLvoid SetArmourMax( GLint iArmourMax )			{ m_iArmourMax = iArmourMax; }
	inline GLint GetArmourMax()								{ return m_iArmourMax; }
	inline GLvoid SetArmourActual( GLint iArmourActual )	{ m_iArmourActual = iArmourActual; }
	inline GLvoid IncreaseArmourActual( GLint iIncrease )	{ m_iArmourActual += iIncrease; }
	inline GLvoid DecreaseArmourActual( GLint iDecrease )	{ m_iArmourActual -= iDecrease; }
	inline GLint GetArmourActual()							{ return m_iArmourActual; }

	inline GLvoid SetArmamentBase( GLint iArmamentBase )	{ m_iArmamentBase = iArmamentBase; }
	inline GLint GetArmamentBase()							{ return m_iArmamentBase; }
	inline GLvoid SetArmamentActual( GLint iArmamentActual ){ m_iArmamentActual = iArmamentActual; }
	inline GLvoid IncreaseArmamentActual( GLint iIncrease )	{ m_iArmamentActual += iIncrease; }
	inline GLint GetArmamentActual()						{ return m_iArmamentActual; }

	inline GLvoid SetManoeuvring( GLint iManoeuvring )		{ m_iManoeuvring = iManoeuvring; }
	inline GLint GetManoeuvring()							{ return m_iManoeuvring; }

	inline GLvoid SetCollisionA( GLfloat fCollisionA )		{ m_fCollisionA = fCollisionA; }
	inline GLfloat GetCollisionA()							{ return m_fCollisionA; }

	inline GLvoid SetCollisionB( GLfloat fCollisionB )		{ m_fCollisionB = fCollisionB; }
	inline GLfloat GetCollisionB()							{ return m_fCollisionB; }

	inline GLvoid SetLives( GLint iLives )			{ m_iLives = iLives; }
	inline GLvoid LivesPlus1()						{ ++m_iLives; }
	inline GLvoid LivesMinus1()						{ --m_iLives; }
	inline GLint GetLives()							{ return m_iLives; }

	inline GLvoid SetScore( GLuint iScore )			{ m_uiScore = iScore; }
	inline GLvoid IncreaseScore( GLuint iScore )	{ m_uiScore += iScore; }
	inline GLuint GetScore()						{ return m_uiScore; }

	inline GLvoid SetIsManoeuvring( GLboolean bIsManoeuvring )	{ m_bIsManoeuvring = bIsManoeuvring; }
	inline GLboolean IsManoeuvring()							{ return m_bIsManoeuvring; }

	inline GLvoid SetPlayerExplode( GLuint iPlayerExplode )		{ m_iPlayerExplode = iPlayerExplode; }
	inline GLint GetPlayerExplode()								{ return m_iPlayerExplode; }

	inline GLvoid SetNumberViewFinder( GLint iNumberViewFinder ){ m_iNumberViewFinder = iNumberViewFinder; }
	inline GLint GetNumberViewFinder()							{ return m_iNumberViewFinder; }

	inline GLfloat GetEngageDistance()							{ return m_fEngageDistance; }
#ifdef DRAW_PLAYER_SHIELD_3D
	inline GLvoid SetDrawPlayerShield3D( GLboolean bDrawShield ) {
		m_bDrawShield3D = bDrawShield;
		m_fShieldRotate = 0.0f;
	}
	inline GLboolean GetDrawPlayerShield3D() { return m_bDrawShield3D; }
	inline GLvoid ResetDrawPlayerShield3D() { 
		m_bDrawShield3D = GL_FALSE;
		m_fShieldRotate = 0.0f;
	}
#endif

	inline GLfloat GetManoeuvringRot()			{ return m_fManoeuvringRot; }
	inline GLfloat GetRot30()					{ return m_fRot30; }

	GLvoid PlayerLoseLive();

	//efekt wstrzasu po zderzeniu gracza z innym obiektem
	GLvoid DoShakePlayer( GLboolean bSlantFly, GLboolean bSlantSign );
	GLvoid RealizeShakePlayer();
	inline GLvoid	SetShakePlayer( GLboolean bShakePlayer )	{ m_bShakePlayer = bShakePlayer; }
	inline GLint	GetShakePlayer()							{ return m_bShakePlayer; }
	inline GLvoid	SetShakeFade(GLfloat fFade)					{ m_fShakeFade = fFade; }

	CRectangle GetCollisionRect();

	GLboolean IsCompleteStars();

private:
	CGameControl	*m_pGameCtrl;

	GLint m_iCenterMouseX;
	GLint m_iCenterMouseY;
	POINT m_ptMouse;

	GLint m_iImmortality; //6 to uaktywnienie, 0 - gracz smiertely, 6 dlatego, ze niesmiertelnosc ma trwac 3 sekundy, a odmiezanie czasu ustawione jest co pol sekundy

	GLvoid MouseMove();
	GLvoid DrawPlayerHit();
	GLvoid SetLight1();
	GLvoid DrawPlayerShip();
	GLvoid DrawViewfinder();
	GLvoid DrawZone( GLfloat fRightX, GLfloat fLeftX, GLfloat fMaxZ, GLfloat fMinZ );

	GLboolean m_bPlayerHit; //true to gracz tafiony
	GLfloat m_fPlayerHitPlane; //dla zobrazowania znikania planszy po oberwaniu gracza

	/////////////////
	GLint m_i3DObjectIndex;
	CVector3 m_cPosition;

	//ilosc gwiazdek
	GLint m_iStarsShield;
	GLint m_iStarsArmour;
	GLint m_iStarsArmament;
	GLint m_iStarsManoeuvring;

	//punkty:
	GLint m_iShieldMax; //na poczatku gry bazwe - wyliczone z gwiazdek
	GLint m_iShieldActual;
	GLint m_iArmourMax; //na poczatku gry bazwe - wyliczone z gwiazdek
	GLint m_iArmourActual;
	GLint m_iArmamentBase; //bazwe uzbrojenie - wyliczone z gwiazdek
	GLint m_iArmamentActual; //aktualne uzbrojenie - gwiazdki + upgrade
	GLint m_iManoeuvring; //zwrotnosc w % - wyliczona z gwiazdek

	GLfloat m_fCollisionA;
	GLfloat m_fCollisionB;
	
	GLint m_iLives;//ilosc zyc
	GLuint m_uiScore; //ilosc punktow

	//dla animacji
	GLboolean m_bIsManoeuvring; //jezeli true to zrob beczke
	GLboolean m_bManoeuvringSign; //0 - obrot ujemny, 1 - obot dodatni
	GLfloat m_fManoeuvringRot;
	GLfloat m_fRot30; //przechyl na skrzydlo przy ruchach na boki
	GLint m_iPlayerExplode; //jezeli GT_TRUE to rysowany jest wybuch gracza

	GLint m_iNumberViewFinder; //ilosc kwadratow celowniczych

	GLint m_iEmitterIndex[ MAX_EMITTER_PLAYER_DAMAGE ];

#ifdef DRAW_PLAYER_SHIELD_3D
	GLfloat m_fShieldRotate;
	GLboolean m_bDrawShield3D;
#endif
	GLfloat m_fEngageDistance;
	CVector4 m_cLight1Pos;
	CVector4 m_cLightPosShadow;

	//efekt wstrzasu po zderzeniu gracza z innym obiektem
	CVector3 m_cShakeVector;
	GLboolean m_bShakePlayer;
	GLfloat m_fShakeFade;
};