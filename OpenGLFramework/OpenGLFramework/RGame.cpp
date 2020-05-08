
#include "StdAfx.h"
#include "RVisualEnemyHit.h"
#include "RStarsEffect.h"
#include "../Fonts/SDLFont.h"
#include "Cursor.h"
#include "WindowData.h"
#include "RDifficultyLevel.h"
#include "ShipsManager.h"
#include "LevelsManager.h"
#include "../Draw/3DObjManager.h"
#include "RHighscore.h"
#include "RAddStar.h"
#include "RBonus.h"
#include "RHelp.h"
#include "RWeaponSelect.h"
#include "RGameMenu.h"
#include "RAddStar.h"
#include "RBoss.h"
#include "RShockWaveEffect.h"
#include "RRemoteRocket.h"
#include "REnemyShotAnimCtrl.h"
#include "RHomingRockets.h"
#include "REmitterRockets.h"
#include "RMultiplayerMenu.h"
#include "RStats.h"
#include "RFade.h"
#include "RGame.h"

const GLfloat g_fTopPercentIndicator = 25.0f;
const GLfloat g_fBottomPercentIndicator = 95.0f;
const GLfloat g_fLeftPercentShieldIndicator = 1.0f;
const GLfloat g_fRightPercentShieldIndicator = 5.0f;
const GLfloat g_fLeftPercentArmourIndicator = 95.0f;
const GLfloat g_fRightPercentArmourIndicator = 99.0f;

#if _USE_MULTIPLYER_
GLvoid CallbackEnemyShipsReady( GLint iResult, STransferTCPData *sTransTCP );
#endif

CRGame::CRGame()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_pSDLFontPoints			= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 3 ), 32, GL_TRUE );
	m_pSDLFontIndic				= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 26, GL_TRUE );

	m_pSDLFontDifficultLevel1	= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 60, GL_TRUE );
	m_pSDLFontDifficultLevel2	= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 40, GL_TRUE );
	m_pSDLFontUserName			= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 2 ), 40, GL_TRUE );
	m_pSDLFontLevel				= new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 1 ), 14, GL_TRUE );

	m_pPlayer = NULL;
	m_pEnemyShip = NULL;
	m_pRBonus = NULL;
	m_pUnivBillboard = NULL;
	m_pRHelp = NULL;
	m_pVisualEnemyHit = NULL;
	m_pStarsEffect = NULL;
	m_pRWeaponSelect = NULL;
	m_pRGameMenu = NULL;
	m_pRAddStar = NULL;
	m_pRBoss = NULL;
	m_pRShockWaveEffect = NULL;
	m_pRRemoteRocket = NULL;
	m_pRHomingRockets = NULL;
	m_pREmitterRockets = NULL;

	m_bMouseHoldL = GL_FALSE;
	m_bSpaceKeyHold = GL_FALSE;
	m_bDrawPlayerZone = GL_FALSE;
	m_bIsMultiplayer = GL_FALSE;

	//inicjalizujemy tablice kolorow dla celownika
	m_aHudColor.push_back( CVector3( 0.0f, 1.0f, 0.0f ) ); //zielony
	m_aHudColor.push_back( CVector3( 1.0f, 0.0f, 0.0f ) ); //czerwony
	m_aHudColor.push_back( CVector3( 0.2f, 0.2f, 1.0f ) ); //niebieski
	m_aHudColor.push_back( CVector3( 1.0f, 1.0f, 0.0f ) ); //zolty
	m_aHudColor.push_back( CVector3( 1.0f, 1.0f, 1.0f ) ); //bialy

	//tymczasowe
#ifdef GROUND_VARIABLES
	m_fGroundPosY = -20.0f;
	m_bGroundPosYSign = GL_FALSE;
#endif

	m_iTextPosY_05 = static_cast< GLint >( 7.68f * 5.0f );
	m_iTextPosY_10 = static_cast< GLint >( 7.68f * 10.0f );
	m_iTextPosY_15 = static_cast< GLint >( 7.68f * 15.0f );
	m_iTextPosY_50 = static_cast< GLint >( 7.68f * 50.0f );

	m_iTextPosX_22 = static_cast< GLint >( 10.24f * 22.0f );
}

CRGame::~CRGame()
{
	delete m_pPlayer;
	delete m_pEnemyShip;
	delete m_pRBonus;
	delete m_pUnivBillboard;
	delete m_pRHelp;
	delete m_pVisualEnemyHit;
	delete m_pStarsEffect;
	delete m_pSDLFontPoints;
	delete m_pSDLFontIndic;
	delete m_pSDLFontDifficultLevel1;
	delete m_pSDLFontDifficultLevel2;
	delete m_pSDLFontUserName;
	delete m_pSDLFontLevel;
	delete m_pRWeaponSelect;
	delete m_pRGameMenu;
	delete m_pRAddStar;
	delete m_pRBoss;
	delete m_pRShockWaveEffect;
	delete m_pRRemoteRocket;
	delete m_pREnemyShotAnimCtrl;
	delete m_pRHomingRockets;
	delete m_pREmitterRockets;
	delete m_pRStats;
	delete m_pRFade;

	m_aPointsForShootDown.clear();
	m_aHudColor.clear();
}

GLvoid CRGame::Initialization()
{
	m_pREnemyShotAnimCtrl = new CREnemyShotAnimCtrl();
	m_pPlayer = new CRPlayer();
	m_pEnemyShip = new CEnemyShip();
	m_pRBonus = new CRBonus();
	m_pUnivBillboard = new CUniversalBillboard();
	m_pRHelp = new CRHelp();
	m_pVisualEnemyHit = new CRVisualEnemyHit();
	m_pStarsEffect = new CRStarsEffect();
	m_pRWeaponSelect = new CRWeaponSelect();
	m_pRGameMenu = new CRGameMenu();
	m_pRAddStar = new CRAddStar();
	m_pRBoss = new CRBoss();
	m_pRShockWaveEffect = new CRShockWaveEffect();
	m_pRRemoteRocket = new CRRemoteRocket();
	m_pRHomingRockets = new CRHomingRockets();
	m_pREmitterRockets = new CREmitterRockets();
	m_pRStats = new CRStats();
	m_pRFade = new CRFade();

	SetRegions();

	//emiter czasteczek dla impulse laser
	m_iEmitterImpulseLaser = m_pGameCtrl->GetParticles()->CreateEmitter(
			CVector3( 0.0f, 0.0f, 0.0f ),	//z, y, x
			m_pGameCtrl->m_iTexParticle01,	//textura
			CVector3(),
			CVector3(),
			1.5f,		//skala
			1.0f,		//life
			GL_FALSE,	//czasteczki nie moga ozywac po wygasnieciu
			0.1f,		//r
			0.3f,		//g
			0.6f,		//b
			0.0f,		//-200.0f,//0.0f, //speed x
			0.0f,		//speed y
			0.0f,
			200,		//ilosc
			10.0f );	//szybkosc poruszania sie
}

GLvoid CRGame::CalculateNumberOfIndicatorText()
{
	//sprawdz ile liter ma oslona
	m_fShieldSignsNumber = 0.0f;
	for( GLint i = 35; i < MAX_INDICATOR_TEXT + 35; ++i ) {
		if( _tcslen( LNG_LINE( i ) ) == 0 )
			break;
		m_fShieldSignsNumber += 1.0f;
	}
	if( m_fShieldSignsNumber == 0.0f )
		m_fShieldSignsNumber = 8.0f;

	//sprawdz ile liter ma pancerz
	m_fArmourSignsNumber = 0.0f;
	for( GLint i = 43; i < MAX_INDICATOR_TEXT + 43; ++i ) {
		if( _tcslen( LNG_LINE( i ) ) == 0 )
			break;
		m_fArmourSignsNumber += 1.0f;
	}
	if( m_fArmourSignsNumber == 0.0f )
		m_fArmourSignsNumber = 8.0f;
}

GLvoid CRGame::SetRegions()
{
	//shield
	SetRegionX( m_rRegions[ 0 ].left, g_fLeftPercentShieldIndicator );
	SetRegionX( m_rRegions[ 0 ].right, g_fRightPercentShieldIndicator );
	SetRegionY( m_rRegions[ 0 ].top, g_fTopPercentIndicator );
	SetRegionY( m_rRegions[ 0 ].bottom, g_fBottomPercentIndicator );

	//armour
	SetRegionX( m_rRegions[ 1 ].left, g_fLeftPercentArmourIndicator );
	SetRegionX( m_rRegions[ 1 ].right, g_fRightPercentArmourIndicator );
	SetRegionY( m_rRegions[ 1 ].top, g_fTopPercentIndicator );
	SetRegionY( m_rRegions[ 1 ].bottom, g_fBottomPercentIndicator );

	memcpy_s( m_rRegionsIndicators, sizeof( m_rRegionsIndicators ), m_rRegions, sizeof( m_rRegions ) );

	//napis shield_/os³ona_
	CalculateNumberOfIndicatorText();
	GLfloat fTop, fBottom;
	for( GLint i = 0; i < 2; ++i ) {
		for( GLint j = 0; j < MAX_INDICATOR_TEXT; ++j ) {
			if( i == 0 ) { //oslona
				SetRegionX( m_rRegionText[ i ][ j ].left, g_fLeftPercentShieldIndicator, GL_TRUE );
				SetRegionX( m_rRegionText[ i ][ j ].right, g_fRightPercentShieldIndicator, GL_TRUE );

				fTop = g_fTopPercentIndicator + ( j * ( ( g_fBottomPercentIndicator - g_fTopPercentIndicator ) / m_fShieldSignsNumber ) );
				fBottom = g_fTopPercentIndicator + ( ( j + 1 ) * ( ( g_fBottomPercentIndicator - g_fTopPercentIndicator ) / m_fShieldSignsNumber ) );
			}
			else { //pancerz
				SetRegionX( m_rRegionText[ i ][ j ].left, g_fLeftPercentArmourIndicator, GL_TRUE );
				SetRegionX( m_rRegionText[ i ][ j ].right, g_fRightPercentArmourIndicator, GL_TRUE );

				fTop = g_fTopPercentIndicator + ( j * ( ( g_fBottomPercentIndicator - g_fTopPercentIndicator ) / m_fArmourSignsNumber ) );
				fBottom = g_fTopPercentIndicator + ( ( j + 1 ) * ( ( g_fBottomPercentIndicator - g_fTopPercentIndicator ) / m_fArmourSignsNumber ) );
			}

			SetRegionY( m_rRegionText[ i ][ j ].top, fTop, GL_TRUE );
			SetRegionY( m_rRegionText[ i ][ j ].bottom, fBottom, GL_TRUE );
		}
	}

	//ustaw regiony dla zyc
	GLfloat fSizeA = 6.0f; //dlugosc boku
	GLfloat fResolutionRelation = static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iWidth ) / static_cast< GLfloat >( CWindowData::GetInstance()->SettingFile().iHeight );
	GLfloat fSizeB = fSizeA * fResolutionRelation; //wysokosc boku
	GLfloat fPercent = 100.0f;
	for( GLint i = 0; i < 3; ++i ) {
		SetRegionX( m_rRegionsLives[ i ].right, fPercent );
		fPercent -= fSizeA;
		SetRegionX( m_rRegionsLives[ i ].left, fPercent );
		fPercent -= 1.0f;

		SetRegionY( m_rRegionsLives[ i ].top, 0.0f );
		SetRegionY( m_rRegionsLives[ i ].bottom, 0.0f + fSizeB );
	}

	//ustaw region dla ikonki uzbrojenia
	fPercent = 100.0f;
	SetRegionX( m_rRegionSelectedWeapon.right, fPercent - ( fSizeA * 1.25f ) - 2.0f );
	SetRegionX( m_rRegionSelectedWeapon.left, fPercent - ( fSizeA * 2.25f ) - 2.0f );
	SetRegionX( m_rPointAmmo.x, fPercent - ( fSizeA * 1.25f ) - 2.0f, GL_TRUE );
	SetRegionY( m_rRegionSelectedWeapon.top, fSizeB ) ;
	SetRegionY( m_rRegionSelectedWeapon.bottom, fSizeB * 2.0f );
	SetRegionY( m_rPointAmmo.y, ( fSizeB * 2.0f ) - ( fSizeB / 4.0f ), GL_TRUE );

	//ustaw region dla ilosci zdalnych rekiet
	fPercent = 100.0f;
	SetRegionX( m_rRegionsRemoteRockets.right, fPercent - fSizeA );
	SetRegionX( m_rRegionsRemoteRockets.left, fPercent - ( fSizeA * 2.0f ) );
	SetRegionX( m_ptRegionsRemoteRocketsFont.x, fPercent - fSizeA, GL_TRUE );
	SetRegionY( m_rRegionsRemoteRockets.top, fSizeB * 2.0f );
	SetRegionY( m_rRegionsRemoteRockets.bottom, fSizeB * 3.0f );
	SetRegionY( m_ptRegionsRemoteRocketsFont.y, ( fSizeB * 3.0f ) - ( fSizeB / 4.0f ), GL_TRUE );

	//ustawienie regionow dla napisow EnterYourName
	fPercent = 70.0f;
	for( GLint i = 0; i < 3; ++i ) {
		SetRegionY( m_rRegionsTextForEnterYourName[ i ], fPercent, GL_TRUE );
		fPercent -= 15.0f;
	}

	//napis w rogu co do aktualnego poziomu
	SetRegionX( m_ptLevelInfo.x, 93.0f, GL_TRUE );
	SetRegionY( m_ptLevelInfo.y, 100.0f, GL_TRUE );
}

GLvoid CRGame::RestartObjects()
{
	SetRegions();
	GetPlayer()->RestartObjects();
	GetRHelp()->RestartObjects();
	GetRWeaponSelect()->RestartObjects();
	GetRGameMenu()->RestartObjects();
	GetRAddStar()->RestartObjects();
	GetRStats()->RestartObjects();
}

GLvoid CRGame::SetStartPlayerVariables( GLint iIndexShip, GLboolean bMultiplayer /*= GL_FALSE*/ )
{
	PROFILER_BEGIN( _T("CRGame::SetStartPlayerVariables()") );

	//TODO: tutaj ustawienie innych zmiennych
#if _MY_SYSTEM_MOUSE_SPEED_
	if( CWindowData::GetInstance()->GetSystemMouseSpeed() > -1 ) {
		if( ::SystemParametersInfo( SPI_SETMOUSESPEED, 0, ( PVOID* )MOUSE_SPEED_FOR_GAME, SPIF_SENDCHANGE ) == 0 )
			__LOG_LAST_ERROR( _T("SystemParametersInfo with SPI_SETMOUSESPEED param") );
	}
#endif

	m_bIsMultiplayer = bMultiplayer;
	GetRStats()->ClearStats();
	GetRBonus()->ResetAllValue();
	GetPlayer()->Set3DObjectIndex( iIndexShip );
	GetPlayer()->SetLives( 3 );
	GetPlayer()->SetScore( 0 );
	GetPlayer()->SetNumberViewFinder( 5 );
	GetPlayer()->SetPlayerExplode( 0 );
#if _USE_MULTIPLYER_
	if( m_bIsMultiplayer ) {
		if( m_pGameCtrl->GetRMultiplayerMenu()->AmICreator() ) //tworca gry zawsze po lewej stronie
			GetPlayer()->SetPosition( CVector3( -MULTIPLAYER_START_POSX, 0.0f, PLAYER_ZONE_MIN ) );
		else
			GetPlayer()->SetPosition( CVector3( MULTIPLAYER_START_POSX, 0.0f, PLAYER_ZONE_MIN ) );
	}
	else
		GetPlayer()->SetPosition( CVector3( 0.0f, 0.0f, PLAYER_ZONE_MIN ) );
#else
	GetPlayer()->SetPosition( CVector3( 0.0f, 0.0f, PLAYER_ZONE_MIN ) );
#endif
	// ustawienie pozycji sluchacza w srodku strefy gracza, po osi Z:
	COpenALManager::GetInstance()->SetListenerPosition( CVector3( 0.0f, 0.0f, PLAYER_ZONE_MAX + ( ( PLAYER_ZONE_MIN - PLAYER_ZONE_MAX ) / 2 ) ) / DIVISION_EXPLO_POSITION_FOR_SOUND );
	GetPlayer()->SetImmortality( 0 );
	GetPlayer()->SetShakePlayer( GL_FALSE );
	GetPlayer()->SetShakeFade( 1.0f );
#ifdef DRAW_PLAYER_SHIELD_3D
	GetPlayer()->ResetDrawPlayerShield3D();
#endif
	m_bYouCanPressAnyKey = GL_FALSE;
	m_bShowEnterNickname = GL_FALSE;
	m_iLevel = 1;
	m_iLevelToDisplay = m_iLevel;
	m_iDrawLevelInfo = 3;
	m_bPause = GL_FALSE;
#ifdef GROUND_VARIABLES
	m_fGroundMove = 0.0f;
#endif
	m_bPlayerWin = GL_FALSE;
	m_iActualExplodeSound = 0;
	GetRAddStar()->SetDraw( GL_FALSE );
	GetRAddStar()->SetWasCalledDraw( GL_FALSE );
	GetRAddStar()->SetScoreThresholds(); //ustaw progi punktowe dla upgrade-u statku
	m_bDrawCursor = GL_FALSE;
	GetRRemoteRocket()->SetIsDraw( GL_FALSE );
	GetRRemoteRocket()->SetBlockRemoteRocket( GL_FALSE );
	m_iMultiKillCount = 0;
	m_fDrawMultiKill = 0.0f;
	m_iActualGroup = 0;
	m_bDrawHelp = GL_FALSE;
	m_bPressKeyP = GL_FALSE;
	m_bPressKeyEsc = GL_FALSE;
	m_bPressKeyF1 = GL_FALSE;
	m_iTexBkgIndex = rand() % m_pGameCtrl->GetTexBackgroundSize();
	m_eWeaponType = EWeaponLaser;
	m_eCameraMode = ECameraModeNormal;
	m_uiCounterPlayerShotGroup = 0;
	m_uiCounterEnemyShotGroup = 0;
	m_iImpulseLaserCounterTime = 0;
	m_bImpulseLaserShot = GL_FALSE;
	m_fImpulseLaserDrawCounter = 0.0f;
	m_iExplosionDrawCounter = 0;
	GetRGameMenu()->NoDraw();
	GetRShockWaveEffect()->SetForStart();

	GetRBonus()->ResetCounterGeneratedBonuses();
	m_iTotalStartedDrawEnemy = 0;

	::SecureZeroMemory( m_cUserName, sizeof( m_cUserName ) );

	m_aPointsForShootDown.clear();

	GetRHomingRockets()->SetForStart();
	GetREmitterRockets()->SetForStart();

	GLuint uiPoints;
	for( GLint i = 0; i < m_pGameCtrl->GetShipsManager()->GetUniqueShipsSize(); ++i ) {
		uiPoints = m_pGameCtrl->GetShipsManager()->GetPointsForShootDown( i ) * static_cast< GLuint >( m_pGameCtrl->GetRDifficultyLevel()->GetDifficultyLevel() );
		m_aPointsForShootDown.push_back( uiPoints );
	}

	//ustawienie punktow za gwiazdki
	GetPlayer()->SetShieldMax( m_pGameCtrl->GetShipsManager()->GetShieldPoints( GetPlayer()->Get3DObjectIndex() ) );
	GetPlayer()->SetShieldActual( GetPlayer()->GetShieldMax() );
	GetPlayer()->SetArmourMax( m_pGameCtrl->GetShipsManager()->GetArmourPoints( GetPlayer()->Get3DObjectIndex() ) );
	GetPlayer()->SetArmourActual( GetPlayer()->GetArmourMax() );
	GetPlayer()->SetArmamentBase( m_pGameCtrl->GetShipsManager()->GetArmamentPoints( GetPlayer()->Get3DObjectIndex() ) );
	GetPlayer()->SetArmamentActual( GetPlayer()->GetArmamentBase() );
	GetPlayer()->SetManoeuvring( m_pGameCtrl->GetShipsManager()->GetManoeuvringPercent( GetPlayer()->Get3DObjectIndex() ) );

	GetPlayer()->SetStarsShield( m_pGameCtrl->GetShipsManager()->GetStarsShield( GetPlayer()->Get3DObjectIndex() ) );
	GetPlayer()->SetStarsArmour( m_pGameCtrl->GetShipsManager()->GetStarsArmour( GetPlayer()->Get3DObjectIndex() ) );
	GetPlayer()->SetStarsArmament( m_pGameCtrl->GetShipsManager()->GetStarsArmament( GetPlayer()->Get3DObjectIndex() ) );
	GetPlayer()->SetStarsManoeuvring( m_pGameCtrl->GetShipsManager()->GetStarsManoeuvring( GetPlayer()->Get3DObjectIndex() ) );

	//ustawienie zmiennych dla kolizji
	GetPlayer()->SetCollisionA( m_pGameCtrl->GetShipsManager()->GetUniqueShip( GetPlayer()->Get3DObjectIndex() ).fCollisionA );
	GetPlayer()->SetCollisionB( m_pGameCtrl->GetShipsManager()->GetUniqueShip( GetPlayer()->Get3DObjectIndex() ).fCollisionB );

	memcpy_s( m_rRegionsIndicators, sizeof( m_rRegionsIndicators ), m_rRegions, sizeof( m_rRegionsIndicators ) );

	GetREnemyShotAnimCtrl()->SetAllOnFree();

	GetUnivBillboard()->SetForStart();

	CreateEnemyShips();

	GetRFade()->DoDrawHalfFade();

	PROFILER_END();
}

