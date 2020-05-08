/** \file
	\brief Plik nag��wkowy dla klasy CSDLFont.
*/
//////////////////////////////////////////////////////////////////////

#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

class CWindowData;

/// Klasa renderuj�ca tekst.
/*! Klasa umo�liwiaj�ca rysowanie tekstu przy u�yciu biblioteki SDL_ttf. Aby umo�liwi� wy�wietlanie polskich znak�w
nale�y zbudowa� aplikacj� w \p UNICODE (domy�lnie OpenGLFramework u�ywa ju� \p UNICODE). */
class CSDLFont  
{
public:
	/// Konstruktor.
	/*! \param[in] lpFontFile Nazwa pliku z czcionk� jakiej chcemy u�y� (domy�lnie \p "fonts/FreeSansBold.ttf").
	OpenGLFramework zawiera w�asne darmowe czcionki � umieszczone s� one w katalogu \a fonts. Mo�na tak�e wczyta�
	pliki zawarte w katalogu \a C:\\Windows\\Fonts, nale�y jednak pami�ta�, �e musimy poda� nazw� pliku a nie nazw�
	zarejestrowanej czcionki.
	\param[in] iSizeFont Wielko�� czcionki.
	\param[in] bUseStorageBuffer Ustawiaj�c t� flag� na \p GL_TRUE, nakazujemy buforowania wy�wietlanych tekst�w, dzi�ki
	czemu, rendering teksu b�dzie wykonywany znacznie szybciej. Oznacza to tak�e troch� wi�ksze absorbowanie pami�ci.
	Ustawiaj�c na \p GL_FALSE, ka�de to samo wywo�anie rysowania tekstu, b�dzie renderowane od nowa.
	\sa SStorageBuffer.	*/
	CSDLFont( const char *lpFontFile = "fonts/FreeSansBold.ttf", GLint iSizeFont = 20, GLboolean bUseStorageBuffer = GL_TRUE );

	/// Destruktor.
	/*! Destruktor */
	virtual ~CSDLFont();

	/// \name Zestaw metod rysuj�cych tekst.
	//@{

	/// Metoda rysuj�ca tekst.
	/*! \param[in] iPosX \param[in] iPosY Pozycja tekstu wzgl�dem ekranu. Punkt \p 0x0 znajduje si� w lewym g�rnym
	rogu ekranu (tekst jest rysowany "na p�aszczy�nie ekranu"). Wa�ne jest aby rysowa� tekst tak jakby mia� by�
	rozmieszczony dla ekranu \p 1024x768 � w�wczas b�dziemy mieli prawid�owe skalowanie tekstu wzgl�dem
	rozdzielczo�ci. Oznacza to, �e je�eli stosujemy rozdzielczo��, np. \p 800x600 i chcemy wy�wietli� tekst na
	samym spodzie okna, to nie ustawiamy \a iPosY na \p 600 tylko na \p 768!
	\param[in] crColor Sk�adowe koloru tekstu. Nale�y u�y� makra \p RGB().
	\param[in] lpText Tekst jaki chcemy wy�wietli�.
	\param[in] bMidScreen Czy tekst ma by� wyr�wnany do �rodka. Je�eli ustawimy t� warto�� na \p GL_TRUE to tekst
	b�dzie wyr�wnany wzgl�dem �rodka ekranu. Oznacza to, �e warto�� \a iPosX nie b�dzie brana pod uwag�, poniewa�
	program sam j� wyliczy. Gdy podamy \p GL_FALSE (warto�� domy�lna), w�wczas tekst b�dzie pozycjonowany na
	podstawie \a iPosX.	*/
	GLvoid DrawText( GLint iPosX, GLint iPosY, COLORREF crColor, LPCTSTR lpText, GLboolean bMidScreen = GL_FALSE );

