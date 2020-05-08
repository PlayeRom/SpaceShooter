/*! \file
	\brief Plik nag��wkowy dla klasy CMatrixOperations.
*/
#pragma once

class CVector2;
class CVector3;

/// Klasa wspieraj�ca obliczenia dla przekszta�ce� dwu i trzywymiarowych.
class CMatrixOperations
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CMatrixOperations();

	/// Destruktor.
	/** Destruktor */
	virtual ~CMatrixOperations();

	/// Metoda przesuwaj�ca wierzcho�ek 3D.
	/** \param[in] in_cVecActual Referencja na wektor, kt�ry chcemy przesun��. 
	\param[in] in_cVecTrans Referencja na wektor przesuni�cia.
	\return Nowy, przesuni�ty wektor. */
	CVector3 Translate3(const CVector3 &in_cVecActual, const CVector3 &in_cVecTrans);

	/// Metoda przesuwaj�ca wierzcho�ek 3D po osi X.
	/** \param[in] in_cVecActual Referencja na wektor, kt�rego wsp�rz�dn� X chcemy przesun��. 
	\param[in] fTransX Skalar przesuni�cia po osi X.
	\return Nowy, przesuni�ty wektor po osi X. */
	CVector3 TranslateX3(const CVector3 &in_cVecActual, GLfloat fTransX);

	/// Metoda przesuwaj�ca wierzcho�ek 3D po osi Y.
	/** \param[in] in_cVecActual Referencja na wektor, kt�rego wsp�rz�dn� Y chcemy przesun��. 
	\param[in] fTransY Skalar przesuni�cia po osi Y.
	\return Nowy, przesuni�ty wektor po osi Y. */
	CVector3 TranslateY3(const CVector3 &in_cVecActual, GLfloat fTransY);

	/// Metoda przesuwaj�ca wierzcho�ek 3D po osi Z.
	/** \param[in] in_cVecActual Referencja na wektor, kt�rego wsp�rz�dn� Z chcemy przesun��. 
	\param[in] fTransZ Skalar przesuni�cia po osi Z.
	\return Nowy, przesuni�ty wektor po osi Z. */
	CVector3 TranslateZ3(const CVector3 &in_cVecActual, GLfloat fTransZ);

	/// Metoda skaluj�ca wierzcho�ek 3D.
	/** \param[in] in_cVecActual Referencja na wektor, kt�ry chcemy przeskalowa�.
	\param[in] in_cVecScale Referencja na wektor skalowania.
	\return Nowy, przeskalowany wektor. */
	CVector3 Scale3(const CVector3 &in_cVecActual, const CVector3 &in_cVecScale);

	/// Metoda obracaj�ca wierzcho�ek 3D po osi X.
	/** \param[in] in_cVecActual Referencja na wektor, kt�ry chcemy obr�ci� po osi X.
	\param[in] fAngle K�t o jaki chcemy obr�ci� wektor po osi X.
	\return Nowy, obr�cony wektor po osi X.	*/
	CVector3 RotateX3(const CVector3 &in_cVecActual, GLfloat fAngle);

	/// Metoda obracaj�ca wierzcho�ek 3D po osi Y.
	/** \param[in] in_cVecActual Referencja na wektor, kt�ry chcemy obr�ci� po osi Y.
	\param[in] fAngle K�t o jaki chcemy obr�ci� wektor po osi Y.
	\return Nowy, obr�cony wektor po osi Y.	*/
	CVector3 RotateY3(const CVector3 &in_cVecActual, GLfloat fAngle);

	/// Metoda obracaj�ca wierzcho�ek 3D po osi Z.
	/** \param[in] in_cVecActual Referencja na wektor, kt�ry chcemy obr�ci� po osi Z.
	\param[in] fAngle K�t o jaki chcemy obr�ci� wektor po osi Z.
	\return Nowy, obr�cony wektor po osi Z.	*/
	CVector3 RotateZ3(const CVector3 &in_cVecActual, GLfloat fAngle);

	/// Metoda obracaj�ca wierzcho�ek 3D.
	/** \param[in] in_cVecActual Referencja na wektor, kt�ry chcemy obr�ci�.
	\param[in] in_cVecAngle Referencja na wektor, kt�ych przechowuje k�ty obrotu odpowiednich osi.
	\return Nowy, obr�cony wektor. */
	CVector3 RotateXYZ3(const CVector3 &in_cVecActual, const CVector3 &in_cVecAngle);

	/// Metoda obracaj�ca wierzcho�ek 3D.
	/** \param[in] in_out_cVecActual Referencja na wektor, kt�ry chcemy obr�ci� i w kt�rym zostanie zwr�cony rezultat
	obrotu.
	\param[in] in_cVecAngle Referencja na wektor, kt�ych przechowuje k�ty obrotu odpowiednich osi. */
	GLvoid RotateXYZ3_ref(CVector3 &in_out_cVecActual, const CVector3 &in_cVecAngle);

	/////////////////////////// 2D ///////////////////////////////////

	/// Metoda przesuwaj�ca wierzcho�ek 2D.
	/** \param[in] in_cVecActual Referencja na wektor, kt�ry chcemy przesun��. 
	\param[in] in_cVecTrans Referencja na wektor przesuni�cia.
	\return Nowy, przesuni�ty wektor. */
	CVector2 Translate2(const CVector2 &in_cVecActual, const CVector2 &in_cVecTrans);

	/// Metoda skaluj�ca wierzcho�ek 2D.
	/** \param[in] in_cVecActual Referencja na wektor, kt�ry chcemy przeskalowa�.
	\param[in] in_cVecScale Referencja na wektor skalowania.
	\return Nowy, przeskalowany wektor. */
	CVector2 Scale2(const CVector2 &in_cVecActual, const CVector2 &in_cVecScale);

	/// Metoda obracaj�ca wierzcho�ek 2D po osi X.
	/** \param[in] in_cVecActual Referencja na wektor, kt�ry chcemy obr�ci� po osi X.
	\param[in] fAngle K�t o jaki chcemy obr�ci� wektor po osi X.
	\return Nowy, obr�cony wektor po osi X.	*/
	CVector2 RotateX2(const CVector2 &in_cVecActual, GLfloat fAngle);

	/// Metoda obracaj�ca wierzcho�ek 2D po osi Y.
	/** \param[in] in_cVecActual Referencja na wektor, kt�ry chcemy obr�ci� po osi Y.
	\param[in] fAngle K�t o jaki chcemy obr�ci� wektor po osi Y.
	\return Nowy, obr�cony wektor po osi Y.	*/
	CVector2 RotateY2(const CVector2 &in_cVecActual, GLfloat fAngle);

	/// Metoda obracaj�ca wierzcho�ek 2D po osi Z.
	/** \param[in] in_cVecActual Referencja na wektor, kt�ry chcemy obr�ci� po osi Z.
	\param[in] fAngle K�t o jaki chcemy obr�ci� wektor po osi Z.
	\return Nowy, obr�cony wektor po osi Z.	*/
	CVector2 RotateZ2(const CVector2 &in_cVecActual, GLfloat fAngle);
};
