
#include "StdAfx.h"
#include "GameControl.h"
#include "../Lighting/Lighting.h"
#include "../Draw/3DObjManager.h"
#include "WindowData.h"
#include "Cursor.h"
#include "ShipsManager.h"
#include "RGame.h"
#include "RBonus.h"
#include "Joystick.h"
#include "RPlayer.h"

CRPlayer::CRPlayer()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_cPosition.x = 0.0f;
	m_cPosition.y = 0.0f;
	m_cPosition.z = PLAYER_ZONE_MIN;
	m_i3DObjectIndex = 1;

	SetLight1();

	m_bIsManoeuvring = GL_FALSE;
	m_fManoeuvringRot = 0.0f;
	m_fRot30 = 0.0f;
	//dla poruszania mysza
	m_iCenterMouseX = CWindowData::GetInstance()->SettingFile().iWidth / 2;
	m_iCenterMouseY = CWindowData::GetInstance()->SettingFile().iHeight / 2;

	m_bPlayerHit = GL_FALSE;
	m_iImmortality = 0;
	m_fPlayerHitPlane = 0.0f;
#ifdef DRAW_PLAYER_SHIELD_3D
	ResetDrawPlayerShield3D();
#endif
	//3 emitery czasteczek do zobrazowania, zniszczen gracza
	m_iEmitterIndex[ 0 ] = m_pGameCtrl->GetParticles()->CreateEmitter(
			//z, y, x
			CVector3( 5.0f, 0.0f, 1.0f ),
			m_pGameCtrl->m_iTexParticle01, //textura
			CVector3(),
			CVector3(),
			1.0f, //skala -> 1.5f za duzo
			0.8f, //life
			GL_FALSE, //czasteczki nie moga ozywac po wygasnieciu
			1.0f, //r
			1.0f, //g
			0.2f, //b
			0.0f, //speed x
			0.0f, //speed y
			0.0f,
			100 ); //ilosc
	m_iEmitterIndex[ 1 ] = m_pGameCtrl->GetParticles()->CreateEmitter(
			//z, y, x
			CVector3( 0.0f, 0.0f, -1.0f ),
			m_pGameCtrl->m_iTexParticle01, //textura
			CVector3(),
			CVector3(),
			1.0f, //skala
			0.8f, //life
			GL_FALSE, //czasteczki nie moga ozywac po wygasnieciu
			1.0f, //r
			0.5f, //g
			0.25f, //b
			0.0f, //speed x
			0.0f, //speed y
			0.0f,
			100 ); //ilosc
	m_iEmitterIndex[ 2 ] = m_pGameCtrl->GetParticles()->CreateEmitter(
			//z, y, x
			CVector3( -5.0f, 0.0f, 0.0f ),
			m_pGameCtrl->m_iTexParticle01, //textura
			CVector3(),
			CVector3(),
			1.0f, //skala
			0.8f, //life
			GL_FALSE, //czasteczki nie moga ozywac po wygasnieciu
			1.0f, //r
			1.0f, //g
			0.1f, //b
			0.0f, //speed x
			0.0f, //speed y
			0.0f,
			100 ); //ilosc
}

CRPlayer::~CRPlayer()
{
}

GLvoid CRPlayer::RestartObjects()
{
	SetLight1();
}

GLvoid CRPlayer::SetLight1()
{
	m_cLight1Pos = CVector4( 0.0f, 3000.0f, 0.0f, 1.0f );
	m_pGameCtrl->SetLight1();
}

GLboolean CRPlayer::MouseLButtonDown( GLint /*iX*/, GLint /*iY*/ )
{
	GLboolean bLButtonWasConsumed = GL_FALSE;

	return bLButtonWasConsumed;
}

GLboolean CRPlayer::MouseRButtonDown( GLint /*iX*/, GLint /*iY*/ )
{
	DoManoeuvring();
	return GL_TRUE;
}

GLvoid CRPlayer::DoManoeuvring()
{
	if( m_pGameCtrl->GetRGame()->IsPause() || m_bIsManoeuvring )
		return;
	//sprawdz czy obiekt gracza moze robic uniki
	if( m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).bCanManoeuvring )
		m_bIsManoeuvring = GL_TRUE;
}

