#pragma once

#include "UniqueShipsStructs.h"

class CShipsManager
{
public:
	CShipsManager();
	~CShipsManager();

	void ConvertStruct();
	bool LoadUniqueShips(LPCTSTR lpFileName = NULL);
	bool SaveUniqueShips();
	bool SaveUniqueShip(const SUniqueShip &sUniqueShip);
	void ClearUniqueShips();
	bool MoveUpIndex(int iIndex);
	bool MoveDownIndex(int iIndex);
	bool AddNewEmitter(int iIndex, const SEmitterData &sNewEmitter);
	bool DeleteEmitter(int iIndexObj, int iIndexEmitter);
	bool SaveModifyEmitter(int iIndexObj, int iIndexEmitter, const SEmitterData &sModEmitter);
	bool AddNewObject(const SUniqueShip &sNewShip);
	bool DeleteObject(int iIndexObj);
	bool SaveModifyObject(int iIndexObj, SUniqueShip &sModShip);

	inline int GetUniqueShipSize(){ return static_cast<int>(m_aUniqueShip.size()); }
	inline SUniqueShip& GetUniqueShip(int iIndex){ return m_aUniqueShip[iIndex]; }

private:
	std::vector<SUniqueShip> m_aUniqueShip;

	TCHAR m_cFileName[256];

	void CopyUniqueShip(SUniqueShip &dest, SUniqueShip &src);
};
