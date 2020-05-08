#pragma once

#if _USE_MULTIPLYER_

#include "../Network/NetworkManager.h"
#include "RLanChat.h"
#include "RMenuBase.h"

class CSDLFont;
class CGameControl;
class CRLanChat;

#define MAX_REGIONS_MULTIPLAYER_MENU_MAIN 3
#define MAX_REGIONS_MULTIPLAYER_MENU_CREATE 4
#define MAX_REGIONS_MULTIPLAYER_MENU_CREATE_WAIT 1
#define MAX_REGIONS_MULTIPLAYER_MENU_JOIN 5
#define MAX_REGIONS_MULTIPLAYER_MENU_JOIN_WAIT 1

class CRMultiplayerMenu : public CRMenuBase
{
public:
	CRMultiplayerMenu();
	~CRMultiplayerMenu();

	//ustawienie regionow menu wzgledem rozdzielczosci
	GLvoid SetMouseRegion();
	GLvoid Draw();
	GLvoid RestartObjects();
	GLvoid SetStartVariables();

	GLboolean MouseWheel(GLint iScrollLines);
	GLboolean MouseLButtonDown(GLint iX, GLint iY);
	GLboolean KeyDown(GLuint uiKey);
	GLvoid Timer();

	inline LPCTSTR GetNickName() { return m_cNickName; }

	GLvoid AnalyseRecvPacket(SPacketUDP &sPacket);

	GLvoid SelectShipDone(GLint iIndexShip);

	// Gettery potrzebne do gry
	GLboolean GetOptionCollision()	{ return m_bOptionCollision; }
	GLboolean GetOptionSameShips()	{ return m_bOptionSameShips; }
	GLint GetMyIndexShip()			{ return m_iIndexShip; }
	GLint GetIndexShipAnother()		{ return m_iIndexShipAnother; }
	GLuint GetIPToConnect()			{ return m_uiIPToConnect; }
	GLboolean AmICreator()			{ return m_bIAmCreator; }

private:
	enum EMultiplayerSubMenu
	{
		EMultiplayerSubMenuMain,
		EMultiplayerSubMenuCreate,
		EMultiplayerSubMenuJoin,
		EMultiplayerSubMenuCreatorWaitToJoin,
		EMultiplayerSubMenuJoinWaitToAccept
	};

	/// Struktura przechwouj¹ca dane utworzonych gier.
	/** Informacje te wyœwietlane s¹ graczowi do³¹czaj¹cemu siê do gry. */
	struct SCreatedGame
	{
		char cCreatorNickname[MAX_MULTIPLAYER_NICKNAME];
		char cCreatorHostname[HOSTNAME_SIZE];
		unsigned char bOptionCollision;
		unsigned char bOptionSameShips;
		int iIndexShip; //indeks statku jakim bedzie gral "tworca gry"
		unsigned int uiCreatorIP; // adres IP "twórcy" gry
		unsigned int uiLastRef;
	};

	CGameControl	*m_pGameCtrl;
	CSDLFont		*m_pSDLFontTitle;
	CSDLFont		*m_pSDLFontMenu;
	CSDLFont		*m_pSDLFontUserNameStatic;
	CSDLFont		*m_pSDLFontUserName;
	CSDLFont		*m_pSDLFontSmall;
	CRLanChat		*m_pRLanChat;

	GLvoid SetRegions(RECT *pRegion, RECT *pRegion_Font, GLint iMaxRegions);

	RECT m_rRegionEnterYourName;
	RECT m_rRegionsMenuMain[MAX_REGIONS_MULTIPLAYER_MENU_MAIN];
	RECT m_rRegionsMenuMain_Font[MAX_REGIONS_MULTIPLAYER_MENU_MAIN];

	RECT m_rRegionsMenuCreate[MAX_REGIONS_MULTIPLAYER_MENU_CREATE];
	RECT m_rRegionsMenuCreate_Font[MAX_REGIONS_MULTIPLAYER_MENU_CREATE];

	RECT m_rRegionsMenuCreateWait[MAX_REGIONS_MULTIPLAYER_MENU_CREATE_WAIT];
	RECT m_rRegionsMenuCreateWait_Font[MAX_REGIONS_MULTIPLAYER_MENU_CREATE_WAIT];

	RECT m_rRegionsMenuJoin[MAX_REGIONS_MULTIPLAYER_MENU_JOIN];
	RECT m_rRegionsMenuJoin_Font[MAX_REGIONS_MULTIPLAYER_MENU_JOIN];

	RECT m_rRegionsMenuJoinWait[MAX_REGIONS_MULTIPLAYER_MENU_JOIN_WAIT];
	RECT m_rRegionsMenuJoinWait_Font[MAX_REGIONS_MULTIPLAYER_MENU_JOIN_WAIT];

	GLboolean m_bDrawCursor; //dla podaj pseudonim
	GLboolean m_bIsNickName;
	TCHAR m_cNickName[MAX_MULTIPLAYER_NICKNAME];

	GLboolean MouseLButtonDownMenuMain(GLint iX, GLint iY);
	GLboolean MouseLButtonDownMenuCreate(GLint iX, GLint iY);
	GLboolean MouseLButtonDownMenuJoin(GLint iX, GLint iY);
	GLboolean MouseLButtonDownMenuCreatorWaitToJoin(GLint iX, GLint iY);
	GLboolean MouseLButtonDownMenuJoinWaitToAccep(GLint iX, GLint iY);

	EMultiplayerSubMenu m_eActualMenu;
	GLvoid DrawMainMenu();
	GLvoid DrawMenuCreate();
	GLvoid DrawMenuJoin();
	GLvoid DrawMenuCreateWaitToJoin();
	GLvoid DrawMenuJoinWaitToAccept();

	// informacje potrzebne do gry
	GLboolean m_bOptionCollision;
	GLboolean m_bOptionSameShips;
	GLint m_iIndexShip;				// ID mojego statku
	GLint m_iIndexShipAnother;		// ID statku drugiego gracza
	GLboolean m_bIAmCreator;		// GL_TRUE jezeli jestem tworca, GL_FALSE jezeli jestem dolaczajacym sie
	GLuint m_uiIPToConnect;			// adres IP drugiego gracza

	GLvoid SendUDPCreateEcho();
	GLvoid SendUDPJoin();
	GLint m_iTimerSecCounterCreateEcho;

	std::vector< SCreatedGame > m_aCreatedGames;
	GLint GetCreatedGamesSize() { return static_cast< GLint >( m_aCreatedGames.size() ); }
	GLvoid AddCreatedGame(SPacketUDP &sPacket);
	GLvoid RemoveCreatedGame(GLint iIndex);
	GLvoid NotifyAboutJoin(SPacketUDP &sPacket);
	GLvoid NotifyAboutAcceptJoin();
	GLvoid NotifyAboutRejectJoin(const char* pNickname);
	GLvoid RunGameMultiplayer();
};

#endif // _USE_MULTIPLYER_