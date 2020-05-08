// GameControl.cpp: implementation of the CGameControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Console.h"
#include "../Fonts/BitmapFont.h"
#include "../Fonts/OutlineFont.h"
#include "../Fonts/TextureFont.h"
#include "../Fonts/SDLFont.h"
#include "../Fps/Fps.h"
#include "../Sound/MasterVolume.h"
#include "WindowData.h"
#include "Cursor.h"
#include "../Extensions/ARBMultiTexturing.h"
#include "../Extensions/WGLEXTSwapControl.h"
#include "../Extensions/ARBVertexBufferObject.h"
#include "../Draw/VertexArrays.h"
#include "../Draw/3DObjManager.h"
#include "../Draw/EmbossBump.h"
#include "../Network/NetworkManager.h"
#include "LoadProgressBar.h"
#include "GameControl.h"

//#include "FakeHDR.h"

#include "RLogo.h"
#include "RMainMenu.h"
#include "RDifficultyLevel.h"
#include "RShipsBrowse.h"
#include "RCredits.h"
#include "RGame.h"
#include "ShipsManager.h"
#include "LevelsManager.h"
#include "RHighscore.h"
#include "ROptions.h"
#include "ROptionsGraphics.h"
#include "ROptionsSound.h"
#include "ROptionsControl.h"
#if _USE_MULTIPLYER_
#include "RMultiplayerMenu.h"
#endif
#include "Joystick.h"

#if _USE_SOUND_OPENAL_
	#pragma comment( lib, "alut.lib" )
	#pragma comment( lib, "OpenAL32.lib" )
	#if _USE_OGG_
		#pragma comment( lib, "ogg.lib" )
		#pragma comment( lib, "vorbisfile.lib" )
	#endif // _USE_OGG_
#else // use FMOD
	#pragma comment( lib, "fmodvc.lib" )
#endif // !_USE_SOUND_OPENAL_

GLvoid MsgExit( GLvoid );
#if _USE_LOAD_PROGESS_BAR_
GLboolean FinishInitialization( GLvoid );
#endif

CGameControl* CGameControl::Construct()
{
	return CSingletonBase::Construct( new CGameControl );
}

CGameControl::CGameControl()
{
	m_pWin = CWindowData::GetInstance();

	m_pLoadProgressBar = NULL;
#if _USE_LOAD_PROGESS_BAR_
	m_pLoadProgressBar = CLoadProgressBar::Construct();
#endif

	::SecureZeroMemory( m_bKeys, 256 );
	
	//przy restarcie ponizszych zmieninnych nie mozna zmieniac
	m_bIs2D = GL_FALSE;
#ifdef _USE_QUADRIC_
	m_pQuadric = NULL;
#endif //_USE_QUADRIC_

	m_eMainGameMode = ELogo;
	m_fRotBackgroundY = static_cast< GLfloat >( rand() % 360 );

	//zmienne dla intra
	m_iFrameCounter = 0;
	m_bLineReady = GL_FALSE;
	m_bFillReady = GL_FALSE;
	m_bTextureReady = GL_FALSE;
	m_bVkControl = GL_FALSE;
	
	m_fBkgMove = 0.0f;

	m_pConsole = NULL;
	m_pRLogo = new CRLogo();
	m_pMatrixOp = NULL;
	m_pFps = NULL;
	m_pSpeedControl = NULL;
	m_pBitmapFont = NULL;
	m_pOutlineFont = NULL;
	m_pSDLFont = NULL;
	m_pARBMultiTexturing = NULL;
	m_pWglExtSwapCtrl = NULL;
	m_pTextureLoader = NULL;
	m_pTextureFont = NULL;
#if _USE_SOUND_OPENAL_
	m_pOpenALManager = NULL;
#else
	m_pSoundFMOD = NULL;
#endif
	m_pMasterVolume = NULL;
	m_pLighting = NULL;
	m_pCursor = NULL;
	m_pEmbossBump = NULL;
	m_pVertexArrays = NULL;
	m_pShipsManager = NULL;
	m_pLevelsManager = NULL;
	m_pStencilShadow = NULL;
	m_p3DObjManager = NULL;
	m_pParticles = NULL;
	m_pBillboard = NULL;
	m_pNetworkManager = NULL;

	m_pRMessageBox = NULL;
	m_pRMainMenu = NULL;
	m_pRDifficultyLevel = NULL;
	m_pRShipsBrowse = NULL;
	m_pCredits = NULL;
	m_pRHighscore = NULL;
	m_pRGame = NULL;
	m_pROptions = NULL;
	m_pROptionsGraphics = NULL;
	m_pROptionsSound = NULL;
	m_pROptionsControl = NULL;
#if _USE_MULTIPLYER_
	m_pRMultiplayerMenu = NULL;
#endif
	m_pJoystick = NULL;

	m_iActualMusicMenu = 0;
	m_iActualMusicGame = 0;
	m_iRenewMusicAfterDelay = -1;

	m_aIndicesQuadAllScreen = new GLint[ 8 ];
	m_aSkyBoxTexCoord = new CVector2[ 24 ];
	m_aSkyBoxIndices = new CVector3[ 24 ];

	GLfloat fSize = MAX_GAME_DISTANCE * 1.5f;

	// Przednia Œciana
	m_aSkyBoxTexCoord[  0 ] = CVector2( 0.0f, 0.0f ); m_aSkyBoxIndices[  0 ] = CVector3( -fSize, -fSize, fSize );
	m_aSkyBoxTexCoord[  1 ] = CVector2( 1.0f, 0.0f ); m_aSkyBoxIndices[  1 ] = CVector3(  fSize, -fSize, fSize );
	m_aSkyBoxTexCoord[  2 ] = CVector2( 1.0f, 1.0f ); m_aSkyBoxIndices[  2 ] = CVector3(  fSize,  fSize, fSize );
	m_aSkyBoxTexCoord[  3 ] = CVector2( 0.0f, 1.0f ); m_aSkyBoxIndices[  3 ] = CVector3( -fSize,  fSize, fSize );
	// Tylna œciana
	m_aSkyBoxTexCoord[  4 ] = CVector2( 1.0f, 0.0f ); m_aSkyBoxIndices[  4 ] = CVector3( -fSize, -fSize, -fSize );
	m_aSkyBoxTexCoord[  5 ] = CVector2( 1.0f, 1.0f ); m_aSkyBoxIndices[  5 ] = CVector3( -fSize,  fSize, -fSize );
	m_aSkyBoxTexCoord[  6 ] = CVector2( 0.0f, 1.0f ); m_aSkyBoxIndices[  6 ] = CVector3(  fSize,  fSize, -fSize );
	m_aSkyBoxTexCoord[  7 ] = CVector2( 0.0f, 0.0f ); m_aSkyBoxIndices[  7 ] = CVector3(  fSize, -fSize, -fSize );
	// Górna œciana
	m_aSkyBoxTexCoord[  8 ] = CVector2( 0.0f, 1.0f ); m_aSkyBoxIndices[  8 ] = CVector3( -fSize,  fSize, -fSize );
	m_aSkyBoxTexCoord[  9 ] = CVector2( 0.0f, 0.0f ); m_aSkyBoxIndices[  9 ] = CVector3( -fSize,  fSize,  fSize );
	m_aSkyBoxTexCoord[ 10 ] = CVector2( 1.0f, 0.0f ); m_aSkyBoxIndices[ 10 ] = CVector3(  fSize,  fSize,  fSize );
	m_aSkyBoxTexCoord[ 11 ] = CVector2( 1.0f, 1.0f ); m_aSkyBoxIndices[ 11 ] = CVector3(  fSize,  fSize, -fSize );
	// Dolna œciana
	m_aSkyBoxTexCoord[ 12 ] = CVector2( 1.0f, 1.0f ); m_aSkyBoxIndices[ 12 ] = CVector3( -fSize, -fSize, -fSize );
	m_aSkyBoxTexCoord[ 13 ] = CVector2( 0.0f, 1.0f ); m_aSkyBoxIndices[ 13 ] = CVector3(  fSize, -fSize, -fSize );
	m_aSkyBoxTexCoord[ 14 ] = CVector2( 0.0f, 0.0f ); m_aSkyBoxIndices[ 14 ] = CVector3(  fSize, -fSize,  fSize );
	m_aSkyBoxTexCoord[ 15 ] = CVector2( 1.0f, 0.0f ); m_aSkyBoxIndices[ 15 ] = CVector3( -fSize, -fSize,  fSize );
	// Prawa œciana
	m_aSkyBoxTexCoord[ 16 ] = CVector2( 1.0f, 0.0f ); m_aSkyBoxIndices[ 16 ] = CVector3(  fSize, -fSize, -fSize );
	m_aSkyBoxTexCoord[ 17 ] = CVector2( 1.0f, 1.0f ); m_aSkyBoxIndices[ 17 ] = CVector3(  fSize,  fSize, -fSize );
	m_aSkyBoxTexCoord[ 18 ] = CVector2( 0.0f, 1.0f ); m_aSkyBoxIndices[ 18 ] = CVector3(  fSize,  fSize,  fSize );
	m_aSkyBoxTexCoord[ 19 ] = CVector2( 0.0f, 0.0f ); m_aSkyBoxIndices[ 19 ] = CVector3(  fSize, -fSize,  fSize );
	// Lewa œciana
	m_aSkyBoxTexCoord[ 20 ] = CVector2( 0.0f, 0.0f ); m_aSkyBoxIndices[ 20 ] = CVector3( -fSize, -fSize, -fSize );
	m_aSkyBoxTexCoord[ 21 ] = CVector2( 1.0f, 0.0f ); m_aSkyBoxIndices[ 21 ] = CVector3( -fSize, -fSize,  fSize );
	m_aSkyBoxTexCoord[ 22 ] = CVector2( 1.0f, 1.0f ); m_aSkyBoxIndices[ 22 ] = CVector3( -fSize,  fSize,  fSize );
	m_aSkyBoxTexCoord[ 23 ] = CVector2( 0.0f, 1.0f ); m_aSkyBoxIndices[ 23 ] = CVector3( -fSize,  fSize, -fSize );
}

CGameControl::~CGameControl()
{
	delete [] m_aIndicesQuadAllScreen;
	delete [] m_aSkyBoxTexCoord;
	delete [] m_aSkyBoxIndices;
	DeleteObjects();
	m_aTexBackground.clear();
	m_aSoundExplosion.clear();
	m_aTexWeaponSelect.clear();
	m_aSoundWeaponSelect.clear();
}

