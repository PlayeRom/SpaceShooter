
#include "StdAfx.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "RShipsBrowse.h"
#include "ShipsManager.h"
#include "Cursor.h"
#include "RGame.h"
#include "RMultiplayerMenu.h"

#if _USE_MULTIPLYER_

//blokuje odbior nowych SPacketJoin, czekajac az "tworca gry" zatwierdzi badz odrzuci aktualna proble polaczenia
GLboolean g_bBlockNewJoin = GL_FALSE;

void NotifyAboutJoinYES( void );
void NotifyAboutJoinNO( void );

CRMultiplayerMenu::CRMultiplayerMenu()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_pSDLFontTitle				= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 50 );
	m_pSDLFontMenu				= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 23 );
	m_pSDLFontUserNameStatic	= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 40, GL_TRUE );
	m_pSDLFontUserName			= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 2 ), 40, GL_TRUE );
	m_pSDLFontSmall				= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 1 ), 14, GL_TRUE );

	m_pRLanChat = new CRLanChat();

	SetMouseRegion();
}

CRMultiplayerMenu::~CRMultiplayerMenu()
{
	delete m_pSDLFontTitle;
	delete m_pSDLFontMenu;
	delete m_pSDLFontUserNameStatic;
	delete m_pSDLFontUserName;
	delete m_pSDLFontSmall;
	delete m_pRLanChat;
}

GLvoid CRMultiplayerMenu::SetRegions( RECT *pRegion, RECT *pRegion_Font, GLint iMaxRegions )
{
	GLint iPercent = 48;
	for( GLint i = 0; i < iMaxRegions; ++i ) {
		if( i == iMaxRegions - 1 ) //Wróc - na samym dole
			iPercent = 84;
		SetRegionX( pRegion[ i ].left, 25 );
		SetRegionX( pRegion[ i ].right, 75 );

		SetRegionX( pRegion_Font[ i ].left, 25, GL_TRUE );
		SetRegionX( pRegion_Font[ i ].right, 75, GL_TRUE );

		SetRegionY( pRegion[ i ].top, iPercent );
		SetRegionY( pRegion_Font[ i ].top, iPercent, GL_TRUE );
		iPercent += 7;
		SetRegionY( pRegion[ i ].bottom, iPercent );
		SetRegionY( pRegion_Font[ i ].bottom, iPercent, GL_TRUE );
		iPercent += 2;
	}
}

GLvoid CRMultiplayerMenu::SetMouseRegion()
{
	// dla regionow menu glownego
	SetRegions( m_rRegionsMenuMain, m_rRegionsMenuMain_Font, MAX_REGIONS_MULTIPLAYER_MENU_MAIN );

	// dla regionow "Create the game"
	SetRegions( m_rRegionsMenuCreate, m_rRegionsMenuCreate_Font, MAX_REGIONS_MULTIPLAYER_MENU_CREATE );

	// dla regionow "Tworca czeka na dolaczenie dolanczajacego"
	SetRegions( m_rRegionsMenuCreateWait, m_rRegionsMenuCreateWait_Font, MAX_REGIONS_MULTIPLAYER_MENU_CREATE_WAIT );

	// dla regionow "Join to game"
	SetRegions( m_rRegionsMenuJoin, m_rRegionsMenuJoin_Font, MAX_REGIONS_MULTIPLAYER_MENU_JOIN );

	// dla regionow "Join czeka na akceptacje"
	SetRegions( m_rRegionsMenuJoinWait, m_rRegionsMenuJoinWait_Font, MAX_REGIONS_MULTIPLAYER_MENU_JOIN_WAIT );

	SetRegionX( m_rRegionEnterYourName.left, 25 );
	SetRegionX( m_rRegionEnterYourName.right, 75 );
	SetRegionY( m_rRegionEnterYourName.top, 32 );
	SetRegionY( m_rRegionEnterYourName.bottom, 65 );
}

GLvoid CRMultiplayerMenu::RestartObjects()
{
	SetMouseRegion();
	m_pRLanChat->RestartObjects();
}

GLvoid CRMultiplayerMenu::SetStartVariables()
{
	m_eActualMenu = EMultiplayerSubMenuMain;
	m_bIsNickName = GL_FALSE;
	m_bDrawCursor = GL_FALSE;
	m_bOptionCollision = GL_TRUE;
	m_bOptionSameShips = GL_FALSE;
	m_bIAmCreator = GL_FALSE;
	m_iIndexShip = -1;
	m_iIndexShipAnother = -1;
	m_iTimerSecCounterCreateEcho = 0;
	::SecureZeroMemory( m_cNickName, sizeof( m_cNickName ) );
	m_pRLanChat->SetStartVariables();
	m_pGameCtrl->GetNetworkManager()->CreateUDPSocket();
	m_pGameCtrl->GetNetworkManager()->StartRecvUDPThread();
}

