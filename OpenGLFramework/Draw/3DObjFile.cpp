
#include "StdAfx.h"
#include "VertexArrays.h"
#include "../OpenGLFramework/Console.h"
#include "../OpenGLFramework/GameControl.h"
#include "../MatrixOperations/MatrixOperations.h"
#include "3DObjFile.h"

C3DObjFile::C3DObjFile( LPCTSTR lpFileName /*= NULL*/ )
{
	m_pGameCtrl = CGameControl::GetInstance();
#ifndef _3DSLOADER_
	if( lpFileName != NULL ) {
		GLboolean bIsElem;
		LoadObject( lpFileName, bIsElem );
	}
#endif
}

C3DObjFile::~C3DObjFile()
{
}

#ifdef _3DSLOADER_
GLboolean C3DObjFile::SaveObject( LPCTSTR lpFileName, const S3DSObject &in_s3DSObject,
								  const CVector3 &in_cScale, const CVector3 &in_cOffsetVector,
								  const CVector3 &in_cRot90, GLboolean bSmoothNormal )
{
	if( lpFileName == NULL )
		return GL_FALSE;

	TCHAR cFileName[ 256 ] = { 0 };
	::wsprintf( cFileName, _T("%s"), lpFileName );
	for( GLint i = 0; i < 256 - 6; ++i ) {
		if( cFileName[ i ] == '.' ) {
			cFileName[ i + 1 ] = '3';
			cFileName[ i + 2 ] = 'D';
			cFileName[ i + 3 ] = 'O';
			cFileName[ i + 4 ] = 'b';
			cFileName[ i + 5 ] = 'j';
			cFileName[ i + 6 ] = 0;
		}
	}

	//obciecie nazwy folderu 3dsobjects
	GLboolean bFound = GL_FALSE;
	GLint j = 0;
	for( GLint i = 0; i < 256; ++i ) {
		if( bFound ) {
			cFileName[ j ] = cFileName[ i ];
			if( cFileName[ j ] == 0 )
				break;
			++j;
		}

		if( cFileName[ i ] == '/' || cFileName[ i ] == '\\' )
			bFound = GL_TRUE;
	}

	TCHAR cFileNameFinally[ 256 ] = { 0 };
	::wsprintf( cFileNameFinally, _T("objects/%s"), cFileName );

	FILE *hFile = NULL;
	if( ::OpenFile( &hFile, cFileNameFinally, _T("wb") ) != 0 )
		return GL_FALSE;

	SaveHeader( hFile, GL_FALSE );

	fwrite( &in_s3DSObject.uiVerticesNum, sizeof( GLushort ), 1, hFile );
	fwrite( &in_s3DSObject.uiNormalsNum, sizeof( GLushort ), 1, hFile );
	fwrite( &in_s3DSObject.uiFacesNum, sizeof( GLushort ), 1, hFile );
	fwrite( &in_s3DSObject.uiTexCoordsNum, sizeof( GLushort ), 1, hFile );

	//transformacje wierzcholkow
	for( GLint i = 0; i < in_s3DSObject.uiVerticesNum; ++i ) {
		in_s3DSObject.pVertices[ i ] = m_pGameCtrl->GetMatrixOp()->Scale3( in_s3DSObject.pVertices[ i ], in_cScale );
		in_s3DSObject.pVertices[ i ] = m_pGameCtrl->GetMatrixOp()->RotateXYZ3( in_s3DSObject.pVertices[ i ], in_cRot90 );
		in_s3DSObject.pVertices[ i ] = m_pGameCtrl->GetMatrixOp()->Translate3( in_s3DSObject.pVertices[ i ], in_cOffsetVector );
	}
	for( GLint i = 0; i < in_s3DSObject.uiNormalsNum; ++i )
		in_s3DSObject.pNormals[ i ] = m_pGameCtrl->GetMatrixOp()->RotateXYZ3( in_s3DSObject.pNormals[ i ], in_cRot90 );

	CVector2 cTexCoords[ 3 ];

	GLint iNormalsCount = -1;
	for( GLint j = 0; j < in_s3DSObject.uiFacesNum; ++j ) {
		GLint v1 = in_s3DSObject.pFaces[ j ].v1;
		GLint v2 = in_s3DSObject.pFaces[ j ].v2;
		GLint v3 = in_s3DSObject.pFaces[ j ].v3;

		if( in_s3DSObject.pTexCoords ) {
			cTexCoords[ 0 ] = in_s3DSObject.pTexCoords[ v1 ];
			cTexCoords[ 1 ] = in_s3DSObject.pTexCoords[ v2 ];
			cTexCoords[ 2 ] = in_s3DSObject.pTexCoords[ v3 ];
		}
		else ::SecureZeroMemory( &cTexCoords, sizeof( CVector2 ) * 3 );

		fwrite( &in_s3DSObject.pVertices[ v1 ], sizeof( CVector3 ), 1, hFile );
		fwrite( &cTexCoords[ 0 ], sizeof( CVector2 ), 1, hFile );
		if( bSmoothNormal )
			fwrite( &in_s3DSObject.pNormals[ ++iNormalsCount ], sizeof( CVector3 ), 1, hFile );
		else 
			fwrite( &in_s3DSObject.pNormals[ v1 ], sizeof( CVector3 ), 1, hFile );

		fwrite( &in_s3DSObject.pVertices[ v2 ], sizeof( CVector3 ), 1, hFile );
		fwrite( &cTexCoords[ 1 ], sizeof( CVector2 ), 1, hFile );
		if( bSmoothNormal )
			fwrite( &in_s3DSObject.pNormals[ ++iNormalsCount ], sizeof( CVector3 ), 1, hFile );
		else
			fwrite( &in_s3DSObject.pNormals[ v2 ], sizeof( CVector3 ), 1, hFile );

		fwrite( &in_s3DSObject.pVertices[ v3 ], sizeof( CVector3 ), 1, hFile );
		fwrite( &cTexCoords[ 2 ], sizeof( CVector2 ), 1, hFile );
		if( bSmoothNormal )
			fwrite( &in_s3DSObject.pNormals[ ++iNormalsCount ], sizeof( CVector3 ), 1, hFile );
		else
			fwrite( &in_s3DSObject.pNormals[ v3 ], sizeof( CVector3 ), 1, hFile );
	}

	fclose( hFile );

	return GL_TRUE;
}
#endif

