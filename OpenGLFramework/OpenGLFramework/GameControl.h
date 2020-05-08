/*! \file
	\brief Plik nag��wkowy dla klasy CGameControl.
*/
#pragma once

#include "SingletonBase.h"

class CWindowData;
class CSpeedControl;
class CTextureLoader;
class CBitmapFont;
class COutlineFont;
class CSDLFont;
class CTextureFont;
class CConsole;
class CFps;

#if _USE_SOUND_OPENAL_
class COpenALManager;
#else
class CSoundFMOD;
#endif

class CMasterVolume;
class CLighting;
class CCursor;
class CARBMultiTexturing;
class CWGLEXTSwapControl;
class CVertexArrays;
class CMatrixOperations;
class C3DObjManager;
class CEmbossBump;
class CParticles;
class CBillboard;
class CStencilShadow;
class CNetworkManager;
class CRMessageBox;
class CLoadProgressBar;
//class CFakeHDR;

class CRLogo;
class CRMainMenu;
class CRDifficultyLevel;
class CRShipsBrowse;
class CRCredits;
class CRGame;
class CShipsManager;
class CLevelsManager;
class CRHighscore;
class CROptions;
class CROptionsGraphics;
class CROptionsSound;
class CROptionsControl;
#if _USE_MULTIPLYER_
class CRMultiplayerMenu;
#endif
class CJoystick;

/* Enumeracja podstawowych tryb�w gry
do ka�dego typu przydzielone jest kolejne enum, itd. w strukture drzewiast� */
enum EMainGameMode
{
	ELogo,				//wyswietlamy logo
	EIntro,				//pokazowka z budowa statku z poligonow
	EMainMenu,			//wyswietlamy menu glowne
	ESingleGame,		//gracz gra w singla
	EOptions,			//wyswietlamy opcje
	EOptionsGraphics,
	EOptionsSound,
	EOptionsControl,
	EHighscore,			//wyswietlamy highscore
#if _USE_MULTIPLYER_
	EMultiplayerMenu,	//pierwsze menu dla multiplayer
#endif
	EDifficultyLevel,
	EShipsBrowser,		//gracz wybiera statek tuz przed gra, lub przegladarka wrogich obiwktow
	ECredits
};

///  G��wna klasa zarz�dzaj�ca gr�. 
/*! Jest to klasa odpowiedzialna za zarz�dzanie tworzon� gr�. Jako, �e jest klas� newralgiczn� dla projektu i zawiera
wa�ne dane programu, CGameControl jest singletonem. Klasa ta tworzy wi�kszo�� obiekt�w opisywanych tu klas oraz
wskazane jest aby obiekty naszych nowo dodanych klas tak�e by�y tutaj tworzone. \n
Poza tym klasa CGameControl otrzymuje wszelkie zdarzenia klawiatury i myszy oraz obs�uguje je, b�d� przekazuje innym
klasom. W metodzie CGameControl::Draw, natomiast, rysowany jest ca�y wirtualny �wiat. Og�lnie m�wi�c jest to klasa,
od kt�rej rozpoczynamy tworzenie naszej gry. */
class CGameControl : public CSingletonBase <CGameControl>
{
public:
	/// Statyczna metoda tworz�ca obiekt klasy CGameControl. 
	/*! CGameControl jest singletonem, wi�c je�eli wywo�amy t� metod� po raz drugi to zwr�ci ona wska�nik na obiekt
	ju� istniej�cy, utworzony wcze�niej.
	\return Wska�nik na obiekt CGameControl. */
	static CGameControl* Construct();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CGameControl();

