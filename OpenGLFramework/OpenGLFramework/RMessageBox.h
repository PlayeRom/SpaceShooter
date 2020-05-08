/*! \file
	\brief Plik nag��wkowy dla klasy CRMessageBox.
*/
#pragma once

#include "RMenuBase.h"

class CSDLFont;

/// Sta�a definiuj�ca maksymaln� d�ugo�� tektu pisanego w MessageBox-ie.
#define MSG_TEXT_LENGTH 512

/// Klasa rysuj�ca MessageBox-a.
/** Za pomoc� tej klasy mo�emy w danej chwili programu narysowa� plansz� z MessageBox-em czyli modalnym oknem
w celu powiadomienia u�ytkownika o jakiej� zaistania�ej sytuacji lub zapytania u�ytkownika o jak�� kwesti�. */

class CRMessageBox : public CRMenuBase
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CRMessageBox();

	/// Destruktor.
	/** Destruktor. */
	~CRMessageBox();

	/// Typ wyliczeniowy okre�laj�cy jakiego typu ma my� MessageBox.
	/** Konkretnie za pomoc� tego typu wyliczeniowego definiujemy jakie przyciski maj� si� pojawi� w Messagebox-ie. */
	enum EMsgType
	{
		EMsgOK,				///< MessageBox z przyciskiem OK, do zwyk�ego informowania.
		EMsgOKCancel,		///< MessageBox z przyciskami OK i Anuluj, do zatwierdzania i anulowania.
		EMsgYesNo,			///< MessageBox z przyciskami Tak i Nie, dla pytania rozstrzygaj�cego.
		EMsgYesNoCancel		///< MessageBox z przyciskami Tak, Nie oraz Anuluj, dla pytania z mo�liwo�ci� anulowania.
	};

	/// Metoda nakazuj�ca rysowania MessageBox-a.
	/** Wywo�anie tej metody w danym miejscu progamu spowoduje rysowanie planszy z MessageBox-em.
	\param[in] lpText Tekst (informacja, pytanie) jaki ma si� wy�wietli� w MessageBox-ie.
	\param[in] eType Typ wyliczeniowy CRMessageBox::EMsgType okre�laj�cy zestaw przycisk�w jakie maj� si� wy�wietli�.
	\param[in] in_lpFunc1 Wska�nik na funkcj� "zatwierdzaj�c�". Tutaj nale�y poda� wska�nik na funkcj�, kt�r� chcemy
	wywo�a�, jako reakcj� na zatwierdzenie MessageBox-a. Zatwierdzie tj. klikni�cie przycisku OK lub Tak. Je�eli
	nie chcemy wywo�yw�� �adnej funkcji zwrotnej nale�y poda� NULL.
	\param[in] in_lpFunc2 Wska�nik na funkcj� "odrzucaj�c�". Tutaj nale�y poda� wska�nik na funkcj�, kt�r� chcemy
	wywo�a�, jako reakcj� na odrzucenie MessageBox-a. Odrzucenie tj. klikni�cie przycisku Nie lub Anuluj. Je�eli
	nie chcemy wywo�yw�� �adnej funkcji zwrotnej nale�y poda� NULL. W przypadku trzech przycisk�w, tj. Tak, Nie,
	Anuluj dla Anuluj nie b�dzie wywo�ywana �adna akcja. */
	GLvoid DoMessageBox( LPCTSTR lpText, EMsgType eType, GLvoid( *in_lpFunc1 )( GLvoid ), GLvoid( *in_lpFunc2 )( GLvoid ) );

	/// Metoda rysuj�ca plansz� z MessageBox-em.
	/** Metoda ta rysuje MessageBox-a wedle parametr�w jakie podali�my w funkcji CRMessageBox::DoMessageBox.
	Metoda ta jest ju� wywo�ywana w odpowiednim miejscu, wewn�trz CGameControl::Draw i nie nale�y ju� jej powt�rnie
	wywo�ywa�. */
	GLvoid Draw();

	/// Metoda restartuj�ca ustawienie region�w reprezentuj�cych plansz� i przyciski MessageBox-a.
	/** Metoda ta jest ju� wywo�ywana w odpowiednim miejscu, wewn�trz CGameControl::RestartObjects. Metoda ta jest
	wywo�ywana gdy restartowane jest okno, np. pod wp�ywem zmiany rozdzielczo�ci, pr�bkowania dla antyaliasingu, itp. */
	GLvoid RestartObjects();

	/// Zdarzenie klikni�cia lewym przyciskiem myszy.
	/** Metoda ta jest ju� wywo�ywana w odpowiednim miejscu, wewn�trz CGameControl::MouseLButtonDown i nie nale�y
	ju� jej powt�rnie wywo�ywa�.
	\param[in] iX Wsp�rz�dna \a X po�o�enia kursora w momencie klikni�cia.
	\param[in] iY Wsp�rz�dna \a Y po�o�enia kursora w momencie klikni�cia.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta� obs�u�ony. */
	GLboolean MouseLButtonDown( GLint iX, GLint iY );

	/// Zdarzenie naci�ni�cia klawisza klawiatury.
	/** Metoda ta jest ju� wywo�ywana w odpowiednim miejscu, wewn�trz CGameControl::KeyDown i nie nale�y
	ju� jej powt�rnie wywo�ywa�.
	\param[in] uiKeyCode Wirtualny kod klawisza jaki zosta� naci�ni�ty.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta� obs�u�ony. */
	GLboolean KeyDown( GLuint uiKeyCode );

	/// Metoda sprawdzaj�ca czy MessageBox jest aktualnie rysowany.
	/** \return \p GL_TRUE gdy MessageBox jest rysowany. W przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean IsDraw() { return m_bDrawMsg; }

private:
	CSDLFont		*m_pSDLFontText;		///< Wska�nik na obiekt klasy CSDLFont.

	/// Wska�nik na funkcj� "zatwierdzaj�c�".
	/** Wska�nik na funkcj� jaka ma si� wywo�a� po zatwierdzeniu MessageBox-a przyciskami OK lub TAK. */
	void( *lpFunc1 )( void );

	/// Wska�nik na funkcj� "odrzucaj�c�".
	/** Wska�nik na funkcj� jaka ma si� wywo�a� po odrzuceniu MessageBox-a przyciskami NIE lub ANULUJ. */
	void( *lpFunc2 )( void );

	/// \name Regiony prostok�t�w pod plansze i przyciski.
	//@{

	/// Tablica prostok�t�w-plansz.
	/** Indeks 0 dla t�a pe�noekranowego, indeks 1 jako ramka pod sam MessageBox. */
	RECT m_rRegionsBkg[ 2 ];
	RECT m_rRegions1Button[ 3 ];
	RECT m_rRegions2Button[ 3 ];
	RECT m_rRegions3Button[ 3 ];
	//@}

	/// \name Regiony prostok�t�w dla tekst�w.
	//@{
	RECT m_rRegionsFont1Button[ 3 ];
	RECT m_rRegionsFont2Button[ 3 ];
	RECT m_rRegionsFont3Button[ 3 ];
	//@}

	/// Metoda ustawiaj�ca wsp�rz�dne plansz i przycisk�w.
	GLvoid SetRegions();

	/// Metoda rysuj�ca przyciski.
	/** \param[in] rc Trzyelementowa tablica obiekt�w \p RECT ze wsp�rz�dnymi przycisk�w do narysowania.
	\param[in] iSize Ilo�� przycisk�w do narysowania. */
	GLvoid DrawButtons( RECT rc[ 3 ], GLint iSize );

	/// Metoda rysuj�ca teksty.
	/** \param[in] rcRegions Trzyelementowa tablica obiekt�w \p RECT ze wsp�rz�dnymi przycisk�w.
	\param[in] rcRegionsFont Trzyelementowa tablica obiekt�w \p RECT ze wsp�rz�dnymi tekst�w pod przyciski.
	\param[in] iSize Ilo�� przycisk�w do narysowania.*/
	GLvoid DrawFonts( RECT rcRegions[ 3 ], RECT rcRegionsFont[ 3 ], GLint iSize );

	EMsgType m_eMsgType;				///< Zmienna pami�taj�ca typ MessageBox-a.
	TCHAR m_cText[ MSG_TEXT_LENGTH ];	///< Zmienna pami�taj�ca tekst rysowany na MessageBox-ie.
	GLboolean m_bDrawMsg;				///< Flaga wskazuj�ca czy MessageBox jest rysowany i czy moze si� rysowa�.

	GLint* m_aButtonIndices;
};