GLvoid CRGame::SetEndPlayerVariables( EMainGameMode eSkipTo )
{
	//TODO: tutaj ustawienie innych zmiennych na zakonczenie / wyjscie z gry

	//powrot szybkosci myszy do systemowej
#if _MY_SYSTEM_MOUSE_SPEED_
	if( CWindowData::GetInstance()->GetSystemMouseSpeed() > -1 ) {
		if( ::SystemParametersInfo( SPI_SETMOUSESPEED, 0, ( PVOID* )CWindowData::GetInstance()->GetSystemMouseSpeed(), SPIF_SENDCHANGE ) == 0 )
			__LOG_LAST_ERROR( _T("SystemParametersInfo with SPI_SETMOUSESPEED param") );
	}
#endif
	//powrot listenera do pozycji 0,0,0
	COpenALManager::GetInstance()->SetListenerPosition( CVector3() );
	CWindowData::GetInstance()->SaveSettingFile(); //zapis, bo mogl sie zmienic kolor celownika
	m_pGameCtrl->SetEMainGameMode( eSkipTo );
	m_pGameCtrl->StartPlayMusicForMenu();
}

GLvoid CRGame::SetShowEnterNickname()
{
	if( GetPlayer()->GetScore() >= g_sHighscoreItem[ 9 ].uiScore ) {
		//sprawdz pozycje jaka zajal gracz
		for( GLint i = 0; i < 10; ++i ) {
			if( GetPlayer()->GetScore() >= g_sHighscoreItem[ i ].uiScore ) {
				m_iPositionOnHighscore = i + 1;
				break;
			}
		}
		//gracz moze podac swoje imie:
		m_bShowEnterNickname = GL_TRUE;
	}
	else {
		//m_bShowEnterNickname pozostaje FALSE
		//wyjscie z gry i przelaczenie sie na plansze z highscore
		m_pGameCtrl->GetCursor()->SetMyCursor( CCursor::EMenu );
		SetEndPlayerVariables( EHighscore );
	}
}

GLboolean CRGame::MouseLButtonDown( GLint iX, GLint iY )
{
	if( GetRGameMenu()->IsDraw() )
		return GetRGameMenu()->MouseLButtonDown( iX, iY );

	if( GetRAddStar()->IsDraw() )
		return GetRAddStar()->MouseLButtonDown( iX, iY );

	if( m_bYouCanPressAnyKey ) {
		SetShowEnterNickname();
		return GL_TRUE;
	}

	m_bMouseHoldL = GL_TRUE;

	return GL_TRUE;
}

GLboolean CRGame::MouseLButtonUp( GLint /*iX*/, GLint /*iY*/ )
{
	m_bMouseHoldL = GL_FALSE;

	return GL_TRUE;
}

GLboolean CRGame::MouseRButtonDown( GLint iX, GLint iY )
{
	if( m_bYouCanPressAnyKey ) {
		SetShowEnterNickname();
		return GL_TRUE;
	}
	
	if( m_bPause ) return GL_TRUE;

	return GetPlayer()->MouseRButtonDown( iX, iY );
}

GLboolean CRGame::MouseWheel( GLint iScrollLines )
{
	if( m_bPause ) return GL_TRUE;

	if( iScrollLines < 0 ) {
		SetNextWeapon();
		GetRWeaponSelect()->DoDraw( m_eWeaponType );
	}
	if( iScrollLines > 0 ) {
		SetPreviousWeapon();
		GetRWeaponSelect()->DoDraw( m_eWeaponType );
	}
	return GL_TRUE;
}

GLboolean CRGame::KeyDown( GLuint uiKey )
{
	if( GetRGameMenu()->IsDraw() )
		return GetRGameMenu()->KeyDown( uiKey );
	if( GetRAddStar()->IsDraw() )
		return GetRAddStar()->KeyDown( uiKey );
	GLboolean bKeyWasConsumed = GL_FALSE;

	if( m_bShowEnterNickname ) {
		switch( uiKey ) {
			case VK_RETURN:
				ConfirmHighscore();
				return GL_TRUE;
			default:
				return m_pGameCtrl->WriteOnKeyboard( uiKey, m_cUserName, MAX_HIGHSCORE_NICKNAME_LEN );
		}
		//return GL_TRUE; //<-nigdy tu nie wejdzie
	}

	if( m_bYouCanPressAnyKey ) {
		SetShowEnterNickname();
		return GL_TRUE;
	}

	if( IsEndGame() ) //<- jezeli mamy koniec gry, ale jeszcze nie "Press eny key", to powstrzymaj dzialanie klawiszy
		return GL_TRUE;

	switch( uiKey ) {
		case VK_F1:
			DoHelp();
			return GL_TRUE;
		case VK_ESCAPE:
		{
			if( m_bDrawHelp ) {
				//jezeli mamy helpa to ESC'em zamykamy helpa
				DoHelp();
				return GL_TRUE;
			}
			m_bPressKeyEsc = GL_TRUE;
			m_pGameCtrl->GetSpeedCtrl()->SetPause( GL_TRUE );
			ShowCursor();
			GetRGameMenu()->DoDraw();
			return GL_TRUE;
		}
		case VK_SPACE:
			if( m_bPause ) return GL_TRUE;
			m_bSpaceKeyHold = GL_TRUE;
			return GL_TRUE;
		case 'C': //kamera
			if( m_eCameraMode == ECameraModeLastElement - 1 ) {
				m_eCameraMode = ECameraModeNormal;
#ifdef DRAW_TUNNEL_FOR_LEVEL5
				if( m_iLevel == 5 || m_iLevel == 6 )
					m_eCameraMode = ECameraModeCabin;
#endif
			}
			else {
				GLint iActual = m_eCameraMode + 1;
				m_eCameraMode = static_cast<ECameraMode>( iActual );
#ifdef DRAW_TUNNEL_FOR_LEVEL5
				if( ( m_iLevel == 5 || m_iLevel == 6 ) && ( m_eCameraMode == ECameraModeNormal ) )
					m_eCameraMode = ECameraModeCabin;
#endif
			}
			return GL_TRUE;
		case 'P':
			if( IsEndGame() ) //jeszcze koniec gry
				return GL_TRUE;

			m_bPressKeyP = !m_bPressKeyP;
			m_bPause = !m_bPause;
			if( !m_bPause && ( m_bPressKeyEsc || m_bPressKeyF1 ) )
				m_bPause = GL_TRUE; //nadal pauzuj
			m_pGameCtrl->GetSpeedCtrl()->SetPause( m_bPause );
			GetPlayer()->SetCursorPosAfterPause();
			return GL_TRUE;
		case 'K': //zmiana koloru celownika
			++CWindowData::GetInstance()->SettingFile().iHudColor;
			if( CWindowData::GetInstance()->SettingFile().iHudColor >= GetHudColorSize() )
				CWindowData::GetInstance()->SettingFile().iHudColor = 0;
			return GL_TRUE;
		case VK_RETURN:
			//nie ma pauzy
			if( m_bPause ) return GL_TRUE;
			
			//sprawdz czy obiekt gracza moze strzelac
			if( !m_pGameCtrl->GetShipsManager()->GetUniqueShip( GetPlayer()->Get3DObjectIndex() ).bCanFire )
				return GL_TRUE;
			
			if( GetPlayer()->GetPlayerExplode() > 0 )
				return GL_TRUE; //gracz eksploduje

			//nie ma napisow ready! i zyjemy to mozemy strzelac
			if( m_iDrawLevelInfo > 0 || IsEndGame() )
				return GL_TRUE;
			
			GetRRemoteRocket()->ServiceEnterKey();

			return GL_TRUE;
		case VK_CONTROL:
			GetPlayer()->DoManoeuvring();
			return GL_TRUE;
		case '1':
			SelectWeapon( EWeaponLaser );
			return GL_TRUE;
		case '2':
			SelectWeapon( EWeaponChaingun );
			return GL_TRUE;
		case '3':
			SelectWeapon( EWeaponImpulseLaser );
			return GL_TRUE;
		case '4':
			SelectWeapon( EWeaponEmitterRocket );
			return GL_TRUE;
		case '5':
			SelectWeapon( EWeaponHomingRocket );
			return GL_TRUE;
	}

	return bKeyWasConsumed;
}

GLvoid CRGame::SelectWeapon( EWeaponType eWeaponType )
{
	if( m_bPause )
		return;

	if( GetRBonus()->IsAvailableWeapon( eWeaponType ) ) {
		m_eWeaponType = eWeaponType;
		GetRWeaponSelect()->DoDraw( m_eWeaponType );
	}
}

GLvoid CRGame::SetNextWeapon()
{
	if( m_eWeaponType == EWeaponLastElement - 1 )
		m_eWeaponType = EWeaponLaser;
	else {
		GLint iActual = m_eWeaponType + 1;
		m_eWeaponType = static_cast<EWeaponType>( iActual );
		if( GetRBonus()->GetAmmoFromActualWeapon( m_eWeaponType ) == 0 )
			SetNextWeapon();
	}
}

GLvoid CRGame::SetPreviousWeapon()
{
	if( m_eWeaponType == EWeaponLaser ) {
		GLint iLast = EWeaponLastElement - 1;
		m_eWeaponType = static_cast<EWeaponType>( iLast );
		if( GetRBonus()->GetAmmoFromActualWeapon( m_eWeaponType ) == 0 )
			SetPreviousWeapon();
	}
	else {
		GLint iActual = m_eWeaponType - 1;
		m_eWeaponType = static_cast<EWeaponType>( iActual );
		if( GetRBonus()->GetAmmoFromActualWeapon( m_eWeaponType ) == 0 && m_eWeaponType != EWeaponLaser )
			SetPreviousWeapon();
	}
}

GLboolean CRGame::KeyUp( GLuint uiKey )
{
	switch( uiKey ) {
		case VK_SPACE:
			m_bSpaceKeyHold = GL_FALSE;
			return GL_TRUE;
	}
	return GL_FALSE;
}

GLvoid CRGame::DoHelp()
{
	m_bPressKeyF1 = !m_bPressKeyF1;
	m_bDrawHelp = !m_bDrawHelp;
	m_bPause = m_bDrawHelp;
	if( !m_bPause && ( m_bPressKeyEsc || m_bPressKeyP ) )
		m_bPause = GL_TRUE; //nadal pauzuj
	m_pGameCtrl->GetSpeedCtrl()->SetPause( m_bPause );
	GetPlayer()->SetCursorPosAfterPause();
}

GLvoid CRGame::SetCamera()
{
	switch( m_eCameraMode ) {
		case ECameraModeCabin:
			gluLookAt( GetPlayer()->GetPosition().x, 8.0f, GetPlayer()->GetPosition().z + 17.0f,
					   GetPlayer()->GetPosition().x, 0.0f, MAX_GAME_DISTANCE,
					   0.0f, 1.0f, 0.0f );
			break;
		case ECameraModeTail:
			gluLookAt( GetPlayer()->GetPosition().x, 30.0f, 300.0f,
					   GetPlayer()->GetPosition().x, 0.0f, MAX_GAME_DISTANCE,
					   0.0f, 1.0f, 0.0f );
			break;
//		case ECameraModeTop:
//			gluLookAt( 0.0f, 700.0f, 650.0f,
//					   0.0f, 0.0f, -300.0f,
//					   0.0f, 1.0f, 0.0f );
//			break;
		default: //ECameraModeNormal
		{
			//rozwiazanie gdzie kamera wychyla sie na boki
			/*GLfloat fPosX = 0.0f;
			if( GetPlayer()->GetPosition().x < -121.0f )
				fPosX = GetPlayer()->GetPosition().x + 121.0f;
			else if( GetPlayer()->GetPosition().x > 121.0f )
				fPosX = GetPlayer()->GetPosition().x - 121.0f;

			gluLookAt( fPosX, 200.0f, 400.0f, fPosX, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
			*/
			
			gluLookAt( /*GetPlayer()->GetPosition().x*/0.0f, 200.0f, 400.0f,
					   /*GetPlayer()->GetPosition().x*/0.0f, 0.0f, 0.0f,
					   0.0f, 1.0f, 0.0f );
			break;
		}
	}
}