GLvoid CRPlayer::RealizeManoeuvring( GLint bMode /*= 0*/ )
{
	//bMode:
	//= -1 - gdy dla cienia
	//= 1 - gdy wywolane z DrawPlayer()
	if( bMode == -1 ) {
		if( m_bIsManoeuvring )
			glRotatef( -m_fManoeuvringRot, 0.0f, 0.0f, 1.0f );
		else
			glRotatef( -m_fRot30, 0.0f, 0.0f, 1.0f );
		
		return;
	}

	if( m_bIsManoeuvring )
		glRotatef( m_fManoeuvringRot, 0.0f, 0.0f, 1.0f );
	else
		glRotatef( m_fRot30, 0.0f, 0.0f, 1.0f );
}

GLvoid CRPlayer::CalculateManoeuvring()
{
	if( m_bIsManoeuvring ) {
		if( m_fManoeuvringRot == 0.0f ) {	//pierwsza klatka obrotu
			m_fManoeuvringRot = m_fRot30;
			if( m_fRot30 > 0.0f )
				m_bManoeuvringSign = GL_TRUE;
			else
				m_bManoeuvringSign = GL_FALSE;
		}

		if( m_bManoeuvringSign )
			m_fManoeuvringRot += ( 400.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		else
			m_fManoeuvringRot -= ( 400.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );

		if( m_fManoeuvringRot > 360.0f || m_fManoeuvringRot < -360.0f )
			m_fManoeuvringRot = 0.0f;
	}
	if( m_fManoeuvringRot == 0.0f && m_bIsManoeuvring )
		m_bIsManoeuvring = GL_FALSE;
}

GLvoid CRPlayer::DrawPlayer()
{
	PROFILER_BEGIN( _T("CRPlayer::DrawPlayer()") );

	if( m_iPlayerExplode > 0 ) {
		//gracza wlasnie eksploduje wiec nie rysuj statku
		m_cPosition = CVector3( 0.0f, 0.0f, PLAYER_ZONE_MIN );
		PROFILER_END();
		return;
	}

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
	glEnable( GL_LIGHT2 );
	glDisable( GL_BLEND );
	m_pGameCtrl->Disable2D();

	if( m_pGameCtrl->GetCursor()->GetMyCursor() == CCursor::EGame )
		MouseMove();
	CalculateManoeuvring();

	if( CWindowData::GetInstance()->SettingFile().bShadows ) {
		//wykonujemy wszelkie transformacje, ale w kolejnosci odwrotnej oraz z przeciwnym znakiem 
		glLoadIdentity();
		RealizeManoeuvring(-1 );
		glRotatef( -180.0f, 0.0f, 1.0f, 0.0f );
		glTranslatef( -m_cPosition.x, -m_cPosition.y, -m_cPosition.z );
		m_pGameCtrl->GetRGame()->SetCamera();

		//obliczamy swiatlo dla cienia
		m_cLightPosShadow = m_pGameCtrl->GetStencilShadow()->GetLightPos( m_pGameCtrl->GetLighting()->GetPosition( GL_LIGHT2 ) );
	}
	// transformacje juz normalnie
	glLoadIdentity();
	m_pGameCtrl->GetRGame()->SetCamera();
	glTranslatef( m_cPosition.x, m_cPosition.y, m_cPosition.z );
	glRotatef( 180.0f, 0.0f, 1.0f, 0.0f );
	RealizeManoeuvring( 1 );

/////////////////////////////////////////////////////////////////////////////////////
	if( m_iImmortality == 0 ) {
		//gracz smiertelny - rysuj normalnie
		m_pGameCtrl->GetSpeedCtrl()->UpdateAnimationTime( 6 ); //update czasu dla trzy sekundowej niesmiertelnosci po utracie zycia
		DrawPlayerShip();
	}
	else {
		//gracz ma niesmiertelnosc na 3 sekundy
		if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 100, 6 ) ) {
			--m_iImmortality;
			DrawPlayerShip();
		}
	}
	DrawViewfinder();

	glEnable( GL_LIGHT0 );
	glDisable( GL_LIGHT2 );

#ifdef DRAW_PLAYER_SHIELD_3D
	DrawPlayerShield3D();
#endif

	//rysujemy plansze z gradientowym kwadratem gdy gracz oberwal
	DrawPlayerHit();

	PROFILER_END();
}

