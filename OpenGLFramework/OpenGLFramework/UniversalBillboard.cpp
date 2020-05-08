#include "StdAfx.h"
#include "GameControl.h"
#include "RPlayer.h"
#include "RGame.h"
#include "RBonus.h"
#include "UniversalBillboard.h"

GLint QSortCompareUniversalBillboard( const GLvoid *arg1, const GLvoid *arg2 );

CUniversalBillboard::CUniversalBillboard()
{
	m_pGameCtrl = CGameControl::GetInstance();

	//MAX_ENEMY_SHIPS_ON_SCREEN * 2 - 2 razy wiecej niz wrogich statkow gdyz animacja
	//explozji bedzie sie rysowala dluzej
	SUniversalBillboard sExplUniBillboard;
	sExplUniBillboard.eType = EUBTypeExplosion;
	sExplUniBillboard.cPos = CVector3();
	sExplUniBillboard.sExpl.iActualAnimFrame = -1; // nie rysuj
	sExplUniBillboard.sExpl.fScale = 1.0f;
	GLint iRand = m_pGameCtrl->GetTexExplosionSize();
	for( GLint i = 0; i < MAX_ENEMY_SHIPS_ON_SCREEN * 2; ++i ) {
		sExplUniBillboard.sExpl.iTexture = rand() % iRand + m_pGameCtrl->m_aTexExplosion[ 0 ]; //losowo od m_iTexExplosion01 do m_iTexExplosion01+ iRand
		sExplUniBillboard.sExpl.iAnimCtrlIndex = m_pGameCtrl->GetSpeedCtrl()->CreateAnimationControl();
		m_aUniversalBillboard.push_back( sExplUniBillboard );
	}

	//inicujalizujemy tablice dla dymu zdalnej rakiety
	//trzeba uwzglednic ze moze pozostac na planszy smuga dymu z poprzednio
	//odpalonej rakiety, wiec tablica musi byc dosc duza
	SUniversalBillboard sSmokeRemoteRocket;
	sSmokeRemoteRocket.eType = EUBTypeSmokeRemoteRocket;
	sSmokeRemoteRocket.cPos = CVector3();
	sSmokeRemoteRocket.bIsDrawSmokeRR = GL_FALSE;
	for( GLint i = 0; i < 2048; ++i )
		m_aUniversalBillboard.push_back( sSmokeRemoteRocket );
	
	//to samo dla smokePlayerDamage
	SUniversalBillboard sSmokePlayerDamage;
	sSmokePlayerDamage.eType = EUBTypeSmokePlayerDamage;
	sSmokePlayerDamage.cPos = CVector3();
	sSmokePlayerDamage.bIsDrawSmokePD = GL_FALSE;
	for( GLint i = 0; i < 64; ++i )
		m_aUniversalBillboard.push_back( sSmokePlayerDamage );
	

#ifndef DRAW_PLAYER_SHIELD_3D
	//dodajemy oslone do m_aUniversalBillboard
	SUniversalBillboard sPlayerShield;
	sPlayerShield.eType = EUBTypePlayerShield;
	sPlayerShield.cPos = CVector3();
	sPlayerShield.sPlayerShield.fScale = 40.0f;
	sPlayerShield.sPlayerShield.iActualAnimFrame = -1; //nie rysuj
	sPlayerShield.sPlayerShield.iAnimCtrlIndex = m_pGameCtrl->GetSpeedCtrl()->CreateAnimationControl();
	sPlayerShield.sPlayerShield.iTexture = m_pGameCtrl->m_iTexShield2D;
	m_aUniversalBillboard.push_back( sPlayerShield );
#endif
}

CUniversalBillboard::~CUniversalBillboard()
{
	m_aUniversalBillboard.clear();
}

