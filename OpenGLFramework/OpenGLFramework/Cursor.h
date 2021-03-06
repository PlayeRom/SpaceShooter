/*! \file
	\brief Plik nagłówkowy dla klasy CCursor.
*/
#pragma once

/// Klasa zarządzająca kursorami.
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

	/// Metoda ustawiająca kursor.
	/** \param[in] iIndex Indeks kursora, który chcemy uaktywnić. */
	GLvoid SetMyCursor( ECursor eIndex );

	/// Metoda zwracająca indeks aktualnie ustawionego kursora.
	/** \return Liczba całkowita zwracająca indeks aktualnie ustawionego ursora. */
	inline ECursor GetMyCursor()	{ return m_eCursor; }

private:
	HCURSOR m_hCursor[ ELastElem ];	///< Tablica przechowująca wczytane kursory.

	/// Zmienna pamiętająca indeks ostatnio ustawionego kursora.
	ECursor m_eCursor;
};