#ifdef _3DSLOADER_
GLboolean C3DObjFile::SaveObjectElem( GLint iIndexObject, LPCTSTR lpFileName, GLboolean /*bSmoothNormal*/, GLboolean bFixed )
{
	if( lpFileName == NULL )
		return GL_FALSE;

	TCHAR cFileNameTemp[ 256 ] = { 0 };
	::wsprintf( cFileNameTemp, _T("%s"), lpFileName );
	for( GLint i = ::GetStringLength( cFileNameTemp ) - 1; i >= 0; --i ) {
		if( cFileNameTemp[ i ] == '.' ) {
			cFileNameTemp[ i ] = 0;
			break;
		}
	}

	TCHAR cFileName[ 256 ] = { 0 };
	if( bFixed )
		::wsprintf( cFileName, _T("%s_fixed.3DObj"), cFileNameTemp );
	else
		::wsprintf( cFileName, _T("%s.3DObj"), cFileNameTemp );

	//obciecie nazwy folderu 3dsobjects
	GLboolean bFound = GL_FALSE;
	GLint j = 0;
	for( GLint i = 0; i < 256; ++i ) {
		if( bFound ) {
			cFileName[ j ] = cFileName[ i ];
			if( cFileName[ j ] == 0 )
				break;
			++j;
		}
		if( cFileName[ i ] == '/' || cFileName[ i ] == '\\' )
			bFound = GL_TRUE;
	}

	TCHAR cFileNameFinally[ 256 ] = { 0 };
	::wsprintf( cFileNameFinally, _T("objects/%s"), cFileName );

	FILE *hFile = NULL;
	if( ::OpenFile( &hFile, cFileNameFinally, _T("wb") ) != 0 )
		return GL_FALSE;

	SaveHeader( hFile, GL_TRUE );

	//znajdz ilosc wierzcholkow niepowtarzalnych z danego SubIndices
	GLint iIndex = 0;
	GLint iCountIndices = 0;
	GLushort usVerticesNum = 0;
	for( GLint i = 0; i < CVertexArrays::GetInstance()->GetSubIndicesSize( iIndexObject ); ++i ) {
		iIndex = CVertexArrays::GetInstance()->GetIndexFromIndices( iIndexObject, i );
		if( iIndex == iCountIndices + 1 || iCountIndices == 0 ) {}
		else 
			continue;
		iCountIndices = iIndex;

		++usVerticesNum;
	}

	fwrite( &usVerticesNum, sizeof( GLushort ), 1, hFile ); //zapis ilosci wierzcholkow

	//CVector4 cColor( 1.0f, 1.0f, 1.0f, 1.0f );
	iIndex = 0;
	iCountIndices = 0;
	for( GLint i = 0; i < CVertexArrays::GetInstance()->GetSubIndicesSize( iIndexObject ); ++i ) {
		iIndex = CVertexArrays::GetInstance()->GetIndexFromIndices( iIndexObject, i );
		if( iIndex == iCountIndices + 1 || iCountIndices == 0 ) {}
		else
			continue;
		iCountIndices = iIndex;

		//zapis wierzcholkow - niepowtarzalnych
		if( bFixed ) {
			CVector3 cVertex = CVertexArrays::GetInstance()->GetVertexElem( iIndex );
			GLint iVertexX( FLOAT_TO_FIXED( cVertex.x ) );
			GLint iVertexY( FLOAT_TO_FIXED( cVertex.y ) );
			GLint iVertexZ( FLOAT_TO_FIXED( cVertex.z ) );
			fwrite( &iVertexX, sizeof( GLint ), 1, hFile );
			fwrite( &iVertexY, sizeof( GLint ), 1, hFile );
			fwrite( &iVertexZ, sizeof( GLint ), 1, hFile );

			CVector2 cTexCoord = CVertexArrays::GetInstance()->GetTexCoordElem( iIndex );
			GLint iTexCoordX( FLOAT_TO_FIXED( cTexCoord.x ) );
			GLint iTexCoordY( FLOAT_TO_FIXED( cTexCoord.y ) );
			fwrite( &iTexCoordX, sizeof( GLint ), 1, hFile );
			fwrite( &iTexCoordY, sizeof( GLint ), 1, hFile );

			CVector3 cNormal = CVertexArrays::GetInstance()->GetNormalElem( iIndex );
			GLint iNormalX( FLOAT_TO_FIXED( cNormal.x ) );
			GLint iNormalY( FLOAT_TO_FIXED( cNormal.y ) );
			GLint iNormalZ( FLOAT_TO_FIXED( cNormal.z ) );
			fwrite( &iNormalX, sizeof( GLint ), 1, hFile );
			fwrite( &iNormalY, sizeof( GLint ), 1, hFile );
			fwrite( &iNormalZ, sizeof( GLint ), 1, hFile );
		}
		else {
			fwrite( &CVertexArrays::GetInstance()->GetVertexElem( iIndex ), sizeof( CVector3 ), 1, hFile );
			fwrite( &CVertexArrays::GetInstance()->GetTexCoordElem( iIndex ), sizeof( CVector2 ), 1, hFile );
			fwrite( &CVertexArrays::GetInstance()->GetNormalElem( iIndex ), sizeof( CVector3 ), 1, hFile );
			//fwrite( &cColor, sizeof( CVector4 ), 1, hFile );
		}
	}

	GLushort iTemp = static_cast<GLushort>( CVertexArrays::GetInstance()->GetSubIndicesSize( iIndexObject ) );
	fwrite( &iTemp, sizeof( GLushort ), 1, hFile ); //ilosc indeksow

	for( GLint i = 0; i < CVertexArrays::GetInstance()->GetSubIndicesSize( iIndexObject ); ++i ) {
		iTemp = static_cast<GLushort>( CVertexArrays::GetInstance()->GetIndexFromIndices( iIndexObject, i ) );
		fwrite( &iTemp, sizeof( GLushort ), 1, hFile ); //zapis indeksu
	}

	fclose( hFile );

	return GL_TRUE;
}
#endif

