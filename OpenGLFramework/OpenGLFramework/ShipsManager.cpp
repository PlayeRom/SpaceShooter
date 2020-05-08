
#include "StdAfx.h"
#include "ShipsManager.h"

CShipsManager::CShipsManager()
{
	if( !ReadUniqueShipsWork( FILE_NAME_SHIPS_WORK ) ) {
		if( !ReadUniqueShips( FILE_NAME_SHIPS_EDIT ) )
			::MessageBox( NULL, _T("Load dat file failed!"), GAME_NAME, MB_OK | MB_ICONERROR );
	}
}

CShipsManager::~CShipsManager()
{
	Clear();
}

GLboolean CShipsManager::ReadUniqueShips( LPCTSTR lpFile )
{
	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, lpFile, _T("rb") ) != 0 )
		return GL_FALSE;

	Clear();
	GLboolean bFirstPlayer = GL_FALSE;
	SUniqueShipWork sUniqueShipWork;
	while( fread( &sUniqueShipWork.sUniqueShips, sizeof( SUniqueShip ), 1, hFile ) != NULL ) {
		m_aUniqueShipsWork.push_back( sUniqueShipWork );
		GLint iIndex = GetUniqueShipsSize() - 1;

		//na starcie standardowo statek niedostepny w przegladarce
		m_aUniqueShipsWork[ iIndex ].bAvailableInBrowse = GL_FALSE;

		//pierwszy gracz "needle" musi byc na stracie dostepny w przegladarce
		if( m_aUniqueShipsWork[ iIndex ].sUniqueShips.bPlayer && !bFirstPlayer ) {
			bFirstPlayer = GL_TRUE;
			m_aUniqueShipsWork[ iIndex ].bAvailableInBrowse = GL_TRUE;
		}

		//odczytanie emitera
		if( sUniqueShipWork.sUniqueShips.iEmitterNumber == 0 )
			m_aUniqueShipsWork[ iIndex ].sUniqueShips.psEmitterData = NULL;
		else {
			m_aUniqueShipsWork[ iIndex ].sUniqueShips.psEmitterData = new SEmitterData[ m_aUniqueShipsWork[ iIndex ].sUniqueShips.iEmitterNumber ];
			for( GLint i = 0; i < m_aUniqueShipsWork[ iIndex ].sUniqueShips.iEmitterNumber; ++i )
				fread( &m_aUniqueShipsWork[ iIndex ].sUniqueShips.psEmitterData[ i ], sizeof( SEmitterData ), 1, hFile );
		}
	}
	fclose( hFile );
	return GL_TRUE;
}

GLboolean CShipsManager::ReadUniqueShipsWork( LPCTSTR lpFile )
{
	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, lpFile, _T("rb") ) != 0 )
		return GL_FALSE;

	Clear();
	SUniqueShipWork sUniqueShipWork;
	while( fread( &sUniqueShipWork.sUniqueShips, sizeof( SUniqueShip ), 1, hFile ) != NULL ) {
		m_aUniqueShipsWork.push_back( sUniqueShipWork );
		GLint iIndex = GetUniqueShipsSize() - 1;

		//odczyt bajta mowiacego czy statek jest dostepny w przegladarce
		fread( &m_aUniqueShipsWork[ iIndex ].bAvailableInBrowse, sizeof( unsigned char ), 1, hFile );

		if( sUniqueShipWork.sUniqueShips.iEmitterNumber == 0 )
			m_aUniqueShipsWork[ iIndex ].sUniqueShips.psEmitterData = NULL;
		else {
			m_aUniqueShipsWork[ iIndex ].sUniqueShips.psEmitterData = new SEmitterData[ m_aUniqueShipsWork[ iIndex ].sUniqueShips.iEmitterNumber ];
			for( GLint i = 0; i < m_aUniqueShipsWork[ iIndex ].sUniqueShips.iEmitterNumber; ++i )
				fread( &m_aUniqueShipsWork[ iIndex ].sUniqueShips.psEmitterData[ i ], sizeof( SEmitterData ), 1, hFile );
		}
	}
	fclose( hFile );
	return GL_TRUE;
}