	/// Metoda rysuj�ca tekst z mo�liwo�ci� formatowania.
	/*! \param[in] iPosX \param[in] iPosY Pozycja tekstu wzgl�dem ekranu. Punkt \p 0x0 znajduje si� w lewym g�rnym
	rogu ekranu (tekst jest rysowany "na p�aszczy�nie ekranu"). Wa�ne jest aby rysowa� tekst tak jakby mia� by�
	rozmieszczony dla ekranu \p 1024x768 � w�wczas b�dziemy mieli prawid�owe skalowanie tekstu wzgl�dem
	rozdzielczo�ci. Oznacza to, �e je�eli stosujemy rozdzielczo��, np. \p 800x600 i chcemy wy�wietli� tekst na
	samym spodzie okna, to nie ustawiamy \a iPosY na \p 600 tylko na \p 768!
	\param[in] bMidScreen Czy tekst ma by� wyr�wnany do �rodka. Je�eli ustawimy t� warto�� na \p GL_TRUE to tekst
	b�dzie wyr�wnany wzgl�dem �rodka ekranu. Oznacza to, �e warto�� \a iPosX nie b�dzie brana pod uwag�, poniewa�
	program sam j� wyliczy. Gdy podamy \p GL_FALSE (warto�� domy�lna), w�wczas tekst b�dzie pozycjonowany na
	podstawie \a iPosX.
	\param[in] crColor Sk�adowe koloru tekstu. Nale�y u�y� makra \p RGB().
	\param[in] fmt Tekst jaki chcemy wy�wietli�.
	\param[in] ... Opcjonalne parametry dla formatowania.	*/
	GLvoid DrawTextFormat( GLint iPosX, GLint iPosY, GLboolean bMidScreen, COLORREF crColor, LPCTSTR fmt, ... );

	/// Metoda rysuj�ca tekst w prostok�cie.
	/*! \param[in] rcRectangle Prostok�t opisuj�cy po�o�enie tekstu. Pozycja tekstu zostanie wyliczona tak aby tekst
	narysowa� si� dok�adanie na �rodku prostok�ta. Wa�ne jest aby pola struktury \p RECT zawiera�y pozycj� prostok�ta
	w pikselach dla rozdzielczo�ci ekranu \p 1024x768 � w�wczas b�dziemy mieli prawid�owe skalowanie tekstu wzgl�dem
	rozdzielczo�ci.
	\param[in] crColor Sk�adowe koloru tekstu. Nale�y u�y� makra \p RGB().
	\param[in] lpText Tekst jaki chcemy wy�wietli�.	*/
	GLvoid DrawText( const RECT &rcRectangle, COLORREF crColor, LPCTSTR lpText );

	/// Metoda rysuj�ca tekst w prostok�cie z mo�liwo�ci� formatowania.
	/*! \param[in] rcRectangle Prostok�t opisuj�cy po�o�enie tekstu. Pozycja tekstu zostanie wyliczona tak aby tekst
	narysowa� si� dok�adanie na �rodku prostok�ta. Wa�ne jest aby pola struktury \p RECT zawiera�y pozycj� prostok�ta
	w pikselach dla rozdzielczo�ci ekranu \p 1024x768 � w�wczas b�dziemy mieli prawid�owe skalowanie tekstu wzgl�dem
	rozdzielczo�ci.
	\param[in] crColor Sk�adowe koloru tekstu. Nale�y u�y� makra \p RGB().
	\param[in] fmt Tekst jaki chcemy wy�wietli�.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid DrawTextFormat( const RECT &rcRectangle, COLORREF crColor, LPCTSTR fmt, ... );
	//@}

	/// Metoda ustawiaj�ca wy�wietlanie t�a pod tekstem.
	/*! \param[in] bDrawBkgText Je�eli podamy \p GL_TRUE to pod ka�dym tekstem b�dzie wy�wietlane t�o. Podaj�c
	\p GL_FALSE	t�o nie b�dzie rysowane.
	\param[in] crColor Sk�adowe koloru t�a. Nale�y u�y� makra \p RGB(). Kolor b�dzie brany pod uwag� tylko wtedy je�eli
	\a bDrawBkgText ustawimy na \p GL_TRUE.	*/
	GLvoid SetBkgText( GLboolean bDrawBkgText, COLORREF crColor = RGB( 0, 0, 0 ) );

