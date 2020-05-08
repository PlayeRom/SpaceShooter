
//dla CShipsManager
#define FILE_NAME_SHIPS_EDIT _T("ships.dat")
#define FILE_NAME_SHIPS_WORK _T("ships_work.dat")

//dla edytora operujemy bezposrednio na pliku w glownym katalgu projektu
#define FILE_NAME_SHIPS_EDIT2 _T("../OpenGLFramework/ships.dat")

#define SIZE_STRING 128

struct SEmitterData
{
	int iIndex; //do rysowania, z CParticles::AddEmitter
	float fEmitterPosX;
	float fEmitterPosY;
	float fEmitterPosZ;
	int iTexEmitter; //tektura czasteczki
	float fR, fG, fB; //kolor
	int iParticlesNumber; //ilosc czasteczek
};

//do przepisywania na nowe wersje struktur
struct SOldUniqueShip
{
	//dane wynikle z wczytania obiektu
	int i3DObjIndex; //indeks do rysowania z 3DObjManagera
	int iTexIndex; //indeks tekstury
	int iTexBumpIndex; //indeks tekstury bump
	int iTexLife;
	
	//informacje o statku
	TCHAR cTexName[SIZE_STRING];
	TCHAR cTexBumpName[SIZE_STRING];
	TCHAR cTexLife[SIZE_STRING]; //textura zyc
	TCHAR cTexLifeMask[SIZE_STRING]; //textura zyc maska
	TCHAR cObjectName[SIZE_STRING];
	unsigned char bPlayer; //1 - player, 0 - enemy
	TCHAR cShipName[SIZE_STRING]; //nazwa statku
	unsigned char bCanFire; //czy moze strzelac, bo np asteroida nie moze
	unsigned char bCanManoeuvring; //czy moze unikac pociskow bo, np. asteroida nie moze
	//ilosc gwiazdek
	//jezeli ilosc wszystkich gwiazdek jest = 0 to mamy do czynienia z innym obiektem, np. ze "znajdükπ"
	int iStarsShield;
	int iStarsArmour;
	int iStarsArmament;
	int iStarsManoeuvring;

	float fCollisionA;
	float fCollisionB;

	//dla czasteczek
	int iEmitterNumber; //ilosc emiterow, np.2
	SEmitterData *psEmitterData;
};

struct SUniqueShip
{
	//dane wynikle z wczytania obiektu
	int i3DObjIndex; //indeks do rysowania z 3DObjManagera
	int iTexIndex; //indeks tekstury
	int iTexBumpIndex; //indeks tekstury bump
	int iTexLife;

	//typ tekstury:
	//	= 0 - normalnie (mipmapa z mozliwoscia bumpa)
	//	= 1 - LoadMultiTextureMaskMipmaps( cTexName, RGB() );
	unsigned char byTexType;
	
	//informacje o statku
	TCHAR cTexName[SIZE_STRING]; //tekstura wlasciwa
	TCHAR cTexBumpName[SIZE_STRING]; //tekstrua bump - opcjonalnie
	unsigned int uiCOLORREF; //kolor maski, tylko wtedy gdy byTexType = 1
	TCHAR cTexLife[SIZE_STRING]; //textura zyc
	TCHAR cTexLifeMask[SIZE_STRING]; //textura zyc maska
	TCHAR cObjectName[SIZE_STRING];
	unsigned char bPlayer; //1 - player, 0 - enemy
	TCHAR cShipName[SIZE_STRING]; //nazwa statku
	unsigned char bCanFire; //czy moze strzelac, bo np asteroida nie moze
	unsigned char bCanManoeuvring; //czy moze unikac pociskow bo, np. asteroida nie moze
	//ilosc gwiazdek
	//jezeli ilosc wszystkich gwiazdek jest = 0 to mamy do czynienia z innym obiektem, np. ze "znajdükπ"
	int iStarsShield;
	int iStarsArmour;
	int iStarsArmament;
	int iStarsManoeuvring;

	float fCollisionA;
	float fCollisionB;

	//dla czasteczek
	int iEmitterNumber; //ilosc emiterow, np.2
	SEmitterData *psEmitterData;
};

struct SUniqueShipWork
{
	SUniqueShip sUniqueShips;

	//dodatkowe pola dla gry, nie wystepujace w edytorze
	unsigned char bAvailableInBrowse; //statek jest dostepny w przegladarce
};