	/// Metoda inicjalizuje wszelkie inne obiekty utworzone w metodzie CGameControl::CreateObjects.
	/*! Tutaj wywo�ywana jest metoda CGameControl::CreateObjects oraz wczytywane s� zasoby dla innych
	obiekt�w oraz wywo�ywane s� inne funkcje takie jak CGameControl::SetVSync, CGameControl::CreateVertices,
	CGameControl::CreateQuadric. \n
	Wewn�trz tej metody mo�emy wywo�ywa� inne inicjalizacje dla naszych klas. \n
	Metoda ta jest ju� wywo�ywana w CFramework::CreateGameControl.
	\return \p GL_TRUE w przypadku powodzenia inicjalizacji wszelkich obiekt�w. \p GL_FALSE w
	przypadku wyst�pienia jakiego� b��du. */
	GLboolean Initialization();

#ifdef _USE_QUADRIC_
	/// Metoda zwracaj�ca wska�nik kwadryki.
	/*! \return Wska�nik na obiekt kwadryki. Je�eli kwadryka nie zosta�a stworzona, metoda zwr�ci NULL.
	\sa CreateQuadric, m_pQuadric. */
	GLUquadricObj* GetQuadric() { return m_pQuadric; }
#endif //_USE_QUADRIC_

	/// Metoda w��czaj�ca lub wy��czaj�ca synchronicazj� pionow�.
	/*! Czy synchronizacja pionowa zostanie w��czona, zale�y od warto�� zmiennej SETTING_FILE::bVSync.
	Metoda ta jest ju� domy�lnie wywo�ywana w CGameControl::Initialization.
	\sa CWGLEXTSwapControl, SETTING_FILE. */
	GLvoid SetVSync();

	/// Metoda zamykaj�ca aplikacj�.
	/*! Je�eli chemy zamkn�� program, nale�y wywo�a� t� metod�. */
	GLvoid Exit();

	/// Metoda restartuje wszelkie obiekty utworzone w CGameControl::CreateObjects, kt�re restartu wymagaj�.
	/*! Metoda ta jest wywo�ywana gdy restartowane jest okno, np. pod wp�ywem zmiany rozdzielczo�ci, pr�bkowania
	dla antyaliasingu, itp. W metodzie tej nale�y wywo�a� funkcj�, np. tak�e RestartObjects() naszych obiekt�w,
	kt�re mog� zosta� utracone pod wp�ywem zmian parametr�w okna, jak np. rozdzielczo��. W ostateczno�ci mo�emy
	tutaj zniszczy� nasz obiekt i utworzy� ponownie nowy. Pod warunkiem, �e obiekt ten nie b�dzie przechowywa�
	danych pami�taj�cych aktualny stan gry. \n
	Przestrzegaj�c tych regu� uzyskujemy mo�liwo�� restaru okna (np. po zmianie rozdzielczo�ci) bez utraty
	aktualnego stanu gry co b�dzie bardzo dogodne dla u�ytkownika. */
	GLvoid RestartObjects();

	/// Zdarzenie naci�ni�cia klawisza klawiatury, przekazywane z \ref WndProc.
	/*! Metoda ta zostanie wywo�ana automatycznie gdy nadejdzie komunikat \p WM_KEYDOWN. Tutaj klawisz mo�e by�
	przekazany do innego obiektu lub mo�e zosta� ustawiona odpowiednia warto�� tablicy CGameControl::m_bKeys w celu
	przetworzenia klawisza wewn�trz metody CGameControl::Keyboard.
	\param[in] uiKey Wirtualny kod klawisza jaki zosta� naci�ni�ty.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta� obs�u�ony.
	\sa m_bKeys, Keyboard, KeyUp. */
	GLboolean KeyDown( GLuint uiKey );

	/// Zdarzenie zwolnienia klawisza klawiatury, przekazywane z \ref WndProc.
	/*! Metoda ta zostanie wywo�ana automatycznie gdy nadejdzie komunikat \p WM_KEYUP. Tutaj klawisz mo�e by�
	przekazany do innego obiektu lub mo�e zosta� ustawiona odpowiednia warto�� tablicy CGameControl::m_bKeys w
	celu nie przetwarzania klawisza wewn�trz metody CGameControl::Keyboard.
	\param[in] uiKey Wirtualny kod klawisza jaki zosta� zwolniony.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta� obs�u�ony.
	\sa m_bKeys, Keyboard, KeyDown. */
	GLboolean KeyUp( GLuint uiKey );

