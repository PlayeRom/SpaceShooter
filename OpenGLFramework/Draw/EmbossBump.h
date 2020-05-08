/*! \file
	\brief Plik nag��wkowy dla klasy CEmbossBump.
*/
#pragma once

class CVector2;
class CVector3;
class CVector4;

/// Warto�� przesuni�cia tekstur dla uzyskania efektu emboss bump mapping.
#define MAX_EMBOSS 0.0025f

/// Klasa tworzenia efektu t�oczonego mapowania wyboj�w.
/** Zadaniem tej klasy jest obliczenie warto�ci tangent i binormal dla przesuni�cia koordynat�w tekstur, utworzenie
tablicy nowych, przesuni�tych koordynat�w tekstur, wsparcie dla rysowania mapowania wyboj�w.*/
class CEmbossBump
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CEmbossBump();

	/// Destruktor.
	/** Destruktor. */
	~CEmbossBump();

	/// Metoda mno��ca macierz przez wektor.
	/** Metoda ta mno�y macierz widoku modelu przez wektor pozycji �wiat�a i ustawia now� warto�� wektoru w zmiennej
	CEmbossBump::m_cLight.
	\param[in] in_cLightPos Pozycja �wiat�a, dla kt�rego b�dziemy obliczali efekt mapowania wyboj�w.  */
	GLvoid VMatMult( const CVector4 &in_cLightPos );

	/// Metoda obliczaj�ca warto�ci tangent i binormal dla obiektu 3D bez indeksowania wierzcho�k�w.
	/** Motod� t� nale�y wywo�a� przed rysowaniem, np. w trakcie inicjalizacji programu.
	\param[in] iIndexObj Indeks obiektu 3D jaki chcemy narysowa�. Indeksy liczone s� od \p 0 w kolejno�ci
	wczytywania	obiekt�w, tj. w kolejno�ci wywo�ywania metody C3DObjManager::Create3DObject. 
	\param[in] iFirstVertex Indeks pierwszego wierzcho�ka danego obiektu z tablicy wierzcho�k�w CVertexArray. 
	\param[in] iCountVertices Ilo�� wierzcho�k�w przypadaj�ca na dany obiekt. 
	\return Indeks tablicy CEmbossBump::m_aEmbossBumpData przechowuj�cej obliczone warto�ci tangent i binormal. */
	GLint CalcTangents( GLint iIndexObj, GLint iFirstVertex, GLsizei iCountVertices );

	/// Metoda ustawiaj�ca koordynaty przesuni�cia tekstury dla obiektu 3D bez indeksowania wierzcho�k�w.
	/** Metod� t� nale�y wywo�a� za ka�dym razem gdy chcemy narysowa� obiekt z efektem bump mappingu.
	\param[in] iIndexObj Indeks obiektu 3D jaki chcemy narysowa�. Indeksy liczone s� od \p 0 w kolejno�ci
	wczytywania	obiekt�w, tj. w kolejno�ci wywo�ywania metody C3DObjManager::Create3DObject. 
	\param[in] iFirstVertex Indeks pierwszego wierzcho�ka danego obiektu z tablicy wierzcho�k�w CVertexArray. 
	\param[in] iCountVertices Ilo�� wierzcho�k�w przypadaj�ca na dany obiekt. */
	GLvoid SetUpBumps( GLint iIndexObj, GLint iFirstVertex, GLsizei iCountVertices );

	/// Metoda obliczaj�ca warto�ci tangent i binormal dla obiektu 3D z indeksowaniem wierzcho�k�w.
	/** Motod� t� nale�y wywo�a� przed rysowaniem, np. w trakcie inicjalizacji programu.
	\param[in] iIndexObj Indeks obiektu 3D jaki chcemy narysowa�. Indeksy liczone s� od \p 0 w kolejno�ci
	wczytywania	obiekt�w, tj. w kolejno�ci wywo�ywania metody C3DObjManager::Create3DObject. 
	\param[in] iCountVertices Ilo�� wierzcho�k�w przypadaj�ca na dany obiekt. 
	\return Indeks tablicy CEmbossBump::m_aEmbossBumpData przechowuj�cej obliczone warto�ci tangent i binormal. */
	GLint CalcTangentsElem( GLint iIndexObj, GLsizei iCountVertices );

	/// Metoda ustawiaj�ca koordynaty przesuni�cia tekstury dla obiektu 3D z indeksowaniem wierzcho�k�w.
	/** Metod� t� nale�y wywo�a� za ka�dym razem gdy chcemy narysowa� obiekt z efektem bump mappingu.
	\param[in] iIndexObj Indeks obiektu 3D jaki chcemy narysowa�. Indeksy liczone s� od \p 0 w kolejno�ci
	wczytywania	obiekt�w, tj. w kolejno�ci wywo�ywania metody C3DObjManager::Create3DObject. 
	\param[in] iCountVertices Ilo�� wierzcho�k�w przypadaj�ca na dany obiekt. */
	GLvoid SetUpBumpsElem( GLint iIndexObj, GLsizei iCountVertices );

	/// \name Zestaw metod do pobierania danych o koordynatach tekstury dla obiektu bez indeksowania wierzcho�k�w.
	//@{

	/// Metoda pobieraj�ca tablic� przesuni�tych koordynat�w tekstury.
	/** \return Wska�nik na tablic� CEmbossBump::m_aTexCoords. */
	inline GLvoid* GetTexCoordsArray() { return &m_aTexCoords[ 0 ]; }

	/// Metoda pobieraj�ca ilo�� przesuni�tych koordynat�w tekstury.
	/** \return Liczba ca�kowita okre�laj�ca wielko�� tablicy CEmbossBump::m_aTexCoords. */
	inline GLint GetTexCoordsSize() { return static_cast<GLint>( m_aTexCoords.size() ); }

	/// Metoda pobieraj�ca koordynaty przesuni�tej tekstury dla danego wierzcho�ka.
	/** \param[in] iIndex Indeks tabicy przesuni�tych koordynat�w testury, z kt�rego chcemy pobra� dane koordynaty.
	\return Obiekt klasy CVector2 zawieraj�cy koordynaty tekstury. */
	CVector2 GetTexCoord( GLint iIndex );
	///@}

	/// \name Zestaw metod do pobierania danych o koordynatach tekstury dla obiektu bez indeksowania wierzcho�k�w.
	//@{

	/// Metoda pobieraj�ca tablic� przesuni�tych koordynat�w tekstury.
	/** \return Wska�nik na tablic� CEmbossBump::m_aTexCoordsElem. */
	inline GLvoid* GetTexCoordsArrayElem() { return &m_aTexCoordsElem[ 0 ]; }

	/// Metoda pobieraj�ca ilo�� przesuni�tych koordynat�w tekstury.
	/** \return Liczba ca�kowita okre�laj�ca wielko�� tablicy CEmbossBump::m_aTexCoordsElem. */
	inline GLint GetTexCoordsElemSize() { return static_cast<GLint>( m_aTexCoordsElem.size() ); }

	/// Metoda pobieraj�ca koordynaty przesuni�tej tekstury dla danego wierzcho�ka.
	/** \param[in] iIndex Indeks tabicy przesuni�tych koordynat�w testury, z kt�rego chcemy pobra� dane koordynaty.
	\return Obiekt klasy CVector2 zawieraj�cy koordynaty tekstury. */
	CVector2 GetTexCoordElem( GLint iIndex );
	///@}
	
	/// Metoda czyszcz�ca wszelkie tablice.
	/** Metoda usuwa z pami�ci wszelkie obliczone tangentsy i binormale oraz koordynaty przesuni�cia tekstury. */
	GLvoid ClearAllArrays();

