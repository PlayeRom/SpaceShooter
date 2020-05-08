/** \file
	\brief Plik nag��wkowy dla klasy CLogger.
*/
#pragma once

/// Domy�lny, pierwszy cz�on nazwy pliku logger'a - bez rozszerzenia.
#define LOG_FILE_NAME _T("Log")

/// \name Makra u�atwiaj�ce u�ywanie metod zapisu do pliku.
//@{

/// Makro u�atwiaj�ce dost�p do metody CLogger::LOG.
/*! Makro to mo�emy wywo�a� w dowolnym miejscu kodu.
\param[in] msg Dowolny tekst jaki chcemy zapisa� do pliku. */
#define __LOG( msg ) CWindowData::GetInstance()->GetLogger()->LOG( msg )

/// Makro u�atwiaj�ce dost�p do metody CLogger::LOG, z parametrami dla formatowania.
/*! Makro to mo�emy wywo�a� w dowolnym miejscu kodu.
\param[in] fmt Dowolny tekst jaki chcemy zapisa� do pliku.
\param[in] ... Dodatkowe parametry dla formatowania. */
#define __LOG_FORMATTED( fmt, ... ) CWindowData::GetInstance()->GetLogger()->LOG( fmt, __VA_ARGS__ )

/// Makro u�atwiaj�ce dost�p do metody CLogger::LOGGetLastError.
/*! Makro to mo�emy wywo�a� w dowolnym miejscu kodu.
\param[in] msg Dowolny tekst dzi�ki, kt�remu b�dziemy wiedzieli co wywo�a�o ewentualny b��d. */
#define __LOG_LAST_ERROR( msg ) CWindowData::GetInstance()->GetLogger()->LOGLastError( msg )
//@}

/// Klasa loggera.
/*! Jest to klasa odpowiedzialna za zapisywanie do pliku przekazywanych jej parametr�w. Dzi�ki temu w fazie
testowania naszej gry b�dziemy mogli rejestrowa� to co si� dzieje w programie. Obiekt klasy CLogger zawsze
jest tworzony w CWindowData, jednak domy�lnie logowanie jest wy��czone. Aby je uaktywni� nale�y uruchomi�
program z parametrem \a /logger. */
class CLogger
{
public:
	/// Konstruktor.
	/*! \param[in] lpFileName Nazwa pliku (a raczej pierwszy cz�on nazwy pliku), do kt�rego b�d� zapisywane informacje.
	Pe�na nazwa pliku zawiera nast�puj�cy format:
	\n lpFileName � YYYY.MM.DD � HH.MM.SS.log
	\n gdzie:
	\n lpFileName � to opisywany tu parametr,
	\n YYYY.MM.DD � data (rok, miesi�c i dzie�) utworzenia pliku,
	\n HH.MM.SS � czas utworzenia pliku, tj. godzina, minuty i sekundy,
	\n .log � rozszerzenia pliku. */
	CLogger( LPCTSTR lpFileName );

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CLogger();

	/// Metoda otwieraj�ca plik loga.
	/*! Metoda ta jest ju� wywo�ywana w CWindowData::Initialization, ale pod warunkiem, �e program zosta�
	uruchomiony z parametrem \a /logger. \n
	Metoda ta w trakcie otwierania nowego pliku mo�e jednocze�nie usun�� stare pliki logger-a. To czy stare
	pliki b�d� usuwane zale�y od ustawienia flagi CLogger:m_bClearOldLog. \n
	Wywo�anie tej metody jednocze�nie ukatywnia logger-a, patrz CLogger::SetActive.
	\return \p GL_TRUE w przypadku powodzenia otwarcia pliku loggera. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean Open();

	/// Metoda uaktywniaj�ca b�d� dezaktywuj�ca logger.
	/*! \param[in] bMode Podaj�c \p GL_TRUE uaktywnimy logger. Je�eli logger zostanie uaktywniony bez wcze�niejszego
	wywo�ania metody CLogger::Open to podczas wywo�ania metody zapisuj�cej do pliku (np. CLogger::LOG),
	automatycznie zostanie wywo�ana tak�e motoda CLogger::Open. Czyli log zostanie utworzony. \n
	Podaj�c \p GL_FALSE logger stanie si� nieaktywny, co oznacza, �e pomimo wywo�ania metody zapisuj�cej do pliku
	(np. CLogger::LOG), do pliku nic nie zostanie zapisane.
	\sa IsActive, m_bActive. */
	GLvoid SetActive( GLboolean bMode );

	/// Metoda sprawdzaj�ca czy logger jest aktywny.
	/*! \return \p GL_TRUE je�eli logger jest aktywny. W przeciwnym wypadku \p GL_FALSE.
	\sa SetActive, m_bActive. */
	inline GLboolean IsActive() { return m_bActive; }

	/// Metoda usuwaj�ca stare pliki log.
	GLvoid ClearOldLog();

	/// Metoda sprawdzaj�ca stan flagi CLogger::m_bClearOldLog.
	inline GLboolean IsClearOldLog() { return m_bClearOldLog; }

