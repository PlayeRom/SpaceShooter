#pragma once

class CGameControl;

//struktura punktu obrazujacego przesuwajaca sie gwiazde przez caly ekran
struct SStar
{
	CVector3 cPos;
	CVector3 cColor;
	GLfloat fSpeed;
	GLboolean bStart; //punkt wystartowal i jest rysowany
};

//klasa do rysowania gwiazdek w grze, jako jasne punkty przewijajace sie przez ekran
class CRStarsEffect
{
public:
	CRStarsEffect();
	~CRStarsEffect();

	GLvoid DrawStars();

private:
	CGameControl	*m_pGameCtrl;
	std::vector<SStar> m_aStars;
	GLvoid CreateStars();
	GLvoid DeleteStars();
};
