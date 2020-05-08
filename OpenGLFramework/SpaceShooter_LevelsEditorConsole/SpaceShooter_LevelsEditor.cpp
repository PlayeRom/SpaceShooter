// SpaceShooter_LevelsEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <vector>

#include "LevelsStructs.h"

#define VERSION "1.0"

using namespace std;

void LoadLevels();
void ClearLevels();
int GetLevelsSize();

void AddLevel();
void AddGroup(SLevel &sLevel);
bool SaveLevel(const SLevel &sLevel);
bool SaveAllLevels();

void EnterLevel(SLevel &sLevel);
void EnterWithBoss(SLevel &sLevel);
void EnterMaxShips(SLevel &sLevel);
void EnterFrequencyShoot(SLevel &sLevel);
void EnterPercentChanceToShoot(SLevel &sLevel);
void EnterNumberGroups(SLevel &sLevel);

void EnterIndexShip(SLevel &sLevel, int i);
void EnterNumberShips(SLevel &sLevel, int i);
void EnterFormation(SLevel &sLevel, int i);
void EnterPosXForRand(SLevel &sLevel, int i);
void EnterSpeed(SLevel &sLevel, int i);
void EnterCosFly(SLevel &sLevel, int i);
void EnterSlantFly(SLevel &sLevel, int i);
void EnterOffsetXPosForSlantFly(SLevel &sLevel, int i);

void BrowseLevels();
void PrintLevelInfo(SLevel &sLevel);
void EditLevel(int iIndex);
void DeleteLevel(int iIndex);
void MoveDownIndex(int iIndex);
void CopyLevel(SLevel &dest, SLevel &src);


vector<SLevel> g_aLevels;

int _tmain(int argc, _TCHAR* argv[])
{
	LoadLevels();

	char cMenu = 0;
	do{
		system("cls");
		cout << "LEVELS EDITOR for Space Shooter project. Version: " << VERSION << endl;
		cout << endl << "MAIN MENU" << endl;
		cout << "[A]dd new level" << endl;
		cout << "[B]rowse levels" << endl;
		cout << "[E]xit" << endl;
		cout << "Press proper key and confirm [Enter]: ";
		cin >> cMenu;

		switch(cMenu){
			case 'b':
			case 'B':
				BrowseLevels();
				break;
			case 'a':
			case 'A':
				AddLevel();
				break;
		}
	}
	while(cMenu != 'e' && cMenu != 'E');

	//sprzatanie
	ClearLevels();

	return 0;
}

void LoadLevels()
{
	ClearLevels();
	FILE *f;
	errno_t error = _wfopen_s(&f, FILE_NAME_LEVEL_EDIT2, _T("rb"));
	if(error != 0){
		cout << "LoadLevels: open file error: " << error << endl;
		system("pause");
		return;
	}

	SLevel sLevel;
	while( fread(&sLevel, sizeof(SLevel), 1, f) != NULL){
		g_aLevels.push_back( sLevel );
		int iIndex = GetLevelsSize() - 1;
		if(g_aLevels[iIndex].iNumberGroups > 0){
			g_aLevels[iIndex].psShipsGroups = new SShipsGroups[g_aLevels[iIndex].iNumberGroups];
			for(int i = 0; i < g_aLevels[iIndex].iNumberGroups; ++i){
				fread(&g_aLevels[iIndex].psShipsGroups[i], sizeof(SShipsGroups), 1, f);
			}
		}
		else
			g_aLevels[iIndex].psShipsGroups = NULL;
	}
	fclose(f);
}

void ClearLevels()
{
	for(int i = GetLevelsSize() - 1; i >= 0; --i){
		if(g_aLevels[i].psShipsGroups)
			delete [] g_aLevels[i].psShipsGroups;
	}
	g_aLevels.clear();
}

int GetLevelsSize()
{
	return static_cast<int>(g_aLevels.size());
}

