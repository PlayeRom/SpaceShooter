/*! \file
	\brief Plik nag��wkowy dla klas CVector2, CVector3, CVector3d oraz CVector4.
*/
#pragma once

#include <math.h>
/*
#pragma pack(push)
#pragma pack(1)
...
#pragma pack(pop)
*/

/// Klasa reprezentuj�ca wektor 2D.
class CVector2
{
public:
	/// Konstruktor.
	/** Konstruktor domy�lny inicjalizuj�cy atrybuty zerami. */
	CVector2();

	/// Konstruktor z przypisaniem.
	/** \param[in] fX Wsp�rz�dna X.
	\param[in] fY Wsp�rz�dna Y. */
	CVector2(GLfloat fX, GLfloat fY);

	/// Konstruktor z przypisaniem.
	/** \param[in] cVec Referencja na obiekt klasy CVector2 inicjalizuj�ca atrybuty. */
	CVector2(const CVector2 &cVec);

	/// Destruktor.
	/** Destruktor. */
	~CVector2();

	/// Metoda normalizuj�ca wektor.
	/** Metoda przycina d�ugo�� wektora do rozmiaru r�wnego dok�adnie \p 1. */
	GLvoid Normalize();

	/// Przeci��ony operator dodawania wektor�w. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie dodawany. 
	\return Nowy wektor jako rezultat dodawania. */
	CVector2 operator+(const CVector2 &cVec) const;

	/// Przeci��ony operator odejmowania wektor�w. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie odejmowany. 
	\return Nowy wektor jako rezultat odejmowania. */
	CVector2 operator-(const CVector2 &cVec) const;

	/// Przeci��ony operator mno�enia wektor�w. 
	/** \param[in] cVec Referencja na wektor, przez kt�ry b�dziemy mno�yli. 
	\return Nowy wektor jako rezultat mno�enia. */
	CVector2 operator*(const CVector2 &cVec) const;

	/// Przeci��ony operator dzielenia wektor�w. 
	/** \param[in] cVec Referencja na wektor, przez kt�ry b�dziemy dzieli�. 
	\return Nowy wektor jako rezultat dzielenia. */
	CVector2 operator/(const CVector2 &cVec) const;

	/// Przeci��ony operator dodawania skalara do wektora. 
	/** \param[in] fValue Skalar, kt�ry b�dzie dodany do wektora. 
	\return Nowy wektor jako rezultat dodania skalara. */
	CVector2 operator+(GLfloat fValue) const;

	/// Przeci��ony operator odejmowania skalara od wektora. 
	/** \param[in] fValue Skalar, kt�ry b�dzie odejmowany od wektora. 
	\return Nowy wektor jako rezultat odejmowania skalara. */
	CVector2 operator-(GLfloat fValue) const;

	/// Przeci��ony operator mno�enia wektora przez skalar. 
	/** \param[in] fValue Skalar, przez kt�ry b�dzie mno�ony wektor. 
	\return Nowy wektor jako rezultat mno�enia przez skalara. */
	CVector2 operator*(GLfloat fValue) const;

	/// Przeci��ony operator dzielenia wektora przez skalar. 
	/** \param[in] fValue Skalar, przez kt�ry b�dzie dzielony wektor. 
	\return Nowy wektor jako rezultat dzielenia przez skalara. */
	CVector2 operator/(GLfloat fValue) const;

	/// Przeci��ony operator przypisania dodawania wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie dodawany. 
	\return Zmodyfikowany wektor jako rezultat dodawania. */
	CVector2& operator+=(const CVector2 &cVec);

	/// Przeci��ony operator przypisania odejmowania wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie odejmowany. 
	\return Zmodyfikowany wektor jako rezultat odejmowania. */
	CVector2& operator-=(const CVector2 &cVec);

	/// Przeci��ony operator przypisania mno�enia wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie mno�ony. 
	\return Zmodyfikowany wektor jako rezultat mno�enia. */
	CVector2& operator*=(const CVector2 &cVec);

	/// Przeci��ony operator przypisania dodawania skalara. 
	/** \param[in] fValue Skalar, kt�ry b�dzie dodawany do wektora. 
	\return Zmodyfikowany wektor jako rezultat dodawania skalara. */
	CVector2& operator+=(GLfloat fValue);

