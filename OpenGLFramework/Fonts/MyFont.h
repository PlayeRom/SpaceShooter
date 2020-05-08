/*! \file
	\brief Plik nag��wkowy dla klasy CMyFont.
*/
#pragma once

/// Klasa rysuj�ca tekst.
class CMyFont  
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CMyFont();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CMyFont();

	/// Metoda rysuj�ca tekst.
	/** \param[in] lpText Tekst jaki chcemy wy�wietli�.
	\param[in] fX Wsp�rz�dna x pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] fY Wsp�rz�dna y pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] fZ Wsp�rz�dna z pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] crColor Sk�adowe koloru tekstu. Podajemy u�ywaj�c makra \p RGB.
	\param[in] fSize Grubo�ci tekstu (linii rysuj�cej tekst).
	\param[in] fScale Rozmiar tekstu. */
	GLvoid DrawText(LPCTSTR lpText, GLfloat fX, GLfloat fY, GLfloat fZ, COLORREF crColor = RGB(255, 255, 255), GLfloat fSize = 1.0f, GLfloat fScale = 0.1f);

	/// Metoda rysuj�ca tekst.
	/** \param[in] fX Wsp�rz�dna x pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] fY Wsp�rz�dna y pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] fZ Wsp�rz�dna z pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] crColor Sk�adowe koloru tekstu. Podajemy u�ywaj�c makra \p RGB.
	\param[in] fSize Grubo�ci tekstu (linii rysuj�cej tekst).
	\param[in] fScale Rozmiar tekstu.
	\param[in] fmt Tekst jaki chcemy wy�wietli�.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid DrawTextFormat(GLfloat fX, GLfloat fY, GLfloat fZ, COLORREF crColor, GLfloat fSize, GLfloat fScale, LPCTSTR fmt, ...);

private:
	/// Metoda rysuj�ca znak.
	/** \param[in] iChar Znak do narysowania.
	\param[in] fX Wsp�rz�dna x pozycji tekstu.
	\param[in] fY Wsp�rz�dna y pozycji tekstu.
	\param[in] fZ Wsp�rz�dna z pozycji tekstu.
	\param[in] fScale Rozmiar tekstu. */
	GLvoid DrawCharacter(GLint iChar, GLfloat fX, GLfloat fY, GLfloat fZ, GLfloat fScale);
};