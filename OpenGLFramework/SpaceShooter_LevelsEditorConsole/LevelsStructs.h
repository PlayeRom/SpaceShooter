
//dla LevelManager
#define FILE_NAME_LEVEL_EDIT _T("levels.dat")

//dla edytora
#define FILE_NAME_LEVEL_EDIT2 _T("../OpenGLFramework/levels.dat")

struct SShipsGroups
{
	int iIndexShip; //index statku
	int iNumberShips; //ilosc statkow w grupie
	int iFormation; //szyk - formacja
	int iPosXForRand; //pozycja startowa po osi X
	float fSpeed;
	unsigned char bCosFly; //lot po cosinusoidzie
	unsigned char bSlantFly; //lot po skosie
	float fOffsetXPosForSlantFly; //przesuniecie pozycji startowej dla lotu po skosie
};

struct SLevel
{
	int iLevel; //numer poziomu
	unsigned char bWithBoss; //poziom z bossem
	int iMaxShipsOnScreen; //max statkow na planszy w czasie gry
	int iFrequencyShoot; //czestotliwosc strzelania
	int iPercentChanceToShoot; //szansa na oddanie strzalu
	int iNumberGroups; //ilosc grup statkow
	SShipsGroups *psShipsGroups; //wskaznik na pablice grup statkow
};