/*! \file
	\brief Plik nag��wkowy dla klasy CFog.
*/
#pragma once

/// Klasa zarz�dzaj�ca ustawieniami mg�y.
/** Klasa zawiera metody i dane niezb�dne do ustawienia wszelkich parametr�w mg�y. */
class CFog
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CFog();

	/// Destruktor.
	/** Destruktor. */
	~CFog();

	/// Metoda ustawiaj�ca kolor mg�y.
	/** \param[in] r Ska�adowa koloru Red.
	\param[in] g Ska�adowa koloru Green.
	\param[in] b Ska�adowa koloru Blue.
	\param[in] a Ska�adowa koloru Alpha. */
	inline GLvoid SetColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f ) {
		m_aColor[ 0 ] = r;
		m_aColor[ 1 ] = g;
		m_aColor[ 2 ] = b;
		m_aColor[ 3 ] = a;
		glFogfv( GL_FOG_COLOR, m_aColor );
	}

	/// Metoda ustawiaj�ca kolor mg�y.
	/** \param[in] aColour Tablica koloru RGBA. */
	inline GLvoid SetColor( GLfloat aColour[ 4 ] ) {
		m_aColor[ 0 ] = aColour[ 0 ];
		m_aColor[ 1 ] = aColour[ 1 ];
		m_aColor[ 2 ] = aColour[ 2 ];
		m_aColor[ 3 ] = aColour[ 3 ];
		glFogfv( GL_FOG_COLOR, m_aColor );
	}

	/// Metoda ustawiaj�ca tryb mg�y.
	/** \param[in] fParam Parametr trybu mg�y. Mo�e przyj�� \p GL_LINEAR, \p GL_EXP lub \p GL_EXP2. */
	inline GLvoid SetFogMode( GLfloat fParam = GL_EXP ) {
		glFogf( GL_FOG_MODE, fParam );
	}

	/// Metoda ustawiaj�ca pocz�tek mg�y.
	/** \param[in] fStart Warto�� pocz�tku mg�y - jak blisko ekranu powinna zacz�� si� mg�a. */
	inline GLvoid SetStart( GLfloat fStart ) {
		m_fStart = fStart;
		glFogf( GL_FOG_START, m_fStart );
	}

	/// Metoda ustawiaj�ca koniec mg�y.
	/** \param[in] fEnd Warto�� ko�ca mg�y - jak daleko od ekranu powinna sko�czy� si� mg�a. */
	inline GLvoid SetEnd( GLfloat fEnd ) {
		m_fEnd = fEnd;
		glFogf( GL_FOG_END, m_fEnd );
	}

	/// Metoda ustawiaj�ca pocz�tek i koniec mg�y.
	/** \param[in] fStart Warto�� pocz�tku mg�y - jak blisko ekranu powinna zacz�� si� mg�a.
	\param[in] fEnd Warto�� ko�ca mg�y - jak daleko od ekranu powinna sko�czy� si� mg�a. */
	inline GLvoid SetStartEnd( GLfloat fStart, GLfloat fEnd ) {
		m_fStart = fStart;
		m_fEnd = fEnd;
		glFogf( GL_FOG_START, m_fStart );
		glFogf( GL_FOG_END, m_fEnd );
	}

	/// Metoda ustawiaj�ca g�sto�� mg�y
	/** \param[in] fDensity Warto�� g�sto�ci mg�y. */
	inline GLvoid SetDensity( GLfloat fDensity = 1.0f ) {
		m_fDensity = fDensity;
		glFogf( GL_FOG_DENSITY, m_fDensity );
	}

	/// Metoda ustawiaj�ca spos�b tworzenia mg�y.
	/** \param[in] fMode Spos�b tworzenia mg�y, mo�na poda�: \p GL_DONT_CARE lub \p GL_FASTEST dla wierzcho�kowego
	tworzenia mg�y lub \p GL_NICEST dla tworzenia �adniejszej mg�y pikselowej. */
	inline GLvoid SetHint( GLenum fMode = GL_DONT_CARE ) {
		glHint( GL_FOG_HINT, fMode );
	}

private:
	GLfloat m_aColor[ 4 ];	///< Tablica aktualnie ustawionego koloru mg�y.
	GLfloat m_fStart;		///< Warto�� aktualnie ustawionego pocz�tku mg�y.
	GLfloat m_fEnd;			///< Warto�� aktualnie ustawionego ko�ca mg�y.
	GLfloat m_fDensity;		///< Warto�� aktualnej g�sto�ci mg�y
};