void AddLevel()
{
	SLevel sLevel;

	EnterLevel(sLevel);
	EnterWithBoss(sLevel);
	EnterMaxShips(sLevel);
	EnterFrequencyShoot(sLevel);
	EnterPercentChanceToShoot(sLevel);
	EnterNumberGroups(sLevel);

	if(sLevel.iNumberGroups < 0){
		sLevel.iNumberGroups = 0;
		sLevel.psShipsGroups = NULL;
	}

	if(sLevel.iNumberGroups > 0){
		sLevel.psShipsGroups = new SShipsGroups[sLevel.iNumberGroups];
		AddGroup(sLevel);
	}

	if(SaveLevel(sLevel))
		cout << "Save level OK!" << endl;
	else
		cout << "Save level failed!" << endl;

	system("pause");

	//wczytanie tablicy na nowo
	LoadLevels();
}

void AddGroup(SLevel &sLevel)
{
	for(int i = 0; i < sLevel.iNumberGroups; ++i){
		cout << endl << "Group number: " << i << endl;
		EnterIndexShip(sLevel, i);
		EnterNumberShips(sLevel, i);
		EnterFormation(sLevel, i);
		EnterPosXForRand(sLevel, i);
		EnterSpeed(sLevel, i);
		EnterCosFly(sLevel, i);
		if(!sLevel.psShipsGroups[i].bCosFly){
			//jezeli nie po cos to moze po skosie
			EnterSlantFly(sLevel, i);
			if(sLevel.psShipsGroups[i].bSlantFly)
				EnterOffsetXPosForSlantFly(sLevel, i);
			else sLevel.psShipsGroups[i].fOffsetXPosForSlantFly = 0.0f;
		}
		else{
			//jest po cos wiec po skosie nie moze byc
			sLevel.psShipsGroups[i].bSlantFly = 0;
			sLevel.psShipsGroups[i].fOffsetXPosForSlantFly = 0.0f;
		}
	}
}

void EnterLevel(SLevel &sLevel)
{
	cout << "Enter level (int): ";
	cin >> sLevel.iLevel;
}

void EnterWithBoss(SLevel &sLevel)
{
	char cMark = 0;
	cout << "Level with boss (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y')
		sLevel.bWithBoss = 1;
	else
		sLevel.bWithBoss = 0;
}

void EnterMaxShips(SLevel &sLevel)
{
	cout << "Enter max ships on screen (int): ";
	cin >> sLevel.iMaxShipsOnScreen;
}

void EnterFrequencyShoot(SLevel &sLevel)
{
	cout << "Enter frequency shoot (int): ";
	cin >> sLevel.iFrequencyShoot;
	if(sLevel.iFrequencyShoot < 1)
		sLevel.iFrequencyShoot = 1;
}

void EnterPercentChanceToShoot(SLevel &sLevel)
{
	cout << "Enter percent chance to shoot (int): ";
	cin >> sLevel.iPercentChanceToShoot;
	if(sLevel.iPercentChanceToShoot > 100)
		sLevel.iPercentChanceToShoot = 100;
	if(sLevel.iPercentChanceToShoot < 1)
		sLevel.iPercentChanceToShoot = 1;
}

void EnterNumberGroups(SLevel &sLevel)
{
	cout << "Enter number groups (int): ";
	cin >> sLevel.iNumberGroups;
}

void EnterIndexShip(SLevel &sLevel, int i)
{
	cout << "Enter index ship (int): ";
	cin >> sLevel.psShipsGroups[i].iIndexShip;
}

void EnterNumberShips(SLevel &sLevel, int i)
{
	cout << "Enter number ships (int): ";
	cin >> sLevel.psShipsGroups[i].iNumberShips;
}

void EnterFormation(SLevel &sLevel, int i)
{
	cout << "Enter formation (int): ";
	cin >> sLevel.psShipsGroups[i].iFormation;
	if(sLevel.psShipsGroups[i].iFormation < 1 || sLevel.psShipsGroups[i].iFormation > 3)
		sLevel.psShipsGroups[i].iFormation = 1;
}

