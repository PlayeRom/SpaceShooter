/*! \file
	\brief Plik nag��wkowy dla klasy CConsole.
*/
#pragma once

#include "RMenuBase.h"

class CBitmapFont;
class CSDLFont;
class CMyFont;
class CSpeedControl;

/// Makro dodaj�ce linijk� tekstu do konsoli w kodowaniu ASCII lub UNICODE.
/** \param[in] type Typ dodawanej informacji. 
\param[in] msg Tekst jaki chcemy wy�wietli� w konsoli.
\param[in] ... Opcjonalne parametry dla formatowania. */
#define CONSOLE_ADDTEXT( type, msg, ... ) CGameControl::GetInstance()->GetConsole()->AddText( type, msg, __VA_ARGS__ )

/// Makro dodaj�ce linijk� tekstu do konsoli w kodowaniu ASCII lub UNICODE z typem ENormal.
/** \param[in] msg Tekst jaki chcemy wy�wietli� w konsoli.
\param[in] ... Opcjonalne parametry dla formatowania. */
#define CONSOLE_ADDTEXT_NORMAL( msg, ... ) CGameControl::GetInstance()->GetConsole()->AddText( msg, __VA_ARGS__ )

/// Wielko�� bufora tekstu pojedynczej linii tekstu.
#define MAX_ADDTEXT_BUFFER 256

/// Klasa konsoli.
/** Klasa ta odpowiada za wy�wietlanie oraz obs�ug� konsoli. */
class CConsole : public CRMenuBase
{
public:

	/// Typ wyliczeniowy reprezentuj�cy rodzaje wiadomo�ci.
	/** Ka�d� informacj� wy�wietlan� w konsoli mo�na okre�li� danym typem przypisuj�c jej odpowiedni kolor.  */
	enum EConsoleMsgType
	{
		ENormal,		///< Tekst normalny w kolorze bia�ym.
		EWarning,		///< Ostrze�enie w kolorze ��tym.
		EError,			///< B��d w kolorze czerwonym.
		ESuccess		///< Sukces w kolorze zielonym.
	};

	/// Konstruktor.
	/** Konstruktor. */
	CConsole();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CConsole();

	/// Metoda restartuj�ca dane konsoli, kt�re tego wymagaj�.
	/** Metoda ta jest wywo�ywana gdy restartowane jest okno, np. pod wp�ywem zmiany rozdzielczo�ci, pr�bkowania
	dla antyaliasingu, itp.  */
	GLvoid RestartObjects();

	/// Metoda wy�wietlaj�ca konsol�.
	/** Nale�y wywo�a� t� metod� aby konsola by�a rysowana. */
	GLvoid ShowConsole();

	/// Metoda ukrywaj�ca konsol�.
	/** Nale�y wywo�a� t� metod� aby zaprzesta� rysowania konsoli. */
	inline GLvoid HideConsole() { m_bShowConsole = GL_FALSE; }

	/// Metoda sprawdzaj�ca czy konsola jest rysowana.
	/** \return \p GL_TRUE je�eli konsola jest rysowana. W przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean IsConsole() { return m_bShowConsole; }

	/// Metoda dodaj�ca linijk� tekstu do konsoli w kodowaniu ASCII.
	/** \param[in] eType Typ dodawanej informacji. 
	\param[in] fmt Tekst jaki chcemy wy�wietli� w konsoli.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid AddText(EConsoleMsgType eType, const char *fmt, ...);

	/// Metoda dodaj�ca linijk� tekstu do konsoli w kodowaniu ASCII z typem ENormal.
	/** \param[in] fmt Tekst jaki chcemy wy�wietli� w konsoli.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid AddText(const char *fmt, ...);

#ifdef UNICODE
	/// Metoda dodaj�ca linijk� tekstu do konsoli w kodowaniu UNICODE.
	/** \param[in] eType Typ dodawanej informacji. 
	\param[in] fmt Tekst jaki chcemy wy�wietli� w konsoli.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid AddText(EConsoleMsgType eType, const wchar_t *fmt, ...);

	/// Metoda dodaj�ca linijk� tekstu do konsoli w kodowaniu UNICODE z typem ENormal.
	/** \param[in] fmt Tekst jaki chcemy wy�wietli� w konsoli.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid AddText(const wchar_t *fmt, ...);
#endif

	/// Metoda rysuj�ca konsol�.
	/** Metoda rysuje konsol� z wykorzystaniem klasy CSDLFont. */
	GLvoid DrawConsoleWithSDLFont();

#if _USE_CONSOLE_WITH_BITMAP_FONT_
	/// Metoda rysuj�ca konsol�.
	/** Metoda rysuje konsol� z wykorzystaniem klasy CBitmapFont. */
	GLvoid DrawConsoleWithBitmapFont();
#endif //_USE_CONSOLE_WITH_BITMAP_FONT_
#if _USE_CONSOLE_WITH_MY_FONT_
	/// Metoda rysuj�ca konsol�.
	/** Metoda rysuje konsol� z wykorzystaniem klasy CMyFont. */
	GLvoid DrawConsoleMyFont();
#endif //_USE_CONSOLE_WITH_MY_FONT_

