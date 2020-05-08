/*! \file
	\brief Plik nag��wkowy dla klasy CBitmapFont.
*/
#pragma once

/// Klasa umo�liwiaj�ca rysowanie tekstu czcionk� rastrow�.
class CBitmapFont  
{
public:
	/// Konstruktor.
	/** \param[in] hDC Kontekst urz�dzenia GDI (nale�y pobra� go z klasy CWindowData).
	\param[in] lpFontName Nazwa czcionki zarejestrowanej w systemie.
	\param[in] iHeight Wielko�� czcionki (nale�y podawa� w warto�ciach ujemnych).*/
	CBitmapFont( HDC hDC, LPCTSTR lpFontName, GLint iHeight = -24 );

	/// Destruktor.
	/** Destruktor. */
	virtual ~CBitmapFont();

	/// Metoda rysuj�ca tekst.
	/** \param[in] fX Wsp�r�dna x po�o�enia tekstu, gdzie 0, 0 to �rodek ekranu. Tekst jest rysowany na p�aszczy�nie
	ekranu.
	\param[in] fY Wsp�r�dna y po�o�enia tekstu, gdzie 0, 0 to �rodek ekranu. Tekst jest rysowany na p�aszczy�nie
	ekranu.
	\param[in] fmt Tekst jaki chcemy wy�wietli�.*/
	GLvoid DrawText( GLfloat fX, GLfloat fY, LPCTSTR fmt );

	/// Metoda rysuj�ca tekst z mo�liwo�ci� formatowania.
	/** \param[in] fX Wsp�r�dna x po�o�enia tekstu, gdzie 0, 0 to �rodek ekranu. Tekst jest rysowany na p�aszczy�nie
	ekranu.
	\param[in] fY Wsp�r�dna x po�o�enia tekstu, gdzie 0, 0 to �rodek ekranu. Tekst jest rysowany na p�aszczy�nie
	ekranu.
	\param[in] fmt Tekst jaki chcemy wy�wietli�.
	\param[in] ... Opcjonalane parametry dla formatowania. */
	GLvoid DrawTextFormat( GLfloat fX, GLfloat fY, LPCTSTR fmt, ... );

private:
	/// Metoda buduj�ca czcionk� rastrow�.
	/** \param[in] lpFontName Nazwa czcionki zarejestrowanej w systemie.
	\param[in] iHeight Wielko�� czcionki (nale�y podawa� w warto�ciach ujemnych).*/
	GLvoid BuildFont( LPCTSTR lpFontName, GLint iHeight = -24 );

	/// Metoda usuwaj�ca zbudowan� czcionk�.
	GLvoid KillFont();

	GLuint	m_uiBase;	///< Identyfikator list wy�wietlania czcionki.
	HDC m_hDC;			///< Kontekst urz�dzenia GDI.
};