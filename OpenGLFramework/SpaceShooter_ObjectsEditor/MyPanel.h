#pragma once

#include "UniqueShipsStructs.h"

class CMyFrame;
class CShipsManager;

class CMyPanel : public wxPanel
{
public:
	CMyPanel(wxFrame *frame,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL);
    virtual ~CMyPanel();

	void Initialize();

	void LoadUniqueShips(wxString strFileName);

	void OnListBoxObjects(wxCommandEvent &eEvent);
	void OnListBoxEmitters(wxCommandEvent &eEvent);
	void OnCheckBoxPlayer(wxCommandEvent &eEvent);
	void OnCheckBoxBumpTex(wxCommandEvent &eEvent);
	void OnRadioBoxTypeOfTexture(wxCommandEvent &eEvent);
	void OnButtonMoveUp(wxCommandEvent &eEvent);
	void OnButtonMoveDown(wxCommandEvent &eEvent);
	void OnButtonEmitterAdd(wxCommandEvent &eEvent);
	void OnButtonEmitterDelete(wxCommandEvent &eEvent);
	void OnButtonEmitterSaveMod(wxCommandEvent &eEvent);
	void OnButtonObjectAdd(wxCommandEvent &eEvent);
	void OnButtonObjectDelete(wxCommandEvent &eEvent);
	void OnButtonObjectSaveMod(wxCommandEvent &eEvent);
	void OnTextNameOfTexture(wxCommandEvent &eEvent);
	void OnTextNameOfLifeTexture(wxCommandEvent &eEvent);
	void OnButtonColourOfTexture(wxCommandEvent &eEvent);

private:
	enum{
		ID_EVENT_LISTBOX_OBJECTS = 100,
		ID_EVENT_LISTBOX_EMITTERS,
		ID_EVENT_CHECKBOX_PLAYER,
		ID_EVENT_BUTTON_COLOUR_TEXTURE,
		ID_EVENT_CHECKBOX_BUMP_TEX,
		ID_EVENT_RADIOBOX_TEX_TYPE,
		ID_EVENT_BUTTON_MOVEUP,
		ID_EVENT_BUTTON_MOVEDOWN,
		ID_EVENT_BUTTON_EMITTER_ADD,
		ID_EVENT_BUTTON_EMITTER_DELETE,
		ID_EVENT_BUTTON_EMITTER_SAVEMOD,
		ID_EVENT_BUTTON_OBJ_ADD,
		ID_EVENT_BUTTON_OBJ_DELETE,
		ID_EVENT_BUTTON_OBJ_SAVEMOD,
		ID_EVENT_TEXT_NAME_OF_TEX,
		ID_EVENT_TEXT_NAME_OF_LIFETEX
	};
	wxString m_strFileName;
	bool FillLoadedObjectsList();
	void FillEmittersList(int iIndexObj = -1);
	void SetItemLoadedObjectsList(int iIndex);
	void SetItemEmitterList(int iIndexObj, int iIndexEmitter);
	void FillAllControls(int iIndex);
	void FillAllControlsForEmitter(int iIndexObj, int iIndexEmitter);
	void SetVisibleControlsForTextureMode();
	void ClearAllEmitterControls();
	wxString CutString(wxString strFirst, wxString strLast, wxString strSrc);
	bool CheckWhetherStringIsFloat(wxString strSrc);
	bool CheckWhetherStringIsInteger(wxString strSrc);
	bool CheckCorrectnessObjectControl();
	bool CheckCorrectnessEmitterControl();
	void FillStructUniqueShip(SUniqueShip &sShip);
	void FillStructEmitter(SEmitterData &sEmitter);

	wxListBox		*m_pListBoxObjects;
	wxButton		*m_pButtonMoveUp;
	wxButton		*m_pButtonMoveDown;

	wxStaticText	*m_pLabelIndex;
	wxTextCtrl		*m_pTextCtrlIndex;

	wxStaticText	*m_pLabelNameOfShip;
	wxTextCtrl		*m_pTextCtrlNameOfShip;

	wxStaticText	*m_pLabelNameOfFile1;
	wxTextCtrl		*m_pTextCtrlNameOfFile;
	wxStaticText	*m_pLabelNameOfFile2;

	wxCheckBox		*m_pCheckBoxPlayer;

	wxRadioBox		*m_pRadioTypeOfTexture;