GLvoid CRGame::Draw()
{
	PROFILER_BEGIN( _T("CRGame::Draw()") );

	//DrawBackground();
	if( m_iExplosionDrawCounter )
		glEnable( GL_LIGHT3 );

///////////////////// rysujemy ziemie ///////////////////////////////////////
#ifdef SHADOW_TEST
	glLoadIdentity();
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_BLEND );
	m_pGameCtrl->Disable2D();
	SetCamera();
	glEnable( GL_TEXTURE_2D );
	m_pGameCtrl->GetTexLoader()->SetTexture( m_pGameCtrl->m_iTexShadowTest );
	glColor3f( 1.0f, 1.0f, 1.0f );
	if( !m_bGroundPosYSign )
		m_fGroundPosY -= ( 4.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	else
		m_fGroundPosY += ( 4.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	if( m_fGroundPosY < -50.0f ) {
		m_fGroundPosY = -50.0f;
		m_bGroundPosYSign = !m_bGroundPosYSign;
	}
	if( m_fGroundPosY > -20.0f ) {
		m_fGroundPosY = -20.0f;
		m_bGroundPosYSign = !m_bGroundPosYSign;
	}
	
	GLfloat t = 60.0f;
	glBegin( GL_QUADS);
		glNormal3f( 0.0f, 1.0f, 0.0f );
		glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 4.0f, m_fGroundPosY, -MAX_DISTANCE_VIEW );
		glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 4.0f, m_fGroundPosY, MIN_GAME_DISTANCE );
		glTexCoord2f( 10.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, MIN_GAME_DISTANCE );
		glTexCoord2f( 10.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );

		glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );
		glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, MIN_GAME_DISTANCE );
		glTexCoord2f( 2.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY - 40.0f, MIN_GAME_DISTANCE );
		glTexCoord2f( 2.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY - 40.0f, -MAX_DISTANCE_VIEW );

		glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY - 40.0f, -MAX_DISTANCE_VIEW );
		glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY - 40.0f, MIN_GAME_DISTANCE );
		glTexCoord2f( 2.0f, 0.0f + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY - 40.0f, MIN_GAME_DISTANCE );
		glTexCoord2f( 2.0f, t + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY - 40.0f, -MAX_DISTANCE_VIEW );

		glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY - 40.0f, -MAX_DISTANCE_VIEW );
		glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY - 40.0f, MIN_GAME_DISTANCE );
		glTexCoord2f( 4.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, MIN_GAME_DISTANCE );
		glTexCoord2f( 4.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );

		glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );
		glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, MIN_GAME_DISTANCE );
		glTexCoord2f( 10.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X * 4.0f, m_fGroundPosY, MIN_GAME_DISTANCE );
		glTexCoord2f( 10.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X * 4.0f, m_fGroundPosY, -MAX_DISTANCE_VIEW );
	glEnd();
	m_fGroundMove += ( 0.5f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	if( m_fGroundMove > t )
		m_fGroundMove = 0.0f;
#elif defined( DRAW_TUNNEL_FOR_LEVEL5 )
	//rysuj tunel dla poziomu piatego
	if( m_iLevel == 5 || m_iLevel == 6 ) {
		if( GetCameraMode() == ECameraModeNormal )
			m_eCameraMode = ECameraModeTail;

		glLoadIdentity();
		glEnable( GL_DEPTH_TEST );
		glDisable( GL_LIGHTING );
		glDisable( GL_BLEND );
		m_pGameCtrl->Disable2D();
		SetCamera();
		glEnable( GL_TEXTURE_2D );
		m_pGameCtrl->GetTexLoader()->SetTexture( m_pGameCtrl->m_iTexShadowTest );
		glColor3f( 1.0f, 1.0f, 1.0f );
		if( !m_bGroundPosYSign )
			m_fGroundPosY -= ( 4.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		else
			m_fGroundPosY += ( 4.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( m_fGroundPosY < -50.0f ) {
			m_fGroundPosY = -50.0f;
			m_bGroundPosYSign = !m_bGroundPosYSign;
		}
		if( m_fGroundPosY > -20.0f ) {
			m_fGroundPosY = -20.0f;
			m_bGroundPosYSign = !m_bGroundPosYSign;
		}
		
		GLfloat t = 60.0f;
		glBegin( GL_QUADS );
		//podloga
			glNormal3f( 0.0f, 1.0f, 0.0f );
			glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 4.0f, m_fGroundPosY, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 4.0f, m_fGroundPosY, MIN_GAME_DISTANCE );
			glTexCoord2f( 10.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, MIN_GAME_DISTANCE );
			glTexCoord2f( 10.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );

			glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, MIN_GAME_DISTANCE );
			glTexCoord2f( 2.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY - 40.0f, MIN_GAME_DISTANCE );
			glTexCoord2f( 2.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY - 40.0f, -MAX_DISTANCE_VIEW );

			glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY - 40.0f, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY - 40.0f, MIN_GAME_DISTANCE );
			glTexCoord2f( 2.0f, 0.0f + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY - 40.0f, MIN_GAME_DISTANCE );
			glTexCoord2f( 2.0f, t + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY - 40.0f, -MAX_DISTANCE_VIEW );

			glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY - 40.0f, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY - 40.0f, MIN_GAME_DISTANCE );
			glTexCoord2f( 4.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, MIN_GAME_DISTANCE );
			glTexCoord2f( 4.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );

			glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, MIN_GAME_DISTANCE );
			glTexCoord2f( 10.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X * 4.0f, m_fGroundPosY, MIN_GAME_DISTANCE );
			glTexCoord2f( 10.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X * 4.0f, m_fGroundPosY, -MAX_DISTANCE_VIEW );
		glEnd();
		glBegin( GL_QUADS );
		//sufit
			m_fGroundPosY += 100.0f;
			glNormal3f( 0.0f, -1.0f, 0.0f );
			glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 4.0f, m_fGroundPosY, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 10.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 10.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, MIN_GAME_DISTANCE );
			glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 4.0f, m_fGroundPosY, MIN_GAME_DISTANCE );

			glNormal3f( 0.0f, -1.0f, 0.0f );
			glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 2.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY + 40.0f, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 2.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY + 40.0f, MIN_GAME_DISTANCE );
			glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X, m_fGroundPosY, MIN_GAME_DISTANCE );

			glNormal3f( 0.0f, -1.0f, 0.0f );
			glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY + 40.0f, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 2.0f, t + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY + 40.0f, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 2.0f, 0.0f + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY + 40.0f, MIN_GAME_DISTANCE );
			glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_LEFT_X * 0.5f, m_fGroundPosY + 40.0f, MIN_GAME_DISTANCE );

			glNormal3f( 0.0f, -1.0f, 0.0f );
			glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY + 40.0f, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 4.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 4.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, MIN_GAME_DISTANCE );
			glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( 0.0f, m_fGroundPosY + 40.0f, MIN_GAME_DISTANCE );

			glNormal3f( 0.0f, -1.0f, 0.0f );
			glTexCoord2f( 0.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 10.0f, t + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X * 4.0f, m_fGroundPosY, -MAX_DISTANCE_VIEW );
			glTexCoord2f( 10.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X * 4.0f, m_fGroundPosY, MIN_GAME_DISTANCE );
			glTexCoord2f( 0.0f, 0.0f + m_fGroundMove ); glVertex3f( PLAYER_ZONE_RIGHT_X, m_fGroundPosY, MIN_GAME_DISTANCE );
		glEnd();

		m_fGroundMove += ( 0.5f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( m_fGroundMove > t )
			m_fGroundMove = 0.0f;
	}
#endif //SHADOW_TEST || DRAW_TUNNEL_FOR_LEVEL5

/////////////////////////////////////////////////////////////////////////////

	DrawEnemy();
	m_pGameCtrl->DrawSkyBox_List( m_iTexBkgIndex, GL_FALSE );

	////////////////////////////////////
	if( m_bMouseHoldL || m_bSpaceKeyHold ) //trzymam strza³
		AddPlayerShot();
	else { //puscilem strzal
		if( m_eWeaponType == EWeaponImpulseLaser && m_iImpulseLaserCounterTime > 0 )
			AddPlayerShot_ImpulseLaser();
		m_iImpulseLaserCounterTime = 0; //zawsze zeroj
	}
	//dla broni impulse laser uaktualnij czas czestotliwosci strzelania
	if( m_eWeaponType != EWeaponImpulseLaser || ( !m_bMouseHoldL && !m_bSpaceKeyHold )
		|| GetPlayer()->GetPlayerExplode() > 0 //gracz eksploduje
		|| m_iDrawLevelInfo > 0 || IsEndGame() ) //sa napisy Ready! lub jest koniec gry
	{
		m_pGameCtrl->GetSpeedCtrl()->UpdateAnimationTime( 16 );
	}
	////////////////////////////////////
	GetStarsEffect()->DrawStars();
	GetRFade()->Draw();
	GetRRemoteRocket()->DrawFlyRemoteRocket();
	GetRHomingRockets()->DrawHomingRocket();
	GetREmitterRockets()->DrawEmitterRocket();
	GetRShockWaveEffect()->DrawShockWave();
	
	glLoadIdentity();

	if( IsEndGame() ) { //koniec gry
		if( m_bPlayerWin ) //jezeli gracz wygral to rysuj statek
			GetPlayer()->DrawPlayer();

		GetRStats()->DrawStats();

		if( !m_bShowEnterNickname ) {
			//napisz game over
			if( !m_bPlayerWin )
				m_pSDLFontDifficultLevel1->DrawText( 0, m_rRegionsTextForEnterYourName[ 2 ], RGB( 255, 0, 0 ), LNG_LINE( 53 ), GL_TRUE );
			else //napisz ze gracz wygral
				m_pSDLFontDifficultLevel1->DrawText( 0, m_rRegionsTextForEnterYourName[ 2 ], RGB( 0, 255, 0 ), LNG_LINE( 57 ), GL_TRUE );

			if( !m_bYouCanPressAnyKey ) {
				if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 2000, 5 ) ) {
					m_bYouCanPressAnyKey = GL_TRUE;
					if( CWindowData::GetInstance()->SettingFile().bSound ) {
						StopPlayMusicAndRenewAfterDelay();
						if( !m_bPlayerWin )
							COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundGameOver );
						else
							COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundCongratulation );
					}
				}
			}

			if( m_bYouCanPressAnyKey ) {
				//wyswietl napis press any key
				if( m_bPlayerWin ) //na zielono
					m_pSDLFontDifficultLevel2->DrawText( 0, m_rRegionsTextForEnterYourName[ 1 ], RGB( 0, 255, 0 ), LNG_LINE( 54 ), GL_TRUE );
				else //na czerowono
					m_pSDLFontDifficultLevel2->DrawText( 0, m_rRegionsTextForEnterYourName[ 1 ], RGB( 255, 0, 0 ), LNG_LINE( 54 ), GL_TRUE );
			}
		}
		else {
			//wpisz pseudonim
			m_pSDLFontDifficultLevel2->DrawTextFormat( 0, m_rRegionsTextForEnterYourName[ 2 ], GL_TRUE, RGB( 32, 255, 32 ), LNG_LINE( 59 ), m_iPositionOnHighscore );
			m_pSDLFontDifficultLevel2->DrawText( 0, m_rRegionsTextForEnterYourName[ 1 ], RGB( 32, 255, 32 ), LNG_LINE( 55 ), GL_TRUE );

			//znak zachêty
			if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 500, 8 ) )
				m_bDrawCursor = !m_bDrawCursor;

			if( m_bDrawCursor )
				m_pSDLFontUserName->DrawTextFormat( 0, m_rRegionsTextForEnterYourName[ 0 ], GL_TRUE, RGB( 32, 255, 32 ), _T("%s_"), m_cUserName );
			else m_pSDLFontUserName->DrawTextFormat( 0, m_rRegionsTextForEnterYourName[ 0 ], GL_TRUE, RGB( 32, 255, 32 ), _T("%s "), m_cUserName );
		}
	}
	else {
		m_pGameCtrl->GetSpeedCtrl()->UpdateAnimationTime( 5 );
		GetPlayer()->DrawPlayer();
	}

	GetVisualEnemyHit()->Draw();
	GetRBonus()->CalculateSlowEnemiesSeconds();
	//sprawdzenie czy przerwac rysowanie impulse laser
	if( m_bImpulseLaserShot ) {
		m_fImpulseLaserDrawCounter += ( 10.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( m_fImpulseLaserDrawCounter > 1.0f )
			m_bImpulseLaserShot = GL_FALSE;
	}
	GetUnivBillboard()->DrawUniversalBillboard();

	//////////////////////////////////////////

	//wypisz info o aktualnie granym poziomie
	m_pSDLFontLevel->DrawTextFormat( m_ptLevelInfo.x, m_ptLevelInfo.y, GL_FALSE, RGB( 255, 255, 255 ), LNG_LINE( 51 ), m_iLevelToDisplay );

	//wypisz punkty
	m_pSDLFontPoints->DrawTextFormat( 5, m_iTextPosY_05, GL_FALSE, RGB( 255, 0, 0 ), _T("%010d"), GetPlayer()->GetScore() );

	if( m_bPause ) //wypisz "Pause"
		m_pSDLFontDifficultLevel1->DrawText( 0, m_iTextPosY_50, RGB( 255, 0, 0 ), LNG_LINE( 56 ), GL_TRUE );

	///////////////////////////////////////////////////////////////////////////
	//rysujemy 2D
	////////////////////////////////////////////////////////////////////////////
	glLoadIdentity();
	m_pGameCtrl->Enable2D();
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	//rysuje paski shield i armour
	GLint i;
	for( i = 0; i < 2; ++i ) {
		glBegin( GL_TRIANGLE_STRIP );
			glNormal3f( 0.0f, 0.0f, +1.0f );
			if( i == 0 )//shield
				glColor4ub( 0, 32, 32, 255 );
			else//armour
				glColor4ub( 22, 16, 16, 255 );
			glVertex2i( m_rRegionsIndicators[ i ].right, m_rRegionsIndicators[ i ].bottom );
			if( i == 0 )//shield
				glColor4ub( 0, 255, 255, 128 );
			else//armour
				glColor4ub( 175, 128, 128, 128 );
			glVertex2i( m_rRegionsIndicators[ i ].right, m_rRegionsIndicators[ i ].top );
			if( i == 0 )//shield
				glColor4ub( 0, 32, 32, 255 );
			else//armour
				glColor4ub( 22, 16, 16, 255 );
			glVertex2i( m_rRegionsIndicators[ i ].left, m_rRegionsIndicators[ i ].bottom );
			if( i == 0 )//shield
				glColor4ub( 0, 255, 255, 128 );
			else//armour
				glColor4ub( 175, 128, 128, 128 );
			glVertex2i( m_rRegionsIndicators[ i ].left, m_rRegionsIndicators[ i ].top );
		glEnd();

		glColor4ub( 255, 255, 255, 255 );
		glBegin( GL_LINE_LOOP );
			glVertex2i( m_rRegions[ i ].left, m_rRegions[ i ].top );
			glVertex2i( m_rRegions[ i ].left, m_rRegions[ i ].bottom );
			glVertex2i( m_rRegions[ i ].right, m_rRegions[ i ].bottom );
			glVertex2i( m_rRegions[ i ].right, m_rRegions[ i ].top );
		glEnd();
	}

	//rysuj zycia
	GLint iSize = GetPlayer()->GetLives();
	if( iSize > 3 ) iSize = 3;
	glColor4ub( 255, 255, 255, 255 );
	m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->GetShipsManager()->GetUniqueShip( GetPlayer()->Get3DObjectIndex() ).iTexLife );
	for( GLint i = 0; i < iSize; ++i ) {
		glBegin( GL_TRIANGLE_STRIP );
			glTexCoord2f( 1.0f, 0.0f ); glVertex2i( m_rRegionsLives[ i ].right, m_rRegionsLives[ i ].bottom );
			glTexCoord2f( 1.0f, 1.0f ); glVertex2i( m_rRegionsLives[ i ].right, m_rRegionsLives[ i ].top );
			glTexCoord2f( 0.0f, 0.0f ); glVertex2i( m_rRegionsLives[ i ].left, m_rRegionsLives[ i ].bottom );
			glTexCoord2f( 0.0f, 1.0f ); glVertex2i( m_rRegionsLives[ i ].left, m_rRegionsLives[ i ].top );
		glEnd();
	}
	//rysuj remote rockets x 0
	if( GetRBonus()->GetRemoteRocketsNumber() > 0 ) {
		glColor4ub( 255, 255, 255, 255 );
		m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexRemoteRocketsCounter );
		glBegin( GL_TRIANGLE_STRIP );
			glTexCoord2f( 1.0f, 0.0f ); glVertex2i( m_rRegionsRemoteRockets.right, m_rRegionsRemoteRockets.bottom );
			glTexCoord2f( 1.0f, 1.0f ); glVertex2i( m_rRegionsRemoteRockets.right, m_rRegionsRemoteRockets.top );
			glTexCoord2f( 0.0f, 0.0f ); glVertex2i( m_rRegionsRemoteRockets.left, m_rRegionsRemoteRockets.bottom );
			glTexCoord2f( 0.0f, 1.0f ); glVertex2i( m_rRegionsRemoteRockets.left, m_rRegionsRemoteRockets.top );
		glEnd();
		m_pSDLFontIndic->DrawTextFormat( m_ptRegionsRemoteRocketsFont.x, m_ptRegionsRemoteRocketsFont.y, GL_FALSE, RGB( 128, 255, 128 ), _T("x%d"), GetRBonus()->GetRemoteRocketsNumber() ); 
	}

	GetRWeaponSelect()->Draw();

	//rysuj ikonke aktualnie wybranej broni
	CGameControl::GetInstance()->Enable2D();
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glColor4ub( 255, 255, 255, 255 );
	m_pGameCtrl->GetTexLoader()->SetTexture( m_pGameCtrl->m_aTexWeaponSelect[ m_eWeaponType ] );
	glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2i( m_rRegionSelectedWeapon.right, m_rRegionSelectedWeapon.bottom );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2i( m_rRegionSelectedWeapon.right, m_rRegionSelectedWeapon.top );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2i( m_rRegionSelectedWeapon.left, m_rRegionSelectedWeapon.bottom );
		glTexCoord2f( 0.0f, 1.0f ); glVertex2i( m_rRegionSelectedWeapon.left, m_rRegionSelectedWeapon.top );
	glEnd();
	glDisable( GL_BLEND );

	//rysuj info o levelu, Level 1, Ready
	if( ( m_bIsMultiplayer && !m_bBlockMultiplayerGame ) || !m_bIsMultiplayer )
		DrawTextReady();

	//rysuj napis Shield i Armour
	for( GLint i = 0; i < MAX_INDICATOR_TEXT; ++i ) {
		m_pSDLFontIndic->DrawText( m_rRegionText[ 0 ][ i ], RGB(   0, 255, 255 ), LNG_LINE( 35 + i ) );
		m_pSDLFontIndic->DrawText( m_rRegionText[ 1 ][ i ], RGB( 175, 128, 128 ), LNG_LINE( 43 + i ) );
	}

	//napisz, ze gracz niezniszczalny
	if( GetPlayer()->IsImmortality() && GetPlayer()->GetLives() > -1 )
		m_pSDLFontIndic->DrawTextFormat( 5, m_iTextPosY_10, GL_FALSE, RGB( 100, 100, 255 ), LNG_LINE( 61 ), GetPlayer()->GetImmortality() / 10 );
	//napisz, ze wrog spowolniony
	if( GetRBonus()->GetSlowEnemiesSeconds() > 0 )
		m_pSDLFontIndic->DrawTextFormat( 5, m_iTextPosY_15, GL_FALSE, RGB( 100, 100, 255 ), LNG_LINE( 62 ), GetRBonus()->GetSlowEnemiesSeconds() );

	//napisz, "Ammunition: %d"
	if( m_eWeaponType != EWeaponLaser && GetRBonus()->GetAmmoFromActualWeapon( m_eWeaponType ) > 0 )
		m_pSDLFontIndic->DrawTextFormat( m_rPointAmmo.x, m_rPointAmmo.y, GL_FALSE, RGB( 128, 255, 128 ), _T(" x%d"), GetRBonus()->GetAmmoFromActualWeapon( m_eWeaponType ) );
	else if( m_eWeaponType != EWeaponLaser )
		m_pSDLFontIndic->DrawText( m_rPointAmmo.x, m_rPointAmmo.y, RGB( 255, 0, 0 ), _T(" x0") );

	CalculateMultiKill();

	glDisable( GL_BLEND );
	m_pGameCtrl->Disable2D();

	if( GetRAddStar()->IsDraw() )
		GetRAddStar()->Draw();

	GetRRemoteRocket()->BlockEnterKey();

	if( m_bDrawHelp )
		GetRHelp()->Draw();

	if( GetRGameMenu()->IsDraw() )
		GetRGameMenu()->Draw();

	glDisable( GL_LIGHT3 );

	PROFILER_END();
}

GLvoid CRGame::DrawTextReady()
{
	if( m_iDrawLevelInfo > 0 && GetPlayer()->GetLives() > -1 ) {
		if( m_iDrawLevelInfo == 2 ) {
			m_pSDLFontDifficultLevel1->DrawTextFormat( 0, m_rRegionsTextForEnterYourName[ 2 ], GL_TRUE, RGB( 255, 0, 0 ), LNG_LINE( 51 ), m_iLevel );
			GLint iIndex = m_pGameCtrl->GetRDifficultyLevel()->GetDifficultyLevel() - 1;
			m_pSDLFontDifficultLevel2->DrawText( 0, m_rRegionsTextForEnterYourName[ 1 ], RGB( 255, 0, 0 ), LNG_LINE( iIndex + 13 ), GL_TRUE );
		}
		else if( m_iDrawLevelInfo == 1 ) //"Ready!"
			m_pSDLFontDifficultLevel1->DrawText( 0, m_rRegionsTextForEnterYourName[ 2 ], RGB( 255, 0, 0 ), LNG_LINE( 52 ), GL_TRUE );

		if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 3000, 1 ) ) {
			//sprawz czy gracz moze dodac gwiazdke
			//nie dodaje gwiazki gdy multiplayer!
			if( !m_bIsMultiplayer && GetRAddStar()->CanAddStar( GetPlayer()->GetScore() ) && !GetPlayer()->IsCompleteStars() )
				GetRAddStar()->DoDrawAddStar();

			if( m_iDrawLevelInfo > 0 )
				--m_iDrawLevelInfo;
		}
	}
}

