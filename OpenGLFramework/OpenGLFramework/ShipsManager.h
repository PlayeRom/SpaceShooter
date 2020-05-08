#pragma once

#include "../SpaceShooter_ObjectsEditor/UniqueShipsStructs.h"

class CShipsManager
{
public:
	CShipsManager();
	~CShipsManager();

	GLint GetNextIndex(GLint iIndex, GLboolean bPlayerShip);
	GLint GetPreviousIndex(GLint iIndex, GLboolean bPlayerShip);

	GLuint GetPointsForShootDown(GLint i3DObjIndex); //ilosc punktow za zestrzelenie ale nie liczac poziomu trudnosci
	inline GLint GetUniqueShipsSize(){
		return static_cast< GLint >( m_aUniqueShipsWork.size() );
	}
	GLboolean IsPlayer(GLint iIndex);
	GLint GetShieldPoints(GLint iIndex, GLint iStarsShield = -1);
	GLint GetArmourPoints(GLint iIndex, GLint iStarsArmour = -1);
	GLint GetArmamentPoints(GLint iIndex, GLint iStarsArmament = -1);
	GLint GetManoeuvringPercent(GLint iIndex, GLint iStarsManoeuvring = -1);

	GLint GetStarsShield(GLint iIndex);
	GLint GetStarsArmour(GLint iIndex);
	GLint GetStarsArmament(GLint iIndex);
	GLint GetStarsManoeuvring(GLint iIndex);

	GLint GetEnemyShipNumber();
	GLint GetPlayerShipNumber();
	GLint GetRandomEnemyShipIndex();

	GLvoid SaveUniqueShipsWork();
	GLboolean IsAvailableInBrowse(GLint iIndex);
	GLvoid SetAvailableInBrowse(GLint iIndex, GLboolean bAvailable);
	inline SUniqueShip& GetUniqueShip(GLint iIndex) { return m_aUniqueShipsWork[iIndex].sUniqueShips; }

private:
	std::vector<SUniqueShipWork> m_aUniqueShipsWork;
	GLboolean ReadUniqueShips(LPCTSTR lpFile);
	GLboolean ReadUniqueShipsWork(LPCTSTR lpFile);
	GLvoid Clear();

	SUniqueShip sErrorReturn;
};
