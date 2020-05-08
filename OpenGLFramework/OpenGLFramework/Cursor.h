/*! \file
	\brief Plik nag³ówkowy dla klasy CCursor.
*/
#pragma once

/// Klasa zarz¹dzaj¹ca kursorami.
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

	/// Metoda ustawiaj¹ca kursor.
	/** \param[in] iIndex Indeks kursora, który chcemy uaktywniæ. */
	GLvoid SetMyCursor( ECursor eIndex );

	/// Metoda zwracaj¹ca indeks aktualnie ustawionego kursora.
	/** \return Liczba ca³kowita zwracaj¹ca indeks aktualnie ustawionego ursora. */
	inline ECursor GetMyCursor()	{ return m_eCursor; }

private:
	HCURSOR m_hCursor[ ELastElem ];	///< Tablica przechowuj¹ca wczytane kursory.

	/// Zmienna pamiêtaj¹ca indeks ostatnio ustawionego kursora.
	ECursor m_eCursor;
};