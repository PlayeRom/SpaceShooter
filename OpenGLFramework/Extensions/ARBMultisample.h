/*! \file
	\brief Plik nag��wkowy dla klasy CARBMultisample.
*/

/*====================================
	Name: ARB_multisample.h
	Author: Colt "MainRoach" McAnlis
	Modification: PlayeRom
	Date: 4/29/04
	Desc:
		This file contains our external items

====================================*/

#pragma once

// Declairations We'll Use
//#define WGL_SAMPLE_BUFFERS_ARB		 0x2041
//#define WGL_SAMPLES_ARB			     0x2042

/// Klasa obs�uguj�ca antyaliasing.
/*! Klasa obs�uguj�ca rozszerzenie WGL_ARB_multisample, wspieraj�ca pe�noekranowy antyaliasing. */
class CARBMultisample  
{
public:
	/// Konstruktor.
	/*! Konstruktor. */
	CARBMultisample();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CARBMultisample();

	/// Inicjalizacja antyaliasingu
	/*! Metoda ustawia antyaliasing o pr�bkowaniu zadanym w CARBMultisample::m_iAntialiasingSamples.
	\param[in] hWnd Uchwyt okna aplikacji.
	\return \p GL_TRUE w przypadku powodzenia ustawienia antyaliasingu. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du.
	\sa m_iAntialiasingSamples. */
	GLboolean InitMultisample( HWND hWnd );

	/// Metoda sprawdzaj�ca czy antyaliasing jest wspierany.
	/*! \return \p GL_TRUE w przypadku wspierania antyaliasingu. W przeciwnym wypadku \p GL_FALSE.
	\sa m_bMultisampleSupported, SetSupported. */
	inline GLboolean IsSupported() { return m_bMultisampleSupported; }

	/// Ustawienie flagi oznaczaj�cej wspieranie antyaliasingu.
	/*! \param[in] bSupported Ustawiaj�c na \p GL_TRUE m�wimy, �e antyaliasing jest wspierany.
	\sa m_bMultisampleSupported, IsSupported. */
	inline GLvoid SetSupported( GLboolean bSupported ) { m_bMultisampleSupported = bSupported; }

	/// Ustawienie pr�bkowania dla antyaliasingu.
	/*! \param[in] iSamples Liczba pr�bkowania. Mo�na poda� takie warto�ci jak 0, 2, 4, 6 itd.
	\sa m_iAntialiasingSamples, GetSamples. */
	inline GLvoid SetSamples( GLint iSamples ) { m_iAntialiasingSamples = iSamples; }

	/// Pobranie aktualnie ustawionej ilo�ci pr�bek dla antyaliasingu.
	/*! \return Liczba pr�bek dla antyaliasungu.
	\sa m_iAntialiasingSamples, SetSamples. */
	inline GLint GetSamples() { return m_iAntialiasingSamples; }

	/// Pobranie formatu piksela z ustawionym antyaliasingiem.
	/*! \return Format piksela jaki musi by� przypisany do kontekstu urz�dzenia w celu
	wykorzystania anytaliasingu.
	\sa m_iMultisampleFormat. */
	inline GLint GetFormat() { return m_iMultisampleFormat; }

private:
	/// Metoda sprawdzaj�ca czy dana karta graficzna wspiera dane rozszerzenie.
	/*! \param[in] lpExtension Nazwa rozszerzenia jakie chcemy sprawdzi�.
	\return \p GL_TRUE je�eli dane rozszerzenie jest wspierane. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean WGLisExtensionSupported( const char *lpExtension );

	GLint		m_iMultisampleFormat;		///< Przechowuje format piksela z antyaliasingiem.
	GLboolean	m_bMultisampleSupported;	///< Czy pe�noekranowy antyaliasing jest wspierany?
	GLint		m_iAntialiasingSamples;		///< Przechowuje aktualn� ilo�� pr�bek dla antyaliasingu.
};