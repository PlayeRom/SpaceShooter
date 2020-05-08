/*! \file
	\brief Plik nag��wkowy dla klasy CRMenuBase.
*/
#pragma once

/// Klasa bazowa dla klas GUI z przyciskami.
/** Klasa ta zawiera metody przeznaczone do odziedziczenia przez inne klasy, reprezentuj�ce graficzny interfejs
u�ytkownika. */

class CRMenuBase
{
public:
	/// Konstruktor
	/** Konstruktor. */
	CRMenuBase();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CRMenuBase();

protected:
	/// Metoda rysuj�ca obramowanie wok� zadanego prostok�ta po najechaniu na niego mysz�.
	/** Metod� t� mo�na wykorzysta�, np. obramowuj�c dany przycisk po najechaniu na niego kursorem myszy.
	\param[in] rcRect Referencja na struktur� \p RECT reprezentuj�ca po�o�enie, np. przycisku.
	\param[in] bCheckMessageBox Flaga m�wi�ca czy sprawdzane ma by�, czy rysowana jest plansza z MessageBox-em klasy
	CRMessageBox. Je�eli podamy \p GL_TRUE to obramowanie nie zostanie narysowane je�eli rysowana b�dzie plansza z
	MessageBox-em. Je�eli podamy \p GL_FALSE to obramowanie zostanie zawsze narysowane. */
	GLvoid DrawRegionIfMouse(const RECT &rcRect, GLboolean bCheckMessageBox = GL_TRUE);

	/// Metoda rysuj�ca obramowanie wok� zadanego prostok�ta.
	/** Metod� t� mo�na wykorzysta� do rysowania ramki.
	\param[in] rcRect Referencja na struktur� \p RECT reprezentuj�ca po�o�enie, np. ramki. */
	GLvoid DrawRegionBorder(const RECT &rcRect);

	/// Metoda sprawdzaj�ca czy klikni�to w obr�bie danego prostok�ta.
	/** \param[in] iX Wsp�rz�dna X po�o�enia kursora myszy.
	\param[in] iY Wsp�rz�dna Y po�o�enia kursora myszy. 
	\param[in] prcRect Wska�nik na tablic� prostok�t�w.
	\param[in] iMaxCount Ilo�� prostok�t�w podana w \a prRect.
	\param[out] out_iIndex Zmienna do kt�rej zapisany zostanie indeks prostok�ta, na kt�ry klikni�to. Pod warunkiem,
	�e metoda zwr�ci \p GL_TRUE.
	\return \p GL_TRUE w przypadku gdy kusor znajduje si� w kt�rym� z region�w. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean CheckWhereClickedUser(GLint iX, GLint iY, const RECT *prcRect, GLint iMaxCount, GLint &out_iIndex);

	/// Metoda rysuj�ca zadany prostok�t.
	/** Za pomoc� tej metody mo�emy rysowa�, np. przyciski.
	\param[in] prcRect Wska�nik na tablic� obiekt�w \p RECT, kt�re maj� by� narysowane.
	\param[in] iMaxCount Maksymala ilo�� element�w tablicy obiekt�w \p RECT. 
	\param[in] iStart Pocz�tkowy indeks tablicy obiekt�w \p RECT od kt�rego rozpocznie si� rysowanie.
	\param[in] bSetDrawParam Flaga wskazuj�ca czy przed rysowanie maj� zosta� wywo�ane odpowiednie funkcje ustawiaj�ce
	prametry rysowania prostok�t�w. */
	GLvoid DrawRegions(const RECT *prcRect, GLint iMaxCount, GLint iStart = 0, GLboolean bSetDrawParam = GL_TRUE);

	/// Metoda przeliczaj�ca procent na piksele po osi X.
	/** Za pomoc� tej metody mo�emy ustali�, np. pozycj� przycisku wzd�u� szeroko�ci ekranu podaj�c warto�� procentow�
	po�o�enia. Dzi�ki temu mo�emy uzyska� wizualne t� sam� pozycj� przycisku niezale�nie od rozdzielczo�ci ekranu.
	\param[out] out_lPixelsVal Referencja na warto�� typu \p LONG. Do tej zmiennej zostanie zapisana warto�� wynikowa,
	reprezentuj�ca ilo�� pikseli wyliczon� na podstawie podanej warto�ci procentowej oraz aktualnej rozdzielczo�ci
	ekranu na jakiej program pracuje.
	\param[in] iPercent Warto�� procentowa szeroko�ci ekranu, gdzie 0% to lewa a 100% prawa strona ekranu.
	\param[in] bFont Flaga wskazuj�ca czy obliczenia przeprowadzane s� dla ustalenia pozycji tekstu. Dotyczy
	tylko rysowania tekstu za pomoc� klasy CSDLFont. */
	GLvoid SetRegionX(LONG &out_lPixelsVal, GLint iPercent, GLboolean bFont = GL_FALSE);

