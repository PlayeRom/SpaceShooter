#pragma once

#include "SingletonBase.h"

#define MAX_LOAD_TEXT_INFO	128

class CLoadProgressBar : public CSingletonBase <CLoadProgressBar>
{
public:
	/// Statyczna metoda tworz¹ca obiekt klasy CLoadProgressBar. 
	/*! CGameControl jest singletonem, wiêc je¿eli wywo³amy tê metodê po raz drugi to zwróci ona wskaŸnik na obiekt
	ju¿ istniej¹cy, utworzony wczeœniej.
	\return WskaŸnik na obiekt CLoadProgressBar. */
	static CLoadProgressBar* Construct();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CLoadProgressBar();

	GLvoid Clear();

	GLint GetActualLoadProgress()							{ return m_iActualLoadProgress; }
	GLvoid IncreaseActualLoadProgress()						{ ++m_iActualLoadProgress; };
	GLint GetLoadProgressMAX()								{ return m_iLoadProgressMAX; };
	GLboolean IsLoadProgressEnd()							{ return m_bLoadProgressEnd; }
	GLvoid SetLoadProgressEnd(GLboolean bEnd)				{ m_bLoadProgressEnd = bEnd; }
	GLboolean IsLoadProgressFinishOK()						{ return m_bLoadProgressFinishOK; }
	GLvoid SetLoadProgressFinishOK( GLboolean bFinishOK )	{ m_bLoadProgressFinishOK = bFinishOK; }

	GLvoid SetLoadTextInfo( LPCTSTR lpLoadTextInfo );
	LPCTSTR GetLoadTextInfo()								{ return m_cLoadTextInfo; }

	GLvoid AddTexMimpmap( GLint* iId, TCHAR* lpFileName );
	GLvoid AddTex( GLint* iId, TCHAR* lpFileName );
	GLvoid AddTexLowQuality( GLint* iId, TCHAR* lpFileName );
	GLvoid AddMultiTexMaskMipmaps( GLint* iId, TCHAR* lpFileNameMask, TCHAR* lpFileNameRGB );
	GLvoid AddMultiTexMaskMipmaps( GLint* iId, TCHAR* lpFileNameMask, COLORREF crColor );
	GLvoid AddMultiTexMask( GLint* iId, TCHAR* lpFileNameMask, TCHAR* lpFileNameRGB );
	GLvoid AddMultiTexMask( GLint* iId, TCHAR* lpFileNameMask, COLORREF crColor );
	GLvoid AddMultiTexMaskLowQuality( GLint* iId, TCHAR* lpFileNameMask, TCHAR* lpFileNameRGB );
	GLvoid AddMultiTexMaskLowQuality( GLint* iId, TCHAR* lpFileNameMask, COLORREF crColor );
	GLvoid AddEmbossBump( GLint* iId, TCHAR* lpFileName );

	GLvoid Add3DObj( GLubyte byTexType,
					 GLint* iTexIndex,
					 GLint* iTexBumpIndex,
					 TCHAR* cTexLife,
					 TCHAR* cTexLifeMask,
					 GLint* iTexLife,
					 GLint* i3DObjIndex,
					 TCHAR* cObjectName,
					 COLORREF uiCOLORREF,
					 TCHAR* cTexBumpName,
					 TCHAR* cTexName );

	GLvoid AddSound( GLint* iId, char* lpFileName );
	GLvoid AddMusic( GLint* iId, char* lpFileName );

	GLvoid DoLoadProgressRes( GLboolean( *pFunc )( GLvoid ) );
	GLvoid LoadProgressRes();
	GLvoid LoadProgressResForRestart();

private:
	/// Konstruktor.
	/*! Konstruktor. */
	CLoadProgressBar();

	enum ETypeOfTex
	{
		ETypeMipmap,
		ETypeLinear,
		ETypeLowQuality,
		ETypeMaskMipmaps,
		ETypeMaskMipmapsColor,
		ETypeMaskLinear,
		ETypeMaskLinearColor,
		ETypeMaskLowQuality,
		ETypeMaskLowQualityColor,
		ETypeBump
	};

	struct STexRes
	{
		ETypeOfTex	eType;
		GLint* ipId;
		TCHAR* lpFileName1;
		union {
			TCHAR* lpFileName2;
			COLORREF crColor;
		};
	};

	struct S3DObjRes
	{
		GLubyte byTexType;
		GLint* iTexIndex;
		GLint* iTexBumpIndex;
		TCHAR* cTexLife;
		TCHAR* cTexLifeMask;
		GLint* iTexLife;
		GLint* i3DObjIndex;
		TCHAR* cObjectName;
		COLORREF uiCOLORREF;
		TCHAR* cTexBumpName;
		TCHAR* cTexName;
	};

	struct SSoundRes
	{
		GLint* ipId;
		char* lpFileName;
		GLboolean bMusic;
	};

	std::vector< STexRes > m_aTexRes;
	inline GLint GetTexResSize()	{ return static_cast< GLint >( m_aTexRes.size() ); }
	GLint m_iActualTexIndex;

	GLvoid AddTexture( ETypeOfTex eType, GLint* iId, TCHAR* lpFileName );
	GLvoid AddTexture( ETypeOfTex eType, GLint* iId, TCHAR* lpFileNameMask, TCHAR* lpFileNameRGB );
	GLvoid AddTexture( ETypeOfTex eType, GLint* iId, TCHAR* lpFileNameMask, COLORREF crColor );

	std::vector< S3DObjRes > m_a3DObjRes;
	inline GLint Get3DObjResSize()	{ return static_cast< GLint >( m_a3DObjRes.size() ); }
	GLint m_iActual3DObjIndex;

	std::vector<SSoundRes> m_aSoundsRes;
	inline GLint GetSoundResSize()	{ return static_cast< GLint >( m_aSoundsRes.size() ); }
	GLint m_iActualSoundIndex;

	GLboolean( *m_pFunc )( GLvoid );

	GLboolean LoadTexture();
	GLboolean Load3DObj();
	GLboolean LoadSound();
	GLboolean LoadMusic();

	GLint m_iActualLoadProgress;
	GLint m_iLoadProgressMAX;
	GLboolean m_bLoadProgressEnd;
	GLboolean m_bLoadProgressFinishOK;
	TCHAR m_cLoadTextInfo[ MAX_LOAD_TEXT_INFO ];

	GLboolean m_bStarted;

	HANDLE m_hDelayStartThread;
	unsigned int m_uiThreadId;
};
