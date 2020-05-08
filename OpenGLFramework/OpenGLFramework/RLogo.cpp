// RLogo.cpp: implementation of the CRLogo class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "../Draw/TextureLoader.h"
#include "LoadProgressBar.h"
#include "RLogo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRLogo::CRLogo()
{
	m_pSpeedControl = new CSpeedControl();
	m_pTextureLoader = new CTextureLoader();
	m_iTexLogo = m_pTextureLoader->LoadTex( _T("textures/logo.jeh") );
	m_iTexMimperMusic = m_pTextureLoader->LoadTex( _T("textures/mimpermusic.jeh") );
	m_bFirstDraw = GL_TRUE;

	m_pSDLFont = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 20 );
#if _USE_LOAD_PROGESS_BAR_
	m_pSDLFontLoad = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 1 ), 14 );
#endif

	m_bDrawPressAnyKey = GL_FALSE;

	m_aTexCoord = new CVector2[ 4 ];
	m_aTexCoord[ 0 ] = CVector2( 1.0f, 0.0f );
	m_aTexCoord[ 1 ] = CVector2( 1.0f, 1.0f );
	m_aTexCoord[ 2 ] = CVector2( 0.0f, 0.0f );
	m_aTexCoord[ 3 ] = CVector2( 0.0f, 1.0f );

	m_aIndices3 = new CVector3[ 4 ];
	m_aIndices3[ 0 ] = CVector3( 2.0f, 0.0f, 0.0f );
	m_aIndices3[ 1 ] = CVector3( 2.0f, 2.0f, 0.0f );
	m_aIndices3[ 2 ] = CVector3(-2.0f, 0.0f, 0.0f );
	m_aIndices3[ 3 ] = CVector3(-2.0f, 2.0f, 0.0f );

	m_aIndices2 = new CVector2[ 4 ];
}

CRLogo::~CRLogo()
{
	delete m_pSpeedControl;
	delete m_pTextureLoader;
	delete m_pSDLFont;
#if _USE_LOAD_PROGESS_BAR_
	delete m_pSDLFontLoad;
#endif

	delete [] m_aTexCoord;
	delete [] m_aIndices3;
	delete [] m_aIndices2;
}

GLvoid CRLogo::Draw()
{
	if( m_bFirstDraw ) {
		m_bFirstDraw = GL_FALSE;
		m_pSpeedControl->CreateAnimationControl();
		m_pSpeedControl->CreateAnimationControl();
#if _USE_LOAD_PROGESS_BAR_
		//ustwienie regionu paska postepu
		SetRegionX( m_rProgressBar.left, LEFT_PROGESS_BAR );
		SetRegionX( m_rProgressBar.right, RIGHT_PROGESS_BAR );
		SetRegionY( m_rProgressBar.top, 92 );
		SetRegionY( m_rProgressBar.bottom, 95 );
#endif // _USE_LOAD_PROGESS_BAR_
	}

	glLoadIdentity();
	CGameControl::GetInstance()->Disable2D();
	glDisable( GL_LIGHTING );
	glDisable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );
	glTranslatef( 0.0f, 0.6f, -7.0f );
	glColor4ub( 255, 255, 255, 255 );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glTexCoordPointer( 2, GL_FLOAT, 0, m_aTexCoord );
	glVertexPointer( 3, GL_FLOAT, 0, m_aIndices3 );

	m_pTextureLoader->SetTexture( m_iTexLogo );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	glTranslatef( 0.0f, -1.5f, -2.2f );

	m_pTextureLoader->SetTexture( m_iTexMimperMusic );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	DrawProgressBar();

	m_pSDLFont->DrawText( 0, static_cast< GLint >( 7.68f * 70.0f ), RGB( 0, 255, 0 ), _T("Presents"), GL_TRUE );