GLboolean CRMultiplayerMenu::KeyDown( GLuint uiKey )
{
	if( !m_bIsNickName ) {
		switch( uiKey ) {
			case VK_ESCAPE:
				m_pGameCtrl->SetEMainGameMode( EMainMenu );
				return GL_TRUE;

			case VK_RETURN:
				if( ::GetStringLength( m_cNickName ) < 3 ) {
					m_pGameCtrl->GetRMessageBox()->DoMessageBox( LNG_LINE( 58 ), CRMessageBox::EMsgOK, NULL, NULL );
					return GL_FALSE;
				}
				m_bIsNickName = GL_TRUE;
				m_pRLanChat->JoinUser( m_cNickName );
				return GL_TRUE;

			default:
				return m_pGameCtrl->WriteOnKeyboard( uiKey, m_cNickName, MAX_MULTIPLAYER_NICKNAME );
		}
	}

	switch( uiKey ) {
		case VK_ESCAPE:
		{
			switch( m_eActualMenu ) {
				case EMultiplayerSubMenuCreate:
				case EMultiplayerSubMenuJoin:
					m_bIAmCreator = GL_FALSE;
					m_eActualMenu = EMultiplayerSubMenuMain;
					return GL_TRUE;
				case EMultiplayerSubMenuCreatorWaitToJoin:
					m_eActualMenu = EMultiplayerSubMenuCreate;
					return GL_TRUE;
				case EMultiplayerSubMenuJoinWaitToAccept:
					m_eActualMenu = EMultiplayerSubMenuJoin;
					return GL_TRUE;
				default: //EMultiplayerSubMenuMain
					m_pGameCtrl->GetNetworkManager()->StopRecvUDPThread();
					m_pGameCtrl->SetEMainGameMode( EMainMenu );
					return GL_TRUE;
			}
		}
	}

	return m_pRLanChat->KeyDown( uiKey );
}

GLboolean CRMultiplayerMenu::MouseWheel( GLint iScrollLines )
{
	return m_pRLanChat->MouseWheel( iScrollLines );
}

GLboolean CRMultiplayerMenu::MouseLButtonDown( GLint iX, GLint iY )
{
	switch( m_eActualMenu ) {
		case EMultiplayerSubMenuCreate:
			return MouseLButtonDownMenuCreate( iX, iY );
		case EMultiplayerSubMenuJoin:
			return MouseLButtonDownMenuJoin( iX, iY );
		case EMultiplayerSubMenuCreatorWaitToJoin:
			return MouseLButtonDownMenuCreatorWaitToJoin( iX, iY );
		case EMultiplayerSubMenuJoinWaitToAccept:
			return MouseLButtonDownMenuJoinWaitToAccep( iX, iY );
		default: //EMultiplayerSubMenuMain
			return MouseLButtonDownMenuMain( iX, iY );
	}
}