	/// Metoda pobieraj�ca wysoko�� czcionki w pikselach.
	/*! \return Liczba ca�kowita okre�laj�ca wysko�� czcionki w pikselach. W przypadku b��du metoda zwr�ci \p -1. */
	GLint GetHeightFont();

	/// \name Metody dla StorageBuffer-a.
	//@{

	/// Metoda czyszcz�ca ca�y StorageBuffer dla danego obiektu klasy CSDLFont.
	/*! \param[in] bLog Dodatkowy parametr dla rejestrowania do pliku. Podaj�� \p GL_TRUE,
	zapisana zostanie do pliku informacja o ilo�ci usuni�tych pozycji bufora (ilo�� sk�adowanych i usuni�tych tekst�w).
	Je�eli podamy \p GL_FALSE, to pomimo aktywno�ci logger-a wspomniana informacja nie zostanie zapisana do pliku.
	\sa SStorageBuffer, CLogger. */
	GLvoid ClearStorageBuffer( GLboolean bLog = GL_TRUE );

	/// Metoda sprawdzaj�ca czy StorageBuffer jest aktywny.
	/*! \return \p GL_TRUE je�eli u�ywany jest StorageBuffer, w przeciwnym wypadku \p GL_FALSE.
	\sa SStorageBuffer, SetStorageBuffer, m_bUseStorageBuffer. */
	inline GLboolean IsStorageBuffer() { return m_bUseStorageBuffer; }

	//// Metoda ustawiaj�ca czy StorageBuffer ma my� u�ywany czy te� nie.
	/*! Za pomoc� tej metody mo�emy w��czy� lub wy��czy� u�ycie StorageBuffer w trakcie "�ycia" obiektu.
	\param[in] bUse Podaj�c \p GL_TRUE uaktywniamy StorageBuffer, podaj�c \p GL_FALSE wy��czamy go.	*/
	inline GLvoid SetStorageBuffer( GLboolean bUse ) { m_bUseStorageBuffer = bUse; }
	//@}

	/// Metoda timer-a wywo�ywana cyklicznie (domy�lnie co sekund�) przez metod� CGameControl::Timer().
	/*! W metodzie tej usuwane s� stare, nieu�ywane teksty ze StorageBuffera. */
	GLvoid Timer();

	/// Metoda zwracaj�ca kompilowan� wersj� bibliotek \p SDL i \p SDL_ttf.
	/*! \return �a�cych znak�w zawieraj�cy nast�puj�ce informacje: g�owny numer wersji \p SDL, pomniejszy numer wersji
	\p SDL, numer �atki \p SDL, nast�pnie mamy g�owny numer wersji \p SDL_ttf, pomniejszy numer wersji \p SDL_ttf
	oraz numer �atki \p SDL_ttf. */
	LPTSTR GetCompileVersionSLD_ttf();

