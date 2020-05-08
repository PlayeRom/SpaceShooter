/*! \file
	\brief Plik nag��wkowy dla klasy CParticles.
*/
#pragma once

class CVector3;
class CGameControl;

/// Klasa cz�steczek.
/** Klasa ta zarz�dza tworzeniem i rysowaniem emiter�w cz�steczek. */
class CParticles
{
private:
	/// Struktura cz�steczki.
	/** Struktura ta przechowuje wszelkie informacje opisuj�ce pojedyncz� cz�steczk�. */
	struct SParticle
	{
		CVector3 cActualPos;	///< Aktuale po�o�enie cz�steczki w przestrzeni 3D.
		CVector3 cSpeed;		///< Szybko�� poruszania si� czosteczki

		GLfloat fLife;			///< Warto�� czasu �ycia cz�steczki, gdy = 0.0 to martwa, maksywmalnie 1.0.
		GLfloat fFade;			///< Warto�� szybko�ci wymierania cz�steczki.

		GLfloat	fScale;			///< Rozmiar cz�steczki.

		GLfloat fColorR;		///< Sk�adowa koloru Red cz�steczki.
		GLfloat fColorG;		///< Sk�adowa koloru Green cz�steczki.
		GLfloat fColorB;		///< Sk�adowa koloru Blue cz�steczki.
	};

	/// Struktura emitera cz�steczek.
	/** Struktura ta przechowuje wszelkie informacje opisuj�ce pojedynczy emiter cz�steczek. */
	struct SEmitter
	{
		CVector3 cStartPos;		///< Po�o�enie emitera w przestrzeni 3D. Jest to tak�e pocz�tkowe po�o�enie ka�dej cz�steczki emitera.
		GLuint uiTexture;		///< Indeks tekstury obrazuj�cej ka�d� cz�steczk�.

		CVector3 cDirection;	///< Wektor kierunku w jakim maj� przesuwa� si� cz�steczki.
		CVector3 cDivergence;	///< Wektor odchylenia od kierunku poruszania si� cz�steczek.

		GLfloat fMaxLife;		///< Maksymalna warto�� czasu �ycia ka�dej cz�steczki.
		GLboolean bIsAlive;		///< Flaga wskazuj�ca czy jak dana cz�steczka wyga�nie to czy mo�e o�y�, \p GL_TRUE - tak, \p GL_FALSE - nie.

		GLfloat	fScale;			///< Rozmiar cz�steczek.

		GLfloat fColorR;		///< Sk�adowa koloru Red cz�steczek.
		GLfloat fColorG;		///< Sk�adowa koloru Green cz�steczek.
		GLfloat fColorB;		///< Sk�adowa koloru Blue cz�steczek.

		GLfloat fSpeedX;		///< Szybko�� przemieszczania si� cz�steczek po osi X. Obowi�zuje tylko gdy \a bIsAlive \p = \p GL_TRUE.
		GLfloat fSpeedY;		///< Szybko�� przemieszczania si� cz�steczek po osi Y. Obowi�zuje tylko gdy \a bIsAlive \p = \p GL_TRUE.
		GLfloat fSpeedZ;		///< Szybko�� przemieszczania si� cz�steczek po osi Z. Obowi�zuje tylko gdy \a bIsAlive \p = \p GL_TRUE.

		GLint iMaxParticles;	///< Maksymalna ilo�� cz�steczek wydawanych z emitera.

		GLfloat fMoveSpeed;		///< Maksymalna warto�� szybko�ci przemieszczania si� cz�steczek.

		GLint iRandSpeedRange1;	///< Pierwsza warto�� zakresu losowo�ci przemieszczania si� cz�steczek.
		GLint iRandSpeedRange2;	///< Druga warto�� zakresu losowo�ci przemieszczania si� cz�steczek.

		std::vector<SParticle> aParticles;	///< Tablica cz�steczek.
	};

public:
	/// Konstruktor.
	/** Konstruktor. */
	CParticles();

	/// Destruktor.
	/** Destruktor. */
	~CParticles();

