/*! \file
	\brief Plik nag��wkowy dla klasy COutlineFont.
*/
#pragma once

/// Klasa umo�liwiaj�ca rysowanie tekstu czcionk� konturow�.
class COutlineFont  
{
public:
	/// Konstruktor.
	/** \param[in] hDC Kontekst urz�dzenia GDI (nale�y pobra� go z klasy CWindowData).
	\param[in] lpFontName Nazwa czcionki zarejestrowanej w systemie. */
	COutlineFont( HDC hDC, LPCTSTR lpFontName );

	/// Destruktor.
	/** Destruktor. */
	virtual ~COutlineFont();

	/// Metoda rysuj�ca tekst.
	/** \param[in] fmt Tekst jaki chcemy wy�wietli�.*/
	GLvoid DrawText( LPCTSTR fmt );

	/// Metoda rysuj�ca tekst z mo�liwo�ci� formatowania.
	/** \param[in] fmt Tekst jaki chcemy wy�wietli�.
	\param[in] ... Opcjonalane parametry dla formatowania. */
	GLvoid DrawTextFormat( LPCTSTR fmt, ... );

private:
	/// Metoda buduj�ca czcionk� rastrow�.
	/** \param[in] lpFontName Nazwa czcionki zarejestrowanej w systemie. */
	GLvoid BuildFont( LPCTSTR lpFontName );

	/// Metoda usuwaj�ca zbudowan� czcionk�.
	GLvoid KillFont();

	GLuint	m_uiBase;				///< Identyfikator list wy�wietlania czcionki.
	HDC m_hDC;						///< Kontekst urz�dzenia GDI.

	/// Tablica przechowuj�ca informacje na temat znak�w konturowych.
	GLYPHMETRICSFLOAT m_sGlyphMetric[ 256 ];

};