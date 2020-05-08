/*! \file
	\brief Plik nag��wkowy dla klasy C3DObjFile.
*/
#pragma once

#ifdef _3DSLOADER_
#include "3DSLoader.h"
class CVector3;
#endif

class CGameControl;

/// Nag�owek pliku \p 3DObj jako identyfikator pliku.
#define _3DOBJFILE_HEADER "OGLF PlayeRom"

/// G�owna wersja pliku \p 3DObj.
#define _3DOBJFILEVERSION_MAJOR 1

/// Pomniejsza wersja pliku \p 3DObj.
#define _3DOBJFILEVERSION_MINOR 0


#define INT_TO_FIXED( x )	GLint( x << 16 )
#define FIXED_TO_INT( x )	GLint( x >> 16 )

#define FLOAT_TO_FIXED( x )	GLint( 65536.0f * x )
#define FIXED_TO_FLOAT( x )	( GLfloat( x ) * ( 1.0f / 65536.0f ) )

#define FIXED_ONE			INT_TO_FIXED( 1 )


/// Klasa zapisu i odczytu pliku \a 3DObj.
/** Za pomoc� tej klasy C3DObjManager wykonuje operacje wczytania i zapisu plik�w \a 3DObj, reprezentuj�cych
tr�jwymiarowe obiekty. */
class C3DObjFile
{
public:
	/// Konstruktor.
	/** \param[in] lpFileName Nazwa pliku \a 3DObj wraz ze �cie�k�. Je�eli podamy nazw� pliku, to plik ten od razu
	zostanie wczytany. Mo�emy tak�e poda� tutaj NULL, w�wczas zostanie tylko	stworzony obiekt klasy. */
	C3DObjFile( LPCTSTR lpFileName = NULL );

	/// Destruktor.
	/** Destruktor. */
	~C3DObjFile();

#ifdef _3DSLOADER_
	GLboolean SaveObject( LPCTSTR lpFileName, const S3DSObject &in_s3DSObject, const CVector3 &in_cScale,
						  const CVector3 &in_cOffsetVector, const CVector3 &in_cRot90, GLboolean bSmoothNormal );
	GLboolean SaveObjectElem( GLint iIndexObject, LPCTSTR lpFileName, GLboolean bSmoothNormal, GLboolean bFixed = GL_FALSE );
#endif

	/// Metoda wczytuj�ca obiekt 3D z pliku \a 3DObj.
	/** \param[in] lpFileName Nazwa pliku \a 3DObj wraz ze �cie�k�, kt�ry chcemy wczyta�.
	\param[out] out_bIsElem Do zmiennej tej zostanie zapisana warto�� \p GL_TRUE je�eli wczytywany obiekt korzysta
	z indeksowania wierzcho�k�w. Je�eli nie, to zmienna ta zostanie ustawiona na \p GL_FALSE.
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku \a 3DObj. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean LoadObject( LPCTSTR lpFileName, GLboolean &out_bIsElem );

	/// Metoda zwracaj�ca ilo�� wierzcho�k�w ostatnio wczytanego pliku.
	/** \return Liczba ca�kowita okre�laj�ca ilo�� wierzcho�k�w jak� zawiera obiekt z ostatnio wczytanego pliku	\a 3DObj.
	\sa LoadObject, m_iVerticesNum. */
	inline GLint GetVerticesNum() { return m_iVerticesNum; }

private:
	CGameControl	*m_pGameCtrl;	///< Wska�nik na obiekt klasy CGameControl.
	GLint m_iVerticesNum;	///< Zmienna przechowuj�ca ilo�� wierzcho�k�w jak� zawiera obiekt z ostatnio wczytanego pliku.

#ifdef _3DSLOADER_
	/// Metoda zapisuj�ca nag��wek pliku \a 3DObj.
	/** \param[in] hFile Wska�nik na uchwyt do wcze�niej otwartego pliku \a 3DObj. Nie mo�e by� NULL.
	\param[in] bIsElem Flaga wskazuj�ca czy zapisywany obiekt 3D korzysta z indeksowania wierzcho�k�w.
	\return \p GL_TRUE w przypadku powodzenia zapisu nag��wka. W przeciwnym wypadku \p GL_FALSE.*/
	GLboolean SaveHeader( FILE *hFile, GLboolean bIsElem );
#endif

	/// Metoda wczytuj�ca nag��wek pliku \a 3DObj.
	/** \param[in] hFile Wska�nik na uchwyt do wcze�niej otwartego pliku \a 3DObj. Nie mo�e by� NULL.
	\param[in] lpFileName Nazwa pliku \a 3DObj wraz ze �cie�k�, z kt�rego chcemy wczyta� nag��wek.
	\param[out] out_bIsElem Do zmiennej tej zostanie zapisana warto�� \p GL_TRUE je�eli wczytywany obiekt korzysta
	z indeksowania wierzcho�k�w. Je�eli nie, to zmienna ta zostanie ustawiona na \p GL_FALSE.
	\return \p GL_TRUE w przypadku powodzenia wczytania nag��wka. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean LoadHeader( FILE *hFile, LPCTSTR lpFileName, GLboolean &out_bIsElem );

	/// Metoda wczytuj�ca dane z pliku \a 3DObj dla obiektu 3D bez indeksowania wierzcho�k�w.
	/** \param[in] hFile Wska�nik na uchwyt do wcze�niej otwartego pliku \a 3DObj. */
	GLboolean LoadDataNoElem( FILE *hFile );

	/// Metoda wczytuj�ca dane z pliku \a 3DObj dla obiektu 3D z indeksowaniem wierzcho�k�w.
	/** \param[in] hFile Wska�nik na uchwyt do wcze�niej otwartego pliku \a 3DObj. */
	GLboolean LoadDataElem( FILE *hFile );
};