GLvoid CUniversalBillboard::SetForStart()
{
		//ustaw dym na niekatywny
	for( GLint i = GetUniversalBillboardSize() - 1; i >= 0; --i ) {
		if( m_aUniversalBillboard[ i ].eType == EUBTypeSmokeRemoteRocket )
			m_aUniversalBillboard[ i ].bIsDrawSmokeRR = GL_FALSE;
		if( m_aUniversalBillboard[ i ].eType == EUBTypeSmokePlayerDamage )
			m_aUniversalBillboard[ i ].bIsDrawSmokePD = GL_FALSE;
#ifndef DRAW_PLAYER_SHIELD_3D
		//ustaw oslone gracza na nieaktywna
		if( m_aUniversalBillboard[ i ].eType == EUBTypePlayerShield )
			m_aUniversalBillboard[ i ].sPlayerShield.iActualAnimFrame = -1;
#endif
		//ustaw explozie na nieaktywne
		if( m_aUniversalBillboard[ i ].eType == EUBTypeExplosion )
			m_aUniversalBillboard[ i ].sExpl.iActualAnimFrame = -1;
	}

	for( GLint i = GetUniversalBillboardSize() - 1; i >= 0; --i ) {
		//usun bonusy
		if( m_aUniversalBillboard[ i ].eType == EUBTypeBonus )
			m_aUniversalBillboard.erase( m_aUniversalBillboard.begin() + i );
		//usun pociski
		if( m_aUniversalBillboard[ i ].eType == EUBTypePlayerShot || m_aUniversalBillboard[ i ].eType == EUBTypeEnemyShot )
			m_aUniversalBillboard.erase( m_aUniversalBillboard.begin() + i );
	}
}

GLint QSortCompareUniversalBillboard( const GLvoid *arg1, const GLvoid *arg2 )
{
	SUniversalBillboard *s1 = ( SUniversalBillboard* )arg1;
	SUniversalBillboard *s2 = ( SUniversalBillboard* )arg2;
	if( s1->cPos.z < s2->cPos.z ) return 1;
	if( s1->cPos.z > s2->cPos.z ) return -1;
	return 0;
}

GLvoid CUniversalBillboard::SortUniversalBillboard()
{
	qsort( &m_aUniversalBillboard[ 0 ], m_aUniversalBillboard.size(), sizeof( SUniversalBillboard ), QSortCompareUniversalBillboard );
}

GLvoid CUniversalBillboard::DrawUniversalBillboard()
{
	PROFILER_BEGIN( _T("CRGame::DrawUniversalBillboard()") );

	SortUniversalBillboard();

	glLoadIdentity();
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	m_pGameCtrl->Disable2D();

	m_pGameCtrl->GetRGame()->SetCamera();

	EUniversalBillboardType eLastType = EUBTypeNone;
	EUniversalBillboardType eTempType = EUBTypeNone;

	for( GLint i = GetUniversalBillboardSize() - 1; i >= 0; --i ) {
		eTempType = m_aUniversalBillboard[ i ].eType;
		switch( eTempType ) {
			case EUBTypeExplosion:
				if( eLastType != m_aUniversalBillboard[ i ].eType ) {
					glEnable( GL_BLEND );
					glBlendFunc( GL_SRC_ALPHA, GL_ONE );
					glColor4f( 1.0f, 1.0f, 1.0f, 1.0f ); //bialy
				}
				m_pGameCtrl->GetRGame()->DrawExplosion( m_aUniversalBillboard[ i ] );
				eLastType = eTempType;
				break;
			case EUBTypeSmokeRemoteRocket:
				if( m_aUniversalBillboard[ i ].bIsDrawSmokeRR ) {
					if( eLastType != m_aUniversalBillboard[ i ].eType ) {
						glEnable( GL_BLEND );
						glBlendFunc( GL_SRC_ALPHA, GL_ONE );
						glColor4f( 0.5f, 0.5f, 0.5f, 0.3f ); //szary
						m_pGameCtrl->GetTexLoader()->SetTexture( m_pGameCtrl->m_iTexParticle01 );
					}
					DrawSmokeRemoteRocket( m_aUniversalBillboard[ i ] );
					eLastType = eTempType;
				}
				else
					eLastType = EUBTypeNone;
				break;
			case EUBTypeSmokePlayerDamage:
				if( m_aUniversalBillboard[ i ].bIsDrawSmokePD ) {
					if( eLastType != m_aUniversalBillboard[ i ].eType ) {
						glEnable( GL_BLEND );
						glBlendFunc( GL_SRC_ALPHA, GL_ONE );
						glColor4f( 0.1f, 0.1f, 0.1f,	1.0f ); //szary
						m_pGameCtrl->GetTexLoader()->SetTexture( m_pGameCtrl->m_iTexSmoke00 );
					}
					DrawSmokePlayerDamage( m_aUniversalBillboard[ i ] );
					eLastType = eTempType;
				}
				else
					eLastType = EUBTypeNone;
				break;
			case EUBTypeBonus:
				if( eLastType != m_aUniversalBillboard[ i ].eType ) {
					glEnable( GL_BLEND );
					glBlendFunc( GL_SRC_ALPHA, GL_ONE );
					glColor4f( 1.0f, 1.0f, 1.0f,	1.0f ); //bialy
				}
				m_pGameCtrl->GetRGame()->GetRBonus()->DrawBonus( m_aUniversalBillboard[ i ], i ); //funkcja moze usunac m_aUniversalBillboard[ i ], dlatego stostujemy eTempType
				eLastType = eTempType;
				break;
#ifndef DRAW_PLAYER_SHIELD_3D
			case EUBTypePlayerShield:
				if( m_pGameCtrl->GetRGame()->GetPlayer()->GetShieldActual() == 0
					|| m_pGameCtrl->GetRGame()->GetPlayer()->GetPlayerExplode() > 0 )
					break;
				if( eLastType != m_aUniversalBillboard[ i ].eType ) {
					glEnable( GL_BLEND );
					glBlendFunc( GL_SRC_ALPHA, GL_ONE );

					//oblicz procent oslony
					//GLfloat fPercent = ( static_cast< GLfloat >( GetPlayer()->GetShieldActual() ) * 100.0f ) / static_cast< GLfloat >( GetPlayer()->GetShieldMax() );
					//fPercent /= 100.0f; //zmianiamy ze 100 na 1.0
					glColor4f( 1.0f, 1.0f, 1.0f,	1.0f );//fPercent ); //bialy
				}
				m_pGameCtrl->GetRGame()->DrawPlayerShield2D( m_aUniversalBillboard[ i ] );
				eLastType = eTempType;
				break;
#endif // !DRAW_PLAYER_SHIELD_3D
			case EUBTypePlayerShot:
				m_pGameCtrl->GetRGame()->DrawPlayerShots( m_aUniversalBillboard[ i ], i );
				eLastType = eTempType;
				break;
			case EUBTypeEnemyShot:
				m_pGameCtrl->GetRGame()->DrawEnemyShots( m_aUniversalBillboard[ i ], i );
				eLastType = eTempType;
				break;
		}
	}

	glDisable( GL_BLEND );
	PROFILER_END();
}

