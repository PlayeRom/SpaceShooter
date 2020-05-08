#include "StdAfx.h"
#include <iostream>
#include <Windows.h>
#include "Convert.h"

using namespace std;

CConvert::CConvert( wxTextCtrl *pLog )
{
	m_pLog = pLog;
}

CConvert::~CConvert()
{
	ClearFileNameArray();
}

bool CConvert::ConvertBMPtoJEH( LPCTSTR lpFileName )
{
	BITMAPFILEHEADER sBmpFileHeader;
	BITMAPINFOHEADER sBmpInfoHeader;

	//sprawdz czy tekstura bmp jest maska lub tekstura bump, sprawdzanie tylko po nazwie pliku
	bool b8BitCount = false;
#if _USE_8BITS_
	if( wcsstr( lpFileName, _T("bump") ) || wcsstr( lpFileName, _T("mask") ) )
		b8BitCount = true;
#endif

	FILE *hFile;
	if(_wfopen_s( &hFile, lpFileName, _T("rb") ) != 0 ) {
		*m_pLog << _T("ERROR. Open file: ") << lpFileName << _T(" -> failed!\n");
		return false;
	}

	//wczytaj naglowek pliku
	fread( &sBmpFileHeader, sizeof( BITMAPFILEHEADER ), 1, hFile );
	if( sBmpFileHeader.bfType != 0x4D42 ) { //0x4D42 - identyfikator bitmapy
		*m_pLog << _T("ERROR. File: ") << lpFileName << _T(" isn't bmp!\n");
		fclose( hFile );
		return false;
	}
	
	//wczytaj informacje o naglowku
	fread( &sBmpInfoHeader, sizeof( BITMAPINFOHEADER ), 1, hFile );

	//sprawdzenie dodatkowych informacji na temat bitmapy:
	if( sBmpInfoHeader.biBitCount != 24 ) {
		*m_pLog << _T("ERROR. ") << lpFileName << _T(" Bits per pixel must be 24 or 8!\n");
		fclose( hFile );
		return false;
	}
	if( sBmpInfoHeader.biHeight < 0 ) {
		*m_pLog << _T("ERROR. ") << lpFileName << _T(" sBmpInfoHeader.biHeight must be positive!\n");
		fclose( hFile );
		return false;
	}
	if( sBmpInfoHeader.biCompression != 0 ) {
		*m_pLog << _T("ERROR. ") << lpFileName << _T(" sBmpInfoHeader.biCompression must be 0!\n");
		fclose( hFile );
		return false;
	}
	
	fseek( hFile, sBmpFileHeader.bfOffBits, SEEK_SET );

	//tworzymy bufor na przechowanie obrazu
	if( sBmpInfoHeader.biSizeImage == 0 ) {
		*m_pLog << _T("WARNING. ") << lpFileName << _T("  sBmpInfoHeader.biSizeImage is 0!\n");
		sBmpInfoHeader.biSizeImage = ( sBmpInfoHeader.biBitCount / 8 ) * sBmpInfoHeader.biHeight * sBmpInfoHeader.biWidth;
	}
	unsigned char *pImgBuffer = new unsigned char[ sBmpInfoHeader.biSizeImage ];
	if( !pImgBuffer ) {
		*m_pLog << _T("ERROR. ") << lpFileName << _T("  Create image buffer failed!\n");
		fclose( hFile );
		return false;
	}

	//wczytaj obraz
	fread( pImgBuffer, 1, sBmpInfoHeader.biSizeImage, hFile );
	fclose( hFile );

	if( pImgBuffer == NULL )	{
		*m_pLog << _T("ERROR. ") << lpFileName << _T("  Read image buffer failed!\n");
		return false;
	}
	
	//zamiana BRG na RGB
	unsigned char tmpRGB;
	for( unsigned int i = 0; i < sBmpInfoHeader.biSizeImage; i += 3 ) {
		tmpRGB = pImgBuffer[ i ];
		pImgBuffer[ i ] = pImgBuffer[ i + 2 ];
		pImgBuffer[ i + 2 ] = tmpRGB;
	}

	if( b8BitCount ) {
		//dla masek lub tekstur bump, przemianowanie tekstury na 8 bitowa
		//zapisujemy tylko jeden bajt na piksel, pobieramy go ze srednij RGB
		unsigned char *pImgTemp = new unsigned char[ sBmpInfoHeader.biSizeImage / 3 ];
		for( unsigned int i = 0, j = 0; i < sBmpInfoHeader.biSizeImage; i += 3, ++j ) {
			// obliczamy sledni¹ koloru i ustawiamy go jako wlasciwy
			unsigned char R = pImgBuffer[ i ];
			unsigned char G = pImgBuffer[ i + 1 ];
			unsigned char B = pImgBuffer[ i + 2 ];
			pImgTemp[ j ] = ( R + G + B ) / 3;
		}

		if( pImgBuffer ) {
			delete [] pImgBuffer;
			pImgBuffer = NULL;
		}
		pImgBuffer = pImgTemp;
	}
#if _USE_16BITS_
	else {
		//konwertujemy na 16 bitow na piksel. Idea jest taka aby sprawdzic czy wszystkie skaldowe RGB sa sobie
		//rowne, jezeli tak, to zapisujemy skladowa R, oraz bajt mowiacy, ze GB sa takie same jak R, do tego celu
		//nalezy wykluczyc kolor 0 - bo tym oznaczymy, ze piksel ma 2 bajty, wiec wszytsko co ma 0 trzeba
		//przekonwertowac na 1.

		//znajdz 0 w kazdej skladowej RGB i zamien na 1
		for( unsigned int i = 0; i < sBmpInfoHeader.biSizeImage; ++i ) {
			if( pImgBuffer[ i ] == 0 )
				pImgBuffer[ i ] = 1;
		}


		//konwertuj na 16 bit
		std::vector<unsigned char> aImgTemp;
		for( unsigned int i = 0; i < sBmpInfoHeader.biSizeImage; i += 3 ) {
			if( pImgBuffer[ i ] == pImgBuffer[ i + 1 ] &&  pImgBuffer[ i ] == pImgBuffer[ i +2 ] ) {
				//R = G = B
				aImgTemp.push_back( pImgBuffer[ i ] );	//wrzucam skladowa R
				aImgTemp.push_back( 0 );			//zaznaczenie
			}
			else { //normalnie
				aImgTemp.push_back( pImgBuffer[ i ] );		//wrzucam skladowa R
				aImgTemp.push_back( pImgBuffer[ i + 1 ] );	//wrzucam skladowa G
				aImgTemp.push_back( pImgBuffer[ i + 2 ] );	//wrzucam skladowa B
			}
		}
		if( pImgBuffer ) {
			delete [] pImgBuffer;
			pImgBuffer = NULL;
		}
		sBmpInfoHeader.biSizeImage = static_cast<DWORD>( aImgTemp.size() );
		pImgBuffer = new unsigned char[ sBmpInfoHeader.biSizeImage ];
		if( !pImgBuffer )
			return false;
		for( unsigned int i = 0; i < aImgTemp.size(); ++i ) {
			pImgBuffer[ i ] = aImgTemp[ i ];
		}
		aImgTemp.clear();
		sBmpInfoHeader.biBitCount = 16;
	}
#endif

	SHeaderJEH sHeaderJEH;
	strcpy_s( sHeaderJEH.cHeader, sizeof( sHeaderJEH.cHeader ), JEH_HEADER );
	sHeaderJEH.uiWidth = sBmpInfoHeader.biWidth;
	sHeaderJEH.uiHeight = sBmpInfoHeader.biHeight;
	if( b8BitCount ) {
		sHeaderJEH.uiSizeImage = sBmpInfoHeader.biSizeImage / 3;
		sHeaderJEH.uiBitCount = 8;
	}
	else {
		sHeaderJEH.uiSizeImage = sBmpInfoHeader.biSizeImage;
		sHeaderJEH.uiBitCount = sBmpInfoHeader.biBitCount;
	}
	SaveJEHFile( lpFileName, sHeaderJEH, pImgBuffer );

	delete [] pImgBuffer;

	return true;
}

