#include "StdAfx.h"
#include "LevelsManager.h"

CLevelsManager::CLevelsManager()
{
	m_cFileName[ 0 ] = 0;
	m_pShipsManager = new CShipsManager();
	if( m_pShipsManager )
		m_pShipsManager->LoadUniqueShips( NULL );
}

CLevelsManager::~CLevelsManager()
{
	ClearLevels();
	delete m_pShipsManager;
}

void CLevelsManager::ConvertStruct()
{
	//funkcja podmieniajaca pliki ze starej struktury na nowa
	//odczytanie starej struktury
	//przepisanie starej struktury na nowa
	//zapisanie nowej struktury

	std::vector< SLevelOld > aOldLevels;
	FILE *hFile = NULL;
	SLevelOld sOldLevels;
	::wcscpy_s( m_cFileName, 255, FILE_NAME_LEVEL_EDIT2 );
	errno_t error = _wfopen_s( &hFile, m_cFileName, _T("rb") );
	if( error != 0 ) {
		wxString strMsg;
		strMsg.Printf( _T("ConvertStruct: open file error: %d"), error );
		wxMessageBox( strMsg, _T("Error"), wxOK | wxICON_ERROR );
		return;
	}
	while( fread( &sOldLevels, sizeof( SLevelOld ), 1, hFile ) != NULL ) {
		aOldLevels.push_back( sOldLevels );
		int iIndex = static_cast<int>( aOldLevels.size() ) - 1;
		if( aOldLevels[ iIndex ].iNumberGroups > 0 ) {
			aOldLevels[ iIndex ].psShipsGroups = new SShipsGroups[ aOldLevels[ iIndex ].iNumberGroups ];
			for( int i = 0; i < aOldLevels[ iIndex ].iNumberGroups; ++i ) {
				fread( &aOldLevels[ iIndex ].psShipsGroups[ i ], sizeof( SShipsGroups ), 1, hFile );
			}
		}
		else
			aOldLevels[ iIndex ].psShipsGroups = NULL;
	}
	fclose( hFile );

	ClearLevels();

	//przepisanie tablic - stara na nowa
	SLevel sLevel;
	for( int i = 0; i < static_cast<int>( aOldLevels.size() ); ++i ) {
		//////////////////////////////////////////////////////////////////////////
		///////nowe wartosci inicjalizujemy, a nie przypisujemy ze starej/////////
		//////////////////////////////////////////////////////////////////////////
		sLevel.iIndexBoss = -1; //nowe
		sLevel.iBossReserve1 = 0; //nowe
		sLevel.iBossReserve2 = 0; //nowe
		sLevel.iBossReserve3 = 0; //nowe
		//////////////////////////////////////////////////////////////////////////
		sLevel.iLevel = aOldLevels[ i ].iLevel;
		sLevel.iMaxShipsOnScreen = aOldLevels[ i ].iMaxShipsOnScreen;
		sLevel.iFrequencyShoot = aOldLevels[ i ].iFrequencyShoot;
		sLevel.iPercentChanceToShoot = aOldLevels[ i ].iPercentChanceToShoot;
		sLevel.iNumberGroups = aOldLevels[ i ].iNumberGroups;

		m_aLevels.push_back( sLevel );
		if( aOldLevels[ i ].iNumberGroups <= 0 ) {
			m_aLevels[ i ].iNumberGroups = 0;
			m_aLevels[ i ].psShipsGroups = NULL;
		}
		else {
			m_aLevels[ i ].psShipsGroups = new SShipsGroups[ m_aLevels[ i ].iNumberGroups ];
			for( int j = 0; j < aOldLevels[ i ].iNumberGroups; ++j ) {
				m_aLevels[ i ].psShipsGroups[ j ].iIndexShip = aOldLevels[ i ].psShipsGroups[ j ].iIndexShip;
				m_aLevels[ i ].psShipsGroups[ j ].iNumberShips = aOldLevels[ i ].psShipsGroups[ j ].iNumberShips;
				m_aLevels[ i ].psShipsGroups[ j ].iFormation = aOldLevels[ i ].psShipsGroups[ j ].iFormation;
				m_aLevels[ i ].psShipsGroups[ j ].iPosXForRand = aOldLevels[ i ].psShipsGroups[ j ].iPosXForRand;
				m_aLevels[ i ].psShipsGroups[ j ].fSpeed = aOldLevels[ i ].psShipsGroups[ j ].fSpeed;
				m_aLevels[ i ].psShipsGroups[ j ].bCosFly = aOldLevels[ i ].psShipsGroups[ j ].bCosFly;
				m_aLevels[ i ].psShipsGroups[ j ].bSlantFly = aOldLevels[ i ].psShipsGroups[ j ].bSlantFly;
				m_aLevels[ i ].psShipsGroups[ j ].fOffsetXPosForSlantFly = aOldLevels[ i ].psShipsGroups[ j ].fOffsetXPosForSlantFly;
			}
		}
	}

	//zapisanie
	SaveAllLevels();
}

