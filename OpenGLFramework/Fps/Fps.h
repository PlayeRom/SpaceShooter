/*! \file
	\brief Plik nag��wkowy dla klasy CFps.
*/
#pragma once

class CSDLFont;

#include "../Fonts/SDLFont.h"

/// Klasa zliczaj�ca i wy�wietlaj�ca ilo�� generowanych klatek na sekund�.
class CFps  
{
public:
	/// Konstruktor.
	/*! Konstruktor. */
	CFps();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CFps();

	/// Metoda ustawiaj�ca tekst z ilo�ci� FPS jaki zostanie wy�wietlony
	/*! Metoda ta jest ju� wywo�ywana w metodzie CGameControl::Timer. */
	inline GLvoid SetTextFps() { wsprintf( m_cFpsText, _T("fps: %d"), m_uiFps ); }

	/// Metoda zeruj�ca licznik FPS.
	/*! Metoda ta jest ju� wywo�ywana w metodzie CGameControl::Timer. */
	inline GLvoid CleanFps() { m_uiFps = 0; }

	/// Metoda inkrementuj�ca licznik FPS.
	/*! Metoda ta jest ju� wywo�ywana co klatk� podczas renderowania sceny.
	\sa m_uiFps. */
	inline GLvoid IncreaseFps()	{ ++m_uiFps; }

	/// Metoda zwracaj�ca aktualn� warto�� licznika FPS.
	/*! \return Aktualna ilo�� klatek na sekund�.
	\sa m_uiFps. */
	inline GLuint GetFps()		{ return m_uiFps; }

	/// Metoda wy�wietlaj�ca napis z aktualnym stanem licznika FPS.
	/*! Aktualny stan licznika wy�wietli si� tylko wtedy je�eli flaga CFps::m_bShowFps b�dzie ustawiona na
	\p GL_TRUE.
	\sa m_uiFps. */
	inline GLvoid DrawFps() {
		if( !m_bShowFps )
			return;
		m_pSDLFont->DrawText( 10, 768, RGB( 255, 255, 255 ), m_cFpsText );
	}

	/// Metoda uaktywniaj�ca wy�wietlanie aktualnego stanu licznika FPS.
	/*! Metoda ustawia flag� CFps::m_bShowFps na \p GL_TRUE.
	\sa m_uiFps. */
	inline GLvoid ShowFps()		{ m_bShowFps = GL_TRUE; }

	/// Metoda dezaktywuj�ca wy�wietlanie aktualnego stanu licznika FPS.
	/*! Metoda ustawia flag� CFps::m_bShowFps na \p GL_FALSE.
	\sa m_uiFps. */
	inline GLvoid HideFps()		{ m_bShowFps = GL_FALSE; }

	/// Metoda sprawdzaj�ca czy licznik FPS jest wy�wietlany.
	/*! \return \p GL_TRUE je�eli licznik FPS jest wy�wietlany. W przeciwnym wypadku \p GL_FALSE.
	\sa m_uiFps. */
	inline GLboolean IsFps()	{ return m_bShowFps; }

private:
	CSDLFont	*m_pSDLFont;		///< Wska�nik na obiekt klasy CSDLFont, za pomoc� kt�rego rysowny jest tekst z aktualnym stanem licznika FPS.
	GLuint		m_uiFps;			///< Licznik FPS.
	GLboolean	m_bShowFps;			///< Flaga wskazuj�ca czy aktualny stan licznika FPS ma my� wy�wietlany.

	TCHAR		m_cFpsText[ 20 ];	///< Wy�wietlany napis z aktualnym stanem licznika FPS.
};