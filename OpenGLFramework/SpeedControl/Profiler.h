/*! \file
	\brief Plik nag��wkowy dla klasy CProfiler.
*/
#pragma once

#define MAX_SIZE_OF_STRING	512

class CLogger;

/// Klasa profilera.
/** Za pomoc� profilera mo�emy zmierzy� czas wykonywania si� poszczeg�lnych funkcji. Dzi�ki temu mo�emy
zdiagnozowa�, kt�re metody spowalniaj� nam program i kt�re nale�a�oby zoptymalizowa�. Zadanie pomiaru czasu
wykonywania si� kodu spoczywa w�a�nie na klasie CProfiler. Klasa CProfiler jest singletonem, jej instancja jest
ju� we frameworku tworzona i usuwana automatycznie, wi�c naszym zadaniem jest tylko wywo�ywanie pary metod
CProfiler::Begin oraz CProfiler::End. Dla �atwego wy��czenia u�ycia profilera zaleca si� wywo�ywa� powy�sze metody
poprzez makra \ref PROFILER_BEGIN oraz \ref PROFILER_END po uprzednim zdefiniowaniu \ref _USE_PROFILER_ na 1. */
class CProfiler
{
public:
	/// Statyczna metoda za pomoc�, kt�rej pobieramy wska�nik na obiekt klasy CProfiler. 
	/** Przy pierwszym wywo�aniu tej metody, jednocze�nie zostanie stworzony obiekt klasy.
	\return Wska�nik na obiekt CProfiler. */
	static CProfiler* GetInstance();

	/// Destruktor.
	/** Destruktor. */
	~CProfiler();

	/// Metoda rozpoczynaj�ca pomiar czasu.
	/** Wywo�uj�c t� metod� zawsze nale�y wywo�a� p�niej metod� CProfiler::End.
	\param[in] lpComment Komentarz jaki zostanie zapisany w logu profilera. Najlepiej jest poda� tu nazw�
	funkcji, kt�r� testujemy.
	\sa End. */
	GLvoid Begin( LPCTSTR lpComment );

	/// Metoda ko�cz�ca pomiar czasu.
	/** Wywo�anie tej metody zawsze musi by� poprzedzone wywo�aniem metody CProfiler::Begin.
	\sa Begin. */
	GLvoid End();

	/// Motoda zapisuj�ca do pliku zebrane informacje.
	/** Metoda ta musi by� wywo�ana zawsze co klatk� w g��wnej p�tli gry. Zadniem tej metody jest zapisanie wszelkich
	zebranych informacji do pliku. */
	GLvoid Flush();

private:
	/// Struktura zapami�tuj�ca wszelkie wywo�ania metody CProfiler::Begin.
	/** Struktura przechowuje takie informacje jak aktualny czas wywo�ania metody CProfiler::Begin oraz
	komentarz jaki zosta� podany jako parametr metody CProfiler::Begin. */
	struct SBegin
	{
		GLboolean bIsUse;						///< Czy jest w u�yciu?
		unsigned long ulBeginMs;				///< Czas w milisekundach w chwili wywo�ania metody CProfiler::Begin.
		TCHAR lpComment[ MAX_SIZE_OF_STRING ];	///< Komentarz jaki zosta� podany przy wywo�aniu metody CProfiler::Begin.
	};

	/// Struktura przechowuj�ca wszelkie informacje jakie zostan� zapisaane do pliku.
	/** Struktura przechowuje takie informacje jak komentarz wywo�ania metody CProfiler::Begin, czas jaki
	up�yn�� mi�dzy wywo�aniami CProfiler::Begin a CProfiler::End oraz ilo�� wci�� dla lepszej czytelno�ci loga. */
	struct SSaveToLog
	{
		GLboolean bIsUse;						///< Czy jest w u�yciu?
		GLint iIndentNumber;					///< Ilo�� wci��.
		TCHAR lpComment[ MAX_SIZE_OF_STRING ];	///< Komentarz jaki zosta� podany przy wywo�aniu metody CProfiler::Begin.
		unsigned long ulTimeMs;					///< Czas w milisekundach jaki up�yn�� mi�dzy wywo�aniem CProfiler::Begin a CProfiler::End.
	};

	static std::auto_ptr<CProfiler> m_pProfiler; ///< Instancja klasy.

	/// Konstruktor.
	/** Konstruktor. */
	CProfiler();

	/// Wska�nik na obiekt klasy CLogger.
	/** Obiekt za pomoc� kt�rego wykonywany jest zapis do pliku loga profilera. */
	CLogger *m_pLogger;

	/// Metoda zwracaj�ca aktualny czas w milisekundach.
	/*! \return Aktualny czas (w milisekundach) jaki up�yn�� od uruchomienia systemu.	*/
	unsigned long GetTimeInMs();

	/// Metoda zwracaj�ca ilo�� element�w tablicy CProfiler::m_aBeginMs.
	/** \return Ilo�� element�w tablicy CProfiler::m_aBeginMs. */
	GLint GetBeginMsSize() { return static_cast<GLint>( m_aBeginMs.size() ); }

	/// Tablica obiekt�w struktury SBegin.
	/** Tablica przechowuj�ca czas wywo�ania ka�dej metody CProfiler::Begin. */
	std::vector<SBegin> m_aBeginMs;

	/// Zmienna przechowuj�ca aktualn� ilo�� wci�� - dla formatowania loga.
	GLint m_iIndentCounter;

	/// Tablica obiekt�w struktury SSaveToLog.
	/** Tablica przechowuj�ca rekordy jakie zostan� zapisane do pliku. */
	std::vector<SSaveToLog> m_aSaveToLog;
};
