#pragma once

#include "EnemyShip.h"

class CREnemyShotAnimCtrl
{
public:
	CREnemyShotAnimCtrl();
	~CREnemyShotAnimCtrl();

	GLvoid SetAllOnFree();
	GLint GetFreeIndex();
	GLvoid FreeAnimCtrlForEnemyShot(SEnemyShip &sEnemyShip);

private:
	struct SEnemyShotAnimCtrl
	{
		GLboolean bIsFree; //jest wolny - nie przydzielony statkowi
		GLint iIndex; //index z CreateAnimationControl
	};

	std::vector< SEnemyShotAnimCtrl > m_aEnemyShotAnimCtrl;
	inline GLint GetEnemyShotAnimCtrlSize() { return static_cast< GLint >( m_aEnemyShotAnimCtrl.size() ); }
};