#ifdef _3DSLOADER_
GLboolean C3DObjFile::SaveHeader( FILE *hFile, GLboolean bIsElem )
{
	if( hFile == NULL )
		return GL_FALSE;

	GLushort usVersionMajor = _3DOBJFILEVERSION_MAJOR;
	GLushort usVersionMinor = _3DOBJFILEVERSION_MINOR;
	char cHeader[ 14 ];
	strcpy_s( cHeader, 14, _3DOBJFILE_HEADER );
	fwrite( cHeader, sizeof( cHeader ), 1, hFile );
	fwrite( &usVersionMajor, sizeof( GLushort ), 1, hFile );
	fwrite( &usVersionMinor, sizeof( GLushort ), 1, hFile );
	fwrite( &bIsElem, sizeof( GLboolean ), 1, hFile );

	return GL_TRUE;
}
#endif

GLboolean C3DObjFile::LoadHeader( FILE *hFile, LPCTSTR lpFileName, GLboolean &out_bIsElem )
{
	if( hFile == NULL )
		return GL_FALSE;

	char cHeader[ 14 ];
	fread( cHeader, sizeof( cHeader ), 1, hFile );
	if( strcmp( cHeader, _3DOBJFILE_HEADER ) != 0 ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: Load %s file failed - incorrect header"), lpFileName );
		return GL_FALSE;
	}

	GLushort usVersionMajor;
	GLushort usVersionMinor;
	fread( &usVersionMajor, sizeof( GLushort ), 1, hFile );
	fread( &usVersionMinor, sizeof( GLushort ), 1, hFile );
	fread( &out_bIsElem, sizeof( GLboolean ), 1, hFile );
	if( usVersionMajor != _3DOBJFILEVERSION_MAJOR || usVersionMinor != _3DOBJFILEVERSION_MINOR ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: Load %s file failed - incorrect file version"), lpFileName );
		return GL_FALSE;
	}

	return GL_TRUE;
}

