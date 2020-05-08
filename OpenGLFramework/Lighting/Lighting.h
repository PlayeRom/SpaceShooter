// Lighting.h: interface for the CLighting class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CVector4;

/*
�wiat�o otoczenia to �wiat�o, kt�re nie pochodzi z
�adnego okre�lonego kierunku i o�wietla wszystkie obiekty w twojej prezentacji.
Drugi nazywa si� �wiat�em rozproszonym. Takie �wiat�o ma swoje �r�d�o i odbija si�
od powierzchni obiekt�w. Ka�da powierzchnia, na kt�r� �wiat�o pada bezpo�rednio
b�dzie bardzo jasna, za� obszary, do kt�rych �wiat�o ledwo dochodzi b�d� ciemniejsze.
Dzi�ki temu uzyskamy �adny efekt cieniowania na naszej skrzynce.
Gdyby nie o�wietlenie otoczenia, to miejsca, do kt�rych nie dochodzi �wiat�o
rozproszone wydawa�yby si� bardzo ciemne.
*/

class CLighting  
{
public:
	enum ELightMode
	{
		EEmission = GL_EMISSION,
		EAmbient = GL_AMBIENT,
		EDiffuse = GL_DIFFUSE,
		ESpecular = GL_SPECULAR,
		EAmbientAndDiffuse = GL_AMBIENT_AND_DIFFUSE
	};

	CLighting();
	virtual ~CLighting();

	//ustaw swiatlo otoczenia
	GLvoid SetAmbient( GLuint uiLight, GLfloat r = 0.5f, GLfloat g = 0.5f, GLfloat b = 0.5f, GLfloat a = 1.0f );
	//ustaw swiatlo rozproszone
	GLvoid SetDiffuse( GLuint uiLight, GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f, GLfloat a = 1.0f );
	// ustaw swiatlo odblyskow
	GLvoid SetSpecular( GLuint uiLight, GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 1.0f );

	GLvoid SetColorMaterial( ELightMode eMode, GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f );
	GLvoid SetColorMaterial( ELightMode eMode, const CVector4 &in_cColor );
	GLvoid SetColorMaterial( ELightMode eMode );
	GLvoid DisableColorMaterial();

	GLvoid SetPosition( GLuint uiLight, GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 1.0f, GLfloat w = 1.0f );
	GLvoid SetPosition( GLuint uiLight, const CVector4 &in_cPos );
	CVector4 GetPosition( GLuint uiLight );
	
	//matrial
	GLvoid SetMatAmbient( GLfloat r = 0.2f, GLfloat g = 0.2f, GLfloat b = 0.2f, GLfloat a = 1.0f );
	GLvoid SetMatDiffuse( GLfloat r = 0.8f, GLfloat g = 0.8f, GLfloat b = 0.8f, GLfloat a = 1.0f );
	GLvoid SetMatSpecular( GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 1.0f );
	GLvoid SetMatEmission( GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 1.0f );
	GLvoid SetMatShinness( GLfloat s = 50.0f );

	GLvoid DrawPositionLight( GLuint uiLight, GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f );

	GLint GetIndex( GLuint uiLight );

private:
	struct SLighting
	{
		GLfloat fLightAmbient[ 4 ];		// Warto�ci o�wietlenia otoczenia
		GLfloat fLightDiffuse[ 4 ];		// Warto�ci �wiat�a rozproszonego (zarowka)
		GLfloat fLightSpecular[ 4 ];	// Warto�� �wiat�� odblask�w
		CVector4 cLightPosition;		// Pozycja �wiat�a
	};

	SLighting m_sLight[ 8 ];

	GLfloat m_fMatAmbient[ 4 ];		// definiuje kolor i nat�enie odbicia �wiat�a otaczaj�cego
	GLfloat m_fMatDiffuse[ 4 ];		// definiuje kolor i nat�enie odbicia �wiat�a rozproszonego 
	GLfloat m_fMatSpecular[ 4 ];	// rozpraszanie swiat�a
	GLfloat m_fMatEmission[ 4 ];	// definiuje stopie� i kolor emisji �wiat�a. Parametr ten s�u�y jedynie nadaniu obiektowi takiego koloru, jakby sam by� �r�d�em �wiat�a.
	GLfloat m_fMatShininess;		// po�ysk, range [0,128] 

	GLfloat m_fArmLength;
	GLfloat m_fArmLength2;

	CVector3* m_aIndices;
};