GLboolean CGameControl::CreateObjects()
{
	SetIndicesQuadAllScreen();

	m_pMatrixOp = new CMatrixOperations();
	if( !m_pMatrixOp ) return GL_FALSE;

	m_pConsole = new CConsole();
	if( !m_pConsole ) return GL_FALSE;

	m_pFps = new CFps();
	if( !m_pFps ) return GL_FALSE;

	m_pSpeedControl = new CSpeedControl();
	if( !m_pSpeedControl ) return GL_FALSE;

	m_pSDLFont = new CSDLFont();
	if( !m_pSDLFont ) return GL_FALSE;

	m_pARBMultiTexturing = CARBMultiTexturing::Construct();
	if( !m_pARBMultiTexturing ) return GL_FALSE;

	m_pWglExtSwapCtrl = CWGLEXTSwapControl::Construct();
	if( !m_pWglExtSwapCtrl ) return GL_FALSE;

	m_pTextureLoader = new CTextureLoader();
	if( !m_pTextureLoader ) return GL_FALSE;

#if _USE_SOUND_OPENAL_
	m_pOpenALManager = COpenALManager::Construct();
	if( !m_pOpenALManager ) return GL_FALSE;
#else
	m_pSoundFMOD = new CSoundFMOD();
	if( !m_pSoundFMOD ) return GL_FALSE;
#endif

	m_pLighting = new CLighting();
	if( !m_pLighting ) return GL_FALSE;

	m_pCursor = new CCursor();
	if( !m_pCursor ) return GL_FALSE;

	m_pEmbossBump = new CEmbossBump();
	if( !m_pEmbossBump ) return GL_FALSE;

	m_pVertexArrays = CVertexArrays::Construct();
	if( !m_pVertexArrays ) return GL_FALSE;

	m_pShipsManager = new CShipsManager();
	if( !m_pShipsManager ) return GL_FALSE;

	m_pLevelsManager = new CLevelsManager();
	if( !m_pLevelsManager ) return GL_FALSE;

	m_pStencilShadow = new CStencilShadow();
	if( !m_pStencilShadow ) return GL_FALSE;

	m_p3DObjManager = new C3DObjManager();
	if( !m_p3DObjManager ) return GL_FALSE;

	m_pParticles = new CParticles();
	if( !m_pParticles ) return GL_FALSE;

	m_pBillboard = new CBillboard();
	if( !m_pBillboard ) return GL_FALSE;

	m_pNetworkManager = new CNetworkManager();
	if( !m_pNetworkManager ) return GL_FALSE;
	
	m_pRMessageBox = new CRMessageBox();
	if( !m_pRMessageBox ) return GL_FALSE;

	//m_pFakeHDR = new CFakeHDR();
	
	m_pRMainMenu = new CRMainMenu();
	m_pRDifficultyLevel = new CRDifficultyLevel();
	m_pRShipsBrowse = new CRShipsBrowse();
	m_pCredits = new CRCredits();
	m_pRHighscore = new CRHighscore();
	m_pRGame = new CRGame();
	m_pROptions = new CROptions();
	m_pROptionsGraphics = new CROptionsGraphics();
	m_pROptionsSound = new CROptionsSound();
	m_pROptionsControl = new CROptionsControl();
#if _USE_MULTIPLYER_
	m_pRMultiplayerMenu = new CRMultiplayerMenu();
#endif
	m_pJoystick = new CJoystick();
	GetJoystick()->Initialize();

	CreateSkyBoxList();

	m_iIndexShipToBackground = GetShipsManager()->GetRandomEnemyShipIndex();
	
	return GL_TRUE;
}

GLvoid CGameControl::DeleteObjects()
{
	m_aArrayLine.clear();
	m_aArrayFill.clear();
	m_aArrayTexture.clear();

#ifdef _USE_QUADRIC_
	if( m_pQuadric ) {
		gluDeleteQuadric( m_pQuadric );
		m_pQuadric = NULL;
	}
#endif //_USE_QUADRIC_

	DeleteSkyBoxList();

	delete m_pSpeedControl;
	delete m_pTextureLoader;
	delete m_pConsole;
	if( m_pFps ) {
		delete m_pFps;
		m_pFps = NULL;
	}
#ifndef _USE_SOUND_OPENAL_
	delete m_pSoundFMOD;
#endif
	delete m_pSDLFont;
	delete m_pLighting;
	delete m_pCursor;
	delete m_pVertexArrays;
	delete m_pMatrixOp;
	delete m_p3DObjManager;
	delete m_pEmbossBump;
	delete m_pParticles;
	delete m_pBillboard;
	delete m_pStencilShadow;
	delete m_pNetworkManager;
	delete m_pRMessageBox;

	//delete m_pFakeHDR;

	delete m_pRLogo;
	delete m_pRMainMenu;
	delete m_pRDifficultyLevel;
	delete m_pRShipsBrowse;
	delete m_pCredits;
	delete m_pRGame;
	delete m_pShipsManager;
	delete m_pLevelsManager;
	delete m_pRHighscore;
	delete m_pROptions;
	delete m_pROptionsGraphics;
	delete m_pROptionsSound;
	delete m_pROptionsControl;
#if _USE_MULTIPLYER_
	delete m_pRMultiplayerMenu;
#endif
	delete m_pLoadProgressBar;
	delete m_pJoystick;
}

#if _USE_LOAD_PROGESS_BAR_
GLboolean CGameControl::Initialization()
{
	if( !CreateObjects() ) {
		__LOG( _T("ERROR: CGameControl::CreateObjects - create objects failed") );
		return GL_FALSE;
	}

	SetLights();

	CreateAllAnimCtrl();

	if( !CMultiLanguage::GetInstance()->LoadLanguage( CWindowData::GetInstance()->SettingFile().iIndexLanguage ) ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadLanguage() failed") );
		return GL_FALSE;
	}

	// Dodaje zasoby to LoadProgressBar:

	GetLoadProgressBar()->Clear();

	if( !LoadAllTextures() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadAllTextures() failed") );
		return GL_FALSE;
	}

	if( !LoadAll3DObjFiles() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadAll3DObjFiles() failed") );
		return GL_FALSE;
	}

	if( !LoadAllSounds() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadAllSounds() failed") );
		return GL_FALSE;
	}

	GetLoadProgressBar()->DoLoadProgressRes( FinishInitialization );

	return GL_TRUE;
}

GLboolean FinishInitialization( GLvoid )
{
	CGameControl::GetInstance()->Get3DObjManager()->Initialization();

	if( !CGameControl::GetInstance()->CreateAllParticles() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - CreateAllParticles() failed") );
		return GL_FALSE;
	}
	
	CGameControl::GetInstance()->SetVSync();
	CGameControl::GetInstance()->CreateVertices();

#ifdef _USE_QUADRIC_
	CGameControl::GetInstance()->CreateQuadric();
#endif

	CGameControl::GetInstance()->GetRGame()->Initialization();

	//CGameControl::GetInstance()->GetFakeHDR()->init_postprocess();

	return GL_TRUE;
}
#else // _USE_LOAD_PROGESS_BAR_
GLboolean CGameControl::Initialization()
{	
	if( !CreateObjects() ) {
		__LOG( _T("ERROR: CGameControl::CreateObjects - create objects failed") );
		return GL_FALSE;
	}

	SetLights();

	CreateAllAnimCtrl();
	
	if( !LoadAllTextures() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadAllTextures() failed") );
		return GL_FALSE;
	}

	if( !CreateAllParticles() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - CreateAllParticles() failed") );
		return GL_FALSE;
	}

	if( !LoadAll3DObjFiles() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadAll3DObjFiles() failed") );
		return GL_FALSE;
	}

	if( !LoadAllSounds() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadAllSounds() failed") );
		return GL_FALSE;
	}

	if( !CMultiLanguage::GetInstance()->LoadLanguage( GetWinData()->SettingFile().iIndexLanguage ) ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadLanguage() failed") );
		return GL_FALSE;
	}
	
	SetVSync();
	CreateVertices();

#ifdef _USE_QUADRIC_
	CreateQuadric();
#endif

	GetRGame()->Initialization();

	//GetFakeHDR()->init_postprocess();

	return GL_TRUE;
}
#endif //_USE_LOAD_PROGESS_BAR_

GLvoid CGameControl::CreateVertices()
{
	GetVertexArrays()->InitialArrays();
}

GLvoid CGameControl::SetVSync()
{
	if( GetWinData()->SettingFile().bVSync )
		GetWGLEXTSwapControl()->EnableVSync();
	else
		GetWGLEXTSwapControl()->DisableVSync();
}

#ifdef _USE_QUADRIC_
GLvoid CGameControl::CreateQuadric()
{
	if( m_pQuadric ) {
		gluDeleteQuadric( m_pQuadric );
		m_pQuadric = NULL;
	}
	//*************** dla kwadrykow *********************
    m_pQuadric = gluNewQuadric(); 
	//
	gluQuadricNormals( m_pQuadric, GLU_NONE ); //bez normalnych
	//gluQuadricNormals( m_pQuadric, GLU_FLAT ); //naormalne na p³aszczyznach
    //gluQuadricNormals( m_pQuadric, GLU_SMOOTH ); //normalne na wierzcholkach
	//
	//gluQuadricDrawStyle( m_pQuadric, GLU_LINE );
	//gluQuadricDrawStyle( m_pQuadric, GLU_POINT );
	//gluQuadricDrawStyle( m_pQuadric, GLU_SILHOUETTE );
	gluQuadricDrawStyle( m_pQuadric, GLU_FILL ); //domyslnie
	//
	//gluQuadricOrientation( m_pQuadric, GLU_INSIDE ); //normalne do wewnatrz kwadryki, np. gdy kamera wernatrz kwadryki
	gluQuadricOrientation( m_pQuadric, GLU_OUTSIDE ); //domyslnie - normalna na zewnatrz kwadryki
    //
	//gluQuadricTexture( m_pQuadric, GL_TRUE );
	gluQuadricTexture( m_pQuadric, GL_FALSE );
	//***************************************************
}
#endif //_USE_QUADRIC_

GLvoid CGameControl::SetLights()
{
	//oswietlenie globalne
	GetLighting()->SetAmbient( GL_LIGHT0, 0.8f, 0.8f, 0.8f );		//ustaw oswietlenie otoczenia
	GetLighting()->SetDiffuse( GL_LIGHT0 );						//ustaw oswietlenie rozpraszajace
	GetLighting()->SetSpecular( GL_LIGHT0 );						//Ustaw swiatlo odb³ysków
	GetLighting()->SetPosition( GL_LIGHT0, 1.0f, 0.0f, -3.0f );	//ustaw pozycje swiatla
	GetLighting()->SetMatSpecular();
	GetLighting()->SetMatShinness();
	glEnable( GL_LIGHT0 );

	// swiatlo dla rysowania bumpa
	SetLight1();

	// swiatlo dla oswietlanie statkow w czasie gry i menu
	SetLight2();

	//swiatlo eksplozji
	GetLighting()->SetAmbient( GL_LIGHT3, 0.0f, 0.0f, 0.0f );			//ustaw oswietlenie otoczenia
	GetLighting()->SetDiffuse( GL_LIGHT3, 1.0f, 1.0f, 0.0f );			//ustaw oswietlenie rozpraszajace
	GetLighting()->SetSpecular( GL_LIGHT3 );							//Ustaw swiatlo odb³ysków
	//GetLighting()->SetPosition( GL_LIGHT3, 0.0f, 3000.0f, 0.0f );	//ustaw pozycje swiatla
	glDisable( GL_LIGHT3 );

	glEnable( GL_LIGHTING );
}