GLvoid CRPlayer::DrawPlayerShip()
{
	switch( m_pGameCtrl->GetRGame()->GetCameraMode() ) {
		case ECameraModeTail:
		case ECameraModeCabin:
			m_pGameCtrl->Get3DObjManager()->Draw3DObject_Lists( m_i3DObjectIndex, GL_TRUE, GL_LIGHT1 );
			break;
		default: //ECameraModeNormal
			//bez bumpa
			m_pGameCtrl->Get3DObjManager()->Draw3DObject_Lists( m_i3DObjectIndex, GL_FALSE, GL_LIGHT1 );
			break;
	}

	if( CWindowData::GetInstance()->SettingFile().bShadows )
		m_pGameCtrl->GetStencilShadow()->DrawShadowForObj( m_i3DObjectIndex, m_cLightPosShadow );

	m_pGameCtrl->Disable2D();
	//Rysujemy czasteczki z silnikow
	for( GLint i = 0; i < m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).iEmitterNumber; ++i )
		m_pGameCtrl->GetParticles()->DrawEmitter( m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).psEmitterData[ i ].iIndex );

	//rysujemy iskrzenie statku:
	//oblicz procent pancerza
	GLfloat fPercent = ( static_cast< GLfloat >( GetArmourActual() ) * 100.0f ) / static_cast< GLfloat >( GetArmourMax() );
	if( fPercent < 100.0f ) {
		//rysuj pierwszy emitter
		if( m_pGameCtrl->GetParticles()->IsAliveParticleInEmitter( m_iEmitterIndex[ 0 ] ) )
			//sa zywe czasteczki, wiec mozemy rysowac
			m_pGameCtrl->GetParticles()->DrawEmitter( m_iEmitterIndex[ 0 ] );
		else
			//wczytskie czasteczki wymarly, wiec je przywracamy do nastepnego wykorzystania
			m_pGameCtrl->GetParticles()->SetParticleInEmitterAsLive( m_iEmitterIndex[ 0 ] );
	}
	if( fPercent < 75.0f ) {
		//rysuj drugi emitter
		if( m_pGameCtrl->GetParticles()->IsAliveParticleInEmitter( m_iEmitterIndex[ 1 ] ) )
			//sa zywe czasteczki, wiec mozemy rysowac
			m_pGameCtrl->GetParticles()->DrawEmitter( m_iEmitterIndex[ 1 ] );
		else
			//wczytskie czasteczki wymarly, wiec je przywracamy do nastepnego wykorzystania
			m_pGameCtrl->GetParticles()->SetParticleInEmitterAsLive( m_iEmitterIndex[ 1 ] );
	}
	if( fPercent < 50.0f ) {
		//rysuj trzeci emitter
		if( m_pGameCtrl->GetParticles()->IsAliveParticleInEmitter( m_iEmitterIndex[ 2 ] ) )
			//sa zywe czasteczki, wiec mozemy rysowac
			m_pGameCtrl->GetParticles()->DrawEmitter( m_iEmitterIndex[ 2 ] );
		else
			//wczytskie czasteczki wymarly, wiec je przywracamy do nastepnego wykorzystania
			m_pGameCtrl->GetParticles()->SetParticleInEmitterAsLive( m_iEmitterIndex[ 2 ] );

		if( !m_pGameCtrl->GetRGame()->IsPause() ) {
			if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 16, 14 ) ) {
				CVector3 cPos = m_cPosition;
				cPos.z += 20.0f;
				m_pGameCtrl->GetRGame()->GetUnivBillboard()->AddSmokePlayerDamage( cPos );
				cPos.z -= 13.0f; //-15.0 original
				m_pGameCtrl->GetRGame()->GetUnivBillboard()->AddSmokePlayerDamage( cPos );
			}
		}
	}
}