GLboolean CUniversalBillboard::IsBonusesOrEnemyShots()
{
	for( GLint i = GetUniversalBillboardSize() - 1; i >= 0; --i ) {
		if( m_aUniversalBillboard[ i ].eType == EUBTypeBonus || m_aUniversalBillboard[ i ].eType == EUBTypeEnemyShot )
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLint CUniversalBillboard::AddExplosion( const CVector3 &in_cPos, GLfloat fScale )
{
	GLint i = 0;
	for( i = 0; i < GetUniversalBillboardSize(); ++i ) {
		if( m_aUniversalBillboard[ i ].eType == EUBTypeExplosion ) {
			if( m_aUniversalBillboard[ i ].sExpl.iActualAnimFrame == -1 ) {
				m_aUniversalBillboard[ i ].cPos = in_cPos;
				m_aUniversalBillboard[ i ].sExpl.fScale = fScale;
				m_aUniversalBillboard[ i ].sExpl.iActualAnimFrame = 0;
				break;
			}
		}
	}
	return i; //zwraca index
}

#ifndef DRAW_PLAYER_SHIELD_3D
GLvoid CUniversalBillboard::SetDrawPlayerShield2D()
{
	for( GLint i = GetUniversalBillboardSize() - 1; i >= 0; --i ) {
		if( m_aUniversalBillboard[ i ].eType == EUBTypePlayerShield ) {
			m_aUniversalBillboard[ i ].sPlayerShield.iActualAnimFrame = 0; //rysuj
			break;
		}
	}
}
#endif // !DRAW_PLAYER_SHIELD_3D

GLvoid CUniversalBillboard::AddSmokeRemoteRocket( const CVector3 &in_cPos )
{
	GLfloat fCount = 0.0f;
	for( GLint i = GetUniversalBillboardSize() - 1; i >= 0; --i ) {
		if( m_aUniversalBillboard[ i ].eType == EUBTypeSmokeRemoteRocket ) {
			if( !m_aUniversalBillboard[ i ].bIsDrawSmokeRR ) {
				m_aUniversalBillboard[ i ].bIsDrawSmokeRR = GL_TRUE;
				m_aUniversalBillboard[ i ].cPos = in_cPos;
				m_aUniversalBillboard[ i ].cPos.z += COLLISION_A_REMOTE_ROCKET + fCount;
				fCount += 6.0f;
				if( fCount == 12.0f )
					break;
			}
		}
	}
}

GLvoid CUniversalBillboard::AddSmokePlayerDamage( const CVector3 &in_cPos )
{
	for( GLint i = GetUniversalBillboardSize() - 1; i >= 0; --i ) {
		if( m_aUniversalBillboard[ i ].eType == EUBTypeSmokePlayerDamage ) {
			if( !m_aUniversalBillboard[ i ].bIsDrawSmokePD ) {
				m_aUniversalBillboard[ i ].bIsDrawSmokePD = GL_TRUE;
				m_aUniversalBillboard[ i ].cPos = in_cPos;
				break;
			}
		}
	}
}

GLvoid CUniversalBillboard::SetNoDrawForShotsGroup( EUniversalBillboardType eType, GLuint uiGroupIndex )
{
	//szuka pociskow gracza o danej grupe i ustawia na nie rysowanie

	if( eType != EUBTypePlayerShot && eType != EUBTypeEnemyShot )
		return;

	for( GLint i = GetUniversalBillboardSize() - 1; i >= 0; --i ) {
		if( m_aUniversalBillboard[ i ].eType == eType ) {
			if( m_aUniversalBillboard[ i ].sShot.uiGroupIndex == uiGroupIndex )
				m_aUniversalBillboard[ i ].sShot.bDraw = GL_FALSE;
		}
	}
}

GLvoid CUniversalBillboard::SetManoeuvringForShotsGroup( EUniversalBillboardType eType, GLuint uiGroupIndex, GLint i3DObjIndex )
{
	//szuka pociskow gracza o danej grupe i ustawia indeks wrogiego statku, ktory
	//wykonal unik dla danej grupy pociskow

	if( eType != EUBTypePlayerShot && eType != EUBTypeEnemyShot )
		return;

	for( GLint i = GetUniversalBillboardSize() - 1; i >= 0; --i ) {
		if( m_aUniversalBillboard[ i ].eType == eType ) {
			if( m_aUniversalBillboard[ i ].sShot.uiGroupIndex == uiGroupIndex )
				m_aUniversalBillboard[ i ].sShot.i3DObjIndex = i3DObjIndex;
		}
	}
}

GLvoid CUniversalBillboard::SetArmamentForShotsGroup( EUniversalBillboardType eType, GLuint uiGroupIndex, GLint iArmament )
{
	//ustawiamy sile pocisku, dla kazdego pocisku tworzacego grupe

	if( eType != EUBTypePlayerShot && eType != EUBTypeEnemyShot )
		return;

	for( GLint i = GetUniversalBillboardSize() - 1; i >= 0; --i ) {
		if( m_aUniversalBillboard[ i ].eType == eType ) {
			if( m_aUniversalBillboard[ i ].sShot.uiGroupIndex == uiGroupIndex )
				m_aUniversalBillboard[ i ].sShot.iArmament = iArmament;
		}
	}
}

GLvoid CUniversalBillboard::DrawSmokeRemoteRocket( SUniversalBillboard &in_sUB )
{
	glPushMatrix();
	glTranslatef( in_sUB.cPos.x, in_sUB.cPos.y, in_sUB.cPos.z );

	m_pGameCtrl->GetBillboard()->DrawQuadBillboard( CVector3(), 3.0f );

	//przesuwamy
	in_sUB.cPos.z += ( PLAYER_FLY_SPEED * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	if( in_sUB.cPos.z > MIN_GAME_DISTANCE )
		in_sUB.bIsDrawSmokeRR = GL_FALSE;

	glPopMatrix();
}

GLvoid CUniversalBillboard::DrawSmokePlayerDamage( SUniversalBillboard &in_sUB )
{
	glPushMatrix();
	glTranslatef( in_sUB.cPos.x, in_sUB.cPos.y, in_sUB.cPos.z );

	m_pGameCtrl->GetBillboard()->DrawQuadBillboard( CVector3(), 7.0f );//orginalnie 9.0

	//przesuwamy
	in_sUB.cPos.z += ( PLAYER_FLY_SPEED * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier() );
	if( in_sUB.cPos.z > MIN_GAME_DISTANCE )
		in_sUB.bIsDrawSmokePD = GL_FALSE;

	glPopMatrix();
}