void EnterPosXForRand(SLevel &sLevel, int i)
{
	cout << "Enter pos X for rand (int): ";
	cin >> sLevel.psShipsGroups[i].iPosXForRand;
	if(sLevel.psShipsGroups[i].iPosXForRand < 1)
		sLevel.psShipsGroups[i].iPosXForRand = 1;
}

void EnterSpeed(SLevel &sLevel, int i)
{
	cout << "Enter speed (float): ";
	cin >> sLevel.psShipsGroups[i].fSpeed;
}

void EnterCosFly(SLevel &sLevel, int i)
{
	char cMark = 0;
	cout << "Is cosinus fly (y/n)?: ";
	cin >> cMark;
	if(cMark == 'Y' || cMark == 'y')
		sLevel.psShipsGroups[i].bCosFly = 1;
	else
		sLevel.psShipsGroups[i].bCosFly = 0;
}

void EnterSlantFly(SLevel &sLevel, int i)
{
	char cMark = 0;
	cout << "Is slant fly (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y')
		sLevel.psShipsGroups[i].bSlantFly = 1;
	else
		sLevel.psShipsGroups[i].bSlantFly = 0;
}

void EnterOffsetXPosForSlantFly(SLevel &sLevel, int i)
{
	cout << "Enter offset X pos for slant fly (float): ";
	cin >> sLevel.psShipsGroups[i].fOffsetXPosForSlantFly;
}