#if _USE_LOAD_PROGESS_BAR_
	m_pSDLFontLoad->DrawText( 0, static_cast< GLint >( 7.68f * 95.0f ), RGB( 255, 255, 255 ), CLoadProgressBar::GetInstance()->GetLoadTextInfo(), GL_TRUE );

	if( !CLoadProgressBar::GetInstance()->IsLoadProgressEnd() ) {
		CLoadProgressBar::GetInstance()->LoadProgressRes();
		m_pSpeedControl->UpdateAnimationTime( 0 );
	}
	else {
#endif // _USE_LOAD_PROGESS_BAR_
		if( m_pSpeedControl->CheckAnimationTime( 7000, 0 ) ) {
			CGameControl::GetInstance()->SetEMainGameMode( EIntro );
			CGameControl::GetInstance()->DeleteLogo();
		}
		else {
			if( m_pSpeedControl->CheckAnimationTime( 400, 1 ) )
				m_bDrawPressAnyKey = !m_bDrawPressAnyKey;
			
			if( m_bDrawPressAnyKey )
				m_pSDLFont->DrawText( 0, static_cast< GLint >( 7.68f * 89.0f ), RGB( 255, 50, 50 ), LNG_LINE( 150 ), GL_TRUE );
		}
#if _USE_LOAD_PROGESS_BAR_
	}
#endif
}

GLvoid CRLogo::DrawProgressBar()
{
#if _USE_LOAD_PROGESS_BAR_
	CGameControl::GetInstance()->Enable2D();
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );

	//szerokosc w pikselach jednej jednoski:
	GLfloat fEntity = static_cast< GLfloat >( m_rProgressBar.right - m_rProgressBar.left ) / static_cast< GLfloat >( CLoadProgressBar::GetInstance()->GetLoadProgressMAX() );

	//rysuj pasek czerwony, na calej szerokosci
	glColor4ub( 128, 0, 0, 255 );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	m_aIndices2[ 0 ] = CVector2( ( GLfloat )m_rProgressBar.right, ( GLfloat )m_rProgressBar.bottom );
	m_aIndices2[ 1 ] = CVector2( ( GLfloat )m_rProgressBar.right, ( GLfloat )m_rProgressBar.top );
	m_aIndices2[ 2 ] = CVector2( ( GLfloat )m_rProgressBar.left, ( GLfloat )m_rProgressBar.bottom );
	m_aIndices2[ 3 ] = CVector2( ( GLfloat )m_rProgressBar.left, ( GLfloat )m_rProgressBar.top );

	glVertexPointer( 2, GL_FLOAT, 0, m_aIndices2 );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	//rysuj pasek zieony - ograniczony
	glColor4ub( 0, 255, 0, 255 );
	GLint iActualBarPos = m_rProgressBar.left + static_cast< GLint >( fEntity * static_cast< GLfloat >( CLoadProgressBar::GetInstance()->GetActualLoadProgress() ) );
	
	if( iActualBarPos > m_rProgressBar.right )
		iActualBarPos = m_rProgressBar.right;
	
	if( iActualBarPos > m_rProgressBar.left ) {
		m_aIndices2[ 0 ] = CVector2( ( GLfloat )iActualBarPos, ( GLfloat )m_rProgressBar.bottom );
		m_aIndices2[ 1 ] = CVector2( ( GLfloat )iActualBarPos, ( GLfloat )m_rProgressBar.top );
		m_aIndices2[ 2 ] = CVector2( ( GLfloat )m_rProgressBar.left, ( GLfloat )m_rProgressBar.bottom );
		m_aIndices2[ 3 ] = CVector2( ( GLfloat )m_rProgressBar.left, ( GLfloat )m_rProgressBar.top );

		glVertexPointer( 2, GL_FLOAT, 0, m_aIndices2 );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}
	//rysuje obramowanie
	glColor4ub( 128, 128, 128, 255 );

	m_aIndices2[ 0 ] = CVector2( ( GLfloat )m_rProgressBar.left, ( GLfloat )m_rProgressBar.top );
	m_aIndices2[ 1 ] = CVector2( ( GLfloat )m_rProgressBar.left, ( GLfloat )m_rProgressBar.bottom );
	m_aIndices2[ 2 ] = CVector2( ( GLfloat )m_rProgressBar.right, ( GLfloat )m_rProgressBar.bottom );
	m_aIndices2[ 3 ] = CVector2( ( GLfloat )m_rProgressBar.right, ( GLfloat )m_rProgressBar.top );

	glVertexPointer( 2, GL_FLOAT, 0, m_aIndices2 );
	glDrawArrays( GL_LINE_LOOP, 0, 4 );

#endif // _USE_LOAD_PROGESS_BAR_
}