#include "StdAfx.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "ShipsManager.h"
#include "RGame.h"
#include "RPlayer.h"
#include "RHelp.h"

CRHelp::CRHelp()
{
	m_pGameCtrl = CGameControl::GetInstance();
	m_pFont = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 1 ), 18 );

	m_iFontMoveX = static_cast< GLint >( 10.24f * 5.0f );
	m_iFontMoveY = static_cast< GLint >( 7.68f * 3.0f );

	SetRegions();
}

CRHelp::~CRHelp()
{
	delete m_pFont;
}

GLvoid CRHelp::SetRegions()
{
	GLfloat fPercentX = 5.0f;
	GLfloat fPercentY = 14.0f;
	for( GLint i = 0; i < MAX_REGION_BONUSES; ++i ) {
		SetRegionX( m_ptBonuses[ i ].x, fPercentX );
		SetRegionY( m_ptBonuses[ i ].y, fPercentY );
		fPercentY += 8.0f;
	}
}

GLvoid CRHelp::RestartObjects()
{
	SetRegions();
}

GLvoid CRHelp::Draw()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	m_pGameCtrl->Enable2D();

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glColor4ub( 12, 18, 12, 200 );
	glEnable( GL_BLEND );
	//rysuj plansze na caly ekran
	m_pGameCtrl->DrawQuadOnAllScreen();
	glDisable( GL_BLEND );
	glColor4ub( 255, 255, 255, 255 );

	//sysuj znajdzki
	m_pFont->DrawText( m_ptBonuses[ 0 ].x, static_cast< GLint >( 7.68f * 5.0f ) + m_iFontMoveY, RGB( 255, 255, 255 ), LNG_LINE( 89 ), GL_FALSE ); 
	m_pGameCtrl->Enable2D();
	GLfloat fLength = 20.0f;
	for( GLint i = 0; i < MAX_REGION_BONUSES; ++i ) {
		//TODO: ustaw odpowiednia teksture wg typu znajdzki
		//m_pGameCtrl->GetTexLoader()->SetTexture( 0 );
		if( i >= 0 && i <= 4 ) {
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		}
		else
			glDisable( GL_BLEND );
		switch( i ) {
			case 0://EBonusWeaponLaser:
				m_pGameCtrl->GetTexLoader()->SetTexture( m_pGameCtrl->m_aTexWeaponSelect[ i ] );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); 
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				break;
			case 1://EBonusWeaponChaingun:
				m_pGameCtrl->GetTexLoader()->SetTexture( m_pGameCtrl->m_aTexWeaponSelect[ i ] );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); 
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				break;
			case 2://EBonusWeaponImpulseLaser:
				m_pGameCtrl->GetTexLoader()->SetTexture( m_pGameCtrl->m_aTexWeaponSelect[ i ] );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); 
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				break;
			case 3://EBonusWeaponEmitterRockets:
				m_pGameCtrl->GetTexLoader()->SetTexture( m_pGameCtrl->m_aTexWeaponSelect[ i ] );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); 
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				break;
			case 4://EBonusWeaponHomungRockets:
				m_pGameCtrl->GetTexLoader()->SetTexture( m_pGameCtrl->m_aTexWeaponSelect[ i ] );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); 
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				break;
			case 5://EBonusLife:
				m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->GetShipsManager()->GetUniqueShip( m_pGameCtrl->GetRGame()->GetPlayer()->Get3DObjectIndex() ).iTexLife );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); 
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				break;
			case 6://EBonusShield:
				m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexBonusShield );
				break;
			case 7://EBonusViewFinder:
				m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexBonusViewfinder );
				break;
			case 8://EBonusIndestructibility:
				m_pGameCtrl->GetTexLoader()->SetTexture( 0 );
				break;
			case 9://EBonusSlowdownEnemy:
				m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexBonusEnemySlowdown );
				break;
			case 10://EBonusRemoteRocket:
				m_pGameCtrl->GetTexLoader()->SetMultiTextures( m_pGameCtrl->m_iTexBonusRemoteRocket );
				break;
		}
		glPushMatrix();
		glTranslatef( static_cast< GLfloat >( m_ptBonuses[ i ].x ), static_cast< GLfloat >( m_ptBonuses[ i ].y ), 0.0f );
		glBegin( GL_TRIANGLE_STRIP );
			glTexCoord2f( 1.0f, 0.0f ); glVertex2f( fLength,  fLength );
			glTexCoord2f( 1.0f, 1.0f ); glVertex2f( fLength, -fLength );
			glTexCoord2f( 0.0f, 0.0f ); glVertex2f(-fLength,  fLength );
			glTexCoord2f( 0.0f, 1.0f ); glVertex2f(-fLength, -fLength );
		glEnd();
		glPopMatrix();

		m_pFont->DrawText( m_ptBonuses[ i ].x + m_iFontMoveX, static_cast< GLint >( 7.68f * static_cast< GLfloat >( 13 + ( i * 8 ) ) ) + m_iFontMoveY, RGB( 255, 255, 255 ), LNG_LINE( i + 90 ), GL_FALSE ); 
		m_pGameCtrl->Enable2D();
	}

	for( GLint i = 0; i < KEYBOARD_NUMBER; ++i )
		m_pFont->DrawText( static_cast< GLint >( 10.24f * 50.0f ), static_cast< GLint >( 7.68f * static_cast< GLfloat >( 5 + ( i * 8 ) ) ) + m_iFontMoveY, RGB( 255, 255, 255 ), LNG_LINE( i + 106 ), GL_FALSE ); 
}