GLvoid CGameControl::SetLight1()
{
	// oswitlenie dla bumpa - do generowania list z obiektami 3ds
	GetLighting()->SetAmbient( GL_LIGHT1, 0.8f, 0.8f, 0.8f );		//ustaw oswietlenie otoczenia
	GetLighting()->SetDiffuse( GL_LIGHT1 );						//ustaw oswietlenie rozpraszajace
	GetLighting()->SetSpecular( GL_LIGHT1 );						//Ustaw swiatlo odb³ysków
	//robimy oszustwo, bo z aktualnym ustawieniem, bump na statkach lepiej wygl¹da
	GetLighting()->SetPosition( GL_LIGHT1, CVector4( 50.0f, 50.0f, 50.0f, 1.0f ) );	//ustaw pozycje swiatla
	glDisable( GL_LIGHT1 );
}

GLvoid CGameControl::SetLight2()
{
	//oswietlenie dla statku - umieszczane dokladnie nad kazdym statkiem
	GetLighting()->SetAmbient( GL_LIGHT2, 0.8f, 0.8f, 0.8f );		//ustaw oswietlenie otoczenia
	GetLighting()->SetDiffuse( GL_LIGHT2 );						//ustaw oswietlenie rozpraszajace
	GetLighting()->SetSpecular( GL_LIGHT2 );						//Ustaw swiatlo odb³ysków
	//robimy oszustwo, bo z aktualnym ustawieniem, bump na statkach lepiej wygl¹da
	GetLighting()->SetPosition( GL_LIGHT2, CVector4( 0.0f, 3000.0f, 0.0f, 1.0f ) );	//ustaw pozycje swiatla
	glDisable( GL_LIGHT2 );
}

GLvoid CGameControl::CreateAllAnimCtrl()
{
	//TODO: Tutaj wywo³uj metody GetSpeedCtrl()->CreateAnimationControl();
	GetSpeedCtrl()->CreateAnimationControl(); //0 - dla intra
	GetSpeedCtrl()->CreateAnimationControl(); //1 - dla rozpoczecia levelu - napis Level X Easy, Ready!
	GetSpeedCtrl()->CreateAnimationControl(); //2 - dla czestotliwosc strzelania gracza, takze zalezne od gwiazdki
	GetSpeedCtrl()->CreateAnimationControl(); //3 - dla animacji eksplozji gracza
	GetSpeedCtrl()->CreateAnimationControl(); //4 - dla zmiany koloru gracza gdy gracz oberwie
	GetSpeedCtrl()->CreateAnimationControl(); //5 - dla odczekania paru sekund na press any key, gdy game over
	GetSpeedCtrl()->CreateAnimationControl(); //6 - dla trzy sekundowej niesmiertelnosci gdy gracz straci zycie
	GetSpeedCtrl()->CreateAnimationControl(); //7 - dla rysowania gwiazdek - SStar w RGame
	GetSpeedCtrl()->CreateAnimationControl(); //8 - migajacy kursor dla "Enter your name"
	GetSpeedCtrl()->CreateAnimationControl(); //9 - dla spowolnienia rywala ( bonus )
	GetSpeedCtrl()->CreateAnimationControl(); //10 - dla dodawania nowych odbloczkow dymu dla zdalnej rakiety
	GetSpeedCtrl()->CreateAnimationControl(); //11 - dla dzwieku w CheckEngage
	GetSpeedCtrl()->CreateAnimationControl(); //12 - dla zliczania multikillow
	GetSpeedCtrl()->CreateAnimationControl(); //13 - dla zablokowania klawisza enter na 1 sec. gdy zdalna rakieta zderzy sie z wrogiem
	GetSpeedCtrl()->CreateAnimationControl(); //14 - dla kontroli dodawania nowych czasteczek dymu zniszczen gracza
	GetSpeedCtrl()->CreateAnimationControl(); //15 - dla dzwieku chaingun-a
	GetSpeedCtrl()->CreateAnimationControl(); //16 - dla czestotliwosci strzelania impulse laser
	GetSpeedCtrl()->CreateAnimationControl(); //17 - dla osi joy'a X
	GetSpeedCtrl()->CreateAnimationControl(); //18 - dla osi joy'a Y
}

#if !_USE_LOAD_PROGESS_BAR_
GLboolean CGameControl::LoadAllTextures()
{
	if( !GetTexLoader() )
		return GL_FALSE;

	//Tutaj nalezy wpisywac kolejne tekstury jake chcemy wykorzystac, wedle ponizszego wzoru
	//TODO: tutaj wczytuj kolejne tekstury

	GetTexLoader()->LoadTexMipmaps( _T("textures/test.jeh") );		//index 0
	GetTexLoader()->LoadTexLowQuality( _T("textures/console_mask.jeh") );	//index 1
	
	m_iTexLogo = GetTexLoader()->LoadTex( _T("textures/logo.jeh") );

	m_iTexBonusViewfinder = GetTexLoader()->LoadMultiTexMask( _T("textures/bonus_viewfinder_mask.jeh"), RGB( 0, 255, 0 ) );
	m_iTexBonusShield = GetTexLoader()->LoadMultiTexMask( _T("textures/bonus_shield_mask.jeh"), RGB( 0, 255, 255 ) );
	m_iTexBonusRemoteRocket = GetTexLoader()->LoadMultiTexMask( _T("textures/bonus_remotebomb_mask.jeh"), _T("textures/bonus_remotebomb.jeh") );
	m_iTexBonusEnemySlowdown = GetTexLoader()->LoadMultiTexMask( _T("textures/bonus_enemy_slowdown_mask.jeh"), _T("textures/bonus_enemy_slowdown.jeh") );

	m_iTexStarFill = GetTexLoader()->LoadMultiTexMaskMipmaps( _T("textures/starfill_mask.jeh"), RGB( 255, 255, 0 ) );
	m_iTexStarEmpty = GetTexLoader()->LoadMultiTexMaskMipmaps( _T("textures/starempty_mask.jeh"), RGB( 255, 255, 0 ) );

	m_aTexBackground.clear();
	m_aTexBackground.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/background01.jeh") ) );
	m_aTexBackground.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/background02.jeh") ) );
	m_aTexBackground.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/background03.jeh") ) );
	m_aTexBackground.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/background04.jeh") ) );

	m_aTexExplosion.clear();
	m_aTexExplosion.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/explosion01.jeh") ) );
	m_aTexExplosion.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/explosion02.jeh") ) );
	m_aTexExplosion.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/explosion03.jeh") ) );

	m_iTexPlayerHitShield = GetTexLoader()->LoadMultiTexMaskMipmaps( _T("textures/playerhit_mask.jeh"), RGB( 0, 255, 255 ) );
	m_iTexPlayerHitArmour = GetTexLoader()->LoadMultiTexMaskMipmaps( _T("textures/playerhit_mask.jeh"), RGB( 255, 0, 0 ) );

	m_iTexRemoteRocketsCounter = GetTexLoader()->LoadMultiTexMaskMipmaps( _T("textures/remote_bomb_counter_mask.jeh"), _T("textures/remote_bomb_counter.jeh") );

	m_iTexParticle01 = GetTexLoader()->LoadTexLowQuality( _T("textures/particle01.jeh") );
	m_iTexSmoke00 = GetTexLoader()->LoadTexLowQuality( _T("textures/smoke00.jeh") );
#ifndef DRAW_PLAYER_SHIELD_3D
	m_iTexShield2D = GetTexLoader()->LoadTexMipmaps( _T("textures/shield2d.jeh") );
#endif

	m_iTexShadowTest = GetTexLoader()->LoadTexMipmaps( _T("textures/shadow_test.jeh") );

	m_aTexWeaponSelect.clear();
	m_aTexWeaponSelect.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/weaponselect1.jeh") ) );
	m_aTexWeaponSelect.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/weaponselect2.jeh") ) );
	m_aTexWeaponSelect.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/weaponselect3.jeh") ) );
	m_aTexWeaponSelect.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/weaponselect4.jeh") ) );
	m_aTexWeaponSelect.push_back( GetTexLoader()->LoadTexMipmaps( _T("textures/weaponselect5.jeh") ) );

	return GL_TRUE;
}
#else
GLboolean CGameControl::LoadAllTextures()
{
	if( !GetLoadProgressBar() )
		return GL_FALSE;

	//Tutaj nalezy wpisywac kolejne tekstury jake chcemy wykorzystac, wedle ponizszego wzoru
	//TODO: tutaj wczytuj kolejne tekstury

	GLint iTemp;
	GetLoadProgressBar()->AddTexMimpmap( &iTemp, _T("textures/test.jeh") );
	GetLoadProgressBar()->AddTexLowQuality( &iTemp, _T("textures/console_mask.jeh") );

	GetLoadProgressBar()->AddTex( &m_iTexLogo, _T("textures/logo.jeh") );
	
	GetLoadProgressBar()->AddMultiTexMask( &m_iTexBonusViewfinder, _T("textures/bonus_viewfinder_mask.jeh"), RGB( 0, 255, 0 ) );
	GetLoadProgressBar()->AddMultiTexMask( &m_iTexBonusShield, _T("textures/bonus_shield_mask.jeh"), RGB( 0, 255, 255 ) );
	GetLoadProgressBar()->AddMultiTexMask( &m_iTexBonusRemoteRocket, _T("textures/bonus_remotebomb_mask.jeh"), _T("textures/bonus_remotebomb.jeh") );
	GetLoadProgressBar()->AddMultiTexMask( &m_iTexBonusEnemySlowdown, _T("textures/bonus_enemy_slowdown_mask.jeh"), _T("textures/bonus_enemy_slowdown.jeh") );
	
	GetLoadProgressBar()->AddMultiTexMaskMipmaps( &m_iTexStarFill, _T("textures/starfill_mask.jeh"), RGB( 255, 255, 0 ) );
	GetLoadProgressBar()->AddMultiTexMaskMipmaps( &m_iTexStarEmpty, _T("textures/starempty_mask.jeh"), RGB( 255, 255, 0 ) );

	m_aTexBackground.clear();
	for( GLint i = 0; i < 5; ++i )
		m_aTexBackground.push_back( 0 );

	GetLoadProgressBar()->AddTexMimpmap( &m_aTexBackground[ 0 ], _T("textures/background01.jeh") );
	GetLoadProgressBar()->AddTexMimpmap( &m_aTexBackground[ 1 ], _T("textures/background02.jeh") );
	GetLoadProgressBar()->AddTexMimpmap( &m_aTexBackground[ 2 ], _T("textures/background03.jeh") );
	GetLoadProgressBar()->AddTexMimpmap( &m_aTexBackground[ 3 ], _T("textures/background04.jeh") );
	GetLoadProgressBar()->AddTexMimpmap( &m_aTexBackground[ 4 ], _T("textures/background05.jeh") );

	m_aTexExplosion.clear();
	for( GLint i = 0; i < 3; ++i )
		m_aTexExplosion.push_back( 0 );

	GetLoadProgressBar()->AddTexMimpmap( &m_aTexExplosion[ 0 ], _T("textures/explosion01.jeh") );
	GetLoadProgressBar()->AddTexMimpmap( &m_aTexExplosion[ 1 ], _T("textures/explosion02.jeh") );
	GetLoadProgressBar()->AddTexMimpmap( &m_aTexExplosion[ 2 ], _T("textures/explosion03.jeh") );

	GetLoadProgressBar()->AddMultiTexMaskMipmaps( &m_iTexPlayerHitShield, _T("textures/playerhit_mask.jeh"), RGB( 0, 255, 255 ) );
	GetLoadProgressBar()->AddMultiTexMaskMipmaps( &m_iTexPlayerHitArmour, _T("textures/playerhit_mask.jeh"), RGB( 255, 0, 0 ) );

	GetLoadProgressBar()->AddMultiTexMaskMipmaps( &m_iTexRemoteRocketsCounter, _T("textures/remote_bomb_counter_mask.jeh"), _T("textures/remote_bomb_counter.jeh") );

	GetLoadProgressBar()->AddTexLowQuality( &m_iTexParticle01, _T("textures/particle01.jeh") );
	GetLoadProgressBar()->AddTexLowQuality( &m_iTexSmoke00, _T("textures/smoke00.jeh") );

#ifndef DRAW_PLAYER_SHIELD_3D
	GetLoadProgressBar()->AddTexMimpmap( &m_iTexShield2D, _T("textures/shield2d.jeh") );
#endif
	GetLoadProgressBar()->AddTexMimpmap( &m_iTexShadowTest, _T("textures/shadow_test.jeh") );

	m_aTexWeaponSelect.clear();
	for( GLint i = 0; i < 5; ++i )
		m_aTexWeaponSelect.push_back( 0 );

	GetLoadProgressBar()->AddTexMimpmap( &m_aTexWeaponSelect[ 0 ], _T("textures/weaponselect1.jeh") );
	GetLoadProgressBar()->AddTexMimpmap( &m_aTexWeaponSelect[ 1 ], _T("textures/weaponselect2.jeh") );
	GetLoadProgressBar()->AddTexMimpmap( &m_aTexWeaponSelect[ 2 ], _T("textures/weaponselect3.jeh") );
	GetLoadProgressBar()->AddTexMimpmap( &m_aTexWeaponSelect[ 3 ], _T("textures/weaponselect4.jeh") );
	GetLoadProgressBar()->AddTexMimpmap( &m_aTexWeaponSelect[ 4 ], _T("textures/weaponselect5.jeh") );


	return GL_TRUE;
}
#endif

