/** \file
	\brief Plik nag��wkowy dla klasy CMultiLanguage.
*/
#pragma once

#include "../OpenGLFramework/WindowData.h"
#include "../OpenGLFramework/SingletonBase.h"

/// Makro pobrania wskazanej linii tekstu z pliku j�zykowego.
/** Makro to wywo�uje metod� CMultiLanguage::GetLangText, zwracaj�c� �a�cuch znak�w z pliku j�zykowego.
\param[in] x Numer linii z pliku tekstowego, kt�ry chcemy pobra�, np. w celu wy�wietlenia. Numery linii
liczone s� od 0.
\return �a�cuch znak�w zapisany w podanej linii pliku \a *.\a lng. */
#define LNG_LINE( x )	CMultiLanguage::GetInstance()->GetLangText( x )

/// Klasa wspieraj�ca wieloj�zyczno��.
/*! Zadanie tej klasy polega na wczytaniu pliku \a lng z linijkami tekst�w przet�umaczonych na dany j�zyk. Nast�pnie
zwracania odpowiednich linii tekstu z wykorzystywanego j�zyka. Odpowiedni plik \a *.\a lng mo�e by� wczytany na
podstawie pliku \a languages_def.xml lub bezpo�rednio gdy nie wykorzystujemy pliku XML. */
class CMultiLanguage : public CSingletonBase <CMultiLanguage>
{
public:
	/// Statyczna metoda tworz�ca obiekt klasy CMultiLanguage. 
	/*! CMultiLanguage jest singletonem, wi�c je�eli wywo�amy t� metod� po raz drugi to zwr�ci ona wska�nik na obiekt
	ju� istniej�cy, utworzony wcze�niej.
	\return Wska�nik na obiekt CMultiLanguage. */
	static CMultiLanguage* Construct();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CMultiLanguage();

	/// \name Interfejs uniwersalny, niezale�nie czy u�ywamy pliku XML czy nie.
	//@{

	/// Metoda inicjalizuj�ca wsparcie wieloj�zyczno�ci.
	/*! Metoda ta jest ju� wywo�ywana w CWindowData::Initialization. */
	GLvoid InitializeLanguage();

	/// Metoda wczytuj�ca podany plik j�zykowy.
	/*! Metoda ta jest ju� wywo�ywana w CGameControl::Initialization.
	\param[in] iIndex Indeks pliku j�zykowego jaki chcemy wczyta�. Najlepiej poda� tutaj pole
	CWindowData::SSettingFile::iIndexLanguage.
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean LoadLanguage(GLint iIndex);

	/// Metoda zwracaj�ca wskazan� lini� tekstu z wcze�niej wczytanego pliku j�zykowego.
	/*! Metoda ta s�u�y do pobrania konkretnej linii tekstu zapisanej w pliku j�zykowym (\a *.\a lng).
	\param[in] iLineNumber Numer linii z pliku tekstowego, kt�ry chcemy pobra�, np. w celu wy�wietlenia. Numery linii
	liczone s� od 0.
	\return �a�cuch znak�w zapisany w podanej linii pliku \a *.\a lng. */
	LPTSTR GetLangText(GLint iLineNumber);

	/// Metoda zwracaj�ca ilo�� plik�w j�zykowych.
	/*! \return Ilo�� plik�w j�zykowych zawartych w katalogu \a languages lub opisanych w pliku
	\a languages/languages_def.xml. */
	GLint GetLangFileNumber();

	/// Metoda zwracaj�ca nazw� pliku j�zykowego.
	/*! Za pomoc� tej metody mo�emy sprawdzi� jaki plik \a *.\a lng jest przypisany dla danego indeksu. 
	\param[in] iIndex Indeks pliku j�zykowego.
	\return Nazwa pliku j�zykowego. */
	LPTSTR GetLangFileName(GLint iIndex);
	//@}

	/// \name Interfejs tylko je�eli wykorzystywany jest plik XML.
	//@{