	/// Przeci��ony operator przypisania odejmowania skalara. 
	/** \param[in] fValue Skalar, kt�ry b�dzie odejmowany od wektora. 
	\return Zmodyfikowany wektor jako rezultat odejmowania skalara. */
	CVector2& operator-=(GLfloat fValue);

	/// Przeci��ony operator przypisania mno�enia przez skalara. 
	/** \param[in] fValue Skalar, kt�ry b�dzie mno�ony przez wektor. 
	\return Zmodyfikowany wektor jako rezultat mno�enia przez skalar. */
	CVector2& operator*=(GLfloat fValue);

	/// Przeci��ony operator minus, jako zmiana zwrotu wektora na przeciwny.
	/** \return Nowy wektor z przeciwnym zwrotem. */
	CVector2 operator-() const;

	/// Przeci��enie operatora plus.
	/** \return Ten sam wektor. */
	CVector2 operator+() const ;

	/// Przeci��ony operator przypisania wektora.
	/** \param[in] cVec Referenzja na wektor, kt�ry zostanie przypisany aktualnemu.
	\return Zmodyfikowany, aktualny wektor o przypisany. */
	CVector2& operator=(const CVector2 &cVec);

	/// Przeci��ony operator por�wnania wektor�w.
	/** \param[in] cVec Referencja na wektor, do kt�rego b�dzie por�wnywany aktualny.
	\return \p GL_TRUE gdy wektory s� sobie r�wne, w przeciwnym wypadku \p GL_FALSE. */
	GLboolean operator==(const CVector2 &cVec) const;

	/// Metoda obliczaj�ca odleg�o�� mi�dzy aktualnym i podanym wektorem.
	/** \param[in] cVec Referencja na drugi wektor, do kt�rego b�dzie obliczana odleg�o��. 
	\return Liczba rzeczywista okre�laj�ca odleg�o�� mi�dzy punktami wektor�w. */
	inline GLfloat CalcDistance2D(const CVector2 &cVec){
		GLfloat fX = (x - cVec.x) * (x - cVec.x);
		GLfloat fY = (y - cVec.y) * (y - cVec.y);
		return sqrt(fX + fY);
	}

	/// Metoda obliczaj�ca d�ugo�� wektoar.
	/** \return Liczba rzeczywista okre�laj�ca d�ugo�� wektora. */
	inline GLfloat GetLength(){
		return sqrt((x * x) + (y * y));
	}

	GLfloat x;	///< Wsp�rz�dna x wektora.
	GLfloat y;	///< Wsp�rz�dna y wektora.
};

/// Klasa reprezentuj�ca wektor 4D.
class CVector4
{
public:
	/// Konstruktor.
	/** Konstruktor domy�lny inicjalizuj�cy atrybuty zerami. */
	CVector4();

	/// Konstruktor z przypisaniem.
	/** \param[in] fX Wsp�rz�dna X.
	\param[in] fY Wsp�rz�dna Y
	\param[in] fZ Wsp�rz�dna Z.
	\param[in] fW Wsp�rz�dna W. */
	CVector4(GLfloat fX, GLfloat fY, GLfloat fZ, GLfloat fW);

	/// Konstruktor z przypisaniem.
	/** \param[in] cVec Referencja na obiekt klasy CVector4 inicjalizuj�ca atrybuty. */
	CVector4(const CVector4 &cVec);

	/// Destruktor.
	/** Destruktor. */
	~CVector4();

	/// Przeci��ony operator dodawania wektor�w. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie dodawany. 
	\return Nowy wektor jako rezultat dodawania. */
	CVector4 operator+(const CVector4 &cVec) const;

	/// Przeci��ony operator odejmowania wektor�w. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie odejmowany. 
	\return Nowy wektor jako rezultat odejmowania. */
	CVector4 operator-(const CVector4 &cVec) const;

	/// Przeci��ony operator przypisania wektora.
	/** \param[in] cVec Referenzja na wektor, kt�ry zostanie przypisany aktualnemu.
	\return Zmodyfikowany, aktualny wektor o przypisany. */
	CVector4& operator=(const CVector4 &cVec);