GLvoid CShipsManager::SaveUniqueShipsWork()
{
	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, FILE_NAME_SHIPS_WORK, _T("wb") ) != 0 )
		return;

	for( GLint i = 0; i < GetUniqueShipsSize(); ++i ) {
		fwrite( &m_aUniqueShipsWork[ i ].sUniqueShips, sizeof( SUniqueShip ), 1, hFile );
		//zapis bajta mowiacego czy statek jest dostepny w przegladarce
		fwrite( &m_aUniqueShipsWork[ i ].bAvailableInBrowse, sizeof( unsigned char ), 1, hFile );

		for( GLint j = 0; j < m_aUniqueShipsWork[ i ].sUniqueShips.iEmitterNumber; ++j )
			fwrite( &m_aUniqueShipsWork[ i ].sUniqueShips.psEmitterData[ j ], sizeof( SEmitterData ), 1, hFile );
	}
	fclose( hFile );
}

GLvoid CShipsManager::Clear()
{
	for( GLint i = GetUniqueShipsSize() - 1; i >= 0; --i ) {
		if( m_aUniqueShipsWork[ i ].sUniqueShips.psEmitterData )
			delete [] m_aUniqueShipsWork[ i ].sUniqueShips.psEmitterData;
	}
	m_aUniqueShipsWork.clear();
}

GLint CShipsManager::GetNextIndex( GLint iIndex, GLboolean bPlayerShip )
{
	for( GLint i = iIndex + 1; i < GetUniqueShipsSize(); ++i ) {
		if( bPlayerShip == m_aUniqueShipsWork[ i ].sUniqueShips.bPlayer
			&& ( m_aUniqueShipsWork[ i ].sUniqueShips.iStarsArmament > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsArmour > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsManoeuvring > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsShield > 0 ) )
		{
			return i;
		}
	}

	//nie zalazl wiec, znajdz pierwszy lepszy
	for( GLint i = 0; i < GetUniqueShipsSize(); ++i ) {
		if( bPlayerShip == m_aUniqueShipsWork[ i ].sUniqueShips.bPlayer
			&& ( m_aUniqueShipsWork[ i ].sUniqueShips.iStarsArmament > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsArmour > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsManoeuvring > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsShield > 0 ) )
		{
			return i;
		}
	}

	//nie znalazl nastepenego, wiec podaj pierwszy
	return 0;
}

GLint CShipsManager::GetPreviousIndex( GLint iIndex, GLboolean bPlayerShip )
{
	GLint i = iIndex - 1;
	if( i < 0 )
		i = GetUniqueShipsSize() - 1;

	for( ; i >= 0; --i ) {
		if( bPlayerShip == m_aUniqueShipsWork[ i ].sUniqueShips.bPlayer
			&& ( m_aUniqueShipsWork[ i ].sUniqueShips.iStarsArmament > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsArmour > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsManoeuvring > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsShield > 0 ) )
		{
			return i;
		}
	}

	//nie zalazl wiec, znajdz pierwszy lepszy
	for( GLint i = GetUniqueShipsSize() - 1; i >= 0; --i ) {
		if( bPlayerShip == m_aUniqueShipsWork[ i ].sUniqueShips.bPlayer
			&& ( m_aUniqueShipsWork[ i ].sUniqueShips.iStarsArmament > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsArmour > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsManoeuvring > 0
			|| m_aUniqueShipsWork[ i ].sUniqueShips.iStarsShield > 0 ) )
		{
			return i;
		}
	}

	return 0;
}

GLuint CShipsManager::GetPointsForShootDown( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return 0;

	return static_cast< GLuint >( m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsShield
								+ m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsArmour
								+ m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsArmament
								+ m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsManoeuvring ) * 5;
}