GLboolean CRMultiplayerMenu::MouseLButtonDownMenuMain( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegionsMenuMain, MAX_REGIONS_MULTIPLAYER_MENU_MAIN, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //Utworz gre
			m_eActualMenu = EMultiplayerSubMenuCreate;
			return GL_TRUE;
		case 1: //Dolacz
			m_eActualMenu = EMultiplayerSubMenuJoin;
			return GL_TRUE;
		case 2: //back
			m_pGameCtrl->GetNetworkManager()->StopRecvUDPThread();
			m_pGameCtrl->SetEMainGameMode( EMainMenu );
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CRMultiplayerMenu::MouseLButtonDownMenuCreate( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegionsMenuCreate, MAX_REGIONS_MULTIPLAYER_MENU_CREATE, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: // opcja - czy kolizje miedzy graczami
			m_bOptionCollision = !m_bOptionCollision;
			return GL_TRUE;
		case 1: // opcja - czy gra tym samym statkiem
			m_bOptionSameShips = !m_bOptionSameShips;
			return GL_TRUE;
		case 2: // Select your ship
			m_pGameCtrl->GetRShipsBrowse()->SetStartVariables();
			m_pGameCtrl->GetRShipsBrowse()->SetMode( EBrowseMultiplayer );
			m_pGameCtrl->SetEMainGameMode( EShipsBrowser );
			break;
		case 3: //back
			m_bIAmCreator = GL_FALSE;
			m_eActualMenu = EMultiplayerSubMenuMain;
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CRMultiplayerMenu::MouseLButtonDownMenuJoin( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegionsMenuJoin, MAX_REGIONS_MULTIPLAYER_MENU_JOIN, i ) )
		return GL_FALSE;

	if( i >= GetCreatedGamesSize() && i != MAX_REGIONS_MULTIPLAYER_MENU_JOIN - 1 )
		return GL_FALSE;

	if( i == MAX_REGIONS_MULTIPLAYER_MENU_JOIN - 1 ) { //back
		m_eActualMenu = EMultiplayerSubMenuMain;
		return GL_TRUE;
	}

	// kliknieto na slot
	m_bOptionCollision = m_aCreatedGames[ i ].bOptionCollision;
	m_bOptionSameShips = m_aCreatedGames[ i ].bOptionSameShips;
	m_uiIPToConnect = m_aCreatedGames[ i ].uiCreatorIP;
	m_iIndexShipAnother = m_aCreatedGames[ i ].iIndexShip;
	if( m_aCreatedGames[ i ].bOptionSameShips ) {
		//bez wyboru statku
		SelectShipDone( m_aCreatedGames[ i ].iIndexShip );
	}
	else {
		//gracz wybiera statek
		m_pGameCtrl->GetRShipsBrowse()->SetStartVariables();
		m_pGameCtrl->GetRShipsBrowse()->SetMode( EBrowseMultiplayer );
		m_pGameCtrl->SetEMainGameMode( EShipsBrowser ); // <- w CRShipsBrowse wywola sie SelectShipDone()
	}

	return GL_TRUE;
}

GLboolean CRMultiplayerMenu::MouseLButtonDownMenuCreatorWaitToJoin( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegionsMenuCreateWait, MAX_REGIONS_MULTIPLAYER_MENU_CREATE_WAIT, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //"Anuluj czekanie"
			m_eActualMenu = EMultiplayerSubMenuCreate;
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CRMultiplayerMenu::MouseLButtonDownMenuJoinWaitToAccep( GLint iX, GLint iY )
{
	GLint i = 0;
	if( !CheckWhereClickedUser( iX, iY, m_rRegionsMenuJoinWait, MAX_REGIONS_MULTIPLAYER_MENU_JOIN_WAIT, i ) )
		return GL_FALSE;

	switch( i ) {
		case 0: //"Anuluj czekanie"
			m_eActualMenu = EMultiplayerSubMenuJoin;
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CRMultiplayerMenu::Draw()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	if( !m_bIsNickName ) {	//najpierw podaj imie
		//rysuj plansze-podklad
		DrawRegions( &m_rRegionEnterYourName, 1 );

		m_pSDLFontUserNameStatic->DrawText( 0, static_cast< GLint >( 7.68f * 45.0f ), RGB( 32, 255, 32 ), LNG_LINE( 55 ), GL_TRUE );

		//znak zachêty
		if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 500, 8 ) )
			m_bDrawCursor = !m_bDrawCursor;

		if( m_bDrawCursor )
			m_pSDLFontUserName->DrawTextFormat( 0, static_cast< GLint >( 7.68f * 55.0f ), GL_TRUE, RGB( 32, 255, 32 ), _T("%s_"), m_cNickName );
		else
			m_pSDLFontUserName->DrawTextFormat( 0, static_cast< GLint >( 7.68f * 55.0f ), GL_TRUE, RGB( 32, 255, 32 ), _T("%s "), m_cNickName );
	}
	else
		m_pRLanChat->Draw();

	switch( m_eActualMenu ) {
		case EMultiplayerSubMenuCreate:
			DrawMenuCreate();
			break;
		case EMultiplayerSubMenuJoin:
			DrawMenuJoin();
			break;
		case EMultiplayerSubMenuCreatorWaitToJoin:
			DrawMenuCreateWaitToJoin();
			break;
		case EMultiplayerSubMenuJoinWaitToAccept:
			DrawMenuJoinWaitToAccept();
			break;
		default: //EMultiplayerSubMenuMain
			DrawMainMenu();
			break;
	}
}

