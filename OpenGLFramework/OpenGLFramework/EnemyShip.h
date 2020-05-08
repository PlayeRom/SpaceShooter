#pragma once

class CRectangle;

struct SEnemyShip
{
	GLint i3DObjIndex;
	GLboolean bBoss;	//czy statek jest bossem
	GLint uiPoints; //ilosc punktow za zestrzelenie
	GLboolean bStart; //true - statek wystartowal, false - nie wystartowal, gracz nie moze go zestrzelic
	GLboolean bAlive; //rysu statek, przy inicjalizacji zawsze true
	
	GLfloat fCollisionA; //odleglosc od srodka do krawedzi przedniej jak i tylnej
	GLfloat fCollisionB; //odleglosc od srodka do krawedzi prawej jak i lewej

	CVector3 cActualPos; //aktualna pozycja
	CVector3 cOldPos; //stara pozycja do obliczenia przechylu na skrzydlo
	GLfloat fSpeed;

	GLboolean bCanManoeuvring;
	GLboolean bCanFire;
	GLboolean bWreck;			//czy obiekt jest wrakiem

	//dla beczki
	GLboolean bIsManoeuvring; //jezeli true to zrob beczke
	GLboolean bManoeuvringSign; //0 - obrot ujemny, 1 - obot dodatni
	GLfloat fManoeuvringRot; //aktualny kat obrotu

	//punkty:
	GLint iShield, iMaxShield;
	GLint iArmour, iMaxArmour;
	GLint iArmament; //uzbrojenie - wyliczone z gwiazdek
	GLint iManoeuvring; //zwrotnosc w % - wyliczona z gwiazdek

	GLint iAnimCtrlIndex;

	//jezeli asteroida to katy pod ktorymi bedzie sie obracac
	CVector3 fRotAsteroid;

	//dla lotu zygzakiem
	GLboolean bCosFly; //czy bedzie lecial po coisinusoidze
	GLboolean bCosSign; //znak 0 = -, 1 = +
	GLfloat fCosActualAngle;
	GLfloat fCosIncreaseValue; //domyslnie 0.05f
	GLfloat fTiltAngle; //kat przechylu na skrzydlo

	//lot po skosie
	GLboolean bSlantFly; //czy leci po skosie
	GLboolean bSlantSign; //0 = -, 1 = + - oznaczenie w ktora strone skosny lot, gdy 1(+) to leci z lewej strony na prawa

	GLint iFormation; //1 - sam, 2 - leci w dwojce, 3 - leci w trojce
	GLint iGroup; //index grupy do jakiej nalezy statek, min 0, max 2 (3-1)
};

class CEnemyShip
{
public:
	CEnemyShip();
	~CEnemyShip();

	GLvoid Add( SEnemyShip &sEnemyShip );
	GLvoid Add(	GLint i3DObjIndex,
				GLint uiPoints,
				CVector3 cActualPos,
				GLfloat fSpeed,
				GLfloat fCollisionA,
				GLfloat fCollisionB,
				GLint iShield,
				GLint iArmour,
				GLint iArmament,
				GLint iManoeuvring,
				GLboolean bCosFly,
				GLboolean bCosSign,
				GLfloat fCosIncreaseValue,
				GLboolean bSlantFly,
				GLint iFormation,
				GLint iGroup,
				GLboolean bCanManoeuvring,
				GLboolean bCanFire );
	
	GLvoid AddBoss( GLint i3DObjIndex,
					GLint uiPoints,
					CVector3 cActualPos,
					GLfloat fSpeed,
					GLfloat fCollisionA,
					GLfloat fCollisionB,
					GLint iShield,
					GLint iArmour,
					GLint iArmament,
					GLint iManoeuvring,
					GLint iGroup );

	inline GLvoid ClearAll()						{ m_aEnemyShip.clear(); }
	inline GLint GetEnemyShipSize()					{ return static_cast< GLint >( m_aEnemyShip.size() ); }
	inline SEnemyShip& GetEnemyShip( GLint iIndex ) { return m_aEnemyShip[ iIndex ]; }
	inline SEnemyShip* GetEnemyShipArray()			{ return &m_aEnemyShip[ 0 ]; }

	GLboolean IsAliveEnemy();
	GLint CheckActualGroup();

	CRectangle GetCollisionRect( GLint iIndex );

private:
	std::vector<SEnemyShip> m_aEnemyShip;
};