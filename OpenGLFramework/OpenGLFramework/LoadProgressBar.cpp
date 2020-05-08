
#include "StdAfx.h"
#include "GameControl.h"
#include "..\Draw\TextureLoader.h"
#include "..\Draw\3DObjManager.h"
#include "..\Sound\OpenALManager.h"
#include "LoadProgressBar.h"

unsigned int __stdcall DelayStartProgress( void *lpParam );
GLboolean g_bCanStart = GL_FALSE;

CLoadProgressBar* CLoadProgressBar::Construct()
{
	return CSingletonBase::Construct( new CLoadProgressBar );
}

CLoadProgressBar::CLoadProgressBar()
{
	m_bStarted = GL_FALSE;
	m_pFunc = NULL;
}

CLoadProgressBar::~CLoadProgressBar()
{
	Clear();
}

GLvoid CLoadProgressBar::Clear()
{
	m_aTexRes.clear();
	m_aSoundsRes.clear();
	m_a3DObjRes.clear();
}

GLvoid CLoadProgressBar::AddTexMimpmap( GLint* iId, TCHAR* lpFileName )
{
	AddTexture( ETypeMipmap, iId, lpFileName );
}

GLvoid CLoadProgressBar::AddTex( GLint* iId, TCHAR* lpFileName )
{
	AddTexture( ETypeLinear, iId, lpFileName );
}

GLvoid CLoadProgressBar::AddTexLowQuality( GLint* iId, TCHAR* lpFileName )
{
	AddTexture( ETypeLowQuality, iId, lpFileName );
}

GLvoid CLoadProgressBar::AddMultiTexMaskMipmaps( GLint* iId, TCHAR* lpFileNameMask, TCHAR* lpFileNameRGB )
{
	AddTexture( ETypeMaskMipmaps, iId, lpFileNameMask, lpFileNameRGB );
}

GLvoid CLoadProgressBar::AddMultiTexMaskMipmaps( GLint* iId, TCHAR* lpFileNameMask, COLORREF crColor )
{
	AddTexture( ETypeMaskMipmapsColor, iId, lpFileNameMask, crColor );
}

GLvoid CLoadProgressBar::AddMultiTexMask( GLint* iId, TCHAR* lpFileNameMask, TCHAR* lpFileNameRGB )
{
	AddTexture( ETypeMaskLinear, iId, lpFileNameMask, lpFileNameRGB );
}

GLvoid CLoadProgressBar::AddMultiTexMask( GLint* iId, TCHAR* lpFileNameMask, COLORREF crColor )
{
	AddTexture( ETypeMaskLinearColor, iId, lpFileNameMask, crColor );
}

GLvoid CLoadProgressBar::AddMultiTexMaskLowQuality( GLint* iId, TCHAR* lpFileNameMask, TCHAR* lpFileNameRGB )
{
	AddTexture( ETypeMaskLowQuality, iId, lpFileNameMask, lpFileNameRGB );
}

GLvoid CLoadProgressBar::AddMultiTexMaskLowQuality( GLint* iId, TCHAR* lpFileNameMask, COLORREF crColor )
{
	AddTexture( ETypeMaskLowQualityColor, iId, lpFileNameMask, crColor );
}

GLvoid CLoadProgressBar::AddEmbossBump( GLint* iId, TCHAR* lpFileName )
{
	AddTexture( ETypeBump, iId, lpFileName );
}

GLvoid CLoadProgressBar::AddTexture( ETypeOfTex eType, GLint* iId, TCHAR* lpFileName )
{
	switch( eType ) {
		case ETypeMipmap:
		case ETypeLinear:
		case ETypeLowQuality:
		case ETypeBump:
			// typ OK
			break;
		default:
			::MessageBox( NULL, _T("CLoadProgressBar::AddTexture() - incorrect type"), _T("Error"), MB_OK | MB_ICONERROR );
			return;
	}

	STexRes sTexRes;
	sTexRes.eType = eType;
	sTexRes.ipId = iId;
	sTexRes.lpFileName1 = lpFileName;

	m_aTexRes.push_back( sTexRes );
}

