#pragma once

class CGameControl;

#define MAX_VISUAL_ENEMY_HIT 20

class CRVisualEnemyHit
{
public:
	CRVisualEnemyHit();
	~CRVisualEnemyHit();

	GLvoid SetToDraw(const CVector3 &in_cPos);
	GLvoid Draw();

	inline GLint GetEmitterEnemyHitSize() {
		return static_cast< GLint >( m_aEmitterEnemyHit.size() );
	}

private:
	struct SEmitterEnemyHit
	{
		GLboolean bIsDraw; //0 - wolny, 1 - zajety, rysowane
		CVector3 cPos; //pozycja dla przesniecia glTranslatef
		GLint iEmitterIndex;
	};

	CGameControl *m_pGameCtrl;
	std::vector<SEmitterEnemyHit> m_aEmitterEnemyHit;

	GLvoid Create();
	GLvoid Clear();
};