bool CLevelsManager::LoadLevels( LPCTSTR lpFileName )
{
	ClearLevels();
	FILE *hFile;
	if( lpFileName == NULL )
		wcscpy_s( m_cFileName, 255, FILE_NAME_LEVEL_EDIT2 );
	else
		wcscpy_s( m_cFileName, 255, lpFileName );
	errno_t error = _wfopen_s( &hFile, m_cFileName, _T("rb") );
	if( error != 0 ) {
		wxString str;
		str.Printf( _T("LoadLevels - Open file: %s failed"), m_cFileName );
		wxMessageBox( str, _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	SLevel sLevel;
	while( fread( &sLevel, sizeof( SLevel ), 1, hFile ) != NULL ) {
		m_aLevels.push_back( sLevel );
		int iIndex = GetLevelsSize() - 1;
		if( m_aLevels[ iIndex ].iNumberGroups > 0 ) {
			m_aLevels[ iIndex ].psShipsGroups = new SShipsGroups[ m_aLevels[ iIndex ].iNumberGroups ];
			for( int i = 0; i < m_aLevels[ iIndex ].iNumberGroups; ++i ) {
				fread( &m_aLevels[ iIndex ].psShipsGroups[ i ], sizeof( SShipsGroups ), 1, hFile );
			}
		}
		else
			m_aLevels[ iIndex ].psShipsGroups = NULL;
	}
	fclose( hFile );
	return true;
}

void CLevelsManager::ClearLevels()
{
	for( int i = GetLevelsSize() - 1; i >= 0; --i ) {
		if( m_aLevels[ i ].psShipsGroups )
			delete [] m_aLevels[ i ].psShipsGroups;
	}
	m_aLevels.clear();
}

bool CLevelsManager::SaveLevel( const SLevel &sLevel )
{
	FILE *hFile;
	errno_t error = _wfopen_s( &hFile, m_cFileName, _T("ab") );
	if( error != 0 ) {
		wxString str;
		str.Printf( _T("SaveLevel - Open file: %s failed"), m_cFileName );
		wxMessageBox( str, _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}
	fwrite( &sLevel, sizeof( SLevel ), 1, hFile );
	for( int i = 0; i < sLevel.iNumberGroups; ++i )
		fwrite( &sLevel.psShipsGroups[ i ], sizeof( SShipsGroups ), 1, hFile );
	fclose( hFile );
	return true;
}

bool CLevelsManager::SaveAllLevels()
{
	FILE *hFile;
	errno_t error = _wfopen_s( &hFile, m_cFileName, _T("wb") );
	if( error != 0 ) {
		wxString str;
		str.Printf( _T("SaveAllLevels - Open file: %s failed"), m_cFileName );
		wxMessageBox( str, _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	for( int i = 0; i < GetLevelsSize(); ++i ) {
		fwrite( &m_aLevels[ i ], sizeof( SLevel ), 1, hFile );
		for( int j = 0; j < m_aLevels[ i ].iNumberGroups; ++j ) {
			fwrite( &m_aLevels[ i ].psShipsGroups[ j ], sizeof( SShipsGroups ), 1, hFile );
		}
	}

	fclose( hFile );

	return true;
}

bool CLevelsManager::MoveUpIndex( int iIndex )
{
	if( iIndex < 0 || iIndex >= GetLevelsSize() ) {
		wxMessageBox( _T("MoveDownIndex: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}
	//strwadz czy aktualna pozycja nie jest pierwsza
	if( iIndex == 0 ) {
		wxMessageBox( _T("This index is first"), _T("Exclamation"), wxOK | wxICON_EXCLAMATION );
		return false;
	}

	SLevel sTempLevel;
	memset( &sTempLevel, 0, sizeof( SLevel ) );
	CopyLevel( sTempLevel, m_aLevels[ iIndex ] );
	CopyLevel( m_aLevels[ iIndex ], m_aLevels[ iIndex - 1 ] );
	CopyLevel( m_aLevels[ iIndex - 1 ], sTempLevel );

	SaveAllLevels();
	return true;
}

bool CLevelsManager::MoveDownIndex( int iIndex )
{
	if( iIndex < 0 || iIndex >= GetLevelsSize() ) {
		wxMessageBox( _T("MoveDownIndex: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}
	//strwadz czy aktualna pozycja nie jest ostatnia
	if( iIndex == GetLevelsSize() - 1 ) {
		wxMessageBox( _T("This index is last"), _T("Exclamation"), wxOK | wxICON_EXCLAMATION );
		return false;
	}

	SLevel sTempLevel;
	memset( &sTempLevel, 0, sizeof( SLevel ) );
	CopyLevel( sTempLevel, m_aLevels[ iIndex ] );
	CopyLevel( m_aLevels[ iIndex ], m_aLevels[ iIndex + 1 ] );
	CopyLevel( m_aLevels[ iIndex + 1 ], sTempLevel );

	SaveAllLevels();
	return true;
}

void CLevelsManager::CopyLevel( SLevel &dest, SLevel &src )
{
	if( dest.psShipsGroups )
		delete [] dest.psShipsGroups;

	dest.iLevel = src.iLevel;
	dest.iIndexBoss = src.iIndexBoss;
	dest.iBossReserve1 = src.iBossReserve1;
	dest.iBossReserve2 = src.iBossReserve2;
	dest.iBossReserve3 = src.iBossReserve3;
	dest.iMaxShipsOnScreen = src.iMaxShipsOnScreen;
	dest.iFrequencyShoot = src.iFrequencyShoot;
	dest.iPercentChanceToShoot = src.iPercentChanceToShoot;
	dest.iNumberGroups = src.iNumberGroups;

	if( dest.iNumberGroups == 0 )
		dest.psShipsGroups = NULL;
	else {
		dest.psShipsGroups = new SShipsGroups[ dest.iNumberGroups ];
		for( int i = 0; i < dest.iNumberGroups; ++i ) {
			dest.psShipsGroups[ i ].iIndexShip = src.psShipsGroups[ i ].iIndexShip;
			dest.psShipsGroups[ i ].iNumberShips = src.psShipsGroups[ i ].iNumberShips;
			dest.psShipsGroups[ i ].iFormation = src.psShipsGroups[ i ].iFormation;
			dest.psShipsGroups[ i ].iPosXForRand = src.psShipsGroups[ i ].iPosXForRand;
			dest.psShipsGroups[ i ].fSpeed = src.psShipsGroups[ i ].fSpeed;
			dest.psShipsGroups[ i ].bCosFly = src.psShipsGroups[ i ].bCosFly;
			dest.psShipsGroups[ i ].bSlantFly = src.psShipsGroups[ i ].bSlantFly;
			dest.psShipsGroups[ i ].fOffsetXPosForSlantFly = src.psShipsGroups[ i ].fOffsetXPosForSlantFly;
		}
	}
}

bool CLevelsManager::AddNewLevel( const SLevel &sLevel )
{
	return SaveLevel( sLevel );
}

bool CLevelsManager::DeleteLevel( int iIndexLevel )
{
	if( iIndexLevel < 0 || iIndexLevel >= GetLevelsSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	if( m_aLevels[ iIndexLevel ].psShipsGroups )
		delete [] m_aLevels[ iIndexLevel ].psShipsGroups;
	m_aLevels.erase( m_aLevels.begin() + iIndexLevel );

	SaveAllLevels();
	return true;
}

bool CLevelsManager::SaveModifyLevel( int iIndexLevel, SLevel &sLevel )
{
	if( iIndexLevel < 0 || iIndexLevel >= GetLevelsSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	//ustaw dane emittera takie jak ma oryginal bo ich nie zmnieniamy tutaj
	sLevel.iNumberGroups = m_aLevels[ iIndexLevel ].iNumberGroups;
	sLevel.psShipsGroups = m_aLevels[ iIndexLevel ].psShipsGroups;

	memcpy_s( &m_aLevels[ iIndexLevel ], sizeof( SLevel ), &sLevel, sizeof( SLevel ) );

	SaveAllLevels();
	return true;
}

bool CLevelsManager::AddNewGroup( int iIndexLevel, const SShipsGroups &sNewGroup )
{
	if( iIndexLevel < 0 || iIndexLevel >= GetLevelsSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	//stworz kobie aktualnych grup
	std::vector<SShipsGroups> aCopyOfGroup;
	for( int i = 0; i < m_aLevels[ iIndexLevel ].iNumberGroups; ++i ) {
		aCopyOfGroup.push_back( m_aLevels[ iIndexLevel ].psShipsGroups[ i ] );
	}

	//usun aktualne emitery i stworz nowe
	if( m_aLevels[ iIndexLevel ].iNumberGroups > 0 && m_aLevels[ iIndexLevel ].psShipsGroups )
		delete [] m_aLevels[ iIndexLevel ].psShipsGroups;

	//zwieksz liczbe emiterow
	++m_aLevels[ iIndexLevel ].iNumberGroups;

	//utworz tablice na nowo z nowym emiterem
	m_aLevels[ iIndexLevel ].psShipsGroups = new SShipsGroups[ m_aLevels[ iIndexLevel ].iNumberGroups ];
	for( int i = 0; i < m_aLevels[ iIndexLevel ].iNumberGroups; ++i ) {
		if( i < static_cast<int>( aCopyOfGroup.size() ) )
			memcpy_s( &m_aLevels[ iIndexLevel ].psShipsGroups[ i ], sizeof( SShipsGroups ), &aCopyOfGroup[ i ], sizeof( SShipsGroups ) );
		else //dopisz nowy
			memcpy_s( &m_aLevels[ iIndexLevel ].psShipsGroups[ i ], sizeof( SShipsGroups ), &sNewGroup, sizeof( SShipsGroups ) );
	}

	aCopyOfGroup.clear();
	SaveAllLevels();
	return true;
}

bool CLevelsManager::DeleteGroup( int iIndexLevel, int iIndexGroup )
{
	if( iIndexLevel < 0 || iIndexLevel >= GetLevelsSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	if( iIndexGroup < 0 || iIndexGroup >= m_aLevels[ iIndexLevel ].iNumberGroups ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	//stworz kobie aktualnych emiterow ale bez usuwanego
	std::vector<SShipsGroups> aCopyOfGroup;
	for( int i = 0; i < m_aLevels[ iIndexLevel ].iNumberGroups; ++i ) {
		if( i == iIndexGroup )
			continue;
		aCopyOfGroup.push_back( m_aLevels[ iIndexLevel ].psShipsGroups[ i ] );
	}

	//usun aktualne emitery
	if( m_aLevels[ iIndexLevel ].iNumberGroups > 0 && m_aLevels[ iIndexLevel ].psShipsGroups )
		delete [] m_aLevels[ iIndexLevel ].psShipsGroups;

	//zmniejsz liczbe emiterow
	--m_aLevels[ iIndexLevel ].iNumberGroups;

	//utworz tablice na nowo
	m_aLevels[ iIndexLevel ].psShipsGroups = new SShipsGroups[ m_aLevels[ iIndexLevel ].iNumberGroups ];
	for( int i = 0; i < m_aLevels[ iIndexLevel ].iNumberGroups; ++i ) {
		memcpy_s( &m_aLevels[ iIndexLevel ].psShipsGroups[ i ], sizeof( SShipsGroups ), &aCopyOfGroup[ i ], sizeof( SShipsGroups ) );
	}

	aCopyOfGroup.clear();
	SaveAllLevels();
	return true;
}

bool CLevelsManager::SaveModifyGroup( int iIndexLevel, int iIndexGroup, const SShipsGroups &sModGroup )
{
	if( iIndexLevel < 0 || iIndexLevel >= GetLevelsSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	if( iIndexGroup < 0 || iIndexGroup >= m_aLevels[ iIndexLevel ].iNumberGroups ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	memcpy_s( &m_aLevels[ iIndexLevel ].psShipsGroups[ iIndexGroup], sizeof( SShipsGroups ), &sModGroup, sizeof( SShipsGroups ) );

	SaveAllLevels();
	return true;
}

///////////////////////////////////////////////////////////////////

int CLevelsManager::GetShipsNumber()
{
	return m_pShipsManager->GetUniqueShipSize();
}

TCHAR* CLevelsManager::GetShipName( int iIndex )
{
	return m_pShipsManager->GetUniqueShip( iIndex ).cShipName;
}