	/// Metoda tworz�ca nowy emiter.
	/** \param[in] in_sEmitter Referencja na struktur� CParticles::SEmitter, z kt�rego stworzony zostanie nowy emiter.
	\return Indeks nowego emitera. */
	GLint CreateEmitter( const SEmitter &in_sEmitter );

	/// Metoda tworz�ca nowy emiter.
	/** \param[in] in_cStartPos Po�o�enie emitera w przestrzeni 3D. Jest to tak�e pocz�tkowe po�o�enie ka�dej cz�steczki emitera.
	\param[in] uiTexture Indeks tekstury obrazuj�cej ka�d� cz�steczk�.
	\param[in] in_cDirection Wektor kierunku w jakim maj� przesuwa� si� cz�steczki. Aktualnie nieu�ywane.
	\param[in] in_cDivergence Wektor odchylenia od kierunku poruszania si� cz�steczek. Aktualnie nieu�ywane.
	\param[in] fScale Rozmiar cz�steczek. Zalecany rozmiar to \p 1.0f.
	\param[in] fLife Maksymalna warto�� czasu �ycia ka�dej cz�steczki. Zalecana warto�� to \p 1.0f.
	\param[in] bAlive Czy cz��teczki mog� o�ywa� po wyga�ni�ciu.
	\param[in] fColorR Sk�adowa koloru Red cz�steczek.
	\param[in] fColorG Sk�adowa koloru Green cz�steczek.
	\param[in] fColorB Sk�adowa koloru Blue cz�steczek.
	\param[in] fSpeedX Szybko�� przemieszczania si� cz�steczek po osi X.
	\param[in] fSpeedY Szybko�� przemieszczania si� cz�steczek po osi Y.
	\param[in] fSpeedZ Szybko�� przemieszczania si� cz�steczek po osi Z.
	\param[in] iMaxParticles Maksymalna ilo�� cz�steczek wydawanych z emitera.
	\param[in] fMoveSpeed Maksymalna warto�� szybko�ci przemieszczania si� cz�steczek.
	\param[in] iRandSpeedRange1 Pierwsza warto�� zakresu losowo�ci przemieszczania si� cz�steczek.
	\return Indeks nowego emitera. */
	GLint CreateEmitter( const CVector3 &in_cStartPos,
						 GLuint uiTexture,
						 const CVector3 &in_cDirection,
						 const CVector3 &in_cDivergence,
						 GLfloat fScale, // 1.0f
						 GLfloat fLife, // 1.0f
						 GLboolean bAlive,
						 GLfloat fColorR,
						 GLfloat fColorG,
						 GLfloat fColorB,
						 GLfloat fSpeedX, // 0.0f
						 GLfloat fSpeedY, // 0.0f
						 GLfloat fSpeedZ, // 0.0f
						 GLint iMaxParticles,
						 GLfloat fMoveSpeed = 60.0f, //szybkosc poruszania sie czasteczek
						 GLint iRandSpeedRange1 = 60 //dla okreslenia rozprysku czaseteczki emitera
						);

	/// Metoda ustawiaj�ca now� warto�ci ilo�ci cz�steczek dla danego emitera.
	/** \param[in] iIndex Indeks emitera, dla kt�rego chcemy zmieni� ilo�� cz�steczek.
	\param[in] iMaxParticles Nowa warto�� ilo�ci cz�steczek dla danego emitera. */
	GLvoid SetMaxParticlesForEmitter( GLint iIndex, GLint iMaxParticles );

	/// Metoda pobieraj�ca aktualn� ilo�� cz�steczek danego emitera.
	/** \param[in] iIndex Indeks emitera, z kt�rego chcemy pobra� warto�ci ilosci cz�steczek.
	\return Liczba ca�kowita okre�laj�ca maksymaln� ilo�� cz�steczek danego emitera. */
	GLint GetMaxParticlesForEmitter( GLint iIndex );

	/// Metoda sprawdzaj�ca czy emiter zawiera przynajmniej jedn� "�yw�", tj. niewygasni�t� cz�steczk�.
	/** \param[in] iIndex Indeks emitera, kt�rego chcemy podda� sprawdzeniu.
	\return \p GL_TRUE w przypadku gdy emiter zawiera przynajmniej jedn� niewyga�ni�t� cz�steczk�. W przeciwnym
	wypadku \p GL_FALSE. */
	GLboolean IsAliveParticleInEmitter( GLint iIndex ); 

