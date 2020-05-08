/*! \file
	\brief Plik nag��wkowy dla klasy CSpeedControl.
*/
#pragma once

/// Klasa kontroli szybko�ci animacji.
/*! Klasa ta dostarcza szereg opcji zwi�zanych z kontrol� pr�dko�ci animacji. Dzi�ki temu wszelkie animacje b�d�
uniezale�nione od pr�dko�ci renderowania sceny. */
class CSpeedControl  
{
public:
	/// Konstruktor.
	/*! Konstruktor. */
	CSpeedControl();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CSpeedControl();

	/// Metoda od�wie�aj�ca aktualny czas dla kontroli pr�dko�ci animacji.
	/*! Metoda ta jest ju� wywo�ywana co klatk� w metodzie CGameControl::Draw. G�ownym zadaniem tej metody
	jest ustawienie zmiennej CSpeedControl::m_fMultiplier. */
	GLvoid RefreshTime();

	/// Metoda pobieraj�ca mno�nik dla wszelkich animacji.
	/*! \return Funkcja ta zwr�ci nam warto�� \p float. Warto�� t� nale�y mno�y� przez wszelkie zmienne wykorzystywane
	do animacji obiekt�w, tj. przesuwania, obracania, itp. Dzi�ki temu uzykamy szybko�� animacji niezale�n� od
	ilo�ci FPS. Je�eli aktywujemy pauz� to metoda ta zawsze zwr�ci \p 0.0f.
	\sa RefreshTime, m_fMultiplier, SetPause. */
	inline GLfloat GetMultiplier() {
		if( m_bPause ) 
			return 0.0f;
		return m_fMultiplier;
	}

	/// Metoda ustawiaj�ca pauz�.
	/*! Je�eli uaktywnimy pauz� to funkcja CSpeedControl::GetMultiplier b�dzie zwraca�a \p 0.0f. Je�eli warto�� ta
	b�dzie mno�ona przez wszelkie zmienne zwi�zane z ruchem i obrotem obiekt�w to w oczywisty spos�b je unieruchomimy
	daj�c efekt spauzowania gry.
	\param[in] bPause Podaj�c \p GL_TRUE uaktywniamy pauz�. Podaj�c \p GL_FALSE dezaktywujemy pauz�.
	\sa GetMultiplier, m_fMultiplier. */
	inline GLvoid SetPause( GLboolean bPause ) { m_bPause = bPause; }

	/// \name Zestaw metod kontroli animacji.
	//@{

	/// Metoda tworz�ca nezale�n� instancj� kontroli animacji.
	/*! Metoda ta utworzy dla nas zmienn� pami�taj�c� czas (liczb� milisekund) ostatniego wywo�ania animacji.
	Jest to niezb�dne do kontroli naszej animacji. Je�eli potrzebujemy kontrolowa� wi�cej animacji, np. pi�� to
	musimy po prostu pi�� razy wywo�a� t� metod�. Metod� t� nale�y wywo�ywa� w CGameControl::CreateAllAnimCtrl.
	\return Indeks kontroli animacji. */
	GLint CreateAnimationControl();

	/// Metoda sprawdzaj�ca czy min�� zadany czas dla danej animacji.
	/** Metod� t� nale�y wywo�ywa� w trakcie renderowania sceny. Dzi�ki tej metodzie mo�emy kontrolowa� animacj�
	poprzez sprawdzenie czy up�yne�a zadana liczbma milisekund dla danej instancji animacji stworzonej przez metod�
	CSpeedControl::CreateAnimationControl.
	\param[in] ulMs Liczba milisekund, czyli tutaj okre�lamy szybko�� naszej animacji.
	\param[in] iIndex Indeks (liczony od 0) zwr�cony przez CSpeedControl::CreateAnimationControl. Indeksy przypisane
	s� danym animacj�. Przypisanie to jest dowolne i okre�lane przez nas, np. ustalamy sobie, �e indeks 0 b�dzie
	odnosi� si� do animacji �o�nierza, indeks 1 do animacji lotu pociski, itd.
	\param[in] bUpdateIfPause Czy aktualizowa� czas je�eli jest pauze. Je�eli ustawimy t� warto�� na \p GL_TRUE
	(i tak jest domy�lnie) to gdy aktywna b�dzie pauza to czas szybko�ci naszej animacji zostanie od�wie�ony
	aktualnym czasem. Jest to wymagane w przypadkach gdy nie chcemy aby po anulowaniu pauzy dana animacja od razu
	ruszy�a (znaczy aby CSpeedControl::CheckAnimationTime od razu zwr�ci� \p TRUE) a stanie si� tak je�eli czas
	pauza potrwa d�u�ej ni� podana \a ulMs. Dzi�ki ustawieniu tej warto�ci na \p GL_TRUE pauza nie b�dzie mia�a
	wp�ywu na czas � czas jakby stanie w miejscu.
	\return \p 0 je�eli jeszcze nie up�yn�a podana w \a ulMs liczba milisekund. Liczba wi�ksza od zera � je�eli
	up�yn�a podana liczba milisekund i wtedy mo�emy wykona� dan� animacj�, np. podmieni� tekstury. Zwracana liczba
	wi�ksza od \p 0 to aktualny czas animacji jaki up�yn��.
	*/
	GLuint CheckAnimationTime( unsigned long ulMs, GLint iIndex, GLboolean bUpdateIfPause = GL_TRUE );