	/// Metoda przetwarzaj�ca naci�ni�te klawisze klawiatury.
	/*! Wewn�trz tej metody mo�emy zaimplementowa� odpowiedni� reakcj� aplikacji na	naciskane klawisze klawiatury.
	Obs�u�enie klawiszy tutaj a nie w CGameControl::KeyDown	umo�liwia obs�ug� klawiszy, kt�re s� naciskane
	jednocze�nie. \n
	Metoda ta jest ju� wywo�ywana w g��wnej p�tli programu, tj. w CFramework::Run.
	\sa m_bKeys, KeyDown, KeyUp, GetKeyStatus. */
	GLvoid Keyboard();

	/// Metoda wype�niaj�ca bufor znakami przekazywanymi z klawiatury.
	/** Metod� t� nale�y wywo�ywa� np. w KeyDown. Metoda WriteOnKeyboard zapewnia obs�ug� klawisza Backspace do
	kasowania ostatnio wpisanego znaku, oraz wszelkie pozosta�e klawisze, w��cznie z kombinacj� z klawiszem Shift.
	\param[in] uiKey Wirtualny kod klawisza, kt�ry zosta� wci�ni�ty.
	\param[in] lpBuffer Wska�nik na �a�cuch znak�w w kt�rym przechowywany b�dzie tekst wpisywany z klawiatury.
	\param[in] iBufferLenMax Wielko�� \a lpBuffer.
	\return \p GL_TRUE w przypadku przetworzenia klawisza. \p GL_FALSE gdy klawisz nie zosta� obs�u�ony. */
	GLboolean WriteOnKeyboard( GLuint uiKey, LPTSTR lpBuffer, GLint iBufferLenMax );

	/// Metoda za pomoc�, kt�rej mo�emy sprawdzi� czy dany klawisz jest wci�ni�ty.
	/*! \param[in] uiKey Wirtualny kod klawisza, kt�ry chcemy sprawdzi�.
	\return \p GL_TRUE je�eli klawisz jest wci�ni�ty. \p GL_FALSE je�eli klawisz nie jest wci�ni�ty.
	\sa m_bKeys, KeyDown, KeyUp, Keyboard. */
	inline GLboolean GetKeyStatus( GLuint uiKey ) { return m_bKeys[ uiKey ]; }

	/// Metoda konwertuj�ca kod klawisza na odpowiadaj�cy mu znak ASCII.
	/*! Funkcja uwzgl�dnia przytrzymanie klawisza Shift.
	\param[in] uiKeyCode Wirtualny kod klawisza.
	\return Znak ASCII przypisany danemu klawiszowi wirtualnemu. */
	GLuint GetKeyChar( GLuint uiKeyCode );
	
	/// Zdarzenie klikni�cia lewym przyciskiem myszy, przekazywane z \ref WndProc.
	/*! Metoda wywo�ywana automatycznie gdy nadejdzie komunikat \p WM_LBUTTONDOWN. Tutaj mo�emy przetworzy�
	klikni�cie myszy lub przekaza� je dalej do innej klasy.
	\param[in] iX Wsp�rz�dna \a X po�o�enia kursora w momencie klikni�cia.
	\param[in] iY Wsp�rz�dna \a Y po�o�enia kursora w momencie klikni�cia.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta� obs�u�ony. */
	GLboolean MouseLButtonDown( GLint iX, GLint iY );

	/// Zdarzenie zwolnienia lewego przycisku myszy, przekazywane z \ref WndProc.
	/*! Metoda wywo�ywana automatycznie gdy nadejdzie komunikat \p WM_LBUTTONUP. Tutaj mo�emy przetworzy� puszczenie
	myszy lub przekaza� je dalej do innej klasy.
	\param[in] iX Wsp�rz�dna \a X po�o�enia kursora w momencie klikni�cia.
	\param[in] iY Wsp�rz�dna \a Y po�o�enia kursora w momencie klikni�cia.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta� obs�u�ony. */
	GLboolean MouseLButtonUp( GLint iX, GLint iY );