	/// Zdarzenie naci�ni�cia klawisza klawiatury, przekazywane z CGameControl.
	/** \param[in] uiKeyCode Wirtualny kod klawisza jaki zosta� naci�ni�ty.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta� obs�u�ony. */
	GLboolean KeyDown(GLuint uiKeyCode);

	/// Zdarzenie zwolnienia klawisza klawiatury, przekazywane z CGameControl.
	/** \param[in] uiKeyCode Wirtualny kod klawisza jaki zosta� zwolniony.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta� obs�u�ony. */
	GLboolean KeyUp(GLuint uiKeyCode);

	/// Zdarzenie przekr�cenia k�ka myszy, przekazywane z CGameControl.
	/** \param[in] iScrollLines Mowi nam o ile linii i w kt�ra stron� wykonano obr�t, warto�� ujemna - w d�l, warto��
	dodatnia - w g�r�. 
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. W przeciwnym wypadku \p GL_FALSE.*/
	GLboolean MouseWheel(GLint iScrollLines);

	/// Metoda ustawiaj�ca blokad� na wy�wietlanie konsoli.
	/** \param[in] bBlockConsole Podaj�c \p GL_TRUE zablokujemy wy�wietlanie konsoli. Podaj�c \p GL_FALSE
	odblokujemy.*/
	inline GLvoid SetBlockConsole(GLboolean bBlockConsole) { m_bBlockConsole = bBlockConsole; }
	
private:

	/// Struktura przechowuj�ca informacj� o pojedynczej linijce tekstu.
	struct SConsoleLine
	{
		TCHAR *lpMsg;				///< Wy�wietlany wiadomo��.
		EConsoleMsgType eType;		///< Typ wiadomo�ci.
	};

	CSpeedControl	*m_pSpeedControl;		///< Wska�nik na obiekt klasy CSpeedControl.
	CMyFont			*m_pMyFont;			///< Wska�nik na obiekt klasy CMyFont.
	CBitmapFont		*m_pBitmapFont;		///< Wska�nik na obiekt klasy CBitmapFont.
	CSDLFont		*m_pSDLFont;			///< Wska�nik na obiekt klasy CSDLFont.

	/// Metoda inicjalizuj�ca wszelkie niezb�dne zmienne odpowiednimi warto�ciami.
	GLvoid SetVariables();

	/// Metoda tworz�ca obiekt klasy CSpeedControl.
	GLvoid CreateSpeedControl();

	GLint m_iAnimCtrl1;		///< Indeks kontroli animacji wysuwania si� konsoli.
	GLint m_iAnimCtrl2;		///< Indeks kontroli animacji mrugania znaku zach�ty w konsoli.

	/////////////////////// Start LastCommand /////////////////////////////////////////

	std::vector<TCHAR*> m_aLastCommand;		///< Tablica przechowuj�ca ostatnio wpisywane komendy.

	/// Metoda zwracaj�ca ilo�� zapami�tanych ostatnio wpisywanych komend.
	/** \return Liczba ca�kowita okre�laj�ca ilo�� zapami�tanych komend. */
	inline GLint GetLastCommandSize() { return static_cast< GLint >( m_aLastCommand.size() ); }
	
	GLint m_iCurrentLastCommand;	///< Zmienna zapami�tuj�ca indeks ostatnio zapami�tanej komendy.

	/// Metoda zapamietuj�ca nast�pn� komend�.
	/** \param[in] lpCommand Tekst ostatnio wpisanej komendy. */
	GLvoid AddLastCommand(LPCTSTR lpCommand);

	/// Metoda usuwaj�ca wszelkie pami�tane komendy.
	GLvoid DeleteLastCommands();