GLvoid CRGame::CalculateMultiKill()
{
	//dla multikill
	if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 500, 12 ) ) {
		if( m_iMultiKillCount >= 10 ) {
			//mamy multi kill
			m_iMultiKillDisplay = m_iMultiKillCount;
			m_fDrawMultiKill = 200.0f;
			GetPlayer()->IncreaseScore( m_iMultiKillCount * 10 );
			GetRStats()->SetTheBestMultiKill( m_iMultiKillCount );
		}
		m_iMultiKillCount = 0;
	}
	if( m_fDrawMultiKill > 0.0f )
		m_pSDLFontIndic->DrawTextFormat( m_iTextPosX_22, m_iTextPosY_05, GL_FALSE, RGB( 255, 255, 0 ), LNG_LINE( 63 ), m_iMultiKillDisplay );
	m_fDrawMultiKill -= ( 100.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	if( m_fDrawMultiKill < 0.0f )
		m_fDrawMultiKill = 0.0f;
}

GLvoid CRGame::DrawEnemy()
{
	if( m_iDrawLevelInfo > 0 )
		return; //jeszcze napisy Ready!

	PROFILER_BEGIN( _T("CRGame::DrawEnemy()") );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
	glEnable( GL_LIGHT2 );
	glDisable( GL_BLEND );
	m_pGameCtrl->Disable2D();

	m_iActualGroup = GetEnemyShip()->CheckActualGroup();
	GLint iSize = GetEnemyShip()->GetEnemyShipSize();
	GLint iFormation = 0;
#ifdef SHADOW_TEST_ENEMY
	if( CWindowData::GetInstance()->SettingFile().bShadows )
		m_pGameCtrl->GetStencilShadow()->Begin_ObjTransfInfo();
#endif
	for( GLint i = 0; i < iSize; ++i ) {
		if( m_iActualGroup < GetEnemyShip()->GetEnemyShip( i ).iGroup )
			continue;
		iFormation = GetEnemyShip()->GetEnemyShip( i ).iFormation;
		if( m_iNumberActualDrawEnemy + iFormation - 1 < m_pGameCtrl->GetLevelsManager()->GetLevel( m_iIndexLevelManager ).iMaxShipsOnScreen
			&& !GetEnemyShip()->GetEnemyShip( i ).bStart )
		{
			//zawsze dodaj conajmniej 1 statek
			GetEnemyShip()->GetEnemyShip( i ).bStart = GL_TRUE;
			++m_iTotalStartedDrawEnemy;
			++m_iNumberActualDrawEnemy;
			
			for( GLint j = 1; j < iFormation; ++j ) {
				//dodaj kolejne statki, drugi i/lub trzeci
				if( i + j < iSize ) {
					GetEnemyShip()->GetEnemyShip( i + j ).bStart = GL_TRUE;
					++m_iNumberActualDrawEnemy;
				}
			}
		}

		if( !GetEnemyShip()->GetEnemyShip( i ).bAlive || !GetEnemyShip()->GetEnemyShip( i ).bStart )
			continue;

		GLfloat fSpeed = GetEnemyShip()->GetEnemyShip( i ).fSpeed;
		if( !GetEnemyShip()->GetEnemyShip( i ).bWreck )
			fSpeed /= GetRBonus()->GetSlowEnemiesDivision();

		CalculateRotForInertiaObject( GetEnemyShip()->GetEnemyShip( i ) );
		CalculateForEnemyManoeuvring( GetEnemyShip()->GetEnemyShip( i ) );

		if( CWindowData::GetInstance()->SettingFile().bShadows ) {
			// transformacje odwrotne
			glLoadIdentity();
			if( GetEnemyShip()->GetEnemyShip( i ).bIsManoeuvring )
				glRotatef( -GetEnemyShip()->GetEnemyShip( i ).fManoeuvringRot, 0.0f, 0.0f, 1.0f );

			//jezeli leci po cosinusoidze lub po skosie to przechylaj na skrzydlo
			if( GetEnemyShip()->GetEnemyShip( i ).bCosFly || GetEnemyShip()->GetEnemyShip( i ).bSlantFly )
				glRotatef( -GetEnemyShip()->GetEnemyShip( i ).fTiltAngle, 0.0f, 0.0f, 1.0f );

			//jeznie nie moze strzelac ani robic unikow to musi to byc asteroida ( lub inny obiekt nie inteligentny )
			//wiec go obracamy, do obrotu wykorzystujemy fManoeuvringRot
			if( !GetEnemyShip()->GetEnemyShip( i ).bCanManoeuvring	&& !GetEnemyShip()->GetEnemyShip( i ).bCanFire )
				glRotatef( -GetEnemyShip()->GetEnemyShip( i ).fManoeuvringRot,
							GetEnemyShip()->GetEnemyShip( i ).fRotAsteroid.x,
							GetEnemyShip()->GetEnemyShip( i ).fRotAsteroid.y,
							GetEnemyShip()->GetEnemyShip( i ).fRotAsteroid.z );

			glTranslatef( -GetEnemyShip()->GetEnemyShip( i ).cActualPos.x,
						  -GetEnemyShip()->GetEnemyShip( i ).cActualPos.y,
						  -GetEnemyShip()->GetEnemyShip( i ).cActualPos.z );
			SetCamera();

			//obliczamy swiatlo dla cienia
			m_cLightPosShadow = m_pGameCtrl->GetStencilShadow()->GetLightPos( m_pGameCtrl->GetLighting()->GetPosition( GL_LIGHT2 ) );
#ifdef SHADOW_TEST_ENEMY
			m_pGameCtrl->GetStencilShadow()->Set_ObjTransfInfo( GetEnemyShip()->GetEnemyShip( i ).i3DObjIndex, m_cLightPosShadow );
#endif
		}

		glLoadIdentity();
		SetCamera();
		glTranslatef( GetEnemyShip()->GetEnemyShip( i ).cActualPos.x,
					  GetEnemyShip()->GetEnemyShip( i ).cActualPos.y,
					  GetEnemyShip()->GetEnemyShip( i ).cActualPos.z );
#ifdef SHADOW_TEST_ENEMY
		if( CWindowData::GetInstance()->SettingFile().bShadows )
			m_pGameCtrl->GetStencilShadow()->Translate_ObjTransfInfo( GetEnemyShip()->GetEnemyShip( i ).cActualPos );
#endif

		//jeznie nie moze strzelac ani robic unikow to musi to byc asteroida ( lub inny obiekt nie inteligentny )
		//wiec go obracamy, do obrotu wykorzystujemy fManoeuvringRot
		if( !GetEnemyShip()->GetEnemyShip( i ).bCanManoeuvring && !GetEnemyShip()->GetEnemyShip( i ).bCanFire ) {
			glRotatef( GetEnemyShip()->GetEnemyShip( i ).fManoeuvringRot,
					   GetEnemyShip()->GetEnemyShip( i ).fRotAsteroid.x,
					   GetEnemyShip()->GetEnemyShip( i ).fRotAsteroid.y,
					   GetEnemyShip()->GetEnemyShip( i ).fRotAsteroid.z );
#ifdef SHADOW_TEST_ENEMY
			if( CWindowData::GetInstance()->SettingFile().bShadows )
				m_pGameCtrl->GetStencilShadow()->Rotate_ObjTransfInfo( GetEnemyShip()->GetEnemyShip( i ).fManoeuvringRot, GetEnemyShip()->GetEnemyShip( i ).fRotAsteroid );
#endif
		}

		//jezeli leci po cosinusoidze lub po skosie to przechylaj na skrzydlo
		if( GetEnemyShip()->GetEnemyShip( i ).bCosFly || GetEnemyShip()->GetEnemyShip( i ).bSlantFly ) {
			glRotatef( GetEnemyShip()->GetEnemyShip( i ).fTiltAngle, 0.0f, 0.0f, 1.0f );
#ifdef SHADOW_TEST_ENEMY
			if( CWindowData::GetInstance()->SettingFile().bShadows )
				m_pGameCtrl->GetStencilShadow()->Rotate_ObjTransfInfo( GetEnemyShip()->GetEnemyShip( i ).fTiltAngle, CVector3( 0.0f, 0.0f, 1.0f ) );
#endif
		}

		if( GetEnemyShip()->GetEnemyShip( i ).bIsManoeuvring ) {
			glRotatef( GetEnemyShip()->GetEnemyShip( i ).fManoeuvringRot, 0.0f, 0.0f, 1.0f );
#ifdef SHADOW_TEST_ENEMY
			if( CWindowData::GetInstance()->SettingFile().bShadows )
				m_pGameCtrl->GetStencilShadow()->Rotate_ObjTransfInfo( GetEnemyShip()->GetEnemyShip( i ).fManoeuvringRot, CVector3( 0.0f, 0.0f, 1.0f ) );
#endif
		}

		GetEnemyShip()->GetEnemyShip( i ).cOldPos = GetEnemyShip()->GetEnemyShip( i ).cActualPos;

		// zamiana pozycji statku
		if( GetEnemyShip()->GetEnemyShip( i ).bBoss )
			GetRBoss()->MoveEnemyBoss( GetEnemyShip()->GetEnemyShip( i ), fSpeed );
		else
			MoveEnemy( GetEnemyShip()->GetEnemyShip( i ), fSpeed );

		CalculateTiltOnWingForEnemy( GetEnemyShip()->GetEnemyShip( i ) );

		//czy statek przelecial przez plansze
		if( GetEnemyShip()->GetEnemyShip( i ).cActualPos.z > MIN_GAME_DISTANCE ) {
			GetEnemyShip()->GetEnemyShip( i ).bAlive = GL_FALSE;
			--m_iNumberActualDrawEnemy;

			GetREnemyShotAnimCtrl()->FreeAnimCtrlForEnemyShot( GetEnemyShip()->GetEnemyShip( i ) );
			
			if( !GetEnemyShip()->GetEnemyShip( i ).bWreck )
				GetRStats()->IncreaseMissedObjects();
		}

		//przyciemnienie dla wraku
		if( GetEnemyShip()->GetEnemyShip( i ).bWreck )
			m_pGameCtrl->GetLighting()->SetMatDiffuse( 0.01f, 0.01f, 0.01f );

		m_pGameCtrl->Get3DObjManager()->Draw3DObject_Lists( GetEnemyShip()->GetEnemyShip( i ).i3DObjIndex, GL_FALSE, GL_LIGHT1 );

		//powrot koloru
		if( GetEnemyShip()->GetEnemyShip( i ).bWreck )
			m_pGameCtrl->GetLighting()->SetMatDiffuse();

		if( CWindowData::GetInstance()->SettingFile().bShadows ) {
#ifdef SHADOW_TEST_ENEMY
			m_pGameCtrl->GetStencilShadow()->End_ObjTransfInfo();
#else
			m_pGameCtrl->GetStencilShadow()->DrawShadowForObj( GetEnemyShip()->GetEnemyShip( i ).i3DObjIndex, m_cLightPosShadow );
#endif
		}

		AddEnemyShot( GetEnemyShip()->GetEnemyShip( i ) );

		CheckCollisionPlayerEnemy( GetEnemyShip()->GetEnemyShip( i ), i );
	}
#ifdef SHADOW_TEST_ENEMY
	if( CWindowData::GetInstance()->SettingFile().bShadows )
		m_pGameCtrl->GetStencilShadow()->DrawShadowForObjects();
#endif

	glDisable( GL_LIGHT2 );
	glEnable( GL_LIGHT0 );

	CheckEndLevel();
	
	PROFILER_END();
}

