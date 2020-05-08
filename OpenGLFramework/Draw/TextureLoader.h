/*! \file
	\brief Plik nag��wkowy dla klasy CTextureLoader.
*/
#pragma once

/// Definicja nag��wka pliku JEH.
#define JEH_HEADER "JEH"

/// Klasa odpowiedzialana za wczytywanie tekstur.
/** Zadaniem tej klasy jest wczytywanie plik�w graficznych BMP, TGA oraz JEH i sporz�dzenie z nich tekstur gotowych
do renderingu. */

class CTextureLoader  
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CTextureLoader();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CTextureLoader();

	/// Metoda tworz�ca tekstur� typu mipmap.
	/** Metoda ta utworzy najlepiej wygl�daj�c� tekstur� mipmap-ow�.
	\param[in] lpFileName Pe�na nazwa pliku wraz ze �cie�k� i rozszerzeniem, kt�ry chcemy wczyta�. Obs�ugiwane formaty
	pliku to BMP, TGA oraz JEH.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1. */
	GLint LoadTexMipmaps( LPCTSTR lpFileName );

	/// Metoda tworz�ca tekstur� typu linear.
	/** \param[in] lpFileName Pe�na nazwa pliku wraz ze �cie�k� i rozszerzeniem, kt�ry chcemy wczyta�. Obs�ugiwane
	formaty	pliku to BMP, TGA oraz JEH.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1. */
	GLint LoadTex( LPCTSTR lpFileName );

	/// Metoda tworz�ca tekstur� typu nearest.
	/** Metoda ta utworzy najgorzej wygl�daj�c� tekstur� nearest.
	\param[in] lpFileName Pe�na nazwa pliku wraz ze �cie�k� i rozszerzeniem, kt�ry chcemy wczyta�. Obs�ugiwane formaty
	pliku to BMP, TGA oraz JEH.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1. */
	GLint LoadTexLowQuality( LPCTSTR lpFileName );

	/// Metoda tworz�ca tekstur� mipmap-ow� z mask� wczytywan� z pliku.
	/** \param[in] lpFileNameMask Pe�na nazwa pliku maski wraz ze �cie�k� i rozszerzeniem. Obraz maski musi by� w
	odcienich szaro�ci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe�nie przezroczysty. 
	\param[in] lpFileNameRGB Pe�na nazwa pliku wraz ze �cie�k� i rozszerzeniem, w kt�rym zosytanie wyci�ta maska.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1.*/
	GLint LoadMultiTexMaskMipmaps( LPCTSTR lpFileNameMask, LPCTSTR lpFileNameRGB );

	/// Metoda tworz�ca tekstur� mipmap-ow� z mask� wczytywan� z pliku.
	/** Odmiana powy�szej metody, ale tutaj zamiast drugiej tekstury podajemy kolor RGB. U�ywamy tej metody, gdy
	druga, kolorowa tekstura, ma jednolity kolor. Wtedy nie musimy tworzy� zb�dnej tektury, tylko od razu podajemy
	ten kolor jako COLORREF. Dzieki temu zaoszcz�dzamy pami�� i miejsce na dysku.
	\param[in] lpFileNameMask Pe�na nazwa pliku maski wraz ze �cie�k� i rozszerzeniem. Obraz maski musi by� w
	odcienich szaro�ci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe�nie przezroczysty. 
	\param[in] crColor Kolor, w kt�rym zostanie wyci�ta maska. Podajemy w makrze RGB.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1.*/
	GLint LoadMultiTexMaskMipmaps( LPCTSTR lpFileNameMask, COLORREF crColor );

	/// Metoda tworz�ca tekstur� linear z mask� wczytywan� z pliku.
	/** \param[in] lpFileNameMask Pe�na nazwa pliku maski wraz ze �cie�k� i rozszerzeniem. Obraz maski musi by� w
	odcienich szaro�ci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe�nie przezroczysty. 
	\param[in] lpFileNameRGB Pe�na nazwa pliku wraz ze �cie�k� i rozszerzeniem, w kt�rym zosytanie wyci�ta maska.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1.*/
	GLint LoadMultiTexMask( LPCTSTR lpFileNameMask, LPCTSTR lpFileNameRGB );

	/// Metoda tworz�ca tekstur� linear z mask� wczytywan� z pliku.
	/** \param[in] lpFileNameMask Pe�na nazwa pliku maski wraz ze �cie�k� i rozszerzeniem. Obraz maski musi by� w
	odcienich szaro�ci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe�nie przezroczysty. 
	\param[in] crColor Kolor, w kt�rym zostanie wyci�ta maska. Podajemy w makrze RGB.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1.*/
	GLint LoadMultiTexMask( LPCTSTR lpFileNameMask, COLORREF crColor );

	/// Metoda tworz�ca tekstur� nearest z mask� wczytywan� z pliku.
	/** \param[in] lpFileNameMask Pe�na nazwa pliku maski wraz ze �cie�k� i rozszerzeniem. Obraz maski musi by� w
	odcienich szaro�ci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe�nie przezroczysty. 
	\param[in] lpFileNameRGB Pe�na nazwa pliku wraz ze �cie�k� i rozszerzeniem, w kt�rym zosytanie wyci�ta maska.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1.*/
	GLint LoadMultiTexMaskLowQuality( LPCTSTR lpFileNameMask, LPCTSTR lpFileNameRGB );

	/// Metoda tworz�ca tekstur� nearest z mask� wczytywan� z pliku.
	/** \param[in] lpFileNameMask Pe�na nazwa pliku maski wraz ze �cie�k� i rozszerzeniem. Obraz maski musi by� w
	odcienich szaro�ci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe�nie przezroczysty. 
	\param[in] crColor Kolor, w kt�rym zostanie wyci�ta maska. Podajemy w makrze RGB.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1. */
	GLint LoadMultiTexMaskLowQuality( LPCTSTR lpFileNameMask, COLORREF crColor );

	/// Metoda wczytuj�ca tekstur� dla efektu emboss bump.
	/** Tekstura emboss bump zawsze jest tworozna jako mipmapa.
	\param[in] lpFileName Pe�na nazwa pliku bump wraz ze �cie�k� i rozszerzeniem. Obraz bump musi by� w
	odcienich szaro�ci.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1. */
	GLint LoadEmbossBump( LPCTSTR lpFileName );

	/// Metoda ustawiaj�ca tekstur� dla aktualnie ustawionej jednostki teksturuj�cej.
	/** Metod� t� nale�y wywo�a� w celu rysowania obiektu z na�o�on� dan� tekstur�.
	\param[in] iIndex Indeks tekstury jak� chcemy uczyni� aktywn�. Nale�y poda� tu indeks zwr�cony przez takie
	matody jak: CTextureLoader::LoadTexMipmaps, CTextureLoader::LoadTex,
	CTextureLoader::LoadTexLowQuality.
	\return \p GL_TRUE w przypadku powodzenia ustawienia tekstury. \p GL_FALSE w przypadku niepowodzenia, tj.
	podania b��dnego indeksu. */
	GLboolean SetTexture( GLint iIndex = 0 );

	/// Metoda ustawiaj�ca multi-tekstur� dla aktualnie ustawionej jednostki teksturuj�cej.
	/** Metod� t� nale�y wywo�a� w celu rysowania obiektu z na�o�on� multi-tekstur�.
	\param[in] iIndex Indeks tekstury jak� chcemy uczyni� aktywn�. Nale�y poda� tu indeks zwr�cony przez takie
	matody jak: CTextureLoader::LoadMultiTexMaskMipmaps, CTextureLoader::LoadMultiTexMask,
	CTextureLoader::LoadMultiTexMaskLowQuality.
	\return \p GL_TRUE w przypadku powodzenia ustawienia tekstury. \p GL_FALSE w przypadku niepowodzenia, tj.
	podania b��dnego indeksu. */
	GLboolean SetMultiTextures( GLint iIndex );

	/// Metoda wy�wietlaj�ca identyfikatory i przypisane im nazwy tekstur.
	GLvoid DisplayIDTextureName();

	/// Metoda zwracaj�ca liczb� okre�lajac� ilo�� wczytanych tekstur.
	/** \return Liczba ca�kowita okre�laj�ca ilo�� wczytanych tekstur.  */
	inline GLint GetTextureSize() { return static_cast<GLint>( m_aTexture.size() ); }

	/// Metoda usuwaj�ca wskazan� tekstur�.
	/** \param[in] iIndex Indeks tekstury, kt�r� chcemy usun��. */
	GLvoid DeleteTexture( GLint iIndex );