bool CConvert::SaveJEHFile( LPCTSTR lpFileName, const SHeaderJEH &sHeaderJEH, const unsigned char *pImgData)
{
	//zmien rozszerzenie pliku na jeh
	size_t uiSize = wcslen( lpFileName );
	TCHAR *pTempFileName = new TCHAR[ uiSize + 1 ];
	wcscpy_s( pTempFileName, uiSize + 1, lpFileName );

	uiSize += 5;
	TCHAR *pYEHFile = new TCHAR[ uiSize ];
	for( size_t i = 0; i < uiSize; ++i ) {
		if( pTempFileName[ i ] == '.') {
			pTempFileName[ i ] = 0;
			break;
		}
	}
	swprintf_s( pYEHFile, uiSize, _T("%s.jeh"), pTempFileName );

	FILE *hFile;
	errno_t iResult = _wfopen_s( &hFile, pYEHFile, _T("wb") );
	if( iResult != 0 ) {
		*m_pLog << _T("ERROR. Open file: ") << pYEHFile << _T(" -> failed!\n");
		return false;
	}
	fwrite( &sHeaderJEH, sizeof( sHeaderJEH), 1, hFile );
	fwrite( pImgData, 1, sHeaderJEH.uiSizeImage, hFile );
	fclose( hFile );

	*m_pLog << _T("Created ") << pYEHFile << _T(" file success!\n");

	delete [] pTempFileName;
	delete [] pYEHFile;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

bool CConvert::ConvertJEHtoBMP( LPCTSTR lpFileName )
{
	SHeaderJEH sHeaderJEH;
	FILE *hFile;
	errno_t iResult = _wfopen_s( &hFile, lpFileName, _T("rb") );
	if( iResult != 0 ) {
		*m_pLog << _T("ERROR. Open file: ") << lpFileName << _T(" -> failed!\n");
		return false;
	}

	//wczytaj naglowek pliku
	fread( &sHeaderJEH, sizeof( SHeaderJEH), 1, hFile );
	if( strcmp( sHeaderJEH.cHeader, JEH_HEADER ) != 0 ) {
		*m_pLog << _T("ERROR. File: ") << lpFileName << _T(" isn't yeh!\n");
		fclose( hFile );
		return false;
	}

	//wczytaj obraz
	unsigned char *pImgBuffer = new unsigned char[ sHeaderJEH.uiSizeImage ];
	if( !pImgBuffer ) {
		*m_pLog << _T("ERROR. ") << lpFileName << _T("  Create image buffer failed!\n");
		fclose( hFile );
		return false;
	}
	fread( pImgBuffer, 1, sHeaderJEH.uiSizeImage, hFile );
	fclose( hFile );

	if( pImgBuffer == NULL )	{
		*m_pLog << _T("ERROR. ") << lpFileName << _T("  Read image buffer failed!\n");
		return false;
	}

	if( sHeaderJEH.uiBitCount == 8 ) {
		//przemianowanie tekstury z 8 bitowej na 24 bitowa
		//z 1 bajta tworzymy 3 bajty na piksel
		unsigned char *pImgTemp = new unsigned char[ sHeaderJEH.uiSizeImage * 3 ];
		for( unsigned int i = 0, j = 0; j < sHeaderJEH.uiSizeImage; i += 3, ++j ) {
			pImgTemp[ i ] = pImgBuffer[ j ];
			pImgTemp[ i + 1 ] = pImgBuffer[ j ];
			pImgTemp[ i + 2 ] = pImgBuffer[ j ];
		}

		delete [] pImgBuffer;
		pImgBuffer = pImgTemp;

		sHeaderJEH.uiSizeImage *= 3;
	}
	else if( sHeaderJEH.uiBitCount == 16 ) {
		//przemianowanie tekstury z 16 bitowej na 24 bitowa
		//skladowa 0 mowi, ze poprzedni bart to skladowa R rowna takze skaldowym G,B, ktore trzeba samemu
		//stworzyc

		std::vector<unsigned char> aImgTemp;
		for( unsigned int i = 0, iModulo = 1; i < sHeaderJEH.uiSizeImage; ++i, ++iModulo ) {
			if( i > 0 && pImgBuffer[ i ] == 0 ) {
				aImgTemp.push_back( pImgBuffer[ i - 1 ] );
				aImgTemp.push_back( pImgBuffer[ i - 1 ] );
				aImgTemp.push_back( pImgBuffer[ i - 1 ] );
				iModulo = 0;
			}
			else if( iModulo % 3 == 0 ) {
				aImgTemp.push_back( pImgBuffer[ i-2 ] );
				aImgTemp.push_back( pImgBuffer[ i - 1 ] );
				aImgTemp.push_back( pImgBuffer[ i ] );
			}
		}
		delete [] pImgBuffer;
		sHeaderJEH.uiSizeImage = static_cast<unsigned int>( aImgTemp.size() );
		pImgBuffer = new unsigned char[ sHeaderJEH.uiSizeImage ];
		for( unsigned int i = 0; i < sHeaderJEH.uiSizeImage; ++i )
			pImgBuffer[ i ] = aImgTemp[ i ];
		aImgTemp.clear();
	}

	//zamiana RGB na BRG
	unsigned char tmpRGB;
	for( unsigned int i = 0; i < sHeaderJEH.uiSizeImage; i += 3 ) {
		tmpRGB = pImgBuffer[ i ];
		pImgBuffer[ i ] = pImgBuffer[ i + 2 ];
		pImgBuffer[ i + 2 ] = tmpRGB;
	}

	SaveBMPFile( lpFileName, sHeaderJEH, pImgBuffer );

	delete [] pImgBuffer;

	return true;
}

bool CConvert::SaveBMPFile( LPCTSTR lpFileName, const SHeaderJEH &sHeaderJEH, const unsigned char *pImgData)
{
	BITMAPFILEHEADER sBmpFileHeader;
	BITMAPINFOHEADER sBmpInfoHeader;

	//zmien rozszerzenie pliku na bmp
	size_t uiSize = wcslen( lpFileName );
	TCHAR *pTempFileName = new TCHAR[ uiSize + 1 ];
	wcscpy_s( pTempFileName, uiSize + 1, lpFileName );

	uiSize += 5;
	TCHAR *pBMPFile = new TCHAR[ uiSize ];
	for( size_t i = 0; i < uiSize; ++i ) {
		if( pTempFileName[ i ] == '.') {
			pTempFileName[ i ] = 0;
			break;
		}
	}
	swprintf_s( pBMPFile, uiSize, _T("%s.bmp"), pTempFileName );

	FILE *hFile;
	errno_t iResult = _wfopen_s( &hFile, pBMPFile, _T("wb") );
	if( iResult != 0 ) {
		*m_pLog << _T("ERROR. Open file: ") << pBMPFile << _T(" -> failed!\n");
		return false;
	}
	//uzupelenie struktur
	sBmpFileHeader.bfType = 0x4D42;
	sBmpFileHeader.bfSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + sHeaderJEH.uiSizeImage;
	sBmpFileHeader.bfReserved1 = 0;
	sBmpFileHeader.bfReserved2 = 0;
	sBmpFileHeader.bfOffBits = 54;
	fwrite( &sBmpFileHeader, sizeof( BITMAPFILEHEADER ), 1, hFile );

	sBmpInfoHeader.biSize = 40;
	sBmpInfoHeader.biWidth = sHeaderJEH.uiWidth;
	sBmpInfoHeader.biHeight = sHeaderJEH.uiHeight;
	sBmpInfoHeader.biPlanes = 1;
	sBmpInfoHeader.biBitCount = 24;
	sBmpInfoHeader.biCompression = 0;
	sBmpInfoHeader.biSizeImage = sHeaderJEH.uiSizeImage;
	sBmpInfoHeader.biXPelsPerMeter = 0;
	sBmpInfoHeader.biYPelsPerMeter = 0;
	sBmpInfoHeader.biClrUsed = 0;
	sBmpInfoHeader.biClrImportant = 0;
	fwrite( &sBmpInfoHeader, sizeof( BITMAPINFOHEADER ), 1, hFile );
	fwrite( pImgData, 1, sHeaderJEH.uiSizeImage, hFile );
	fclose( hFile );

	*m_pLog << _T("Created ") << pBMPFile << _T(" file success!\n");

	delete [] pTempFileName;
	delete [] pBMPFile;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool CConvert::ConvertAllBMPtoJEH( LPCTSTR lpCurDir )
{
	ClearFileNameArray();
	SearchFile( lpCurDir, _T("bmp") );
	for( int i = 0; i < GetFileNameArraySize(); ++i ) {
		ConvertBMPtoJEH( m_aFileName[ i ].pFileName );
	}
	*m_pLog << _T("Done.\n");
	return true;
}

bool CConvert::ConvertAllJEHtoBMP( LPCTSTR lpCurDir )
{
	ClearFileNameArray();
	SearchFile( lpCurDir, _T("jeh") );
	for( int i = 0; i < GetFileNameArraySize(); ++i ) {
		ConvertJEHtoBMP( m_aFileName[ i ].pFileName );
	}
	*m_pLog << _T("Done.\n");
	return true;
}

void CConvert::SearchFile( LPCTSTR lpCurDir, LPCTSTR lpExtension )
{
	WIN32_FIND_DATA wfd; 
	BOOL IsFile = TRUE; 
	HANDLE hFile;
	TCHAR cPath[ 256 ];
	::SetCurrentDirectory( lpCurDir ); 
	hFile = ::FindFirstFile( _T("*.*"), &wfd ); 

	while( IsFile ) {
		::GetFullPathName( wfd.cFileName, 255, cPath, NULL );
		if( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && wcsstr( cPath, lpExtension ) ) {
			size_t iSize = wcslen( cPath ) + 1;
			SFileName sFileName;
			sFileName.pFileName = new TCHAR[ iSize ];
			wcscpy_s( sFileName.pFileName, iSize, cPath );
			m_aFileName.push_back( sFileName );

		}

		IsFile = ::FindNextFile( hFile, &wfd );
	}
    if( lpCurDir[ 0 ] > 3 )
		::SetCurrentDirectory( _T("..") );
}

void CConvert::ClearFileNameArray()
{
	for( int i = GetFileNameArraySize() - 1; i >= 0; --i ) {
		delete [] m_aFileName[ i ].pFileName;
	}
	m_aFileName.clear();
}