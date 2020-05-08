#pragma once

class CRWeaponSelect : public CRMenuBase
{
public:
	CRWeaponSelect();
	~CRWeaponSelect();

	GLvoid DoDraw(GLint iItem);
	GLvoid Draw();
	GLvoid RestartObjects();

private:
	GLboolean m_bDraw; //czy moze sie rysowac
	GLint m_iItem; //ktora bron wybrano od 0 do 8, gdzie 0 do klawisz 1, itd. czyli [klawisz - 1]

	GLvoid SetRegions();
	RECT m_rRegions[3];
	GLfloat m_fFade;

	GLvoid DrawQuad(GLint iRectIndex, GLint iTexIndex, GLboolean bSelectedColor = GL_FALSE);
};