bool SaveLevel(const SLevel &sLevel)
{
	FILE *f;
	errno_t error = _wfopen_s(&f, FILE_NAME_LEVEL_EDIT2, _T("ab"));
	if(error != 0){
		cout << "SaveLevel : open file error: " << error << endl;
		system("pause");
		return false;
	}
	fwrite(&sLevel, sizeof(SLevel), 1, f);
	for(int i = 0; i < sLevel.iNumberGroups; ++i)
		fwrite(&sLevel.psShipsGroups[i], sizeof(SShipsGroups), 1, f);
	fclose(f);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////BROWSE/////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void BrowseLevels()
{
	char cMenu = 0;
	int iIndex = 0; //aktualny index w tablicy

	do{
		system("cls");
		cout << "LEVELS BROWSE. Actual record number: " << iIndex + 1 << "/" << GetLevelsSize() << endl;
		if(GetLevelsSize() > 0){
			if(iIndex < 0 || iIndex >= GetLevelsSize())
				cout << "Incorrect array index" << endl;
			else
				PrintLevelInfo(g_aLevels[iIndex]);
		}
		else{
			iIndex = 0;
			cout << "Any objects" << endl;
		}

		cout << endl << "MENU BROWSE:" << endl;
		cout << "[P]revious level" << endl;
		cout << "[N]ext level" << endl;
		cout << "[E]dit level" << endl;
		cout << "[D]elete level" << endl;
		cout << "[M]ove down index" << endl;
		cout << "[B]ack to main menu" << endl;
		cout << "Press proper key and confirm [Enter]: ";
		cin >> cMenu;

		switch(cMenu){
			case 'p':
			case 'P':
				if(--iIndex < 0)
					iIndex = GetLevelsSize() - 1;
				break;
			case 'n':
			case 'N':
				if(++iIndex >= GetLevelsSize())
					iIndex = 0;
				break;
			case 'e':
			case 'E':
				EditLevel(iIndex);
				break;
			case 'd':
			case 'D':
				DeleteLevel(iIndex);
				iIndex = 0;
				break;
			case 'm':
			case 'M':
				MoveDownIndex(iIndex);
				break;
		}
	}
	while(cMenu != 'b' && cMenu != 'B');
}

void PrintLevelInfo(SLevel &sLevel)
{
	cout << "Level: " << sLevel.iLevel << endl;
	if(sLevel.bWithBoss)
		cout << "With boss: YES" << endl;
	else cout << "With boss: NO" << endl;
	cout << "Max ships on screen: " << sLevel.iMaxShipsOnScreen << endl;
	cout << "Frequency shoot: " << sLevel.iFrequencyShoot << endl;
	cout << "Percent chance to shoot: " << sLevel.iPercentChanceToShoot << "%" << endl;
	cout << "Number groups: " << sLevel.iNumberGroups << endl;

	for(int i = 0; i < sLevel.iNumberGroups; ++i){
		cout << endl << "Gropu: " << i << endl;
		cout << "Index ship: " << sLevel.psShipsGroups[i].iIndexShip << endl;
		cout << "Number ships in group: " << sLevel.psShipsGroups[i].iNumberShips << endl;
		cout << "Formation: " << sLevel.psShipsGroups[i].iFormation << endl;
		cout << "Pos X for rand: " << sLevel.psShipsGroups[i].iPosXForRand << endl;
		cout << "Speed: " << sLevel.psShipsGroups[i].fSpeed << endl;
		if(sLevel.psShipsGroups[i].bCosFly)
			cout << "Cosinus fly: YES" << endl;
		else cout << "Cosinus fly: NO" << endl;
		if(sLevel.psShipsGroups[i].bSlantFly)
			cout << "Slant fly: YES" << endl;
		else cout << "Slant fly: NO" << endl;
		cout << "Offset X pos for slant fly: " << sLevel.psShipsGroups[i].fOffsetXPosForSlantFly << endl;
	}
}

void EditLevel(int iIndex)
{
	if(iIndex < 0 || iIndex >= GetLevelsSize()){
		cout << "EditLevel: Incorrect index array" << endl;
		system("pause");
		return;
	}

	char cMark = 0;
	cout << "Edit lavel number " << iIndex << ". Are you sure? (y/n) :";
	cin >> cMark;
	if(cMark != 'y' && cMark != 'Y')
		return;

	cout << "Level: " << g_aLevels[iIndex].iLevel << endl;
	cout << "Change level (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y')
		EnterLevel(g_aLevels[iIndex]);

	if(g_aLevels[iIndex].bWithBoss)
		cout << "With boss: YES" << endl;
	else cout << "With boss: NO" << endl;
	cout << "Change With boss (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y')
		EnterWithBoss(g_aLevels[iIndex]);

	cout << "Max ships on screen: " << g_aLevels[iIndex].iMaxShipsOnScreen << endl;
	cout << "Change max ships on screen (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y')
		EnterMaxShips(g_aLevels[iIndex]);

	cout << "Frequency shoot: " << g_aLevels[iIndex].iFrequencyShoot << endl;
	cout << "Change frequency shoot (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y')
		EnterFrequencyShoot(g_aLevels[iIndex]);

	cout << "Percent chance to shoot: " << g_aLevels[iIndex].iPercentChanceToShoot << "%" << endl;
	cout << "Change percent chance to shoot (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y')
		EnterPercentChanceToShoot(g_aLevels[iIndex]);

	cout << "Number groups: " << g_aLevels[iIndex].iNumberGroups << endl;
	cout << "Change number groups (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y'){
		EnterNumberGroups(g_aLevels[iIndex]);

		if(g_aLevels[iIndex].psShipsGroups)
			delete [] g_aLevels[iIndex].psShipsGroups;

		if(g_aLevels[iIndex].iNumberGroups <= 0)
			g_aLevels[iIndex].psShipsGroups = NULL;
		else{
			g_aLevels[iIndex].psShipsGroups = new SShipsGroups[g_aLevels[iIndex].iNumberGroups];
			AddGroup(g_aLevels[iIndex]);
		}
	}

	cout << "Save change (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y'){
		if(SaveAllLevels())
			cout << "Save levels OK!" << endl;
		else
			cout << "Save levels failed!" << endl;
	}
	system("pause");
}

