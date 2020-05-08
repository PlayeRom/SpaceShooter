#pragma once

class CGameControl;

class CRBoss
{
public:
	CRBoss();
	~CRBoss();

	GLvoid MoveEnemyBoss(SEnemyShip &in_sEnemy, GLfloat fSpeed);
	GLvoid SetNewDestinationPointForBoss();

private:
	CGameControl *m_pGameCtrl;

	CVector3 m_cBossDestinationPoint; // punkt docelowy dla bossa, do korego leci
	GLvoid DrawEnergyBossBars(const CVector3 &in_cPos, GLint iActualPointsShield, GLint iMaxPointsShield, GLint iActualPointsArmour, GLint iMaxPointsArmour);
	GLvoid DrawEnergyBossBar(GLint iActualPoints, GLint iMaxPoints);
	GLfloat m_fEnergyBossBar_left;
	GLfloat m_fEnergyBossBar_right;
	GLfloat m_fEnergyBossBar_top;
	GLfloat m_fEnergyBossBar_bottom;
};