	/////////////////////// End LastCommand /////////////////////////////////////////

	/// Tablica sk�aduj�ca linijki tekstu wy�wietlane w konsoli.
	std::vector<SConsoleLine> m_aConsoleLine;

	/// Metoda zwracaj�ca ilo�� sk��dowanych linijek tekstu wy�wietlanych w konsoli.
	/** \return Liczba ca�kowita reprezentuj�ca ilo�� linijek tekstu wy�wietlanych w konsoli. */
	inline GLint GetConsoleLineSize() { return static_cast< GLint >( m_aConsoleLine.size() ); }

	/// Zmienna zapamietuj�ca jakie typy wiadomo�ci maj� by� wy�wietlany w konsoli.
	EConsoleMsgType m_eFilter;

	/// Metoda usuwaj�ca wszelkie linijki tekstu wy�wietlane w konsoli.
	GLvoid DeleteConsoleLines();

	GLint m_iStartLine;			///< Indeks linijki tekstu jaki ma by� wy�wietlany w pierwszej linijce konsoli.
	GLint m_iEndLine;			///< Indeks linijki tekstu jaki ma by� wy�wietlany w ostatniej linijce konsoli.
	GLint m_iPageCountLine;		///< Maksymalna ilo�� linijek tekstu wy�wietlana aktualnie w konsoli.

	GLfloat m_fPosX;			///< Wsp�rz�dna x pozycji tekstu w linijce.
	GLfloat m_fPosY;			///< Wsp�rz�dna y pozycji tekstu w linijce.
	GLfloat m_fPosYConsole;		///< Pozycja ca�ego okna konsoli dla efektu wysuwania si�.

	RECT m_rcRegionBoard;		///< Prostok�t okre�laj�cy po�orzenie i rozmiar konsoli.
	GLint m_iPosYConsole;		///< Aktualna pozycja konsoli w osi Y dla efektu wysuwania si�.
	GLint m_iRateYConsole;		///< Skok wysuwania si� konsoli.
	GLboolean m_bDrawCursor;	///< Flaga wskazuj�ca czy w danej chili ma by� wy�wietlany znak zach�ty.

	GLboolean m_bShowConsole;	///< Flaga wskazuj�ca czy konsola jest rysowana.

	TCHAR m_cLastLineConst[8];	///< Znak zach�ty konsoli.
	TCHAR m_cLastLine[128];		///< Aktualnie pisana linia w linii komend.

	GLboolean m_bBlockConsole;	///< Flaga wskazuj�ca czy wy�witlanie konsoli ma by� blokowane.

	/// Metoda wykonuj�ca podan� komend�.
	/** \param[in] lpCommand Komenda do przeanalizowania i wykonania. */
	GLvoid ExecuteCommand(LPCTSTR lpCommand);

	/// Metoda ustawiaj�ca wy�wietlanie w konsoli tak aby widoczna by�a ostatnia linijka.
	GLvoid SetStartEndLineOnEnd();

	/// Metoda wyci�gaj�ca parametr z komendy.
	/** \param[in] lpCommand Komenda do przeanalizowania.
	\param[in] lpParameter Zwracany rezultat w postaci parametru komendy.
	\return \p GL_TRUE w przypadku powodzenia wyci�gni�cia parametru. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean GetParameterCommand(LPCTSTR lpCommand, LPTSTR lpParameter);

	/// Metoda przygotowuj�ca tekst do wy�wietlenia w konsoli.
	/** \param[in] eType Typ wiadomo�ci.
	\param[in] lpText Tekst wiadomo�ci. */
	GLvoid PrepareText(EConsoleMsgType eType, const char *lpText);

	/// Motoda dodaj�c� linijk� tekstu do wy�wietlenia w konsoli.
	/** \param[in] eType Typ wiadomo�ci.
	\param[in] lpText Tekst wiadomo�ci.
	\param uiLength D�ugo�� tekstu. */
	GLvoid AddTextFinally(EConsoleMsgType eType, LPCTSTR lpText, size_t uiLength);

	/// Metoda skroluj�ca tekst o jedn� linijk� w g�r�.
	GLvoid LineScrollUp();

	/// Metoda skroluj�ca tekst o jedn� linijk� w d�.
	GLvoid LineScrollDown();

	GLvoid DrawRect( GLboolean bTexCoord = GL_TRUE );

	CVector2* m_aTextCoord;
	CVector2* m_aIndices;
};