GLvoid CRGame::MoveEnemy( SEnemyShip &in_sEnemy, GLfloat fSpeed )
{
	//zmiana pozycji X statku gdy po cosinusoidze
	if( in_sEnemy.bCosFly ) {
		if( in_sEnemy.fCosActualAngle > 360.0f )
			in_sEnemy.fCosActualAngle -= 360.0f;
		else
			in_sEnemy.fCosActualAngle += ( in_sEnemy.fCosIncreaseValue * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );

		if( in_sEnemy.bCosSign )
			in_sEnemy.cActualPos.x += static_cast< GLfloat >( cos( DEG2RAD( in_sEnemy.fCosActualAngle ) ) ) * ( fSpeed / 2.0f ) * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier();
		else
			in_sEnemy.cActualPos.x -= static_cast< GLfloat >( cos( DEG2RAD( in_sEnemy.fCosActualAngle ) ) ) * ( fSpeed / 2.0f ) * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier();
	}
	//zmiana pozycji X statku gdy leci po skosie,
	// * 1.2f aby przyspieszyc lot po skosie bo leci wolniej niz normalnie
	if( in_sEnemy.bSlantFly ) {
		if( in_sEnemy.bSlantSign )
			in_sEnemy.cActualPos.x += ( fSpeed * 1.2f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() ) / 3.5f;
		else
			in_sEnemy.cActualPos.x -= ( fSpeed * 1.2f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() ) / 3.5f;

		//zmiana pozycji Z statku ( po lini prostej )
		//gdy leci po skosie zwiekszamy po Z
		in_sEnemy.cActualPos.z += ( fSpeed * 1.2f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	}
	else //gdy nie leci po skosie to tylko Z
		in_sEnemy.cActualPos.z += ( fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
}

GLvoid CRGame::CalculateTiltOnWingForEnemy( SEnemyShip &in_sEnemy )
{
	//obliczenie przechylu statku na skrzydlo
	if( in_sEnemy.cOldPos.x - in_sEnemy.cActualPos.x > 0 ) {
		in_sEnemy.fTiltAngle += ( 10.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( in_sEnemy.fTiltAngle > 15.0f )
			in_sEnemy.fTiltAngle = 15.0f;
	}
	else {
		in_sEnemy.fTiltAngle -= ( 10.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( in_sEnemy.fTiltAngle < -15.0f )
			in_sEnemy.fTiltAngle = -15.0f;
	}
}

GLvoid CRGame::CalculateRotForInertiaObject( SEnemyShip &in_sEnemy )
{
	//jeznie nie moze strzelac ani robic unikow to musi to byc asteroida ( lub inny obiekt nie inteligentny )
	//wiec go obracamy, do obrotu wykorzystujemy fManoeuvringRot
	if( !in_sEnemy.bCanManoeuvring && !in_sEnemy.bCanFire ) {
		in_sEnemy.fManoeuvringRot += ( 400.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( in_sEnemy.fManoeuvringRot > 360.0f )
			in_sEnemy.fManoeuvringRot -= 360.0f;
	}
}

GLvoid CRGame::CalculateForEnemyManoeuvring( SEnemyShip &in_sEnemy )
{
	if( in_sEnemy.bIsManoeuvring ) {
		//kreci beczke
		if( in_sEnemy.bManoeuvringSign )
			in_sEnemy.fManoeuvringRot += ( 400.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		else
			in_sEnemy.fManoeuvringRot -= ( 400.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( in_sEnemy.fManoeuvringRot > 360.0f || in_sEnemy.fManoeuvringRot < -360.0f )
			in_sEnemy.fManoeuvringRot = 0.0f;
	}
	if( in_sEnemy.fManoeuvringRot == 0.0f && in_sEnemy.bIsManoeuvring )
		in_sEnemy.bIsManoeuvring = GL_FALSE;
}

GLvoid CRGame::CheckEndLevel()
{
	//sprawdzamy czy konic poziomu
	//koniec jest wowczas gdy wszytskie wrogie statki wystartowaly i nie ma ani
	//jednego zywego oraz gdy wszystkie wrogie pociski wylecialy poza plansze

	if( GetEnemyShip()->IsAliveEnemy() )
		return;

	//sprawdzamy czy sa znajdzki lub strzaly wroga do zebrania
	if( GetUnivBillboard()->IsBonusesOrEnemyShots() )
		return;

	if( CanDoFade() ) {
		if( !GetRFade()->IsDraw() )
			GetRFade()->DoDrawFullFade(); // ale nie jak gracz wygral

		if( !GetRFade()->CanChangeLevel() )
			return;
	}

	//mamy koniec poziomu
	++m_iLevel; //zwieksz poziom
	if( m_iLevel > m_pGameCtrl->GetLevelsManager()->GetLevelsSize() ) {
		if( GetPlayer()->GetLives() > -1 && !m_bPlayerWin ) {
			//koniec gry - gracz wygral!
			m_bPlayerWin = GL_TRUE;

			//TODO tutaj udostepniaj statki
			//udostepnij statki i zapisz je
			switch( m_pGameCtrl->GetRDifficultyLevel()->GetDifficultyLevel() ) {
				case ELevelEasy:
					//udostepnij wazke
					m_pGameCtrl->GetShipsManager()->SetAvailableInBrowse( 3, GL_TRUE );
					break;
				case ELevelNormal:
					//udostepnij wazke
					m_pGameCtrl->GetShipsManager()->SetAvailableInBrowse( 3, GL_TRUE );
					//udostepnij unnamed
					m_pGameCtrl->GetShipsManager()->SetAvailableInBrowse( 5, GL_TRUE );
					break;
				case ELevelHard:
					//udostepnij wazke
					m_pGameCtrl->GetShipsManager()->SetAvailableInBrowse( 3, GL_TRUE );
					//udostepnij unnamed
					m_pGameCtrl->GetShipsManager()->SetAvailableInBrowse( 5, GL_TRUE );
					//udostepnij razora 
					m_pGameCtrl->GetShipsManager()->SetAvailableInBrowse( 6, GL_TRUE );
					break;
			}
			//zapisac
			m_pGameCtrl->GetShipsManager()->SaveUniqueShipsWork();
		}
		else {
			//TODO: ? - gracz przegral, ale to juz powinno byc wykryte
		}
	}
	else {
		if( GetPlayer()->GetLives() > -1 ) {
			//graj nastepny poziom
			++m_iTexBkgIndex;
			if( m_iTexBkgIndex >= m_pGameCtrl->GetTexBackgroundSize() )
				m_iTexBkgIndex = 0;

			m_iActualGroup = 0;
			m_iDrawLevelInfo = 3;
			m_bBlockMultiplayerGame = GL_TRUE;
			m_iLevelToDisplay = m_iLevel;
			CreateEnemyShips();
			GetRAddStar()->SetWasCalledDraw( GL_FALSE );
			GetRBonus()->ResetCounterGeneratedBonuses();
			m_iTotalStartedDrawEnemy = 0;

			m_uiCounterPlayerShotGroup = 0;
			m_uiCounterEnemyShotGroup = 0;

			PlayNextMusic();
		}
		else {
			//TODO: ? - gracz przegral, ale to juz powinno byc wykryte
		}
	}
}

GLboolean CRGame::CanDoFade()
{
	return ( m_iLevel + 1 <= m_pGameCtrl->GetLevelsManager()->GetLevelsSize() && GetPlayer()->GetLives() > -1 );
}

GLvoid CRGame::JumpToLevel( GLint iLevel )
{
	m_iLevel = iLevel;
	m_iLevelToDisplay = m_iLevel;
	m_iActualGroup = 0;
	m_iDrawLevelInfo = 3;
	CreateEnemyShips();
}

GLvoid CRGame::CreateEnemyShips()
{
#if _USE_MULTIPLYER_
	if( !m_bIsMultiplayer ) {
		CreateEnemyShipsFinally();
		return;
	}

	m_bBlockMultiplayerGame = GL_FALSE;
		
	STransferTCPData sTransTCP = { 0 };
	sTransTCP.iPortNumber = PORT_TCP;
	sTransTCP.Callback = CallbackEnemyShipsReady;

	if( m_pGameCtrl->GetRMultiplayerMenu()->AmICreator() ) {
		//utworz sobie statki i je wyslij do drugiego gracza
		CreateEnemyShipsFinally();

		sTransTCP.iDataLength = GetEnemyShip()->GetEnemyShipSize() * sizeof( SEnemyShip );
		//sTransTCP.pData = new char[ sTransTCP.iDataLength ];
		//memcpy_s( sTransTCP.pData, sTransTCP.iDataLength, 
		//	GetEnemyShip()->GetEnemyShipArray(), sTransTCP.iDataLength );
		sTransTCP.pData = ( char* )GetEnemyShip()->GetEnemyShipArray();

		m_pGameCtrl->GetNetworkManager()->StartSendTCPTransfer( &sTransTCP );
	}
	else {
		SetVariablesForCreateEnemyShips();
		m_pGameCtrl->GetNetworkManager()->StartRecvTCPTransfer( &sTransTCP );
	}
#else
	CreateEnemyShipsFinally();
#endif // _USE_MULTIPLYER_
}

#if _USE_MULTIPLYER_
GLvoid CallbackEnemyShipsReady( GLint iResult, STransferTCPData *sTransTCP )
{
	if( !iResult ) {
		//TODO: obsluga bledu, np. totalne wyjscie z gry multiplayer.
		return;
	}

	if( sTransTCP->bSender ) {
		// czekaj na pakiet EPacketTypeStartMultiGame od drugiego gracza ( bo musi przetworzyc dane statkow )
	}
	else {
		// z otrzymanych danych zbuduj sobie tablice EnemyShip
		const GLint iEnemyShipSize = sizeof( SEnemyShip );
		GLint iStart = 0;
		while( iStart < sTransTCP->iDataLength ) {
			SEnemyShip *sEnemyShip = ( SEnemyShip* )&sTransTCP->pData[ iStart ];
			CGameControl::GetInstance()->GetRGame()->GetEnemyShip()->Add( *sEnemyShip );

			iStart += iEnemyShipSize;
		}

		// wyslij pakiet informujacy o rozpoczeciu gry
		CGameControl::GetInstance()->GetNetworkManager()->SendUDP(
			EPacketTypeStartMultiGame,
			NULL, 0, 
			CGameControl::GetInstance()->GetRMultiplayerMenu()->GetIPToConnect() );

		CGameControl::GetInstance()->GetRGame()->StartMultiplayerGame();
	}
}
#endif // _USE_MULTIPLYER_

GLvoid CRGame::SetVariablesForCreateEnemyShips()
{
	GetEnemyShip()->ClearAll();
	m_iNumberActualDrawEnemy = 0;
	//znajdz index z LevelsManagera odpowiadajacy m_iLevel
	m_iIndexLevelManager = m_pGameCtrl->GetLevelsManager()->GetIndexLevel( m_iLevel ); 
	GetRBoss()->SetNewDestinationPointForBoss();
}

GLvoid CRGame::CreateEnemyShipsFinally()
{
	PROFILER_BEGIN( _T("CRGame::CreateEnemyShips()") );

	SetVariablesForCreateEnemyShips();

	GLint iDivisor = 3;
	//UWAGA, dalej dzielimy ( iDivisor - 1 ), wiec iDivisor musi byc >= 2
	switch( m_pGameCtrl->GetRDifficultyLevel()->GetDifficultyLevel() ) {
		case ELevelEasy:
			iDivisor = 4;
			break;
		case ELevelNormal:
			iDivisor = 3;
			break;
		case ELevelHard:
			iDivisor = 2;
			break;
	}

	//dodajemy statki
	for( GLint i = 0; i < m_pGameCtrl->GetLevelsManager()->GetLevel( m_iIndexLevelManager ).iNumberGroups; ++i ) {
		GLint i3DObjIndex = m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).iIndexShip;
		//udostepnienie statku w przegladarce
		m_pGameCtrl->GetShipsManager()->SetAvailableInBrowse( i3DObjIndex, GL_TRUE );
		
		//przesuniecie dla lotu po skosie, od razu ze znakiem z randa
		GLfloat fOffsetXPosForSlantFly = m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).fOffsetXPosForSlantFly;

		GLint iFormation = 0;
		CVector3 cLeaderPos;
		GLfloat fSpeed = 0.0f;
		GLfloat fLeaderSpeed = 0.0f;
		GLint iNumberShips = m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).iNumberShips;
		//zmieniamy ilosc statkow zaleznie od poziomu trudnosci, dla normal nie zmieniamy
		switch( m_pGameCtrl->GetRDifficultyLevel()->GetDifficultyLevel() ) {
			case ELevelEasy:
				iNumberShips = static_cast< GLint >( ( static_cast< GLfloat >( iNumberShips ) * 0.7f ) );
				break;
			case ELevelHard:
				iNumberShips = static_cast< GLint >( ( static_cast< GLfloat >( iNumberShips ) * 1.3f ) );
				break;
		}
		for( GLint j = 0; j < iNumberShips; ++j ) {

			//ustalenie znaku dla lotu po skosie
			if( !( rand() % 2 ) ) //zmien znak na ujemny
				fOffsetXPosForSlantFly = -fOffsetXPosForSlantFly;

			//obliczenie formacji
			CVector3 cPos;
			switch( m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).iFormation ) {
				case 1: //kazdy statek osobno
					cPos.x = static_cast< GLfloat >( rand() % m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).iPosXForRand ) - static_cast< GLfloat >( m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).iPosXForRand / 2 ) + fOffsetXPosForSlantFly;
					cPos.y = 0.0f;
					cPos.z = MAX_GAME_DISTANCE;
					fSpeed = static_cast< GLfloat >( rand() % 140 ) + m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).fSpeed;
					iFormation = 0;
					break;
				case 2:
					++iFormation;
					if( iFormation == 1 ) {
						//statek pierwszy - lider
						cPos.x = static_cast< GLfloat >( rand() % m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).iPosXForRand ) - static_cast< GLfloat >( m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).iPosXForRand / 2 ) + fOffsetXPosForSlantFly;
						cPos.y = 0.0f;
						cPos.z = MAX_GAME_DISTANCE;
						cLeaderPos = cPos;
						fSpeed = static_cast< GLfloat >( rand() % 140 ) + m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).fSpeed;
						fLeaderSpeed = fSpeed;
					}
					else if( iFormation == 2 ) {
						//skrzydlowy
						cPos.x = cLeaderPos.x - 20.0f;
						cPos.y = 0.0f;
						cPos.z = cLeaderPos.z - 30.0f;
						fSpeed = fLeaderSpeed;
						iFormation = 0;
					}
					break;
				case 3:
					++iFormation;
					if( iFormation == 1 ) {
						//statek pierwszy - lider
						cPos.x = static_cast< GLfloat >( rand() % m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).iPosXForRand ) - static_cast< GLfloat >( m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).iPosXForRand / 2 ) + fOffsetXPosForSlantFly;
						cPos.y = 0.0f;
						cPos.z = MAX_GAME_DISTANCE;
						cLeaderPos = cPos;
						fSpeed = static_cast< GLfloat >( rand() % 140 ) + m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).fSpeed;
						fLeaderSpeed = fSpeed;
					}
					else if( iFormation == 2 ) {
						//skrzydlowy1
						cPos.x = cLeaderPos.x - 20.0f;
						cPos.y = 0.0f;
						cPos.z = cLeaderPos.z - 30.0f;
						fSpeed = fLeaderSpeed;
					}
					else if( iFormation == 3 ) {
						//skrzydlowy2
						cPos.x = cLeaderPos.x + 20.0f;
						cPos.y = 0.0f;
						cPos.z = cLeaderPos.z - 30.0f;
						fSpeed = fLeaderSpeed;
						iFormation = 0;
					}
					break;
			}
			
			GetEnemyShip()->Add(
				i3DObjIndex, //id obiektu
				m_aPointsForShootDown[ i3DObjIndex ], //ilosc punktow za zestrzelenie
				cPos, //pozycja startowa
				fSpeed, //speed
				m_pGameCtrl->GetShipsManager()->GetUniqueShip( i3DObjIndex ).fCollisionA,//dla kolizji
				m_pGameCtrl->GetShipsManager()->GetUniqueShip( i3DObjIndex ).fCollisionB,
				m_pGameCtrl->GetShipsManager()->GetShieldPoints( i3DObjIndex ) / iDivisor,
				m_pGameCtrl->GetShipsManager()->GetArmourPoints( i3DObjIndex ) / iDivisor,
				m_pGameCtrl->GetShipsManager()->GetArmamentPoints( i3DObjIndex ) / iDivisor,
				m_pGameCtrl->GetShipsManager()->GetManoeuvringPercent( i3DObjIndex ) / ( iDivisor - 1 ),
				m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).bCosFly, //po cosinusoidzie
				static_cast< GLboolean >( rand() % 2 ), //znak na cosinusoidy, -/+
				80.0f, //wartosc zwiekszania kata cos
				m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).bSlantFly, //czy lot po skosie
				m_pGameCtrl->GetLevelsManager()->GetLavelGroup( m_iIndexLevelManager, i ).iFormation, //formacja
				i, //index grupy
				m_pGameCtrl->GetShipsManager()->GetUniqueShip( i3DObjIndex ).bCanManoeuvring,
				m_pGameCtrl->GetShipsManager()->GetUniqueShip( i3DObjIndex ).bCanFire
				);
		}
	}

	//dodajemy statek bossa
	GLint iIndexOfBoss = m_pGameCtrl->GetLevelsManager()->GetLevel( m_iIndexLevelManager ).iIndexBoss;
	if( iIndexOfBoss > -1 ) {
		CVector3 cPos( GLfloat( rand() % GLint( PLAYER_ZONE_RIGHT_X * 2.0f ) ) - PLAYER_ZONE_RIGHT_X,
					   0.0f, MAX_GAME_DISTANCE ) ;

		GLfloat fSpeed = static_cast< GLfloat >( rand() % 140 ) + 160.0f;
		GLint iExtraPoints = 5 * m_iLevel;

		GetEnemyShip()->AddBoss(
			iIndexOfBoss,
			m_aPointsForShootDown[ iIndexOfBoss ] * iExtraPoints, //ilosc punktow za zestrzelenie
			cPos, //pozycja startowa
			fSpeed, //szybkosc lotu
			m_pGameCtrl->GetShipsManager()->GetUniqueShip( iIndexOfBoss ).fCollisionA,//dla kolizji
			m_pGameCtrl->GetShipsManager()->GetUniqueShip( iIndexOfBoss ).fCollisionB,
			( m_pGameCtrl->GetShipsManager()->GetShieldPoints( iIndexOfBoss ) / iDivisor ) * iExtraPoints,
			( m_pGameCtrl->GetShipsManager()->GetArmourPoints( iIndexOfBoss ) / iDivisor ) * iExtraPoints,
			( m_pGameCtrl->GetShipsManager()->GetArmamentPoints( iIndexOfBoss ) / iDivisor ) * iExtraPoints,
			( m_pGameCtrl->GetShipsManager()->GetManoeuvringPercent( iIndexOfBoss ) / ( iDivisor - 1 ) ) * iExtraPoints,
			m_pGameCtrl->GetLevelsManager()->GetLevel( m_iIndexLevelManager ).iNumberGroups // indeks grupy, jako ostatni
			);
	}

	//zapisac udostepnione statki wroga
	m_pGameCtrl->GetShipsManager()->SaveUniqueShipsWork();

	PROFILER_END();
}

GLvoid CRGame::AddPlayerShot()
{
	//sprawdz czy obiekt gracza moze strzelac
	if( !m_pGameCtrl->GetShipsManager()->GetUniqueShip( GetPlayer()->Get3DObjectIndex() ).bCanFire )
		return;

	if( GetPlayer()->GetPlayerExplode() > 0 )
		return; //gracz eksploduje

	//nie ma napisow ready! i zyjemy to mozemy strzelac
	if( m_iDrawLevelInfo > 0 || IsEndGame() )
		return;

	//sprawdz czy gracz ma amunicje
	if( m_eWeaponType != EWeaponLaser ) {
		if( GetRBonus()->GetAmmoFromActualWeapon( m_eWeaponType ) == 0 ) {
			//zmien bron na pierwsza
			SelectWeapon( EWeaponLaser );
			return;
		}
	}

	//sprawdzamy jaka bron ma gracz
	switch( m_eWeaponType ) {
		case EWeaponLaser:
			AddPlayerShot_Laser();
			break;
		case EWeaponChaingun:
			AddPlayerShot_Chaingun();
			break;
		case EWeaponImpulseLaser:
			CounterPlayerShot_ImpulseLaser();
			break;
		case EWeaponEmitterRocket:
			GetREmitterRockets()->AddPlayerShot_EmitterRocket();
			break;
		case EWeaponHomingRocket:
			GetRHomingRockets()->AddPlayerShot_HomingRocket();
			break;
	}
}

GLvoid CRGame::AddShot( SUniversalBillboard &sBillboarShot, GLfloat fOffset /*= 0.0f*/, GLint iModeFly /*= 0*/, GLint iExtras /*= 0*/ )
{
	sBillboarShot.sShot.uiGroupIndex = ++m_uiCounterPlayerShotGroup;
	sBillboarShot.cPos = GetPlayer()->GetPosition();
	sBillboarShot.cPos.x += fOffset;
	sBillboarShot.cPos.z -= GetPlayer()->GetCollisionA();
	sBillboarShot.sShot.iModeFly = iModeFly;
	GetUnivBillboard()->AddUniversalBillboard( sBillboarShot );

	//dodaj dodatkowe pocisnki, np. dla lasera
	for( GLint i = 0; i < iExtras; ++i ) {
		sBillboarShot.cPos.z += ( 0.5f * static_cast< GLfloat >( i + 1 ) );
		GetUnivBillboard()->AddUniversalBillboard( sBillboarShot );
	}
}

GLvoid CRGame::AddPlayerShot_Laser()
{
	//oblicz czestotliwosc wystrzalu zaleznie od gwiazdki dla uzbrojenia
	unsigned long ulMS = static_cast<unsigned long>( GetPlayer()->GetStarsArmament() );
	switch( ulMS ) {
		case 0:
			ulMS = 450 - GetRBonus()->GetLaserRateShoot();
			break;
		case 1:
			ulMS = 385 - GetRBonus()->GetLaserRateShoot();
			break;
		case 2:
			ulMS = 320 - GetRBonus()->GetLaserRateShoot();
			break;
		case 3:
			ulMS = 255 - GetRBonus()->GetLaserRateShoot();
			break;
	}
	if( !m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( ulMS, 2 ) )
		return;

	SUniversalBillboard sBillboarShot;
	sBillboarShot.eType = EUBTypePlayerShot;

	sBillboarShot.sShot.bDraw = GL_TRUE;
	sBillboarShot.sShot.fSpeed = 750.0f;//150.0f; //900.0f i predkosc statku wroga 260.0f - za szybko
	sBillboarShot.sShot.iTexture = m_pGameCtrl->m_iTexParticle01;//czasteczka
	sBillboarShot.sShot.iArmament = GetPlayer()->GetArmamentActual() + GetRBonus()->GetLaserPowerShoot();
	sBillboarShot.sShot.i3DObjIndex = -1;
	sBillboarShot.sShot.eWeaponType = EWeaponLaser;

	switch( GetRBonus()->GetLaserNumberShots() ) {
		case 1:
			AddShot( sBillboarShot,  0.0f,  0, 8 );
			break;
		case 2:
			AddShot( sBillboarShot,  3.0f,  0, 8 );
			AddShot( sBillboarShot, -3.0f,  0, 8 );
			break;
		case 3:
			AddShot( sBillboarShot,  0.0f,  0, 8 );
			AddShot( sBillboarShot,  4.0f,  0, 8 );
			AddShot( sBillboarShot, -4.0f,  0, 8 );
			break;
		case 4:
			AddShot( sBillboarShot,  3.0f,  0, 8 );
			AddShot( sBillboarShot, -3.0f,  0, 8 );
			AddShot( sBillboarShot,  3.5f,  1, 8 );
			AddShot( sBillboarShot, -3.5f, -1, 8 );
			break;
		case 5:
			AddShot( sBillboarShot,  0.0f,  0, 8 );
			AddShot( sBillboarShot,  4.0f,  0, 8 );
			AddShot( sBillboarShot, -4.0f,  0, 8 );
			AddShot( sBillboarShot,  4.5f,  1, 8 );
			AddShot( sBillboarShot, -4.5f, -1, 8 );
			break;
		case 6:
			AddShot( sBillboarShot,  3.0f,  0, 8 );
			AddShot( sBillboarShot, -3.0f,  0, 8 );
			AddShot( sBillboarShot,  3.5f,  1, 8 );
			AddShot( sBillboarShot, -3.5f, -1, 8 );
			AddShot( sBillboarShot,  5.0f,  2, 8 );
			AddShot( sBillboarShot, -5.0f, -2, 8 );
			break;
		default://case 7:
			AddShot( sBillboarShot,  0.0f,  0, 8 );
			AddShot( sBillboarShot,  4.0f,  0, 8 );
			AddShot( sBillboarShot, -4.0f,  0, 8 );
			AddShot( sBillboarShot,  4.5f,  1, 8 );
			AddShot( sBillboarShot, -4.5f, -1, 8 );
			AddShot( sBillboarShot,  6.0f,  2, 8 );
			AddShot( sBillboarShot, -6.0f, -2, 8 );
			break;
	}

	if( CWindowData::GetInstance()->SettingFile().bSound )
		COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundLaser );
}