	/// Metoda zwracaj�ca nazw� j�zyka dla danego j�zyka.
	/*! Metoda zwraca warto�� parametru \p name, znacznika \p \<language\> dla indeksu j�zyka zawartego w polu
	CWindowData::SSettingFile::iIndexLanguage.
	\return Nazwa j�zyka zdefiniowana w pliku \a languages_def.xml. W przypadku jakiego� b��du, np. nie u�ywania
	pliku XML, zwr�cony zostanie pusty �a�cuch znak�w: \a "". */
	inline LPTSTR GetXMLLngName() {
		GLint iIndex = CWindowData::GetInstance()->SettingFile().iIndexLanguage;
		if( !m_bUseXML || ( iIndex < 0 || iIndex >= GetXMLLngDataSize() ) )
			return _T("");

		return m_aXMLLngData[ iIndex ].lpName;
	}

	/// Metoda zwracaj�ca nazw� pliku czcionki (\a *.\a ttf).
	/*! Metoda zwraca warto�� parametru \p file, znacznika \p \<font\> dla indeksu j�zyka zawartego w polu
	CWindowData::SSettingFile::iIndexLanguage.
	\param[in] iIndexTTF Indeks pliku \a *.\a ttf zawartego w pliku \a languages_def.xml w znaczniku \p \<font\>.
	Indeks jest warto�ci� umown� liczon� od zera, pobieran� w kolejno�ci podawania znacznik�w \p \<font\>.
	\return Nazwa pliku czcionki zdefiniowana w pliku \a languages_def.xml. W przypadku jakiego� b��du, np. nie
	u�ywania pliku XML, zwr�cony zostanie pusty �a�cuch znak�w: \a "". */
	inline char* GetXMLLngFileTTF(GLint iIndexTTF){
		GLint iIndex = CWindowData::GetInstance()->SettingFile().iIndexLanguage;
		if( !m_bUseXML || ( iIndex < 0 || iIndex >= GetXMLLngDataSize() ) )
			return "";
		if( iIndexTTF < 0 || iIndexTTF >= GetXMLLngDataTTFSize( iIndex ) )
			return "";

		return m_aXMLLngData[iIndex].aFileTTF[iIndexTTF];
	}
	//@}
private:
	/// Konstruktor.
	/*! Konstruktor. */
	CMultiLanguage();

	/// Struktura danych dla pliku XML.
	/*! Struktura ta przechowuje warto�ci parametr�w znacznik�w opisanych w pliku \a languages_def.xml. */
	struct SXMLLngData
	{
		/// Nazwa j�zyka.
		/*! Przechowuje warto�� parametru \p name znacznika \p \<language\>. */
		TCHAR *lpName;

		/// Nazwa pliku (\a *.\a lng) zawieraj�cego tekst.
		/*! Przechowuje warto�� parametru \p file znacznika \p \<language\>. */
		TCHAR *lpFileLNG;

		/// Tablica nazw czcionek.
		/*! Przechowuje warto�ci parametu \p file znacznik�w \p \<font\>. */
		std::vector<char*> aFileTTF;
	};

	/// Tablica przechowuj�ca linie tekstu pliku j�zykowego (\a *.\a lng).
	/*! Tablica ta przechowuje linie tekstu aktualnie wczytanego pliku j�zykowego (\a *.\a lng). */
	std::vector<TCHAR*> m_aLineText;

	/// Metoda czyszcz�ca tablic� CMultiLanguage::m_aLineText.
	GLvoid ClearLineText();

	/// Metoda zwracaj�ca ilo�� linii tekstu pliku j�zykowego (\a *.\a lng).
	/*! \return Ilo�� linii tekstu zapisanych w pliku j�zykowym \a *.\a lng. Jest to rozmiar tablicy
	CMultiLanguage::m_aLineText. */
	inline GLint GetLineTextSize() { return static_cast<GLint>( m_aLineText.size() ); }