	/// Zdarzenie klikni�cia prawym przyciskiem myszy, przekazywane z \ref WndProc.
	/*! Metoda wywo�ywana automatycznie gdy nadejdzie komunikat \p WM_RBUTTONDOWN. Tutaj mo�emy przetworzy�
	klikni�cie myszy lub przekaza� je dalej do innej klasy.
	\param[in] iX Wsp�rz�dna \a X po�o�enia kursora w momencie klikni�cia.
	\param[in] iY Wsp�rz�dna \a Y po�o�enia kursora w momencie klikni�cia.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta� obs�u�ony. */
	GLboolean MouseRButtonDown( GLint iX, GLint iY );

	/// Zdarzenie zwolnienia prawego przycisku myszy, przekazywane z \ref WndProc.
	/*! Metoda wywo�ywana automatycznie gdy nadejdzie komunikat \p WM_RBUTTONUP. Tutaj mo�emy przetworzy� puszczenie
	myszy lub przekaza� je dalej do innej klasy.
	\param[in] iX Wsp�rz�dna \a X po�o�enia kursora w momencie klikni�cia.
	\param[in] iY Wsp�rz�dna \a Y po�o�enia kursora w momencie klikni�cia.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta� obs�u�ony. */
	GLboolean MouseRButtonUp( GLint iX, GLint iY );

	/// Zdarzenie przekr�cenia k�ka myszy, przekazywane z \ref WndProc.
	/*! Metoda wywo�ywana automatycznie gdy nadejdzie komunikat \p WM_MOUSEWHEEL.
	\param[in] iScrollLines Mowi nam o ile linii i w kt�ra stron� wykonano obr�t, warto�� ujemna - w d�l, warto��
	dodatnia - w g�r�.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean MouseWheel( GLint iScrollLines );

	///  G��wna funkcja rysuj�ca scen�.
	/*! Funkcja ta wywo�ywana jest z metody CFramework::DrawGLScene. Wew�trz tej funkcji nale�y wywo�ywa� kolejne
	metody \a Draw() innych obiekt�w mog�cych co� rysowa�.
	\sa CFramework::DrawGLScene. */
	GLvoid Draw();

	/// Metoda zwracaj�ca aktualn� pozycj� kurosra myszy.
	/**	/return Struktura typu \p POINT zawieraj�ca aktualne wsp�rz�dne kursora myszy.	*/
	POINT GetPointCursor() { return m_ptCursor; }

	TCHAR m_cCharKey; ///< Znak ASCII klawisza jaki zosta� naci�ni�ty. Ustawiane przez komunikat \p WM_CHAR.

	/// Metoda w��czaj�ca tryb grafiki 2D
	/*! Nale�y wywo�a� t� metod�, je�eli chcemy rysowa� w trybie 2D, na powierzchni ekranu.	Domy�lnie tryb 2D jest
	wy��czony - framework dzia�a w trybie 3D.
	\sa Disable2D, makro _USE_2D_ONLY_. */
	GLvoid Enable2D();

	/// Metoda wy��czaj�ca tryb grafiki 2D - powraca do 3D.
	/*! Je�eli w��czyli�my tryb grafiki 2D za pomoc� metody CGameControl::Enable2D. To teraz za pomoc�
	CGameControl::Disable2D mo�emy powr�ci� do trybu 3D.
	\sa Enable2D, makro _USE_2D_ONLY_. */
	GLvoid Disable2D();

	/// Metoda timer-a, wywo�ywana automatycznie gdy nadejdzie zdarzenie \p WM_TIMER, przekazywane z \ref WndProc.
	/*! Domy�lnie timer wywo�ywany jest co 1000 milisekund.
	\sa CWindowData::m_uiTimer. */
	GLvoid Timer();

	inline GLvoid SetEMainGameMode( EMainGameMode eParam ) { m_eMainGameMode = eParam; }
	GLvoid DeleteLogo();
	inline GLvoid SetZeroReIntroShow() { m_iCounterReIntroShow = 0; }