GLvoid CRGame::AddPlayerShot_Chaingun()
{
	//czestotliwosc strzelania stala, co 50ms
	if( !m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 40/*20*/, 2 ) )
		return;

	SUniversalBillboard sBillboarShot;
	sBillboarShot.eType = EUBTypePlayerShot;

	sBillboarShot.sShot.bDraw = GL_TRUE;
	sBillboarShot.sShot.fSpeed = 1500.0f;//150.0f; //900.0f i predkosc statku wroga 260.0f - za szybko
	sBillboarShot.sShot.iTexture = m_pGameCtrl->m_iTexParticle01;//czasteczka
	//
	//1 gwiazdka uzbrojenia to 2 pkt, + 1 pkt ( bo moze byc 0 gwiazdek )
	//1 znajdzka daje + 1 pkt sily pocisku.
	sBillboarShot.sShot.iArmament = ( ( GetPlayer()->GetStarsArmament() * 2 ) + 1 + GetRBonus()->GetChaingunPowerShoot() );
	sBillboarShot.sShot.i3DObjIndex = -1;
	sBillboarShot.sShot.iModeFly = 0;
	sBillboarShot.sShot.eWeaponType = EWeaponChaingun;

	GetRBonus()->ReduceAmmoChaingun( 1 );

	switch( GetRBonus()->GetChaingunNumberShots() ) {
		case 1:
			AddShot( sBillboarShot,  8.0f,  0, 3 );//pierwsza wiazka
			AddShot( sBillboarShot, -8.0f,  0, 3 );//druga wiazka
			break;
		case 2:
			//pierwsza wiazka
			AddShot( sBillboarShot, -8.0f,  0, 3 );
			AddShot( sBillboarShot,-10.0f,  0, 3 );
			//druga wiazka
			AddShot( sBillboarShot,  8.0f,  0, 3 );
			AddShot( sBillboarShot, 10.0f,  0, 3 );
			break;
		case 3:
			//pierwsza wiazka
			AddShot( sBillboarShot, -8.0f,  0, 3 );
			AddShot( sBillboarShot,-10.0f,  0, 3 );
			AddShot( sBillboarShot,-12.0f,  0, 3 );
			//druga wiazka
			AddShot( sBillboarShot,  8.0f,  0, 3 );
			AddShot( sBillboarShot, 10.0f,  0, 3 );
			AddShot( sBillboarShot, 12.0f,  0, 3 );
			break;
		default: //case 4:
			//pierwsza wiazka
			AddShot( sBillboarShot, -8.0f,  1, 3 );
			AddShot( sBillboarShot,-10.0f,  0, 3 );
			AddShot( sBillboarShot,-12.0f,  0, 3 );
			AddShot( sBillboarShot,-14.0f, -1, 3 );
			//druga wiazka
			AddShot( sBillboarShot,  8.0f, -1, 3 );
			AddShot( sBillboarShot, 10.0f,  0, 3 );
			AddShot( sBillboarShot, 12.0f,  0, 3 );
			AddShot( sBillboarShot, 14.0f,  1, 3 );
			break;
	}

	if( CWindowData::GetInstance()->SettingFile().bSound ) {
		if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 70, 15 ) )
			COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundChaingun );
	}
}

GLvoid CRGame::CounterPlayerShot_ImpulseLaser()
{
	if( m_bImpulseLaserShot ) //jeszcze rysuje stary impuls
		return;

	// Czestotliwosc strzelania stala, co 1000ms.

	// Gdy w CheckAnimationTime zastosuje domyœlnie GL_TRUE to powstaje bug ID: #46. Ten bug mo¿na naprawiæ, podaj¹c
	// do CheckAnimationTime GL_FALSE. Ale wtedy powstaje inny bug opisany w uwagach do bug-a ID: #46.
	if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 1000, 16/*, GL_FALSE*/ ) ) {
		m_iImpulseLaserCounterTime = 500;//<- z 1000 / 2
		AddPlayerShot_ImpulseLaser();
		m_iImpulseLaserCounterTime = 0;
	}
	else {
		//warunek z pauza aby m_iImpulseLaserCounterTime nie zwiekszal sie gdy bede trzymal fire w trakcie pauzy
		if( !m_bPause )
			m_iImpulseLaserCounterTime = ( m_pGameCtrl->GetSpeedCtrl()->GetActualAnimationTime( 16 ) / 2 );

		m_pGameCtrl->Disable2D();
		glLoadIdentity();
		SetCamera();
		glTranslatef( GetPlayer()->GetPosition().x, GetPlayer()->GetPosition().y, GetPlayer()->GetPosition().z );
		glRotatef( 180.0f, 0.0f, 1.0f, 0.0f );
		m_pGameCtrl->GetParticles()->DrawEmitter( m_iEmitterImpulseLaser, CVector3( GetPlayer()->GetCollisionA() + 5.0f, 0.0f, 0.0f ) );
	}
}

GLvoid CRGame::AddPlayerShot_ImpulseLaser()
{
	SUniversalBillboard sBillboarShot;
	sBillboarShot.eType = EUBTypePlayerShot;

	sBillboarShot.sShot.bDraw = GL_TRUE;
	sBillboarShot.sShot.fSpeed = 10000.0f; //900.0f i predkosc statku wroga 260.0f - za szybko
	sBillboarShot.sShot.iTexture = m_pGameCtrl->m_iTexParticle01;//czasteczka
	//sBillboarShot.sShot.iArmament = m_iImpulseLaserCounterTime + GetPlayer()->GetArmamentActual() + GetRBonus()->GetPowerShoot();
	//1 gwiazdka uzbrojenia to 2 pkt, + 1 pkt ( bo moze byc 0 gwiazdek )
	//1 znajdzka daje + 1 pkt sily pocisku.
	sBillboarShot.sShot.iArmament = ( m_iImpulseLaserCounterTime / 5 ) + ( ( GetPlayer()->GetStarsArmament() * 2 ) + 1 + GetRBonus()->GetImpulseLaserPowerShoot() );
	sBillboarShot.sShot.iScaleImpulseLaser = m_iImpulseLaserCounterTime;
	sBillboarShot.sShot.i3DObjIndex = -1;
	sBillboarShot.sShot.eWeaponType = EWeaponImpulseLaser;
	sBillboarShot.sShot.iModeFly = 0;
	sBillboarShot.sShot.uiGroupIndex = 0;//++m_uiCounterPlayerShotGroup;

	GetRBonus()->ReduceAmmoImpulseLaser( 1 );

	//dodaj na caly ekran
	switch( GetRBonus()->GetImpulseLaserNumberShots() ) {
		case 1:
		case 2:
		{	//jedna wiazka
			sBillboarShot.cPos = GetPlayer()->GetPosition();
			sBillboarShot.cPos.z -= GetPlayer()->GetCollisionA();
			GLfloat fPosZ = sBillboarShot.cPos.z;
			for( GLfloat f = 0.0f; f < -MAX_GAME_DISTANCE; f += 10.0f ) {
				sBillboarShot.cPos.z = fPosZ - f;
				GetUnivBillboard()->AddUniversalBillboard( sBillboarShot );
			}
			break;
		}
		default:
		{
			//2 wiazki max
			sBillboarShot.cPos = GetPlayer()->GetPosition();
			sBillboarShot.cPos.z -= GetPlayer()->GetCollisionA();
			GLfloat fPosZ = sBillboarShot.cPos.z;
			for( GLfloat f = 0.0f; f < -MAX_GAME_DISTANCE; f += 10.0f ) {
				sBillboarShot.cPos.z = fPosZ - f;

				sBillboarShot.cPos.x -= 6.0f;
				GetUnivBillboard()->AddUniversalBillboard( sBillboarShot );

				sBillboarShot.cPos.x += 12.0f;
				GetUnivBillboard()->AddUniversalBillboard( sBillboarShot );
				sBillboarShot.cPos.x -= 6.0f;
			}
			break;
		}
	}

	m_bImpulseLaserShot = GL_TRUE;
	m_fImpulseLaserDrawCounter = 0.0f;

	//aby dla nastepnego strzalu czasteczki byly zresetowane:
	m_pGameCtrl->GetParticles()->SetParticleInEmitterAsLive( m_iEmitterImpulseLaser );

	if( CWindowData::GetInstance()->SettingFile().bSound )
		COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundImpulseLaser );
}

GLvoid CRGame::AddShot( const SEnemyShip &sEnemyShip, SUniversalBillboard &sBillboarShot, GLfloat fOffset /*= 0.0f*/, GLint iModeFly /*= 0*/, GLint iExtras /*= 0*/ )
{
	sBillboarShot.sShot.uiGroupIndex = ++m_uiCounterEnemyShotGroup;
	sBillboarShot.cPos = sEnemyShip.cActualPos;
	sBillboarShot.cPos.x += fOffset;
	//sBillboarShot.cPos.z -= GetPlayer()->GetCollisionA();
	sBillboarShot.sShot.iModeFly = iModeFly;
	GetUnivBillboard()->AddUniversalBillboard( sBillboarShot );

	//dodaj dodatkowe pocisnki, np. dla lasera
	for( GLint i = 0; i < iExtras; ++i ) {
		sBillboarShot.cPos.z -= ( 0.5f * static_cast< GLfloat >( i + 1 ) );
		GetUnivBillboard()->AddUniversalBillboard( sBillboarShot );
	}
}

GLvoid CRGame::AddEnemyShot( SEnemyShip &sEnemyShip )
{
	//sprawdzenie czy obiekt moze strzelac
	if( !sEnemyShip.bCanFire )
		return;

	if( GetPlayer()->GetLives() == -1 )
		return; //gracz nie zyje - nie ma co strzelac

	//jezeli statek poza polem gracza to nie ma co strzelac
	if( sEnemyShip.cActualPos.x < PLAYER_ZONE_LEFT_X || sEnemyShip.cActualPos.x > PLAYER_ZONE_RIGHT_X )
		return;

	if( sEnemyShip.iAnimCtrlIndex == 0 ) {
		//znajdz wolny index dla anim ctrl
		GLint iIndex = GetREnemyShotAnimCtrl()->GetFreeIndex();
		if( iIndex > -1 )
			sEnemyShip.iAnimCtrlIndex = iIndex;
	}

	//oblicz czestotliwosc wystrzalu zaleznie od gwiazdki dla uzbrojenia
	unsigned long ulMS = static_cast<unsigned long>( m_pGameCtrl->GetShipsManager()->GetUniqueShip( sEnemyShip.i3DObjIndex ).iStarsArmament );
	switch( ulMS ) {
		case 1:
			ulMS = 385 * m_pGameCtrl->GetLevelsManager()->GetLevel( m_iIndexLevelManager ).iFrequencyShoot;
			break;
		case 2:
			ulMS = 320 * m_pGameCtrl->GetLevelsManager()->GetLevel( m_iIndexLevelManager ).iFrequencyShoot;
			break;
		case 3:
			ulMS = 255 * m_pGameCtrl->GetLevelsManager()->GetLevel( m_iIndexLevelManager ).iFrequencyShoot;
			break;
		default: //0
			ulMS = 450 * m_pGameCtrl->GetLevelsManager()->GetLevel( m_iIndexLevelManager ).iFrequencyShoot;
			break;
	}
	if( !m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( ulMS, sEnemyShip.iAnimCtrlIndex ) )
		return;

	//modyfikator do szansy na oddanie strzalu, zaleznie od poziomu trudnosci
	GLint iPercentMod = 0;
	switch( m_pGameCtrl->GetRDifficultyLevel()->GetDifficultyLevel() ) {
		case ELevelEasy:
			iPercentMod = -10;
			break;
		case ELevelHard:
			iPercentMod = 15;
			break;
	}
	if( !sEnemyShip.bBoss ) { //boss zawsze wystrzeli
		if( rand() % 100 > m_pGameCtrl->GetLevelsManager()->GetLevel( m_iIndexLevelManager ).iPercentChanceToShoot + iPercentMod )
			return;
	}

	SUniversalBillboard sBillboarShot;
	sBillboarShot.eType = EUBTypeEnemyShot;

	sBillboarShot.sShot.bDraw = GL_TRUE;
	sBillboarShot.sShot.fSpeed = sEnemyShip.fSpeed + 300.0f;
	sBillboarShot.sShot.iTexture = m_pGameCtrl->m_iTexParticle01; //czasteczka
	sBillboarShot.sShot.iArmament = sEnemyShip.iArmament;
	sBillboarShot.sShot.i3DObjIndex = -1;

	AddShot( sEnemyShip, sBillboarShot, 0.0f, 0, 8 );
	if( sEnemyShip.bBoss ) {
		AddShot( sEnemyShip, sBillboarShot,  3.0f,  2, 8 );
		AddShot( sEnemyShip, sBillboarShot, -3.0f, -2, 8 );
		AddShot( sEnemyShip, sBillboarShot,  6.0f,  10, 8 );
		AddShot( sEnemyShip, sBillboarShot, -6.0f, -10, 8 );
	}
}

GLfloat CRGame::CalculateScaleShot( GLfloat fArmament, GLfloat fMaxBorder /*= 2.0f*/ )
{
	/*
	fMaxBorder - maksymalna wartosc skalowania, domyslnie 2.0. Jezeli podamy 0.0
					to znaczy, ze ma max granicy
	*/
	GLfloat fScale = 0.85f;
	fScale += ( fArmament / 200.0f );
	if( fScale < 1.0f ) fScale = 1.0f;
	if( fMaxBorder > 0.0f ) {
		if( fScale > fMaxBorder ) fScale = fMaxBorder;
	}

	//przy / 100:
	//0 gwiazdek = 0.15f + 0.85f = 1.0f
	//1 gwiazdka = 0.75f + 0.85f = 1.6f
	//2 gwiazdki = 1.35f + 0.85f = 2.2f => czyli 2.0f
	//3 gwiazdki = 1.95f + 0.85f = 2.8f => czyli 2.0f

	return fScale;
}

GLvoid CRGame::DrawPlayerShots( SUniversalBillboard &in_sUB, GLint iIndex )
{
	//dla impulse laser - przestan rysowac i usun
	if( !m_bImpulseLaserShot && in_sUB.sShot.eWeaponType == EWeaponImpulseLaser )
		in_sUB.sShot.bDraw = GL_FALSE;

	if( in_sUB.cPos.z < MAX_GAME_DISTANCE || !in_sUB.sShot.bDraw ) {
		GetUnivBillboard()->DeleteUniversalBillboard( iIndex );
		return;
	}

	glPushMatrix();
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	m_pGameCtrl->GetTexLoader()->SetTexture( in_sUB.sShot.iTexture );

	GLfloat fScale = 0.75f;
	switch( in_sUB.sShot.eWeaponType ) {
		case EWeaponLaser:
			//wielkosc pocisku zalezna od jego sily
			fScale = CalculateScaleShot( static_cast< GLfloat >( in_sUB.sShot.iArmament ) );
			glColor4f( 0.1f, 0.7f, 1.0f, 1.0f ); //jasny niebieski
			break;
		case EWeaponChaingun:
			fScale = 0.5f;
			glColor4f( 1.0f, 1.0f, 0.2f, 1.0f ); //zolty
			break;
		case EWeaponImpulseLaser:
			//wielkosc pocisku zalezna od jego sily * 2.2f
			fScale = CalculateScaleShot( static_cast< GLfloat >( in_sUB.sShot.iArmament + in_sUB.sShot.iScaleImpulseLaser ), 0.0f ) * 2.2f;
			glColor4f( 0.1f, 0.3f, 0.6f, 1.0f ); //jasny niebieski
			break;
		default:
			glColor4f( 1.0f, 1.0f, 1.0f,	1.0f ); //bialy
			break;
	}

	m_pGameCtrl->GetBillboard()->DrawQuadBillboard( in_sUB.cPos, fScale );

	if( in_sUB.sShot.eWeaponType != EWeaponImpulseLaser ) {	// nie dla impulse laser
		//dla broni z poruszajacymi sie pociskami
		in_sUB.cPos.z -= ( in_sUB.sShot.fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		GLfloat fDivision = static_cast< GLfloat >( in_sUB.sShot.iModeFly );
		if( fDivision < 0 )
			fDivision = -fDivision;
		fDivision = 20.0f - ( fDivision * 1.25f );
		if( in_sUB.sShot.iModeFly < 0 )
			in_sUB.cPos.x -= ( ( in_sUB.sShot.fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() ) / fDivision );
		else if( in_sUB.sShot.iModeFly > 0 )
			in_sUB.cPos.x += ( ( in_sUB.sShot.fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() ) / fDivision );
	}

	CheckCollisionEnemyShot( in_sUB.sShot, in_sUB.cPos );

	glPopMatrix();
	glDisable( GL_BLEND );	
}

GLvoid CRGame::DrawEnemyShots( SUniversalBillboard &in_sUB, GLint iIndex )
{
	if( in_sUB.cPos.z > MIN_GAME_DISTANCE || !in_sUB.sShot.bDraw ) {
		GetUnivBillboard()->DeleteUniversalBillboard( iIndex );
		return;
	}

	glPushMatrix();
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glColor4f( 1.0f, 0.1f, 0.1f,	1.0f ); //czerwony

	m_pGameCtrl->GetTexLoader()->SetTexture( in_sUB.sShot.iTexture );

	GLfloat fScale = CalculateScaleShot( static_cast< GLfloat >( in_sUB.sShot.iArmament ) );
	m_pGameCtrl->GetBillboard()->DrawQuadBillboard( in_sUB.cPos, fScale );

	GLfloat fSpeed = ( in_sUB.sShot.fSpeed / GetRBonus()->GetSlowEnemiesDivision() );
	in_sUB.cPos.z += ( fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );

	GLfloat fDivision = static_cast< GLfloat >( in_sUB.sShot.iModeFly );
	if( fDivision < 0 )
		fDivision = -fDivision;
	fDivision = 20.0f - ( fDivision * 1.25f );
	if( in_sUB.sShot.iModeFly < 0 )
		in_sUB.cPos.x -= ( ( fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() ) / fDivision );
	else if( in_sUB.sShot.iModeFly > 0 )
		in_sUB.cPos.x += ( ( fSpeed * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() ) / fDivision );

	CheckCollisionPlayerShot( in_sUB.sShot, in_sUB.cPos );

	glPopMatrix();
	glDisable( GL_BLEND );
}

GLvoid CRGame::CheckCollisionPlayerShot( SShot &sShot, CVector3 &cShotPos )
{
	//gracz nie ¿yje lub eksploduje
	if( GetPlayer()->GetLives() == -1 || GetPlayer()->GetPlayerExplode() > 0 )
		return;

	//pocisk jest poza obszarem garcza
	if( cShotPos.z < PLAYER_ZONE_MAX - GetPlayer()->GetCollisionA() )
		return;

	//czy gracz jest niezniszczalny
	if( GetPlayer()->IsImmortality() )
		return;

	//gracz wykrecil beczke dla tego pocisku i nie moze byc juz sprawdzana dla niego kolizja
	if( sShot.i3DObjIndex == 1 )
		return;

	if( GetPlayer()->GetCollisionRect().PtInRect( CVector2( cShotPos.x, cShotPos.z ) ) ) {
		//sparwdzenie czy gracz wymiga sie od kolizji poprzez beczke
		if( GetPlayer()->IsManoeuvring() ) {
			if( rand() % 100 <= GetPlayer()->GetManoeuvring() ) {
				sShot.i3DObjIndex = 1;
				//jezeli pocisk tworzy grupe, to wszystkim pociskom z grupy zaznaczamy, ze i3DObjIndex
				//manewrowal na tym pocisku
				if( sShot.uiGroupIndex > 0 )
					GetUnivBillboard()->SetManoeuvringForShotsGroup( EUBTypeEnemyShot, sShot.uiGroupIndex, 1 );
				if( CWindowData::GetInstance()->SettingFile().bSound )
					COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundJoy );

				return; //wymigal sie
			}
		}

		CalculatePlayerDestroy( sShot );
	}
}