	/// Przeci��ony operator por�wnania wektor�w.
	/** \param[in] cVec Referencja na wektor, do kt�rego b�dzie por�wnywany aktualny.
	\return \p GL_TRUE gdy wektory s� sobie r�wne, w przeciwnym wypadku \p GL_FALSE. */
	GLboolean operator==(const CVector4 &cVec) const;

	/// Przeci��ony operator przypisania dodawania wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie dodawany. 
	\return Zmodyfikowany wektor jako rezultat dodawania. */
	inline CVector4& operator+=(const CVector4 &cVec) {
		x += cVec.x; y += cVec.y; z += cVec.z;
		return *this;
	}

	/// Przeci��ony operator przypisania odejmowania wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie odejmowany. 
	\return Zmodyfikowany wektor jako rezultat odejmowania. */
	inline CVector4& operator-=(const CVector4 &cVec) {
		x -= cVec.x; y -= cVec.y; z -= cVec.z; 
		return *this;
	}

	/// Przeci��ony operator przypisania mno�enia wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie mno�ony. 
	\return Zmodyfikowany wektor jako rezultat mno�enia. */
	inline CVector4& operator*=(const CVector4 &cVec) {
		x *= cVec.x; y *= cVec.y; z *= cVec.z;
		return *this;
	}

	/// Metoda obliczaj�ca iloczyn skalarny mi�dzy aktualnym i podanym wektorem.
	/** \param[in] cVec Referencja na drugi wektor, mi�dzy kt�rym b�dzie liczony iloczyn skalarny.
	\return Liczba rzeczywista okre�laj�ca iloczyn skalarny. */
	inline GLfloat DotProduct(const CVector4 &cVec){
		return	x * cVec.x +
				y * cVec.y +
				z * cVec.z +
				w * cVec.w;
	}

	GLfloat x;	///< Wsp�rz�dna x wektora.
	GLfloat y;	///< Wsp�rz�dna y wektora.
	GLfloat z;	///< Wsp�rz�dna z wektora.
	GLfloat w;	///< Wsp�rz�dna w wektora.
};

/// Klasa reprezentuj�ca wektor 3D typu float.
class CVector3
{
public:
	/// Konstruktor.
	/** Konstruktor domy�lny inicjalizuj�cy atrybuty zerami. */
	CVector3(): x(0.0f), y(0.0f), z(0.0f) {}

	/// Konstruktor z przypisaniem.
	/** \param[in] fX Wsp�rz�dna X.
	\param[in] fY Wsp�rz�dna Y
	\param[in] fZ Wsp�rz�dna Z. */
	CVector3(GLfloat fX, GLfloat fY, GLfloat fZ): x(fX), y(fY), z(fZ) {}

	/// Konstruktor z przypisaniem.
	/** \param[in] cVec Referencja na obiekt klasy CVector3 inicjalizuj�ca atrybuty. */
	CVector3(const CVector3 &cVec) { x = cVec.x; y = cVec.y; z = cVec.z; }
	
	/// Destruktor.
	/** Destruktor. */
	~CVector3() {}

	/// Metoda normalizuj�ca wektor.
	/** Metoda przycina d�ugo�� wektora do rozmiaru r�wnego dok�adnie \p 1. */
	GLvoid Normalize();

	/// Przeci��ony operator dodawania wektor�w. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie dodawany. 
	\return Nowy wektor jako rezultat dodawania. */
	inline CVector3 operator+(const CVector3 &cVec) const {
		return CVector3(x + cVec.x, y + cVec.y, z + cVec.z);
	}

	/// Przeci��ony operator odejmowania wektor�w. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie odejmowany. 
	\return Nowy wektor jako rezultat odejmowania. */
	inline CVector3 operator-(const CVector3 &cVec) const {
		return CVector3(x - cVec.x,	y - cVec.y,	z - cVec.z);
	}