GLboolean CGameControl::CreateAllParticles()
{
	if( !GetParticles() || !GetShipsManager() )
		return GL_FALSE;
	
	for( GLint i = 0; i < GetShipsManager()->GetUniqueShipsSize(); ++i ) {
		for( GLint j = 0; j < static_cast< GLint >( GetShipsManager()->GetUniqueShip( i ).iEmitterNumber ); ++j ) {
			
			GLint iTexIndex = GetShipsManager()->GetUniqueShip( i ).psEmitterData[ j ].iTexEmitter;
			if( iTexIndex == 5 )
				iTexIndex = m_iTexParticle01;

			GetShipsManager()->GetUniqueShip( i ).psEmitterData[ j ].iIndex = GetParticles()->CreateEmitter(
				//        z       y      x
				CVector3( GetShipsManager()->GetUniqueShip( i ).psEmitterData[ j ].fEmitterPosX,
						  GetShipsManager()->GetUniqueShip( i ).psEmitterData[ j ].fEmitterPosY,
						  GetShipsManager()->GetUniqueShip( i ).psEmitterData[ j ].fEmitterPosZ ),
				iTexIndex, //textura
				CVector3( 0.0f, 0.0f, 0.0f ),
				CVector3( 0.0f, 0.0f, 0.0f ),
				1.0f, //scale
				1.0f, //life
				GL_TRUE, //czasteczka po wygasnieciu moze ozywac
				GetShipsManager()->GetUniqueShip( i ).psEmitterData[ j ].fR, //r
				GetShipsManager()->GetUniqueShip( i ).psEmitterData[ j ].fG, //g
				GetShipsManager()->GetUniqueShip( i ).psEmitterData[ j ].fB, //b
				-200.0f, //speedX
				0.0f, //speedY
				0.0f, //speedZ
				GetShipsManager()->GetUniqueShip( i ).psEmitterData[ j ].iParticlesNumber, //ilosc czastek
				60.0f,	//fMoveSpeed
				60		//iRandSpeedRange1
				);
		}
	}

	return GL_TRUE;
}

#if !_USE_LOAD_PROGESS_BAR_
GLboolean CGameControl::LoadAll3DObjFiles()
{
	if( !Get3DObjManager() || !GetShipsManager() || !GetTexLoader() )
		return GL_FALSE;
	
	//TODO: Tutaj wczytuj pliki .3DObj
	for( GLint i = 0; i < GetShipsManager()->GetUniqueShipsSize(); ++i ) {
		GLint iTexIndex = 0;
		switch( GetShipsManager()->GetUniqueShip( i ).byTexType ) {
			case 2: //bez tekstury
				iTexIndex = -1;
				break;
			case 1:
				iTexIndex = GetTexLoader()->LoadMultiTexMaskMipmaps( GetShipsManager()->GetUniqueShip( i ).cTexName, GetShipsManager()->GetUniqueShip( i ).uiCOLORREF );
				break;
			default: //0
				iTexIndex = GetTexLoader()->LoadTexMipmaps( GetShipsManager()->GetUniqueShip( i ).cTexName );
				//sprawdz czy jest textura bump
				if( ::GetStringLength( GetShipsManager()->GetUniqueShip( i ).cTexBumpName ) > 0 )
					GetShipsManager()->GetUniqueShip( i ).iTexBumpIndex = GetTexLoader()->LoadEmbossBump( GetShipsManager()->GetUniqueShip( i ).cTexBumpName );
				else
					GetShipsManager()->GetUniqueShip( i ).iTexBumpIndex = -1;
				break;
		}
		GetShipsManager()->GetUniqueShip( i ).iTexIndex = iTexIndex;

		//czy jest tekstura z "zyciem"
		if( ::GetStringLength( GetShipsManager()->GetUniqueShip( i ).cTexLife ) > 0 &&
			::GetStringLength( GetShipsManager()->GetUniqueShip( i ).cTexLifeMask ) > 0 )
		{
			GetShipsManager()->GetUniqueShip( i ).iTexLife = GetTexLoader()->LoadMultiTexMaskMipmaps( GetShipsManager()->GetUniqueShip( i ).cTexLifeMask, GetShipsManager()->GetUniqueShip( i ).cTexLife );
		}
		else
			GetShipsManager()->GetUniqueShip( i ).iTexLife = 0;

		//wczytaj obiekt
		GetShipsManager()->GetUniqueShip( i ).i3DObjIndex = Get3DObjManager()->Create3DObject( 
				GetShipsManager()->GetUniqueShip( i ).cObjectName,
				GetShipsManager()->GetUniqueShip( i ).iTexIndex,
				-1,
				GetShipsManager()->GetUniqueShip( i ).iTexBumpIndex );
	}

	Get3DObjManager()->Initialization(); // Musi byæ wywo³ana na samym koñcu

	return GL_TRUE;
}

GLboolean CGameControl::LoadAllSounds()
{
#if _USE_SOUND_OPENAL_

	if( !GetOpenALManager() )
		return GL_FALSE;

	if( !GetOpenALManager()->IsInitOK() )
		return GL_FALSE;

	//TODO: Tutaj wczytuj wszelkie dŸwiêki dla biblioteki OpenAL

	m_iSoundTest =			GetOpenALManager()->LoadSound( "sounds/test.wav" );		//0
//	m_iSoundTestOgg =		GetOpenALManager()->LoadSound( "sounds/test.ogg", AL_TRUE, AL_TRUE );
	m_iSoundMouseOn =		GetOpenALManager()->LoadSound( "sounds/mouseon.wav" );
	
	m_aSoundExplosion.clear();
	for( ALint i = 0; i < 20; ++i )
		m_aSoundExplosion.push_back( GetOpenALManager()->LoadSound( "sounds/explosion_noncommercial.wav" ) );
	//m_aSoundExplosion.push_back( GetOpenALManager()->LoadSound( "sounds/explosion_noncommercial.wav" ) );
	//for( ALint i = 0; i < 19; ++i )
	//	GetOpenALManager()->LoadSoundFromBuffer( GetOpenALManager()->GetBufferFromSoundOAL( 0 ) );
	
	m_aSoundWeaponSelect.clear();
	m_aSoundWeaponSelect.push_back( GetOpenALManager()->LoadSound( "sounds/weaponselect1.wav" ) );
	m_aSoundWeaponSelect.push_back( GetOpenALManager()->LoadSound( "sounds/weaponselect2.wav" ) );
	m_aSoundWeaponSelect.push_back( GetOpenALManager()->LoadSound( "sounds/weaponselect3.wav" ) );
	m_aSoundWeaponSelect.push_back( GetOpenALManager()->LoadSound( "sounds/weaponselect4.wav" ) );
	m_aSoundWeaponSelect.push_back( GetOpenALManager()->LoadSound( "sounds/weaponselect5.wav" ) );

	m_iSoundLaser =			GetOpenALManager()->LoadSound( "sounds/laser_noncommercial.wav" );
	m_iSoundChaingun =		GetOpenALManager()->LoadSound( "sounds/machine_-Diode111-8769.wav" );
	m_iSoundImpulseLaser =	GetOpenALManager()->LoadSound( "sounds/PulseGun-Mark_E_B-707.wav" );
	m_iSoundHomingRocket =	GetOpenALManager()->LoadSound( "sounds/homingrocket.wav" );
	m_iSoundTakeBonus =		GetOpenALManager()->LoadSound( "sounds/take_bonus00.wav" );
	m_iSoundRemoteRocket =	GetOpenALManager()->LoadSound( "sounds/remoterocket00.wav" );
	m_iSoundJoy =			GetOpenALManager()->LoadSound( "sounds/joy02.wav" );
	m_iSoundFanfary =		GetOpenALManager()->LoadSound( "sounds/fanfary01.wav" );
	m_iSoundShieldHit =		GetOpenALManager()->LoadSound( "sounds/shield_hit03.wav" );
	m_iSoundArmourHit =		GetOpenALManager()->LoadSound( "sounds/armour_hit01.wav" );

	return AL_TRUE;

#else // FMOD

	if( !GetSoundFMOD() )
		return GL_FALSE;

	//TODO: Tutaj wczytuj wszelkie dŸwiêki dla biblioteki FMOD

	GetSoundFMOD()->LoadSample( "sounds/test.wav" );

	return AL_TRUE;
#endif // _USE_SOUND_OPENAL_
}
#else // _USE_LOAD_PROGESS_BAR_
GLboolean CGameControl::LoadAll3DObjFiles()
{
	if( !GetLoadProgressBar() )
		return GL_FALSE;

	for( GLint i = 0; i < GetShipsManager()->GetUniqueShipsSize(); ++i ) {
		GetLoadProgressBar()->Add3DObj( GetShipsManager()->GetUniqueShip( i ).byTexType,
										&GetShipsManager()->GetUniqueShip( i ).iTexIndex,
										&GetShipsManager()->GetUniqueShip( i ).iTexBumpIndex,
										GetShipsManager()->GetUniqueShip( i ).cTexLife,
										GetShipsManager()->GetUniqueShip( i ).cTexLifeMask,
										&GetShipsManager()->GetUniqueShip( i ).iTexLife,
										&GetShipsManager()->GetUniqueShip( i ).i3DObjIndex,
										GetShipsManager()->GetUniqueShip( i ).cObjectName,
										GetShipsManager()->GetUniqueShip( i ).uiCOLORREF,
										GetShipsManager()->GetUniqueShip( i ).cTexBumpName,
										GetShipsManager()->GetUniqueShip( i ).cTexName );
	}

	return GL_TRUE;
}