GLvoid CRMultiplayerMenu::DrawMainMenu()
{
	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), LNG_LINE( 4 ), GL_TRUE );

	//rysujemy przyciski
	if( !m_bIsNickName )
		DrawRegions( m_rRegionsMenuMain, MAX_REGIONS_MULTIPLAYER_MENU_MAIN, MAX_REGIONS_MULTIPLAYER_MENU_MAIN - 1 );
	else
		DrawRegions( m_rRegionsMenuMain, MAX_REGIONS_MULTIPLAYER_MENU_MAIN );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 0; i < MAX_REGIONS_MULTIPLAYER_MENU_MAIN; ++i ) {
		if( !m_bIsNickName && i < MAX_REGIONS_MULTIPLAYER_MENU_MAIN - 1 )
			continue; //dla enter your name rysuj tylko przycisk powrotu

		if( CheckPtCursorInRect( m_pGameCtrl->GetPointCursor(), m_rRegionsMenuMain[ i ] ) ) {
			bPtCursorInRect = GL_TRUE;
			m_iWhereIsMouse = i;
			DrawRegionIfMouse( m_rRegionsMenuMain[ i ] );
		}
		else {
			bPtCursorInRect = GL_FALSE;
			if( m_iWhereIsMouse == i )
				m_iWhereIsMouseOld = -1; //nigdzie
		}

		if( i == MAX_REGIONS_MULTIPLAYER_MENU_MAIN - 1 ) { //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
			m_pSDLFontMenu->DrawText( m_rRegionsMenuMain_Font[ i ], crColor, LNG_LINE( 16 ) );
		}
		else {
			crColor = bPtCursorInRect ? RGB( 50, 255, 50 ) : RGB( 128, 128, 128 );
			m_pSDLFontMenu->DrawText( m_rRegionsMenuMain_Font[ i ], crColor, LNG_LINE( i + 130 ) );
		}
	}
}

GLvoid CRMultiplayerMenu::DrawMenuCreate()
{
	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), LNG_LINE( 134 ), GL_TRUE );

	//rysujemy przyciski
	DrawRegions( m_rRegionsMenuCreate, MAX_REGIONS_MULTIPLAYER_MENU_CREATE );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 0; i < MAX_REGIONS_MULTIPLAYER_MENU_CREATE; ++i ) {
		if( CheckPtCursorInRect( m_pGameCtrl->GetPointCursor(), m_rRegionsMenuCreate[ i ] ) ) {
			bPtCursorInRect = GL_TRUE;
			m_iWhereIsMouse = i;
			DrawRegionIfMouse( m_rRegionsMenuCreate[ i ] );
		}
		else {
			bPtCursorInRect = GL_FALSE;
			if( m_iWhereIsMouse == i )
				m_iWhereIsMouseOld = -1; //nigdzie
		}

		if( i == MAX_REGIONS_MULTIPLAYER_MENU_CREATE - 1 ) { //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
			m_pSDLFontMenu->DrawText( m_rRegionsMenuCreate_Font[ i ], crColor, LNG_LINE( 16 ) );
		}
		else if( i == MAX_REGIONS_MULTIPLAYER_MENU_CREATE - 2 ) { //"Ready" lekko na zielono
			crColor = bPtCursorInRect ? RGB( 0, 255, 0 ) : RGB( 50, 255, 50 );
			m_pSDLFontMenu->DrawText( m_rRegionsMenuCreate_Font[ i ], crColor, LNG_LINE( 142 ) );
		}
		else {
			crColor = bPtCursorInRect ? RGB( 50, 255, 50 ) : RGB( 128, 128, 128 );

			GLint iYesNo = 10; //tak
			if( i == 0 )
				iYesNo = m_bOptionCollision ? 10 : 11;
			else if( i == 1 )
				iYesNo = m_bOptionSameShips ? 10 : 11;
			
			m_pSDLFontMenu->DrawTextFormat( m_rRegionsMenuCreate_Font[ i ], crColor, LNG_LINE( i + 135 ), LNG_LINE( iYesNo ) );
		}
	}
}

