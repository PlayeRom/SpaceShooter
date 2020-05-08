
#include "StdAfx.h"
#include "../Fonts/SDLFont.h"
#include "RStats.h"

#define LEFT_STATS_EDGE 25
#define RIGHT_STATS_EDGE 75

CRStats::CRStats()
{
	m_pSDLFont = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 16 );
	m_pSDLFontItems = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 1 ), 14 );

	ClearStats();
	SetMouseRegion();

	m_iPosX = static_cast< GLint >( 10.24f * static_cast< GLfloat >( LEFT_STATS_EDGE + 6 ) );
}

CRStats::~CRStats()
{
	delete m_pSDLFont;
	delete m_pSDLFontItems;
}

GLvoid CRStats::ClearStats()
{
	m_uiShootDownObjects = 0;
	m_uiMissedObjects = 0;
	for( GLint i = 0; i < MAX_WEAPON_STATS; ++i )
		m_uiUsedWeaponSec[ i ] = 0;
	m_iTheBestMultiKill = 0;
}

GLvoid CRStats::SetMouseRegion()
{
	// podklad na statsy
	SetRegionX( m_rRegions[ 0 ].left, LEFT_STATS_EDGE );
	SetRegionX( m_rRegions[ 0 ].right, RIGHT_STATS_EDGE );
	SetRegionY( m_rRegions[ 0 ].top, 5 );
	SetRegionY( m_rRegions[ 0 ].bottom, 25 );

	// podklad na "enter your name"
	SetRegionX( m_rRegions[ 1 ].left, LEFT_STATS_EDGE - 12 );
	SetRegionX( m_rRegions[ 1 ].right, RIGHT_STATS_EDGE + 12 );
	SetRegionY( m_rRegions[ 1 ].top, 27 );
	SetRegionY( m_rRegions[ 1 ].bottom, 75 );
}

GLvoid CRStats::DrawStats()
{
	glLoadIdentity();
	glDisable( GL_LIGHTING );

	DrawRegions( m_rRegions, MAX_REGIONS_STATS);

	GLint iTextLine = 158;
	GLfloat fPosY = 10.0f;
	m_pSDLFont->DrawText( static_cast< GLint >( 10.24f * static_cast< GLfloat >( LEFT_STATS_EDGE + 3 ) ),
						  static_cast< GLint >( 7.68f * fPosY ),
						  RGB( 255, 255, 255 ), LNG_LINE( iTextLine ) );

	fPosY += 4;
	m_pSDLFontItems->DrawTextFormat( m_iPosX, static_cast< GLint >( 7.68f * fPosY ), GL_FALSE, RGB( 255, 255, 255 ),
									 LNG_LINE( ++iTextLine ), m_uiShootDownObjects );

	fPosY += 3;
	m_pSDLFontItems->DrawTextFormat( m_iPosX, static_cast< GLint >( 7.68f * fPosY ), GL_FALSE, RGB( 255, 255, 255 ),
									 LNG_LINE( ++iTextLine ), m_uiMissedObjects );

	fPosY += 3;
	m_pSDLFontItems->DrawTextFormat( m_iPosX, static_cast< GLint >( 7.68f * fPosY ), GL_FALSE, RGB( 255, 255, 255 ),
									 LNG_LINE( ++iTextLine ), m_iTheBestMultiKill );

	LPTSTR lpWeaponName = _T("Laser");
	switch( GetWeaponType() ) {
		case EWeaponChaingun:
			lpWeaponName = _T("Chaingun");
			break;

		case EWeaponImpulseLaser:
			lpWeaponName = _T("Impulse laser");
			break;

		case EWeaponEmitterRocket:
			lpWeaponName = _T("Emitter rockets");
			break;

		case EWeaponHomingRocket:
			lpWeaponName = _T("Homing rockets");
			break;

		case EWeaponRemoteRocket:
			lpWeaponName = _T("Remote rockets");
			break;
	}
	fPosY += 3;
	m_pSDLFontItems->DrawTextFormat( m_iPosX, static_cast< GLint >( 7.68f * fPosY ), GL_FALSE, RGB( 255, 255, 255 ),
									 LNG_LINE( ++iTextLine ), lpWeaponName );
}

GLvoid CRStats::RestartObjects()
{
	SetMouseRegion();
}

EWeaponType CRStats::GetWeaponType()
{
	GLuint uiBiggerValue = 0;
	GLint iBiggerValueIndex = 0;
	for( GLint i = 0; i < MAX_WEAPON_STATS; ++i ) {
		if( m_uiUsedWeaponSec[ i ] > uiBiggerValue ) {
			uiBiggerValue = m_uiUsedWeaponSec[ i ];
			iBiggerValueIndex = i;
		}
	}

	if( iBiggerValueIndex == EWeaponLastElement )
		return EWeaponRemoteRocket;

	return static_cast<EWeaponType>( iBiggerValueIndex );
}