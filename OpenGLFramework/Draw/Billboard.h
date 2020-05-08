/*! \file
	\brief Plik nag��wkowy dla klasy CBillboard.
*/
#pragma once

/// Klasa rysuj�ca billboardy.
/** Klasa ta zawiera metod� za pomoc� kt�rej mo�emy narysowa� tzw. billboard czyli sprite, kt�ry zawsze zwr�cony
jest przodem do widza (kamery), nawet wtedy (a zw�aszcza wtedy) gdy kamera i/lub sprite zmienia swoje po�o�enie. */
class CBillboard
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CBillboard();

	/// Destruktor.
	/** Destruktor. */
	~CBillboard();

	/// Metoda rysuj�ca billboard.
	/** \param[in] in_cPos Centralny punkt po�o�enia sprite'a w przestrzeni 3D.
	\param[in] fScale Rozmiar sprite'a, standardowo \p 1.0f, podaj�c np. \p 2.0f sprite b�dzie dwa razy wi�kszy.
	\param[in] eMode Tryb rysowania sprite'a. Mo�emy tu poda� \p GL_TRIANGLE_STRIP (standardowo), w�wczas narysowany
	zostanie quad lub \p GL_LINE_LOOP, gdzie narysowany zostanie kwadratowy kontur z linii.
	\param[in] fTexCoordX1 Koordynaty tekstury \a x1.
	\param[in] fTexCoordX2 Koordynaty tekstury \a x2.
	\param[in] fTexCoordY1 Koordynaty tekstury \a y1.
	\param[in] fTexCoordY2 Koordynaty tekstury \a y2.*/
	GLvoid DrawQuadBillboard( const CVector3 &in_cPos,
							  GLfloat fScale = 1.0f,
							  GLenum eMode = GL_TRIANGLE_STRIP,
							  GLfloat fTexCoordX1 = 0.0f,
							  GLfloat fTexCoordX2 = 1.0f,
							  GLfloat fTexCoordY1 = 0.0f,
							  GLfloat fTexCoordY2 = 1.0f );

	CVector2*	m_pTexCoord;
	CVector3*	m_pVertices;
};
