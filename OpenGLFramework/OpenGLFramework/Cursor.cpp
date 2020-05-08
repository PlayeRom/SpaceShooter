// Cursor.cpp: implementation of the CCursor class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "WindowData.h"
#include "Cursor.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCursor::CCursor()
{
	m_hCursor[ 0 ] = ::LoadCursor( CWindowData::GetInstance()->GetHINSTANCE(), MAKEINTRESOURCE( IDC_CURSOR1 ) );	// nie u¿ywane
	m_hCursor[ 1 ] = ::LoadCursor( CWindowData::GetInstance()->GetHINSTANCE(), MAKEINTRESOURCE( IDC_CURSOR2 ) );	// menu
	m_hCursor[ 2 ] = ::LoadCursor( CWindowData::GetInstance()->GetHINSTANCE(), MAKEINTRESOURCE( IDC_CURSOR3 ) );	// ukryty - gramy
	m_hCursor[ 3 ] = ::LoadCursor( CWindowData::GetInstance()->GetHINSTANCE(), MAKEINTRESOURCE( IDC_CURSOR4 ) );	// obracanie statkiem w przegl¹darce

	SetMyCursor( EMenu );
}

CCursor::~CCursor()
{
	for( GLint i = ELastElem - 1; i >= 0; --i )
		::DestroyCursor( m_hCursor[ i ] );
}

GLvoid CCursor::SetMyCursor( ECursor eIndex )
{
	if( eIndex < ENone || eIndex >= ELastElem )
		return;

	m_eCursor = eIndex;

	::SetCursor( m_hCursor[ eIndex ] );
}