	/// Metoda zwracaj�ca linkowan� wersj� bibliotek \p SDL i \p SDL_ttf.
	/*! \return �a�cych znak�w zawieraj�cy nast�puj�ce informacje: g�owny numer wersji \p SDL, pomniejszy numer wersji
	\p SDL, numer �atki \p SDL, nast�pnie mamy g�owny numer wersji \p SDL_ttf, pomniejszy numer wersji \p SDL_ttf
	oraz numer �atki \p SDL_ttf. */
	LPTSTR GetLinkedVersionSLD_ttf();

private:
	/// Struktura StorageBuffer wspomagaj�ca rysowanie tekstu za pomoc� klasy CSDLFont.
	/*! Aby zoptymalizowa� rendering tekstu, ka�dy obiekt klasy CSDLFont zwiera zmienn� CSDLFont::m_bUseStorageBuffer
	(ustawian� przez konstruktor CSDLFont) m�wi�c� o tym czy nale�y zapami�tywa� wcze�niej wyrenderowane teksty.
	Je�eli StorageBuffer jest w u�yciu to przy ka�dym wywo�aniu metod CSDLFont::DrawText czy CSDLFont::DrawTextFormat,
	najpierw sprawdzane jest czy podany tekst nie zosta� ju� wcze�niej wyrenderowany (sprawdzany jest tekst i jego kolor).
	Je�eli nie, to podany tekst zostanie wyrenderowany na nowo oraz zapisany do bufora. Czyli nast�pnym razem przy
	kolejnym wywo�aniu CSDLFont::DrawText tekst (o ile jest identyczny) nie b�dzie musia� by� renderowany od nowa tylko
	u�yta zostanie wcze�niej przygotowana i buforowana tekstura. \n
	Je�eli StorageBuffer nie jest u�ywany to ka�de wywo�anie metod rysuj�cych tekst powoduje renderowanie tekstu od
	nowa co trwa znacznie d�u�ej. \n
	Aby zminimalizowa� zu�ycie pami�ci, podczas u�ywania StorageBuffer, ka�demu buforowanemu tekstowi przypisywany jest
	czas ostatniego odwo�ania si� do niego. Nast�pnie co dany kwarta� czasu (domy�lnie co 30 sekund) usuwane s� z bufora
	stare teksty, kt�rych czas ostatniego wykorzystania przekracza okre�lony limit (domy�lnie 30 sekund). \n
	Dodatkowo, ka�dy obiekt klasy CSDLFont zawiera limit na buforowanie tekst�w � domy�lnie obiekt mo�e buforowa�
	maksymalnie 200 tekst�w. */
	struct SStorageBuffer
	{
		TCHAR *lpText;			///< Renderowany tekst.
		GLuint uiTexture;		///< Gotowa tekstura z wyrenderowanym tekstem.
		GLint iWidthPowOfTwo;	///< Szeroko�� tekstury (w piselach) b�d�ca pot�g� dw�jki.
		GLint iHeightPowOfTwo;	///< Wysoko�� tekstury (w piselach) b�d�ca pot�g� dw�jki.
		GLint iOriginalWidth;	///< Szeroko�� tekstu w pikselach.
		GLint iOriginalHeight;	///< Wysoko�� tekstu w pikselach.
		SDL_Color sdlColor;		///< Struktura przechowuj�ca kolor tekstu.
		GLboolean bMidScreen;	///< Flaga okre�laj�ca czy tekst ma by� pozycjonowany na �rodku ekranu.
		GLuint uiRefTime;		///< Czas ostatniego u�ycia (w sekundach) od uruchomienia programu.
	};

	CWindowData	*m_pWinData;	///< Wska�nik na obiekt klasy CWindowData.
	TTF_Font *m_pTTFFont;		///< Wska�nik na obiekt struktury TTF_Font, reprezentuj�cej czcionk�.

	/// Metoda zwracaj�ca najbli�sz�, nast�pn� liczb� b�d�c� pot�g� dw�jki.
	/*! \param[in] iNumber Liczba, od kt�rej szukana b�dzie najbli�sza pot�ga dw�jki.
	\return Liczba b�d�ca pot�g� dw�jki. */
	GLint NextPowerOfTwo( GLint iNumber );

	/// Wst�pna metoda renderowania tekstu.
	/*! W metodzie tej podejmowana jest decyzja czy nele�y renderowa� tekst od nowa, czy skorzysta� ju� z istniej�cego,
	sk�adowanego w StorageBuffer.
	\param[in] lpText Tekst do wy�wietlenia.
	\param[in] pFont Wska�nik na struktur� \p TTF_Font reprezentuj�c� czcionk�.
	\param[in] in_sdlColor Obiekt struktury \p SDL_Color reprezentuj�cej sk�adowe koloru tekstu.
	\param[in,out] in_out_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj�cej po�o�enie tekstu.
	\param[in] bMidScreen Flaga wskazuj�ca czy tekst ma by� wyr�wnany do �rodka.
	\param[in] bIsRect Flaga wskazuj�ca czy tekst ma by� umieszczony w �rodku prostok�ta \p SDL_Rect.
	\sa SStorageBuffer.	*/
	GLvoid RenderText( LPCTSTR lpText, TTF_Font *pFont, const SDL_Color &in_sdlColor, SDL_Rect &in_out_sdlLocation, GLboolean bMidScreen, GLboolean bIsRect = GL_FALSE );

