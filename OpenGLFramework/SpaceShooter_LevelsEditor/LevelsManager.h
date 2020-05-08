#pragma once

#include "../SpaceShooter_ObjectsEditor/ShipsManager.h"

class CLevelsManager
{
public:
	CLevelsManager();
	~CLevelsManager();

	void ConvertStruct();

	bool LoadLevels(LPCTSTR lpFileName = NULL);
	void ClearLevels();
	bool SaveLevel(const SLevel &sLevel);
	bool SaveAllLevels();
	bool MoveDownIndex(int iIndex);
	bool MoveUpIndex(int iIndex);
	void CopyLevel(SLevel &dest, SLevel &src);

	bool AddNewLevel(const SLevel &sLevel);
	bool DeleteLevel(int iIndexLevel);
	bool SaveModifyLevel(int iIndexLevel, SLevel &sLevel);

	bool AddNewGroup(int iIndexLevel, const SShipsGroups &sNewGroup);
	bool DeleteGroup(int iIndexLevel, int iIndexGroup);
	bool SaveModifyGroup(int iIndexLevel, int iIndexGroup, const SShipsGroups &sModGroup);

	inline int GetLevelsSize() { return static_cast<int>(m_aLevels.size()); }
	inline SLevel& GetLevel(int iIndexLevel) { return m_aLevels[iIndexLevel]; }

	// dla obiektów
	int GetShipsNumber();
	TCHAR* GetShipName(int iIndex);

private:
	std::vector<SLevel> m_aLevels;
	TCHAR m_cFileName[256];

	// dla obiektów
	CShipsManager*	 m_pShipsManager;
};