GLboolean CShipsManager::IsPlayer( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return GL_FALSE;

	return m_aUniqueShipsWork[ iIndex ].sUniqueShips.bPlayer;
}

//ilosc punktow
GLint CShipsManager::GetShieldPoints( GLint iIndex, GLint iStarsShield /*= -1*/ )
{
	if( iStarsShield > -1 )
		return iStarsShield * 70 + 20;

	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return -1;

	return m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsShield * 70 + 20;
}

GLint CShipsManager::GetArmourPoints( GLint iIndex, GLint iStarsArmour /*= -1*/ )
{
	if( iStarsArmour > -1 )
		return iStarsArmour * 50 + 10;

	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return -1;

	return m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsArmour * 50 + 10;
}

GLint CShipsManager::GetArmamentPoints( GLint iIndex, GLint iStarsArmament /*= -1*/ )
{
	if( iStarsArmament > -1 )
		return iStarsArmament * 60 + 15;

	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return -1;

	return m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsArmament * 60 + 15;
}

GLint CShipsManager::GetManoeuvringPercent( GLint iIndex, GLint iStarsManoeuvring /*= -1*/ )
{
	if( iStarsManoeuvring > -1 )
		return iStarsManoeuvring * 15 + 20;

	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return -1;

	return m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsManoeuvring * 15 + 20;
}

GLint CShipsManager::GetEnemyShipNumber()
{
	//zwraca ilosc samych statków wroga - czyli bez asteroidów itp.
	GLint iCounter = 0;
	for( GLint i = 0; i < GetUniqueShipsSize(); ++i ) {
		if( !m_aUniqueShipsWork[ i ].sUniqueShips.bPlayer ) {
			//czy moze strzelac i manewrowac - czyli jest statkiem
			if( m_aUniqueShipsWork[ i ].sUniqueShips.bCanFire && m_aUniqueShipsWork[ i ].sUniqueShips.bCanManoeuvring )
				++iCounter;
		}
	}

	return iCounter;
}

GLint CShipsManager::GetPlayerShipNumber()
{
	//zwraca ilosc statków gracza
	GLint iCounter = 0;
	for( GLint i = 0; i < GetUniqueShipsSize(); ++i ) {
		if( m_aUniqueShipsWork[ i ].sUniqueShips.bPlayer )
			++iCounter;
	}

	return iCounter;
}

GLint CShipsManager::GetRandomEnemyShipIndex()
{
	std::vector<GLint> aIndexArray;
	for( GLint i = 0; i < GetUniqueShipsSize(); ++i ) {
		if( !m_aUniqueShipsWork[ i ].sUniqueShips.bPlayer ) {
			//czy moze strzelac i manewrowac - czyli jest statkiem
			if( m_aUniqueShipsWork[ i ].sUniqueShips.bCanFire && m_aUniqueShipsWork[ i ].sUniqueShips.bCanManoeuvring )
				aIndexArray.push_back( i );
		}
	}

	GLint iRand = aIndexArray[ rand() % aIndexArray.size() ]; //losuj index tablicy
	aIndexArray.clear();
	
	return iRand;
}

GLboolean CShipsManager::IsAvailableInBrowse( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return 0;

	return m_aUniqueShipsWork[ iIndex ].bAvailableInBrowse;
}

GLvoid CShipsManager::SetAvailableInBrowse( GLint iIndex, GLboolean bAvailable )
{
	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return;

	m_aUniqueShipsWork[ iIndex ].bAvailableInBrowse = bAvailable;
}

GLint CShipsManager::GetStarsShield( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return -1;

	return m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsShield;
}

GLint CShipsManager::GetStarsArmour( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return -1;

	return m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsArmour;
}

GLint CShipsManager::GetStarsArmament( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return -1;

	return m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsArmament;
}

GLint CShipsManager::GetStarsManoeuvring( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetUniqueShipsSize() )
		return -1;

	return m_aUniqueShipsWork[ iIndex ].sUniqueShips.iStarsManoeuvring;
}