	/// Metoda przywracaj�ca do "�ycia" wszytskie cz�steczki danego emitera.
	/** \param[in] iIndex Indeks emitera, kt�rego cz�steczki chemy przywr�ci� do "�ycia". */
	GLvoid SetParticleInEmitterAsLive( GLint iIndex );

	/// Metoda zwracajaca ilo�� stworzonych emiter�w.
	/** \return Liczba ca�kowita okre�laj�ca ilo�� stworzonych emiter�w. */
	inline GLint GetEmittersSize() { return static_cast<GLint>( m_aEmitters.size() ); }

	/// Metoda rysuj�ca cz�steczki danego emitera.
	/** \param[in] iIndex Indeks emitera, kt�rego cz�steczki chcemy rysowa�.
	\param[in] cPos Pozycja po�o�enia emitera w przestrzeni 3D. Je�li pozostawimy warto�� domy�ln� (czyli zerow�), to
	u�yta zostanie warto�� po�o�enia podana przy tworzeniu emitera. */
	GLvoid DrawEmitter( GLint iIndex, const CVector3 &cPos = CVector3() );

	/// Metoda usuwaj�ca pojedyncz� cz�steczk� z danego emitera.
	/** \param[in] iIndex Indeks emitera, z kt�rego chcemy usun�� cz�steczk�.
	\param[in] iIndexParticle Indeks cz�steczki, kt�r� chcemy usun��. */
	GLvoid DeleteParticle( GLint iIndex, GLint iIndexParticle );

	/// Metoda usuwaj�ca stworzyny wcze�niej emiter.
	/** \param[in] iIndex Indeks emitera jaki chcemy usun��. */
	GLvoid DeleteEmitter( GLint iIndex );

private:
	CGameControl		*m_pGameCtrl;		///< Wska�nik na obiekt klasy CGameControl.

	std::vector<SEmitter> m_aEmitters;		///< Tablica emiter�w.

	/// Metoda tworz�ca nowy emiter.
	/** \param[in] in_sEmitter Referencja na struktur� CParticles::SEmitter, z kt�rego stworzony zostanie nowy emiter.
	\return Indeks nowego emitera. */
	GLint AddAndCreateEmitter( const SEmitter &in_sEmitter );

	/// Metoda tworz�ca cz�steczki dla danego emitera.
	/** Metoda ta ca�kowicie na nowo tworzy wszystkie cz�steczki dla danego emitera.
	\param[in] iIndex Indeks emitera, dla kt�rego zostan� stworzone cz�steczki. */
	GLvoid CreateParticles( GLint iIndex );

	/// Metoda dodaj�ca nowe cz�steczki do ju� istniej�cych dla danego emitera.
	/** \param[in] iIndex Indeks emitera, dla kt�rego zostan� dodane nowe cz�steczki.
	\param[in] iMaxParticles Nowa warto�� ilo�ci cz�steczek dla danego emitera. */
	GLvoid AddParticles( GLint iIndex, GLint iMaxParticles );

	/// Metoda przywracaj�ca do "�ycia" pojedyncz� cz�steczk�.
	/** \param[in] iEmitterIndex Indeks emitera, kt�rego cz�steczk� chcmy przywr�ci� do "�ycia".
	\param[in] iParticleIndex Indeks cz�steczki, kt�r� chcemy na nowo przywr�ci� do "�ycia". */
	GLvoid SetAliveParticle( GLint iEmitterIndex, GLint iParticleIndex );

	GLfloat m_fSlowDown;	///< Zmienna zwalniaj�ca przemieszczanie si� cz�steczek.
	
	/// Metoda zwracaj�ca losow� warto�� dla szybko�ci wygasania cz�steczki.
	/** \return Liczba rzeczywista okre�laj�ca losow� warto�� dla szybko�ci wygasania cz�steczki. */
	inline GLfloat GetFade() {
		return static_cast<GLfloat>( rand() % 100 + 25 ) / 1000.0f + 0.003f; //od 0.0 do 0.099 + 0.003
	}
};