GLvoid CRPlayer::DrawViewfinder()
{
	///////////////// celownik - kwadraty
/*	
	//UWAGA! z glLoadIdentity() celownk zle sie przesuwa i nie obraca sie wraz ze statkiem
	glLoadIdentity();
	m_pGameCtrl->GetRGame()->SetCamera();
	glTranslatef( m_cPosition.x, m_cPosition.y, m_cPosition.z );
	glRotatef( 180.0f, 0.0f, 1.0f, 0.0f );*/
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
	GLint iIndexColor = CWindowData::GetInstance()->SettingFile().iHudColor;
	glColor3f( m_pGameCtrl->GetRGame()->GetHudColor( iIndexColor ).x,
			   m_pGameCtrl->GetRGame()->GetHudColor( iIndexColor ).y,
			   m_pGameCtrl->GetRGame()->GetHudColor( iIndexColor ).z );
	glRotatef( 45.0f, 0.0f, 0.0f, 1.0f );

	m_fEngageDistance = 30;
	glTranslatef( 0.0f, 0.0f, m_fEngageDistance );

	GLfloat fLength = 3.0f;
	GLint iNumberSquare = GetNumberViewFinder() + m_pGameCtrl->GetRGame()->GetRBonus()->GetNumberViewFinder(); 
	for( GLint i = 0; i < iNumberSquare; ++i ) {
		GLfloat fDist = static_cast< GLfloat >( i + 1 ) * 10.0f;
		glTranslatef( 0.0f, 0.0f, fDist );
		m_fEngageDistance += fDist;
		glBegin( GL_LINE_LOOP );
			glVertex3f( -fLength,  fLength, 0.0f );
			glVertex3f(  fLength,  fLength, 0.0f );
			glVertex3f(  fLength, -fLength, 0.0f );
			glVertex3f( -fLength, -fLength, 0.0f );
		glEnd();
	}
	m_pGameCtrl->GetRGame()->CheckEngage();

	/////////////// linie pomocnicze zasiegu dzialania
	if( m_pGameCtrl->GetRGame()->IsDrawPlayerZone()
		|| m_pGameCtrl->GetRGame()->GetRBonus()->GetNumberViewFinder() >= 5 )
	{
		DrawZone( PLAYER_ZONE_RIGHT_X, PLAYER_ZONE_LEFT_X, PLAYER_ZONE_MAX, PLAYER_ZONE_MIN );
		//DrawZone( BOSS_ZONE_RIGHT_X, BOSS_ZONE_LEFT_X, BOSS_ZONE_MAX, BOSS_ZONE_MIN );
	}
}

GLvoid CRPlayer::DrawZone( GLfloat fRightX, GLfloat fLeftX, GLfloat fMaxZ, GLfloat fMinZ )
{
	glLoadIdentity();
	m_pGameCtrl->GetRGame()->SetCamera();

	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	GLint iIndexColor = CWindowData::GetInstance()->SettingFile().iHudColor;
	glColor3f( m_pGameCtrl->GetRGame()->GetHudColor( iIndexColor ).x,
			   m_pGameCtrl->GetRGame()->GetHudColor( iIndexColor ).y,
			   m_pGameCtrl->GetRGame()->GetHudColor( iIndexColor ).z );

	glBegin( GL_LINES );
		glVertex3f( fLeftX, 0.0f, fMinZ );
		glVertex3f( fLeftX, 0.0f, fMaxZ );

		glVertex3f( fRightX, 0.0f, fMinZ );
		glVertex3f( fRightX, 0.0f, fMaxZ );

		glVertex3f( fLeftX, 0.0f, fMaxZ );
		glVertex3f( fRightX, 0.0f, fMaxZ );
	glEnd();
}

#ifdef DRAW_PLAYER_SHIELD_3D
GLvoid CRPlayer::DrawPlayerShield3D()
{
	if( GetShieldActual() <= 0 || !m_bDrawShield3D ) { //czy ma oslone
		SetDrawPlayerShield3D( GL_FALSE );
		return;
	}

	glLoadIdentity();
	m_pGameCtrl->GetRGame()->SetCamera();

	//aby oslona byla widoczna z kamera "z kabiny"
	if( m_pGameCtrl->GetRGame()->GetCameraMode() == ECameraModeCabin )
		glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	m_pGameCtrl->Disable2D();

	glColor4ub( 255, 255, 255, 255 );

	glTranslatef( m_cPosition.x, m_cPosition.y, m_cPosition.z );
	glRotatef( 180.0f, 0.0f, 1.0f, 0.0f );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glRotatef( -m_fShieldRotate, 1.0f, 0.0f, 0.0f );
	m_pGameCtrl->Get3DObjManager()->Draw3DObject_Lists( SHIELD_3DOBJ_INDEX, GL_FALSE );
	m_fShieldRotate += ( 400.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	if( m_fShieldRotate >= 360.0f )
		SetDrawPlayerShield3D( GL_FALSE );

	if( m_pGameCtrl->GetRGame()->GetCameraMode() == ECameraModeCabin )
		glEnable( GL_CULL_FACE );
}
#endif

GLvoid CRPlayer::SetPlayerHit()
{
	if( m_iShieldActual > 0 ) {//gracz obrwal w oslony
		if( CWindowData::GetInstance()->SettingFile().bSound )
			COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundShieldHit );
	}
	else {//gracz oberwal w pancerz
		if( CWindowData::GetInstance()->SettingFile().bSound )
			COpenALManager::GetInstance()->PlayNo3D( m_pGameCtrl->m_iSoundArmourHit );
	}
	//wylacz dzwiek radosci w razie gdyby byly
	COpenALManager::GetInstance()->Stop( m_pGameCtrl->m_iSoundJoy );

	m_fPlayerHitPlane = 0.0f;
	m_bPlayerHit = GL_TRUE;
}

