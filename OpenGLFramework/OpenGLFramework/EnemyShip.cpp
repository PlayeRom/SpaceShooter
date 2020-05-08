
#include "StdAfx.h"
#include "Rectangle.h"
#include "RGame.h"
#include "RPlayer.h"
#include "RVisualEnemyHit.h"
#include "ShipsManager.h"
#include "RBonus.h"
#include "EnemyShip.h"

CEnemyShip::CEnemyShip()
{
}

CEnemyShip::~CEnemyShip()
{
}

GLvoid CEnemyShip::Add( SEnemyShip &sEnemyShip )
{
	m_aEnemyShip.push_back( sEnemyShip );
}

GLvoid CEnemyShip::Add(	GLint i3DObjIndex,
						GLint uiPoints,
						CVector3 cActualPos,
						GLfloat fSpeed,
						GLfloat fCollisionA,
						GLfloat fCollisionB,
						GLint iShield,
						GLint iArmour,
						GLint iArmament,
						GLint iManoeuvring,
						GLboolean bCosFly,
						GLboolean bCosSign,
						GLfloat fCosIncreaseValue,
						GLboolean bSlantFly,
						GLint iFormation,
						GLint iGroup,
						GLboolean bCanManoeuvring,
						GLboolean bCanFire )
{
	SEnemyShip sEnemyShip;

	sEnemyShip.i3DObjIndex = i3DObjIndex;
	sEnemyShip.bBoss = GL_FALSE;
	sEnemyShip.uiPoints = uiPoints;
	sEnemyShip.bStart = GL_FALSE;
	sEnemyShip.bAlive = GL_TRUE;
	sEnemyShip.cActualPos = cActualPos;
	sEnemyShip.cOldPos = cActualPos;
	sEnemyShip.fSpeed = fSpeed;
	sEnemyShip.fCollisionA = fCollisionA;
	sEnemyShip.fCollisionB = fCollisionB;
	sEnemyShip.bIsManoeuvring = GL_FALSE;
	sEnemyShip.bManoeuvringSign = GL_FALSE;
	sEnemyShip.fManoeuvringRot = 0.0f;
	sEnemyShip.iShield = iShield;
	sEnemyShip.iMaxShield = iShield;
	sEnemyShip.iArmour = iArmour;
	sEnemyShip.iMaxArmour = iArmour;
	sEnemyShip.iArmament = iArmament;
	sEnemyShip.iManoeuvring = iManoeuvring;
	sEnemyShip.iAnimCtrlIndex = 0;
	sEnemyShip.bWreck = GL_FALSE;
	sEnemyShip.bCanManoeuvring = bCanManoeuvring;
	sEnemyShip.bCanFire = bCanFire;

	sEnemyShip.bCosFly = bCosFly;
	sEnemyShip.bCosSign = bCosSign;
	sEnemyShip.fCosActualAngle = 0.0f;
	sEnemyShip.fCosIncreaseValue = fCosIncreaseValue;
	sEnemyShip.fTiltAngle = 0.0f;

	sEnemyShip.bSlantFly = bSlantFly;
	if( cActualPos.x < 0.0f )
		sEnemyShip.bSlantSign = GL_TRUE;
	else if( cActualPos.x > 0.0f )
		sEnemyShip.bSlantSign = GL_FALSE;
	sEnemyShip.iFormation = iFormation;
	sEnemyShip.iGroup = iGroup;

	sEnemyShip.fRotAsteroid = CVector3( static_cast< GLfloat >( rand() % 2 ),
										static_cast< GLfloat >( rand() % 2 ),
										static_cast< GLfloat >( rand() % 2 ) );
	//jezeli wszystko po 0.0 to losuj ktory bedzie 1.0 bo nie moze byc wszedzie 0.0
	if( sEnemyShip.fRotAsteroid == 0.0f ) {
		switch( rand() % 3 ) {
			case 0:
				sEnemyShip.fRotAsteroid.x = 1.0f;
				break;
			case 1:
				sEnemyShip.fRotAsteroid.y = 1.0f;
				break;
			case 2:
				sEnemyShip.fRotAsteroid.z = 1.0f;
				break;
		}
	}

	m_aEnemyShip.push_back( sEnemyShip );
}

