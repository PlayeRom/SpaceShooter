/*! \file
	\brief Plik nag��wkowy dla klasy CFramework.
*/

#pragma once

class CWindowData;
class CGameControl;

/// Klasa szkieletu aplikacji. 
/*! Zadaniem tej klasy jest inicjalizacja oraz niszczenie ca�ego frameworka oraz zarz�dzanie g��wn� p�tl� programu. \n
Nale�y utworzy� obiekt tej klasy w g��wnej metodzie programu WinMain oraz wywo�a� jej jedyn� publiczn� metod�
CFramework::Run. */
class CFramework
{
public:
	/// Konstruktor.
	/*! Konstruktor. */
	CFramework();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CFramework();

	/// Metoda zarz�dzaj�ca ca�ym frameworkiem.
	/*! \return Kod wyj�cia programu. Je�eli \p 0 to program zako�czy� si� prawid�owo. Warto�� ta musi by� tak�e
	zwr�cona przez g��wn� funkcj� programu WinMain. */
	GLint Run();

	/// G��wna metoda, od kt�rej rozpoczyna si� rysowanie ca�ej sceny.
	/*! Wewn�trz tej metody wywo�ywana jest nie mniej wa�na metoda CGameControl::Draw.
	\return \p GL_TRUE w przypadku powodzenia. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean DrawGLScene();

	/// Metoda usuwaj�ca ca�e okno frameworka oraz obiekt klasy CGameControl.
	/*! \param[in] bKillGLWindow Podaj�c jako \p GL_FALSE, zniszczony zostanie tylko obiekt klasy CGameControl.
	Podaj�c \p GL_TRUE, opr�cz zniszczenia CGameControl, zniszczone zostanie tak�e okno apliakcji. */
	GLvoid Destroy( GLboolean bKillGLWindow );

	static CWindowData*		g_pWinData;					///< Wska�nik na obiekt klasy singletonu CWindowData.
	static CGameControl*	g_pGameCtrl;				///< Wska�nik na obiekt klasy singletonu CGameControl.
	static GLboolean		g_bActive;					///< Znacznik aktywno�ci okna, domy�lnie ustawiony na \p GL_TRUE np. okno nie zminimalizowane.
	static GLint			g_iCompleteWheelRotate;		///< Licznik kompletnego obrotu rolki myszy.

protected:
	/// G��wna metoda inicjalizuj�ca wst�pne parametry OpenGL-a.
	/*! Wewn�trz tej metody wywo�ywana jest tak�e metoda CFramework::CreateGameControl.
	\return \p GL_TRUE w przypadku powodzenia inicjalizacji. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean InitalizationGL();

	/// Metoda tworz�ca oraz inicjalizuj�ca obiekt klasy CGameControl.
	/*! \return \p GL_TRUE w przypadku powodzenia inicjalizacji. \p GL_FALSE w przypadku wyst�pienia jakiego� b��du. */
	GLboolean CreateGameControl();
};