void DeleteLevel(int iIndex)
{
	if(iIndex < 0 || iIndex >= GetLevelsSize()){
		cout << "DeleteLevel: Incorrect index array" << endl;
		system("pause");
		return;
	}

	char cMark = 0;
	cout << "Delete level number " << iIndex << ". Are you sure? (y/n) :";
	cin >> cMark;

	if(cMark != 'y' && cMark != 'Y')
		return;

	if(g_aLevels[iIndex].psShipsGroups)
		delete [] g_aLevels[iIndex].psShipsGroups;
	g_aLevels.erase( g_aLevels.begin() + iIndex );

	SaveAllLevels();
	cout << "Delete level OK" << endl;
	system("pause");
}

void MoveDownIndex(int iIndex)
{
	if(iIndex < 0 || iIndex >= GetLevelsSize()){
		cout << "MoveDownIndex: incorrect index array" << endl;
		system("pause");
		return;
	}
	//strwadz czy aktualna pozycja nie jest ostatnia
	if(iIndex == GetLevelsSize() - 1){
		cout << "This index is last" << endl;
		system("pause");
		return;
	}

	SLevel sTempLevel;
	memset(&sTempLevel, 0, sizeof(SLevel));
	CopyLevel(sTempLevel, g_aLevels[iIndex]);
	CopyLevel(g_aLevels[iIndex], g_aLevels[iIndex+1]);
	CopyLevel(g_aLevels[iIndex+1], sTempLevel);

	SaveAllLevels();
}

void CopyLevel(SLevel &dest, SLevel &src)
{
	if(dest.psShipsGroups)
		delete [] dest.psShipsGroups;

	dest.iLevel = src.iLevel;
	dest.bWithBoss = src.bWithBoss;
	dest.iMaxShipsOnScreen = src.iMaxShipsOnScreen;
	dest.iFrequencyShoot = src.iFrequencyShoot;
	dest.iPercentChanceToShoot = src.iPercentChanceToShoot;
	dest.iNumberGroups = src.iNumberGroups;

	if(dest.iNumberGroups == 0)
		dest.psShipsGroups = NULL;
	else{
		dest.psShipsGroups = new SShipsGroups[dest.iNumberGroups];
		for(int i = 0; i < dest.iNumberGroups; ++i){
			dest.psShipsGroups[i].iIndexShip = src.psShipsGroups[i].iIndexShip;
			dest.psShipsGroups[i].iNumberShips = src.psShipsGroups[i].iNumberShips;
			dest.psShipsGroups[i].iFormation = src.psShipsGroups[i].iFormation;
			dest.psShipsGroups[i].iPosXForRand = src.psShipsGroups[i].iPosXForRand;
			dest.psShipsGroups[i].fSpeed = src.psShipsGroups[i].fSpeed;
			dest.psShipsGroups[i].bCosFly = src.psShipsGroups[i].bCosFly;
			dest.psShipsGroups[i].bSlantFly = src.psShipsGroups[i].bSlantFly;
			dest.psShipsGroups[i].fOffsetXPosForSlantFly = src.psShipsGroups[i].fOffsetXPosForSlantFly;
		}
	}
}

bool SaveAllLevels()
{
	FILE *f;
	errno_t error = _wfopen_s(&f, FILE_NAME_LEVEL_EDIT2, _T("wb"));
	if(error != 0){
		cout << "SaveAllLevels: open file error: " << error << endl;
		system("pause");
		return false;
	}

	for(int i = 0; i < GetLevelsSize(); ++i){
		fwrite(&g_aLevels[i], sizeof(SLevel), 1, f);
		for(int j = 0; j < g_aLevels[i].iNumberGroups; ++j){
			fwrite(&g_aLevels[i].psShipsGroups[j], sizeof(SShipsGroups), 1, f);
		}
	}

	fclose(f);

	return true;
}