/*! \file
	\brief Plik nag��wkowy dla klasy CCursor.
*/
#pragma once

/// Klasa zarz�dzaj�ca kursorami.
class CCursor  
{
public:
	enum ECursor
	{
		ENone = 0,
		EMenu,
		EGame,
		ERotateObj,
		ELastElem
	};

	/// Konstruktor.
	/** Konstruktor. */
	CCursor();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CCursor();

	/// Metoda ustawiaj�ca kursor.
	/** \param[in] iIndex Indeks kursora, kt�ry chcemy uaktywni�. */
	GLvoid SetMyCursor( ECursor eIndex );

	/// Metoda zwracaj�ca indeks aktualnie ustawionego kursora.
	/** \return Liczba ca�kowita zwracaj�ca indeks aktualnie ustawionego ursora. */
	inline ECursor GetMyCursor()	{ return m_eCursor; }

private:
	HCURSOR m_hCursor[ ELastElem ];	///< Tablica przechowuj�ca wczytane kursory.

	/// Zmienna pami�taj�ca indeks ostatnio ustawionego kursora.
	ECursor m_eCursor;
};