GLvoid CRGame::CalculatePlayerDestroy( SShot &sShot )
{
	if( GetPlayer()->GetShieldActual() >= sShot.iArmament ) {
		GetPlayer()->DecreaseShieldActual( sShot.iArmament );
		sShot.bDraw = GL_FALSE; //pocis "martwy"
	}
	else {
		//pocisk calkowicie niszczy oslone gracza i jeszcz ma punkty
		sShot.iArmament -= GetPlayer()->GetShieldActual();
		GetPlayer()->SetShieldActual( 0 );
		if( GetPlayer()->GetArmourActual() >= sShot.iArmament ) {
			GetPlayer()->DecreaseArmourActual( sShot.iArmament );
			sShot.bDraw = GL_FALSE; //pocis "martwy"
		}
		else {
			//pocisk calkowicie nieszczy pacerz gracza i jescze ma punkty
			sShot.iArmament -= GetPlayer()->GetArmourActual();
			GetPlayer()->SetArmourActual( 0 );
			if( sShot.iArmament > 0 ) {
				//statek zniszczony
				if( --sShot.iArmament == 0 )
					sShot.bDraw = GL_FALSE; //pocis "martwy"

				PlayerLoseLive();

				if( GetPlayer()->GetLives() > -1 ) {
					GetPlayer()->SetArmourActual( GetPlayer()->GetArmourMax() );
					GetPlayer()->SetShieldActual( GetPlayer()->GetShieldMax() );
				}

				//wstawienie animacji wybuchu gracza
				AddExplosion( GetPlayer()->GetPosition(), 25.0f, GL_FALSE );
			}
		}
	}

	//jezeli pocisk matrwy, to ustaw na martwe takze inne pociski z jego grupy
	if( !sShot.bDraw ) {
		if( sShot.uiGroupIndex > 0 ) //pocisk tworzy grupe
			GetUnivBillboard()->SetNoDrawForShotsGroup( EUBTypeEnemyShot, sShot.uiGroupIndex );
	}
	else {
		//jezeli pocisk zyje, to ustaw sShot.iArmament wszytskim pociskom z grupy
		if( sShot.uiGroupIndex > 0 ) //pocisk tworzy grupe
			GetUnivBillboard()->SetArmamentForShotsGroup( EUBTypeEnemyShot, sShot.uiGroupIndex, sShot.iArmament );
	}

	CalculateShieldIndicator();
	CalculateArmourIndicator();

	GetPlayer()->SetPlayerHit();
#ifdef DRAW_PLAYER_SHIELD_3D
	GetPlayer()->SetDrawPlayerShield3D( GL_TRUE );
#else
	GetUnivBillboard()->SetDrawPlayerShield2D();
#endif
}

GLvoid CRGame::CheckCollisionEnemyShot( SShot &sShot, CVector3 &cShotPos )
{
	//czy jest trafienie wrogiego statku, wywolywane co klatke
	//jezeli trafienie ustaw bAlive na FALSE

	for( GLint i = GetEnemyShip()->GetEnemyShipSize() - 1; i >= 0; --i ) {
		if( !GetEnemyShip()->GetEnemyShip( i ).bAlive || !GetEnemyShip()->GetEnemyShip( i ).bStart )
			continue;

		//ten statek wykrecil juz beczke dla tego pocisku i nie
		//moze byc juz sprawdzana dla niego kolizja
		if( sShot.i3DObjIndex == i )
			continue;

		GLboolean bIsCollision = GL_FALSE;
		if( sShot.eWeaponType == EWeaponHomingRocket || sShot.eWeaponType == EWeaponRemoteRocket ) {
			CVector2 cPtTopLeft( cShotPos.x - COLLISION_B_REMOTE_ROCKET,
								 cShotPos.z - COLLISION_A_REMOTE_ROCKET );
			CVector2 cPtBottomRight( cShotPos.x + COLLISION_B_REMOTE_ROCKET,
									 cShotPos.z + COLLISION_A_REMOTE_ROCKET );
			CRectangle cRocketRect( cPtTopLeft, cPtBottomRight );
			CRectangle cEnemyRect = GetEnemyShip()->GetCollisionRect( i );

			bIsCollision = cRocketRect.RectInRect( cEnemyRect );
			if( !bIsCollision )
				bIsCollision = cEnemyRect.RectInRect( cRocketRect );
		}
		else
			bIsCollision = GetEnemyShip()->GetCollisionRect( i ).PtInRect( CVector2( cShotPos.x, cShotPos.z ) );

		if( !bIsCollision )
			continue; //nie ma kolizji
			
		//jest kolizja

		//Jezeli pocisk to zdalna rakieta, to eksploduj i return
		if( sShot.eWeaponType == EWeaponRemoteRocket ) {
			//tutaj zdalna rakieta zawsze eksploduje a nie przebija
			//sie prze statek wroga jak zwykly pocisk
			GetRRemoteRocket()->DetonationRemoteRocket( GL_FALSE );
			return;
		}

		//sprawdzenie czy moze robic uniki
		if( GetEnemyShip()->GetEnemyShip( i ).bCanManoeuvring ) {
			//sprawdzam czy nie wywinie beczki
			if( ( rand() % 100 ) <= GetEnemyShip()->GetEnemyShip( i ).iManoeuvring ) {
				if( !GetEnemyShip()->GetEnemyShip( i ).bIsManoeuvring ) {
					GetEnemyShip()->GetEnemyShip( i ).bIsManoeuvring = GL_TRUE;
					GetEnemyShip()->GetEnemyShip( i ).bManoeuvringSign = static_cast< GLboolean >( rand() % 2 );
					GetEnemyShip()->GetEnemyShip( i ).fManoeuvringRot = 0.0f;
				}
				sShot.i3DObjIndex = i;
				//jezeli pocisk tworzy grupe, to innym pociskom z grupy takze zaznaczamy, ze i3DObjIndex
				//manewrowal na tym pocisku
				if( sShot.uiGroupIndex > 0 )
					GetUnivBillboard()->SetManoeuvringForShotsGroup( EUBTypePlayerShot, sShot.uiGroupIndex, i );
				continue;
			}
		}
		CalculateEnemyDestroy( GetEnemyShip()->GetEnemyShip( i ), sShot, cShotPos );
	}
}

GLvoid CRGame::CalculateEnemyDestroy( SEnemyShip &sEnemyShip, SShot &sShot, CVector3 &cShotPos )
{
	GetVisualEnemyHit()->SetToDraw( CVector3( cShotPos.x, cShotPos.y, cShotPos.z + sEnemyShip.fCollisionA ) );

	GLboolean bWasExplosion = GL_FALSE;
	if( sEnemyShip.iShield >= sShot.iArmament ) {
		sEnemyShip.iShield -= sShot.iArmament;
		sShot.bDraw = GL_FALSE; //pocis "martwy", taze gdy udezy o wrak
	}
	else {
		//pocisk calkowicie niszczy oslone wroga i jeszcze ma punkty
		if( !sEnemyShip.bWreck || sShot.eWeaponType == EWeaponRemoteRocket ) {
			sShot.iArmament -= sEnemyShip.iShield;
			sEnemyShip.iShield = 0;
		}
		if( sEnemyShip.iArmour >= sShot.iArmament || ( sEnemyShip.bWreck && sShot.eWeaponType != EWeaponRemoteRocket ) ) {
			//tu ma wchodzic takze jezeli wrak trafiany jest zwyklymi pociskami - nie zdalna rakieta
			//niezaleznie od iArmour
			if( !sEnemyShip.bWreck )
				sEnemyShip.iArmour -= sShot.iArmament;
			sShot.bDraw = GL_FALSE; //pocis "martwy"
		}
		else {
			//pocisk calkowicie niszczy pancerz wroga i jeszcze ma punkty
			sShot.iArmament -= sEnemyShip.iArmour;
			sEnemyShip.iArmour = 0;
			if( sShot.iArmament > 0 ) {
				//statek zniszczony
				if( --sShot.iArmament == 0 )
					sShot.bDraw = GL_FALSE; //pocisk "martwy"

				GLboolean bWrack = GL_FALSE;
				//jezeli nie asteroida i jezeli nie strzal ze zdalnej rakiety
				if( ( sEnemyShip.bCanFire && sEnemyShip.bCanManoeuvring ) 
					&& !( sEnemyShip.bWreck && sShot.eWeaponType == EWeaponRemoteRocket ) )
				{
					GLint iArmourPointOriginal = m_pGameCtrl->GetShipsManager()->GetArmourPoints( sEnemyShip.i3DObjIndex );
					GLint iArmourPointMax = m_pGameCtrl->GetShipsManager()->GetArmourPoints( sEnemyShip.i3DObjIndex, 3 );
					GLint iPercent = ( ( iArmourPointOriginal * 100 ) / iArmourPointMax ) / 3;
				
					if( rand() % 100 + 1 < iPercent )
						bWrack = GL_TRUE;
				}

				if( !bWrack ) {
					// wrogi obiekt znika z planszy
					sEnemyShip.bAlive = GL_FALSE;
					--m_iNumberActualDrawEnemy;
				}
				else {
					//zamien na wrak, jezeli to juz nie wrak
					sEnemyShip.bWreck = GL_TRUE;
					sEnemyShip.bCanFire = GL_FALSE;
					sEnemyShip.bCanManoeuvring = GL_FALSE;
					sEnemyShip.bCosFly = GL_FALSE;
					//ustaw armour, ktory zadziala tylko przy kolizji gracza z wrakiem
					sEnemyShip.iArmour = 10;
					sEnemyShip.fManoeuvringRot = 0.0f;
				}
				GetREnemyShotAnimCtrl()->FreeAnimCtrlForEnemyShot( sEnemyShip );
				GetPlayer()->IncreaseScore( sEnemyShip.uiPoints );
				GetRStats()->IncreaseShootDownObjects();
				GetRStats()->IncreaseUsedWeapon( sShot.eWeaponType );

				//generuj bonus, ale nie dla obiektow typu asteroida
				//czyli obiekt musi strzelac i moc manewrowac
				if( ( sEnemyShip.bCanFire && sEnemyShip.bCanManoeuvring ) || sEnemyShip.bWreck )
					GetRBonus()->GenerateBonus( sEnemyShip.cActualPos );
				
				//wstawienie animacji wybuchu
				if( sEnemyShip.bBoss ) {
					for( GLfloat fLoop = -20.0f, fZ = 0.0f; fLoop < 20.0f; fLoop += 20.0f, fZ += 1.0f ) {
						CVector3 cPos( sEnemyShip.cActualPos );
						cPos.x += fLoop;
						cPos.z += fZ;
						AddExplosion( cPos, 25.0f );

						if( fLoop == 0.0f )
							continue;
						cPos = sEnemyShip.cActualPos;
						cPos.x += fLoop;
						cPos.y += fLoop;
						cPos.z += fZ;
						AddExplosion( cPos, 25.0f );
					}
				}
				else
					AddExplosion( sEnemyShip.cActualPos, 25.0f );
				bWasExplosion = GL_TRUE;
			}
		}
	}

	if( sShot.eWeaponType == EWeaponHomingRocket || sShot.eWeaponType == EWeaponEmitterRocket ) {
		sShot.bDraw = GL_FALSE; //rakieta zawsze zniszczona po kolizji
		if( !bWasExplosion )
			AddExplosion( sEnemyShip.cActualPos, 25.0f, GL_TRUE, GL_FALSE );
	}

	//jezeli pocisk martwy, to ustaw na martwe takze inne pociski z jego grupy
	if( !sShot.bDraw ) {
		if( sShot.uiGroupIndex > 0 ) //pocisk tworzy grupe
			GetUnivBillboard()->SetNoDrawForShotsGroup( EUBTypePlayerShot, sShot.uiGroupIndex );
	}
	else {
		//jezeli pocisk zyje, to ustaw sShot.iArmament wszytskim pociskom z grupy
		if( sShot.uiGroupIndex > 0 ) //pocisk tworzy grupe
			GetUnivBillboard()->SetArmamentForShotsGroup( EUBTypePlayerShot, sShot.uiGroupIndex, sShot.iArmament );
	}
}

GLboolean CRGame::CheckCollisionPlayerBonus( const CVector3 &in_cPos, EBonusType eBonusType, GLint iIndex )
{
	if( GetPlayer()->GetLives() == -1 || GetPlayer()->GetPlayerExplode() > 0 )
		return GL_FALSE;

	if( in_cPos.z < PLAYER_ZONE_MAX - GetPlayer()->GetCollisionA() )
		return GL_FALSE;

	if( !GetPlayer()->GetCollisionRect().PtInRect( CVector2( in_cPos.x, in_cPos.z ) ) )
		return GL_FALSE;//nie ma kolizji

	//jest kolizja - gracz schwytal znajdzke
	GetPlayer()->IncreaseScore( SCORE_FOR_TAKE_BONUS ); //punkty za schwytanie znajdzki
	if( CWindowData::GetInstance()->SettingFile().bSound )
		COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundTakeBonus );

	switch( eBonusType ) {
		case EBonusWeaponLaser:
			GetRBonus()->UpgradeWeaponLaser();
			break;
		case EBonusWeaponChaingun:
			GetRBonus()->UpgradeWeaponChaingun();
			break;
		case EBonusWeaponImpulseLaser:
			GetRBonus()->UpgradeWeaponImpulseLaser();
			break;
		case EBonusWeaponHomingRockets:
			GetRBonus()->UpgradeWeaponHomingRockets();
			break;
		case EBonusWeaponEmitterRockets:
			GetRBonus()->UpgradeWeaponEmitterRockets();
			break;
		case EBonusLife:
			if( GetPlayer()->GetLives() < 3 )
				GetPlayer()->LivesPlus1();
			break;
		case EBonusShield:
			GetPlayer()->IncreaseShieldActual( ( GetPlayer()->GetShieldMax() / 2 ) );
			if( GetPlayer()->GetShieldActual() > GetPlayer()->GetShieldMax() )
				GetPlayer()->SetShieldActual( GetPlayer()->GetShieldMax() );
			CalculateShieldIndicator();
			break;
		case EBonusViewFinder:
			GetRBonus()->AddNumberViewFinder();
			break;
		case EBonusIndestructibility:
			GetPlayer()->SetImmortality( 50 );// niesmiertelnosc na 5 sec
			break;
		case EBonusSlowdownEnemy:
			GetRBonus()->AddSlowEnemiesSeconds();
			break;
		case EBonusRemoteRocket:
			GetRBonus()->AddRemoteRocketsNumber();
			break;
	}

	GetUnivBillboard()->DeleteUniversalBillboard( iIndex );
	
	return GL_TRUE; //jest kolizja
}

