
#include "StdAfx.h"
#include "LevelsManager.h"

CLevelsManager::CLevelsManager()
{
	if( !ReadLevels( FILE_NAME_LEVEL_EDIT ) )
		::MessageBox( NULL, _T("Load file with levels - failed!"), GAME_NAME, MB_OK | MB_ICONERROR );
}

CLevelsManager::~CLevelsManager()
{
	Clear();
}

GLboolean CLevelsManager::ReadLevels( LPCTSTR lpFileName )
{
	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, lpFileName, _T("rb") ) != 0 )
		return GL_FALSE;

	Clear();
	SLevel sLevel;
	while( fread( &sLevel, sizeof( SLevel ), 1, hFile ) != NULL ) {
		m_aLevels.push_back( sLevel );
		GLint iIndex = GetLevelsSize() - 1;

		//odczytanie grup
		if( m_aLevels[ iIndex ].iNumberGroups <= 0 )
			//sytuacja niedopuszczalna - zawsze musi byc conajmniej jedna grupa statkow
			m_aLevels[ iIndex ].psShipsGroups = NULL;
		else {
			m_aLevels[ iIndex ].psShipsGroups = new SShipsGroups[ m_aLevels[ iIndex ].iNumberGroups ];
			for( GLint i = 0; i < m_aLevels[ iIndex ].iNumberGroups; ++i )
				fread( &m_aLevels[ iIndex ].psShipsGroups[ i ], sizeof( SShipsGroups ), 1, hFile );
		}
	}
	fclose( hFile );
	return GL_TRUE;
}

GLvoid CLevelsManager::Clear()
{
	for( GLint i = GetLevelsSize() - 1; i >= 0; --i ) {
		if( m_aLevels[ i ].psShipsGroups )
			delete [] m_aLevels[ i ].psShipsGroups;
	}
	m_aLevels.clear();
}

GLint CLevelsManager::GetIndexLevel( GLint iLevel )
{
	//znajdz index z LevelsManagera odpowiadajacy iLevel
	for( GLint i = 0; i < GetLevelsSize(); ++i ) {
		if( iLevel == m_aLevels[ i ].iLevel )
			return i;
	}
	return 0;
}