	/// Metoda wczytuj�ca plik j�zykowy \a *.\a lng.
	/*! Metoda ta jest wywo�ywana bezpo�rednio z CMultiLanguage::LoadLanguage.
	\param[in] lpFileName Nazwa pliku j�zykowego \a *.\a lng. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean LoadLanguageFile(LPCTSTR lpFileName);

	/// Metoda wczytuj�ca plik j�zykowy \a *.\a lng z kodowaniem \p UNICODE.
	/*! Metoda ta jest wywo�ywana z CMultiLanguage::LoadLanguageFile. W metodzie tej tworzona jest tablica
	CMultiLanguage::m_aLineText.
	\param[in] f Uchwyt do otwartego pliku \a *.\a lng. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean LoadUnicodeFile(FILE *f);

	/// Metoda wczytuj�ca plik j�zykowy \a *.\a lng z kodowaniem \p ANSI.
	/*! Metoda ta jest wywo�ywana z CMultiLanguage::LoadLanguageFile. W metodzie tej tworzona jest tablica
	CMultiLanguage::m_aLineText.
	\param[in] f Uchwyt do otwartego pliku \a *.\a lng. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean LoadAnsiFile(FILE *f);

	TCHAR m_cErrText[128]; ///< Pomocniczy tekst do zwracania informacji o b��dzie.

	/// \name Metody u�ywane tylko wtedy gdy nie wykorzystywany jest XML.
	//@{

	/// Metoda wyszukuj�ca pliki \a *.\a lng.
	/*! Metoda ta przeszukuje katalog \a languages pod wzgl�dem wyst�powania w nim plik�w z rozszerzeniem \a lng.
	Ka�dy nazwa znalezionego pliku \a lng zapisywana jest w tablicy CMultiLanguage::m_aLanguageFileName. \n
	Metoda ta jest wywo�ywana w CMultiLanguage::InitializeLanguage pod warunkiem, �e nie istnieje plik 
	\a languages\\languages_def.xml. */
	GLvoid SearchLngFile();

	/// Metoda sprawdzaj�ca rozszerzenie pliku.
	/*! Metoda ta sprawdza czy podana nazwa pliku posiada rozszerzenie \a lng.
	\param[in] lpPath Pe�na nazwa pliku wraz ze �cie�k�.
	\return \p GL_TRUE je�eli nazwa ko�czy si� rozszerzeniem \a lng. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean CheckExtensionLNG(LPCTSTR lpPath);

	/// Metoda zwracaj�ca ilo�� znalezionych plik�w \a lng.
	/*! \return Ilo�� element�w tablicy CMultiLanguage::m_aLanguageFileName. */
	inline GLint GetLngFileNameSize() { return static_cast<GLint>( m_aLanguageFileName.size() ); }

	/// Metoda zwracaj�ca nazw� pliku j�zykowego.
	/*! Metoda ta jest wywo�ywana z CMultiLanguage::GetLangFileName. 
	\param[in] iIndex Indeks pliku j�zykowego.
	\return Nazwa pliku j�zykowego. W przypadku b��du zwraca \a "languages\\english.lng". */
	inline LPTSTR GetLngFileName(GLint iIndex){
		if( iIndex < 0 || iIndex >= GetLngFileNameSize() )
			return _T("languages\\english.lng");
		return m_aLanguageFileName[ iIndex ];
	}
	//@}

	/// \name Pola u�ywane tylko wtedy gdy nie wykorzystywany jest XML.
	//@{

	/// Tablica nazw plik�w \a *.\a lng.
	/*! Tablica ta przechowuje nazwy znalezionych plik�w z rozszerzeniem \a lng w katalogu \a languages. */
	std::vector<TCHAR*> m_aLanguageFileName;
	//@}

	/// \name Metody u�ywane tylko wtedy gdy wykorzystywany jest XML.
	//@{