GLboolean CGameControl::LoadAllSounds()
{
#if _USE_SOUND_OPENAL_

	GetLoadProgressBar()->AddSound( &m_iSoundTest, "sounds/test.wav");
	GetLoadProgressBar()->AddSound( &m_iSoundMouseOn, "sounds/mouseon.wav");

#if _USE_OGG_
	m_aMusicMenu.clear();
	for( GLint i = 0; i < 1; ++i )
		m_aMusicMenu.push_back( 0 );
	GetLoadProgressBar()->AddMusic( &m_aMusicMenu[ 0 ], "music/menu1.ogg" );

	m_aMusicGame.clear();
	for( GLint i = 0; i < 10; ++i )
		m_aMusicGame.push_back( 0 );
	GetLoadProgressBar()->AddMusic( &m_aMusicGame[ 0 ], "music/game1.ogg" );
	GetLoadProgressBar()->AddMusic( &m_aMusicGame[ 1 ], "music/game2.ogg" );
	GetLoadProgressBar()->AddMusic( &m_aMusicGame[ 2 ], "music/game3.ogg" );
	GetLoadProgressBar()->AddMusic( &m_aMusicGame[ 3 ], "music/game4.ogg" );
	GetLoadProgressBar()->AddMusic( &m_aMusicGame[ 4 ], "music/game5.ogg" );
	GetLoadProgressBar()->AddMusic( &m_aMusicGame[ 5 ], "music/game6.ogg" );
	GetLoadProgressBar()->AddMusic( &m_aMusicGame[ 6 ], "music/game7.ogg" );
	GetLoadProgressBar()->AddMusic( &m_aMusicGame[ 7 ], "music/game8.ogg" );
	GetLoadProgressBar()->AddMusic( &m_aMusicGame[ 8 ], "music/game9.ogg" );
	GetLoadProgressBar()->AddMusic( &m_aMusicGame[ 9 ], "music/game10.ogg" );
#endif // _USE_OGG_

	m_aSoundExplosion.clear();
	for( GLint i = 0; i < 20; ++i )
		m_aSoundExplosion.push_back( 0 );

	for( GLint i = 0; i < 20; ++i )
		GetLoadProgressBar()->AddSound( &m_aSoundExplosion[ i ], "sounds/explosion.wav" );

	m_aSoundWeaponSelect.clear();
	for( GLint i = 0; i < 5; ++i )
		m_aSoundWeaponSelect.push_back( 0 );

	GetLoadProgressBar()->AddSound( &m_aSoundWeaponSelect[ 0 ], "sounds/weaponselect1.wav" );
	GetLoadProgressBar()->AddSound( &m_aSoundWeaponSelect[ 1 ], "sounds/weaponselect2.wav" );
	GetLoadProgressBar()->AddSound( &m_aSoundWeaponSelect[ 2 ], "sounds/weaponselect3.wav" );
	GetLoadProgressBar()->AddSound( &m_aSoundWeaponSelect[ 3 ], "sounds/weaponselect4.wav" );
	GetLoadProgressBar()->AddSound( &m_aSoundWeaponSelect[ 4 ], "sounds/weaponselect5.wav" );

	GetLoadProgressBar()->AddSound( &m_iSoundLaser, "sounds/laser.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundChaingun, "sounds/machine_-Diode111-8769.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundImpulseLaser, "sounds/impulse_laser.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundHomingRocket, "sounds/homingrocket.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundTakeBonus, "sounds/take_bonus00.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundRemoteRocket, "sounds/remoterocket00.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundJoy, "sounds/joy02.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundFanfary, "sounds/fanfary01.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundShieldHit, "sounds/shield_hit03.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundArmourHit, "sounds/armour_hit01.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundCongratulation, "sounds/congratulations.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundGameOver, "sounds/game_over.wav" );
	GetLoadProgressBar()->AddSound( &m_iSoundEngage, "sounds/engage.wav" );

#endif // _USE_SOUND_OPENAL_

	return GL_TRUE;
}
#endif // !_USE_LOAD_PROGESS_BAR_

GLvoid CGameControl::Exit()
{
	GetRMessageBox()->DoMessageBox( LNG_LINE( 34 ), CRMessageBox::EMsgYesNo, MsgExit, NULL );
}

GLvoid MsgExit( GLvoid ) //odpowiedz na klikniecie na tak w msgboxie przy wychodzeniu z gry
{
	::PostQuitMessage( 0 );
}

GLvoid CGameControl::RestartObjects()
{
	SetIndicesQuadAllScreen();

	//przy restartowaniu nalezy dla kazdego obiektu klasy CSDLFont wywolac funkcje ClearStorageBuffer
	for( GLint i = static_cast< GLint >( g_aPointerSDLFont.size() - 1 ); i >= 0; --i ) {
		if( g_aPointerSDLFont[ i ] )
			g_aPointerSDLFont[ i ]->ClearStorageBuffer();
	}

	delete m_p3DObjManager;
	delete m_pTextureLoader;
	delete m_pVertexArrays;

	m_pTextureLoader = new CTextureLoader();
	GetEmbossBump()->ClearAllArrays();
	m_pVertexArrays = CVertexArrays::Construct();
	GetStencilShadow()->RestartObjects();
	m_p3DObjManager = new C3DObjManager();

	GetConsole()->RestartObjects();
	GetRMessageBox()->RestartObjects();
	GetRMainMenu()->RestartObjects();
	GetRDifficultyLevel()->RestartObjects();
	GetRShipsBrowse()->RestartObjects();
	GetRCredits()->RestartObjects();
	GetRHighscore()->RestartObjects();
	GetROptions()->RestartObjects();
	GetROptionsGraphics()->RestartObjects();
	GetROptionsSound()->RestartObjects();
	GetROptionsControl()->RestartObjects();
#if _USE_MULTIPLYER_
	GetRMultiplayerMenu()->RestartObjects();
#endif

	DeleteSkyBoxList();
	CreateSkyBoxList();

#if _USE_LOAD_PROGESS_BAR_
	GetLoadProgressBar()->Clear();
#endif
	SetLights();
	LoadAllTextures();
	LoadAll3DObjFiles();
#if _USE_LOAD_PROGESS_BAR_
	GetLoadProgressBar()->LoadProgressResForRestart();
	Get3DObjManager()->Initialization();
#endif
	
	SetVSync();
	CreateVertices();

#ifdef _USE_QUADRIC_
	CreateQuadric();
#endif //_USE_QUADRIC_

	GetRGame()->RestartObjects();
}

GLboolean CGameControl::KeyDown( GLuint uiKey )
{
	SetZeroReIntroShow();

	if( GetConsole()->KeyDown( uiKey ) )
		return GL_TRUE;
	
	if( GetRMessageBox()->KeyDown( uiKey ) )
		return GL_TRUE;
	
	if( uiKey == 0xC0 ) { //tylda - otwieramy konsole
		m_bKeys[ uiKey ] = GL_TRUE;
		return GL_TRUE;
	}
	
	GLboolean bKeyWasConsumed = GL_FALSE;
	switch( m_eMainGameMode ) {
		case ELogo:
			GetRMainMenu()->SetStartMenuAnimY( GL_FALSE );
			SetEMainGameMode( EIntro );
			DeleteLogo();
			return GL_TRUE;
		case EIntro:
			BreakShowIntro();
			return GL_TRUE;
		case EMainMenu:
			bKeyWasConsumed = GetRMainMenu()->KeyDown( uiKey );
			break;
		case EDifficultyLevel:
			bKeyWasConsumed = GetRDifficultyLevel()->KeyDown( uiKey );
			break;
		case EShipsBrowser:
			bKeyWasConsumed = GetRShipsBrowse()->KeyDown( uiKey );
			break;
		case ECredits:
			bKeyWasConsumed = GetRCredits()->KeyDown( uiKey );
			break;
		case ESingleGame:
			//aby zapewnic mozliwosc naciskania kilku klawiszy na raz
			//CRGame::KeyDown() wywolujemy w CGameControl::Keyboard()
			if( GetRGame()->IsShowEnterNickname() ||	//gracz pisze imie
				GetRGame()->GetYouCanPressAnyKey() )	//lub press any key
			{
				bKeyWasConsumed = GetRGame()->KeyDown( uiKey );
			}
			else {
				bKeyWasConsumed = GL_FALSE;
				//naprawa buga 22:
				if( uiKey == VK_CONTROL ) {//<- jezeli mamy komunikat VK_CONTROL
					if( HIBYTE( ::GetAsyncKeyState( VK_MENU ) ) )//<- ale wcisniety jest Alt
						bKeyWasConsumed = GL_TRUE;  //<- to nie analizuj w Keyboard()
				}
			}
			break;
		case EHighscore:
			bKeyWasConsumed = GetRHighscore()->KeyDown( uiKey );
			break;
		case EOptions:
			bKeyWasConsumed = GetROptions()->KeyDown( uiKey );
			break;
		case EOptionsGraphics:
			bKeyWasConsumed = GetROptionsGraphics()->KeyDown( uiKey );
			break;
		case EOptionsSound:
			bKeyWasConsumed = GetROptionsSound()->KeyDown( uiKey );
			break;
		case EOptionsControl:
			bKeyWasConsumed = GetROptionsControl()->KeyDown( uiKey );
			break;
#if _USE_MULTIPLYER_
		case EMultiplayerMenu:
			bKeyWasConsumed = GetRMultiplayerMenu()->KeyDown( uiKey );
			break;
#endif
	}

	//aby klawisz przeszedl do funkcji CGameControl::Keyboard()
	//gdy nie zostal skonsumowany:
	if( !bKeyWasConsumed ) {
		m_bKeys[ uiKey ] = GL_TRUE;
		return GL_TRUE;
	}

	return bKeyWasConsumed;
}

GLboolean CGameControl::KeyUp( GLuint uiKey )
{
	SetZeroReIntroShow();

	if( GetConsole()->KeyUp( uiKey ) )
		return GL_TRUE;

	m_bKeys[ uiKey ] = GL_FALSE;
	if( uiKey == VK_CONTROL )
		m_bVkControl = GL_FALSE;

	return GL_TRUE;
}

GLboolean CGameControl::MouseLButtonDown( GLint iX, GLint iY )
{
	SetZeroReIntroShow();

	if( GetRMessageBox()->MouseLButtonDown( iX, iY ) )
		return GL_TRUE;

	GLboolean bLButtonWasConsumed = GL_FALSE;
	switch( m_eMainGameMode ) {
		case ELogo:
			GetRMainMenu()->SetStartMenuAnimY( GL_FALSE );
			SetEMainGameMode( EIntro );
			DeleteLogo();
			return GL_TRUE;
		case EIntro:
			BreakShowIntro();
			return GL_TRUE;
		case EMainMenu:
			bLButtonWasConsumed = GetRMainMenu()->MouseLButtonDown( iX, iY );
			break;
		case EDifficultyLevel:
			bLButtonWasConsumed = GetRDifficultyLevel()->MouseLButtonDown( iX, iY );
			break;
		case EShipsBrowser:
			bLButtonWasConsumed = GetRShipsBrowse()->MouseLButtonDown( iX, iY );
			break;
		case ECredits:
			bLButtonWasConsumed = GetRCredits()->MouseLButtonDown( iX, iY );
			break;
		case ESingleGame:
			bLButtonWasConsumed = GetRGame()->MouseLButtonDown( iX, iY );
			break;
		case EHighscore:
			bLButtonWasConsumed = GetRHighscore()->MouseLButtonDown( iX, iY );
			break;
		case EOptions:
			bLButtonWasConsumed = GetROptions()->MouseLButtonDown( iX, iY );
			break;
		case EOptionsGraphics:
			bLButtonWasConsumed = GetROptionsGraphics()->MouseLButtonDown( iX, iY );
			break;
		case EOptionsSound:
			bLButtonWasConsumed = GetROptionsSound()->MouseLButtonDown( iX, iY );
			break;
		case EOptionsControl:
			bLButtonWasConsumed = GetROptionsControl()->MouseLButtonDown( iX, iY );
			break;
#if _USE_MULTIPLYER_
		case EMultiplayerMenu:
			bLButtonWasConsumed = GetRMultiplayerMenu()->MouseLButtonDown( iX, iY );
			break;
#endif
	}

	return bLButtonWasConsumed;
}

GLboolean CGameControl::MouseLButtonUp( GLint iX, GLint iY )
{
	SetZeroReIntroShow();

	GLboolean bLButtonWasConsumed = GL_FALSE;
	switch( m_eMainGameMode ) {
		case EShipsBrowser:
			bLButtonWasConsumed = GetRShipsBrowse()->MouseLButtonUp( iX, iY );
			break;
		case ESingleGame:
			bLButtonWasConsumed = GetRGame()->MouseLButtonUp( iX, iY );
			break;
	}
	return bLButtonWasConsumed;
}

GLboolean CGameControl::MouseRButtonDown( GLint iX, GLint iY )
{
	//tutaj przetwarzamy klikniecie prawego przycisku
	//myszy lub przekazujemy je dalej do innej klasy

	SetZeroReIntroShow();

	GLboolean bRButtonWasConsumed = GL_FALSE;
	switch( m_eMainGameMode ) {
		case ESingleGame:
			bRButtonWasConsumed = GetRGame()->MouseRButtonDown( iX, iY );
			break;
	}
	return bRButtonWasConsumed;
}

GLboolean CGameControl::MouseRButtonUp( GLint /*iX*/, GLint /*iY*/ )
{
	SetZeroReIntroShow();

	return GL_FALSE;
}

GLboolean CGameControl::MouseWheel( GLint iScrollLines )
{
	SetZeroReIntroShow();

	if( GetConsole()->MouseWheel( iScrollLines ) )
		return GL_TRUE;
	
	//tutaj implementujemy obsluge rolki myszy
	//parametr iScrollLines mowi nam o ile linii i w ktora strone 
	//wykonano obrot, wartosc ujemna - w dol, wartosc dodatnia - w gore

	GLboolean bWheelWasConsumed = GL_FALSE;
	switch( m_eMainGameMode ) {
		case EShipsBrowser:
			bWheelWasConsumed = GetRShipsBrowse()->MouseWheel( iScrollLines );
			break;
		case ESingleGame:
			bWheelWasConsumed = GetRGame()->MouseWheel( iScrollLines );
			break;
#if _USE_MULTIPLYER_
		case EMultiplayerMenu:
			bWheelWasConsumed = GetRMultiplayerMenu()->MouseWheel( iScrollLines );
			break;
#endif
	}
	return bWheelWasConsumed;
}

GLvoid CGameControl::Enable2D()
{
	if( m_bIs2D )
		return;
	m_bIs2D = GL_TRUE;

	GLdouble vPort[ 4 ];
	glGetDoublev( GL_VIEWPORT, vPort );
  
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
  
	glOrtho( 0.0, vPort[ 2 ], vPort[ 3 ], 0.0, -1.0, 1.0 );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
}

GLvoid CGameControl::Disable2D()
{
	if( !m_bIs2D )
		return;
	m_bIs2D = GL_FALSE;

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();   
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();	
}

GLvoid CGameControl::Draw()
{
	PROFILER_BEGIN( _T("CGameControl::Draw()") );
	
#if _USE_LOAD_PROGESS_BAR_
	if( GetLoadProgressBar()->IsLoadProgressEnd() ) {
#endif
		GetSpeedCtrl()->RefreshTime();
		GetFps()->IncreaseFps();
#if _USE_LOAD_PROGESS_BAR_
	}
#endif

//******************** tutaj rysujemy ****************************************
	//wyszsto co jest zawarte miedzy /*****/ jest tylko przyk³adem i oczywiscie
	//nale¿y w to miejsce rysowaæ w³asne obiekty, wedle w³asnego uznania
	
	if( m_eMainGameMode != ELogo &&
		m_eMainGameMode != EShipsBrowser &&
		m_eMainGameMode != EIntro &&
		m_eMainGameMode != ESingleGame )
	{
		DrawBackground();
	}

	GetCursorPosition();

	switch( m_eMainGameMode ) {
		case ELogo:
			if( GetRLogo() )
				GetRLogo()->Draw();
			break;
		case EIntro:
			DrawIntro();
			break;
		case EMainMenu:
			GetRMainMenu()->Draw();
			break;
		case ESingleGame:
			GetRGame()->Draw();
			break;
		case EOptions:
			GetROptions()->Draw();
			break;
		case EOptionsGraphics:
			GetROptionsGraphics()->Draw();
			break;
		case EOptionsSound:
			GetROptionsSound()->Draw();
			break;
		case EOptionsControl:
			GetROptionsControl()->Draw();
			break;
		case EHighscore:
			GetRHighscore()->Draw();
			break;
#if _USE_MULTIPLYER_
		case EMultiplayerMenu:
			GetRMultiplayerMenu()->Draw();
			break;
#endif
		case EDifficultyLevel:
			GetRDifficultyLevel()->Draw();
			break;
		case EShipsBrowser:
			GetRShipsBrowse()->Draw();
			break;
		case ECredits:
			GetRCredits()->Draw();
			break;
	}

#if _USE_LOAD_PROGESS_BAR_
	if( GetLoadProgressBar()->IsLoadProgressEnd() )
#endif
		GetRMessageBox()->Draw();

//*************************** koniec rysowania ************************************
	glLoadIdentity();

#if _USE_LOAD_PROGESS_BAR_
	if( GetLoadProgressBar()->IsLoadProgressEnd() ) {
#endif
		GetFps()->DrawFps();

#if _USE_2D_ONLY_
		//jezeli mamy sta³y tryb 2D to mo¿emy wyœwietliæ konsolê tylko
		//za pomoc¹ CSDLFont
		GetConsole()->DrawConsoleWithSDLFont();
#else
	#if _USE_CONSOLE_WITH_MY_FONT_
		GetConsole()->DrawConsoleMyFont();
	#elif _USE_CONSOLE_WITH_BITMAP_FONT_
		GetConsole()->DrawConsoleWithBitmapFont();
	#else
		GetConsole()->DrawConsoleWithSDLFont();
	#endif //_USE_CONSOLE_WITH_MY_FONT_ _USE_CONSOLE_WITH_BITMAP_FONT_
#endif //_USE_2D_ONLY_

		//rysowanie jasnosci, poni¿ej tej instrukcji nie mo¿na ju¿ niczego rysowaæ
		CBrightness::GetInstance()->Draw();

#if _USE_LOAD_PROGESS_BAR_
	}
#endif

	PROFILER_END();
}

GLvoid CGameControl::DrawBackground()
{
	glDisable( GL_BLEND );
	glEnable( GL_LIGHTING );
	glEnable( GL_DEPTH_TEST );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	Disable2D();
	glDisable( GL_LIGHT0 );
	glEnable( GL_LIGHT2 );

	CVector4 cLightPosShadow;
	if( CWindowData::GetInstance()->SettingFile().bShadows ) {
		// przeksztalcenia odwrotne
		glLoadIdentity();
		glRotatef( -m_fRotBackgroundY, 0.0f, 1.0f, 0.0f );
		glRotatef( -30.0f, 1.0f, 0.0f, 0.0f );
		if( m_iIndexShipToBackground == INDEX_BIG_SHIP_BOSS_01 )
			glTranslatef( 0.0f, 0.0f, -DISTANCE_BIG_SHIP_BOSS );
		else
			glTranslatef( 0.0f, 0.0f, 40.0f );

		cLightPosShadow = GetStencilShadow()->GetLightPos( GetLighting()->GetPosition( GL_LIGHT2 ) );
	}
	//przeksztalcenia normalne
	glLoadIdentity();
	if( m_iIndexShipToBackground == INDEX_BIG_SHIP_BOSS_01 )
		glTranslatef( 0.0f, 0.0f, DISTANCE_BIG_SHIP_BOSS );
	else
		glTranslatef( 0.0f, 0.0f, -40.0f );
	glRotatef( 30.0f, 1.0f, 0.0f, 0.0f );
	glRotatef( m_fRotBackgroundY, 0.0f, 1.0f, 0.0f );

	Get3DObjManager()->Draw3DObject_Lists( m_iIndexShipToBackground, GL_TRUE, GL_LIGHT1 );

	if( CWindowData::GetInstance()->SettingFile().bShadows )
		GetStencilShadow()->DrawShadowForObj( m_iIndexShipToBackground, cLightPosShadow );

	m_fRotBackgroundY += ( 10.0f * GetSpeedCtrl()->GetMultiplier() );
	if( m_fRotBackgroundY > 360.0f )
		m_fRotBackgroundY -= 360.0f;

	glDisable( GL_LIGHT2 );
	glEnable( GL_LIGHT0 );

	DrawSkyBox_List();
}

GLvoid CGameControl::DrawSkyBox()
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glTexCoordPointer( 2, GL_FLOAT, 0, m_aSkyBoxTexCoord ); 
	glVertexPointer( 3, GL_FLOAT, 0, m_aSkyBoxIndices );
	glDrawArrays( GL_QUADS, 0, 24 );
}