	/// Przeci��ony operator odejmowania wektor�w. 
	/** \param[in] cVec Referencja na wektor CVector4, kt�ry b�dzie odejmowany z pomini�ciem sk�adowej CVector4::w.
	\return Nowy wektor jako rezultat odejmowania. */
	inline CVector3 operator-(const CVector4 &cVec) const {
		return CVector3(x - cVec.x,	y - cVec.y,	z - cVec.z);
	}

	/// Przeci��ony operator mno�enia wektor�w. 
	/** \param[in] cVec Referencja na wektor, przez kt�ry b�dziemy mno�yli. 
	\return Nowy wektor jako rezultat mno�enia. */
	inline CVector3 operator*(const CVector3 &cVec) const {
		return CVector3(x * cVec.x, y * cVec.y,	z * cVec.z);
	}

	/// Przeci��ony operator dzielenia wektor�w. 
	/** \param[in] cVec Referencja na wektor, przez kt�ry b�dziemy dzieli�. 
	\return Nowy wektor jako rezultat dzielenia. */
	inline CVector3 operator/(const CVector3 &cVec) const {
		return CVector3(x / cVec.x,	y / cVec.y,	z / cVec.z);
	}

	/// Przeci��ony operator dodawania skalara do wektora. 
	/** \param[in] fValue Skalar, kt�ry b�dzie dodany do wektora. 
	\return Nowy wektor jako rezultat dodania skalara. */
	inline CVector3 operator+(GLfloat fValue) const {
		return CVector3(x + fValue,	y + fValue,	z + fValue);
	}

	/// Przeci��ony operator odejmowania skalara od wektora. 
	/** \param[in] fValue Skalar, kt�ry b�dzie odejmowany od wektora. 
	\return Nowy wektor jako rezultat odejmowania skalara. */
	inline CVector3 operator-(GLfloat fValue) const {
		return CVector3(x - fValue,	y - fValue,	z - fValue);
	}

	/// Przeci��ony operator mno�enia wektora przez skalar. 
	/** \param[in] fValue Skalar, przez kt�ry b�dzie mno�ony wektor. 
	\return Nowy wektor jako rezultat mno�enia przez skalara. */
	inline CVector3 operator*(GLfloat fValue) const {
		return CVector3(x * fValue,	y * fValue,	z * fValue);
	}

	/// Przeci��ony operator dzielenia wektora przez skalar. 
	/** \param[in] fValue Skalar, przez kt�ry b�dzie dzielony wektor. 
	\return Nowy wektor jako rezultat dzielenia przez skalara. */
	inline CVector3 operator/(GLfloat fValue) const {
		return CVector3(x / fValue,	y / fValue,	z / fValue);
	}

	/// Przeci��ony operator przypisania dodawania wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie dodawany. 
	\return Zmodyfikowany wektor jako rezultat dodawania. */
	inline CVector3& operator+=(const CVector3 &cVec) {
		x += cVec.x; y += cVec.y; z += cVec.z;
		return *this;
	}

	/// Przeci��ony operator przypisania odejmowania wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie odejmowany. 
	\return Zmodyfikowany wektor jako rezultat odejmowania. */
	inline CVector3& operator-=(const CVector3 &cVec) {
		x -= cVec.x; y -= cVec.y; z -= cVec.z; 
		return *this;
	}

	/// Przeci��ony operator przypisania mno�enia wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie mno�ony. 
	\return Zmodyfikowany wektor jako rezultat mno�enia. */
	inline CVector3& operator*=(const CVector3 &cVec) {
		x *= cVec.x; y *= cVec.y; z *= cVec.z;
		return *this;
	}

	/// Przeci��ony operator przypisania dodawania skalara. 
	/** \param[in] fValue Skalar, kt�ry b�dzie dodawany do wektora. 
	\return Zmodyfikowany wektor jako rezultat dodawania skalara. */
	inline CVector3& operator+=(GLfloat fValue) {
		x += fValue; y += fValue; z += fValue;
		return *this;
	}

	/// Przeci��ony operator przypisania odejmowania skalara. 
	/** \param[in] fValue Skalar, kt�ry b�dzie odejmowany od wektora. 
	\return Zmodyfikowany wektor jako rezultat odejmowania skalara. */
	inline CVector3& operator-=(GLfloat fValue) {
		x -= fValue; y -= fValue; z -= fValue;
		return *this;
	}

