/*! \file
	\brief Modu³ makr.
*/

#ifndef _DEFINE_H_
#define _DEFINE_H_

/// Nazwa frameworka
#define FRAME_NAME						_T(" PlayeRom OpenGLFramework v.1.05 ")

/// Nazwa gry/programu.
#define GAME_NAME						_T("Space Shooter")
/// Adres strony.
#define HOMEPAGE						_T("www.playerom.prv.pl")

/// \name Wersja gry - ka¿dy cz³on mo¿e byæ w zakresie od 0 do 255.
//@{
#define MAJOR_VERSION					0
#define MINOR_VERSION					5
#define RELEASE_VERSION					2
//@}

/// \name Wersja pliku konfiguracyjnego - ka¿dy cz³on mo¿e byæ w zakresie od 0 do 255.
//@{
#define MAJOR_VERSION_FILE				1
#define MINOR_VERSION_FILE				0
#define RELEASE_VERSION_FILE			2
//@}

/// Nazwa pliku konfiguracyjnego.
#define SETTING_FILE_NAME				"setting.cfg"

/// Maksymalna d³ugoœæ tekstu w jednej linii dla konsoli.
#define CONSOLE_MSG_LENGTH				128

/// Sta³a PI.
#define PI								3.141592f//6535897932384626433832795f
/// Makro przekszta³cenia stopni na radiany.
#define DEG2RAD( x )					( ( x * PI ) / 180.0f )

// UWAGA! U¿ycie, b¹dŸ nie UNICODE ustawiamy we w³aœciwoœciach projektu:
// Alt+F7 -> Configuration Properties -> General -> Character set

//################################################################
/// Jakiej biblioteki dŸwiêkowej u¿yæ?
/*! Je¿eli makro to zostanie ustawione na 1 to do odgrywania dŸwiêków i muzyki u¿yta zostanie biblioteka OpenAL.
Je¿eli ustawimy to makro na 0 to u¿yta zostanie biblioteka FMOD. Domyœlnie makro _USE_SOUND_OPENAL_ jest zdefiniowane. */
#define _USE_SOUND_OPENAL_				1

/// Czy w OpenAL u¿ywaæ formatu pliku muzycznego OGG?
/** Ustawiaj¹c to makro na 1 udostêpnimy mo¿liwoœæ odgrywania plików OGG poprzez bibliotekê OpenAL. Oznacza to,
¿e makro \ref _USE_SOUND_OPENAL_ tak¿e musi byæ ustawione na 1. */
#define _USE_OGG_						1
//################################################################

/// Zapytanie o tryb pe³noekranowy.
/*! Zdefiniowanie tego makra spowoduje, i¿ w trakcie uruchamiania naszej aplikacji, zostaniemy zapytani czy program
ma siê uruchomiæ w trybie pe³noekranowym czy te¿ nie. Je¿eli makro jest niezdefiniowane (i tak jest domyœlnie) – to
program zawsze uruchomi siê na pe³nym ekranie. */
#define _QUESTION_FULLSCREEN_			0

/// Czy pobraæ systemow¹ rozdzielczoœæ i kolory?
/*! Je¿eli makro to jest zdefiniowane to w trakcie inicjalizacji okna, rozdzielczoœæ i glêbia kolorów zostan¹ pobrane
z ustawien systemowych pulpitu. Je¿eli chcemy aby u¿yte by³y wartoœci zdefiniowane przez nas (patrz konstruktor klasy
CWindowData), makro nale¿y dezaktywowaæ. */
#define _USE_SYSTEM_RES_AND_BITSPERPIXEL_	1

#if !_USE_SYSTEM_RES_AND_BITSPERPIXEL_
	// Tutaj ustaw w³asn¹, sta³¹ rozdzielczoœæ ekranu oraz g³êbiê kolorów
	#define _SCREEN_WIDTH_				800
	#define _SCREEN_HEIGHT_				600
	#define _SCREEN_BITS_				32
#endif

/// Czy u¿yæ trybu tylko 2D?
/*! Zdefiniuj poni¿sze makro je¿eli twoja gra u¿ywaæ ma jedynie trybu 2D.
Je¿eli gra ma wykorzystywaæ grafikê 3D, makro nale¿y dezaktywowaæ. */
#define _USE_2D_ONLY_					0

// Wy³¹czenie warningów o niezgodnoœci z ISO
//#define _CRT_SECURE_NO_DEPRECATE
//#define _CRT_NON_CONFORMING_SWPRINTFS

//################################################################
/// Jakiej czcionki u¿yæ w konsoli? 
/*! Jakiej czcionki (sposobu rysowania) u¿yæ w konsoli?
Je¿eli oba makra dezaktywowane to u¿yta zostajnie CSDLFont. Zalecane u¿ycie CSDLFont. */
#define _USE_CONSOLE_WITH_MY_FONT_		0
#define _USE_CONSOLE_WITH_BITMAP_FONT_	0

// Je¿eli zdefiniowane oba _USE_CONSOLE_WITH_MY_FONT_ i _USE_CONSOLE_WITH_BITMAP_FONT_ to u¿ywamy CSDLFont
#if _USE_CONSOLE_WITH_MY_FONT_
	#if _USE_CONSOLE_WITH_BITMAP_FONT_
		#undef _USE_CONSOLE_WITH_MY_FONT_
		#undef _USE_CONSOLE_WITH_BITMAP_FONT_
	#endif