GLvoid CEnemyShip::AddBoss( GLint i3DObjIndex,
							GLint uiPoints,
							CVector3 cActualPos,
							GLfloat fSpeed,
							GLfloat fCollisionA,
							GLfloat fCollisionB,
							GLint iShield,
							GLint iArmour,
							GLint iArmament,
							GLint iManoeuvring,
							GLint iGroup )
{
	SEnemyShip sEnemyShip;

	sEnemyShip.i3DObjIndex = i3DObjIndex;
	sEnemyShip.bBoss = GL_TRUE;				//<--------- boss
	sEnemyShip.uiPoints = uiPoints;
	sEnemyShip.bStart = GL_FALSE;
	sEnemyShip.bAlive = GL_TRUE;
	sEnemyShip.cActualPos = cActualPos;
	sEnemyShip.cOldPos = cActualPos;
	sEnemyShip.fSpeed = fSpeed;
	sEnemyShip.fCollisionA = fCollisionA;
	sEnemyShip.fCollisionB = fCollisionB;
	sEnemyShip.bIsManoeuvring = GL_FALSE;
	sEnemyShip.bManoeuvringSign = GL_FALSE;
	sEnemyShip.fManoeuvringRot = 0.0f;
	sEnemyShip.iShield = iShield;
	sEnemyShip.iMaxShield = iShield;
	sEnemyShip.iArmour = iArmour;
	sEnemyShip.iMaxArmour = iArmour;
	sEnemyShip.iArmament = iArmament;
	sEnemyShip.iManoeuvring = iManoeuvring;
	sEnemyShip.iAnimCtrlIndex = 0;
	sEnemyShip.bWreck = GL_FALSE;
	sEnemyShip.bCanManoeuvring = GL_FALSE;
	sEnemyShip.bCanFire = GL_TRUE;

	sEnemyShip.bCosFly = GL_FALSE;
	sEnemyShip.bCosSign = GL_FALSE;
	sEnemyShip.fCosActualAngle = 0.0f;
	sEnemyShip.fCosIncreaseValue = 0.0f;
	sEnemyShip.fTiltAngle = 0.0f;

	sEnemyShip.bSlantFly = GL_FALSE;
	if( cActualPos.x < 0.0f )
		sEnemyShip.bSlantSign = GL_TRUE;
	else if( cActualPos.x > 0.0f )
		sEnemyShip.bSlantSign = GL_FALSE;
	sEnemyShip.iFormation = 1;
	sEnemyShip.iGroup = iGroup;

	sEnemyShip.fRotAsteroid = CVector3( static_cast< GLfloat >( rand() % 2 ),
										static_cast< GLfloat >( rand() % 2 ),
										static_cast< GLfloat >( rand() % 2 ) );
	//jezeli wszystko po 0.0 to losuj ktory bedzie 1.0 bo nie moze byc wszedzie 0.0
	if( sEnemyShip.fRotAsteroid == 0.0f ) {
		switch( rand() % 3 ) {
			case 0:
				sEnemyShip.fRotAsteroid.x = 1.0f;
				break;
			case 1:
				sEnemyShip.fRotAsteroid.y = 1.0f;
				break;
			case 2:
				sEnemyShip.fRotAsteroid.z = 1.0f;
				break;
		}
	}

	m_aEnemyShip.push_back( sEnemyShip );
}

GLboolean CEnemyShip::IsAliveEnemy()
{
	for( GLint i = GetEnemyShipSize() - 1; i >= 0; --i ) {
		if( m_aEnemyShip[ i ].bAlive )
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLint CEnemyShip::CheckActualGroup()
{
	for( GLint i = 0; i < GetEnemyShipSize(); ++i ) {
		if( !m_aEnemyShip[ i ].bStart )
			return m_aEnemyShip[ i ].iGroup;
	}

 	return m_aEnemyShip[ GetEnemyShipSize() - 1 ].iGroup; //zwroc ostatnia grupe
}

CRectangle CEnemyShip::GetCollisionRect( GLint iIndex )
{
	CVector2 cPtTopLeft( m_aEnemyShip[ iIndex ].cActualPos.x - m_aEnemyShip[ iIndex ].fCollisionB,
						 m_aEnemyShip[ iIndex ].cActualPos.z - m_aEnemyShip[ iIndex ].fCollisionA );
	CVector2 cPtBottomRight( m_aEnemyShip[ iIndex ].cActualPos.x + m_aEnemyShip[ iIndex ].fCollisionB,
							 m_aEnemyShip[ iIndex ].cActualPos.z + m_aEnemyShip[ iIndex ].fCollisionA );

	return CRectangle( cPtTopLeft, cPtBottomRight );
}