// SpaceShooter_ShipsEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <vector>

#include "UniqueShipsStructs.h"

#define VERSION "1.0"

using namespace std;

void LoadUniqueShips();
bool SaveUniqueShips();
void ClearUniqueShips();
int GetUniqueShipSize();

void AddShip();
void AddEmitter(SUniqueShip &sUniqueShip);
bool SaveUniqueShip(SUniqueShip &sUniqueShip);
void ConvertChar2TCHAR(char *cChar, TCHAR *cTCHAR);
void PrintTCHAR(TCHAR *cTCHAR, bool bEndl = true);

void BrowseShips();
void PrintShipInfo(SUniqueShip &sUniqueShip);

void DeleteShip(int iIndex);
void EditShip(int iIndex);

void EnterFileName(SUniqueShip &sUniqueShip);
void EnterShipName(SUniqueShip &sUniqueShip);
void EnterIsPlayerShip(SUniqueShip &sUniqueShip);
void EnterTextureType(SUniqueShip &sUniqueShip);
void EnterTextureFileName_Type0(SUniqueShip &sUniqueShip);
void EnterTextureFileName_Type1(SUniqueShip &sUniqueShip);
void EnterTextureFileName_Type2(SUniqueShip &sUniqueShip);
void EnterTextureLifeName(SUniqueShip &sUniqueShip);
void EnterCollisionA(SUniqueShip &sUniqueShip);
void EnterCollisionB(SUniqueShip &sUniqueShip);
void EnterCanFire(SUniqueShip &sUniqueShip);
void EnterCanManoeuvring(SUniqueShip &sUniqueShip);
void EnterShieldStars(SUniqueShip &sUniqueShip);
void EnterArmourStars(SUniqueShip &sUniqueShip);
void EnterArmamentStars(SUniqueShip &sUniqueShip);
void EnterManoeuvringStars(SUniqueShip &sUniqueShip);
void EnterEmitterNumber(SUniqueShip &sUniqueShip);
void MoveDownIndex(int iIndex);
void CopyUniqueShip(SUniqueShip &dest, SUniqueShip &src);

vector<SUniqueShip> g_aUniqueShip;

