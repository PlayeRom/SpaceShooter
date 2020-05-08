#include "StdAfx.h"
#include "GameControl.h"
#include "REnemyShotAnimCtrl.h"

CREnemyShotAnimCtrl::CREnemyShotAnimCtrl()
{
	for( GLint i = 0; i < MAX_ENEMY_SHIPS_ON_SCREEN; ++i ) {
		SEnemyShotAnimCtrl sEnemyShotAnim;
		sEnemyShotAnim.bIsFree = GL_TRUE;
		sEnemyShotAnim.iIndex = CGameControl::GetInstance()->GetSpeedCtrl()->CreateAnimationControl();
		m_aEnemyShotAnimCtrl.push_back( sEnemyShotAnim );
	}
}

CREnemyShotAnimCtrl::~CREnemyShotAnimCtrl()
{
	m_aEnemyShotAnimCtrl.clear();
}

GLvoid CREnemyShotAnimCtrl::SetAllOnFree()
{
	for( GLint i = GetEnemyShotAnimCtrlSize() - 1; i >= 0; --i )
		m_aEnemyShotAnimCtrl[ i ].bIsFree = GL_TRUE;
}

GLint CREnemyShotAnimCtrl::GetFreeIndex()
{
	for( GLint i = GetEnemyShotAnimCtrlSize() - 1; i >= 0; --i ) {
		if( m_aEnemyShotAnimCtrl[ i ].bIsFree ) {
			m_aEnemyShotAnimCtrl[ i ].bIsFree = GL_FALSE;
			return m_aEnemyShotAnimCtrl[ i ].iIndex;
		}
	}
	return -1;
}

GLvoid CREnemyShotAnimCtrl::FreeAnimCtrlForEnemyShot( SEnemyShip &sEnemyShip )
{
	for( GLint i = GetEnemyShotAnimCtrlSize() - 1; i >= 0; --i ) {
		if( m_aEnemyShotAnimCtrl[ i ].iIndex == sEnemyShip.iAnimCtrlIndex ) {
			m_aEnemyShotAnimCtrl[ i ].bIsFree = GL_TRUE;
			break;
		}
	}
}