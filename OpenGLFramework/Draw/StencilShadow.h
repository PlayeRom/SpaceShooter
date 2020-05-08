/*! \file
	\brief Plik nag��wkowy dla klasy CStencilShadow.
*/
#pragma once

/// Klasa wspomagaj�ca rysowanie dynamicznych cieni shadow volume.
/** Klasa udost�pnia metody rysuj�ce cie� w oparciu o bufor szablonowy. */
class CStencilShadow
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CStencilShadow();

	/// Destruktor.
	/** Destruktor. */
	~CStencilShadow();

	/// Metoda restartuj�ca wszelkie niezb�dne dane.
	/** Metoda ta jest ju� wywo�ywana w trakcie restartowania okna aplikacji. */
	GLvoid RestartObjects();
	
	/// Metoda obliczaj�ca r�wnanie p�aszczyzny dla danego obiektu 3D bez indeksowania wierzcho�k�w.
	/** Metoda ta dla ka�dego tr�j�ta obiektu 3D, oblicza r�wnanie p�aszczyzny. Wyniki oblicze� zapisywane s� w
	dynamicznie tworzonej tablicy CStencilShadow::m_aObjFaces. Metoda ta jest ju� wywo�ywana dla ka�dego wczytywanego
	obiektu w C3DObjManager::Create3DObject.
	\param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] iFirstVertex Indeks pierwszego wierzcho�ka obiektu 3D z tablicy wierzcho�k�w CVertexArray.
	\param[in] iCountVertices Ilo�� wierzcho�k�w jak� zawiera obiekt 3D. */
	GLvoid CreateFaceEquation( GLint iIndexObj, GLint iFirstVertex, GLsizei iCountVertices );

	/// Metoda obliczaj�ca r�wnanie p�aszczyzny dla danego obiektu 3D z indeksowaniem wierzcho�k�w.
	/** Metoda ta dla ka�dego tr�j�ta obiektu 3D, oblicza r�wnanie p�aszczyzny. Wyniki oblicze� zapisywane s� w
	dynamicznie tworzonej tablicy CStencilShadow::m_aObjFaces. Metoda ta jest ju� wywo�ywana dla ka�dego wczytywanego
	obiektu w C3DObjManager::Create3DObject.
	\param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] iCountVertices Ilo�� wierzcho�k�w jak� zawiera obiekt 3D. */
	GLvoid CreateFaceEquationElem( GLint iIndexObj, GLsizei iCountVertices );

	/// Metoda obliczajac� pozycj� �wiat�a niezb�d� dla narysowania cienia.
	/** Metod� t� nale�y wywo�a� po przekszta�ceniu macierzy widoku w ca�kowicie odwrotny spos�b od tego jak
	by�my przekszta�cali dla normalnego rysowania obiektu.
	\param[in] in_cLightPos Oryginalna pozycja �wiat�a w przestrzeni 3D. 
	\return Nowa pozycja �wiat�a wyliczona z odr�conej macierzy widoku. */
	CVector4 GetLightPos( const CVector4 &in_cLightPos = CVector4( 0.0f, 0.0f, 0.0f, 1.0f ) );

	/// Metoda rysuj�ca cie� dla danego obiektu 3D.
	/** \param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] in_cLightPos Pozycja �wiat�a wyliczona metod� CStencilShadow::GetLightPos. */
	GLvoid DrawShadowForObj( GLint iIndexObj, const CVector4 &in_cLightPos );

	/// Metoda rysuj�ca cienie dla kilku obiekt�w na raz.
	/** Aby skorzysta� z tej metody nale�y uprzednio, odpowiednio wywo�a� seri� metod \a ..._ObjTransfInfo.
	\sa Begin_ObjTransfInfo, Set_ObjTransfInfo, Translate_ObjTransfInfo, Rotate_ObjTransfInfo, End_ObjTransfInfo. */
	GLvoid DrawShadowForObjects();

	/// \name Zestaw metod do rysowania cieni dla kilku obiekt�w na raz.
	//@{

	/// Metod inicjalizuj�ca zbieranie informacji o przekszta�ceniach macierzy widoku dla kilku obiekt�w 3D.
	/** Metod� t� nale�y wywo�a� na samym pocz�tku rysowania danych obiekt�w 3D, kt�re maj� rzuca� cie�.
	\sa End_ObjTransfInfo. */
	GLvoid Begin_ObjTransfInfo();

	/// Metoda ustawiaj�ca indeks obiektu 3D oraz pozycj� �wiat�a.
	/** Przed wywyo�aniem tej metody nale�y uprzednio wywo�a� CStencilShadow::Begin_ObjTransfInfo.
	\param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj, kt�ry b�dzie rzuca� cie�.
	\param[in] in_cLightPos Pozycja �wiat�a wyliczona z odwonie przekszta�conej macierzy widoku.
	\sa GetLightPos. */
	GLvoid Set_ObjTransfInfo( GLint iIndexObj, const CVector4 &in_cLightPos );

	/// Metoda zapamietuj�ca pojedyncz� opracj� translacji.
	/** Przed wywyo�aniem tej metody nale�y uprzednio wywo�a� CStencilShadow::Begin_ObjTransfInfo.
	Metod� t� nale�y wywo�a� zawsze wraz z wykonaniem operacji glTranslate podczas normalnego rysowania obiektu 3D.
	\param[in] in_cTranslate Wektor o jaki zostanie wykonane przesuni�cie, zgodnie z glTranslate. */
	GLvoid Translate_ObjTransfInfo( const CVector3 &in_cTranslate );

	/// Metoda zapamietuj�ca pojedyncz� opracj� rotacji.
	/** Przed wywyo�aniem tej metody nale�y uprzednio wywo�a� CStencilShadow::Begin_ObjTransfInfo.
	Metod� t� nale�y wywo�a� zawsze wraz z wykonaniem operacji glRotate podczas normalnego rysowania obiektu 3D.
	\param[in] fAngle K�t o jaki zostanie wykonany obr�t, zgodnie z glRotate.
	\param[in] in_cAxesRot Wektor wskazuj�cy, wok� kt�rych osi zostanie wykonany obr�t - zgodnie z tym co podajemy w 
	glRotate. */
	GLvoid Rotate_ObjTransfInfo( GLfloat fAngle, const CVector3 &in_cAxesRot );

	/// Metoda zapamietuj�ca pojedyncz� opracj� skalowania.
	/** Przed wywyo�aniem tej metody nale�y uprzednio wywo�a� CStencilShadow::Begin_ObjTransfInfo.
	Metod� t� nale�y wywo�a� zawsze wraz z wykonaniem operacji glScale podczas normalnego rysowania obiektu 3D.
	\param[in] in_cScale Wektor o jaki zostanie wykonane skalowanie, zgodnie z glScale. */
	GLvoid Scale_ObjTransfInfo( const CVector3 &in_cScale );

	/// Metod zamykajaca zbieranie informacji o przekszta�ceniach macierzy widoku dla kilku obiekt�w 3D.
	/** Metod� t� nale�y wywo�a� na samym ko�cu rysowania danych obiekt�w 3D, kt�re maj� rzuca� cie�.
	\sa Begin_ObjTransfInfo. */
	GLvoid End_ObjTransfInfo();

	/// Metoda czyszcz�ca wszelkie tablice zwi�zane ze zbieraniem informacji o przekszta�ceniach macierzy widoku dla kilku obiekt�w 3D.
	GLvoid ClearObjTransformInfo();
	//@}