	/// Przeci��ony operator przypisania mno�enia przez skalara. 
	/** \param[in] fValue Skalar, kt�ry b�dzie mno�ony przez wektor. 
	\return Zmodyfikowany wektor jako rezultat mno�enia przez skalar. */
	inline CVector3& operator*=(GLfloat fValue) {
		x *= fValue; y *= fValue; z *= fValue;
		return *this;
	}

	/// Przeci��ony operator minus, jako zmiana zwrotu wektora na przeciwny.
	/** \return Nowy wektor z przeciwnym zwrotem. */
	inline CVector3 operator-() const { return CVector3(-x, -y, -z); }

	/// Przeci��enie operatora plus.
	/** \return Ten sam wektor. */
	inline CVector3 operator+() const { return *this; }

	/// Przeci��ony operator przypisania wektora.
	/** \param[in] cVec Referenzja na wektor, kt�ry zostanie przypisany aktualnemu.
	\return Zmodyfikowany, aktualny wektor o przypisany. */
	inline CVector3& operator=(const CVector3 &cVec) {
		x = cVec.x; y = cVec.y; z = cVec.z;
		return *this;
	}

	/// Przeci��ony operator por�wnania wektor�w.
	/** \param[in] cVec Referencja na wektor, do kt�rego b�dzie por�wnywany aktualny.
	\return \p GL_TRUE gdy wektory s� sobie r�wne, w przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean operator==(const CVector3 &cVec) const {
		return (x == cVec.x && y == cVec.y && z == cVec.z);
	}

	/// Przeci��ony operator por�wnania wektora ze skalarem.
	/** \param[in] fValue Skalar, z kt�rym b�dzie por�wnywany wektor.
	\return \p GL_TRUE gdy wsp�rz�dne wektora s� r�wny skalarowi, w przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean operator==(GLfloat fValue) const {
		return (x == fValue && y == fValue && z == fValue);
	}

	/// Metoda obliczaj�ca d�ugo�� wektoar.
	/** \return Liczba rzeczywista okre�laj�ca d�ugo�� wektora. */
	inline GLfloat GetLength(){
		return sqrt((x * x) + (y * y) + (z * z));
	}
	
	/// Metoda obliczaj�ca odleg�o�� mi�dzy aktualnym i podanym wektorem.
	/** \param[in] cVec Referencja na drugi wektor, do kt�rego b�dzie obliczana odleg�o��. 
	\return Liczba rzeczywista okre�laj�ca odleg�o�� mi�dzy punktami wektor�w. */
	inline GLfloat CalcDistance3D(const CVector3 &cVec){
		GLfloat fX = (x - cVec.x) * (x - cVec.x);
		GLfloat fY = (y - cVec.y) * (y - cVec.y);
		GLfloat fZ = (z - cVec.z) * (z - cVec.z);
		return sqrt(fX + fY + fZ);
	}

	/// Metoda obliczaj�ca odleg�o�� mi�dzy aktualnym i podanym wektorem na p�aszczy�nie X-Y.
	/** \param[in] cVec Referencja na drugi wektor, do kt�rego b�dzie obliczana odleg�o��. 
	\return Liczba rzeczywista okre�laj�ca odleg�o�� mi�dzy punktami wektor�w na p�aszczy�nie X-Y. */
	inline GLfloat CalcDistanceXY(const CVector3 &cVec){
		GLfloat fX = (x - cVec.x) * (x - cVec.x);
		GLfloat fY = (y - cVec.y) * (y - cVec.y);
		return sqrt(fX + fY);
	}

	/// Metoda obliczaj�ca odleg�o�� mi�dzy aktualnym i podanym wektorem na p�aszczy�nie X-Z.
	/** \param[in] cVec Referencja na drugi wektor, do kt�rego b�dzie obliczana odleg�o��. 
	\return Liczba rzeczywista okre�laj�ca odleg�o�� mi�dzy punktami wektor�w na p�aszczy�nie X-Z. */
	inline GLfloat CalcDistanceXZ(const CVector3 &cVec){
		GLfloat fX = (x - cVec.x) * (x - cVec.x);
		GLfloat fZ = (z - cVec.z) * (z - cVec.z);
		return sqrt(fX + fZ);
	}