#endif
//################################################################

/// Czy u¿yæ profilera?
/** Wszelkie wywo³ania funkcji CProfiler::Begin oraz CProfiler::End dobrze jest wywo³ywaæ za pomoc¹ makr
PROFILER_BEGIN oraz PROFILER_END. Wówczas przypisuj¹c _USE_PROFILER_ 1 lub 0 w ³atwy sposób bêdziemy mogli aktywowaæ
i dezaktywowaæ profiler w aplikacji. */
#define _USE_PROFILER_					0

#if _USE_PROFILER_
	#define PROFILER_BEGIN( strDesc )	CProfiler::GetInstance()->Begin( strDesc );
	#define PROFILER_END()				CProfiler::GetInstance()->End();
	#define PROFILER_FLUSH()			CProfiler::GetInstance()->Flush();
#else
	#define PROFILER_BEGIN( strDesc )
	#define PROFILER_END()
	#define PROFILER_FLUSH()
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////// define-y do gry /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//czy uzywac paska ladowania przy uruchomieniu programu
#define _USE_LOAD_PROGESS_BAR_			1

#define MAX_HIGHSCORE_NICKNAME_LEN		16 //dlugosc nazy uzytkownika podawana w "Enter your name"

#define MOUSE_SPEED_FOR_GAME			16 //zmiania ustawienia systemowe w panelu sterowania dla szybkosci myszy, na czas gry ustawiamy szybkosc na 16 z przedzialu od 1 do 20
#define _MY_SYSTEM_MOUSE_SPEED_			1 //zdefinuj aby na czas gry zmienic szybkosc myszy na MOUSE_SPEED_FOR_GAME

#define MAX_DISTANCE_VIEW				7000.0f

#define MAX_GAME_DISTANCE				-2300.0f
#define MIN_GAME_DISTANCE				250.0f

//obszar poruszania sie garcza
#define PLAYER_ZONE_RIGHT_X				121.0f //161.0f
#define PLAYER_ZONE_LEFT_X				-PLAYER_ZONE_RIGHT_X
#define PLAYER_ZONE_MAX					95.0f //w sensie masymalnie oddalone (czyli w glab ekranu)
#define PLAYER_ZONE_MIN					195.0f //w sensie minimalnie oddalone (czyli blizej ekranu)
#define MULTIPLAYER_START_POSX			80.0f

#define BOSS_ZONE_RIGHT_X				242.0f
#define BOSS_ZONE_LEFT_X				-BOSS_ZONE_RIGHT_X
#define BOSS_ZONE_MAX					-600.0f	//w gloab ekranu
#define BOSS_ZONE_MIN					-500.0f		//blizej ekranu

//predkosc lotu - przewijania sie statkow, a tym samym przesuwania sie np. bonusow
#define PLAYER_FLY_SPEED				210.0f
#define REMOTE_ROCKET_POWER				300 //sila zdalnej rakiety
#define TOTAL_LENGTH_REMOTE_ROCKET		14.91f //calkowita dlugosc obiektu 3D - remote rocket
#define COLLISION_A_REMOTE_ROCKET		TOTAL_LENGTH_REMOTE_ROCKET / 2.0f
#define TOTAL_WIDTH_REMOTE_ROCKET		5.63f
#define COLLISION_B_REMOTE_ROCKET		TOTAL_WIDTH_REMOTE_ROCKET / 2.0f

#define MAX_ENEMY_SHIPS_ON_SCREEN		32
#define MAX_HOMING_ROCKETS_ARRAY		128
#define MAX_EMITTER_ROCKETS_ARRAY		128

#define NEEDLE_3DOBJ_INDEX				2
#define REMOTE_ROCKET_3DOBJ_INDEX		11
#define SHIELD_3DOBJ_INDEX				12
#define SHOCKWAVE_3DOBJ_FIRST_INDEX		13 //indeks pierwszego obiektu fali uderzeniowej
#define SHOCKWAVE_3DOBJ_COUNT			5 //ilosc obiektow animacji fali uderzeniowej
#define SHOCKWAVE_3DOBJ //<- bez tego, rysowane kwadrykami

#ifndef SHOCKWAVE_3DOBJ
	#define _USE_QUADRIC_
#endif

//#define SHADOW_TEST //rysuje ziemie i cien statku gracza
//#define SHADOW_TEST_ENEMY //rysuj cienie wrogich statkow
#define DRAW_TUNNEL_FOR_LEVEL5

#ifdef SHADOW_TEST
	#define GROUND_VARIABLES
#elif defined( DRAW_TUNNEL_FOR_LEVEL5 )
	#define GROUND_VARIABLES
#endif

//jezeli nie zdefiniowane to rysuje oslone metoda 2d
#define DRAW_PLAYER_SHIELD_3D

// dla multiplayer
#define _USE_MULTIPLYER_				0
#define MAX_MULTIPLAYER_NICKNAME		15

#define INDEX_BIG_SHIP_BOSS_01			7
#define DISTANCE_BIG_SHIP_BOSS			-130.0f

#define DEFAULT_SOUND_VOLUME			90
#define DEFAULT_MUSIC_VOLUME			60

#endif //_DEFINE_H_