	wxStaticText	*m_pLabelNameOfTexture1;
	wxTextCtrl		*m_pTextCtrlNameOfTexture;
	wxStaticText	*m_pLabelNameOfTexture2;

	wxCheckBox		*m_pCheckBoxIsBumpTexture;

	wxStaticText	*m_pLabelNameOfBumpTexture1;
	wxTextCtrl		*m_pTextCtrlNameOfBumpTexture;
	wxStaticText	*m_pLabelNameOfBumpTexture2;

	wxButton		*m_pButtonColourOfMask;
	wxStaticText	*m_pLabelColourOfMaskR;
	wxTextCtrl		*m_pTextCtrlColourOfTextureR;
	wxStaticText	*m_pLabelColourOfMaskG;
	wxTextCtrl		*m_pTextCtrlColourOfTextureG;
	wxStaticText	*m_pLabelColourOfMaskB;
	wxTextCtrl		*m_pTextCtrlColourOfTextureB;

	wxStaticText	*m_pLabelNameOfLifeTexture1;
	wxTextCtrl		*m_pTextCtrlNameOfLifeTexture;
	wxStaticText	*m_pLabelNameOfLifeTexture2;

	wxStaticText	*m_pLabelNameOfLifeTextureMask1;
	wxTextCtrl		*m_pTextCtrlNameOfLifeTextureMask;
	wxStaticText	*m_pLabelNameOfLifeTextureMask2;

	wxStaticText	*m_pLabelCollisionA;
	wxTextCtrl		*m_pTextCtrlCollisionA;

	wxStaticText	*m_pLabelCollisionB;
	wxTextCtrl		*m_pTextCtrlCollisionB;

	wxCheckBox		*m_pCheckBoxCanFire;
	wxCheckBox		*m_pCheckBoxCanManoeuvre;

	wxStaticText	*m_pLabelStarsShield;
	wxTextCtrl		*m_pTextCtrlStarsShield;
	wxStaticText	*m_pLabelStarsArmour;
	wxTextCtrl		*m_pTextCtrlStarsArmour;
	wxStaticText	*m_pLabelStarsArmament;
	wxTextCtrl		*m_pTextCtrlStarsArmament;
	wxStaticText	*m_pLabelStarsManoeuvring;
	wxTextCtrl		*m_pTextCtrlStarsManoeuvring;

	wxStaticText	*m_pLabelEmitterNumber;
	wxTextCtrl		*m_pTextCtrlEmitterNumber;

	wxListBox		*m_pListBoxEmitters;

	wxStaticText	*m_pLabelEmitterIndexOfEmitter;
	wxTextCtrl		*m_pTextCtrlEmitterIndexOfEmitter;

	wxStaticText	*m_pLabelEmitterPosX;
	wxTextCtrl		*m_pTextCtrlEmitterPosX;
	wxStaticText	*m_pLabelEmitterPosY;
	wxTextCtrl		*m_pTextCtrlEmitterPosY;
	wxStaticText	*m_pLabelEmitterPosZ;
	wxTextCtrl		*m_pTextCtrlEmitterPosZ;

	wxStaticText	*m_pLabelEmitterIndexOfTexture;
	wxTextCtrl		*m_pTextCtrlEmitterIndexOfTexture;

	wxStaticText	*m_pLabelEmitterColourR;
	wxTextCtrl		*m_pTextCtrlEmitterColourR;
	wxStaticText	*m_pLabelEmitterColourG;
	wxTextCtrl		*m_pTextCtrlEmitterColourG;
	wxStaticText	*m_pLabelEmitterColourB;
	wxTextCtrl		*m_pTextCtrlEmitterColourB;

	wxStaticText	*m_pLabelEmitterNumberOfParticles;
	wxTextCtrl		*m_pTextCtrlEmitterNumberOfParticles;

	wxButton		*m_pButtonEmitterAdd;
	wxButton		*m_pButtonEmitterDelete;
	wxButton		*m_pButtonEmitterSaveModify;

	wxButton		*m_pButtonAddObj;
	wxButton		*m_pButtonDeleteObj;
	wxButton		*m_pButtonSaveModifyObj;

	CMyFrame		*m_pMyFrame;
	CShipsManager	*m_pShipManager;

	bool m_bReady;

	DECLARE_EVENT_TABLE()
};
