#pragma once

#include "RMenuBase.h"

#define MAX_REGIONS_SHIP_BROWSE 5
#define MAX_REGIONS_DESCRIPTION 13 //ilosc linijek opisu

class CGameControl;
class CFog;

enum EModeShipBrowse
{
	EBrowseEnemyShips,
	EBrowsePlayerShips,
	EBrowseMultiplayer
};

class CRShipsBrowse : public CRMenuBase
{
public:
	CRShipsBrowse();
	~CRShipsBrowse();

	GLvoid Draw();
	GLvoid RestartObjects();

	GLboolean MouseLButtonDown(GLint iX, GLint iY);
	GLboolean MouseLButtonUp(GLint iX, GLint iY);
	GLboolean MouseWheel(GLint iScrollLines);
	GLboolean KeyDown(GLuint uiKey);

	GLvoid ResetVariables();
	GLvoid SetStartVariables();
	GLvoid SetEndVariables();

	GLvoid SetMode(EModeShipBrowse eMode);

	GLvoid RunGame(GLint iIndexShip, GLboolean bCheckCorrectness = GL_TRUE);

private:
	CGameControl *m_pGameCtrl;
	CFog		*m_pFog;
	CSDLFont	*m_pSDLFontTitle;
	CSDLFont	*m_pSDLFontMenu;
	CSDLFont	*m_pSDLFontParam;

	GLvoid SetMouseRegion();
	GLvoid ResetDescVariable();

	RECT m_rRegions[MAX_REGIONS_SHIP_BROWSE];
	RECT m_rRegions_Font[MAX_REGIONS_SHIP_BROWSE];
	
	RECT m_rRegionDescription; //animowana plansza - podklad pod opis
	GLint m_iDescRollActual;
	GLboolean m_bDescRoll; //jezeli true to rozwijaj
	POINT m_rRegionsDesc_Font[MAX_REGIONS_DESCRIPTION];
	GLint m_iActualPrintDescLine;
	GLint m_iActualPrintCharInLine;
	GLvoid PrintDescription();
	GLboolean m_bPrintDescForThisShip; //aby drukowalo opis tylko za piwerwszym razem dla danego statku

	RECT m_rRegionsStars[12];
	RECT m_rRegionsStarsFont[12];

	GLvoid CalcRotateShip();
	GLvoid DrawShip();
	GLvoid DrawStatistics();
	GLvoid DrawAll2D();
	GLvoid DrawSchemeSide();
	GLvoid DrawSchemeTop();
	GLvoid DrawSchemeFront();
	GLvoid DrawElementsScheme();
	GLvoid ScaleElementsScheme();
	GLvoid DrawShipScheme();
	std::vector<GLushort> m_aShipSchemeArray;

	GLfloat m_fRotX;
	GLfloat m_fRotY;
	GLfloat m_fRotZ;
	GLfloat m_fZoom;
	GLboolean m_bHoldMouse;
	POINT m_ptCursorPos;
	POINT m_ptCursorOldPos;

	GLfloat m_fConstZoom;
	GLshort m_sAutoZoom; // =1 - obiekt najezdza, =0 - obiekt najechal, = -1 - obiekt odjezdza

	GLvoid DrawStar(GLint iIndex, GLboolean bFill);
	GLvoid SetFog();
	GLvoid SetFogDensity(GLboolean bShipAvailable);

	EModeShipBrowse m_eActualMode;

	GLvoid SetPreviousShip();
	GLvoid SetNextShip();
	GLvoid Back();

	//wyswietlane informacje o statku
	TCHAR *lpNameShip;
	//ilosc gwiazdek, jako tablica, w kolejnosci oslona, pancerz, uzbrojenie, zwrotnosc
	GLint m_iStarsParameters[4];
	//
	GLint m_iShipsManagerIndex; //ideks tablicy CShipsManager::m_aUniqueShips
	GLvoid SetShipInformation();
	//zmienna do oznaczenie na ktory klinieto przycisk: next czy previous
	GLint m_iNextOrPrev; // 0 - nic, -1 - kliknieto na poprzedni, 1 - kliknieto na nastepny

	GLint GetNextIndexShip(GLint iIndex);
	GLint GetPreviousIndexShip(GLint iIndex);

	GLint m_iTextPosYAvailable;
	GLint m_iTextPosYTitle;
};
