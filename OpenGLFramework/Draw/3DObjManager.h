/*! \file
	\brief Plik nag��wkowy dla klasy C3DObjManager.
*/
#pragma once

class C3DObjFile;
class CGameControl;
class CVector3;

/// Manager zarz�dzajacy obiektami 3D, wczytanymi z autorskiego formatu pliku \a 3DObj.
/** Zadaniem tej klasy jest zapewnienie wczytania plik�w \a 3DObj oraz umo�liwienie rysowania tr�jwymiarowych
obiekt�w zawartych w tych plikach. */
class C3DObjManager
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	C3DObjManager();

	/// Destruktor.
	/** Destruktor. */
	~C3DObjManager();

	/// Metoda inicjalizuj�ca ustawienia 3D Object Manager-a.
	/** Metoda ta jest ju� wywo�ywana w CGameControl::LoadAll3DObjFiles i musi by� wywo�ana na samym ko�cu, przed
	wczytywaniem plik�w \a 3DObj. */
	GLvoid Initialization();

	/// Metoda wczytuj�ca plik \a 3DObj.
	/** Metod� t� nale�y wywo�ywa� wewn�trz	CGameControl::LoadAll3DObjFiles tyle razy ile chcemy wczyta� obiekt�w
	\a 3DObj. 
	\param[in] lpFileName Nazwa pliku z rozszerzeniem \a 3DObj, kt�ry chcemy wczyta�.
	\param[in] iTextureFirst Indeks tekstury jaka zostanie na�o�ona na obiekt. Indeksy liczone s� od \p 0 w kolejno�ci
	wczytywania tekstur w metodzie CGameControl::LoadAllTextures. Je�eli zamierzamy wykorzysta� multiteksturing,
	parametr ten stanowi indeks pierwszej tekstury jaka zostanie na�o�ona na obiekt.
	\param[in] iTextureCount Tylko dla multiteksturingu � ilo�� kolejnych tekstur po \a iTextureFirst, jakie zostan�
	na�o�one na obiekt. Aby mia�o sens, nale�y poda� co najmniej warto�� \p 2. Je�eli podamy warto�� mniejsz�
	od \p 2, multiteksturing nie zostanie u�yty. Domy�lna warto�� dla \a iTextureCount wynosi \p -1, czyli bez
	multiteksturingu.
	\param[in] iTextureEmbossBump Indeks tekstury jaka zostanie na�o�ona na obiekt w celu uzyskania efektu wyboj�w.
	Musi to by� specjalnie do tego celu przygotowana tekstura, najcz�ciej w odcieniach szaro�ci. Nale�y pami�ta�,
	�e je�eli chcemy wykorzysta� tekstur� bump to parametr \a iTextureCount musi by� r�wny warto�ci domy�lnej, tj.
	\p -1. Je�eli nie chcemy wykorzysta� mapowania wyboj�w to nale�y parametr ten ustawi� na warto�� domy�ln�,
	czyli \p -1.
	\return Indeks nowo stworzonego obiektu. W przypadku wyst�pienia b��du, zwracana jest warto�� \p -1. */
	GLint Create3DObject( LPCTSTR lpFileName, GLint iTextureFirst, GLint iTextureCount = -1, GLint iTextureEmbossBump = -1 );
	
	/// Metoda rysuj�ca pojedynczy obiekt.
	/** Za pomoc� tej metody mo�emy narysowa� pojedynczy obiekt wczytany z pliku \a 3DObj. Jednak dla lepszej
	wydajno�ci zaleca si� u�y� C3DObjManager::Draw3DObject_Lists.
	\param[in] iIndex Indeks obiektu jaki chcemy narysowa�. Indeksy liczone s� od \p 0 w kolejno�ci wczytywania
	obiekt�w, tj. w kolejno�ci wywo�ywania metody C3DObjManager::Create3DObject.
	\param[in] bUseEmbossBump Ustawiaj�c na \p GL_TRUE obiekt b�dzie rysowany z efektem mapowania wyboj�w. Aby
	jednak tak si� sta�o nale�y uprzednio w metodzie C3DObjManager::Create3DObject poda� indeks tekstury bump oraz
	ilo�� kolejnych tekstur dla multiteksturingu pozostawi� w warto�ci domy�lnej. Ustawiaj�c na \p GL_FALSE (warto��
	domy�lna) efekt mapowania wyboj�w nie b�dzie u�ywany przy rysowaniu obiektu, nawet je�eli przy tworzeniu obiektu
	podali�my tekstur� bump.
	\param[in] uiLight Identyfikator �wiat�a, u�ywany jest tylko dla efektu mapowania wyboj�w. Nale�y u�y�
	\p GL_LIGHTx, gdzie \p x to cyfra od \p 0 do \p 7.
	\sa Create3DObject_Lists. */
	GLvoid Draw3DObject( GLint iIndex, GLboolean bUseEmbossBump = GL_FALSE, GLuint uiLight = GL_LIGHT0 );

	/// Metoda rysuj�ca pojedynczy obiekt.
	/** Za pomoc� tej metody mo�emy narysowa� pojedynczy obiekt wczytany z pliku \a 3DObj. Obiekt taki zostanie
	narysowany z wykorzystaniem listy wy�wietlania co znacznie zwi�kszy wydajno�� rysowania w pr�wnaniu z
	metod� C3DObjManager::Draw3DObject.
	\param[in] iIndex Indeks obiektu jaki chcemy narysowa�. Indeksy liczone s� od \p 0 w kolejno�ci wczytywania
	obiekt�w, tj. w kolejno�ci wywo�ywania metody C3DObjManager::Create3DObject.
	\param[in] bUseEmbossBump Ustawiaj�c na \p GL_TRUE obiekt b�dzie rysowany z efektem mapowania wyboj�w. Aby
	jednak tak si� sta�o nale�y uprzednio w metodzie C3DObjManager::Create3DObject poda� indeks tekstury bump oraz
	ilo�� kolejnych tekstur dla multiteksturingu pozostawi� w warto�ci domy�lnej. Ustawiaj�c na \p GL_FALSE (warto��
	domy�lna) efekt mapowania wyboj�w nie b�dzie u�ywany przy rysowaniu obiektu, nawet je�eli przy tworzeniu obiektu
	podali�my tekstur� bump.
	\param[in] uiLight Identyfikator �wiat�a, u�ywany jest tylko dla efektu mapowania wyboj�w. Nale�y u�y�
	\p GL_LIGHTx, gdzie \p x to cyfra od \p 0 do \p 7.
	\sa Draw3DObject. */
	GLvoid Draw3DObject_Lists( GLint iIndex, GLboolean bUseEmbossBump = GL_FALSE, GLuint uiLight = GL_LIGHT0 );

	/// \cond DEV
	GLboolean SaveObject( const CVector3 &in_cScale, const CVector3 &in_cVector, const CVector3 &in_cRot90, GLboolean bSmoothNormal );
	GLboolean SaveObjectElem( GLboolean bSmoothNormal, GLboolean bFixed );
	/// \endcond DEV

	/// Metoda pobieraj�ca pierwsz� tekstur� danego obiektu
	/**
	\param[in] iIndex Indeks obiektu z kt�rego chcemy pobra� pierwsz� tekstur�. Indeksy liczone s� od \p 0 w
	kolejno�ci wczytywania obiekt�w, tj. w kolejno�ci wywo�ywania metody C3DObjManager::Create3DObject.
	\return Index pierwszej tekstury nak�adanej na obiekt. */
	GLint GetTextureFirstIndex( GLint iIndex );