	/// Metoda zwracajaca aktualny czas jaki up�yn�� dla dnaje instancji animacji.
	/**
	\param[in] iIndex Indeks (liczony od 0) zwr�cony przez CSpeedControl::CreateAnimationControl. Indeksy przypisane
	s� danym animacj�. Przypisanie to jest dowolne i okre�lane przez nas.
	\return Aktualna liczba milisekund jaka up�yn�a dla danej instancji animacji. */
	GLint GetActualAnimationTime( GLint iIndex );

	/// Metoda aktualizuj�ca czas dla danej instancji animacji.
	/** Je�eli nie zawsze w ka�dej klatce b�dziemy mogli wywo�a� CSpeedControl::CheckAnimationTime() bo, np.
	uzale�nione to b�dzie od jakiej� instrukcji warunkowej, to je�eli po d�ugim okresie czasu, np. dw�ch sekundach
	wywo�amy, np. \p CheckAnimationTime(500, ...) to funkcja ta zawsze zwr�ci wynik wi�kszy od \p 0. Je�eli jednak
	chcemy aby odczeka� te \p 500 milisekund to w miejscu tego� warunku tworzymy \p else i tam wywo�ujemy
	\p UpdateAnimationTime.
	\param[in] iIndex Indeks (liczony od 0) zwr�cony przez CSpeedControl::CreateAnimationControl. Indeksy przypisane
	s� danym animacj�. Przypisanie to jest dowolne i okre�lane przez nas. */
	GLvoid UpdateAnimationTime( GLint iIndex );
	//@}
private:
	/// Mno�nik dla wszelkiego ruchu animacji w renderingu.
	/** Warto�� tego mno�lika jest ustawiana co klatk� w metodze CSpeedControl::RefreshTime. Warto�� mno�lika nale�y
	pobra� metod� CSpeedControl::GetMultiplier i mno�li� przez wszelkie zmienne odpowiedzialne za animacj�, jak
	przesuwanie, obracanie, itp.*/
	GLfloat m_fMultiplier;

	/// Zmienna pomocnicza przy obliczaniu aktualnego czasu dla CSpeedControl::m_fMultiplier.
	GLfloat m_fTimeLastFrame;

	/// Zmienna pomocnicza przy obliczaniu aktualnego czasu dla CSpeedControl::m_fMultiplier.
	/** Je�eli nie odpowiada nam szybko�� zwracana przez CSpeedControl::GetMultiplier, mo�emy w konstruktorze klasy
	CSpeedControl spr�bowa� zmodyfikowa� warto�� przypisywan� do zmiennej CSpeedControl::m_fRegulationSpeed.
	Je�eli zmniejszymy przypisywan� warto�� dla tej zmiennej, w�wczas animacja b�dzie wolniejsza. */
	GLfloat m_fRegulationSpeed;

	/// Metoda zwracaj�ca ilo�� instancji animacji.
	inline GLint GetAnimationSize() { return static_cast<GLint>( m_aAnimation.size() ); }

	/// Tablica przechowuj�ca aktualne czasy (w milisekundach) dla danych instancji animacji.
	std::vector<unsigned long> m_aAnimation;

	/// Metoda zwracaj�ca aktualny czas w milisekundach.
	/*! \return Aktualny czas (w milisekundach) jaki up�yn�� od uruchomienia systemu.	*/
	unsigned long GetTimeInMs();

	GLboolean m_bPause; ///< Flaga wskazuj�ca czy aktywna jest pauza.
};