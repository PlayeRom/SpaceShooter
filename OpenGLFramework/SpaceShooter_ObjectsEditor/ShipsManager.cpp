#include "StdAfx.h"
#include "ShipsManager.h"

CShipsManager::CShipsManager()
{
	m_cFileName[ 0 ] = 0;
}

CShipsManager::~CShipsManager()
{
	ClearUniqueShips();
}

void CShipsManager::ConvertStruct()
{
	//funkcja podmieniajaca pliki ze starej struktury na nowa
	//odczytanie starej struktury
	//przepisanie starej struktury na nowa
	//zapisanie nowej struktury

	std::vector<SOldUniqueShip> aOldUniqueShip;
	FILE *hFile;
	SOldUniqueShip sOldUniqueShip;
	errno_t error = _wfopen_s( &hFile, m_cFileName, _T("rb") );
	if( error != 0 ) {
		wxString strMsg;
		strMsg.Printf( _T("ConvertStruct: open file error: %d"), error );
		wxMessageBox( strMsg, _T("Error"), wxOK | wxICON_ERROR );
		return;
	}
	while( fread( &sOldUniqueShip, sizeof( SOldUniqueShip ), 1, hFile ) != NULL ) {
		aOldUniqueShip.push_back( sOldUniqueShip );
		int iIndex = static_cast<int>( aOldUniqueShip.size() ) - 1;
		if( aOldUniqueShip[ iIndex ].iEmitterNumber > 0 ) {
			aOldUniqueShip[ iIndex ].psEmitterData = new SEmitterData[aOldUniqueShip[ iIndex ].iEmitterNumber ];
			for( int i = 0; i < aOldUniqueShip[ iIndex ].iEmitterNumber; ++i ) {
				fread( &aOldUniqueShip[ iIndex ].psEmitterData[ i ], sizeof( SEmitterData), 1, hFile );
			}
		}
		else
			aOldUniqueShip[ iIndex ].psEmitterData = NULL;
	}
	fclose( hFile );

	ClearUniqueShips();

	//przepisanie tablic - stara na nowa
	SUniqueShip sUniqueShip;
	for( int i = 0; i < static_cast<int>( aOldUniqueShip.size() ); ++i ) {
		//////////////////////////////////////////////////////////////////////////
		///////nowe wartosci inicjalizujemy, a nie przypisujemy ze starej/////////
		//////////////////////////////////////////////////////////////////////////
		sUniqueShip.byTexType = 0; //nowe
		sUniqueShip.uiCOLORREF = 0; //nowe
		//////////////////////////////////////////////////////////////////////////
		sUniqueShip.bCanFire = aOldUniqueShip[ i ].bCanFire;
		sUniqueShip.bCanManoeuvring = aOldUniqueShip[ i ].bCanManoeuvring;
		sUniqueShip.bPlayer = aOldUniqueShip[ i ].bPlayer;
		wcscpy_s( sUniqueShip.cObjectName, SIZE_STRING, aOldUniqueShip[ i ].cObjectName );
		wcscpy_s( sUniqueShip.cShipName, SIZE_STRING, aOldUniqueShip[ i ].cShipName );
		wcscpy_s( sUniqueShip.cTexBumpName, SIZE_STRING, aOldUniqueShip[ i ].cTexBumpName );
		wcscpy_s( sUniqueShip.cTexLife, SIZE_STRING, aOldUniqueShip[ i ].cTexLife );
		wcscpy_s( sUniqueShip.cTexLifeMask, SIZE_STRING, aOldUniqueShip[ i ].cTexLifeMask );
		sUniqueShip.iTexLife = aOldUniqueShip[ i ].iTexLife;
		wcscpy_s( sUniqueShip.cTexName, SIZE_STRING, aOldUniqueShip[ i ].cTexName );
		sUniqueShip.fCollisionA = aOldUniqueShip[ i ].fCollisionA;
		sUniqueShip.fCollisionB = aOldUniqueShip[ i ].fCollisionB;
		sUniqueShip.i3DObjIndex = aOldUniqueShip[ i ].i3DObjIndex;
		sUniqueShip.iTexIndex = aOldUniqueShip[ i ].iTexIndex;
		sUniqueShip.iTexBumpIndex = aOldUniqueShip[ i ].iTexBumpIndex;
		sUniqueShip.iStarsShield = aOldUniqueShip[ i ].iStarsShield;
		sUniqueShip.iStarsArmour = aOldUniqueShip[ i ].iStarsArmour;
		sUniqueShip.iStarsArmament = aOldUniqueShip[ i ].iStarsArmament;
		sUniqueShip.iStarsManoeuvring = aOldUniqueShip[ i ].iStarsManoeuvring;
		sUniqueShip.iEmitterNumber = aOldUniqueShip[ i ].iEmitterNumber;

		m_aUniqueShip.push_back( sUniqueShip );
		if( aOldUniqueShip[ i ].iEmitterNumber <= 0 ) {
			m_aUniqueShip[ i ].iEmitterNumber = 0;
			m_aUniqueShip[ i ].psEmitterData = NULL;
		}
		else {
			m_aUniqueShip[ i ].psEmitterData = new SEmitterData[ m_aUniqueShip[ i ].iEmitterNumber ];
			for( int j = 0; j < aOldUniqueShip[ i ].iEmitterNumber; ++j ) {
				m_aUniqueShip[ i ].psEmitterData[ j ].iIndex = aOldUniqueShip[ i ].psEmitterData[ j ].iIndex;
				m_aUniqueShip[ i ].psEmitterData[ j ].fEmitterPosX = aOldUniqueShip[ i ].psEmitterData[ j ].fEmitterPosX;
				m_aUniqueShip[ i ].psEmitterData[ j ].fEmitterPosY = aOldUniqueShip[ i ].psEmitterData[ j ].fEmitterPosY;
				m_aUniqueShip[ i ].psEmitterData[ j ].fEmitterPosZ = aOldUniqueShip[ i ].psEmitterData[ j ].fEmitterPosZ;
				m_aUniqueShip[ i ].psEmitterData[ j ].iTexEmitter = aOldUniqueShip[ i ].psEmitterData[ j ].iTexEmitter;
				m_aUniqueShip[ i ].psEmitterData[ j ].fR = aOldUniqueShip[ i ].psEmitterData[ j ].fR;
				m_aUniqueShip[ i ].psEmitterData[ j ].fG = aOldUniqueShip[ i ].psEmitterData[ j ].fG;
				m_aUniqueShip[ i ].psEmitterData[ j ].fB = aOldUniqueShip[ i ].psEmitterData[ j ].fB;
				m_aUniqueShip[ i ].psEmitterData[ j ].iParticlesNumber = aOldUniqueShip[ i ].psEmitterData[ j ].iParticlesNumber;
			}
		}
	}

	//zapisanie
	SaveUniqueShips();
}

