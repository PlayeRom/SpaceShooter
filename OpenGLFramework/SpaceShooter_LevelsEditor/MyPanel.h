#pragma once

class CMyFrame;
class CLevelsManager;
class CSelectObjectDlg;

class CMyPanel : public wxPanel
{
public:
	CMyPanel(wxFrame *frame,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL);
    virtual ~CMyPanel();

	void LoadLevels(wxString strFileName);

	void Initialize();
	void OnListBoxLevels(wxCommandEvent &eEvent);
	void OnListBoxGroups(wxCommandEvent &eEvent);
	void OnCheckBoxZigzagFlight(wxCommandEvent &eEvent);
	void OnCheckBoxSlantFlight(wxCommandEvent &eEvent);
	void OnButtonIndexOfBoss(wxCommandEvent &eEvent);
	void OnButtonMoveUp(wxCommandEvent &eEvent);
	void OnButtonMoveDown(wxCommandEvent &eEvent);
	void OnButtonGroupAdd(wxCommandEvent &eEvent);
	void OnButtonGroupDelete(wxCommandEvent &eEvent);
	void OnButtonGroupSaveMod(wxCommandEvent &eEvent);
	void OnButtonLevelAdd(wxCommandEvent &eEvent);
	void OnButtonLevelDelete(wxCommandEvent &eEvent);
	void OnButtonLevelSaveMod(wxCommandEvent &eEvent);
	void OnButtonIndexOfObject(wxCommandEvent &eEvent);

private:
	enum{
		ID_EVENT_LISTBOX_LEVELS = 100,
		ID_EVENT_LISTBOX_GROUPS,
		ID_EVENT_CHECKBOX_ZIGZAGFLIGHT,
		ID_EVENT_CHECKBOX_SLANTFLIGHT,
		ID_EVENT_BUTTON_INDEX_OF_BOSS,
		ID_EVENT_BUTTON_MOVEUP,
		ID_EVENT_BUTTON_MOVEDOWN,
		ID_EVENT_BUTTON_GROUP_ADD,
		ID_EVENT_BUTTON_GROUP_DELETE,
		ID_EVENT_BUTTON_GROUP_SAVEMOD,
		ID_EVENT_BUTTON_LEVEL_ADD,
		ID_EVENT_BUTTON_LEVEL_DELETE,
		ID_EVENT_BUTTON_LEVEL_SAVEMOD,
		ID_EVENT_BUTTON_INDEX_OF_OBJ
	};
	wxString m_strFileName;
	bool FillLoadedLevelsList();
	void FillGroupsList(int iIndexLevel = -1);
	void SetItemLoadedLevelList(int iIndex);
	void SetItemGroupList(int iIndexLevel, int iIndexGroup);
	void FillAllControls(int iIndex);
	void FillAllControlsForGroup(int iIndexLevel, int iIndexGroup);
	void ClearAllGroupsControls();
	bool CheckWhetherStringIsFloat(wxString strSrc);
	bool CheckWhetherStringIsInteger(wxString strSrc);
	bool CheckCorrectnessLevelControl();
	bool CheckCorrectnessGroupControl();
	void FillStructLevel(SLevel &sLevel);
	void FillStructGroup(SShipsGroups &sGroup);
	CSelectObjectDlg* CreateDialog(int iActualIndex, const wxString& strName);

	wxListBox		*m_pListBoxLevels;
	wxButton		*m_pButtonMoveUp;
	wxButton		*m_pButtonMoveDown;

	wxStaticText	*m_pLabelNumberOfLevel;
	wxTextCtrl		*m_pTextCtrlNumberOfLevel;

	//wxStaticText	*m_pLabelIndexOfBoss;
	wxButton		*m_pButtonIndexOfBoss;
	wxTextCtrl		*m_pTextCtrlIndexOfBoss;

	wxCheckBox		*m_pCheckBoxWithBoss;

	wxStaticText	*m_pLabelMaxObjOnScreen;
	wxTextCtrl		*m_pTextCtrlMaxObjOnScreen;

	wxStaticText	*m_pLabelFrequencyOfShoot;
	wxTextCtrl		*m_pTextCtrlFrequencyOfShoot;

	wxStaticText	*m_pLabelChanceOnShot;
	wxTextCtrl		*m_pTextCtrlChanceOnShot;

	//////////// grupy //////////

	wxStaticText	*m_pLabelGroupsNumber;
	wxTextCtrl		*m_pTextCtrlGroupsNumber;

	wxListBox		*m_pListBoxGroups;

	//wxStaticText	*m_pLabelGroupIndexOfObject;
	wxButton		*m_pButtonIndexOfObject;
	wxTextCtrl		*m_pTextCtrlGroupIndexOfObject;

	wxStaticText	*m_pLabelGroupNumberOfObjects;
	wxTextCtrl		*m_pTextCtrlGroupNumberOfObjects;

	wxRadioBox		*m_pRadioGroupFormation;
	
	wxStaticText	*m_pLabelGroupWidthOfFlying;
	wxTextCtrl		*m_pTextCtrlGroupWidthOfFlying;

	wxStaticText	*m_pLabelGroupSpeed;
	wxTextCtrl		*m_pTextCtrlGroupSpeed;

	wxCheckBox		*m_pCheckGroupZigzagFlight;
	wxCheckBox		*m_pCheckBoxSlantFlight;

	wxStaticText	*m_pLabelGroupOffsetForSlant;
	wxTextCtrl		*m_pTextCtrlOffsetForSlant;
	
	wxButton		*m_pButtonGroupAdd;
	wxButton		*m_pButtonGroupDelete;
	wxButton		*m_pButtonGroupSaveModify;

	wxButton		*m_pButtonAddLevel;
	wxButton		*m_pButtonDeleteLevel;
	wxButton		*m_pButtonSaveModifyLevel;

	CLevelsManager		*m_pLevelsManager;
	CMyFrame			*m_pMyFrame;
	CSelectObjectDlg	*m_pSelectObjDlg;

	wxArrayString		m_strArrayChoices;
	bool m_bReady;

	DECLARE_EVENT_TABLE()
};