//rysujemy plansze z gradientowym kwadratem gdy gracz oberwal
GLvoid CRPlayer::DrawPlayerHit()
{
	if( m_bPlayerHit ) {
		if( m_iShieldActual > 0 )	//gracz oberwal w oslony
			m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexPlayerHitShield );
		else	//gracz oberwal w pancerz
			m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexPlayerHitArmour );

		glPushMatrix();
		glLoadIdentity();
		m_pGameCtrl->Enable2D();
		glDisable( GL_LIGHTING );
		glDisable( GL_DEPTH_TEST );

		glColor4ub( 255, 255, 255, 255 );

		//rysuje plansze
		glBegin( GL_TRIANGLE_STRIP );
			glTexCoord2f( 1.0f - m_fPlayerHitPlane, 0.0f + m_fPlayerHitPlane );
				glVertex2i( CWindowData::GetInstance()->SettingFile().iWidth, CWindowData::GetInstance()->SettingFile().iHeight );
			glTexCoord2f( 1.0f - m_fPlayerHitPlane, 1.0f - m_fPlayerHitPlane );
				glVertex2i( CWindowData::GetInstance()->SettingFile().iWidth, 0 );
			glTexCoord2f( 0.0f + m_fPlayerHitPlane, 0.0f + m_fPlayerHitPlane );
				glVertex2i( 0, CWindowData::GetInstance()->SettingFile().iHeight );
			glTexCoord2f( 0.0f + m_fPlayerHitPlane, 1.0f - m_fPlayerHitPlane );
				glVertex2i( 0, 0 );
		glEnd();

		m_fPlayerHitPlane += m_pGameCtrl->GetSpeedCtrl()->GetMultiplier();

		m_pGameCtrl->Disable2D();
		glPopMatrix();

		if( m_pGameCtrl->GetSpeedCtrl()->CheckAnimationTime( 500, 4, GL_FALSE ) )
			m_bPlayerHit = GL_FALSE;
	}
	else
		m_pGameCtrl->GetSpeedCtrl()->UpdateAnimationTime( 4 );
}

//tutaj ustawiamy rozne wartosci gdy gracz straci zycie
GLvoid CRPlayer::PlayerLoseLive()
{
	LivesMinus1();
	SetImmortality();

	m_fManoeuvringRot = 0.0f;
	m_fRot30 = 0.0f;
	m_bIsManoeuvring = GL_FALSE;
	m_bShakePlayer = GL_FALSE;
}