	/// \name Metody get zwracaj�ce wska�niki na r�ne obiekty tworzone w CGameControl.
	//@{
	inline CWindowData*			GetWinData()			{ return m_pWin; }
	inline CSpeedControl*		GetSpeedCtrl()			{ return m_pSpeedControl; }
	inline CTextureLoader*		GetTexLoader()			{ return m_pTextureLoader; }
	inline CBitmapFont*			GetBitmapFont()			{ return m_pBitmapFont; }
	inline COutlineFont*		GetOutlineFont()		{ return m_pOutlineFont; }
	inline CConsole*			GetConsole()			{ return m_pConsole; }
	inline CFps*				GetFps()				{ return m_pFps; }
#if _USE_SOUND_OPENAL_
	inline COpenALManager*		GetOpenALManager()		{ return m_pOpenALManager; }
#else
	inline CSoundFMOD*			GetSoundFMOD()			{ return m_pSoundFMOD; }
#endif
	inline CMasterVolume*		GetMasterVolume()		{ return m_pMasterVolume; }
	inline CSDLFont*			GetSDLFont()			{ return m_pSDLFont; }
	inline CLighting*			GetLighting()			{ return m_pLighting; }
	inline CTextureFont*		GetTextureFont()		{ return m_pTextureFont; }
	inline CCursor*				GetCursor()				{ return m_pCursor; }
	inline CARBMultiTexturing*	GetARBMultiTexturing()	{ return m_pARBMultiTexturing; }
	inline CWGLEXTSwapControl*	GetWGLEXTSwapControl()	{ return m_pWglExtSwapCtrl; }
	inline CVertexArrays*		GetVertexArrays()		{ return m_pVertexArrays; }
	inline CMatrixOperations*	GetMatrixOp()			{ return m_pMatrixOp; }
	inline C3DObjManager*		Get3DObjManager()		{ return m_p3DObjManager; }
	inline CEmbossBump*			GetEmbossBump()			{ return m_pEmbossBump; }
	inline CParticles*			GetParticles()			{ return m_pParticles; }
	inline CBillboard*			GetBillboard()			{ return m_pBillboard; }
	inline CStencilShadow*		GetStencilShadow()		{ return m_pStencilShadow; }
	inline CNetworkManager*		GetNetworkManager()		{ return m_pNetworkManager; }
	inline CRMessageBox*		GetRMessageBox()		{ return m_pRMessageBox; }
	inline CLoadProgressBar*	GetLoadProgressBar()	{ return m_pLoadProgressBar; }

	//inline CFakeHDR*			GetFakeHDR()			{ return m_pFakeHDR; }
	
	inline CRLogo*				GetRLogo()				{ return m_pRLogo; }
	inline CRMainMenu*			GetRMainMenu()			{ return m_pRMainMenu; }
	inline CRDifficultyLevel*	GetRDifficultyLevel()	{ return m_pRDifficultyLevel; }
	inline CRShipsBrowse*		GetRShipsBrowse()		{ return m_pRShipsBrowse; }
	inline CRCredits*			GetRCredits()			{ return m_pCredits; }
	inline CRGame*				GetRGame()				{ return m_pRGame; }
	inline CShipsManager*		GetShipsManager()		{ return m_pShipsManager; }
	inline CLevelsManager*		GetLevelsManager()		{ return m_pLevelsManager; }
	inline CRHighscore*			GetRHighscore()			{ return m_pRHighscore; }
	inline CROptions*			GetROptions()			{ return m_pROptions; }
	inline CROptionsGraphics*	GetROptionsGraphics()	{ return m_pROptionsGraphics; }
	inline CROptionsSound*		GetROptionsSound()		{ return m_pROptionsSound; }
	inline CROptionsControl*	GetROptionsControl()	{ return m_pROptionsControl; }
#if _USE_MULTIPLYER_
	inline CRMultiplayerMenu*	GetRMultiplayerMenu()	{ return m_pRMultiplayerMenu; }
#endif
	inline CJoystick*			GetJoystick()			{ return m_pJoystick; }
	//@}

