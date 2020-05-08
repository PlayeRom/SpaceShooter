#include "StdAfx.h"
#include "Rectangle.h"

CRectangle::CRectangle()
{
	m_cTl = CVector2();
	m_cBr = CVector2();
}

CRectangle::CRectangle( const CVector2 &in_cTl, const CVector2 &in_cBr )
{
	m_cTl = in_cTl;
	m_cBr = in_cBr;
}

CRectangle::~CRectangle()
{
}

GLboolean CRectangle::PtInRect( const CVector2 &in_cPt )
{
	if( m_cTl.y < in_cPt.y && m_cBr.y > in_cPt.y ) {
		if( m_cTl.x < in_cPt.x && m_cBr.x > in_cPt.x )
			return GL_TRUE;
	}
	return GL_FALSE;
}

GLboolean CRectangle::RectInRect( const CRectangle &in_cRec )
{
	//Funkcja sprawdza czy prostokaty nakladaja sie na siebie.
	//Bierzemy kazdy naroznik z in_cRec - czyli mamy punkt
	//i sprawdzamy metoda PtInRect, czy sie zawiera, jezeli tak to koliduja
	CVector2 cPt[ 4 ];
	//kolejnosc wierzcholkow w talicy
	//[0]----------[1]
	// |            |
	// |            |
	// |            |
	// |            |
	//[3]----------[2]
	cPt[ 0 ] = in_cRec.m_cTl;
	cPt[ 1 ] = CVector2( in_cRec.m_cBr.x, in_cRec.m_cTl.y );
	cPt[ 2 ] = in_cRec.m_cBr;
	cPt[ 3 ] = CVector2( in_cRec.m_cTl.x, in_cRec.m_cBr.y );

	for( GLint i = 0; i < 4; ++i ) {
		if( PtInRect( cPt[ i ] ) )
			return GL_TRUE;
	}

	return GL_FALSE;
}