	/// Metoda sprawdzaj�ca czy dany tekst znajduje si� w StorageBuffer.
	/*!
	\param[in] lpText Tekst do wy�wietlenia.
	\param[in] in_sdlColor Obiekt struktury \p SDL_Color reprezentuj�cej sk�adowe koloru tekstu.
	\param[in] bMidScreen Flaga wskazuj�ca czy tekst ma by� wyr�wnany do �rodka.
	\param[out] out_iIndex Indeks elementu tablicy StorageBuffer-a, w kt�rym zosta� znaleziony tekst.
	\return \p GL_TRUE je�eli tekst znajduje si� w StorageBuffer, w przeciwnym wypadku \p GL_FALSE. Je�eli metoda
	zwr�ci \p GL_TRUE to parametr \a out_iIndex zostanie ustawiony numerem indeksu tablicy StorageBuffer-a.
	\sa SStorageBuffer.	*/
	GLboolean IsTextInStorageBuffer( LPCTSTR lpText, const SDL_Color &in_sdlColor, GLboolean bMidScreen, GLint &out_iIndex );

	/// Metoda renderuj�ca tekst ze StorageBuffer-a.
	/*! \param[in,out] in_out_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj�cej po�o�enie tekstu.
	\param[in] iIndex Indeks tablicy StorageBuffer-a, z kt�rej zostan� pobrane informacje do wyrenderowania tekstu.
	\param[in] bIsRect Flaga wskazuj�ca czy tekst ma by� umieszczony w �rodku prostok�ta \p SDL_Rect.
	\sa SStorageBuffer.	*/
	GLvoid RenderAlreadyExistText( SDL_Rect &in_out_sdlLocation, GLint iIndex, GLboolean bIsRect );

	/// Metoda renderuj�ca nowy tekst.
	/*! Je�eli u�ywany jest StorageBuffer to informacje o nowo wyrenderowanym tek�cie zostan� do niego zapisane.
	Dzi�ki temu przy kolejnym rysowaniu tego samego tekstu, nie b�dzie on musia� by� na nowo tworzony.
	\param[in] lpText Tekst do wy�wietlenia.
	\param[in] pFont Wska�nik na struktur� \p TTF_Font reprezentuj�c� czcionk�.
	\param[in] in_sdlColor Obiekt struktury \p SDL_Color reprezentuj�cej sk�adowe koloru tekstu.
	\param[in,out] in_out_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj�cej po�o�enie tekstu.
	\param[in] bMidScreen Flaga wskazuj�ca czy tekst ma by� wyr�wnany do �rodka.
	\param[in] bIsRect Flaga wskazuj�ca czy tekst ma by� umieszczony w �rodku prostok�ta \p SDL_Rect.
	\sa SStorageBuffer.	*/
	GLvoid RenderNewText( LPCTSTR lpText, TTF_Font *pFont, const SDL_Color &in_sdlColor, SDL_Rect &in_out_sdlLocation, GLboolean bMidScreen, GLboolean bIsRect );

	GLboolean m_bDrawBkgText;				///< Flaga wskazuj�ca czy ma by� rysowane t�o pod tekstem.
	GLubyte m_ubBkgR;						///< Sk�adowa \p R koloru t�a rysowanego pod tekstem.
	GLubyte m_ubBkgG;						///< Sk�adowa \p G koloru t�a rysowanego pod tekstem.
	GLubyte m_ubBkgB;						///< Sk�adowa \p B koloru t�a rysowanego pod tekstem.

	GLint m_iHeightFont;					///< Zmienna przechowuj�ca wysoko�� czcionki.
	static GLint ms_iCountRef;				///< Licznik zliczaj�cy ilo�� stworzonych instancji klasy CSDLFont.
	GLint m_iID;							///< Identyfikator (indeks tablicy g_aPointerSDLFont) danej instancji klasy CSDLFont.
	TCHAR m_tcVersion[ 256 ];				///< Tekst zwracany przez metody CSDLFont::GetCompileVersionSLD_ttf i CSDLFont::GetLinkedVersionSLD_ttf.