private:
	/// Struktura przechowuj�ca wyliczone warto�ci tangent i binormal dla danego obiektu 3D.
	/** Struktura ta przechowuje tablice wyliczonych warto�ci tangent i binormal dla ka�dego wierzcho�ka
	danego obiektu 3D. */
	struct SEmbossBumpData
	{
		GLint iIndexObj;					///< Indeks obiektu 3D wczytanego z pliku \a 3DObj.
		std::vector<CVector3> aTangent;		///< Tablica wyliczonych wektor�w tangent dla ka�dego wierzcho�ka obiektu.
		std::vector<CVector3> aBinormal;	///< Tablica wyliczonych wektor�w binormal dla ka�dego wierzcho�ka obiektu.
	};

	CVector4 m_cLight;			///< Pomocniczy wektor �wiat�a do obliczenia przesuni�cia koordynat�w tekstury.
	GLboolean m_bIsSetLight;	///< Flaga wskazuj�ca czy wywo�ano metod� CEmbossBump::VMatMult ustawiaj�c� wektor CEmbossBump::m_cLight.

	/// Tablica obiekt�w struktury CEmbossBump::SEmbossBumpData.
	std::vector<SEmbossBumpData> m_aEmbossBumpData;

	/// Metoda zwracaj�ca ilo�� element�w tablicy CEmbossBump::m_aEmbossBumpData.
	inline GLint GetEmbossBumpDataSize() { return static_cast<GLint>( m_aEmbossBumpData.size() ); }

	/// Tablica przesuni�tych koordynat�w tekstury bump dla rysowania glDrawArrays.
	std::vector<CVector2> m_aTexCoords;

	/// Tablica przesuni�tych koordynat�w tekstury bump dla rysowania glDrawElements.
	std::vector<CVector2> m_aTexCoordsElem;
};
