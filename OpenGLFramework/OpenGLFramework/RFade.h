#pragma once

/// Klasa efektu przejścia między poziomami
/** Efekt polega na całkowitym przyciemnieniu ekranu i ponownym rozjaśnieniu. */

class CRFade
{
public:
	CRFade();
	~CRFade();

	/// Full fade - czyli najpierw przyciemniamy ekaran i potem rozjasniamy
	GLvoid DoDrawFullFade();
	/// Half fade - czyli startujemy z czarnego ekranu i tylko rozjasniamy
	GLvoid DoDrawHalfFade();
	GLvoid Draw();
	GLboolean CanChangeLevel() { return m_bChangeLevel; }
	GLboolean IsDraw() { return m_bDraw; }

private:
	GLfloat m_fFade;
	GLboolean m_bDraw;
	GLboolean m_bChangeLevel; // czy mozna zmienic poziom, mamy czern na ekranie
	GLboolean m_bFullFade;

	GLvoid CalculateFullFade();
	GLvoid CalculateHalfFade();
};