GLvoid CRMultiplayerMenu::DrawMenuJoin()
{
	// rysuj listê stworzonych gier

	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), LNG_LINE( 4 ), GL_TRUE );

	//rysujemy przyciski
	DrawRegions( m_rRegionsMenuJoin, MAX_REGIONS_MULTIPLAYER_MENU_JOIN );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 0; i < MAX_REGIONS_MULTIPLAYER_MENU_JOIN; ++i ) {
		if( CheckPtCursorInRect( m_pGameCtrl->GetPointCursor(), m_rRegionsMenuJoin[ i ] )
			&& ( i < GetCreatedGamesSize() || i == MAX_REGIONS_MULTIPLAYER_MENU_JOIN - 1 ) )
		{
			bPtCursorInRect = GL_TRUE;
			m_iWhereIsMouse = i;
			DrawRegionIfMouse( m_rRegionsMenuJoin[ i ] );
		}
		else {
			bPtCursorInRect = GL_FALSE;
			if( m_iWhereIsMouse == i )
				m_iWhereIsMouseOld = -1; //nigdzie
		}

		if( i == MAX_REGIONS_MULTIPLAYER_MENU_JOIN - 1 ) { //wyjscie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
			m_pSDLFontMenu->DrawText( m_rRegionsMenuJoin_Font[ i ], crColor, LNG_LINE( 16 ) );
		}
		else {
			crColor = bPtCursorInRect ? RGB( 50, 255, 50 ) : RGB( 128, 128, 128 );

			if( i < GetCreatedGamesSize() ) {
				m_pSDLFontSmall->DrawTextFormat( 0,
					m_rRegionsMenuJoin_Font[ i ].bottom - static_cast< GLint >( 7.68f * 3.0f ),
					GL_TRUE, crColor, LNG_LINE( 145 ),
					::ConvertCharToWchar( m_aCreatedGames[ i ].cCreatorNickname, MAX_MULTIPLAYER_NICKNAME ),
					::ConvertCharToWchar( m_aCreatedGames[ i ].cCreatorHostname, HOSTNAME_SIZE ),
					m_pGameCtrl->GetNetworkManager()->IPntoaw( m_aCreatedGames[ i ].uiCreatorIP ) );

				m_pSDLFontSmall->DrawTextFormat( 0,
					m_rRegionsMenuJoin_Font[ i ].bottom,
					GL_TRUE, crColor, LNG_LINE( 146 ),
					m_aCreatedGames[ i ].bOptionCollision ? LNG_LINE( 10 ) : LNG_LINE( 11 ),
					m_aCreatedGames[ i ].bOptionSameShips ? LNG_LINE( 10 ) : LNG_LINE( 11 ),
					m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_aCreatedGames[ i ].iIndexShip ).cShipName );
			}
			else
				m_pSDLFontSmall->DrawText( m_rRegionsMenuJoin_Font[ i ], crColor, LNG_LINE( 147 ) );
		}
	}
}

GLvoid CRMultiplayerMenu::DrawMenuCreateWaitToJoin()
{
	// gdy tworca gry czeka na dolaczajacego
	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), LNG_LINE( 4 ), GL_TRUE ) ;
	m_pSDLFontMenu->SetBkgText( GL_TRUE, RGB( 0, 0, 0 ) );
	m_pSDLFontMenu->DrawText( 0, static_cast< GLint >( 7.68f * 60.0f ), RGB( 50, 255, 50 ), LNG_LINE( 143 ), GL_TRUE );
	m_pSDLFontMenu->SetBkgText( GL_FALSE );

	//rysujemy przyciski
	DrawRegions( m_rRegionsMenuCreateWait, MAX_REGIONS_MULTIPLAYER_MENU_CREATE_WAIT );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 0; i < MAX_REGIONS_MULTIPLAYER_MENU_CREATE_WAIT; ++i ) {
		if( CheckPtCursorInRect( m_pGameCtrl->GetPointCursor(), m_rRegionsMenuCreateWait[ i ] ) ) {
			bPtCursorInRect = GL_TRUE;
			m_iWhereIsMouse = i;
			DrawRegionIfMouse( m_rRegionsMenuCreateWait[ i ] );
		}
		else {
			bPtCursorInRect = GL_FALSE;
			if( m_iWhereIsMouse == i )
				m_iWhereIsMouseOld = -1; //nigdzie
		}

		if( i == MAX_REGIONS_MULTIPLAYER_MENU_CREATE_WAIT - 1 ) { //"anuluj" lekko na czerwono
			if( bPtCursorInRect )
				crColor = RGB( 255, 0, 0 );
			else
				crColor = RGB( 255, 50, 50 );

			m_pSDLFontMenu->DrawText( m_rRegionsMenuCreateWait_Font[ i ], crColor, LNG_LINE( 9 ) );
		}
	}
}