	/// Metoda parsuj�ca plik XML.
	/*! Metoda ta parsuje specjalnie przygotowany plik XML ze zdefiniowanymi j�zykami.
	\param[in] lpFileName Nazwa pliku XML jaki zostanie sparsowany. Domy�lnie jest to \a "languages\\languages_def.xml".
	\return \p GL_TRUE w przypadku powodzenia otwarcia pliku XML. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean ParseXMLLngData(LPCTSTR lpFileName = _T("languages\\languages_def.xml"));

	/// Metoda szukaj�ca znacznika w zadanym tek�cie.
	/*! Metoda ta wywo�ywana jest z CMultiLanguage::ParseXMLLngData.
	\param[in] lpSource Tekst, w kt�rym poszukiwany b�dzie znacznik.
	\param[in] lpTag Nazwa poszukiwanego znacznika.
	\return \p GL_TRUE w przypadku znalezienia znacznika. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean SearchTag(LPCTSTR lpSource, LPCTSTR lpTag);

	/// Metoda szukaj�ca parametru w zadanym tek�cie.
	/*! Metoda ta wywo�ywana jest z CMultiLanguage::ParseXMLLngData. Zadaniem tej metody jest odnale�� w tek�cie
	dany parametr i wy�uska� z niego warto��.
	\param[in] lpSource Tekst, w kt�rym poszukiwany b�dzie parametr.
	\param[in] lpParameter Nazwa poszukiwanego parametru.
	\param[out] lpResult Wska�nik na bufor, do kt�rego zapisana zostanie warto�� parametru. Je�eli parametr
	nie zostanie odnaleziony to lpResult b�dzie wype�niony zerami.
	\param[in] iSizeResult Rozmiar bufora lpResult.
	\return \p GL_TRUE w przypadku odnalezienia szukanego parametru. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean SearchParameter(LPCTSTR lpSource, LPCTSTR lpParameter, LPTSTR lpResult, GLsizei iSizeResult);

	/// Metoda tworz�ca nowy j�zyk na podstawie parsowania pliku XML.
	/*! Metoda ta tworzy nowy element tablicy CMultiLanguage::m_aXMLLngData, do kt�rego zapisuje nazw� j�zyka
	oraz nazw� pliku \a lng.
	\param[in] lpName Nazwa j�zyka jako warto�� parametru \p name znacznika \p \<language\>.
	\param[in] lpLNG Nazwa pliku \a lng jako warto�� parametru \p file znacznika \p \<language\>. */
	GLvoid AddXMLLngData(LPCTSTR lpName, LPCTSTR lpLNG);

	/// Metoda dodaj�ca nazy plik�w z czcionk�.
	/*! Metoda ta dodaje nowy element do tablicy CMultiLanguage::SXMLLngData::aFileTTF.
	\param[in] lpTTF Nazwa pliku z czcionk� jako warto�� parametru \p file znacznika \p \<font\>. */
	GLvoid AddXMLLngDataTTF(LPCTSTR lpTTF);

	/// Metoda usuwaj�ca wszelkie dane zapisane w tablicy CMultiLanguage::m_aXMLLngData.
	GLvoid ClearXMLLngData();

	/// Metoda zwracaj�ca ilo�� zdefiniowanych j�zyk�w w pliku \a languages_def.xml.
	/*! \return Ilo�� element�w tablicy CMultiLanguage::m_aXMLLngData. */
	inline GLint GetXMLLngDataSize() { return static_cast<GLint>(m_aXMLLngData.size()); }

	/// Metoda zwracaj�ca nazw� pliku j�zykowego dla danego j�zyka.
	/*! Metoda ta jest wywo�ywana z CMultiLanguage::GetLangFileName. 
	\param[in] iIndex Indeks pliku j�zykowego.
	\return Nazwa pliku j�zykowego. W przypadku b��du zwraca \a "languages\\english.lng". */
	inline LPTSTR GetXMLLngFileLNG(GLint iIndex){
		if( iIndex < 0 || iIndex >= GetXMLLngDataSize() )
			return _T("languages\\english.lng");
		return m_aXMLLngData[ iIndex ].lpFileLNG;
	}

	/// Metoda zwracaj�ca ilo�� zdefiniowanych czcionek dla danego j�zyka.
	/*! Metoda zwracaj�ca ilo�� zdefiniowanych czcionek dla indeksu j�zyka zawartego w polu
	CWindowData::SSettingFile::iIndexLanguage.
	\param[in] iIndex Indeks j�zyka zdefiniowanego w pliku XML.
	\return Ilo�� zdefiniowanych czcionek dla danego j�zyka. W przypadku jakiego� b��du, np. nie u�ywania pliku XML,
	zwr�cona zostanie warto�� \p -1. */
	inline GLint GetXMLLngDataTTFSize(GLint iIndex) {
		if( !m_bUseXML || ( iIndex < 0 || iIndex > GetXMLLngDataSize() ) )
			return -1;
		return static_cast<GLint>( m_aXMLLngData[iIndex].aFileTTF.size() );
	}
	//@}

	/// \name Pola u�ywane tylko wtedy gdy wykorzystywany jest XML.
	//@{

	std::vector<SXMLLngData> m_aXMLLngData;		///< Tablica danych j�zyk�w opisanych w pliku XML.
	//@}

	GLboolean m_bUseXML;						///< Flaga wskazuj�ca czy wykorzystywany jest plik XML.
};