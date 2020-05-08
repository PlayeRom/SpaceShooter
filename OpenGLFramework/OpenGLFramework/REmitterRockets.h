#pragma once

#include "UniversalBillboard.h"

class CGameControl;

class CREmitterRockets
{
public:
	CREmitterRockets();
	~CREmitterRockets();

	GLvoid SetForStart();
	GLvoid AddPlayerShot_EmitterRocket();
	GLvoid DrawEmitterRocket();

private:
	CGameControl*	m_pGameCtrl;

	struct SEmitterRocket
	{
		CVector3 cPos;
		GLint iIndexOfEmitter;
		GLint iIndexOfEmitter2;
		GLfloat fCosActualAngle;
		GLboolean bCosSign;
		SShot sShot;
	};

	std::vector< SEmitterRocket > m_aEmitterRockets; //tablica odpalonych rakiet
	inline GLint GetEmitterRocketsSize() { return static_cast< GLint >(m_aEmitterRockets.size()); }
};