	/// Tworzy wszelkie obiekty innych klasy, niezb�dnych do funkcjonowania gry.
	/*! Tutaj nale�y tworzy� obiekty w�asnych klas. Metoda ta jest ju� wywo�ywana w CGameControl::Initialization.
	\return \p GL_TRUE w przypadku powodzenia utworzenia wszelkich obiekt�w. \p GL_FALSE w przypadku wyst�pienia
	jakiego� b��du. */
	GLboolean CreateObjects();

	/// Usuwa wszelkie obiekty innych klasy, utworzonych w CGameControl::CreateObjects.
	/*! Tutaj nale�y usuwa� obiekty w�asnych klas, kt�re utworzyli�my w metodzie CGameControl::CreateObjects. */
	GLvoid DeleteObjects();

	/// Metoda ustawia wst�pne parametry o�wietlenia.
	GLvoid SetLights();
	GLvoid SetLight1();
	GLvoid SetLight2();

	/// Metoda tworz�ca wszelkie obiekty dla kontroli animacji.
	/*! Metoda ta jest ju� wywo�ywana w CGameControl::Initialization. Tutaj nale�y wywo�ywa� CSpeedControl::CreateAnimationControl.
	\sa CSpeedControl. */
	GLvoid CreateAllAnimCtrl();

	/// Metoda wczytuj�ca tekstury.
	/*! Metoda ta jest ju� wywo�ywana w CGameControl::Initialization. Tutaj nale�y wczytywa� wszelkie tekstury.
	\return \p GL_TRUE w przypadku powodzenia wczytania tekstur. \p GL_FALSE w przypadku wyst�pienia b��du.
	\sa CTextureLoader. */
	GLboolean LoadAllTextures();

	/// Metoda tworz�ca emitery cz�steczek.
	/*! Metoda ta jest ju� wywo�ywana w CGameControl::Initialization. Tutaj nale�y tworzy� wszelkie emitery
	cz�steczek.
	\return \p GL_TRUE w przypadku powodzenia utworzenia emiter�w. \p GL_FALSE w przypadku wyst�pienia b��du.
	\sa CParticles. */
	GLboolean CreateAllParticles();

	/// Metoda wczytuj�ca wszystkie obiekty 3D.
	/*! Metoda ta jest ju� wywo�ywana w CGameControl::Initialization. W tej metodzie nale�y wczytywa� wszelkie obiekty
	3D - pliki \a *.\a 3DObj.
	\return \p GL_TRUE w przypadku powodzenia wczytania obiekt�w. \p GL_FALSE w przypadku wyst�pienia b��du.
	\sa C3DObjManager. */
	GLboolean LoadAll3DObjFiles();

	/// Metoda wczytuj�ca wszelkie d�wieki i muzyk�.
	/*! Metoda ta jest ju� wywo�ywana w CGameControl::Initialization. Tutaj nale�y wczytywa� wszelkie d�wi�ki
	i muzyk�.
	\return \p GL_TRUE w przypadku powodzenia wczytania d�wi�k�w. \p GL_FALSE w przypadku wyst�pienia b��du. */
	GLboolean LoadAllSounds();

	/// Metoda tworz�ca w�asnor�cznie dodawane wierzcho�ki w celu ich p�niejszego renderowania.
	/*! Wewn�trz tej metody mo�emy dodawa� w�asne wierzcho�ki, poprzez r�czne wpisanie ich koordynat�w jak i innych
	parametr�w, jak werktor normalny, kolor itp.
	\sa CVertexArrays. */
	GLvoid CreateVertices();
	
	GLvoid DrawSkyBox_List( GLint iTexBkgIndex = 0, GLboolean bForMenu = GL_TRUE );
	inline EMainGameMode GetGameMode() { return m_eMainGameMode; }