	/// Metoda obliczaj�ca odleg�o�� mi�dzy aktualnym i podanym wektorem na p�aszczy�nie Y-Z.
	/** \param[in] cVec Referencja na drugi wektor, do kt�rego b�dzie obliczana odleg�o��. 
	\return Liczba rzeczywista okre�laj�ca odleg�o�� mi�dzy punktami wektor�w na p�aszczy�nie Y-Z. */
	inline GLfloat CalcDistanceYZ(const CVector3 &cVec){
		GLfloat fY = (y - cVec.y) * (y - cVec.y);
		GLfloat fZ = (z - cVec.z) * (z - cVec.z);
		return sqrt(fY + fZ);
	}

	/// Metoda obliczaj�ca odleg�o�� mi�dzy aktualnym i podanym wektorem na osi X.
	/** \param[in] cVec Referencja na drugi wektor, do kt�rego b�dzie obliczana odleg�o��. 
	\return Liczba rzeczywista okre�laj�ca odleg�o�� mi�dzy punktami wektor�w na osi X. */
	inline GLfloat CalcDistanceX(const CVector3 &cVec){
		return sqrt((x - cVec.x) * (x - cVec.x));
	}

	/// Metoda obliczaj�ca odleg�o�� mi�dzy aktualnym i podanym wektorem na osi Y.
	/** \param[in] cVec Referencja na drugi wektor, do kt�rego b�dzie obliczana odleg�o��. 
	\return Liczba rzeczywista okre�laj�ca odleg�o�� mi�dzy punktami wektor�w na osi Y. */
	inline GLfloat CalcDistanceY(const CVector3 &cVec){
		return sqrt((y - cVec.y) * (y - cVec.y));
	}

	/// Metoda obliczaj�ca odleg�o�� mi�dzy aktualnym i podanym wektorem na osi Z.
	/** \param[in] cVec Referencja na drugi wektor, do kt�rego b�dzie obliczana odleg�o��. 
	\return Liczba rzeczywista okre�laj�ca odleg�o�� mi�dzy punktami wektor�w na osi Z. */
	inline GLfloat CalcDistanceZ(const CVector3 &cVec){
		return sqrt((z - cVec.z) * (z - cVec.z));
	}

	/// Przeci�zony operator [] do wy�uskania sk�adowych wektora.
	/** \param[in] iIndex Indeks sk�adowej wektora, gdzie odpowiednio indeks \p 0 odpowiada warto�ci CVector3::x,
	\p 1 CVector3::y, \p 2 CVector3::z. 
	\return Wy�uskany jeden z atrybut�w klasy. */
	inline GLfloat& operator[](GLint iIndex){
		return (&x)[iIndex];
	}

	/// Metoda obliczaj�ca iloczyn skalarny mi�dzy aktualnym i podanym wektorem.
	/** \param[in] cVec Referencja na drugi wektor, mi�dzy kt�rym b�dzie liczony iloczyn skalarny.
	\return Liczba rzeczywista okre�laj�ca iloczyn skalarny. */
	inline GLfloat DotProduct(const CVector3 &cVec){
		return	x * cVec.x +
				y * cVec.y +
				z * cVec.z;
	}

	/// Metoda obliczaj�ca iloczyn wektorowy mi�dzy aktualnym i podanym wektorem.
	/** \param[in] cVec Referencja na drugi wektor, mi�dzy kt�rym b�dzie obliczany iloczyn wektorowy.
	\return Liczba rzeczywista okre�laj�ca iloczyn wektorowy. */
	inline CVector3 CrossProduct(const CVector3 &cVec){
		CVector3 cTemp;
		cTemp.x = (y * cVec.z) - (z * cVec.y);
		cTemp.y = (z * cVec.x) - (x * cVec.z);
		cTemp.z = (x * cVec.y) - (y * cVec.x); 
		return cTemp;
	}

