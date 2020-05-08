#pragma once

class CGameControl;

class CRShockWaveEffect
{
public:
	CRShockWaveEffect();
	~CRShockWaveEffect();

	GLvoid SetForStart();

	GLvoid DoDrawShockWave(const CVector3 &cPos);
	GLvoid DrawShockWave();

private:
	CGameControl *m_pGameCtrl;

	GLfloat m_fShockWaveScale;
	GLboolean m_bDrawShockWave;
	CVector3 m_cPosShockWave;
#ifdef SHOCKWAVE_3DOBJ
	GLvoid DoDrawShockWave_3DObj(const CVector3 &cPos);
	GLvoid DrawShockWave_3DObj();
#else
	GLvoid DoDrawShockWave_Quadric(const CVector3 &cPos);
	GLvoid DrawShockWave_Quadric();
#endif
};