GLvoid CGameControl::CreateSkyBoxList()
{
	m_fSkyBoxRot = 0.0f;
	m_uiSkyBoxList = glGenLists( 1 );
	glNewList( m_uiSkyBoxList, GL_COMPILE );
		DrawSkyBox();
	glEndList();
}

GLvoid CGameControl::DeleteSkyBoxList()
{
	glDeleteLists( m_uiSkyBoxList, 1 );
}

GLvoid CGameControl::DrawSkyBox_List( GLint iTexBkgIndex /*= 0*/, GLboolean bForMenu /*= GL_TRUE*/ )
{
	/*
	iTexBkgIndex - dla menu = 0
	bForMenu = GL_FALSE to rysowane dla gry
	*/
	Disable2D();

	if( bForMenu )
		CWindowData::GetInstance()->ReSizeGLScene( 60.0f );
	else {
		if( GetRGame()->GetCameraMode() == ECameraModeNormal )
			CWindowData::GetInstance()->ReSizeGLScene( 50.0f );
	}

	glLoadIdentity();
	glDisable( GL_LIGHTING );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );

	glColor4ub( 192, 192, 192, 255 );

	if( bForMenu )
		glRotatef( m_fSkyBoxRot, 0.0f, 1.0f, 0.0f );
	else {
		GetRGame()->SetCamera();
		switch( GetRGame()->GetCameraMode() ) {
			case ECameraModeNormal:
				glRotatef( 90.0f, 0.0f, 0.0f, 1.0f );
				glRotatef( m_fSkyBoxRot, 0.0f, 1.0f, 0.0f );
				break;
			default:
				glRotatef( 45.0f, 1.0f, 0.0f, 0.0f );
				break;
		}
	}

