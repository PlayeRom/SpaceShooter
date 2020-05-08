/*! \file
	\brief Plik nag��wkowy dla klasy CWindowData.
*/

#pragma once

#include "../Extensions/ARBMultisample.h"
#include "SingletonBase.h"
#include "Joystick.h"

#define ID_TIMER 1 ///< Identyfikator timer-a okna aplikacji.

class CLogger;
class CMultiLanguage;

/// Klasa okna aplikacji.
/*! Klata ta odpowiada za tworzenie i usuwanie okna oraz co wa�ne przechowuje r�ne informacje takie jak:
     - rozdzielczo�� ekranu,
     - g��bia kolor�w,
     - kolor t�a,
     - nazw� okna i klasy okna,
     - aktywno�� antyaliasingu, trybu pe�noekranowego, wy�wietlanie kursora
 */
class CWindowData : public CSingletonBase <CWindowData>
{
public:
	/// Struktura przechowuj�ca ustawienia aplikacji.
	/*! S� tu przechowywane r�ne opcje, kt�re mog� by� modyfikowane w trakcie dzia�ania apliakcji.
	Je�eli planujemy doda� wi�cej opcji konfiguracyjnych (np. w��czenie/wy��czenie d�wi�ku, wyb�r j�zyka itp.),
	to nale�y doda� je do struktury SSettingFile. Zapewni to automatyczny zapis i odczyt tych�e ustawie�. \n
	Dodaj�c z czasem nowe pola nale�y tak�e pami�ta� o zmianie numerowania wersji pliku. Dzi�ki temu unikniemy
	sytuacji wczytania przez u�ytkownika starszego pliku konfiguracyjnego, kt�ry b�dzie zawiera� starsz� wersj�
	struktury. Dla zmiany wersji pliku konfiguracyjnego, patrz plik define.h. */
	struct SSettingFile
	{
		GLubyte ubMajorVersion;		///< G��wna wersja pliku konfiguracyjnego.
		GLubyte ubMinorVersion;		///< Pomniejsza wersja pliku konfiguracyjnego.
		GLubyte ubReleaseVersion;	///< Trzecia sk�adowa wersji pliku konfiguracyjnego.
		//
		GLint iWidth;				///< Szeroko�� okna aplikacji - rozdzielczo�� X.
		GLint iHeight;				///< Wysoko�� okna aplikacji - rozdzielczo�� Y.
		GLubyte ubBitsPerPixel;		///< G��bia kolor�w.
		GLint iAntialiasing;		///< Ilo�� sampli dla antyaliasingu.
		GLboolean bFullScreen;		///< Czy aplikacja ma pracowa� w trybie pe�noekranowym.
		GLboolean bVSync;			///< Czy w��czona jest synchronizacja pionowa.
		GLboolean bClearOldLog;		///< Czy stary plik log-a ma by� usuwany przy kolejnym uruchomieniem aplikacji.
		GLint iIndexLanguage;		///< Indeks pliku j�zykowego aplikacji.
		GLboolean bSound;			///< Czy d�wi�k ma by� w��czony.
		GLboolean bMusic;			///< Czy muzyka ma by� w��czona.
		GLint iVolumeSound;			///< G�o�no�� d�wi�k�w okre�lona w procentach.
		GLint iVolumeMusic;			///< G�o�no�� muzyki okre�lona w procentach.
		GLint iBrightness;			///< Jasno��, od -255 (czer�) do 255 (biel), 0 - normalnie.
		GLint iHudColor;
		GLboolean bShadows;
		GLboolean bUseJoy;
		DWORD aButtonBindKey[ MAX_JOY_BUTTONS ];	// VK_ podpiete pod dany index, gdzie index 0 = JOY_BUTTON1, index 1 = JOY_BUTTON2
	};

	/// Statyczna metoda tworz�ca obiekt klasy CWindowData. 
	/*! CWindowData jest singletonem, wi�c je�eli wywo�amy t� metod� po raz drugi to zwr�ci ona wska�nik na obiekt
	ju� istniej�cy, utworzony wcze�niej.
	\return Wska�nik na obiekt CWindowData. */
	static CWindowData* Construct();

	/// Inicjalizacja obiektu CWindowData.
	/*! Tutaj tworzone s� wszelkie obiekty innych klas oraz ustawiane pocz�tkowe warto�ci zmiennych.
	\return \p GL_TRUE w przypadku powodzenia inicjalizacji wszelkich obiekt�w. \p GL_FALSE w
	przypadku wyst�pienia jakiego� b��du. */
	GLboolean Initialization();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CWindowData();