	//dla indeksow tekstur
	GLint m_iTexBonusViewfinder;
	GLint m_iTexBonusShield;
	GLint m_iTexBonusRemoteRocket;
	//GLint m_iTexBonusWeapon;
	GLint m_iTexBonusEnemySlowdown;
	GLint m_iTexStarFill;
	GLint m_iTexStarEmpty;
	GLint m_iTexParticle01;
	std::vector< GLint > m_aTexBackground;
	inline GLint GetTexBackgroundSize() { return static_cast< GLint >( m_aTexBackground.size() ); }
	std::vector< GLint > m_aTexExplosion;
	inline GLint GetTexExplosionSize() { return static_cast< GLint >( m_aTexExplosion.size() ); }
	GLint m_iTexLogo;
	GLint m_iTexPlayerHitShield;
	GLint m_iTexPlayerHitArmour;
	GLint m_iTexRemoteRocketsCounter;
	GLint m_iTexSmoke00;
	GLint m_iTexShadowTest;
#ifndef DRAW_PLAYER_SHIELD_3D
	GLint m_iTexShield2D;
#endif
	std::vector<GLint> m_aTexWeaponSelect;
	inline GLint GetTexWeaponSelectSize() { return static_cast< GLint >( m_aTexWeaponSelect.size() ); }

	//dla indeksow dzwiekow
	GLint m_iSoundTest;
	//GLint m_iSoundTestOgg;
	GLint m_iSoundMouseOn;
	std::vector<GLint> m_aSoundExplosion;
	inline GLint GetSoundExplosionSize() { return static_cast< GLint >( m_aSoundExplosion.size() ); }
	GLint m_iSoundLaser;
	GLint m_iSoundChaingun;
	GLint m_iSoundImpulseLaser;
	GLint m_iSoundHomingRocket;
	GLint m_iSoundTakeBonus;
	GLint m_iSoundRemoteRocket;
	GLint m_iSoundJoy;
	GLint m_iSoundFanfary;
	GLint m_iSoundShieldHit;
	GLint m_iSoundArmourHit;
	GLint m_iSoundCongratulation;
	GLint m_iSoundGameOver;
	GLint m_iSoundEngage;
	std::vector<GLint> m_aSoundWeaponSelect;
	inline GLint GetSoundWeaponSelectSize() { return static_cast< GLint >( m_aSoundWeaponSelect.size() ); }
#if _USE_OGG_
	std::vector<GLint> m_aMusicMenu;
	inline GLint GetMusicMenuSize() { return static_cast< GLint >( m_aMusicMenu.size() ); }
	std::vector<GLint> m_aMusicGame;
	inline GLint GetMusicGameSize() { return static_cast< GLint >( m_aMusicGame.size() ); }

	GLint m_iActualMusicMenu;
	GLint m_iActualMusicGame;
	GLint m_iRenewMusicAfterDelay; // =-1 default
#endif
	GLvoid StartPlayMusicForGame();
	GLvoid StartPlayMusicForMenu();

	/// Metoda rysuj�ca prostok�d na ca�ym ekranie.
	/** Metoda rysuje prostok�d na ca�ym ekranie.
	\sa SetIndicesQuadAllScreen, m_aIndicesQuadAllScreen.*/
	GLvoid DrawQuadOnAllScreen();

private:
	/// Konstruktor.
	/*! Konstruktor. */
	CGameControl();

	CWindowData		*m_pWin;	///< Wska�nik na obiekt klasy CWindowData.

#ifdef _USE_QUADRIC_
	/// Metoda tworz�ca kwadryk�.
	/*! Tutaj tworzony jest obiekt kwadryki oraz ustawiane s� jej wszelkie domy�lne parametry. \n
	Metoda ta jest ju� domy�lnie wywo�ywana w CGameControl::Initialization.
	\sa GetQuadric, m_pQuadric. */
	GLvoid CreateQuadric();

	GLUquadricObj	*m_pQuadric; ///< Wska�nik na obiekt kwadryki.
#endif //_USE_QUADRIC_
	
	/// Metoda pobieraj�ca i obliczaj�ca pozycj� kursora myszy.
	GLvoid GetCursorPosition();
	
