#pragma once

class CGameControl;

class CRRemoteRocket
{
public:
	CRRemoteRocket();
	~CRRemoteRocket();

	GLvoid DetonationRemoteRocket(GLboolean bManualDetonation = GL_TRUE);
	inline GLvoid SetBlockRemoteRocket(GLboolean bBlock) { m_bBlockRemoteRocket = bBlock; }
	inline GLboolean IsBlockRemoteRocket() { return m_bBlockRemoteRocket; }

	inline GLvoid SetIsDraw(GLboolean bIsDraw) { m_bIsDraw = bIsDraw; }
	inline GLboolean IsDraw() { return m_bIsDraw; }
	inline CVector3 GetPosition() { return m_cPosition; }

	GLvoid ServiceEnterKey();
	GLvoid BlockEnterKey();

	GLvoid DrawFlyRemoteRocket();
	GLboolean IsCollisionEnemyRemoteRocket(CVector3 in_cPosExpl, const CVector3 &in_cPosShip);

private:
	CGameControl *m_pGameCtrl;

	GLvoid CheckCollisionEnemyRemoteRocket(const CVector3 &in_cPosExpl);
	GLint m_iEmitterIndexForRemoteRocket;
	GLboolean m_bBlockRemoteRocket;

	// dane samej rakiety potrzebne do jej rysowania
	CVector3 m_cPosition;
	GLfloat m_fSpeed;
	GLboolean m_bIsDraw;
};