	/// G��wna metoda tworz�ca okno aplikacji.
	/*! \param[in] pWndProc Wska�nik na funkcje \ref WndProc.
	\param[in] iIcon ID zasobu ikonki jaka zostanie przypisana aplikacji.
	\param[in] iCursor ID zasobu kursora jaki zostanie wykorzystany w aplikacji.
	\return \p GL_TRUE w przypadku powodzenia utworzenia okna. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean CreateGLWindow( WNDPROC *pWndProc, const GLint iIcon, const GLint iCursor );

	/// Metoda niszcz�ca okno aplikacji.
	GLvoid KillGLWindow();

	/// Metoda restaru okna.
	/*! Metod� t� nale�y wywo�a� je�eli wymagany jest restart okna, np. pod wp�ywem zmiany rozdzielczo�ci, g��bi
	kolor�w, pr�bkowania anyatliasingu, prze��czaj�c si� z trybu okienkowego na pe�no-ekranowy i na odwr�t.
	\sa m_bRestart, SetRestart, IsRestart. */
	GLvoid RestartWindow();

	/// Metoda prze��czaj�ca pomi�dzy aplikacj� okienkow� a pe�no-ekranow�.
	GLvoid FullscreenSwitch();

	/// Metoda ustawiaj�ca zmienn� nakazuj�c� restartowa� okno.
	/*! \param[in] bRestart \p GL_TRUE aby restartowa� okno, \p GL_FALSE aby nie restartowa�.
	\sa m_bRestart, IsRestart, RestartWindow. */
	inline GLvoid SetRestart( GLboolean bRestart )	{ m_bRestart = bRestart; }

	/// Metoda sprawdzaj�ca czy nale�y zrestartowa� okno aplikacji.
	/*! \return \p GL_TRUE je�eli nale�y restartowa� okno, \p GL_FALSE je�eli nie.
	\sa m_bRestart, SetRestart, RestartWindow. */
	inline GLboolean IsRestart()					{ return m_bRestart; }

	/// Metoda zwracaj�ca struktur� SSettingFile przechowuj�c� ustawienia programu.
	/** \return Referencja na struktur� SSettingFile. */
	inline SSettingFile& SettingFile()				{ return m_sSettingFile; }

	/// Metoda wczytuj�ca plik konfiguracyjny.
	/*! Zwszelkia konfiguracja wczytana z pliku zostanie zapisana do struktury CWindowData::m_sSettingFile.
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku konfiguracyjnego. \p GL_FALSE je�eli wyst�pi� jaki� b��d.
	\sa SaveSettingFile, SETTING_FILE_NAME, m_sSettingFile. */
	GLboolean LoadSettingFile();

	/// Metoda zapisuj�ca plik konfiguracyjny.
	/*! Do pliku konfiguracyjnego zostanie zapisana struktura CWindowData::m_sSettingFile.
	\param[in] bDefaultSave Je�eli podamy \p GL_TRUE to zostan� zapisane ustawienia domy�lne
	a nie rzeczywiste jakie s� aktualnie przechowywane w CWindowData::m_sSettingFile. Ustawienia
	domy�lne zapisywane s� tylko przy pierwszym uruchomieniu aplikacji. Dalej nale�y
	zapisywa� ustawienia rzeczywiste czyli skorzysta� w domy�lnej warto�ci parametru: \p GL_FALSE.
	\return \p GL_TRUE w przypadku powodzenia zapisu pliku konfiguracyjnego. \p GL_FALSE je�eli wyst�pi� jaki� b��d.
	\sa LoadSettingFile, SETTING_FILE_NAME, m_sSettingFile. */
	GLboolean SaveSettingFile( GLboolean bDefaultSave = GL_FALSE );
	GLvoid CleanUpHighscore();

	GLboolean m_bCharModeKeyboard; ///< Je�eli \p GL_TRUE to komunikaty WM_KEYDOWN maj� by� ignorowane, bo wtedy dzia�a WM_CHAR.

	/// Metoda sprawdzaj�ca czy program zosta� uruchomiony z danym parametrem.
	/*! \param[in] lpParam Tekst jaki chcemy sprawdzi�, czy zosta� zawarty w parametrze programu.
	\return \p GL_TRUE je�eli lpParam zostanie znaleziony w parametrze uruchomionym wraz z programem. W przeciwnym
	wypadku \p GL_FALSE. */
	GLboolean IsCommandLine( LPCTSTR lpParam );

	/// Pobranie aktualnej ilo�ci sampli dla antyaliasingu.
	/*! \return Aktualna ilo�� sampli dla anyaliasingu.
	\sa CARBMultisample, SetAntialiasingSamples, m_pARBMultisample. */
	inline GLint GetAntialiasingSamples() { return m_pARBMultisample->GetSamples(); }