	std::vector<SStorageBuffer> m_aStorageBuffer;	///< Dynamiczna tablica StorageBuffer.
	GLboolean m_bUseStorageBuffer;			///< Flaga wskazuj�ca czy StorageBuffer jest u�ywany.
	GLuint m_uiCountTimer;					///< Licznik timer-a wskazuj�cy kiedy nale�y usn�� stare elementy StorageBuffer-a.

	/// Metoda usuwaj�ca jeden element StorageBuffer-a.
	/*! \param[in] iIndex Pozycja StorageBuffer-a do usuni�cia. */
	GLvoid DeleteOneStorageBuffer( GLint iIndex );

	/// Metoda usuwaj�ca stare, nieu�ywane elementy StorageBuffer-a.
	/*! Metoda ta wywo�ywana jest cyklicznie w metodzie CSDLFont::Timer co zadany interwa� czasu. */
	GLvoid DeleteOldStorageBuffer();

	/// Metoda pobieraj�ca ilo�� sk�adowanych element�w w StorageBuffer.
	/*! \return Ilo�� element�w tablicy CSDLFont::m_aStorageBuffer.	*/
	inline GLint GetStorageBufferSize() { return static_cast<GLint>( m_aStorageBuffer.size() ); }

	/// \name Metody pomocnicze przy renderowaniu teksu.
	//@{

	/// Metoda dopasowuj�ca rozmiar tekstu do aktualnie u�ywanej rozdzielczo�ci okna.
	GLvoid ScaleFont();

	/// Metoda obliczaj�ca pozycj� teksu na �rodku ekranu.
	/*! \param[in,out] in_out_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj�cej po�o�enie tekstu.
	\param[in] iIndex Indeks tablicy StorageBuffer-a, z kt�rej zostan� pobrane informacje o renderowanym tek�cie. */
	GLvoid MidScreenFont( SDL_Rect &in_out_sdlLocation, GLint iIndex );

	/// Metoda wyliczaj�ca po�o�enie tekstu w �rodku prostok�ta.
	/*! \param[in,out] in_out_sdlLocation Obiekt struktury prostok�ta, w kt�rym znajdowa� ma si� tekst.
	\param[in] iIndex Indeks tablicy StorageBuffer-a, z kt�rej zostan� pobrane informacje o renderowanym tek�cie. */
	GLvoid CalculatePosTextFromRect( SDL_Rect &in_out_sdlLocation, GLint iIndex );

	/// Metoda rysuj�ca t�o pod tekstem.
	/*! \param[in] in_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj�cej po�o�enie tekstu.
	\param[in] iIndex Indeks tablicy StorageBuffer-a, z kt�rej zostan� pobrane informacje o renderowanym tek�cie. */
	GLvoid DrawBkgText( const SDL_Rect &in_sdlLocation, GLint iIndex );

	/// Metoda rysuj�ca tekst.
	/*! \param[in] in_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj�cej po�o�enie tekstu.
	\param[in] iIndex Indeks tablicy StorageBuffer-a, z kt�rej zostan� pobrane informacje o renderowanym tek�cie. */
	GLvoid DrawQuadText( const SDL_Rect &in_sdlLocation, GLint iIndex );
	//@}

	GLuint m_uiInterval;	///< Interwa� czasu wskazuj�cy co ile sekund kasowa� stare elementy StorageBuffer-a.
	GLint m_iBufferLimit;	///< Limit buforowania tekst�w w StorageBuffer.
	GLuint m_uiTimeOut;		///< Ilo�� sekund przedawnienia StorageBuffer-a.

	CVector2*	m_aTexCoord;
	CVector2*	m_aIndices;
};

/// Globalna tablica wska�nik�w na wszelkie obiekty CSDLFont.
extern std::vector<CSDLFont*> g_aPointerSDLFont;