private:
	enum EDrawArrayMode
	{
		EFloat,
		EInt
	};

	/// Struktura r�wnania p�aszczyzny.
	/** Struktura ta przechowuje wszelkie informacje o danym tr�jk�cie obiektu 3D potrzebnych do wyliczenia cieni. */
	struct SFaceEquation
	{
		/// Tablica przechowuj�ca indeksy trzech wierzcho�k�w.
		/** Tablica przechowuj�ca indeksy (z tablicy CVertexArray::m_aShortIndices) trzech wierzcho�k�w tworz�cych
		tr�jk�, dla kt�rego obliczane jest r�wanie p�aszczyzny. */
		GLint aIndexFace[ 3 ];

		/// Dane dla rownania plaszczyzny ax + by + cz + d = 0.
		CVector4 cFaceEq;
		//GLfloat a, b, c, d;
		GLboolean bVisible;		///< Flaga wskazuj�ca czy tr�jk�t jest o�wietlony.
		GLint iNeighbour[ 3 ];	///< Indeksy innych tr�jk�t�w s�siaduj�cych z dan� kraw�dzi� tr�jk�ta.
	};

	/// Struktura tr�jkat�w obiektu 3D.
	/** Struktura ta przechowuje niezb�dne informacje o wszystkich tr�jk�tach dla danego obiektu 3D. */
	struct SObjFaces
	{
		GLint iIndexObj;		///< Index obiektu kt�remu przynale�y dana stuktura.
		GLboolean bIsElem;		///< Flaga wskazuj�ca czy dla danego obiektu 3D u�ywane jest indeksowanie wierzcho�k�w.
		std::vector<SFaceEquation> aFaceArray; ///< Tablica wszystkich tr�jkat�w z jakich sk��da si� obiekt 3D.
	};

	//idea jest taka aby wywolac operacje na stencil buforze tylko raz, i pomiedzy 
	//DECR i INCR rysowac wszytskie obiekty na raz, ktore maja rzucic cien.
	//Dlatego potrzebujemy struktury, ktora przechowa wszelkie informacje o
	//przeksztalceniach dla danego obiektu

	/// Typ wyliczeniowy rodzaj�w przekszta�ce� macierzy widoku.
	enum ETrnasformMode
	{
		ETransfTranslate,
		ETransfRotate,
		ETransfScale
	};

	/// Struktura przechowuj�ca pojedyncz� informacj� o przekszta�ceniach macierzy widoku.
	struct STransform
	{
		ETrnasformMode eTransfMode;		///< Rodzaj przekszta�cenia.
		CVector3 cTransform;			///< Wektor przesuni�cia, osie obrotu lub warto�ci skalowania.
		GLfloat fAngleRot;				///< K�t obrotu, je�eli eTransfMode = ETrnasformMode::ETransfRotate.
	};

	/// Struktura przechowuj�ca wszelkie informacje o przekszta�ceniach macierzy widoku.
	/** Struktura przechowuj�ca wszelkie informacje o przekszta�ceniach macierzy widoku dla danego obiektu 3D. */
	struct SObjectTransformInfo
	{
		GLint iIndexObj;		///< Indeks obiektu 3D wczytanego z pliku \a 3DObj.
		CVector4 cLightPos;		///< Pozycja �wiat�a wyliczona przez CStencilShadow::GetLightPos.
		std::vector<STransform> aTransform;	///< Tablica wszelkich przekszta�ace� macierzy widoku.
	};

	std::vector< SObjFaces > m_aObjFaces;		///< Tablica tr�jk�t�w dla danego obiektu 3D.

	/// Metoda zwracaj�ca ilo�� element�w tablicy CStencilShadow::m_aObjFaces.
	/** \return Liczba ca�kowita okre�laj�ca ilo�� element�w tablicy CStencilShadow::m_aObjFaces. */
	inline GLint GetObjFacesSize() { return static_cast<GLint>( m_aObjFaces.size() ); }

	/// Metoda pobieraj�ca ilo�� tr�jk�t�w danego obiektu 3D.
	/** \param[in] iIndex Indeks tablicy CStencilShadow::m_aObjFaces z kt�rego chcemy pobta� ilo�� tr�jkat�w. */
	inline GLint GetFaceArraySize( GLint iIndex )	{ 
		return static_cast<GLint>( m_aObjFaces[ iIndex ].aFaceArray.size() );
	}

	/// Metoda czyszcz�ca tablic� CStencilShadow::m_aObjFaces i jego podtablice.
	GLvoid ClearObjFaces();

	SObjectTransformInfo m_sObjTransfInfo;	///< Obiekt struktury CStencilShadow::SObjectTransformInfo.
	std::vector<SObjectTransformInfo> m_aObjTransformInfo; ///< Tablica przechowuj�ca informacje o wszelkch przekszta�ceniach macierzy widoku.

	/// Metoda zwracajaca wielko�� tablicy CStencilShadow::m_aObjTransformInfo.
	/** \return Liczba ca�kowita okre�laj�ca ilo�� element�w tablicy CStencilShadow::m_aObjTransformInfo. */
	inline GLint GetObjTransformInfoSize() { return static_cast<GLint>( m_aObjTransformInfo.size() ); }

	/// Metoda wyszuj�ca s�siaduj�ce tr�jkaty.
	/** \param[in] iIndex Indeks tablicy CStencilShadow::m_aObjFaces obiektu 3D, dla kt�rego tr�jkat�w b�d�
	wyszukiwanie s�siadu�ce tr�jk�ty. */
	GLvoid CalculateNeighbours( GLint iIndex );

	/// Metoda rysuj�ca cie� dla obiektu 3D z indeksowaniem wierzcho�k�w.
	/** \param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] in_cLightPos Pozycja �wiat�a wyliczona metod� CStencilShadow::GetLightPos. */
	GLvoid DoDrawShadowForObjElem( GLint iIndexObj, const CVector4 &in_cLightPos );

	/// Metoda rysuj�ca cie� dla obiektu 3D bez indeksowania wierzcho�k�w.
	/** \param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] in_cLightPos Pozycja �wiat�a wyliczona metod� CStencilShadow::GetLightPos. */
	GLvoid DoDrawShadowForObjNoElem( GLint iIndexObj, const CVector4 &in_cLightPos );

	/// Metoda mno��ca wektor pozycji �wiat�� przez macierz.
	/** \param[in] M Tablica opisuj�ca macierz 4x4. 
	\param[in,out] in_out_cLightPos Oryginalna pozycja �wiat�a w przestrzeni 3D.  */
	GLvoid VMatMult( GLfloat M[ 16 ], CVector4 &in_out_cLightPos );

	GLboolean m_bIsGL_EXT_stencil_wrap; ///< Flaga wskazuj�ca czy karta graficzna obs�uguje rozszerzenie "GL_EXT_stencil_wrap".

	/// Metoda wyznaczajaca czy dany tr�jk� obiektu 3D jest o�wietlony przez �wiat�o.
	/** \param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] in_cLightPos Pozycja �wiat�a wyliczona metod� CStencilShadow::GetLightPos. */
	GLvoid SetVisibleFaces( GLint iIndexObj, const CVector4 &in_cLightPos );

	/// Metoda aktywuj�ca u�ycie bufora szablonowego.
	GLvoid StartStencil();

	/// Metoda dezaktywuj�ca u�ycie bufora szablonowego oraz ostatecznie rysuj�ca prostok�t z cieniem.
	GLvoid FinishStencil();

	GLvoid DrawArrays( EDrawArrayMode eMode, GLvoid* pArray, GLenum uiDAMode, GLsizei iCount );

	std::vector< CVector3 > m_aVertices;
};