private:

	/// Struktura przechowuj�ca identyfikatory wczytanych tekstur.
	struct STextureData
	{
		GLuint uiTexture;		///< Identyfikator tekstury.
		TCHAR *lpFileName;		///< Nazwa pliku tekstury, kt�rej przypisany jest identyfikator. 
	};

	/// Struktua pomocnicza, przechowuj�ca dane obrazu.
	struct STexture
	{
		GLubyte* ubImageData;		///< Wska�nik na dane obrazu.
		GLuint uiBitsPerPixel;		///< Ilo�� bit�w na piksel.
		GLuint uiWidth;				///< Szeroko�� obrazu w pikselach.
		GLuint uiHeight;			///< Wysoko�� obrazu w pikselach.
		GLuint uiType;				///< Format tekstury, tj. GL_RGB, GL_RGBA.
	};

	/// Struktura pomocnicza przy wczytywaniu pliku TGA.
	struct STGAFile
	{
		GLubyte ubHeader[ 18 ];		///< Nag�owek pliku TGA.
		GLuint uiBytesPerPixel;		///< Ilo�� bajt�w na piksel.
		GLuint uiImageSize;			///< Rozmiar obrazu w bajtach/
		GLuint uiType;				///< Format tekstury, tj. GL_RGB, GL_RGBA.
		GLuint uiWidth;				///< Szeroko�� obrazu w pikselach.
		GLuint uiHeight;			///< Wysoko�� obrazu w pikselach.
		GLuint uiBitsPerPixel;		///< Ilo�� bit�w na piksel.
	};

	/// Struktura reprezentuj�ca nag��wek pliku JEH.
	struct SHeaderJEH
	{
		char cHeader[ 4 ];			///< Nag�owek identyfikuj�cy format pliku.
		unsigned int uiWidth;		///< Szeroko�� obrazu w pikselach.
		unsigned int uiHeight;		///< Wysoko�� obrazu w pikselach.
		unsigned int uiSizeImage;	///< Wielko�� danych obrazu w bajtach.
		unsigned int uiBitCount;	///< Ilo�� bit�w na piksel.
	};

	/// Metoda sprawdzaj�ca rozszerzenie pliku.
	/** \param[in] lpFileName Pe�na nazwa pliku wraz z rozszerzeniem, kt�r� chcemy sprawdzi�.
	\return Liczba ca�kowita okre�laj�ca rozszerzenie pliku, gdzie \p 0 to BMP, \p 1 TGA, \p 2 JEH. W przypadku
	innego, nie rozpoznawalnego rozszerzenia zwraca \p -1. */
	GLint CheckExtension( LPCTSTR lpFileName );

	/// Metoda wczytuj�c oraz tworz�ca normaln� tekstur�.
	/** \param[in] lpFileName Pe�na nazwa pliku wraz ze �cie�k� i rozszerzeniem, kt�ry chcemy wczyta�. 
	\param[in] iMode Typ tekstury, gdzie \p 0 - mipmapa, \p 1 - linear, \p 2 - nearest. W przypadku podania
	nieobs�ugiwanego typu, przyj�te zostanie \p 0.
	\param[in] iFormat Format pliku podanego w \a lpFileName. Mo�liwe warto�ci: \p 0 - BMP, \p 1 - TGA oraz \p 2 - 
	JEH.
	\param[in] bEmbossBump Flaga wskazuj�ca czy utworzona ma by� tekstura emboss bump.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1. */
	GLint LoadGLTex( LPCTSTR lpFileName, GLint iMode = 0, GLint iFormat = 0, GLboolean bEmbossBump = GL_FALSE );
	
	/// Metoda wczytuj�c oraz tworz�ca multi-tekstur� z mask� wczytan� z pliku.
	/** \param[in] lpFileName1 Pe�na nazwa pliku maski wraz ze �cie�k� i rozszerzeniem. Obraz maski musi by� w
	odcienich szaro�ci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe�nie przezroczysty.
	\param[in] lpFileName2 Pe�na nazwa pliku wraz ze �cie�k� i rozszerzeniem, w kt�rym zosytanie wyci�ta maska.
	\param[in] iMode Typ tekstury, gdzie \p 0 - mipmapa, \p 1 - linear, \p 2 - nearest. W przypadku podania
	nieobs�ugiwanego typu, przyj�te zostanie \p 0.
	\param[in] iFormat1 Format pliku podanego w \a lpFileName1. Mo�liwe warto�ci: \p 0 - BMP, \p 1 - TGA oraz \p 2 - 
	JEH.
	\param[in] iFormat2 Format pliku podanego w \a lpFileName2. Mo�liwe warto�ci: \p 0 - BMP, \p 1 - TGA oraz \p 2 - 
	JEH.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1. */
	GLint LoadGLMultiTexMask( LPCTSTR lpFileName1, LPCTSTR lpFileName2, GLint iMode = 0, GLint iFormat1 = 0, GLint iFormat2 = 0 );

	/// Metoda wczytuj�c oraz tworz�ca multi-tekstur� z mask� wczytan� z pliku.
	/** Odmiana powy�szej metody, ale tutaj zamiast drugiej tekstury podajemy kolor RGB. U�ywamy tej metody, gdy
	druga, kolorowa tekstura, ma jednolity kolor. Wtedy nie musimy tworzy� zb�dnej tektury, tylko od razu podajemy
	ten kolor jako COLORREF. Dzieki temu zaoszcz�dzamy pami�� i miejsce na dysku.
	\param[in] lpFileNameMask Pe�na nazwa pliku maski wraz ze �cie�k� i rozszerzeniem. Obraz maski musi by� w
	odcienich szaro�ci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe�nie przezroczysty.
	\param[in] crColor Kolor, w kt�rym zostanie wyci�ta maska. Podajemy w makrze RGB.
	\param[in] iMode Typ tekstury, gdzie \p 0 - mipmapa, \p 1 - linear, \p 2 - nearest. W przypadku podania
	nieobs�ugiwanego typu, przyj�te zostanie \p 0.
	\param[in] iFormat Format pliku podanego w \a lpFileNameMask. Mo�liwe warto�ci: \p 0 - BMP, \p 1 - TGA oraz \p 2 - 
	JEH.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1. */
	GLint LoadGLMultiTexMask( LPCTSTR lpFileNameMask, COLORREF crColor, GLint iMode = 0, GLint iFormat = 0 );

	/// Metoda tworz�ca tekstur� dla efektu emboss bump.
	/** \param[in] iIndex Indeks tablicy CTextureLoader::m_aTexture.
	\param[in] texTexture Referencja na obiekt typu CTextureLoader::STexture, gdzie przechowywane s� dane obrazu.
	\param[in] iMode Typ tekstury, gdzie \p 0 - mipmapa, \p 1 - linear, \p 2 - nearest. W przypadku podania
	nieobs�ugiwanego typu, przyj�te zostanie \p 0.
	\return Indeks utworzonej tekstury licz�c od \p 0. W przypadku b��du zwraca \p -1. */
	GLint CreateEmbossBumpTexture( GLint iIndex, STexture &texTexture, GLint iMode );

	/// Metoda wczytuj�ca plik BMP.
	/** \param[out] out_sTexture Referencja na obiekt typu CTextureLoader::STexture, przez kt�ry zwracane s� dane
	dotycz�ce wczytanego obrazu.
	\param[in] lpFileName Nazwa pliku, kt�ry zostanie wczytany. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean LoadBMP( STexture &out_sTexture, LPCTSTR lpFileName );

	/// Metoda wczytuj�ca plik TGA.
	/** \param[out] out_sTexture Referencja na obiekt typu CTextureLoader::STexture, przez kt�ry zwracane s� dane
	dotycz�ce wczytanego obrazu.
	\param[in] lpFileName Nazwa pliku, kt�ry zostanie wczytany. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean LoadTGA( STexture &out_sTexture, LPCTSTR lpFileName );

	/// Metoda wczytuj�ca plik JEH.
	/** \param[out] out_sTexture Referencja na obiekt typu CTextureLoader::STexture, przez kt�ry zwracane s� dane
	dotycz�ce wczytanego obrazu.
	\param[in] lpFileName Nazwa pliku, kt�ry zostanie wczytany. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean LoadJEH( STexture &out_sTexture, LPCTSTR lpFileName );

	/// Licznik kontrolny, ilo�ci wczytywanych tekstur.
	GLint m_iCounterTexture;

	/// Tablica przechowuj�ca identyfikatory wszytskich wczytanych tekstur.
	std::vector<STextureData> m_aTexture;
};