
#include "StdAfx.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "../Network/NetworkManager.h"
#include "RLanChat.h"

CRLanChat::CRLanChat()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_pSDLFont = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 1 ), 14, GL_TRUE );

	m_bDrawCursor = GL_FALSE;
	SetRegions();

	m_iStartLine = 0;
	m_iEndLine = 0;
	m_iPageCountLine = 10;
}

CRLanChat::~CRLanChat()
{
	ClearLanChatLines();
	delete m_pSDLFont;
}

GLvoid CRLanChat::ClearLanChatLines()
{
	for( GLint i = GetLanChatLines() - 1; i >= 0; --i )
		delete [] m_aLanChatLines[ i ];

	m_aLanChatLines.clear();
	m_iStartLine = 0;
	m_iEndLine = 0;
}

GLvoid CRLanChat::SetRegions()
{
	SetRegionX( m_rRegions[ 0 ].left, 5.0f );
	SetRegionX( m_rRegions[ 0 ].right, 95.0f );
	SetRegionY( m_rRegions[ 0 ].top, 15.0f );
	SetRegionY( m_rRegions[ 0 ].bottom, 40.0f );

	SetRegionX( m_rRegions[ 1 ].left, 5.0f );
	SetRegionX( m_rRegions[ 1 ].right, 95.0f );
	SetRegionY( m_rRegions[ 1 ].top, 41.0f );
	SetRegionY( m_rRegions[ 1 ].bottom, 45.0f );
}

GLvoid CRLanChat::RestartObjects()
{
	SetRegions();
}

GLvoid CRLanChat::SetStartVariables()
{
	::SecureZeroMemory( m_tcMessage, sizeof( m_tcMessage ) );
	ClearLanChatLines();
}

GLvoid CRLanChat::JoinUser( LPCTSTR lpUserName )
{
#ifdef UNICODE
	CUHF::ConvertWcharToChar( m_cUserName, MAX_MULTIPLAYER_NICKNAME, lpUserName );
#else
	strcpy_s( m_cUserName, MAX_MULTIPLAYER_NICKNAME, lpUserName );
#endif
	char cMsg[ 256 ];
	sprintf_s( cMsg, 256, "%s joined", m_cUserName );
	m_pGameCtrl->GetNetworkManager()->SendUDP( EPacketTypeJoinUser, cMsg, strlen( cMsg ) );
}

GLboolean CRLanChat::KeyDown( GLuint uiKey )
{
	switch( uiKey ) {
		case VK_RETURN:
		{
			//send
			char cMessage[ MAX_LANCHAT_MESSAGE_LNG ];
#ifdef UNICODE
			CUHF::ConvertWcharToChar( cMessage, MAX_LANCHAT_MESSAGE_LNG, m_tcMessage );
#else
			strcpy_s( cMessage, MAX_LANCHAT_MESSAGE_LNG, lpUserName );
#endif
			char cMsg[ 256 ];
			sprintf_s( cMsg, 256, "<%s> %s", m_cUserName, cMessage );
			m_pGameCtrl->GetNetworkManager()->SendUDP( EPacketTypeChatMsg, cMsg, strlen( cMsg ) );

			::SecureZeroMemory( m_tcMessage, sizeof( m_tcMessage ) );
			return GL_TRUE;
		}
		default:
			return m_pGameCtrl->WriteOnKeyboard( uiKey, m_tcMessage, MAX_LANCHAT_MESSAGE_LNG );
	}
}

GLboolean CRLanChat::MouseWheel( GLint iScrollLines )
{
	if( iScrollLines > 0 )
		LineScrollUp();
	else if( iScrollLines < 0 )
		LineScrollDown();

	return GL_TRUE;
}

GLvoid CRLanChat::Draw()
{
	glLoadIdentity();

	glDisable( GL_LIGHTING );

	//rysuj plansze-podklad
	DrawRegions( m_rRegions, MAX_REGIONS_LAN_CHAT );

	GLint iPosYConsole = static_cast< GLint >( 7.68f * 18.0f );
	for( GLint i = m_iStartLine; i < m_iEndLine; ++i ) {
		m_pSDLFont->DrawText( static_cast< GLint >( 10.24f * 6.0f ), iPosYConsole, RGB( 255, 255, 255 ), m_aLanChatLines[ i ] );
		iPosYConsole += static_cast< GLint >( 7.68f * 2.5f ); //skok linijki
	}

	//znak zachêty
	if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 500, 8 ) )
		m_bDrawCursor = !m_bDrawCursor;

	if( m_bDrawCursor )
		m_pSDLFont->DrawTextFormat( static_cast< GLint >( 10.24f * 6.0f ), static_cast< GLint >( 7.68f * 45.0f ),
									GL_FALSE, RGB( 255, 255, 255 ), _T("%s %s_"), LNG_LINE( 133 ), m_tcMessage );
	else
		m_pSDLFont->DrawTextFormat( static_cast< GLint >( 10.24f * 6.0f ), static_cast< GLint >( 7.68f * 45.0f ),
									GL_FALSE, RGB( 255, 255, 255 ), _T("%s %s "), LNG_LINE( 133 ), m_tcMessage );
}

GLvoid CRLanChat::AddMessage( const char *pMessage )
{
	TCHAR cTextUni[ 256 ];
#ifdef UNICODE
	CUHF::ConvertCharToWchar( cTextUni, 255, pMessage );
#else
	_tcscpy_s( cTextUni, 255, pMessage );
#endif

	m_aLanChatLines.push_back( new TCHAR[ _tcslen( cTextUni ) + 12 ] );
	GLint iIndex = GetLanChatLines() - 1;
	
	SYSTEMTIME sSysTime;
	::GetLocalTime( &sSysTime );
	::wsprintf( m_aLanChatLines[ iIndex ], _T("[%02d:%02d:%02d] %s"),
				sSysTime.wHour, sSysTime.wMinute, sSysTime.wSecond, cTextUni );

	if( GetLanChatLines() > m_iPageCountLine )
		++m_iStartLine;
	++m_iEndLine;

	SetOnLastLine();
}

GLvoid CRLanChat::SetOnLastLine() //ustawia wyswietlanie ostatnich linii
{
	m_iStartLine = 0;
	m_iEndLine = GetLanChatLines();
	if( GetLanChatLines() > m_iPageCountLine )
		m_iStartLine = GetLanChatLines() - m_iPageCountLine;
}

GLvoid CRLanChat::LineScrollUp()
{
	--m_iStartLine;
	--m_iEndLine;
	if( m_iStartLine < 0 ) {
		m_iStartLine = 0;
		if( GetLanChatLines() > m_iPageCountLine )
			m_iEndLine = m_iPageCountLine;
		else m_iEndLine = GetLanChatLines();
	}
	if( m_iEndLine < 0 ) m_iEndLine = 0;
}

GLvoid CRLanChat::LineScrollDown()
{
	++m_iStartLine;
	++m_iEndLine;
	if( m_iStartLine > GetLanChatLines() - m_iPageCountLine )
		m_iStartLine = GetLanChatLines() - m_iPageCountLine;
	if( m_iStartLine < 0 ) m_iStartLine = 0;
	if( m_iEndLine > GetLanChatLines() )
		m_iEndLine = GetLanChatLines();
}