GLvoid CRMultiplayerMenu::DrawMenuJoinWaitToAccept()
{
	// gdy dolaczajacy czeka na akceptacje polaczenia
	m_pSDLFontTitle->DrawText( 0, static_cast< GLint >( 7.68f * 13.0f ), RGB( 100, 100, 255 ), LNG_LINE( 4 ), GL_TRUE );
	m_pSDLFontMenu->SetBkgText( GL_TRUE, RGB( 0, 0, 0 ) );
	m_pSDLFontMenu->DrawText( 0, static_cast< GLint >( 7.68f * 60.0f ), RGB( 50, 255, 50 ), LNG_LINE( 144 ), GL_TRUE );
	m_pSDLFontMenu->SetBkgText( GL_FALSE );

	//rysujemy przyciski
	DrawRegions( m_rRegionsMenuJoinWait, MAX_REGIONS_MULTIPLAYER_MENU_JOIN_WAIT );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;

	for( GLint i = 0; i < MAX_REGIONS_MULTIPLAYER_MENU_JOIN_WAIT; ++i ) {
		if( CheckPtCursorInRect( m_pGameCtrl->GetPointCursor(), m_rRegionsMenuJoinWait[ i ] ) ) {
			bPtCursorInRect = GL_TRUE;
			m_iWhereIsMouse = i;
			DrawRegionIfMouse( m_rRegionsMenuJoinWait[ i ] );
		}
		else {
			bPtCursorInRect = GL_FALSE;
			if( m_iWhereIsMouse == i )
				m_iWhereIsMouseOld = -1; //nigdzie
		}

		if( i == MAX_REGIONS_MULTIPLAYER_MENU_JOIN_WAIT - 1 ) { //"anuluj" lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );
			m_pSDLFontMenu->DrawText( m_rRegionsMenuJoinWait_Font[ i ], crColor, LNG_LINE( 9 ) );
		}
	}
}

GLvoid CRMultiplayerMenu::SelectShipDone( GLint iIndexShip )
{
	m_iIndexShip = iIndexShip;
	switch( m_eActualMenu ) {
		case EMultiplayerSubMenuCreate:
			m_bIAmCreator = GL_TRUE;
			m_eActualMenu = EMultiplayerSubMenuCreatorWaitToJoin;
			SendUDPCreateEcho();
			break;

		case EMultiplayerSubMenuJoin:
			m_bIAmCreator = GL_FALSE;
			m_eActualMenu = EMultiplayerSubMenuJoinWaitToAccept;
			SendUDPJoin();
			break;
	}
}

GLvoid CRMultiplayerMenu::Timer()
{
	if( m_eActualMenu == EMultiplayerSubMenuCreatorWaitToJoin ) {
		if( ++m_iTimerSecCounterCreateEcho == 5 ) {
			m_iTimerSecCounterCreateEcho = 0;
			SendUDPCreateEcho();
		}
	}

	//sprawdzaj stare stworzone gry i usuwaj przestarzale
	GLuint uiCurrentSeconds = ::timeGetTime() / 1000;
	for( GLint i = GetCreatedGamesSize() - 1; i >= 0; --i ) {
		if( uiCurrentSeconds - 6 <= m_aCreatedGames[ i ].uiLastRef )
			continue;
	
		RemoveCreatedGame( i );
	}
}

GLvoid CRMultiplayerMenu::SendUDPCreateEcho()
{
	// pakiet wysy³any co 5 sek. informujacy ze "tworca" zalozyl gre i czeka na polaczenie
	if( m_eActualMenu != EMultiplayerSubMenuCreatorWaitToJoin )
		return;

	SPacketCreateEcho sCreateEcho = { 0 };
	::ConvertWcharToChar( sCreateEcho.cCreatorNickname, MAX_MULTIPLAYER_NICKNAME, m_cNickName );
	strcpy_s( sCreateEcho.cCreatorHostname, HOSTNAME_SIZE, m_pGameCtrl->GetNetworkManager()->GetHostName() );
	sCreateEcho.bOptionCollision = m_bOptionCollision;
	sCreateEcho.bOptionSameShips = m_bOptionSameShips;
	sCreateEcho.iIndexShip = m_iIndexShip;

	m_pGameCtrl->GetNetworkManager()->SendUDP( EPacketTypeCreateEcho, ( const void* )&sCreateEcho, sizeof( SPacketCreateEcho ) );
}

GLvoid CRMultiplayerMenu::SendUDPJoin()
{
	// gracz dolaczajacy wysyla pakiet o polaczeniu sie z "tworca" gry

	if( m_eActualMenu != EMultiplayerSubMenuJoin )
		return;

	SPacketJoin sJoin = { 0 };
	::ConvertWcharToChar( sJoin.cNickname, MAX_MULTIPLAYER_NICKNAME, m_cNickName );
	strcpy_s( sJoin.cHostname, HOSTNAME_SIZE, m_pGameCtrl->GetNetworkManager()->GetHostName() );
	sJoin.iIndexShip = m_iIndexShip;

	m_pGameCtrl->GetNetworkManager()->SendUDP( EPacketTypeJoin, ( const void* )&sJoin, sizeof( SPacketJoin ), m_uiIPToConnect );
}