void ConvertStruct()
{
	//funkcja podmieniajaca pliki ze starej strktury na nowa
	//odczytanie starej struktury
	//przepisanie starej struktury na nowa
	//zapisanie nowej struktury

	vector<SOldUniqueShip> aOldUniqueShip;
	FILE *f;
	SOldUniqueShip sOldUniqueShip;
	errno_t error = _wfopen_s(&f, FILE_NAME_SHIPS_EDIT2, _T("rb"));
	if(error != 0){
		cout << "ConvertStruct: open file error: " << error << endl;
		system("pause");
		return;
	}
	while( fread(&sOldUniqueShip, sizeof(SOldUniqueShip), 1, f) != NULL){
		aOldUniqueShip.push_back( sOldUniqueShip );
		int iIndex = static_cast<int>(aOldUniqueShip.size()) - 1;
		if(aOldUniqueShip[iIndex].iEmitterNumber > 0){
			aOldUniqueShip[iIndex].psEmitterData = new SEmitterData[aOldUniqueShip[iIndex].iEmitterNumber];
			for(int i = 0; i < aOldUniqueShip[iIndex].iEmitterNumber; ++i){
				fread(&aOldUniqueShip[iIndex].psEmitterData[i], sizeof(SEmitterData), 1, f);
			}
		}
		else
			aOldUniqueShip[iIndex].psEmitterData = NULL;
	}
	fclose(f);

	ClearUniqueShips();

	//przepisanie tablic - stara na nowa
	SUniqueShip sUniqueShip;
	for(int i = 0; i < static_cast<int>(aOldUniqueShip.size()); ++i){
		//////////////////////////////////////////////////////////////////////////
		///////nowe wartosci inicjalizujemy, a nie przypisujemy ze starej/////////
		//////////////////////////////////////////////////////////////////////////
		sUniqueShip.byTexType = 0; //nowe
		sUniqueShip.uiCOLORREF = 0; //nowe
		//////////////////////////////////////////////////////////////////////////
		sUniqueShip.bCanFire = aOldUniqueShip[i].bCanFire;
		sUniqueShip.bCanManoeuvring = aOldUniqueShip[i].bCanManoeuvring;
		sUniqueShip.bPlayer = aOldUniqueShip[i].bPlayer;
		wcscpy_s(sUniqueShip.cObjectName, SIZE_STRING, aOldUniqueShip[i].cObjectName);
		wcscpy_s(sUniqueShip.cShipName, SIZE_STRING, aOldUniqueShip[i].cShipName);
		wcscpy_s(sUniqueShip.cTexBumpName, SIZE_STRING, aOldUniqueShip[i].cTexBumpName);
		wcscpy_s(sUniqueShip.cTexLife, SIZE_STRING, aOldUniqueShip[i].cTexLife);
		wcscpy_s(sUniqueShip.cTexLifeMask, SIZE_STRING, aOldUniqueShip[i].cTexLifeMask);
		sUniqueShip.iTexLife = aOldUniqueShip[i].iTexLife;
		wcscpy_s(sUniqueShip.cTexName, SIZE_STRING, aOldUniqueShip[i].cTexName);
		sUniqueShip.fCollisionA = aOldUniqueShip[i].fCollisionA;
		sUniqueShip.fCollisionB = aOldUniqueShip[i].fCollisionB;
		sUniqueShip.i3DObjIndex = aOldUniqueShip[i].i3DObjIndex;
		sUniqueShip.iTexIndex = aOldUniqueShip[i].iTexIndex;
		sUniqueShip.iTexBumpIndex = aOldUniqueShip[i].iTexBumpIndex;
		sUniqueShip.iStarsShield = aOldUniqueShip[i].iStarsShield;
		sUniqueShip.iStarsArmour = aOldUniqueShip[i].iStarsArmour;
		sUniqueShip.iStarsArmament = aOldUniqueShip[i].iStarsArmament;
		sUniqueShip.iStarsManoeuvring = aOldUniqueShip[i].iStarsManoeuvring;
		sUniqueShip.iEmitterNumber = aOldUniqueShip[i].iEmitterNumber;

		g_aUniqueShip.push_back( sUniqueShip );
		if(aOldUniqueShip[i].iEmitterNumber <= 0){
			g_aUniqueShip[i].iEmitterNumber = 0;
			g_aUniqueShip[i].psEmitterData = NULL;
		}
		else{
			g_aUniqueShip[i].psEmitterData = new SEmitterData[g_aUniqueShip[i].iEmitterNumber];
			for(int j = 0; j < aOldUniqueShip[i].iEmitterNumber; ++j){
				g_aUniqueShip[i].psEmitterData[j].iIndex = aOldUniqueShip[i].psEmitterData[j].iIndex;
				g_aUniqueShip[i].psEmitterData[j].fEmitterPosX = aOldUniqueShip[i].psEmitterData[j].fEmitterPosX;
				g_aUniqueShip[i].psEmitterData[j].fEmitterPosY = aOldUniqueShip[i].psEmitterData[j].fEmitterPosY;
				g_aUniqueShip[i].psEmitterData[j].fEmitterPosZ = aOldUniqueShip[i].psEmitterData[j].fEmitterPosZ;
				g_aUniqueShip[i].psEmitterData[j].iTexEmitter = aOldUniqueShip[i].psEmitterData[j].iTexEmitter;
				g_aUniqueShip[i].psEmitterData[j].fR = aOldUniqueShip[i].psEmitterData[j].fR;
				g_aUniqueShip[i].psEmitterData[j].fG = aOldUniqueShip[i].psEmitterData[j].fG;
				g_aUniqueShip[i].psEmitterData[j].fB = aOldUniqueShip[i].psEmitterData[j].fB;
				g_aUniqueShip[i].psEmitterData[j].iParticlesNumber = aOldUniqueShip[i].psEmitterData[j].iParticlesNumber;
			}
		}
	}

	//zapisanie
	SaveUniqueShips();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//ConvertStruct();
	LoadUniqueShips();

	char cMenu = 0;
	do{
		system("cls");
		cout << "SHIPS EDITOR for Space Shooter project. Version: " << VERSION << endl;
		cout << endl << "MAIN MENU" << endl;
		cout << "[A]dd new object" << endl;
		cout << "[B]rowse objects" << endl;
		cout << "[E]xit" << endl;
		cout << "Press proper key and confirm [Enter]: ";
		cin >> cMenu;

		switch(cMenu){
			case 'b':
			case 'B':
				BrowseShips();
				break;
			case 'a':
			case 'A':
				AddShip();
				break;
		}
	}
	while(cMenu != 'e' && cMenu != 'E');

	//sprzatanie
	ClearUniqueShips();

	return 0;
}