//	glRotatef(  45.0f, 1.0f, 0.0f, 0.0f ); //<- ok
//	glRotatef( 135.0f, 1.0f, 0.0f, 0.0f ); //<- 2 i 4 zle
//	glRotatef( 225.0f, 1.0f, 0.0f, 0.0f ); //<- 4 srednio ok ale moze byc
//	glRotatef( 315.0f, 1.0f, 0.0f, 0.0f ); //<- 2 i 4 zle

	GetTexLoader()->SetTexture( m_aTexBackground[ iTexBkgIndex ] );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glCallList( m_uiSkyBoxList );

	if( bForMenu )
		m_fSkyBoxRot += ( /*1.0f **/ GetSpeedCtrl()->GetMultiplier() );
	else
		m_fSkyBoxRot += ( 0.75f * GetSpeedCtrl()->GetMultiplier() );
	if( m_fSkyBoxRot > 360.0f )
		m_fSkyBoxRot -= 360.0f;

	if( bForMenu )
		CWindowData::GetInstance()->ReSizeGLScene();
	else {
		if( GetRGame()->GetCameraMode() == ECameraModeNormal )
			CWindowData::GetInstance()->ReSizeGLScene();
	}
}

GLvoid CGameControl::DrawIntro()
{
	DrawSkyBox_List();

	glLoadIdentity();
	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
	glColor3f( 1.0f, 1.0f, 1.0f );
	if( m_iIndexShipToBackground == INDEX_BIG_SHIP_BOSS_01 )
		glTranslatef( 0.0f, 0.0f, DISTANCE_BIG_SHIP_BOSS );
	else
		glTranslatef( 0.0f, 0.0f, -40.0f );
	glRotatef( 30.0f, 1.0f, 0.0f, 0.0f );
	glRotatef( m_fRotBackgroundY, 0.0f, 1.0f, 0.0f );

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glDisable( GL_CULL_FACE );

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	if( GetSpeedCtrl()->CheckAnimationTime( 15, 0 ) ) {
		if( !m_bLineReady ) {
			m_iFrameCounter += 3;
			if( m_iFrameCounter > ( GLuint )GetVertexArrays()->GetSubIndicesSize( m_iIndexShipToBackground ) ) {
				m_iFrameCounter = 0;
				m_bLineReady = GL_TRUE;
			}
			if( !m_bLineReady ) {
				for( GLuint i = m_aArrayLine.size(); i < m_iFrameCounter; ++i ) {
					m_aArrayLine.push_back( static_cast< GLushort >( GetVertexArrays()->GetIndexFromIndices( m_iIndexShipToBackground, i ) ) );
				}
			}
		}
		else if( !m_bFillReady ) {
			m_iFrameCounter += 3;
			if( m_iFrameCounter > ( GLuint )GetVertexArrays()->GetSubIndicesSize( m_iIndexShipToBackground ) ) {
				m_iFrameCounter = 0;
				m_bFillReady = GL_TRUE;
			}
			if( !m_bFillReady ) {
				for( GLuint i = m_aArrayFill.size(); i < m_iFrameCounter; ++i ) {
					m_aArrayFill.push_back( static_cast< GLushort >( GetVertexArrays()->GetIndexFromIndices( m_iIndexShipToBackground, i ) ) );

					//usuwamy z ArrayLine
					m_aArrayLine.erase( m_aArrayLine.begin() );
				}
			}
		}
		else { //tekstury
			m_iFrameCounter += 3;
			if( m_iFrameCounter > ( GLuint )GetVertexArrays()->GetSubIndicesSize( m_iIndexShipToBackground ) ) {
				m_iFrameCounter = 0;
				m_bTextureReady = GL_TRUE;
			}
			if( !m_bTextureReady ) {
				for( GLuint i = m_aArrayTexture.size(); i < m_iFrameCounter; ++i ) {
					m_aArrayTexture.push_back( static_cast< GLushort >( GetVertexArrays()->GetIndexFromIndices( m_iIndexShipToBackground, i ) ) );

					//usuwamy z m_aArrayFill
					m_aArrayFill.erase( m_aArrayFill.begin() );
				}
			}
		}
	}
	GetVertexArrays()->EnableArraysElem();
	//rysujemy line
	if( m_aArrayLine.size() > 0 )
		glDrawElements( GL_TRIANGLES, m_aArrayLine.size(), GL_UNSIGNED_SHORT, &m_aArrayLine[ 0 ] );

	//rysujemy fill
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT2 );
	glDisable( GL_LIGHT0 );
	CVector4 cLightPos1 = CVector4( 0.0f, 3000.0f, 0.0f, 1.0f );

	if( m_aArrayFill.size() > 0 )
		glDrawElements( GL_TRIANGLES, m_aArrayFill.size(), GL_UNSIGNED_SHORT, &m_aArrayFill[ 0 ] );

	//rysujemy z texturami
	GetTexLoader()->SetTexture( Get3DObjManager()->GetTextureFirstIndex( m_iIndexShipToBackground ) );
	glEnable( GL_TEXTURE_2D );
	if( m_aArrayTexture.size() > 0 )
		glDrawElements( GL_TRIANGLES, m_aArrayTexture.size(), GL_UNSIGNED_SHORT, &m_aArrayTexture[ 0 ] );

	GetVertexArrays()->DisableClientState();

	if( m_bTextureReady ) {
		m_aArrayLine.clear();
		m_aArrayFill.clear();
		m_aArrayTexture.clear();

		BreakShowIntro();
	}
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	m_fRotBackgroundY += ( 10.0f * GetSpeedCtrl()->GetMultiplier() );
	if( m_fRotBackgroundY > 360.0f )
		m_fRotBackgroundY -= 360.0f;

	glEnable( GL_LIGHT0 );
	glDisable( GL_LIGHT2 );
}

GLvoid CGameControl::BreakShowIntro()
{
	GetRMainMenu()->SetStartMenuAnimY( GL_TRUE );
	SetEMainGameMode( EMainMenu );
	glEnable( GL_CULL_FACE );
}

GLvoid CGameControl::GetCursorPosition()
{
	::GetCursorPos( &m_ptCursor );
	if( !GetWinData()->SettingFile().bFullScreen ) {
		RECT rcWin, rcClient;
		::GetWindowRect( GetWinData()->GetHWND(), &rcWin );
		::GetClientRect( GetWinData()->GetHWND(), &rcClient );

		//jako, ¿e otrzymana pozycja kursora
		//jest wzgledem ekranu, to dla programu okienkowego trzeba pozycjê
		//kursora przekszta³ciæ tak aby by³a wzglêdem okna klienta:

		//iWidthFrame to szerokosc ramki okna, ramki tej po bokach i od do³u.
		//Obliczam d³ugoœæ okna (zawsze jest wiêksza od ustawionej rozdzielczoœci
		//bo dochodzi w³aœnie ramka) i odejmuje od niej d³ugoœæ okna klienat ( czyli X
		//rozdzielczoœci )
		//teraz mam gruboœæ dwóch ramek z lewej i prawej strony okna, aby obliczyæ
		//gruboœæ pojedynczej ramki, dzielimy przez 2
		GLint iWidthFrame = ( ( rcWin.right - rcWin.left - rcClient.right ) / 2 );
		//pozycja kursora wzglêdem okna klienta:
		m_ptCursor.x -= ( rcWin.left + iWidthFrame );
		m_ptCursor.y -= ( rcWin.top + ( ( rcWin.bottom - rcWin.top - rcClient.bottom ) - iWidthFrame ) );
	}
}

GLvoid CGameControl::DeleteLogo()
{	
	delete m_pRLogo;
	m_pRLogo = NULL;
#if _USE_OGG_
	GetOpenALManager()->DoUpdateAllStream();
	StartPlayMusicForMenu();
#endif
}

GLvoid CGameControl::Timer()
{
#if _USE_LOAD_PROGESS_BAR_
	if( !GetLoadProgressBar()->IsLoadProgressEnd() )
		return;
#endif

	GetFps()->SetTextFps();
	GetFps()->CleanFps();
	
	if( ++m_iCounterReIntroShow > 30 ) { //30 sekund bezczynnoœci
		m_iCounterReIntroShow = 0;
		if( m_eMainGameMode == EMainMenu ) { // wlacz intro jezeli jestem w menu glownym
			m_aArrayLine.clear();
			m_aArrayFill.clear();
			m_aArrayTexture.clear();
			m_bLineReady = GL_FALSE;
			m_bFillReady = GL_FALSE;
			m_bTextureReady = GL_FALSE;
			m_iIndexShipToBackground = GetShipsManager()->GetRandomEnemyShipIndex();
			GetRMainMenu()->SetStartMenuAnimY( GL_FALSE );
			SetEMainGameMode( EIntro );
		}
	}

	for( GLint i = static_cast< GLint >( g_aPointerSDLFont.size() - 1 ); i >= 0; --i ) {
		if( g_aPointerSDLFont[ i ] )
			g_aPointerSDLFont[ i ]->Timer();
	}

#if _USE_MULTIPLYER_
	switch( m_eMainGameMode ) {
		case EMultiplayerMenu:
			GetRMultiplayerMenu()->Timer();
			break;
	}
#endif

#if _USE_SOUND_OPENAL_ && _USE_OGG_
	if( m_iRenewMusicAfterDelay > -1 )
	{
		--m_iRenewMusicAfterDelay;
		if( m_iRenewMusicAfterDelay == -1 && m_eMainGameMode == ESingleGame )
			COpenALManager::GetInstance()->Play( m_aMusicGame[ m_iActualMusicGame ] );
	}
#endif

#if _USE_SOUND_OPENAL_
	COpenALManager::GetInstance()->Timer();
#endif
}

