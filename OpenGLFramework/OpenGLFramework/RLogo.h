// RLogo.h: interface for the CRLogo class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CSpeedControl;
class CTextureLoader;
class CSDLFont;

#if _USE_LOAD_PROGESS_BAR_
	#define LEFT_PROGESS_BAR 20.0f		//pozycja paska postepu w procentach
	#define RIGHT_PROGESS_BAR 80.0f
#endif

class CRLogo : CRMenuBase
{
public:
	CRLogo();
	virtual ~CRLogo();

	GLvoid Draw();

private:
	CSpeedControl	*m_pSpeedControl;
	CTextureLoader	*m_pTextureLoader;
	CSDLFont		*m_pSDLFont;
#if _USE_LOAD_PROGESS_BAR_
	CSDLFont		*m_pSDLFontLoad;
#endif

	GLvoid DrawProgressBar();
	RECT m_rProgressBar;

	GLboolean m_bFirstDraw;
	GLint m_iTexLogo;
	GLint m_iTexMimperMusic;
	GLboolean m_bDrawPressAnyKey;

	CVector2* m_aTexCoord;
	CVector3* m_aIndices3;
	CVector2* m_aIndices2;
};