void BrowseShips()
{
	char cMenu = 0;
	int iIndex = 0; //aktualny index w tablicy

	do{
		system("cls");
		cout << "SHIPS BROWSE. Actual record number: " << iIndex + 1 << "/" << GetUniqueShipSize() << endl;
		if(GetUniqueShipSize() > 0){
			if(iIndex < 0 || iIndex >= GetUniqueShipSize())
				cout << "Incorrect array index" << endl;
			else
				PrintShipInfo(g_aUniqueShip[iIndex]);
		}
		else{
			iIndex = 0;
			cout << "Any objects" << endl;
		}

		cout << endl << "MENU BROWSE:" << endl;
		cout << "[P]revious object" << endl;
		cout << "[N]ext object" << endl;
		cout << "[E]dit object" << endl;
		cout << "[D]elete object" << endl;
		cout << "[M]ove down index" << endl;
		cout << "[B]ack to main menu" << endl;
		cout << "Press proper key and confirm [Enter]: ";
		cin >> cMenu;

		switch(cMenu){
			case 'p':
			case 'P':
				if(--iIndex < 0)
					iIndex = GetUniqueShipSize() - 1;
				break;
			case 'n':
			case 'N':
				if(++iIndex >= GetUniqueShipSize())
					iIndex = 0;
				break;
			case 'e':
			case 'E':
				EditShip(iIndex);
				break;
			case 'd':
			case 'D':
				DeleteShip(iIndex);
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

//wczytanie wszystkich statkow i utworzenie tablicy g_aUniqueShip
void LoadUniqueShips()
{
	ClearUniqueShips();
	FILE *f;
	SUniqueShip sUniqueShip;
	errno_t error = _wfopen_s(&f, FILE_NAME_SHIPS_EDIT2, _T("rb"));
	if(error != 0){
		cout << "LoadUniqueShips: open file error: " << error << endl;
		system("pause");
		return;
	}
	while( fread(&sUniqueShip, sizeof(SUniqueShip), 1, f) != NULL){
		g_aUniqueShip.push_back( sUniqueShip );
		int iIndex = GetUniqueShipSize() - 1;
		if(g_aUniqueShip[iIndex].iEmitterNumber > 0){
			g_aUniqueShip[iIndex].psEmitterData = new SEmitterData[g_aUniqueShip[iIndex].iEmitterNumber];
			for(int i = 0; i < g_aUniqueShip[iIndex].iEmitterNumber; ++i){
				fread(&g_aUniqueShip[iIndex].psEmitterData[i], sizeof(SEmitterData), 1, f);
			}
		}
		else
			g_aUniqueShip[iIndex].psEmitterData = NULL;
	}
	fclose(f);
}

//nadpisuje plik
bool SaveUniqueShips()
{
	FILE *f;
	errno_t error = _wfopen_s(&f, FILE_NAME_SHIPS_EDIT2, _T("wb"));
	if(error != 0){
		cout << "SaveUniqueShips: open file error: " << error << endl;
		system("pause");
		return false;
	}

	for(int i = 0; i < GetUniqueShipSize(); ++i){
		fwrite(&g_aUniqueShip[i], sizeof(SUniqueShip), 1, f);
		for(int j = 0; j < g_aUniqueShip[i].iEmitterNumber; ++j){
			fwrite(&g_aUniqueShip[i].psEmitterData[j], sizeof(SEmitterData), 1, f);
		}
	}

	fclose(f);

	return true;
}

void ClearUniqueShips()
{
	for(int i = GetUniqueShipSize() - 1; i >= 0; --i){
		if(g_aUniqueShip[i].psEmitterData)
			delete [] g_aUniqueShip[i].psEmitterData;
	}
	g_aUniqueShip.clear();
}

int GetUniqueShipSize()
{
	return static_cast<int>(g_aUniqueShip.size());
}

void PrintShipInfo(SUniqueShip &sUniqueShip)
{
	cout << "Name of ship: ";
	PrintTCHAR(sUniqueShip.cShipName);

	cout << "Name of file: ";
	PrintTCHAR(sUniqueShip.cObjectName);

	if(sUniqueShip.bPlayer)
		cout << "Player's ship" << endl;
	else
		cout << "Enemy's ship" << endl;

	cout << "Type of texture: ";
	switch(sUniqueShip.byTexType){
		case 0:
			cout << "normal" << endl;
			cout << "File name of textures: ";
			PrintTCHAR(sUniqueShip.cTexName, false);
			cout << " ";
			PrintTCHAR(sUniqueShip.cTexBumpName);
			break;
		case 1:
		{
			cout << "muli mask with COLORREF" << endl;
			cout << "File name of textures: ";
			PrintTCHAR(sUniqueShip.cTexName, false);
			int r = GetRValue(sUniqueShip.uiCOLORREF);
			int g = GetGValue(sUniqueShip.uiCOLORREF);
			int b = GetBValue(sUniqueShip.uiCOLORREF);
			cout << " RGB(" << r << ", " << g << ", " << b << ")" << endl;
			break;
		}
		case 2:
			cout << "none" << endl;
			break;
		default:
			break;
	}

	cout << "File names of life textures: ";
	PrintTCHAR(sUniqueShip.cTexLife, false);
	cout << " ";
	PrintTCHAR(sUniqueShip.cTexLifeMask);

	cout << "Shield stars: " << sUniqueShip.iStarsShield << endl;
	cout << "Armour stars: " << sUniqueShip.iStarsArmour << endl;
	cout << "Armament stars: " << sUniqueShip.iStarsArmament << endl;
	cout << "Manoeuvring stars: " << sUniqueShip.iStarsManoeuvring << endl;

	cout << "Collision values A: " << sUniqueShip.fCollisionA << ", B: " << sUniqueShip.fCollisionB << endl;

	if(sUniqueShip.bCanFire)
		cout << "Can fire" << endl;
	else
		cout << "Cannot fire" << endl;

	if(sUniqueShip.bCanManoeuvring)
		cout << "Can manoeuvring" << endl;
	else
		cout << "Cannot manoeuvring" << endl;

	cout << "Number of emitter: " << sUniqueShip.iEmitterNumber << endl;

	//drukuj info emiterow
	for(int i = 0; i < sUniqueShip.iEmitterNumber; ++i){
		cout << endl << "Emitter:" << i << endl;
		//printf("%f\n", sUniqueShip.psEmitterData[i].fEmitterPosX);
		cout << "Position X: " << sUniqueShip.psEmitterData[i].fEmitterPosX << ", Y: " << sUniqueShip.psEmitterData[i].fEmitterPosY << ", Z: " << sUniqueShip.psEmitterData[i].fEmitterPosZ << endl;
		cout << "Number of texture: " <<  sUniqueShip.psEmitterData[i].iTexEmitter << endl;
		cout << "Colour R: " << sUniqueShip.psEmitterData[i].fR << ", G: " << sUniqueShip.psEmitterData[i].fG << ", B: " << sUniqueShip.psEmitterData[i].fB << endl;
		cout << "Number of particles: " << sUniqueShip.psEmitterData[i].iParticlesNumber << endl;
	}
}

void DeleteShip(int iIndex)
{
	if(iIndex < 0 || iIndex >= GetUniqueShipSize()){
		cout << "DeleteShip: Incorrect index array" << endl;
		system("pause");
		return;
	}

	char cChoice = 0;
	cout << "Delete ship number " << iIndex << ". Are you sure? (y/n) :";
	cin >> cChoice;

	if(cChoice != 'y' && cChoice != 'Y')
		return;

	if(g_aUniqueShip[iIndex].psEmitterData)
		delete [] g_aUniqueShip[iIndex].psEmitterData;
	g_aUniqueShip.erase( g_aUniqueShip.begin() + iIndex );

	SaveUniqueShips();
	cout << "Delete ship OK" << endl;
	system("pause");

}

void EditShip(int iIndex)
{
	if(iIndex < 0 || iIndex >= GetUniqueShipSize()){
		cout << "EditShip: Incorrect index array" << endl;
		system("pause");
		return;
	}

	char cChange = 0;
	cout << "Edit ship number " << iIndex << ". Are you sure? (y/n) :";
	cin >> cChange;
	if(cChange != 'y' && cChange != 'Y')
		return;

	cout << "File name: ";
	PrintTCHAR(g_aUniqueShip[iIndex].cObjectName);
	cout << "Change file name (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterFileName(g_aUniqueShip[iIndex]);

	cout << "Ship name: ";
	PrintTCHAR(g_aUniqueShip[iIndex].cShipName);
	cout << "Change ship name (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterShipName(g_aUniqueShip[iIndex]);

	if(g_aUniqueShip[iIndex].bPlayer)
		cout << "Player ship" << endl;
	else
		cout << "Enemy ship" << endl;
	cout << "Change player/enemy ship (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterIsPlayerShip(g_aUniqueShip[iIndex]);

	cout << "Texture type: ";
	switch(g_aUniqueShip[iIndex].byTexType){
		case 0:
			cout << "normal" << endl;
			cout << "Textures name: ";
			PrintTCHAR(g_aUniqueShip[iIndex].cTexName, false);
			cout << " ";
			PrintTCHAR(g_aUniqueShip[iIndex].cTexBumpName);
			break;
		case 1:
		{
			cout << "muli mask with COLORREF" << endl;
			cout << "Textures name: ";
			PrintTCHAR(g_aUniqueShip[iIndex].cTexName, false);
			int r = GetRValue(g_aUniqueShip[iIndex].uiCOLORREF);
			int g = GetGValue(g_aUniqueShip[iIndex].uiCOLORREF);
			int b = GetBValue(g_aUniqueShip[iIndex].uiCOLORREF);
			cout << " RGB(" << r << ", " << g << ", " << b << ")" << endl;
			break;
		}
		default:
			break;
	}
	cout << "Change texture type (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterTextureType(g_aUniqueShip[iIndex]);
	/*
	cout << "Textures files name: ";
	PrintTCHAR(g_aUniqueShip[iIndex].cTexName, false);
	cout << ", ";
	PrintTCHAR(g_aUniqueShip[iIndex].cTexBumpName);
	cout << "Change textures file name (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterTextureFileName_Type0(g_aUniqueShip[iIndex]);
	*/
	cout << "Textures for life: ";
	PrintTCHAR(g_aUniqueShip[iIndex].cTexLife, false);
	cout << ", ";
	PrintTCHAR(g_aUniqueShip[iIndex].cTexLifeMask);
	cout << "Change textures for life (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterTextureLifeName(g_aUniqueShip[iIndex]);

	cout << "Collision A: " << g_aUniqueShip[iIndex].fCollisionA << endl;
	cout << "Change collision A (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterCollisionA(g_aUniqueShip[iIndex]);

	cout << "Collision B: " << g_aUniqueShip[iIndex].fCollisionB << endl;
	cout << "Change collision B (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterCollisionB(g_aUniqueShip[iIndex]);

	if(g_aUniqueShip[iIndex].bCanFire)
		cout << "Can fire." << endl;
	else
		cout << "Cannot fire." << endl;
	cout << "Change can fire (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterCanFire(g_aUniqueShip[iIndex]);

	if(g_aUniqueShip[iIndex].bCanManoeuvring)
		cout << "Can manoeuvring." << endl;
	else
		cout << "Cannot manoeuvring." << endl;
	cout << "Change can manoeuvring (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterCanManoeuvring(g_aUniqueShip[iIndex]);

	cout << "Shield stars: " << g_aUniqueShip[iIndex].iStarsShield << endl;
	cout << "Change shield stars (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterShieldStars(g_aUniqueShip[iIndex]);

	cout << "Armour stars: " << g_aUniqueShip[iIndex].iStarsArmour << endl;
	cout << "Change armour stars (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterArmourStars(g_aUniqueShip[iIndex]);

	cout << "Armament stars: " << g_aUniqueShip[iIndex].iStarsArmament << endl;
	cout << "Change armament stars (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterArmamentStars(g_aUniqueShip[iIndex]);

	cout << "Manoeuvring stars: " << g_aUniqueShip[iIndex].iStarsManoeuvring << endl;
	cout << "Change manoeuvring stars (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y')
		EnterManoeuvringStars(g_aUniqueShip[iIndex]);

	cout << "Emitter number: " << g_aUniqueShip[iIndex].iEmitterNumber << endl;
	cout << "Change emitter number (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y'){
		EnterEmitterNumber(g_aUniqueShip[iIndex]);

		if(g_aUniqueShip[iIndex].psEmitterData){
			delete g_aUniqueShip[iIndex].psEmitterData;
		}

		if(g_aUniqueShip[iIndex].iEmitterNumber <= 0){
			g_aUniqueShip[iIndex].psEmitterData = NULL;
		}
		else{
			g_aUniqueShip[iIndex].psEmitterData = new SEmitterData[g_aUniqueShip[iIndex].iEmitterNumber];
			AddEmitter(g_aUniqueShip[iIndex]);
		}
	}

	cout << "Save change (y/n)?: ";
	cin >> cChange;
	if(cChange == 'y' || cChange == 'Y'){
		if(SaveUniqueShips())
			cout << "Save ship OK!" << endl;
	}
	system("pause");
}

void AddShip()
{
	SUniqueShip sUniqueShip;

	EnterFileName(sUniqueShip);
	EnterShipName(sUniqueShip);
	EnterIsPlayerShip(sUniqueShip);
	EnterTextureType(sUniqueShip);
	EnterTextureLifeName(sUniqueShip);
	EnterCollisionA(sUniqueShip);
	EnterCollisionB(sUniqueShip);
	EnterCanFire(sUniqueShip);
	EnterCanManoeuvring(sUniqueShip);
	EnterShieldStars(sUniqueShip);
	EnterArmourStars(sUniqueShip);
	EnterArmamentStars(sUniqueShip);
	EnterManoeuvringStars(sUniqueShip);
	EnterEmitterNumber(sUniqueShip);

	if(sUniqueShip.iEmitterNumber < 0){
		sUniqueShip.iEmitterNumber = 0;
		sUniqueShip.psEmitterData = NULL;
	}

	if(sUniqueShip.iEmitterNumber > 0){
		sUniqueShip.psEmitterData = new SEmitterData[sUniqueShip.iEmitterNumber];
		AddEmitter(sUniqueShip);
	}

	if(SaveUniqueShip(sUniqueShip))
		cout << "Save ship OK!" << endl;
	else
		cout << "Save ship failed!" << endl;

	system("pause");

	//wczytanie tablicy na nowo
	LoadUniqueShips();
}

void AddEmitter(SUniqueShip &sUniqueShip)
{
	for(int i = 0; i < sUniqueShip.iEmitterNumber; ++i){
		cout << endl << "Emitter number: " << i << endl;
		cout << "Enter position X emitter (float): ";
		cin >> sUniqueShip.psEmitterData[i].fEmitterPosX;
		cout << "Enter position Y emitter (float): ";
		cin >> sUniqueShip.psEmitterData[i].fEmitterPosY;
		cout << "Enter position Z emitter (float): ";
		cin >> sUniqueShip.psEmitterData[i].fEmitterPosZ;
		cout << "Enter number texture particle (int): ";
		cin >> sUniqueShip.psEmitterData[i].iTexEmitter;
		cout << "Enter colour R (float 1.0f - 0.0f): ";
		cin >> sUniqueShip.psEmitterData[i].fR;
		cout << "Enter colour G (float 1.0f - 0.0f): ";
		cin >> sUniqueShip.psEmitterData[i].fG;
		cout << "Enter colour B (float 1.0f - 0.0f): ";
		cin >> sUniqueShip.psEmitterData[i].fB;
		cout << "Enter particles number (int): ";
		cin >> sUniqueShip.psEmitterData[i].iParticlesNumber;
	}
}

void EnterFileName(SUniqueShip &sUniqueShip)
{
	char cTempText[128];
	char cTemp[128];
	cout << "Enter file name (string - only name): ";
	cin >> cTempText;
	strcpy_s(cTemp, sizeof(cTemp), "objects/");
	strcat_s(cTemp, sizeof(cTemp), cTempText);
	strcat_s(cTemp, sizeof(cTemp), ".3DObj");
	ConvertChar2TCHAR(cTemp, sUniqueShip.cObjectName);
}

void EnterShipName(SUniqueShip &sUniqueShip)
{
	char cTempText[128];
	cout << "Enter ship name (string): ";
	cin >> cTempText;
	ConvertChar2TCHAR(cTempText, sUniqueShip.cShipName);
}

void EnterIsPlayerShip(SUniqueShip &sUniqueShip)
{
	char cMark;
	cout << "Player ship (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y')
		sUniqueShip.bPlayer = 1;
	else
		sUniqueShip.bPlayer = 0;
}

void EnterTextureType(SUniqueShip &sUniqueShip)
{
	int x;
	do{
		cout << "Enter texture type (0 - normal, 1 - multi mask with COLORREF, 2 - none): ";
		cin >> x;
		if(x < 0 || x > 2)
			cout << "Wrong data!!" << endl;
	}
	while(x < 0 || x > 2);
	sUniqueShip.byTexType = static_cast<unsigned char>(x);

	switch(sUniqueShip.byTexType){
		case 0:
			EnterTextureFileName_Type0(sUniqueShip);
			break;
		case 1:
			EnterTextureFileName_Type1(sUniqueShip);
			break;
		case 2:
			EnterTextureFileName_Type2(sUniqueShip);
			break;
	}
}

void EnterTextureFileName_Type0(SUniqueShip &sUniqueShip)
{
	char cTempText[128];
	char cTemp[128];
	char cMark;

	cout << "Enter texture file name (string - only name): ";
	cin >> cTempText;
	strcpy_s(cTemp, sizeof(cTemp), "textures/");
	strcat_s(cTemp, cTempText);
	strcat_s(cTemp, ".jeh");
	ConvertChar2TCHAR(cTemp, sUniqueShip.cTexName);

	cout << "Is bump texture (y/n)?: ";
	cin >> cMark;

	if(cMark == 'y' || cMark == 'Y'){

		strcpy_s(cTemp, sizeof(cTemp), "textures/");
		strcat_s(cTemp, sizeof(cTemp), cTempText);
		strcat_s(cTemp, sizeof(cTemp), "bump.jeh");
		ConvertChar2TCHAR(cTemp, sUniqueShip.cTexBumpName);
		cout << "Texture bump file name (auto gen.): ";
		PrintTCHAR(sUniqueShip.cTexBumpName);
	}
	else 
		sUniqueShip.cTexBumpName[0] = 0;

	sUniqueShip.uiCOLORREF = 0;
}

void EnterTextureFileName_Type1(SUniqueShip &sUniqueShip)
{
	char cTempText[128];
	char cTemp[128];
	//unsigned char r, g, b;
	int r, g, b;

	cout << "Enter texture file name (string - only name): ";
	cin >> cTempText;
	strcpy_s(cTemp, sizeof(cTemp), "textures/");
	strcat_s(cTemp, sizeof(cTemp), cTempText);
	strcat_s(cTemp, sizeof(cTemp), ".jeh");
	ConvertChar2TCHAR(cTemp, sUniqueShip.cTexName);
	sUniqueShip.cTexBumpName[0] = 0; //bez bumpa

	do{
		cout << "Enter color R (int): ";
		cin >> r;
		if(r < 0 || r > 255) cout << "Wrong data!!" << endl;
	}
	while(r < 0 || r > 255);

	do{
		cout << "Enter color G (int): ";
		cin >> g;
		if(g < 0 || g > 255) cout << "Wrong data!!" << endl;
	}
	while(g < 0 || g > 255);

	do{
		cout << "Enter color B (int): ";
		cin >> b;
		if(b < 0 || b > 255) cout << "Wrong data!!" << endl;
	}
	while(b < 0 || b > 255);

	sUniqueShip.uiCOLORREF = RGB(BYTE(r), BYTE(g), BYTE(b));
}

void EnterTextureFileName_Type2(SUniqueShip &sUniqueShip)
{
	sUniqueShip.cTexName[0] = 0; //bez tekstury
	sUniqueShip.cTexBumpName[0] = 0; //bez tekstury bump
	sUniqueShip.uiCOLORREF = 0;
}

void EnterTextureLifeName(SUniqueShip &sUniqueShip)
{
	memset(sUniqueShip.cTexLife, 0, sizeof(sUniqueShip.cTexLife));
	memset(sUniqueShip.cTexLifeMask, 0, sizeof(sUniqueShip.cTexLifeMask));
	if(!sUniqueShip.bPlayer){
		return;
	}
	char cTempText[128];
	char cTemp[128];
	char cMark = 0;

	cout << "Is life textures (y/n)?: ";
	cin >> cMark;
	if(cMark != 'y' && cMark != 'Y'){
		return;
	}
	cout << "Enter texture name for life (string - only name): ";
	cin >> cTempText;
	strcpy_s(cTemp, sizeof(cTemp), "textures/");
	strcat_s(cTemp, sizeof(cTemp), cTempText);
	strcat_s(cTemp, sizeof(cTemp), ".jeh");
	ConvertChar2TCHAR(cTemp, sUniqueShip.cTexLife);

	strcpy_s(cTemp, sizeof(cTemp), "textures/");
	strcat_s(cTemp, sizeof(cTemp), cTempText);
	strcat_s(cTemp, sizeof(cTemp), "mask.jeh");
	ConvertChar2TCHAR(cTemp, sUniqueShip.cTexLifeMask);
	cout << "Texture life mask (auto gen.): ";
	PrintTCHAR(sUniqueShip.cTexLifeMask);
}

void EnterCollisionA(SUniqueShip &sUniqueShip)
{
	cout << "Enter collision A (float): ";
	cin >> sUniqueShip.fCollisionA;
}

void EnterCollisionB(SUniqueShip &sUniqueShip)
{
	cout << "Enter collision B (float): ";
	cin >> sUniqueShip.fCollisionB;
}

void EnterCanFire(SUniqueShip &sUniqueShip)
{
	char cMark;
	cout << "Can fire (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y')
		sUniqueShip.bCanFire = 1;
	else
		sUniqueShip.bCanFire = 0;
}

void EnterCanManoeuvring(SUniqueShip &sUniqueShip)
{
	char cMark;
	cout << "Can manoeuvring (y/n)?: ";
	cin >> cMark;
	if(cMark == 'y' || cMark == 'Y')
		sUniqueShip.bCanManoeuvring = 1;
	else
		sUniqueShip.bCanManoeuvring = 0;
}

void EnterShieldStars(SUniqueShip &sUniqueShip)
{
	cout << "Enter shield stars number (int): ";
	cin >> sUniqueShip.iStarsShield;
}

void EnterArmourStars(SUniqueShip &sUniqueShip)
{
	cout << "Enter armour stars number (int): ";
	cin >> sUniqueShip.iStarsArmour;
}

void EnterArmamentStars(SUniqueShip &sUniqueShip)
{
	cout << "Enter armament stars number (int): ";
	cin >> sUniqueShip.iStarsArmament;
}

void EnterManoeuvringStars(SUniqueShip &sUniqueShip)
{
	cout << "Enter manoeuvring stars number (int): ";
	cin >> sUniqueShip.iStarsManoeuvring;
}

void EnterEmitterNumber(SUniqueShip &sUniqueShip)
{
	if(sUniqueShip.bPlayer == 0){
		sUniqueShip.iEmitterNumber = 0;
		sUniqueShip.psEmitterData = NULL;
	}
	else{
		cout << "Enter emitter number (int): ";
		cin >> sUniqueShip.iEmitterNumber;
	}
}

bool SaveUniqueShip(SUniqueShip &sUniqueShip)
{
	FILE *f;
	errno_t error = _wfopen_s(&f, FILE_NAME_SHIPS_EDIT2, _T("ab"));
	if(error != 0){
		cout << "SaveUniqueShip : open file error: " << error << endl;
		system("pause");
		return false;
	}
	fwrite(&sUniqueShip, sizeof(SUniqueShip), 1, f);
	for(int i = 0; i < sUniqueShip.iEmitterNumber; ++i)
		fwrite(&sUniqueShip.psEmitterData[i], sizeof(SEmitterData), 1, f);
	fclose(f);
	return true;
}

void ConvertChar2TCHAR(char *cChar, TCHAR *cTCHAR)
{
	unsigned int i = 0;
	for( ; i < strlen(cChar); ++i){
		cTCHAR[i] = cChar[i];
	}
	cTCHAR[i] = 0;
}

void PrintTCHAR(TCHAR *cTCHAR, bool bEndl)
{
	for(unsigned int i = 0; i < wcslen(cTCHAR); ++i){
		printf("%c", cTCHAR[i]);
	}
	if(bEndl)
		cout << endl;
}

void MoveDownIndex(int iIndex)
{
	if(iIndex < 0 || iIndex > GetUniqueShipSize()){
		cout << "Error: incorrect index array" << endl;
		system("pause");
		return;
	}
	//strwadz czy aktualna pozycja nie jest ostatnia
	if(iIndex == GetUniqueShipSize() - 1){
		cout << "This index is last" << endl;
		system("pause");
		return;
	}

	SUniqueShip sTempShip;
	memset(&sTempShip, 0, sizeof(SUniqueShip));
	CopyUniqueShip(sTempShip, g_aUniqueShip[iIndex]);
	CopyUniqueShip(g_aUniqueShip[iIndex], g_aUniqueShip[iIndex+1]);
	CopyUniqueShip(g_aUniqueShip[iIndex+1], sTempShip);

	SaveUniqueShips();
}

void CopyUniqueShip(SUniqueShip &dest, SUniqueShip &src)
{
	if(dest.psEmitterData)
		delete [] dest.psEmitterData;

	dest.bCanFire = src.bCanFire;
	dest.bCanManoeuvring = src.bCanManoeuvring;
	dest.bPlayer = src.bPlayer;

	dest.iStarsShield = src.iStarsShield;
	dest.iStarsArmour = src.iStarsArmour;
	dest.iStarsArmament = src.iStarsArmament;
	dest.iStarsManoeuvring = src.iStarsManoeuvring;

	dest.fCollisionA = src.fCollisionA;
	dest.fCollisionB = src.fCollisionB;

	dest.iEmitterNumber = src.iEmitterNumber;

	wcscpy_s(dest.cTexName, SIZE_STRING/*sizeof(dest.cTexName)*/, src.cTexName);
	wcscpy_s(dest.cTexBumpName, SIZE_STRING/*sizeof(dest.cTexBumpName)*/, src.cTexBumpName);
	wcscpy_s(dest.cObjectName, SIZE_STRING/*sizeof(dest.cObjectName)*/, src.cObjectName);
	wcscpy_s(dest.cShipName, SIZE_STRING/*sizeof(dest.cShipName)*/, src.cShipName);

	if(dest.iEmitterNumber == 0)
		dest.psEmitterData = NULL;
	else{
		dest.psEmitterData = new SEmitterData[dest.iEmitterNumber];
		for(int i = 0; i < dest.iEmitterNumber; ++i){
			dest.psEmitterData[i].iIndex = 0;
			dest.psEmitterData[i].fEmitterPosX = src.psEmitterData[i].fEmitterPosX;
			dest.psEmitterData[i].fEmitterPosY = src.psEmitterData[i].fEmitterPosY;
			dest.psEmitterData[i].fEmitterPosZ = src.psEmitterData[i].fEmitterPosZ;
			dest.psEmitterData[i].iTexEmitter = src.psEmitterData[i].iTexEmitter;
			dest.psEmitterData[i].fR = src.psEmitterData[i].fR;
			dest.psEmitterData[i].fG = src.psEmitterData[i].fG;
			dest.psEmitterData[i].fB = src.psEmitterData[i].fB;
			dest.psEmitterData[i].iParticlesNumber = src.psEmitterData[i].iParticlesNumber;
		}
	}
}