	/// \name Wska�niki na wszelkie obiekty jakie s� tworzone w CGameControl.
	//@{
	CSpeedControl		*m_pSpeedControl;
	CTextureLoader		*m_pTextureLoader;
	CBitmapFont			*m_pBitmapFont;
	COutlineFont		*m_pOutlineFont;
	CConsole			*m_pConsole;
	CFps				*m_pFps;
#if _USE_SOUND_OPENAL_
	COpenALManager		*m_pOpenALManager;
#else
	CSoundFMOD			*m_pSoundFMOD;
#endif
	CMasterVolume		*m_pMasterVolume;
	CSDLFont			*m_pSDLFont;
	CLighting			*m_pLighting;
	CTextureFont		*m_pTextureFont;
	CCursor				*m_pCursor;
	CARBMultiTexturing	*m_pARBMultiTexturing;
	CWGLEXTSwapControl	*m_pWglExtSwapCtrl;
	CVertexArrays		*m_pVertexArrays;
	CMatrixOperations	*m_pMatrixOp;
	C3DObjManager		*m_p3DObjManager;
	CEmbossBump			*m_pEmbossBump;
	CParticles			*m_pParticles;
	CBillboard			*m_pBillboard;
	CStencilShadow		*m_pStencilShadow;
	CNetworkManager		*m_pNetworkManager;
	CRMessageBox		*m_pRMessageBox;
	CLoadProgressBar	*m_pLoadProgressBar;
	//CFakeHDR			*m_pFakeHDR;

	CRLogo				*m_pRLogo;
	CRMainMenu			*m_pRMainMenu;
	CRDifficultyLevel	*m_pRDifficultyLevel;
	CRShipsBrowse		*m_pRShipsBrowse;
	CRCredits			*m_pCredits;
	CRGame				*m_pRGame;
	CShipsManager		*m_pShipsManager;
	CLevelsManager		*m_pLevelsManager;
	CRHighscore			*m_pRHighscore;
	CROptions			*m_pROptions;
	CROptionsGraphics	*m_pROptionsGraphics;
	CROptionsSound		*m_pROptionsSound;
	CROptionsControl	*m_pROptionsControl;
#if _USE_MULTIPLYER_
	CRMultiplayerMenu	*m_pRMultiplayerMenu;
#endif
	CJoystick			*m_pJoystick;
	//@}

	POINT m_ptCursor;			///< Pozycja kursora w pikselach wzgl�dem okna programu.
	GLboolean m_bKeys[ 256 ];	///< Tablica u�ywana do obs�ugi klawiatury.
	GLboolean m_bIs2D;			///< Zmienna zapami�tuj�ca ostatnio u�ywany tryb: 2D lub 3D.

	EMainGameMode m_eMainGameMode;
	
	GLvoid DrawBackground();

	GLfloat m_fRotBackgroundY;
	GLfloat m_fBkgMove;

	//zmienne dla intra
	std::vector<GLushort> m_aArrayLine; //pierwsz tablica indeksow, ktora rozpoczyna - na poczatku jest pusta
	std::vector<GLushort> m_aArrayFill;
	std::vector<GLushort> m_aArrayTexture;
	GLuint m_iFrameCounter;
	GLboolean m_bLineReady;
	GLboolean m_bFillReady;
	GLboolean m_bTextureReady;
	GLvoid DrawIntro();
	GLint m_iIndexShipToBackground;

	GLvoid BreakShowIntro();

	GLfloat m_fSkyBoxRot;
	GLuint m_uiSkyBoxList;
	GLvoid CreateSkyBoxList();
	GLvoid DeleteSkyBoxList();
	GLvoid DrawSkyBox();

	//aby VK_CONTROL nie mogl byc przytrzymywany
	GLboolean m_bVkControl;

	GLuint m_iCounterReIntroShow;

	/// Metoda ustawiaj�ca tablic� wierzcho�k�w dla rysowania prostok�ta na ca�ym ekranie
	/** Metoda ustawia tablic� wierzcho�k�w CGameControl::m_aIndicesQuadAllScreen.
	\sa DrawQuadOnAllScreen, m_aIndicesQuadAllScreen.*/
	GLvoid SetIndicesQuadAllScreen();

	/// Tablica wierzcho�k�w przekazywana do glDrawArrays dla rysowania prostok�ta na ca�ym ekranie.
	GLint* m_aIndicesQuadAllScreen;

	CVector2* m_aSkyBoxTexCoord;
	CVector3* m_aSkyBoxIndices;
};