GLvoid CLoadProgressBar::AddTexture( ETypeOfTex eType, GLint* iId, TCHAR* lpFileNameMask, TCHAR* lpFileNameRGB )
{
	switch( eType ) {
		case ETypeMipmap:
		case ETypeLinear:
		case ETypeLowQuality:
		case ETypeMaskMipmapsColor:
		case ETypeMaskLinearColor:
		case ETypeMaskLowQualityColor:
		case ETypeBump:
			::MessageBox( NULL, _T("CLoadProgressBar::AddTexture() - incorrect type"), _T("Error"), MB_OK | MB_ICONERROR );
			return;
	}

	STexRes sTexRes;
	sTexRes.eType = eType;
	sTexRes.ipId = iId;
	sTexRes.lpFileName1 = lpFileNameMask;
	sTexRes.lpFileName2 = lpFileNameRGB;

	m_aTexRes.push_back( sTexRes );
}

GLvoid CLoadProgressBar::AddTexture( ETypeOfTex eType, GLint* iId, TCHAR* lpFileNameMask, COLORREF crColor )
{
	switch( eType ) {
		case ETypeMipmap:
		case ETypeLinear:
		case ETypeLowQuality:
		case ETypeMaskMipmaps:
		case ETypeMaskLinear:
		case ETypeMaskLowQuality:
		case ETypeBump:
			::MessageBox( NULL, _T("CLoadProgressBar::AddTexture() - incorrect type"), _T("Error"), MB_OK | MB_ICONERROR );
			return;
	}

	STexRes sTexRes;
	sTexRes.eType = eType;
	sTexRes.ipId = iId;
	sTexRes.lpFileName1 = lpFileNameMask;
	sTexRes.crColor = crColor;

	m_aTexRes.push_back( sTexRes );
}

GLvoid CLoadProgressBar::Add3DObj( GLubyte byTexType,
								   GLint* iTexIndex,
								   GLint* iTexBumpIndex,
								   TCHAR* cTexLife,
								   TCHAR* cTexLifeMask,
								   GLint* iTexLife,
								   GLint* i3DObjIndex,
								   TCHAR* cObjectName,
								   COLORREF uiCOLORREF,
								   TCHAR* cTexBumpName,
								   TCHAR* cTexName )
{
	S3DObjRes s3DObj;
	s3DObj.byTexType = byTexType;
	s3DObj.iTexIndex = iTexIndex;
	s3DObj.iTexBumpIndex = iTexBumpIndex;
	s3DObj.cTexLife = cTexLife;
	s3DObj.cTexLifeMask = cTexLifeMask;
	s3DObj.iTexLife = iTexLife;
	s3DObj.i3DObjIndex = i3DObjIndex;
	s3DObj.cObjectName = cObjectName;
	s3DObj.uiCOLORREF = uiCOLORREF;
	s3DObj.cTexBumpName = cTexBumpName;
	s3DObj.cTexName = cTexName;

	m_a3DObjRes.push_back( s3DObj );
}

GLvoid CLoadProgressBar::AddSound( GLint* iId, char* lpFileName )
{
	SSoundRes sSoundRes;
	sSoundRes.ipId = iId;
	sSoundRes.lpFileName = lpFileName;
	sSoundRes.bMusic = GL_FALSE;

	m_aSoundsRes.push_back( sSoundRes );
}

GLvoid CLoadProgressBar::AddMusic( GLint* iId, char* lpFileName )
{
	SSoundRes sSoundRes;
	sSoundRes.ipId = iId;
	sSoundRes.lpFileName = lpFileName;
	sSoundRes.bMusic = GL_TRUE;

	m_aSoundsRes.push_back( sSoundRes );
}

GLvoid CLoadProgressBar::SetLoadTextInfo( LPCTSTR lpLoadTextInfo )
{
	::SecureZeroMemory( m_cLoadTextInfo, MAX_LOAD_TEXT_INFO );
	_tcscpy_s( m_cLoadTextInfo, MAX_LOAD_TEXT_INFO, lpLoadTextInfo ); 
}