GLvoid CRMultiplayerMenu::AnalyseRecvPacket( SPacketUDP &sPacket )
{
	switch( sPacket.iPacketType ) {
		case EPacketTypeJoinUser:
		case EPacketTypeChatMsg:
			m_pRLanChat->AddMessage( ( const char* )sPacket.cData );
			break;
		
		case EPacketTypeCreateEcho:	// odbiera doalczajacy
			AddCreatedGame( sPacket );
			break;
		
		case EPacketTypeJoin:	// odbiera tworca
			NotifyAboutJoin( sPacket );
			break;
		
		case EPacketTypeCreatorNotWait: // odbiera dolaczajacy
			if( m_eActualMenu == EMultiplayerSubMenuJoinWaitToAccept )
				m_eActualMenu = EMultiplayerSubMenuJoin;
			break;
		
		case EPacketTypeAcceptJoin:	// odbiera dolaczajacy
			NotifyAboutAcceptJoin(); // dolaczajacy potwierdza zaakceptowanie przez tworce
			break;
		
		case EPacketTypeRejectJoin:	// odbiera dolaczajacy
			NotifyAboutRejectJoin( ( const char* )sPacket.cData );
			break;
		
		case EPacketTypeTransactionOfConnectDone: //odbiera tworca
			RunGameMultiplayer();
			break;

		case EPacketTypeStartMultiGame:	// odbiera tworca
			m_pGameCtrl->GetRGame()->StartMultiplayerGame();
			break;
	}
}

GLvoid CRMultiplayerMenu::AddCreatedGame( SPacketUDP &sPacket )
{
	if( m_eActualMenu == EMultiplayerSubMenuCreatorWaitToJoin )
		return;

	SPacketCreateEcho *sPacketCreateEcho = ( SPacketCreateEcho* )&sPacket.cData;

	SCreatedGame sCreatedGame = { 0 };
	strcpy_s( sCreatedGame.cCreatorNickname, MAX_MULTIPLAYER_NICKNAME, sPacketCreateEcho->cCreatorNickname );
	strcpy_s( sCreatedGame.cCreatorHostname, HOSTNAME_SIZE, sPacketCreateEcho->cCreatorHostname );
	sCreatedGame.bOptionCollision = sPacketCreateEcho->bOptionCollision;
	sCreatedGame.bOptionSameShips = sPacketCreateEcho->bOptionSameShips;
	sCreatedGame.iIndexShip = sPacketCreateEcho->iIndexShip;
	sCreatedGame.uiCreatorIP = sPacket.uiIPSender;
	sCreatedGame.uiLastRef = ( ::timeGetTime() / 1000 );

	// porownaj czy juz istnieje
	GLboolean bAlreadyExist = GL_FALSE;
	GLint i;
	for( i = 0; i < GetCreatedGamesSize(); ++i ) {
		if( strcmp( sCreatedGame.cCreatorNickname, m_aCreatedGames[ i ].cCreatorNickname ) == 0
			&& strcmp( sCreatedGame.cCreatorHostname, m_aCreatedGames[ i ].cCreatorHostname ) == 0
			&& sCreatedGame.bOptionCollision == m_aCreatedGames[ i ].bOptionCollision
			&& sCreatedGame.bOptionSameShips == m_aCreatedGames[ i ].bOptionSameShips
			&& sCreatedGame.iIndexShip == m_aCreatedGames[ i ].iIndexShip
			&& sCreatedGame.uiCreatorIP == m_aCreatedGames[ i ].uiCreatorIP )
		{
			bAlreadyExist = GL_TRUE;
			break;
		}
	}

	if( bAlreadyExist )
		m_aCreatedGames[ i ].uiLastRef = ( ::timeGetTime() / 1000 );
	else
		m_aCreatedGames.push_back( sCreatedGame );
}

GLvoid CRMultiplayerMenu::RemoveCreatedGame( GLint iIndex )
{
	m_aCreatedGames.erase( m_aCreatedGames.begin() + iIndex );
}

