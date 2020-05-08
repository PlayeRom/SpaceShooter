/*! \file
	\brief Plik nag��wkowy dla klasy CTextureFont.
*/
#pragma once

class CTextureLoader;

/// Klasa umo�liwiaj�ca rysowanie tekstu za pomoc� tekstury z czcionk�.
class CTextureFont  
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CTextureFont();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CTextureFont();

	/// Metoda rysj�ca tekst z mo�liwo�ci� formatowania.
	/**	\param[in] fPosX Wsp�rz�dna x pozycji tekstu w przestrzeni 3D.
	\param[in] fPosY Wsp�rz�dna y pozycji tekstu w przestrzeni 3D.
	\param[in] iFont Rodzaj czcionki, do wyboru 0 i 1.
	\param[in] fR Sk�adowa R koloru tekstu.
	\param[in] fG Sk�adowa G koloru tekstu.
	\param[in] fB Sk�adowa B koloru tekstu.
	\param[in] fScale Wielko�� wy�wietlanej linijki tekstu.
	\param[in] lpFormat Tekst jaki chcemy wy�wietli�.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid DrawTextFormat( GLfloat fPosX, GLfloat fPosY,
						   GLint iFont,
						   GLfloat fR, GLfloat fG, GLfloat fB,
						   GLfloat fScale, LPCTSTR lpFormat, ... );

	/// Metoda rysj�ca tekst.
	/**	\param[in] fPosX Wsp�rz�dna x pozycji tekstu w przestrzeni 3D.
	\param[in] fPosY Wsp�rz�dna y pozycji tekstu w przestrzeni 3D.
	\param[in] lpText Tekst jaki chcemy wy�wietli�.
	\param[in] iFont Rodzaj czcionki, do wyboru 0 i 1.
	\param[in] fR Sk�adowa R koloru tekstu.
	\param[in] fG Sk�adowa G koloru tekstu.
	\param[in] fB Sk�adowa B koloru tekstu.
	\param[in] fScale Wielko�� wy�wietlanej linijki tekstu. */
	GLvoid DrawText( GLfloat fPosX, GLfloat fPosY,
					 LPCTSTR lpText, GLint iFont,
					 GLfloat fR = 1.0f, GLfloat fG = 1.0f, GLfloat fB = 1.0f,
					 GLfloat fScale = 0.00002f );

private:
	CTextureLoader *m_pTextureLoader;		///< Wska�nik na obiekt klasy  CTextureLoader.
	GLuint m_uiBase;						///< Identyfikator list wy�wietlania czcionki.

	/// Metoda buduj�ca czcionk� rastrow�.
	GLvoid BuildFont();
	GLvoid ScaleFont( GLfloat fScale );

	CVector2* m_aTextCoord;
	CVector2* m_aIndices;
};