//////////////////////////////////////////////////////////////////////////////
GLvoid CLoadProgressBar::DoLoadProgressRes( GLboolean( *pFunc )( GLvoid ) )
{
	m_pFunc = pFunc;

	m_iActualLoadProgress = 0;
	m_iLoadProgressMAX = GetTexResSize() + Get3DObjResSize() + GetSoundResSize();
	m_bLoadProgressEnd = GL_FALSE;
	m_bLoadProgressFinishOK = GL_FALSE;
	SetLoadTextInfo( LNG_LINE( 151 ) );

	m_iActualTexIndex = 0;
	m_iActualSoundIndex = 0;
	m_iActual3DObjIndex = 0;
	g_bCanStart = GL_FALSE;

	m_hDelayStartThread = NULL;

	m_bStarted = GL_TRUE;
}

GLvoid CLoadProgressBar::LoadProgressResForRestart()
{
	m_iActualTexIndex = 0;
	m_iActual3DObjIndex = 0;
	//m_iActualSoundIndex = 0;

	while( LoadTexture() );
	while( Load3DObj() );
	//while( LoadSound() );
}

GLvoid CLoadProgressBar::LoadProgressRes()
{
	if( !m_bStarted )
		return;

	if( m_iActualLoadProgress == 0 && !m_hDelayStartThread ) {
		GLint iDelay = 1500;
		m_hDelayStartThread = ( HANDLE )::_beginthreadex( NULL, 0, &DelayStartProgress, ( void* )iDelay, 0, &m_uiThreadId );
	}

	if( !g_bCanStart )
		return;

	if( LoadTexture() )
		return;
	if( Load3DObj() )
		return;
	if( LoadSound() )
		return;

	IncreaseActualLoadProgress();

	SetLoadTextInfo( LNG_LINE( 155 ) );
	GLboolean bResult = GL_FALSE;
	if( m_pFunc )
		bResult = m_pFunc();

	m_bLoadProgressEnd = GL_TRUE;
	m_bLoadProgressFinishOK = bResult;
}

unsigned int __stdcall DelayStartProgress( void *lpParam )
{
	GLint* iDelay = ( GLint* )&lpParam;

	::Sleep( *iDelay );
	g_bCanStart = GL_TRUE;

	return 0;
}
//////////////////////////////////////////////////////////////////////////////

GLboolean CLoadProgressBar::LoadTexture()
{
	if( m_iActualTexIndex >= GetTexResSize() )
		return GL_FALSE;

	CTextureLoader *pTexLoader = CGameControl::GetInstance()->GetTexLoader();
	if( !pTexLoader ) {
		__LOG( _T("ERROR: Load texture null pointer") );
		return GL_FALSE;
	}

	GLint iIndex = 0;
	GLint i = m_iActualTexIndex;
	switch( m_aTexRes[ i ].eType ) {
		case ETypeMipmap:
			iIndex = pTexLoader->LoadTexMipmaps( m_aTexRes[ i ].lpFileName1 );
			break;
		case ETypeLinear:
			iIndex = pTexLoader->LoadTex( m_aTexRes[ i ].lpFileName1 );
			break;
		case ETypeLowQuality:
			iIndex = pTexLoader->LoadTexLowQuality( m_aTexRes[ i ].lpFileName1 );
			break;
		case ETypeMaskMipmaps:
			iIndex = pTexLoader->LoadMultiTexMaskMipmaps( m_aTexRes[ i ].lpFileName1, m_aTexRes[ i ].lpFileName2 );
			break;
		case ETypeMaskMipmapsColor:
			iIndex = pTexLoader->LoadMultiTexMaskMipmaps( m_aTexRes[ i ].lpFileName1, m_aTexRes[ i ].crColor );
			break;
		case ETypeMaskLinear:
			iIndex = pTexLoader->LoadMultiTexMask( m_aTexRes[ i ].lpFileName1, m_aTexRes[ i ].lpFileName2 );
			break;
		case ETypeMaskLinearColor:
			iIndex = pTexLoader->LoadMultiTexMask( m_aTexRes[ i ].lpFileName1, m_aTexRes[ i ].crColor );
			break;
		case ETypeMaskLowQuality:
			iIndex = pTexLoader->LoadMultiTexMaskLowQuality( m_aTexRes[ i ].lpFileName1, m_aTexRes[ i ].lpFileName2 );
			break;
		case ETypeMaskLowQualityColor:
			iIndex = pTexLoader->LoadMultiTexMaskLowQuality( m_aTexRes[ i ].lpFileName1, m_aTexRes[ i ].crColor );
			break;
		case ETypeBump:
			iIndex = pTexLoader->LoadEmbossBump( m_aTexRes[ i ].lpFileName1 );
			break;
	}

	*m_aTexRes[ i ].ipId = iIndex;

	++m_iActualTexIndex;
	IncreaseActualLoadProgress();
	SetLoadTextInfo( LNG_LINE( 152 ) );

	return GL_TRUE;
}