	/// Metoda przeliczaj�ca procent na piksele po osi Y.
	/** Za pomoc� tej metody mo�emy ustali�, np. pozycj� przycisku wzd�u� wysoko�ci ekranu podaj�c warto�� procentow�
	po�o�enia. Dzi�ki temu mo�emy uzyska� wizualne t� sam� pozycj� przycisku niezale�nie od rozdzielczo�ci ekranu.
	\param[out] out_lPixelsVal Referencja na warto�� typu \p LONG. Do tej zmiennej zostanie zapisana warto�� wynikowa,
	reprezentuj�ca ilo�� pikseli wyliczon� na podstawie podanej warto�ci procentowej oraz aktualnej rozdzielczo�ci
	ekranu na jakiej program pracuje.
	\param[in] iPercent Warto�� procentowa wysoko�ci ekranu, gdzie 0% to g�rna a 100% dolna strona ekranu.
	\param[in] bFont Flaga wskazuj�ca czy obliczenia przeprowadzane s� dla ustalenia pozycji tekstu. Dotyczy
	tylko rysowania tekstu za pomoc� klasy CSDLFont. */
	GLvoid SetRegionY(LONG &out_lPixelsVal, GLint iPercent, GLboolean bFont = GL_FALSE);

	/// Metoda przeliczaj�ca procent na piksele po osi X z dok�adno�ci� liczby rzeczywistej.
	/** Za pomoc� tej metody mo�emy ustali�, np. pozycj� przycisku wzd�u� szeroko�ci ekranu podaj�c warto�� procentow�
	po�o�enia. Dzi�ki temu mo�emy uzyska� wizualne t� sam� pozycj� przycisku niezale�nie od rozdzielczo�ci ekranu.
	\param[out] out_lPixelsVal Referencja na warto�� typu \p LONG. Do tej zmiennej zostanie zapisana warto�� wynikowa,
	reprezentuj�ca ilo�� pikseli wyliczon� na podstawie podanej warto�ci procentowej oraz aktualnej rozdzielczo�ci
	ekranu na jakiej program pracuje.
	\param[in] fPercent Warto�� procentowa szeroko�ci ekranu, gdzie 0% to lewa a 100% prawa strona ekranu.
	\param[in] bFont Flaga wskazuj�ca czy obliczenia przeprowadzane s� dla ustalenia pozycji tekstu. Dotyczy
	tylko rysowania tekstu za pomoc� klasy CSDLFont. */
	GLvoid SetRegionX(LONG &out_lPixelsVal, GLfloat fPercent, GLboolean bFont = GL_FALSE);

	/// Metoda przeliczaj�ca procent na piksele po osi Y z dok�adno�ci� liczby rzeczywistej.
	/** Za pomoc� tej metody mo�emy ustali�, np. pozycj� przycisku wzd�u� wysoko�ci ekranu podaj�c warto�� procentow�
	po�o�enia. Dzi�ki temu mo�emy uzyska� wizualne t� sam� pozycj� przycisku niezale�nie od rozdzielczo�ci ekranu.
	\param[out] out_lPixelsVal Referencja na warto�� typu \p LONG. Do tej zmiennej zostanie zapisana warto�� wynikowa,
	reprezentuj�ca ilo�� pikseli wyliczon� na podstawie podanej warto�ci procentowej oraz aktualnej rozdzielczo�ci
	ekranu na jakiej program pracuje.
	\param[in] fPercent Warto�� procentowa wysoko�ci ekranu, gdzie 0% to g�rna a 100% dolna strona ekranu.
	\param[in] bFont Flaga wskazuj�ca czy obliczenia przeprowadzane s� dla ustalenia pozycji tekstu. Dotyczy
	tylko rysowania tekstu za pomoc� klasy CSDLFont. */
	GLvoid SetRegionY(LONG &out_lPixelsVal, GLfloat fPercent, GLboolean bFont = GL_FALSE);

	/// Metoda sprawdzaj�ca czy dany punkt zawiera si� w zadanym prostok�cie.
	/** \param[in] ptCursor Referencja na obiekt \p POINT, np. pozycja kursora myszy.
	\param[in] rcRect Referencja na obiekt \p RECT, np. przycisk. 
	\return \p GL_TRUE w przypadku gdy wsp�rz�dne punktu znajduj� si� wewn�trz prostok�ta. W przeciwnym wypadku
	\p GL_FALSE. */
	GLboolean CheckPtCursorInRect(const POINT &ptCursor, const RECT &rcRect);

	/// Zmienna zapami�tuj�ca aktualny indeks prostok�ta (np. przycisku), nad kt�rym znajduje si� kursor myszy.
	/** Zmienna ta wykorzystywana jest dla stwierdzenia faktu najechania kursora myszy na dany prostok�t. */
	GLint m_iWhereIsMouse;

	/// Zmienna zapami�tuj�ca ostatni indeks prostok�ta (np. przycisku), nad kt�rym znajduje si� kursor myszy.
	/** Zmienna ta wykorzystywana jest dla stwierdzenia faktu najechania kursora myszy na dany prostok�t. */
	GLint m_iWhereIsMouseOld;

	/// Tablica wierzcho�k�w przekazywana do glDrawArrays.
	GLint* m_aIndices;
};