	/// Ustawienie aktualnej ilo�ci sampli dla antyaliasingu.
	/*! \param[in] iSamples Ilo�� sampli jak� chcemy ustawi�.
	\param[in] bRestart Czy od razu restartowa� okno? Je�eli podamy \p GL_TRUE to okno aplikacji zostanie zrestartowane.
	\sa CARBMultisample, GetAntialiasingSamples, m_pARBMultisample. */
	GLvoid SetAntialiasingSamples( GLint iSamples, GLboolean bRestart = GL_FALSE );

	//! Przeskalowanie okna OpenGL.
	/*! \param[in] fViewAngle K�t pola widzenia przekazywany do funkcji \a gluPerspective. */
	GLvoid ReSizeGLScene( GLfloat fViewAngle = 45.0f );

	/// Przeskalowanie okna OpenGL.
	/*! \param[in] iWidth Szeroko�� okna - rozdzielczo�� X.
	\param[in] iHeight Wysoko�� okna - rozdzielczo�� Y.
	\param[in] fViewAngle K�t pola widzenia przekazywany do funkcji \a gluPerspective. */
	GLvoid ReSizeGLScene( GLsizei iWidth, GLsizei iHeight, GLfloat fViewAngle = 45.0f );

	/// \name Gettery dla danych obiekt�w
	//@{
	inline CLogger*				GetLogger()		{ return m_pLogger; }
	inline CMultiLanguage*		GetMultiLang()	{ return m_pMultiLang; }
	inline HDC					GetHDC()		{ return m_hDC; }
	inline HINSTANCE			GetHINSTANCE()	{ return m_hInstance; }
	inline HWND					GetHWND()		{ return m_hWnd; }
	inline HGLRC				GetHRC()		{ return m_hRC; }
	//@}

	/// \name Settery i gettery dla podw�jnego restartu
	//@{ 
	//- naprawa bug 12
	inline GLvoid		SetDoubleRestart( GLboolean bParam )	{ m_bDoubleRestart = bParam; }
	inline GLboolean	IsDoubleRestart()						{ return m_bDoubleRestart; }
	inline GLvoid		SetResDoubleRestartX( GLint iResX )		{ m_iResDoubleRestartX = iResX; }
	inline GLint		GetResDoubleRestartX()					{ return m_iResDoubleRestartX; }
	inline GLvoid		SetResDoubleRestartY( GLint iResY )		{ m_iResDoubleRestartY = iResY; }
	inline GLint		GetResDoubleRestartY()					{ return m_iResDoubleRestartY; }
	//@}

	/// \name Settery i gettery dla r�nych p�l
	//@{ 
	inline GLboolean IsUseAntialiasing()	{ return m_bUseAntialiasing; }
	inline GLvoid SetUseAntialiasing( GLboolean bAntialiasing ) { m_bUseAntialiasing = bAntialiasing; }
	inline GLfloat GetBkColorR()			{ return m_fBkColor[ 0 ]; }
	inline GLfloat GetBkColorG()			{ return m_fBkColor[ 1 ]; }
	inline GLfloat GetBkColorB()			{ return m_fBkColor[ 2 ]; }
	inline GLfloat GetBkColorA()			{ return m_fBkColor[ 3 ]; }
	inline GLubyte GetZBufferBits()			{ return m_ubZBufferBits; }
	inline GLint GetDesktopWidth()			{ return m_iDesktopWidth; }
	inline GLint GetDesktopHeight()			{ return m_iDesktopHeight; }
	//@}

	inline GLint GetSystemMouseSpeed() { return m_iSystemMouseSpeed; }

protected:
	/// Konstruktor.
	/*! Konstruktor. */
	CWindowData();

	HGLRC			m_hRC;				///< Sta�y kontekst renderowania.
	HDC				m_hDC;				///< Prywatny kontekst urz�dzenia GDI.
	HWND			m_hWnd;				///< Przechowuje uchwyt okna.
	HINSTANCE		m_hInstance;		///< Przechowuje instancj� aplikacji.

	SSettingFile	m_sSettingFile;		///< Struktura przechowuj�ca wczelk� konfiguracj� programu.
	
	/// \name Wska�niki na wszelkie obiekty jakie s� tworzone w CWindowData.
	//@{
	CLogger			*m_pLogger;			///< Wska�nik na obiekt klasy CLogger.
	CARBMultisample	*m_pARBMultisample;	///< Wska�nik na obiekt klasy CARBMultisample.
	CMultiLanguage	*m_pMultiLang;		///< Wska�nik na obiekt klasy CMultiLanguage.
	//@}
	WNDPROC			*m_pWndProc;			///< Wska�nik na funkcj� \ref WndProc.

