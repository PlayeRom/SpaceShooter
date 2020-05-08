#pragma once

#include "UniversalBillboard.h"

class CGameControl;

class CRHomingRockets
{
public:
	CRHomingRockets();
	~CRHomingRockets();

	GLvoid SetForStart();
	GLvoid AddPlayerShot_HomingRocket();
	GLvoid DrawHomingRocket();

private:
	CGameControl	*m_pGameCtrl;

	struct SHomingRockets
	{
		CVector3 cPos;
		GLint iAnimIndex;
		GLint iSmokeCounter;
		//poczatkowoa pozycja x, dla wyznaczenie jak dlugo rakieta ma nie alokowac sie na cel, gdy leci po skosie,
		//wykorzytywane dla wystrzalu kilku rakiet na raz
		GLfloat fStartPosX; 
		SShot sShot;
	};

	std::vector< SHomingRockets > m_aHomingRockets; //tablica odpalonych rakiet
	inline GLint GetHomingRocketsSize() { return static_cast< GLint >( m_aHomingRockets.size() ); }

	CVector3* m_aIndices;
};