GLboolean CLoadProgressBar::Load3DObj()
{
	if( m_iActual3DObjIndex >= Get3DObjResSize() )
		return GL_FALSE;

	CTextureLoader *pTexLoader = CGameControl::GetInstance()->GetTexLoader();
	if( !pTexLoader ) {
		__LOG( _T("ERROR: Load 3DObj null pointer") );
		return GL_FALSE;
	}

	GLint i = m_iActual3DObjIndex;

	GLint iTexIndex = 0;
	switch( m_a3DObjRes[ i ].byTexType ) {
		case 2: //bez tekstury
			iTexIndex = -1;
			break;
		case 1:
			iTexIndex = pTexLoader->LoadMultiTexMaskMipmaps( m_a3DObjRes[ i ].cTexName, m_a3DObjRes[ i ].uiCOLORREF );
			break;
		default: //0
			iTexIndex = pTexLoader->LoadTexMipmaps( m_a3DObjRes[ i ].cTexName );
			//sprawdz czy jest textura bump
			if( _tcslen( m_a3DObjRes[ i ].cTexBumpName ) > 0 )
				*m_a3DObjRes[ i ].iTexBumpIndex = pTexLoader->LoadEmbossBump( m_a3DObjRes[ i ].cTexBumpName );
			else
				*m_a3DObjRes[ i ].iTexBumpIndex = -1;
			break;
	}
	*m_a3DObjRes[ i ].iTexIndex = iTexIndex;

	//czy jest tekstura z "zyciem"
	if( _tcslen( m_a3DObjRes[ i ].cTexLife ) > 0 && _tcslen( m_a3DObjRes[ i ].cTexLifeMask ) > 0 )
		*m_a3DObjRes[ i ].iTexLife = pTexLoader->LoadMultiTexMaskMipmaps( m_a3DObjRes[ i ].cTexLifeMask,
																		  m_a3DObjRes[ i ].cTexLife );
	else
		*m_a3DObjRes[ i ].iTexLife = 0;

	//wczytaj obiekt
	C3DObjManager *p3DObjMgr = CGameControl::GetInstance()->Get3DObjManager();
	*m_a3DObjRes[ i ].i3DObjIndex = p3DObjMgr->Create3DObject( m_a3DObjRes[ i ].cObjectName,
															   *m_a3DObjRes[ i ].iTexIndex,
															   -1,
															   *m_a3DObjRes[ i ].iTexBumpIndex );

	++m_iActual3DObjIndex;
	IncreaseActualLoadProgress();
	SetLoadTextInfo( LNG_LINE( 153 ) );

	return GL_TRUE;
}

GLboolean CLoadProgressBar::LoadSound()
{
	if( m_iActualSoundIndex >= GetSoundResSize() )
		return GL_FALSE;

	COpenALManager *pPointer = CGameControl::GetInstance()->GetOpenALManager();
	if( !pPointer ) {
		__LOG( _T("ERROR: Load sound null pointer") );
		return GL_FALSE;
	}

	GLboolean bMusic = m_aSoundsRes[ m_iActualSoundIndex ].bMusic;
	GLint iIndex = pPointer->LoadSound( m_aSoundsRes[ m_iActualSoundIndex ].lpFileName, bMusic, bMusic );
	*m_aSoundsRes[ m_iActualSoundIndex ].ipId = iIndex;

	++m_iActualSoundIndex;
	IncreaseActualLoadProgress();
	SetLoadTextInfo( LNG_LINE( 154 ) );

	return GL_TRUE;
}