GLvoid CRGame::CheckCollisionPlayerEnemy( SEnemyShip &sEnemy, GLint iIndex )
{
	if( GetPlayer()->GetLives() == -1 || GetPlayer()->GetPlayerExplode() > 0 )
		return;

	if( sEnemy.cActualPos.z + sEnemy.fCollisionA < PLAYER_ZONE_MAX - GetPlayer()->GetCollisionA() )
		return;

	if( GetPlayer()->IsImmortality() )
		return;

	CRectangle cPlayerRect = GetPlayer()->GetCollisionRect();
	CRectangle cEnemyRect = GetEnemyShip()->GetCollisionRect( iIndex );

	GLboolean bIsCollision = cPlayerRect.RectInRect( cEnemyRect );
	if( !bIsCollision )
		bIsCollision = cEnemyRect.RectInRect( cPlayerRect );

	if( !bIsCollision )
		return;

	// jest kolizja
	GetPlayer()->SetPlayerHit();
	GetPlayer()->DoShakePlayer( sEnemy.bSlantFly, sEnemy.bSlantSign );

	GLboolean bEnemyDestroy = GL_FALSE;

	//obliczamy zniszczenia gracza
	if( GetPlayer()->GetShieldActual() >= sEnemy.iShield + sEnemy.iArmour ) {
		GetPlayer()->DecreaseShieldActual( sEnemy.iShield + sEnemy.iArmour );
		bEnemyDestroy = GL_TRUE; //wrog zniszczony
	}
	else {
		GLint iRestShieldArmour = sEnemy.iShield + sEnemy.iArmour - GetPlayer()->GetShieldActual();
		GetPlayer()->SetShieldActual( 0 );
		if( GetPlayer()->GetArmourActual() >= iRestShieldArmour ) {
			GetPlayer()->DecreaseArmourActual( iRestShieldArmour );
			bEnemyDestroy = GL_TRUE; //wrog zniszczony
		}
		else {
			iRestShieldArmour -= GetPlayer()->GetArmourActual();
			GetPlayer()->SetArmourActual( 0 );

			if( iRestShieldArmour > 0 ) {
				//gracz trup, enemy zyje

				//ustawienie parametow wroga
				if( sEnemy.iArmour < iRestShieldArmour )
					sEnemy.iShield -= ( sEnemy.iShield + sEnemy.iArmour - iRestShieldArmour );
				else {
					sEnemy.iShield = 0;
					sEnemy.iArmour = iRestShieldArmour;
				}

				PlayerLoseLive();

				if( GetPlayer()->GetLives() > -1 ) {
					GetPlayer()->SetArmourActual( GetPlayer()->GetArmourMax() );
					GetPlayer()->SetShieldActual( GetPlayer()->GetShieldMax() );
				}
				//wstawienie animacji wybuchu gracza
				AddExplosion( GetPlayer()->GetPosition(), 25.0f, GL_FALSE );
			}
		}
	}

	if( bEnemyDestroy ) {
		sEnemy.bAlive = GL_FALSE;
		--m_iNumberActualDrawEnemy;
		GetREnemyShotAnimCtrl()->FreeAnimCtrlForEnemyShot( sEnemy );
		if( !sEnemy.bWreck ) {
			GetPlayer()->IncreaseScore( sEnemy.uiPoints );
			GetRStats()->IncreaseShootDownObjects();
		}

		if( sEnemy.bCanFire && sEnemy.bCanManoeuvring )
			GetRBonus()->GenerateBonus( sEnemy.cActualPos );

		//wstawienie animacji wybuchu wroga
		AddExplosion( sEnemy.cActualPos, 25.0f );
	}

	CalculateShieldIndicator();
	CalculateArmourIndicator();

	GetPlayer()->SetPlayerHit();
#ifdef DRAW_PLAYER_SHIELD_3D
	GetPlayer()->SetDrawPlayerShield3D( GL_TRUE );
#else
	GetUnivBillboard()->SetDrawPlayerShield2D();
#endif
}

GLvoid CRGame::CalculateShieldIndicator()
{
	GLfloat fPercent = ( static_cast< GLfloat >( GetPlayer()->GetShieldActual() ) * 100.0f ) / static_cast< GLfloat >( GetPlayer()->GetShieldMax() );
	GLfloat fTemp = g_fTopPercentIndicator - g_fBottomPercentIndicator;
	GLfloat fResult = ( ( fTemp * fPercent ) / 100.0f ) + g_fBottomPercentIndicator;
	SetRegionY( m_rRegionsIndicators[ 0 ].top, fResult );
}

GLvoid CRGame::CalculateArmourIndicator()
{
	GLfloat fPercent = ( static_cast< GLfloat >( GetPlayer()->GetArmourActual() ) * 100.0f ) / static_cast< GLfloat >( GetPlayer()->GetArmourMax() );
	GLfloat fTemp = g_fTopPercentIndicator - g_fBottomPercentIndicator;
	GLfloat fResult = ( ( fTemp * fPercent ) / 100.0f ) + g_fBottomPercentIndicator;
	SetRegionY( m_rRegionsIndicators[ 1 ].top, fResult );
}

GLvoid CRGame::CheckEngage()
{
	//funkcja rysuje kwadrat na statku ktory jest namierzony

	if( GetPlayer()->GetLives() == -1 || GetPlayer()->GetPlayerExplode() > 0 )
		return;

	glLoadIdentity();
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	m_pGameCtrl->Disable2D();

	SetCamera();

	GLint iIndexColor = CWindowData::GetInstance()->SettingFile().iHudColor;
	glColor3f( m_aHudColor[ iIndexColor ].x, m_aHudColor[ iIndexColor ].y, m_aHudColor[ iIndexColor ].z ); //zielony

	//maksymalna odleglosc dla jakiej bedzie sprawdzane
	GLfloat fDistance = GetPlayer()->GetEngageDistance();

	GLuint uiCount = GetEnemyShip()->GetEnemyShipSize();
	for( GLfloat f = GetPlayer()->GetPosition().z; f > -fDistance; f -= 20.0f ) {
		for( GLuint i = 0; i < uiCount; ++i ) {
			if( !GetEnemyShip()->GetEnemyShip( i ).bAlive || !GetEnemyShip()->GetEnemyShip( i ).bStart )
				continue;

			GLboolean bIsCollision = GL_FALSE;

			//sprawdzamy kolizje z celownikiem
			if( GetEnemyShip()->GetEnemyShip( i ).cActualPos.z >= GetPlayer()->GetPosition().z - fDistance ) {
				if( GetEnemyShip()->GetEnemyShip( i ).cActualPos.z + GetEnemyShip()->GetEnemyShip( i ).fCollisionA > f
					&& GetEnemyShip()->GetEnemyShip( i ).cActualPos.z - GetEnemyShip()->GetEnemyShip( i ).fCollisionA < f )
				{
					if( GetEnemyShip()->GetEnemyShip( i ).cActualPos.x + GetEnemyShip()->GetEnemyShip( i ).fCollisionB > GetPlayer()->GetPosition().x
						&& GetEnemyShip()->GetEnemyShip( i ).cActualPos.x - GetEnemyShip()->GetEnemyShip( i ).fCollisionB < GetPlayer()->GetPosition().x )
					{
						//jest kolizja z celownikiem
						bIsCollision = GL_TRUE;
					}
				}
			}

			//sprawdz kolizje ze zdaln¹ rakieta, czy statek jest w jej zasiegu razenia
			if( GetRBonus()->GetNumberViewFinder() >= 1 && !bIsCollision && GetRRemoteRocket()->IsDraw() )
				bIsCollision = GetRRemoteRocket()->IsCollisionEnemyRemoteRocket( GetRRemoteRocket()->GetPosition(), GetEnemyShip()->GetEnemyShip( i ).cActualPos );

			if( bIsCollision )
				DrawEngageSquare( GetEnemyShip()->GetEnemyShip( i ).cActualPos );
		}
	}

	//sprawdz czy gracz jest w zasiegu zdalnej rakiety
	//jezeli jest to zaznacz jego statek czerwonym kwadracikiem
	if( GetRRemoteRocket()->IsDraw() ) {
		//sprawdz kolizje ze zdaln¹ rakieta, czy statek jest w jej zasiegu razenia
		if( GetRRemoteRocket()->IsCollisionEnemyRemoteRocket( GetRRemoteRocket()->GetPosition(), GetPlayer()->GetPosition() ) ) {
			glColor3f( m_aHudColor[ iIndexColor ].x, m_aHudColor[ iIndexColor ].y, m_aHudColor[ iIndexColor ].z ); //czerwony
			DrawEngageSquare( GetPlayer()->GetPosition(), 10.0f );
		}
	}
}

GLvoid CRGame::DrawEngageSquare( const CVector3 &in_cPos, GLfloat fSquareScale /*= 8.0f*/ )
{
	glPushMatrix();
	glTranslatef( in_cPos.x, in_cPos.y, in_cPos.z );
	m_pGameCtrl->GetBillboard()->DrawQuadBillboard( CVector3(), fSquareScale, GL_LINE_LOOP );
	glPopMatrix();

	if( CWindowData::GetInstance()->SettingFile().bSound ) {
		if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 100, 11 ) )
			COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundEngage );

	}
}

GLvoid CRGame::AddExplosion( const CVector3 &in_cPos, GLfloat fScale, GLboolean bForEnemy /*= GL_TRUE*/, GLboolean bMultikill /*= GL_TRUE*/ )
{	
	//znajdz pierwsza wolna eksplozje
	GLint iIndex = GetUnivBillboard()->AddExplosion( in_cPos, fScale );

	if( bForEnemy ) {
		if( bMultikill ) {
			//zliczamy multikilla
			m_pGameCtrl->GetSpeedCtrl()->UpdateAnimationTime( 12 );
			++m_iMultiKillCount;
		}
	}
	else
		GetPlayer()->SetPlayerExplode( GetUnivBillboard()->GetAnimCtrlIndexFromExplosion( iIndex ) );

	++m_iExplosionDrawCounter; //dla swiatla eksplozji

	AddExplosionSound( in_cPos );
}

GLvoid CRGame::AddExplosionSound( const CVector3 &in_cPos )
{
	if( !CWindowData::GetInstance()->SettingFile().bSound )
		return;

	COpenALManager::GetInstance()->Move( m_pGameCtrl->m_aSoundExplosion[ m_iActualExplodeSound ], in_cPos / DIVISION_EXPLO_POSITION_FOR_SOUND );
	COpenALManager::GetInstance()->Play( m_pGameCtrl->m_aSoundExplosion[ m_iActualExplodeSound ] );
	if( ++m_iActualExplodeSound >= m_pGameCtrl->GetSoundExplosionSize() )
		m_iActualExplodeSound = 0;
}

GLboolean CRGame::DrawExplosion( SUniversalBillboard &in_sUB )
{
	if( in_sUB.sExpl.iActualAnimFrame == -1 ) {
		//znajdz animacje ktora odpowiada za wybuch gracza
		if( in_sUB.sExpl.iAnimCtrlIndex == GetPlayer()->GetPlayerExplode() )
			GetPlayer()->SetPlayerExplode( 0 ); //gracz nie eksploduje
		return GL_FALSE;
	}

	glPushMatrix();
	glTranslatef( in_sUB.cPos.x, in_sUB.cPos.y, in_sUB.cPos.z );
	m_pGameCtrl->GetLighting()->SetPosition( GL_LIGHT3, in_sUB.cPos.x, in_sUB.cPos.y + 5.0f, in_sUB.cPos.z );

	m_pGameCtrl->GetTexLoader()->SetTexture( in_sUB.sExpl.iTexture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	//w iTempFrame dostejemy klatki od 0 do 3 czyli rzedy
	GLint iTempFrameX = in_sUB.sExpl.iActualAnimFrame % 4;
	GLfloat fX1 = 0.25f * static_cast< GLfloat >( iTempFrameX ); //x mniejszy
	GLfloat fX2 = 0.25f * static_cast< GLfloat >( iTempFrameX + 1 ); //x wiekszy

	GLfloat fTempFrameY = static_cast< GLfloat >( in_sUB.sExpl.iActualAnimFrame ) / 4.0f;
	fTempFrameY = floor( fTempFrameY );
	fTempFrameY = 3.0f - fTempFrameY; //odwaracamy

	GLfloat fY1 = 0.25f * fTempFrameY; //y mniejszy
	GLfloat fY2 = 0.25f * ( fTempFrameY + 1.0f ); //y wiekszy

	m_pGameCtrl->GetBillboard()->DrawQuadBillboard( CVector3(), in_sUB.sExpl.fScale, GL_TRIANGLE_STRIP, fX1, fX2, fY1, fY2 );

	if( in_sUB.sExpl.iAnimCtrlIndex == 0 )
		in_sUB.sExpl.iActualAnimFrame = -1; //nie rysuj, bledny index animacji
	else {
		if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 50, in_sUB.sExpl.iAnimCtrlIndex ) ) {
			++in_sUB.sExpl.iActualAnimFrame;
			if( in_sUB.sExpl.iActualAnimFrame == 16 )
				in_sUB.sExpl.iActualAnimFrame = -1; //nie rysuj
		}
	}
	if( in_sUB.sExpl.iActualAnimFrame == -1 ) {
		if( --m_iExplosionDrawCounter < 0 ) //zmniejsz licznik dla swiatla eksplozji
			m_iExplosionDrawCounter = 0;
	}

	glPopMatrix();

	return GL_TRUE;
}

#ifndef DRAW_PLAYER_SHIELD_3D
GLboolean CRGame::DrawPlayerShield2D( SUniversalBillboard &in_sUB )
{
	if( in_sUB.sPlayerShield.iActualAnimFrame == -1 )
		return GL_FALSE;

	glPushMatrix();
	in_sUB.cPos = GetPlayer()->GetPosition();
	in_sUB.cPos.z += 26.0; //przesun oslone na tyl statku bo widac ze jest wewnatrz statku
	switch( GetCameraMode() ) {
		case ECameraModeNormal:
			in_sUB.cPos.y += 15.0; //po przesuniecie na tyl statku, oslona jest za nisko, wiec ja podnies
			//z powodu perspektywy, oslona 2d wydaje sie odchylona na bok, wiec niwelujemy to:
			if( in_sUB.cPos.x > 0.0f )
				in_sUB.cPos.x -= ( in_sUB.cPos.x / 14.0f );
			else
				in_sUB.cPos.x += ( ( -in_sUB.cPos.x ) / 14.0f );
			break;
		case ECameraModeTail:
			in_sUB.cPos.y += 5.0; //po przesuniecie na tyl statku, oslona jest za nisko, wiec ja podnies
			break;
	}
	glTranslatef( in_sUB.cPos.x, in_sUB.cPos.y, in_sUB.cPos.z );

	m_pGameCtrl->GetTexLoader()->SetTexture( in_sUB.sPlayerShield.iTexture );

	//w iTempFrame dostejemy klatki od 0 do 3 czyli rzedy
	GLint iTempFrameX = in_sUB.sPlayerShield.iActualAnimFrame % 4;
	GLfloat fX1 = 0.25f * static_cast< GLfloat >( iTempFrameX ); //x mniejszy
	GLfloat fX2 = 0.25f * static_cast< GLfloat >( iTempFrameX + 1 ); //x wiekszy

	GLfloat fTempFrameY = static_cast< GLfloat >( in_sUB.sPlayerShield.iActualAnimFrame ) / 4.0f;
	fTempFrameY = floor( fTempFrameY );
	fTempFrameY = 3.0f - fTempFrameY; //odwaracamy

	GLfloat fY1 = 0.25f * fTempFrameY; //y mniejszy
	GLfloat fY2 = 0.25f * ( fTempFrameY + 1.0f ); //y wiekszy

	m_pGameCtrl->GetBillboard()->DrawQuadBillboard( CVector3(), in_sUB.sPlayerShield.fScale, GL_TRIANGLE_STRIP, fX1, fX2, fY1, fY2 );

	if( in_sUB.sPlayerShield.iAnimCtrlIndex == 0 )
		in_sUB.sPlayerShield.iActualAnimFrame = -1; //nie rysuj, bledny index animacji
	else {
		if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 50, in_sUB.sPlayerShield.iAnimCtrlIndex ) ) {
			++in_sUB.sPlayerShield.iActualAnimFrame;
			if( in_sUB.sPlayerShield.iActualAnimFrame == 12 )
				in_sUB.sPlayerShield.iActualAnimFrame = -1; //nie rysuj
		}
	}

	glPopMatrix();

	return GL_TRUE;
}
#endif // DRAW_PLAYER_SHIELD_3D

GLvoid CRGame::ConfirmHighscore()
{
	//potwierdzenie wyniku
	if( _tcslen( m_cUserName ) < 3 ) {
		ShowCursor();
		m_pGameCtrl->GetRMessageBox()->DoMessageBox( LNG_LINE( 58 ), CRMessageBox::EMsgOK, MsgExitGameNo, NULL );
		return;
	}
	::SecureZeroMemory( g_sHighscoreItem[ 9 ].cNickname, sizeof( g_sHighscoreItem[ 9 ].cNickname ) );
	::wsprintf( g_sHighscoreItem[ 9 ].cNickname, m_cUserName );
	g_sHighscoreItem[ 9 ].uiScore = GetPlayer()->GetScore();
	g_sHighscoreItem[ 9 ].iLevel = m_iLevelToDisplay;
	m_pGameCtrl->GetRHighscore()->SortItems();

	CWindowData::GetInstance()->SaveSettingFile();

	m_pGameCtrl->GetCursor()->SetMyCursor( CCursor::EMenu );

	//wyjscie z gry i przelaczenie sie na plansze z highscore
	SetEndPlayerVariables( EHighscore );
}

GLvoid CRGame::PlayerLoseLive()
{
	if( GetPlayer()->GetLives() > -1 ) {
		GetPlayer()->PlayerLoseLive();

		//gracz traci o jeden poziom usprawnienia z bonusow
		GetRBonus()->ReduceWeaponLaser();
		GetRBonus()->ReduceWeaponChaingun();
		GetRBonus()->ReduceWeaponImpulseLaser();
		GetRBonus()->ReduceWeaponEmitterRockets();
		GetRBonus()->ReduceWeaponHomingRockets();
		GetRBonus()->ReduceNumberViewFinder();
		GetRBonus()->ReduceSlowEnemiesSeconds();
	}
}

GLvoid CRGame::ShowCursor()
{
	POINT ptCursorPos;
	ptCursorPos.x = CWindowData::GetInstance()->SettingFile().iWidth / 2;
	ptCursorPos.y = CWindowData::GetInstance()->SettingFile().iHeight / 2;

	if( !CWindowData::GetInstance()->SettingFile().bFullScreen )
		::ClientToScreen( CWindowData::GetInstance()->GetHWND(), &ptCursorPos );

	::SetCursorPos( ptCursorPos.x, ptCursorPos.y );
	m_pGameCtrl->GetCursor()->SetMyCursor( CCursor::EMenu );
}

GLvoid CRGame::StartMultiplayerGame()
{
	m_bBlockMultiplayerGame = GL_TRUE;
}

GLvoid CRGame::PlayNextMusic()
{
	if( !CWindowData::GetInstance()->SettingFile().bMusic )
		return;

#if _USE_SOUND_OPENAL_ && _USE_OGG_
	// zmiana muzyki co 2 poziomy
	if( ( m_iLevelToDisplay - 1 ) % 2 == 0 ) {
		COpenALManager::GetInstance()->Stop( m_pGameCtrl->m_aMusicGame[ m_pGameCtrl->m_iActualMusicGame ] );
		if( ++m_pGameCtrl->m_iActualMusicGame >= m_pGameCtrl->GetMusicGameSize() )
			m_pGameCtrl->m_iActualMusicGame = 0;

		COpenALManager::GetInstance()->Play( m_pGameCtrl->m_aMusicGame[ m_pGameCtrl->m_iActualMusicGame ] );
	}
#endif
}

GLvoid CRGame::StopPlayMusicAndRenewAfterDelay()
{
	if( !CWindowData::GetInstance()->SettingFile().bMusic )
		return;

#if _USE_SOUND_OPENAL_ && _USE_OGG_
	COpenALManager::GetInstance()->Stop( m_pGameCtrl->m_aMusicGame[ m_pGameCtrl->m_iActualMusicGame ] );
	m_pGameCtrl->m_iRenewMusicAfterDelay = 4;
#endif
}