GLvoid CRPlayer::MouseMove()
{
	if( m_pGameCtrl->GetRGame()->IsPause() )
		return;

	RealizeShakePlayer();

	::GetCursorPos( &m_ptMouse );

	// ruch prawo/lewo
	GLfloat fDeltaMouse = GLfloat( m_ptMouse.x - m_iCenterMouseX );
	fDeltaMouse = fDeltaMouse * 0.25f * m_fShakeFade;
	m_cPosition.x += fDeltaMouse;
	if( m_cPosition.x > PLAYER_ZONE_RIGHT_X )
		m_cPosition.x = PLAYER_ZONE_RIGHT_X;
	else if( m_cPosition.x < PLAYER_ZONE_LEFT_X )
		m_cPosition.x = PLAYER_ZONE_LEFT_X;

	//poglebsz przechyl
	m_fRot30 += ( 0.1f * fDeltaMouse );
	if( m_fRot30 > 30.0f )
		m_fRot30 = 30.0f;
	else if( m_fRot30 < -30.0f )
		m_fRot30 = -30.0f;

	if( fDeltaMouse == 0.0f ) {
		//daz do m_fRot30 = 0.0
		if( m_fRot30 > 0.0f ) m_fRot30 -= ( 100.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
		if( m_fRot30 < 0.0f ) m_fRot30 += ( 100.0f * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	}

	//ruch do przod/tylu
	fDeltaMouse = GLfloat( m_ptMouse.y - m_iCenterMouseY );
	fDeltaMouse = fDeltaMouse * 0.25f * m_fShakeFade;
	m_cPosition.z += fDeltaMouse;
	if( m_cPosition.z > PLAYER_ZONE_MIN )
		m_cPosition.z = PLAYER_ZONE_MIN;
	else if( m_cPosition.z < PLAYER_ZONE_MAX )
		m_cPosition.z = PLAYER_ZONE_MAX;

	if( fDeltaMouse < 0.0f ) {	//zwiekszenie ilosci czasteczek *3
		for( GLint i = 0; i < m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).iEmitterNumber; ++i )
			m_pGameCtrl->GetParticles()->SetMaxParticlesForEmitter(
				m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).psEmitterData[ i ].iIndex,
				m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).psEmitterData[ i ].iParticlesNumber * 3 );
	}
	else if( fDeltaMouse > 0.0f ) {	//zmniejszenie ilosci czasteczek /3
		for( GLint i = 0; i < m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).iEmitterNumber; ++i )
			m_pGameCtrl->GetParticles()->SetMaxParticlesForEmitter(
				m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).psEmitterData[ i ].iIndex,
				m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).psEmitterData[ i ].iParticlesNumber / 3 );
	}
	else {
		for( GLint i = 0; i < m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).iEmitterNumber; ++i )
			m_pGameCtrl->GetParticles()->SetMaxParticlesForEmitter(
				m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).psEmitterData[ i ].iIndex,
				m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_i3DObjectIndex ).psEmitterData[ i ].iParticlesNumber );
	}

	::SetCursorPos( m_iCenterMouseX, m_iCenterMouseY );
}

GLvoid CRPlayer::SetCursorPosAfterPause()
{
	::SetCursorPos( m_ptMouse.x, m_ptMouse.y );
}

GLvoid CRPlayer::DoShakePlayer( GLboolean bSlantFly, GLboolean bSlantSign )
{
	//Zneczenie takie same jak dla SEnemyShip:
	//bSlantFly - czy leci po skosie
	//bSlantSign - gdy 1(+) to leci z lewej strony na prawa
	
	m_bShakePlayer = GL_TRUE;
	m_fShakeFade = 0.5f;
	m_cShakeVector = CVector3();

	if( bSlantFly ) {
		if( bSlantSign )
			m_cShakeVector.x = 10.0f;
		else
			m_cShakeVector.x = -10.0f;
		m_cShakeVector.z = 10.0f;
	}
	else
		m_cShakeVector.z = 10.0f;

	m_cShakeVector.Normalize();
	m_cShakeVector *= 2.5f;
}

GLvoid CRPlayer::RealizeShakePlayer()
{
	if( !m_bShakePlayer )
		return;

	m_cPosition += m_cShakeVector;

	if( m_cPosition.x < PLAYER_ZONE_LEFT_X )
		m_cPosition.x = PLAYER_ZONE_LEFT_X;
	if( m_cPosition.x > PLAYER_ZONE_RIGHT_X )
		m_cPosition.x = PLAYER_ZONE_RIGHT_X;
	if( m_cPosition.z > PLAYER_ZONE_MIN )
		m_cPosition.z = PLAYER_ZONE_MIN;

	m_fShakeFade += m_pGameCtrl->GetSpeedCtrl()->GetMultiplier();
	if( m_fShakeFade >= 0.75f ) {
		m_bShakePlayer = GL_FALSE;
		m_fShakeFade = 1.0f;
	}
}

CRectangle CRPlayer::GetCollisionRect()
{
	CVector2 cPtTopLeft( GetPosition().x - GetCollisionB(), GetPosition().z - GetCollisionA() );
	CVector2 cPtBottomRight( GetPosition().x + GetCollisionB(), GetPosition().z + GetCollisionA() );
	return CRectangle( cPtTopLeft, cPtBottomRight );
}

GLboolean CRPlayer::IsCompleteStars()
{
	return ( m_iStarsShield == 3
			&& m_iStarsArmour == 3
			&& m_iStarsArmament == 3
			&& m_iStarsManoeuvring == 3 );
}