	/// Metoda ustawiaj�ca ilo�� tabulator�w przed ka�ym z wpis�w.
	/*! Za pomoc� tej metody mo�emy tworzy� wci�cia przed ka�d� linijk� zapisywan� do pliku. Metod� t� nale�y wywo�a�
	przed wywo�aniem jakiejkolwiek metody zapisuj�cej do pliku (np. CLogger::LOG), podaj�c odpowiedni� liczb�
	tabulator�w. Teraz je�eli wywo�amy jed� z funkcji zapisuj�cej do pliku to nasza linia tekstu zostanie porzedzona
	odpwiedni� ilo�ci� wci��. W ka�dej metodzie zapisuj�cej do pliku, liczba tabulator�w jest zerowana.
	\param[in] iTabNumber Liczba okre�laj�ca ilo�� tabulator�w - wielko�� wci�cia jak� chcemy utworzy� dla danej
	linii tekstu, dla lepszej czytelno�ci log-a.
	\sa m_iTabNumber. */
	inline GLvoid SetTabulatorNumber( GLint iTabNumber ) { m_iTabNumber = iTabNumber; }

	/// \name Metody zapisuj�ce do pliku.
	//@{

	/// Metoda zapisuj�ca do pliku, lini� tekstu (\p UNICODE), z mo�liwo�ci� formatowania.
	/*! Za pomoc� tej metody mo�emy zapisa� do pliku dowolny ci�g znak�w. Otwarta lista parametr�w umo�liwia
	formatowanie tekstu, dzi�ki czemu mo�emy zapisywa� warto�ci zmiennych, itp. \n
	Tekst zostanie zapisany do pliku tylko wtedy je�eli logger jest aktywny, tj. je�eli flaga CLogger::m_bActive
	ustawiona jest na \p GL_TRUE. \n
	Tekst zawsze b�dzie zapisywany w pliku od nowej linii.
	\param[in] fmt Dowolny tekst jaki chcemy zapisa� do pliku. Tekst mo�e zawiera� znaki formatowania (%d, %s, %c itp.).
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid LOG( const wchar_t *fmt, ... );

	/// Metoda zapisuj�ca do pliku, lini� tekstu, z mo�liwo�ci� formatowania.
	/*! Za pomoc� tej metody mo�emy zapisa� do pliku dowolny ci�g znak�w. Otwarta lista parametr�w umo�liwia
	formatowanie tekstu, dzi�ki czemu mo�emy zapisywa� warto�ci zmiennych, itp. \n
	Tekst zostanie zapisany do pliku tylko wtedy je�eli logger jest aktywny, tj. je�eli flaga CLogger::m_bActive
	ustawiona jest na \p GL_TRUE. \n
	Tekst zawsze b�dzie zapisywany w pliku od nowej linii.
	\param[in] fmt Dowolny tekst jaki chcemy zapisa� do pliku. Tekst mo�e zawiera� znaki formatowania (%d, %s, %c itp.).
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid LOG( const char *fmt, ... );

	/// Metoda zapisuj�ca do pliku pojedynczy znak \p UNICODE.
	/*! Znak zostanie zapisany do pliku tylko wtedy je�eli logger jest aktywny, tj. je�eli flaga CLogger::m_bActive
	ustawiona jest na \p GL_TRUE. \n
	Znak zawsze b�dzie dopisywany na koniec pliku.
	\param[in] cChar Znak jaki chemy zapisa�. */
	GLvoid LOGFreeChar( wchar_t cChar );

	/// Metoda zapisuj�ca do pliku pojedynczy znak.
	/*! Znak zostanie zapisany do pliku tylko wtedy je�eli logger jest aktywny, tj. je�eli flaga CLogger::m_bActive
	ustawiona jest na \p GL_TRUE. \n
	Znak zawsze b�dzie dopisywany na koniec pliku.
	\param[in] cChar Znak jaki chemy zapisa�. */
	GLvoid LOGFreeChar( char cChar );

	/// Metoda zapisuj�ca do pliku, lini� teksu, z ostatnio wywo�anym b��dem WinAPI.
	/*! Metoda ta zapisuje do pliku opis b��du, zwr�conego przez funkcj� WinAPI: \a GetLastError.
	\param[in] lpWhatCallError Dowolny tekst dzi�ki, kt�remu b�dziemy wiedzieli co wywo�a�o b��d. */
	GLvoid LOGLastError( LPCTSTR lpWhatCallError = _T("") );
	//@}

private:
	/// Metoda usuwaj�ca stare pliki logger-a.
	/*! Metoda usuwa wszystkie pliku z rozszerzeniem \a .log znajduj�ce si� w bierz�cym katalogu. Metoda ta jest
	wywo�ywana w CLogger::Open pod warunkiem, �e flaga CLogger::m_bClearOldLog ustawiona jest na \p GL_TRUE.
	\param[in] lpCurDir Nazwa katalogu, z kt�rego zostan� usuni�te pliki logger-a.
	\sa m_bClearOldLog, ClearOldLog, IsClearOldLog. */
	GLvoid DeleteLogFiles( LPCTSTR lpCurDir );

	/// Metoda zamykaj�ca plik logger-a.
	/*! Metoda ta jest wywo�ywana w destruktorze klasy. */
	GLvoid Close();

	FILE *m_hFile;			///< Uchwyt do pliku log-a.
	GLboolean m_bActive;	///< Flaga okre�laj�ca czy logger jest aktywny.
	GLuint m_uiCounterLine;	///< Licznik aktualnie zapisanych linii tekstu do pliku.

	/// Flaga czyszczenia starych plik�w logger-a.
	/*! Zmienna wskazuj�ca czy w trakcie wywo�ywania metody CLogger:Open, maj� by� jednocze�nie usuwane stare pliki logger-a.
	\sa ClearOldLog, IsClearOldLog. */
	GLboolean m_bClearOldLog;
	GLint m_iTabNumber;		///< Zmienna przechowuj�ca aktualnie ustawion� ilo�� tabulator�w.

	/// Przechowuje pe�n� nazw� pliku loga.
	TCHAR m_cFimeNameWithData[ 256 ];
};
