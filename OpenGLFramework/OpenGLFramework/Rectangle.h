#pragma once

class CRectangle
{
public:
	CRectangle();
	CRectangle(const CVector2 &in_cTl, const CVector2 &in_cBr);
	~CRectangle();

	GLboolean PtInRect(const CVector2 &in_cPt);
	GLboolean RectInRect(const CRectangle &in_cRec);

	CVector2 m_cTl; //naroznik, gorny-lewy
	CVector2 m_cBr; //naroznik, dolny-prawy
};