	GLboolean m_bRestart;	///< Czy okno b�dzie restartowane. Zmienna wykorzystywana w g��wnej p�tli programu.

	TCHAR *m_lpTitle;		///< Tytu� okna aplikacji.
	TCHAR *m_lpClassName;	///< Nazwa klasy aplikacji.

	GLint m_iIcon;			///< ID ikony aplikacji.
	GLint m_iCursor;		///< ID kursora aplikacji.

	//dla podw�jnego restartu - naprawa bug 12
	GLboolean m_bDoubleRestart; ///< Czy wymagany podw�jny restart okna. 
	GLint m_iResDoubleRestartX;	///< Oczekiwana szeroko�� (rozdzielczo�� X) okna aplikacji po podw�jnym restarcie.
	GLint m_iResDoubleRestartY; ///< Oczekiwana wysoko�� (rozdzielczo�� Y) okna aplikacji po podw�jnym restarcie.

	GLfloat m_fBkColor[ 4 ];			///< Kolor t�a RGB + alfa.
	GLuint m_uiTimer;					///< Cz�stotliwo�� tikania timera w milisekundach. Domy�lnie 1000, je�eli ustawimy na 0 to timer nieaktywny.
	GLboolean m_bUseAntialiasing;		///< Czy aktywny pe�noekranowy antyaliasing.
	GLboolean m_bShowMouseCursor;		///< Czy kursor myszy ma by� pokazywany - tylko gdy fullscreen.
	GLubyte m_ubBitsPerPixel;			///< G��bia kolor�w pulpitu.
	GLubyte m_ubZBufferBits;			///< Bufor g��bi.
	GLint m_iDesktopWidth;				///< Szeroko�� pulpitu - rozdzielczo�� X.
	GLint m_iDesktopHeight;				///< Wysoko�� pulpitu - rozdzielczo�� Y.

	GLint m_iSystemMouseSpeed;

	/// \name Metody wywo�ywane przy tworzeniu okna.
	//@{ 

	/// Metoda pobieraj�ca instancj� naszego programu.
	/*! Metoda ustawia uchwyt CWindowData::m_hInstance instancj� naszej aplikacji.
	\return \p GL_TRUE w przypadku powodzenia pobrania instancji. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean CreateInstance();

	/// \brief Metoda tworz�ca oraz rejestruj�ca klas� okna naszej aplikacji.
	/*! \return \p GL_TRUE w przypadku powodzenia. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean RegisterMyClass();

	/// Metoda ustawiaj�ca tryb urz�dzenia graficznego na tryb pe�noekranowy.
	/*! Pr�ba ustawienia trybu pe�noekranowego b�dzie realizowana tylko wtedy je�eli flaga SSettingFile::bFullScreen
	ustawiona b�dzie na \p GL_TRUE. \n
	Je�eli pr�ba ustawienia urz�dzenia graficznego na tryb pe�noekranowy zako�czy si� niepowodzeniem, aplikacja
	zostanie uruchomiona w trybie okna oraz flaga SSettingFile::bFullScreen zostanie ustawiona na \p GL_FALSE. */
	GLvoid SetDeviceOnFullscreen();

	/// Metoda tworz�ca okno naszej aplikacji.
	/*! Metoda ustawia uchwyt CWindowData::m_hWnd.
	\return \p GL_TRUE w przypadku powodzenia utworzenia okna. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean CreateMyWindow();

	/// Metoda pobieraj�ca kontekst urz�dzenia naszej aplikacji.
	/*! Metoda ustawia zmienn� CWindowData::m_hDC.
	\return \p GL_TRUE w przypadku powodzenia pobrania kontekstu urz�dzenia. \p GL_FALSE w przypadku wyst�pienia
	jakiego� b��du. */
	GLboolean GetDeviceContent();

	/// Metoda ustawiaj�ca format piksela.
	/*! Je�eli aplikacja wykorzystuje pe�noekranowy antyaliasing to format piksela zostanie wybrany z klasy
	CARBMultisample. Je�eli nie to format piksela zostanie wybrany z kontekstu urz�dzenia.
	\return \p GL_TRUE w przypadku znalezienia pasuj�cego formatu piksela. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean SetPixelFormat();

	/// Metoda tworz�ca kontekst renderowania.
	/*! Metoda ustawia zmienn� CWindowData::m_hRC.
	\return \p GL_TRUE w przypadku powodzenia. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean CreateRenderingContext();
	//@}
};