private:

	/// Struktura przechowuj�ca niezb�dne informacje dla ka�dego obiektu.
	/** Struktura ta przechowuje niezb�dne informacje dla ka�dego wczytanego obiektu. Informacje te s� potrzebne
	przy rysowaniu obiekt�w. */
	struct S3DObject
	{
		GLint iIndexStart;			///< Pierwszy indeks z tablicy wierzcho�k�w dla danego obiektu.
		GLint iIndexEnd;			///< Ostatni indeks z tablicy wierzcho�k�w dla danego obiektu.
		GLint iTextureFirst;		///< Indeks pierwszej tekstury nak�adanej na obiekt.
		GLint iTextureCount;		///< Ilo�� kolejnych tekstur jakie b�d� nak��dane na obiekt.
		GLint iTextureEmbossBump;	///< Indeks tekstury emboss bump.
		GLboolean bIsElem;			///< Flaga wskazuj�ca czy wierzcho�ki obiektu s� indeksowane.
	};

	CGameControl	*m_pGameCtrl;		///< Wska�nik na obiekt klasy singletonu CGameControl.
	C3DObjFile		*m_p3DObjFile;	///< Wska�nik na obiekt klasy C3DObjFile.
	std::vector<S3DObject> m_a3DObject; ///< Dynamiczna tablica obiekt�w C3DObjManager::S3DObject.
	
	/// Metoda zwracaj�ca wielko�� tablicy C3DObjManager::m_a3DObject.
	/** \return Ilo�� element�w tablicy C3DObjManager::m_a3DObject. */
	inline GLint Get3DObjectSize() { return static_cast<GLint>( m_a3DObject.size() ); }

	/// Metoda rysuj�ca pojedynczy obiekt.
	/** Metoda ta rysuje obiekt wczytany z pliku \a 3DObj, bez indeksowania wierzcho�k�w.
	\param[in] iIndex Indeks obiektu jaki chcemy narysowa�. Indeksy liczone s� od \p 0 w kolejno�ci wczytywania
	obiekt�w, tj. w kolejno�ci wywo�ywania metody C3DObjManager::Create3DObject.
	\param[in] bUseEmbossBump Ustawiaj�c na \p GL_TRUE obiekt b�dzie rysowany z efektem mapowania wyboj�w. Aby
	jednak tak si� sta�o nale�y uprzednio w metodzie C3DObjManager::Create3DObject poda� indeks tekstury bump oraz
	ilo�� kolejnych tekstur dla multiteksturingu pozostawi� w warto�ci domy�lnej. Ustawiaj�c na \p GL_FALSE (warto��
	domy�lna) efekt mapowania wyboj�w nie b�dzie u�ywany przy rysowaniu obiektu, nawet je�eli przy tworzeniu obiektu
	podali�my tekstur� bump.
	\param[in] uiLight Identyfikator �wiat�a, u�ywany jest tylko dla efektu mapowania wyboj�w. Nale�y u�y�
	\p GL_LIGHTx, gdzie \p x to cyfra od \p 0 do \p 7.
	\sa Draw3DObjectElem. */
	GLvoid Draw3DObjectNoElem( GLint iIndex, GLboolean bUseEmbossBump = GL_FALSE, GLuint uiLight = GL_LIGHT0 );

	/// Metoda rysuj�ca pojedynczy obiekt.
	/** Metoda ta rysuje obiekt wczytany z pliku \a 3DObj, z indeksowaniem wierzcho�k�w.
	\param[in] iIndex Indeks obiektu jaki chcemy narysowa�. Indeksy liczone s� od \p 0 w kolejno�ci wczytywania
	obiekt�w, tj. w kolejno�ci wywo�ywania metody C3DObjManager::Create3DObject.
	\param[in] bUseEmbossBump Ustawiaj�c na \p GL_TRUE obiekt b�dzie rysowany z efektem mapowania wyboj�w. Aby
	jednak tak si� sta�o nale�y uprzednio w metodzie C3DObjManager::Create3DObject poda� indeks tekstury bump oraz
	ilo�� kolejnych tekstur dla multiteksturingu pozostawi� w warto�ci domy�lnej. Ustawiaj�c na \p GL_FALSE (warto��
	domy�lna) efekt mapowania wyboj�w nie b�dzie u�ywany przy rysowaniu obiektu, nawet je�eli przy tworzeniu obiektu
	podali�my tekstur� bump.
	\param[in] uiLight Identyfikator �wiat�a, u�ywany jest tylko dla efektu mapowania wyboj�w. Nale�y u�y�
	\p GL_LIGHTx, gdzie \p x to cyfra od \p 0 do \p 7.
	\sa Draw3DObjectNoElem. */
	GLvoid Draw3DObjectElem( GLint iIndex, GLboolean bUseEmbossBump = GL_FALSE, GLuint uiLight = GL_LIGHT0 );

	/// Metoda tworz�ca listy wy�wietlania.
	/** Metoda ta tworzy listy wy�wietlania dla rysowania wczelkich wczytanych obiekt�w \a 3DObj bez u�yca
	efektu emboss bump. Metoda ta jest ju� wywo�ywana w C3DObjManager::Initialization. 
	\sa CreateListsBump. */
	GLvoid CreateLists();

	/// Metoda tworz�ca listy wy�wietlania z efektem bump.
	/** Metoda ta tworzy listy wy�wietlania dla rysowania wczelkich wczytanych obiekt�w \a 3DObj z u�yciem
	efektu emboss bump. Metoda ta jest ju� wywo�ywana w C3DObjManager::Initialization. 
	\param[in] iMaxLights Dla ilu �wiate� (rozpoczynaj�c od \p GL_LIGHT0) maj� by� tworzone listy wy�wietlania
	dla jednego obiektu.
	\sa CreateLists. */
	GLvoid CreateListsBump( GLint iMaxLights = 8 );

	/// Metoda usuwaj�ca wszelkie listy wy�wietlania.
	/** Metoda ta usuwa listy wy�wietlania stworozne przez metody C3DObjManager::CreateLists oraz
	C3DObjManager::CreateListsBump. */
	GLvoid DeleteLists();
	
	GLuint m_ui3DObjLists;	///< Identyfikator przylegaj�cych do siebie list wy�wietlania stworzonych metod� C3DObjManager::CreateLists.

	/// Tablica identyfikator�w przylegaj�cych do siebie list wy�wietlania.
	/** Tablica identyfikator�w przylegaj�cych do siebie list wy�wietlania stworzonych metod�
	C3DObjManager::CreateListsBump. Indeks tablicy odnosi si� do indeksu �wiat�a, indeks \p 0 dla \p GL_LIGHT0,
	indeks \p 1 dla \p GL_LIGHT1, itd. do \p GL_LIGHT7.	*/
	GLuint m_ui3DObjListsBump[ 8 ];
	
	GLint m_iMaxLightsForLists; ///< Parametr \a iMaxLights pochodz�cy z metody C3DObjManager::CreateListsBump.
};
