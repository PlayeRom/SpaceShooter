#pragma once

#include "RMenuBase.h"

class CSDLFont;

#define MAX_LANCHAT_MESSAGE_LNG 100
#define MAX_REGIONS_LAN_CHAT 2

class CRLanChat : public CRMenuBase
{
public:
	CRLanChat();
	~CRLanChat();

	GLvoid SetRegions();
	GLvoid RestartObjects();
	GLvoid SetStartVariables();
	GLvoid JoinUser(LPCTSTR lpUserName);
	GLboolean KeyDown(GLuint uiKey);
	GLboolean MouseWheel(GLint iScrollLines);
	GLvoid Draw();

	GLvoid AddMessage(const char *pMessage);

private:
	CGameControl	*m_pGameCtrl;
	CSDLFont		*m_pSDLFont;

	TCHAR m_tcMessage[ MAX_LANCHAT_MESSAGE_LNG ];
	char m_cUserName[ MAX_MULTIPLAYER_NICKNAME ];

	GLboolean m_bDrawCursor;
	RECT m_rRegions[ MAX_REGIONS_LAN_CHAT ]; //0- plansza pod chat, 1 - plansza pod msg

	std::vector< TCHAR* > m_aLanChatLines;
	GLint GetLanChatLines() { return static_cast< GLint >( m_aLanChatLines.size() ); }
	GLvoid ClearLanChatLines();

	GLint m_iStartLine;
	GLint m_iEndLine;
	GLint m_iPageCountLine;		///< Iloœæ linii jak mo¿e byæ jednoczeœnie wyœwietlona.
	GLvoid SetOnLastLine();
	GLvoid LineScrollUp();
	GLvoid LineScrollDown();
};
