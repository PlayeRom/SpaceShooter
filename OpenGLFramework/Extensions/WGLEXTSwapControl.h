/*! \file
	\brief Plik nag��wkowy dla klasy CWGLEXTSwapControl.
*/

#pragma once

#include "../OpenGLFramework/SingletonBase.h"

/// Klasa kontroluj�ca synchronizacj� renderowania klatek z od�wie�aniem ekranu monitora.
/** Za pomoc� tej klasy kontrolujemy synchronizacj� pionow� dla aplikacji. */

class CWGLEXTSwapControl : public CSingletonBase <CWGLEXTSwapControl>
{
public:
	/// Statyczna metoda tworz�ca obiekt klasy CWGLEXTSwapControl. 
	/** CWGLEXTSwapControl jest singletonem, wi�c je�eli wywo�amy t� metod� po raz drugi to zwr�ci ona wska�nik
	na obiekt ju� istniej�cy, utworzony wcze�niej.
	\return Wska�nik na obiekt CWGLEXTSwapControl. */
	static CWGLEXTSwapControl* Construct();

	/// Destruktor.
	/** Destruktor. */
	~CWGLEXTSwapControl();

	/// Metoda sprawdzaj�ca czy WGL_EXT_swap_control jest wspierany.
	/** \return \p GL_TRUE je�eli WGL_EXT_swap_control jest wspierany. W przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean IsSupported() { return m_bIsSupported; }

	/// Metoda w��czaj�ca / wy��czajaca synchronizacj� pionow� wedle uznania.
	/** \param[in] iInterval Nowa warto�� interwa�u. */
	GLvoid SetSwapInterval( GLint iInterval );

	/// Metoda ustawiaj�ca synchronizacj� pionow� na domy�ln�.
	GLvoid DefaultVSync();

	/// Metoda w��czaj�ca synchronizacj� pionow�.
	/** Metoda ustawia interwa� na \p 1. */
	GLvoid EnableVSync();

	/// Metoda wy��czaj�ca synchronizacj� pionow�.
	/** Metoda ustawia interwa� na \p 0. */
	GLvoid DisableVSync();

private:
	/// Konstruktor.
	/** Konstruktor. */
	CWGLEXTSwapControl();

	/// Metoda inicjalizuj�ca WGL_EXT_swap_control.
	/** \return \p GL_TRUE w przypadku powodzenia inicjalizaji tj. wsparcia dla WGL_EXT_swap_control. W przeciwnym
	wypadku	\p GL_FALSE.*/
	GLboolean InitSwapControl();

	GLboolean m_bIsSupported;			///< Flaga wskazuj�ca czy WGL_EXT_swap_control jest wspierany.
	GLint m_iDefaultSwapInterval;		///< Zmienna zapami�tuj�ca domy�ln� warto�� interwa�u.
};