bool CShipsManager::LoadUniqueShips( LPCTSTR lpFileName )
{
	ClearUniqueShips();
	FILE *hFile;
	SUniqueShip sUniqueShip;
	if( lpFileName == NULL )
		wcscpy_s( m_cFileName, 256, FILE_NAME_SHIPS_EDIT2 );
	else
		wcscpy_s( m_cFileName, 256, lpFileName );
	errno_t error = _wfopen_s( &hFile, m_cFileName, _T("rb") );
	if( error != 0 ) {
		wxString strMsg;
		strMsg.Printf( _T("LoadUniqueShips: open file error: %d"), error );
		wxMessageBox( strMsg, _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}
	while( fread( &sUniqueShip, sizeof( SUniqueShip ), 1, hFile ) != NULL ) {
		m_aUniqueShip.push_back( sUniqueShip );
		int iIndex = GetUniqueShipSize() - 1;
		if( m_aUniqueShip[ iIndex ].iEmitterNumber > 0 ) {
			m_aUniqueShip[ iIndex ].psEmitterData = new SEmitterData[ m_aUniqueShip[ iIndex ].iEmitterNumber ];
			for( int i = 0; i < m_aUniqueShip[ iIndex ].iEmitterNumber; ++i ) {
				fread( &m_aUniqueShip[ iIndex ].psEmitterData[ i ], sizeof( SEmitterData), 1, hFile );
			}
		}
		else
			m_aUniqueShip[ iIndex ].psEmitterData = NULL;
	}
	fclose( hFile );
	return true;
}

//nadpisuje plik
bool CShipsManager::SaveUniqueShips()
{
	FILE *hFile;
	errno_t error = _wfopen_s( &hFile, m_cFileName, _T("wb") );
	if( error != 0 ) {
		wxString strMsg;
		strMsg.Printf( _T("SaveUniqueShips: open file error: %d"), error );
		wxMessageBox( strMsg, _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	for( int i = 0; i < GetUniqueShipSize(); ++i ) {
		fwrite( &m_aUniqueShip[ i ], sizeof( SUniqueShip ), 1, hFile );
		for( int j = 0; j < m_aUniqueShip[ i ].iEmitterNumber; ++j ) {
			fwrite( &m_aUniqueShip[ i ].psEmitterData[ j ], sizeof( SEmitterData), 1, hFile );
		}
	}

	fclose( hFile );

	return true;
}

//funkcja dopisuje jeden statek
bool CShipsManager::SaveUniqueShip( const SUniqueShip &sUniqueShip )
{
	FILE *hFile;
	errno_t error = _wfopen_s( &hFile, m_cFileName, _T("ab") );
	if( error != 0 ) {
		wxString strMsg;
		strMsg.Printf( _T("SaveUniqueShip: open file error: %d"), error );
		wxMessageBox( strMsg, _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}
	fwrite( &sUniqueShip, sizeof( SUniqueShip ), 1, hFile );
	for( int i = 0; i < sUniqueShip.iEmitterNumber; ++i )
		fwrite( &sUniqueShip.psEmitterData[ i ], sizeof( SEmitterData), 1, hFile );
	fclose( hFile );
	return true;
}

void CShipsManager::ClearUniqueShips()
{
	for( int i = GetUniqueShipSize() - 1; i >= 0; --i ) {
		if( m_aUniqueShip[ i ].psEmitterData)
			delete [] m_aUniqueShip[ i ].psEmitterData;
	}
	m_aUniqueShip.clear();
}

bool CShipsManager::MoveUpIndex( int iIndex )
{
	if( iIndex < 0 || iIndex >= GetUniqueShipSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}
	//strwadz czy aktualna pozycja nie jest ostatnia
	if( iIndex == 0 ) {
		wxMessageBox( _T("This index is first"), _T("Exclamation"), wxOK | wxICON_EXCLAMATION );
		return false;
	}

	SUniqueShip sTempShip;
	memset( &sTempShip, 0, sizeof( SUniqueShip ) );
	CopyUniqueShip( sTempShip, m_aUniqueShip[ iIndex ] );
	CopyUniqueShip( m_aUniqueShip[ iIndex ], m_aUniqueShip[ iIndex - 1 ] );
	CopyUniqueShip( m_aUniqueShip[ iIndex - 1 ], sTempShip );

	SaveUniqueShips();

	return true;
}

bool CShipsManager::MoveDownIndex( int iIndex )
{
	if( iIndex < 0 || iIndex >= GetUniqueShipSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}
	//strwadz czy aktualna pozycja nie jest ostatnia
	if( iIndex == GetUniqueShipSize() - 1 ) {
		wxMessageBox( _T("This index is last"), _T("Exclamation"), wxOK | wxICON_EXCLAMATION );
		return false;
	}

	SUniqueShip sTempShip;
	memset( &sTempShip, 0, sizeof( SUniqueShip ) );
	CopyUniqueShip( sTempShip, m_aUniqueShip[ iIndex ] );
	CopyUniqueShip( m_aUniqueShip[ iIndex ], m_aUniqueShip[ iIndex + 1 ] );
	CopyUniqueShip( m_aUniqueShip[ iIndex + 1 ], sTempShip );

	SaveUniqueShips();

	return true;
}

void CShipsManager::CopyUniqueShip( SUniqueShip &dest, SUniqueShip &src )
{
	if( dest.psEmitterData)
		delete [] dest.psEmitterData;

	dest.bCanFire = src.bCanFire;
	dest.bCanManoeuvring = src.bCanManoeuvring;
	dest.bPlayer = src.bPlayer;

	dest.iStarsShield = src.iStarsShield;
	dest.iStarsArmour = src.iStarsArmour;
	dest.iStarsArmament = src.iStarsArmament;
	dest.iStarsManoeuvring = src.iStarsManoeuvring;

	dest.fCollisionA = src.fCollisionA;
	dest.fCollisionB = src.fCollisionB;

	dest.iEmitterNumber = src.iEmitterNumber;

	wcscpy_s( dest.cTexName, SIZE_STRING, src.cTexName );
	wcscpy_s( dest.cTexBumpName, SIZE_STRING, src.cTexBumpName );
	wcscpy_s( dest.cObjectName, SIZE_STRING, src.cObjectName );
	wcscpy_s( dest.cShipName, SIZE_STRING, src.cShipName );

	if( dest.iEmitterNumber == 0 )
		dest.psEmitterData = NULL;
	else {
		dest.psEmitterData = new SEmitterData[dest.iEmitterNumber ];
		for( int i = 0; i < dest.iEmitterNumber; ++i ) {
			dest.psEmitterData[ i ].iIndex = 0;
			dest.psEmitterData[ i ].fEmitterPosX = src.psEmitterData[ i ].fEmitterPosX;
			dest.psEmitterData[ i ].fEmitterPosY = src.psEmitterData[ i ].fEmitterPosY;
			dest.psEmitterData[ i ].fEmitterPosZ = src.psEmitterData[ i ].fEmitterPosZ;
			dest.psEmitterData[ i ].iTexEmitter = src.psEmitterData[ i ].iTexEmitter;
			dest.psEmitterData[ i ].fR = src.psEmitterData[ i ].fR;
			dest.psEmitterData[ i ].fG = src.psEmitterData[ i ].fG;
			dest.psEmitterData[ i ].fB = src.psEmitterData[ i ].fB;
			dest.psEmitterData[ i ].iParticlesNumber = src.psEmitterData[ i ].iParticlesNumber;
		}
	}
}

bool CShipsManager::AddNewEmitter( int iIndex, const SEmitterData &sNewEmitter )
{
	if( iIndex < 0 || iIndex >= GetUniqueShipSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	//stworz kobie aktualnych emiterow
	std::vector<SEmitterData> aCopyOfEmitters;
	for( int i = 0; i < m_aUniqueShip[ iIndex ].iEmitterNumber; ++i ) {
		aCopyOfEmitters.push_back( m_aUniqueShip[ iIndex ].psEmitterData[ i ] );
	}

	//usun aktualne emitery i stworz nowe
	if( m_aUniqueShip[ iIndex ].iEmitterNumber > 0 && m_aUniqueShip[ iIndex ].psEmitterData )
		delete [] m_aUniqueShip[ iIndex ].psEmitterData;

	//zwieksz liczbe emiterow
	++m_aUniqueShip[ iIndex ].iEmitterNumber;

	//utworz tablice na nowo z nowym emiterem
	m_aUniqueShip[ iIndex ].psEmitterData = new SEmitterData[ m_aUniqueShip[ iIndex ].iEmitterNumber ];
	for( int i = 0; i < m_aUniqueShip[ iIndex ].iEmitterNumber; ++i ) {
		if( i < static_cast<int>( aCopyOfEmitters.size() ) )
			memcpy_s( &m_aUniqueShip[ iIndex ].psEmitterData[ i ], sizeof( SEmitterData), &aCopyOfEmitters[ i ], sizeof( SEmitterData) );
		else //dopisz nowy
			memcpy_s( &m_aUniqueShip[ iIndex ].psEmitterData[ i ], sizeof( SEmitterData), &sNewEmitter, sizeof( SEmitterData) );
	}

	aCopyOfEmitters.clear();
	SaveUniqueShips();
	return true;
}

bool CShipsManager::DeleteEmitter( int iIndexObj, int iIndexEmitter )
{
	if( iIndexObj < 0 || iIndexObj >= GetUniqueShipSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	if( iIndexEmitter < 0 || iIndexEmitter >= m_aUniqueShip[ iIndexObj ].iEmitterNumber ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	//stworz kobie aktualnych emiterow ale bez usuwanego
	std::vector<SEmitterData> aCopyOfEmitters;
	for( int i = 0; i < m_aUniqueShip[ iIndexObj ].iEmitterNumber; ++i ) {
		if( i == iIndexEmitter )
			continue;
		aCopyOfEmitters.push_back( m_aUniqueShip[ iIndexObj ].psEmitterData[ i ] );
	}

	//usun aktualne emitery
	if( m_aUniqueShip[ iIndexObj ].iEmitterNumber > 0 && m_aUniqueShip[ iIndexObj ].psEmitterData )
		delete [] m_aUniqueShip[ iIndexObj ].psEmitterData;

	//zmniejsz liczbe emiterow
	--m_aUniqueShip[ iIndexObj ].iEmitterNumber;

	//utworz tablice na nowo
	m_aUniqueShip[ iIndexObj ].psEmitterData = new SEmitterData[ m_aUniqueShip[ iIndexObj ].iEmitterNumber ];
	for( int i = 0; i < m_aUniqueShip[ iIndexObj ].iEmitterNumber; ++i ) {
		memcpy_s( &m_aUniqueShip[ iIndexObj ].psEmitterData[ i ], sizeof( SEmitterData), &aCopyOfEmitters[ i ], sizeof( SEmitterData) );
	}

	aCopyOfEmitters.clear();
	SaveUniqueShips();
	return true;
}

bool CShipsManager::SaveModifyEmitter( int iIndexObj, int iIndexEmitter, const SEmitterData &sModEmitter )
{
	if( iIndexObj < 0 || iIndexObj >= GetUniqueShipSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	if( iIndexEmitter < 0 || iIndexEmitter >= m_aUniqueShip[ iIndexObj ].iEmitterNumber ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	memcpy_s( &m_aUniqueShip[ iIndexObj ].psEmitterData[ iIndexEmitter ], sizeof( SEmitterData), &sModEmitter, sizeof( SEmitterData) );

	SaveUniqueShips();
	return true;
}

bool CShipsManager::AddNewObject( const SUniqueShip &sNewShip )
{
	return SaveUniqueShip( sNewShip );
}

bool CShipsManager::DeleteObject( int iIndexObj )
{
	if( iIndexObj < 0 || iIndexObj >= GetUniqueShipSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	if( m_aUniqueShip[ iIndexObj ].psEmitterData)
		delete [] m_aUniqueShip[ iIndexObj ].psEmitterData;
	m_aUniqueShip.erase( m_aUniqueShip.begin() + iIndexObj );

	SaveUniqueShips();

	return true;
}

bool CShipsManager::SaveModifyObject( int iIndexObj, SUniqueShip &sModShip )
{
	if( iIndexObj < 0 || iIndexObj >= GetUniqueShipSize() ) {
		wxMessageBox( _T("Error: incorrect index array"), _T("Error"), wxOK | wxICON_ERROR );
		return false;
	}

	//ustaw dane emittera takie jak ma oryginal bo ich nie zmnieniamy tutaj
	sModShip.iEmitterNumber = m_aUniqueShip[ iIndexObj ].iEmitterNumber;
	sModShip.psEmitterData = m_aUniqueShip[ iIndexObj ].psEmitterData;

	memcpy_s( &m_aUniqueShip[ iIndexObj ], sizeof( SUniqueShip ), &sModShip, sizeof( SUniqueShip ) );

	SaveUniqueShips();
	return true;
}