GLvoid CGameControl::Keyboard()
{
	if( m_bKeys[ VK_RETURN ] ) { //nacisnieto ENTER
		m_bKeys[ VK_RETURN ] = GL_FALSE;

		if( HIBYTE( ::GetKeyState( VK_MENU ) ) ) //kombinacja Alt+Enter
			::ShowWindow( GetWinData()->GetHWND(), SW_MINIMIZE ); //minimalizacja
		else {
			//Tutaj obsluga samego ENTERa
			switch( m_eMainGameMode ) {
				case ESingleGame:
					GetRGame()->KeyDown( VK_RETURN );
					break;
			}
		}
	}

	//obsluga klawiatury, w czasie gry, jest umieszczona tutaj aby
	//mozliwe bylo naciskanie kilku klawiszy na raz
	switch( m_eMainGameMode ) {
		case ELogo:
			if( m_bKeys[ VK_SPACE ] ) {
				m_bKeys[ VK_SPACE ] = GL_FALSE;
#if _USE_LOAD_PROGESS_BAR_
				if( !GetLoadProgressBar()->IsLoadProgressEnd() )
					return;
#endif
				m_eMainGameMode = EMainMenu;
				DeleteLogo();
				return;
			}
			break;
		case ESingleGame:
			if( m_bKeys[ VK_F1 ] ) {
				m_bKeys[ VK_F1 ] = GL_FALSE;
				GetRGame()->KeyDown( VK_F1 );
			}
			if( m_bKeys[ VK_BACK ] ) {
				m_bKeys[ VK_BACK ] = GL_FALSE;
				GetRGame()->KeyDown( VK_BACK );
			}
			if( m_bKeys[ VK_ESCAPE ] ) {
				m_bKeys[ VK_ESCAPE ] = GL_FALSE;
				GetRGame()->KeyDown( VK_ESCAPE );
			}
			if( m_bKeys[ VK_SPACE ] )
				GetRGame()->KeyDown( VK_SPACE );
			else
				GetRGame()->KeyUp( VK_SPACE );

			if( m_bKeys[ 'C' ] ) {
				m_bKeys[ 'C' ] = GL_FALSE;
				GetRGame()->KeyDown( 'C' );
			}
			if( m_bKeys[ 'P' ] ) {
				m_bKeys[ 'P' ] = GL_FALSE;
				GetRGame()->KeyDown( 'P' );
			}
			if( m_bKeys[ 'K' ] ) {
				m_bKeys[ 'K' ] = GL_FALSE;
				GetRGame()->KeyDown( 'K' );
			}
			if( m_bKeys[ '1' ] ) {
				m_bKeys[ '1' ] = GL_FALSE;
				GetRGame()->KeyDown( '1' );
			}
			if( m_bKeys[ '2' ] ) {
				m_bKeys[ '2' ] = GL_FALSE;
				GetRGame()->KeyDown( '2' );
			}
			if( m_bKeys[ '3' ] ) {
				m_bKeys[ '3' ] = GL_FALSE;
				GetRGame()->KeyDown( '3' );
			}
			if( m_bKeys[ '4' ] ) {
				m_bKeys[ '4' ] = GL_FALSE;
				GetRGame()->KeyDown( '4' );
			}
			if( m_bKeys[ '5' ] ) {
				m_bKeys[ '5' ] = GL_FALSE;
				GetRGame()->KeyDown( '5' );
			}
			if( m_bKeys[ VK_CONTROL ] && !m_bVkControl ) {
				m_bVkControl = GL_TRUE;
				m_bKeys[ VK_CONTROL ] = GL_FALSE;
				GetRGame()->KeyDown( VK_CONTROL );
			}
			break;
		default:
			if( m_bKeys[ VK_ESCAPE ] ) { //wyjœcie z programu
				m_bKeys[ VK_ESCAPE ] = GL_FALSE;
				Exit();
			}
			break;
	}

	if( m_bKeys[ 0xC0 ] ) { //~
		m_bKeys[ 0xC0 ] = GL_FALSE;
		if( GetConsole()->IsConsole() )
			GetConsole()->HideConsole();
		else 
			GetConsole()->ShowConsole();
	}

/*
	//przyklad wykorzystania funkcji CWindowData::FullscreenSwitch()
	//gdzie klawiszem F2 mamy mozliwosc przelaczania sie pomiedzy trybami
	//pelnoekranowym a okienkowym
	if( m_bKeys[ VK_F2 ] ) {
		m_bKeys[ VK_F2 ] = GL_FALSE;
		GetWinData()->FullscreenSwitch();
	}
*/
}

GLboolean CGameControl::WriteOnKeyboard( GLuint uiKey, LPTSTR lpBuffer, GLint iBufferLenMax )
{
	GLint iLength = static_cast< GLint >( _tcslen( lpBuffer ) );
	
	switch( uiKey ) {
		case VK_BACK:
			if( iLength > 0 ) --iLength;
			lpBuffer[ iLength ] = 0;
			return GL_TRUE;
		default:
			if( iLength == 0 && uiKey == VK_SPACE )
				return GL_TRUE; //pierwszym znakiem nie moze byc spacja
			if( iLength >= iBufferLenMax - 1 ) {
				if( CWindowData::GetInstance()->SettingFile().bSound )
					COpenALManager::GetInstance()->PlayNo3D( m_iSoundMouseOn );
				return GL_TRUE; //ograniczenie dlugosci
			}
			if(/*number*/( uiKey >= 0x30 && uiKey <= 0x39 )
				/*alphabet*/|| ( uiKey >= 0x41 && uiKey <= 0x5A ) || uiKey == 0x20 
				/*NUMPAD*/|| ( uiKey >= 0x60 && uiKey <= 0x6F ) 
				/*pozostale ,.;'[]*/|| ( uiKey >= 0xBA && uiKey <= 0xBF ) || ( uiKey >= 0xDB && uiKey <= 0xDE )
			) {
				lpBuffer[ iLength ] = static_cast<TCHAR>( GetKeyChar( uiKey ) );
				lpBuffer[ iLength + 1 ] = 0;
				return GL_TRUE;
			}
			break;
	}

	return GL_FALSE;
}

GLuint CGameControl::GetKeyChar( GLuint uiKeyCode )
{
	//numpad
	if( uiKeyCode == 0x60 ) return 0x30;
	if( uiKeyCode == 0x61 ) return 0x31;
	if( uiKeyCode == 0x62 ) return 0x32;
	if( uiKeyCode == 0x63 ) return 0x33;
	if( uiKeyCode == 0x64 ) return 0x34;
	if( uiKeyCode == 0x65 ) return 0x35;
	if( uiKeyCode == 0x66 ) return 0x36;
	if( uiKeyCode == 0x67 ) return 0x37;
	if( uiKeyCode == 0x68 ) return 0x38;
	if( uiKeyCode == 0x69 ) return 0x39;
	if( uiKeyCode == 0x6A ) return 0x2A; //*
	if( uiKeyCode == 0x6B ) return 0x2B; //+
	if( uiKeyCode == 0x6C ) return 0x2C; //,
	if( uiKeyCode == 0x6D ) return 0x2D; //-
	if( uiKeyCode == 0x6E ) return 0x2E; //.
	if( uiKeyCode == 0x6F ) return 0x2F; ///

	if( HIBYTE( ::GetKeyState( VK_SHIFT ) ) ) { //kombinacja Shift+...
		//Shift+numer
		if( uiKeyCode == 0x30 ) uiKeyCode = 0x29; // )
		if( uiKeyCode == 0x31 ) uiKeyCode = 0x21; //!
		if( uiKeyCode == 0x32 ) uiKeyCode = 0x40; //@
		if( uiKeyCode == 0x33 ) uiKeyCode = 0x23; //#
		if( uiKeyCode == 0x34 ) uiKeyCode = 0x24; //$
		if( uiKeyCode == 0x35 ) uiKeyCode = 0x25; //%
		if( uiKeyCode == 0x36 ) uiKeyCode = 0x5E; //^
		if( uiKeyCode == 0x37 ) uiKeyCode = 0x26; //&
		if( uiKeyCode == 0x38 ) uiKeyCode = 0x2A; //*
		if( uiKeyCode == 0x39 ) uiKeyCode = 0x28; //(
		//if( uiKeyCode >= 0x41 && uiKeyCode <= 0x5A ) uiKeyCode += 32; //alfabet jet ok bo od razu dostajemy wielkimi
		if( uiKeyCode == 0xBA ) uiKeyCode = 0x3A; // znak :
		if( uiKeyCode == 0xBB ) uiKeyCode = 0x2B; // znak +
		if( uiKeyCode == 0xBC ) uiKeyCode = 0x3C; // znak <
		if( uiKeyCode == 0xBD ) uiKeyCode = 0x5F; // znak _
		if( uiKeyCode == 0xBE ) uiKeyCode = 0x3E; // znak >
		if( uiKeyCode == 0xBF ) uiKeyCode = 0x3F; // znak ?
		if( uiKeyCode == 0xDB ) uiKeyCode = 0x7B; // znak {
		if( uiKeyCode == 0xDC ) uiKeyCode = 0x7C; // znak |
		if( uiKeyCode == 0xDD ) uiKeyCode = 0x7D; // znak }
		if( uiKeyCode == 0xDE ) uiKeyCode = 0x22; // znak "
	}
	else { //bez shifta
		if( uiKeyCode >= 0x41 && uiKeyCode <= 0x5A ) uiKeyCode += 32; //alfabet malymi literami
		if( uiKeyCode == 0xBA ) uiKeyCode = 0x3B; // znak ;
		if( uiKeyCode == 0xBB ) uiKeyCode = 0x3D; // znak =
		if( uiKeyCode == 0xBC ) uiKeyCode = 0x2C; // znak ,
		if( uiKeyCode == 0xBD ) uiKeyCode = 0x2D; // znak -
		if( uiKeyCode == 0xBE ) uiKeyCode = 0x2E; // znak .
		if( uiKeyCode == 0xBF ) uiKeyCode = 0x2F; // znak /
		if( uiKeyCode == 0xDB ) uiKeyCode = 0x5B; // znak [
		if( uiKeyCode == 0xDC ) uiKeyCode = 0x5C; // znak '\'
		if( uiKeyCode == 0xDD ) uiKeyCode = 0x5D; // znak ]
		if( uiKeyCode == 0xDE ) uiKeyCode = 0x27; // znak '
	}

	return uiKeyCode;
}

GLvoid CGameControl::StartPlayMusicForGame()
{
#if _USE_SOUND_OPENAL_ && _USE_OGG_
	GetOpenALManager()->Stop( m_aMusicMenu[ m_iActualMusicMenu ] );
	m_iActualMusicGame = rand() % GetMusicGameSize();
	if( GetWinData()->SettingFile().bMusic )
		GetOpenALManager()->Play( m_aMusicGame[ m_iActualMusicGame ] );
#endif
}

GLvoid CGameControl::StartPlayMusicForMenu()
{
#if _USE_SOUND_OPENAL_ && _USE_OGG_
	GetOpenALManager()->Stop( m_aMusicGame[ m_iActualMusicGame ] );
	m_iActualMusicMenu = rand() % GetMusicMenuSize();
	if( GetWinData()->SettingFile().bMusic )
		GetOpenALManager()->Play( m_aMusicMenu[ m_iActualMusicMenu ] );
#endif
}

GLvoid CGameControl::SetIndicesQuadAllScreen()
{
	m_aIndicesQuadAllScreen[ 0 ] = CWindowData::GetInstance()->SettingFile().iWidth;
	m_aIndicesQuadAllScreen[ 1 ] = CWindowData::GetInstance()->SettingFile().iHeight;
	m_aIndicesQuadAllScreen[ 2 ] = CWindowData::GetInstance()->SettingFile().iWidth;
	m_aIndicesQuadAllScreen[ 3 ] = 0;
	m_aIndicesQuadAllScreen[ 4 ] = 0;
	m_aIndicesQuadAllScreen[ 5 ] = CWindowData::GetInstance()->SettingFile().iHeight;
	m_aIndicesQuadAllScreen[ 6 ] = 0;
	m_aIndicesQuadAllScreen[ 7 ] = 0;
}

GLvoid CGameControl::DrawQuadOnAllScreen()
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glVertexPointer( 2, GL_INT, 0, m_aIndicesQuadAllScreen );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}