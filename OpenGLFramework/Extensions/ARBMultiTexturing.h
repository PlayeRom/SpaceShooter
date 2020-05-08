/*! \file
	\brief Plik nag��wkowy dla klasy CARBMultiTexturing.
*/

#pragma once

#include "../OpenGLFramework/SingletonBase.h"

class CGameControl;

/// Klasa obs�uguj�ca multiteksturing.
/** Klasa ta daje wsparcie do obs�ugi rozszerzenia GL_ARB_multitexture oraz zarz�dza dost�pnymi jednostami
teksturuj�cymi. */
class CARBMultiTexturing : public CSingletonBase <CARBMultiTexturing>
{
public:
	/// Statyczna metoda tworz�ca obiekt klasy CARBMultiTexturing. 
	/*! CARBMultiTexturing jest singletonem, wi�c je�eli wywo�amy t� metod� po raz drugi to zwr�ci ona wska�nik na
	obiekt ju� istniej�cy, utworzony wcze�niej.
	\return Wska�nik na obiekt CARBMultiTexturing. */
	static CARBMultiTexturing* Construct();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CARBMultiTexturing();

	/// Metoda sprawdzaj�ca czy multiteksturing jest wspierany przez kart� graficzn�.
	/** \return \p GL_TRUE je�eli multiteksturing jest wspierany. W przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean IsSupported() { return m_bIsSupported; }

	/// Metoda ustawiaj�ce tekstury do renderingu.
	/** Metod� t� nale�y wywo�a� przed rysowaniem obiektu, na kt�ry chcemy nalo�y� kilka tekstur.
	Warunkiem jest aby tekstury te by�y wczytane (w CGameControl::LoadAllTextures) jedna po drugiej.
	\param[in] iFirstTex Indeks pierwszej tekstury. 
	\param[in] iCount Ilo�� kolejnych tekstur jaka ma by� aktywowana. Musi by� co najmniej 2.
	\param[in] bEmbossBump Flaga wskazuj�ca czy chcemy na�o�y� na obiekt tekstur� bump. Je�eli tak, to
	wymagane jest aby \a iCount by� r�wny \p 2.
	\return \p GL_TRUE w przypadku powodzenia aktywowania tekstur. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean BindMultiTextures( GLint iFirstTex, GLsizei iCount = 2, GLboolean bEmbossBump = GL_FALSE );

	/// Metoda aktywuj�ca dan� jednostk� teksturuj�c�.
	/** \param[in] iIndex Indeks jednostki teksturuj�cej, kt�r� chcemy aktywowa�.
	\return \p GL_TRUE w przypadku powodzenia aktywacji. W przeciwnym razie \p GL_FALSE. */
	GLboolean ClientActiveTextureARB( GLint iIndex );

	/// Metoda aktywuj�ca dan� jednostk� teksturuj�c�.
	/** \param[in] iIndex Indeks jednostki teksturuj�cej, kt�r� chcemy aktywowa�.
	\return \p GL_TRUE w przypadku powodzenia aktywacji. W przeciwnym razie \p GL_FALSE. */
	GLboolean ActiveTextureARB( GLint iIndex = 0 );

	/// Metoda dezaktywuj�ca dan� jednostk� teksturuj�c�.
	/** \param[in] iIndex Indeks jednostki teksturuj�cej, kt�r� chcemy dezaktywowa�.
	\return \p GL_TRUE w przypadku powodzenia dezaktywacji. W przeciwnym razie \p GL_FALSE. */
	GLboolean InactiveTextureARB( GLint iIndex = 0 );

	/// Metoda wy��czaj�ca multitekturing.
	/** Metoda ta dezaktywuje wszystkie jednostki teksturuj�ce poza pierwsz�. */
	GLvoid DisableMultiTeksturing();

	/// Metoda ustawiaj�ca recznie koordynaty tekstury dla danej jednostki teksturuj�cej.
	/** \param[in] iIndex Indeks jednostki teksturuj�cej dla kt�rej chcemy ustawi� koordydany.
	\param[in] s Koordynata tekstury s.
	\param[in] t Koordynata tekstury t.
	\return \p GL_TRUE w przypadku powodzenia ustawienia koordynat�w tekstury. \p GL_FALSE w przypadku niepowodzenia.*/
	GLboolean MultiTexCoord2fARB( GLint iIndex, GLfloat s, GLfloat t );

	/// Metoda aktywuj�ca tylko jedn� zadan� jednostk� teksturuj�c�.
	/** Metoda ta aktywuje tylko jedn� podan� jednostk� teksturuj�c� a wszytskie pozosta�e dezaktywuje.
	\param[in] iIndex Index jednostki teksturuj�cej jak� chcemy ukatywni�.
	\return \p GL_TRUE w przypadku powodzenia. W przeciwnym razie \p GL_FALSE.*/
	GLboolean ActiveOnlyOneUnit( GLint iIndex );

	/// Metoda zwracaj�ca ilo�� jednostek teksturuj�cych.
	/** \return Liczba ca�kowita reprezentuj�ca ilo�� jednostek teksturuj�cych jakimi dysponuje karta graficzna. */
	inline GLint GetMaxTexelUnits() { return m_iMaxTexUnits; }

private:
	/// Konstruktor.
	/** Konstruktor. */
	CARBMultiTexturing();

	/// Metoda inicjalizuj�ca multiteksturing.
	/** \return \p GL_TRUE w przypadku powodzenia inicjalizaji tj. wsparcia dla multiteksturingu. W przeciwnym wypadku
	\p GL_FALSE.*/
	GLboolean InitMultiTexturing();

	GLboolean m_bIsSupported;	///< Flaga wskazuj�ca czy multiteksturing jest wspierany.
	GLint m_iMaxTexUnits;		///< Zmienna przechowuj�ca liczb� ilo�� jednostek teksturuj�cych.
	
	/// Zmienna zpami�tuj�ca ilo�� podanych tekstur w metodzie CARBMultiTexturing::BindMultiTextures.
	GLint m_iCountUnit;
};