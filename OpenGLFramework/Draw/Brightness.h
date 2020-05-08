/*! \file
	\brief Plik nag��wkowy dla klasy CBrightness.
*/
#pragma once

#include "../OpenGLFramework/SingletonBase.h"

/// Klasa kontroli jasno�ci ekranu.
/** Zadaniem tej klasy jest rysowanie prostok�ta na ca�ym ekranie z odpowienio ustawion� przezroczysto�ci�
oraz kolorem dla uzyskania efektu rozja�nienia lub przyciemnienie ekranu. Obiekt tej klasy jest ju� tworzony w
CGameControl oraz �rysowanie jasno�ci� tak�e jest ju� wywo�ywane w CGameControl::Draw. */
class CBrightness : public CSingletonBase <CBrightness>
{
public:
	/// Statyczna metoda tworz�ca obiekt klasy CBrightness. 
	/*! CBrightness jest singletonem, wi�c je�eli wywo�amy t� metod� po raz drugi to zwr�ci ona wska�nik na obiekt
	ju� istniej�cy, utworzony wcze�niej.
	\return Wska�nik na obiekt CBrightness. */
	static CBrightness* Construct();

	/// Destruktor.
	/** Destruktor. */
	~CBrightness();

	/// Metoda ustawiaj�ca warto�� jasno�ci ekranu.
	/** \param[in] iBrightness Warto�� jasno�ci od \p -255 do \p 255, gdzie \p -255 to ca�kowite przyciemnienie
	(obraz b�dzie czarny) a \p 255 to ca�kowite rozja�nienie (obraz b�dzie bia�y). Podaj�c \p 0 (warto�� domy�lna)
	obraz b�dzie rysowany w oryginalnej jasno�ci. */
	GLvoid SetBrightness( GLint iBrightness = 0 );

	/// Metoda zwracaj�ca aktualnie ustawion� warto�� jasno�ci ekranu.
	/** \return Liczba ca�kowita z przedzia�u od \p -255 do \p 255 okre�laj�ca aktualn� warto�� jasno�ci ekranu. */
	inline GLint GetBrightness() { return m_iBrightness; }

	/// Metoda rysuj�ca "prostok�t jasno�ci".
	/** Metoda ta jest ju� wywo�ywana w CGameControl::Draw. */
	GLvoid Draw();

private:
	/// Konstruktor.
	/** Konstruktor. */
	CBrightness();

	/// Zmienna przechowuj�ca aktualn� warto�� jasno�ci.
	/** Warto�� jasno�ci od \p -255 do \p 255, gdzie \p -255 to ca�kowite przyciemnienie (obraz b�dzie czarny) 
	a \p 255 to ca�kowite rozja�nienie (obraz b�dzie bia�y). W zmiennej ten ustotna jest mo�liwo�� usawienia warto�ci
	ujemnej, kt�ra wskazuje na przyciemnienie obrazu. */
	GLint m_iBrightness;

	/// Zmienna przechowuj�ca aktualn� warto�� przezroczysto�ci dla rysowania.
	/** Warto�� przeroczysto�ci od \p 0 do \p 255, gdzie \p 0 to ca�kowita przezroczysto�� (jasno�� bez z mian) 
	a \p 255 to	ca�kowite rozja�nienie lub przyciemnienie. */
	GLubyte m_ubBrightness;
};