GLboolean C3DObjFile::LoadObject( LPCTSTR lpFileName, GLboolean &out_bIsElem )
{
	if( lpFileName == NULL )
		return GL_FALSE;

	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, lpFileName, _T("rb") ) != 0 )
		return GL_FALSE;

	if( !LoadHeader( hFile, lpFileName, out_bIsElem ) ) {
		if( hFile )
			fclose( hFile );

		return GL_FALSE;
	}

	if( out_bIsElem )
		LoadDataElem( hFile );
	else
		LoadDataNoElem( hFile );

	fclose( hFile );

	return GL_TRUE;
}

GLboolean C3DObjFile::LoadDataNoElem( FILE *hFile )
{
	m_iVerticesNum = 0;

	GLushort usVerticesNum;
	GLushort usNormalsNum;
	GLushort usFacesNum;
	GLushort usTexCoordsNum;

	CVector3 cVertex;
	CVector2 cTexCoord;
	CVector3 cNormal;

	fread( &usVerticesNum, sizeof( GLushort ), 1, hFile );
	fread( &usNormalsNum, sizeof( GLushort ), 1, hFile );
	fread( &usFacesNum, sizeof( GLushort ), 1, hFile );
	fread( &usTexCoordsNum, sizeof( GLushort ), 1, hFile );

	for( GLushort j = 0; j < usFacesNum; ++j ) {

		fread( &cVertex, sizeof( cVertex ), 1, hFile );
		fread( &cTexCoord, sizeof( cTexCoord ), 1, hFile );
		fread( &cNormal, sizeof( cNormal ), 1, hFile );

		CVertexArrays::GetInstance()->AddFullVertex( cVertex, cTexCoord, cNormal );

		fread( &cVertex, sizeof( cVertex ), 1, hFile );
		fread( &cTexCoord, sizeof( cTexCoord ), 1, hFile );
		fread( &cNormal, sizeof( cNormal ), 1, hFile );

		CVertexArrays::GetInstance()->AddFullVertex( cVertex, cTexCoord, cNormal );

		fread( &cVertex, sizeof( cVertex ), 1, hFile );
		fread( &cTexCoord, sizeof( cTexCoord ), 1, hFile );
		fread( &cNormal, sizeof( cNormal ), 1, hFile );

		CVertexArrays::GetInstance()->AddFullVertex( cVertex, cTexCoord, cNormal );

		m_iVerticesNum += 3;
	}

	return GL_TRUE;
}

GLboolean C3DObjFile::LoadDataElem( FILE *hFile )
{
	m_iVerticesNum = 0;

	GLushort usVerticesNum;
	fread( &usVerticesNum, sizeof( GLushort ), 1, hFile ); //odczyt ilosci wierzcholkow

	CVector3 cVertex;
	CVector2 cTexCoord;
	CVector3 cNormal;

	//pobranie aktualnej ilosc ElemVertices
	GLint iElemVertSize = CVertexArrays::GetInstance()->GetVerticesElemSize();

	for( GLushort i = 0; i < usVerticesNum; ++i ) {
		//odczyt wierzcholkow - niepowtarzalnych
		fread( &cVertex, sizeof( CVector3 ), 1, hFile );
		fread( &cTexCoord, sizeof( CVector2 ), 1, hFile );
		fread( &cNormal, sizeof( CVector3 ), 1, hFile );

		CVertexArrays::GetInstance()->AddVertexElem( cVertex );
		CVertexArrays::GetInstance()->AddTexCoordElem( cTexCoord );
		CVertexArrays::GetInstance()->AddNormalElem( cNormal );
	}

	m_iVerticesNum = static_cast<GLint>( usVerticesNum );

	GLushort usIndicesNum;
	fread( &usIndicesNum, sizeof( GLushort ), 1, hFile ); //ilosc indeksow

	std::vector< GLushort > aSingleShortIndices;
	GLushort usIndex;
	for( GLushort i = 0; i < usIndicesNum; ++i ) {
		fread( &usIndex, sizeof( GLushort ), 1, hFile ); //odczyt indeksu
		//poniewaz moga juz byc wczytane inne obiekty musimy zwiekszyc indeks o istniejace juz indeksy
		usIndex = usIndex + static_cast<GLushort>( iElemVertSize );
		aSingleShortIndices.push_back( usIndex );
	}
	CVertexArrays::GetInstance()->AddSubIndices( aSingleShortIndices );

	return GL_TRUE;
}