	GLfloat x;	///< Wsp�rz�dna x wektora.
	GLfloat y;	///< Wsp�rz�dna y wektora.
	GLfloat z;	///< Wsp�rz�dna z wektora.
};

/// Klasa reprezentuj�ca wektor 3D typu double.
class CVector3d
{
public:
	/// Konstruktor.
	/** Konstruktor domy�lny inicjalizuj�cy atrybuty zerami. */
	CVector3d() { x = y = z = 0.0; }

	/// Konstruktor z przypisaniem.
	/** \param[in] dX Wsp�rz�dna X.
	\param[in] dY Wsp�rz�dna Y
	\param[in] dZ Wsp�rz�dna Z. */
	CVector3d(GLdouble dX, GLdouble dY, GLdouble dZ){ x = dX; y = dY; z = dZ; }

	/// Konstruktor z przypisaniem.
	/** \param[in] cVec Referencja na obiekt klasy CVector3d inicjalizuj�ca atrybuty. */
	CVector3d(const CVector3d &cVec){ x = cVec.x; y = cVec.y; z = cVec.z; }

	/// Destruktor.
	/** Destruktor. */
	~CVector3d() {}

	/// Metoda normalizuj�ca wektor.
	/** Metoda przycina d�ugo�� wektora do rozmiaru r�wnego dok�adnie \p 1. */
	GLvoid Normalize();

	/// Przeci��ony operator dodawania wektor�w. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie dodawany. 
	\return Nowy wektor jako rezultat dodawania. */
	inline CVector3d operator+(const CVector3d &cVec) const{
		return CVector3d(x + cVec.x, y + cVec.y, z + cVec.z);
	}

	/// Przeci��ony operator odejmowania wektor�w. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie odejmowany. 
	\return Nowy wektor jako rezultat odejmowania. */
	inline CVector3d operator-(const CVector3d &cVec) const{
		return CVector3d(x - cVec.x, y - cVec.y, z - cVec.z);
	}

	/// Przeci��ony operator mno�enia wektor�w. 
	/** \param[in] cVec Referencja na wektor, przez kt�ry b�dziemy mno�yli. 
	\return Nowy wektor jako rezultat mno�enia. */
	inline CVector3d operator*(const CVector3d &cVec) const{
		return CVector3d(x * cVec.x, y * cVec.y, z * cVec.z);
	}

	/// Przeci��ony operator dzielenia wektor�w. 
	/** \param[in] cVec Referencja na wektor, przez kt�ry b�dziemy dzieli�. 
	\return Nowy wektor jako rezultat dzielenia. */
	inline CVector3d operator/(const CVector3d &cVec) const{
		return CVector3d(x / cVec.x, y / cVec.y, z / cVec.z);
	}

	/// Przeci��ony operator dodawania skalara do wektora. 
	/** \param[in] dValue Skalar, kt�ry b�dzie dodany do wektora. 
	\return Nowy wektor jako rezultat dodania skalara. */
	inline CVector3d operator+(GLdouble dValue) const{
		return CVector3d(x + dValue, y + dValue, z + dValue);
	}

	/// Przeci��ony operator odejmowania skalara od wektora. 
	/** \param[in] dValue Skalar, kt�ry b�dzie odejmowany od wektora. 
	\return Nowy wektor jako rezultat odejmowania skalara. */
	inline CVector3d operator-(GLdouble dValue) const{
		return CVector3d(x - dValue, y - dValue, z - dValue);
	}

	/// Przeci��ony operator mno�enia wektora przez skalar. 
	/** \param[in] dValue Skalar, przez kt�ry b�dzie mno�ony wektor. 
	\return Nowy wektor jako rezultat mno�enia przez skalara. */
	inline CVector3d operator*(GLdouble dValue) const{
		return CVector3d(x * dValue, y * dValue, z * dValue);
	}

	/// Przeci��ony operator dzielenia wektora przez skalar. 
	/** \param[in] dValue Skalar, przez kt�ry b�dzie dzielony wektor. 
	\return Nowy wektor jako rezultat dzielenia przez skalara. */
	inline CVector3d operator/(GLdouble dValue) const{
		return CVector3d(x / dValue, y / dValue, z / dValue);
	}