GLvoid CRMultiplayerMenu::NotifyAboutJoin( SPacketUDP &sPacket )
{
	// powiadomienie "tworcy" o dolaczeniu dolaczajacego

	if( m_eActualMenu != EMultiplayerSubMenuCreatorWaitToJoin || g_bBlockNewJoin ) {
		// wyslij pakiet, ktory wroci dolaczajacego do menu z wyborem gier
		m_pGameCtrl->GetNetworkManager()->SendUDP( EPacketTypeCreatorNotWait, NULL, 0, sPacket.uiIPSender );
		return;
	}

	g_bBlockNewJoin = GL_TRUE; // zablokuj innych doalaczajacych

	SPacketJoin *sPacketJoin = ( SPacketJoin* )&sPacket.cData;

	// zapisz dane dolaczajacego
	m_uiIPToConnect = sPacket.uiIPSender;
	m_iIndexShipAnother = sPacketJoin->iIndexShip;

	TCHAR tcMsg[ 128 ];
	::wsprintf( tcMsg, LNG_LINE( 148 ),
#ifdef UNICODE
				::ConvertCharToWchar( sPacketJoin->cNickname, MAX_MULTIPLAYER_NICKNAME ),
				::ConvertCharToWchar( sPacketJoin->cHostname, HOSTNAME_SIZE ),
#else
				sPacketJoin->cNickname,
				sPacketJoin->cHostname,
#endif
				m_pGameCtrl->GetNetworkManager()->IPntoaw( sPacket.uiIPSender ) );

	m_pGameCtrl->GetRMessageBox()->DoMessageBox( tcMsg, CRMessageBox::EMsgYesNo, NotifyAboutJoinYES, NotifyAboutJoinNO );
}

void NotifyAboutJoinYES()
{
	// akceptacja polaczenia z dolaczajacym
	g_bBlockNewJoin = GL_FALSE; // odblokuj innych dolaczajacych

	GLuint uiIPToConnect = CGameControl::GetInstance()->GetRMultiplayerMenu()->GetIPToConnect();
	
	CGameControl::GetInstance()->GetNetworkManager()->SendUDP( EPacketTypeAcceptJoin, NULL, 0, uiIPToConnect );
}

void NotifyAboutJoinNO()
{
	// odrzucenia polaczenia z dolaczajacym
	g_bBlockNewJoin = GL_FALSE; // odblokuj innych dolaczajacych
	CGameControl::GetInstance()->GetNetworkManager()->SendUDP(
		EPacketTypeRejectJoin,
		CGameControl::GetInstance()->GetRMultiplayerMenu()->GetNickName(),
		::GetStringLength( CGameControl::GetInstance()->GetRMultiplayerMenu()->GetNickName() ),
		CGameControl::GetInstance()->GetRMultiplayerMenu()->GetIPToConnect() );
}

GLvoid CRMultiplayerMenu::NotifyAboutAcceptJoin()
{
	// powiadomienie "dolaczajacege" ze "tworca gry" zaakceptowal proble polaczenia sie

	if( m_eActualMenu != EMultiplayerSubMenuJoinWaitToAccept )
		return;

	m_pGameCtrl->GetNetworkManager()->SendUDP( EPacketTypeTransactionOfConnectDone,
											   NULL,
											   0,
											   m_uiIPToConnect );

	RunGameMultiplayer();
}

GLvoid CRMultiplayerMenu::NotifyAboutRejectJoin( const char* pNickname )
{
	// powiadomienie "dolaczajacege" ze "tworca gry" odrzucil proble polaczenia sie

	if( m_eActualMenu != EMultiplayerSubMenuJoinWaitToAccept || !pNickname )
		return;

	//wroc do menu z wyborem "stworzonych gier" - mozna by to wrzucic pod DoMessageBox - OK
	m_eActualMenu = EMultiplayerSubMenuJoin;

	TCHAR tcMsg[ 128 ];
	::wsprintf( tcMsg, LNG_LINE( 149 ),
#ifdef UNICODE
				::ConvertCharToWchar( pNickname, MAX_MULTIPLAYER_NICKNAME )
#else
				pNickname
#endif
				);

	m_pGameCtrl->GetRMessageBox()->DoMessageBox( tcMsg, CRMessageBox::EMsgOK, NULL, NULL );
}

GLvoid CRMultiplayerMenu::RunGameMultiplayer()
{
	m_eActualMenu = EMultiplayerSubMenuMain; // aby przestal wysylac echo, czy odbierac echo
	// przejscie do gry
	::SetCursorPos( CWindowData::GetInstance()->SettingFile().iWidth / 2,
					CWindowData::GetInstance()->SettingFile().iHeight / 2 );
	m_pGameCtrl->GetCursor()->SetMyCursor( CCursor::EGame );

	m_pGameCtrl->GetRGame()->SetStartPlayerVariables( m_iIndexShip, GL_TRUE );
	m_pGameCtrl->SetEMainGameMode( ESingleGame );
}

#endif // _USE_MULTIPLYER_