	/// Przeci��ony operator przypisania dodawania wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie dodawany. 
	\return Zmodyfikowany wektor jako rezultat dodawania. */
	inline CVector3d& operator+=(const CVector3d &cVec){
		x += cVec.x; y += cVec.y; z += cVec.z;
		return *this;
	}

	/// Przeci��ony operator przypisania odejmowania wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie odejmowany. 
	\return Zmodyfikowany wektor jako rezultat odejmowania. */
	inline CVector3d& operator-=(const CVector3d &cVec){
		x -= cVec.x; y -= cVec.y; z -= cVec.z;
		return *this;
	}

	/// Przeci��ony operator przypisania mno�enia wektora. 
	/** \param[in] cVec Referencja na wektor, kt�ry b�dzie mno�ony. 
	\return Zmodyfikowany wektor jako rezultat mno�enia. */
	inline CVector3d& operator*=(const CVector3d &cVec){
		x *= cVec.x; y *= cVec.y; z *= cVec.z;
		return *this;
	}

	/// Przeci��ony operator przypisania dodawania skalara. 
	/** \param[in] dValue Skalar, kt�ry b�dzie dodawany do wektora. 
	\return Zmodyfikowany wektor jako rezultat dodawania skalara. */
	inline CVector3d& operator+=(GLdouble dValue){
		x += dValue; y += dValue; z += dValue;
		return *this;
	}

	/// Przeci��ony operator przypisania odejmowania skalara. 
	/** \param[in] dValue Skalar, kt�ry b�dzie odejmowany od wektora. 
	\return Zmodyfikowany wektor jako rezultat odejmowania skalara. */
	inline CVector3d& operator-=(GLdouble dValue){
		x -= dValue; y -= dValue; z -= dValue;
		return *this;
	}

	/// Przeci��ony operator przypisania mno�enia przez skalara. 
	/** \param[in] dValue Skalar, kt�ry b�dzie mno�ony przez wektor. 
	\return Zmodyfikowany wektor jako rezultat mno�enia przez skalar. */
	inline CVector3d& operator*=(GLdouble dValue){
		x *= dValue; y *= dValue; z *= dValue;
		return *this;
	}

	/// Przeci��ony operator minus, jako zmiana zwrotu wektora na przeciwny.
	/** \return Nowy wektor z przeciwnym zwrotem. */
	inline CVector3d operator-() const{
		return CVector3d(-x, -y, -z);
	}

	/// Przeci��enie operatora plus.
	/** \return Ten sam wektor. */
	inline CVector3d operator+() const { return *this; }

	/// Przeci��ony operator przypisania wektora.
	/** \param[in] cVec Referenzja na wektor, kt�ry zostanie przypisany aktualnemu.
	\return Zmodyfikowany, aktualny wektor o przypisany. */
	inline CVector3d& operator=(const CVector3d &cVec){
		x = cVec.x; y = cVec.y; z = cVec.z;
		return *this;
	}

	/// Przeci��ony operator por�wnania wektor�w.
	/** \param[in] cVec Referencja na wektor, do kt�rego b�dzie por�wnywany aktualny.
	\return \p GL_TRUE gdy wektory s� sobie r�wne, w przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean operator==(const CVector3d &cVec) const{
		return (x == cVec.x && y == cVec.y && z == cVec.z);
	}

	/// Przeci��ony operator por�wnania wektora ze skalarem.
	/** \param[in] dValue Skalar, z kt�rym b�dzie por�wnywany wektor.
	\return \p GL_TRUE gdy wsp�rz�dne wektora s� r�wny skalarowi, w przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean operator==(GLdouble dValue) const{
		return (x == dValue && y == dValue && z == dValue);
	}

	/// Metoda obliczaj�ca d�ugo�� wektoar.
	/** \return Liczba rzeczywista okre�laj�ca d�ugo�� wektora. */
	inline GLdouble GetLength(){
		return sqrt((x * x) + (y * y) + (z * z));
	}

	GLdouble x;	///< Wsp�rz�dna x wektora.
	GLdouble y;	///< Wsp�rz�dna y wektora.
	GLdouble z;	///< Wsp�rz�dna z wektora.
};