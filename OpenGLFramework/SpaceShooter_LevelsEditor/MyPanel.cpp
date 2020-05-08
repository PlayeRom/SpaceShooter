#include "StdAfx.h"
#include "MyFrame.h"
#include "LevelsManager.h"
#include "SelectObjectDlg.h"
#include "MyPanel.h"

//#include <vld.h>

BEGIN_EVENT_TABLE( CMyPanel, wxPanel )
	EVT_LISTBOX( ID_EVENT_LISTBOX_LEVELS,		CMyPanel::OnListBoxLevels )
	EVT_LISTBOX( ID_EVENT_LISTBOX_GROUPS,		CMyPanel::OnListBoxGroups )
	EVT_CHECKBOX( ID_EVENT_CHECKBOX_ZIGZAGFLIGHT,CMyPanel::OnCheckBoxZigzagFlight )
	EVT_CHECKBOX( ID_EVENT_CHECKBOX_SLANTFLIGHT,	CMyPanel::OnCheckBoxSlantFlight )
	EVT_BUTTON( ID_EVENT_BUTTON_INDEX_OF_BOSS,	CMyPanel::OnButtonIndexOfBoss )
	EVT_BUTTON( ID_EVENT_BUTTON_MOVEUP,			CMyPanel::OnButtonMoveUp )
	EVT_BUTTON( ID_EVENT_BUTTON_MOVEDOWN,		CMyPanel::OnButtonMoveDown )
	EVT_BUTTON( ID_EVENT_BUTTON_GROUP_ADD,		CMyPanel::OnButtonGroupAdd )
	EVT_BUTTON( ID_EVENT_BUTTON_GROUP_DELETE,	CMyPanel::OnButtonGroupDelete )
	EVT_BUTTON( ID_EVENT_BUTTON_GROUP_SAVEMOD,	CMyPanel::OnButtonGroupSaveMod )
	EVT_BUTTON( ID_EVENT_BUTTON_LEVEL_ADD,		CMyPanel::OnButtonLevelAdd )
	EVT_BUTTON( ID_EVENT_BUTTON_LEVEL_DELETE,	CMyPanel::OnButtonLevelDelete )
	EVT_BUTTON( ID_EVENT_BUTTON_LEVEL_SAVEMOD,	CMyPanel::OnButtonLevelSaveMod )
	EVT_BUTTON( ID_EVENT_BUTTON_INDEX_OF_OBJ,	CMyPanel::OnButtonIndexOfObject )
END_EVENT_TABLE()

CMyPanel::CMyPanel( wxFrame *frame, const wxPoint& pos, const wxSize& size, long style )
       : wxPanel( frame, wxID_ANY, pos, size, style )
{
	m_bReady = false;

	m_pMyFrame = ( CMyFrame* )frame;

	( void )new wxStaticBox( this, wxID_ANY, _T("Loaded levels"), wxPoint( 5, 5 ), wxSize( 120, 500 ) );
	m_pListBoxLevels = new wxListBox( this, ID_EVENT_LISTBOX_LEVELS, wxPoint( 15, 25 ), wxSize( 100, 400 ), 0, 0 );
	m_pButtonMoveUp = new wxButton( this, ID_EVENT_BUTTON_MOVEUP, _T("Move up"), wxPoint( 15, 435 ), wxSize( 100, 24 ) );
	m_pButtonMoveDown = new wxButton( this, ID_EVENT_BUTTON_MOVEDOWN, _T("Move down"), wxPoint( 15, 469 ), wxSize( 100, 24 ) );

	wxColour colourReadOnly( 192, 192, 192 );
	int iStartPosY = 5; //pozycja Y pierwszych kontrolek
	int iOffsetPosY = 25;
	int iLabelLength = 150; //dlugosc napisow przed edit boxami
	int iLabelPosX = 5 + 130; //pozycja napisow przed edit boxami
	int iEditPosX = iLabelPosX + iLabelLength + 5;//210; //pozycja X edit boxow

	int iPosY = iStartPosY;

	m_pLabelNumberOfLevel = new wxStaticText( this, wxID_ANY, _T("Number of level:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlNumberOfLevel = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX, iPosY ), wxDefaultSize, wxTE_READONLY );
	m_pTextCtrlNumberOfLevel->SetBackgroundColour( colourReadOnly );
	iPosY += iOffsetPosY;
	//m_pLabelIndexOfBoss = new wxStaticText( this, wxID_ANY, _T("Index of boss:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pButtonIndexOfBoss = new wxButton( this, ID_EVENT_BUTTON_INDEX_OF_BOSS, _T("Index of boss:"), wxPoint( iLabelPosX, iPosY ), wxSize( 150, 20 ) );
	m_pTextCtrlIndexOfBoss = new wxTextCtrl( this, wxID_ANY, _T("-1"), wxPoint( iEditPosX, iPosY ) );
	m_pTextCtrlIndexOfBoss->SetToolTip( _T("When -1 then level without boss") );
	//m_pCheckBoxWithBoss = new wxCheckBox( this, wxID_ANY, _T("Level with boss"), wxPoint( iEditPosX, iPosY ) );
	iPosY += iOffsetPosY;
	m_pLabelMaxObjOnScreen = new wxStaticText( this, wxID_ANY, _T("Max objects on screen:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlMaxObjOnScreen = new wxTextCtrl( this, wxID_ANY, _T("15"), wxPoint( iEditPosX, iPosY ) );
	m_pTextCtrlMaxObjOnScreen->SetToolTip( _T("Expect integer between ~15 and ~35") );
	iPosY += iOffsetPosY;
	m_pLabelFrequencyOfShoot = new wxStaticText( this, wxID_ANY, _T("Frequency of shoot:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlFrequencyOfShoot = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX, iPosY ) );
	m_pTextCtrlFrequencyOfShoot->SetToolTip( _T("Expect integer between 1 and 6. They value is smaller then quickly it shoots.") );
	iPosY += iOffsetPosY;
	m_pLabelChanceOnShot = new wxStaticText( this, wxID_ANY, _T("Chance on give shoot:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlChanceOnShot = new wxTextCtrl( this, wxID_ANY, _T("40"), wxPoint( iEditPosX, iPosY ) );
	m_pTextCtrlChanceOnShot->SetToolTip( _T("Expect integer between 0 and 100. Suggested rand between 40% and 85%.") );

	///////////////////////////////////
	iPosY += ( iOffsetPosY * 5 );
	m_pButtonAddLevel = new wxButton( this, ID_EVENT_BUTTON_LEVEL_ADD, _T("Add new level"), wxPoint( iEditPosX, iPosY ), wxSize( 150, 24 ) );
	iPosY += iOffsetPosY + 5;
	m_pButtonDeleteLevel = new wxButton( this, ID_EVENT_BUTTON_LEVEL_DELETE, _T("Delete selected level"), wxPoint( iEditPosX, iPosY ), wxSize( 150, 24 ) );
	iPosY += iOffsetPosY + 5;
	m_pButtonSaveModifyLevel = new wxButton( this, ID_EVENT_BUTTON_LEVEL_SAVEMOD, _T("Save modify for level"), wxPoint( iEditPosX, iPosY ), wxSize( 150, 24 ) );

	//////////////////////////////////////
	/////////////////////////////////////

	iPosY = iStartPosY;
	int iLabelPosX2 = iLabelPosX + iLabelLength + 100 + 100; //100 - dlugosc exit boxa
	iLabelLength = 150;// nowa dlugosc label-a
	int iEditPosX2 = iLabelPosX2 + iLabelLength + 5;

	( void )new wxStaticBox( this, wxID_ANY, _T("Gropus of objects/ships"), wxPoint( iLabelPosX2 - 10, iPosY ), wxSize( 330, 450 ) );

	iPosY += iOffsetPosY;
	m_pLabelGroupsNumber = new wxStaticText( this, wxID_ANY, _T("Number of groups:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlGroupsNumber = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX2, iPosY ), wxDefaultSize, wxTE_READONLY );
	m_pTextCtrlGroupsNumber->SetBackgroundColour( colourReadOnly );

	iPosY += iOffsetPosY;
	m_pListBoxGroups = new wxListBox( this, ID_EVENT_LISTBOX_GROUPS, wxPoint( iEditPosX2, iPosY ), wxSize( 100, 60 ), 0, 0 );

	iPosY += ( iOffsetPosY * 3 );
	//m_pLabelGroupIndexOfObject = new wxStaticText( this, wxID_ANY, _T("Index of object:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pButtonIndexOfObject = new wxButton( this, ID_EVENT_BUTTON_INDEX_OF_OBJ, _T("Index of object:"), wxPoint( iLabelPosX2, iPosY ), wxSize( 150, 20 ) );
	m_pTextCtrlGroupIndexOfObject = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX2, iPosY ) );
	m_pTextCtrlGroupIndexOfObject->SetToolTip( _T("Index of object from ships.dat file") );

	iPosY += iOffsetPosY;
	m_pLabelGroupNumberOfObjects = new wxStaticText( this, wxID_ANY, _T("Number of objects in group:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlGroupNumberOfObjects = new wxTextCtrl( this, wxID_ANY, _T("60"), wxPoint( iEditPosX2, iPosY ) );
	m_pTextCtrlGroupNumberOfObjects->SetToolTip( _T("Expect integer larger from 0.") );
	iPosY += iOffsetPosY;
	wxString strTypesOfFormation[] = { _T("1"), _T("2"), _T("3") };
	m_pRadioGroupFormation = new wxRadioBox( this, wxID_ANY, _T("Type of formation"), wxPoint( iEditPosX2, iPosY ), wxDefaultSize, WXSIZEOF( strTypesOfFormation ), strTypesOfFormation, 0 );
	iPosY += iOffsetPosY * 2;
	//to jest "Pos X for rand"
	m_pLabelGroupWidthOfFlying = new wxStaticText( this, wxID_ANY, _T("Width of flying the objects:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlGroupWidthOfFlying = new wxTextCtrl( this, wxID_ANY, _T("242"), wxPoint( iEditPosX2, iPosY ) );
	m_pTextCtrlGroupWidthOfFlying->SetToolTip( _T("Expect integer larger from 0. Standard: 242.") );
	iPosY += iOffsetPosY;
	m_pLabelGroupSpeed = new wxStaticText( this, wxID_ANY, _T("Speed:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlGroupSpeed = new wxTextCtrl( this, wxID_ANY, _T("120.0"), wxPoint( iEditPosX2, iPosY ) );
	m_pTextCtrlGroupSpeed->SetToolTip( _T("Expect float number. Suggested minimum: 120.0") );
	iPosY += iOffsetPosY;
	m_pCheckGroupZigzagFlight = new wxCheckBox( this, ID_EVENT_CHECKBOX_ZIGZAGFLIGHT, _T("Zigzag flight"), wxPoint( iEditPosX2, iPosY ) );
	iPosY += iOffsetPosY;
	m_pCheckBoxSlantFlight = new wxCheckBox( this, ID_EVENT_CHECKBOX_SLANTFLIGHT, _T("Slant flight"), wxPoint( iEditPosX2, iPosY ) );
	iPosY += iOffsetPosY;
	m_pLabelGroupOffsetForSlant = new wxStaticText( this, wxID_ANY, _T("Offset for slant:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlOffsetForSlant = new wxTextCtrl( this, wxID_ANY, _T("605.0"), wxPoint( iEditPosX2, iPosY ) );
	m_pTextCtrlOffsetForSlant->SetToolTip( _T("Expect float number. Standard: 605.0") );
	
	iPosY += iOffsetPosY + 5;
	m_pButtonGroupAdd = new wxButton( this, ID_EVENT_BUTTON_GROUP_ADD, _T("Add new group"), wxPoint( iEditPosX2, iPosY ), wxSize( 150, 24 ) );
	iPosY += iOffsetPosY + 5;
	m_pButtonGroupDelete = new wxButton( this, ID_EVENT_BUTTON_GROUP_DELETE, _T("Delete selected group"), wxPoint( iEditPosX2, iPosY ), wxSize( 150, 24 ) );
	iPosY += iOffsetPosY + 5;
	m_pButtonGroupSaveModify = new wxButton( this, ID_EVENT_BUTTON_GROUP_SAVEMOD, _T("Save modify for group"), wxPoint( iEditPosX2, iPosY ), wxSize( 150, 24 ) );

	m_strFileName = FILE_NAME_LEVEL_EDIT2;
	m_pLevelsManager = new CLevelsManager();
	//m_pLevelsManager->ConvertStruct();
	m_pSelectObjDlg = NULL;
	
	m_bReady = true;
}

void CMyPanel::Initialize()
{
	if( !FillLoadedLevelsList() )
		m_pMyFrame->OpenFile( true );
	SetItemLoadedLevelList( 0 );
}

CMyPanel::~CMyPanel()
{
	delete m_pLevelsManager;
}

void CMyPanel::LoadLevels( wxString strFileName )
{
	m_strFileName = strFileName;
	FillLoadedLevelsList();
	SetItemLoadedLevelList( 0 );
}

bool CMyPanel::FillLoadedLevelsList()
{
	bool bLoadOK = m_pLevelsManager->LoadLevels( m_strFileName.c_str() );
	m_pListBoxLevels->Clear();
	for( int i = 0; i < m_pLevelsManager->GetLevelsSize(); ++i ) {
		wxString str;
		str.Printf( _T("%2d Level"), i );
		m_pListBoxLevels->Append( str );
	}

	return bLoadOK;
}

void CMyPanel::FillGroupsList( int iIndexLevel )
{
	ClearAllGroupsControls();
	if( iIndexLevel == -1 )
		iIndexLevel = m_pListBoxGroups->GetSelection();
	if( iIndexLevel == -1 )
		return;
	wxString str;
	for( int i = 0; i < m_pLevelsManager->GetLevel( iIndexLevel ).iNumberGroups; ++i ) {
		str.Printf( _T("%2d Group"), i );
		m_pListBoxGroups->Append( str );
	}
}

void CMyPanel::SetItemLoadedLevelList( int iIndex )
{
	if( iIndex < 0 || iIndex >= m_pLevelsManager->GetLevelsSize() )
		return;

	if( iIndex < static_cast<int>( m_pListBoxLevels->GetCount() ) )
		m_pListBoxLevels->SetSelection( iIndex );
	FillAllControls( iIndex );
}

void CMyPanel::SetItemGroupList( int iIndexLevel, int iIndexGroup )
{
	if( iIndexLevel < 0 || iIndexLevel >= m_pLevelsManager->GetLevelsSize() )
		return;

	m_pCheckGroupZigzagFlight->Show( true );
	m_pCheckBoxSlantFlight->Show( true );
	m_pLabelGroupOffsetForSlant->Show( false );
	m_pTextCtrlOffsetForSlant->Show( false );

	if( iIndexGroup < 0 || iIndexGroup >= m_pLevelsManager->GetLevel( iIndexLevel ).iNumberGroups )
		return;

	if( iIndexGroup < static_cast<int>( m_pListBoxGroups->GetCount() ) )
		m_pListBoxGroups->SetSelection( iIndexGroup );
	FillAllControlsForGroup( iIndexLevel, iIndexGroup );
}

void CMyPanel::FillAllControls( int iIndex )
{
	wxString str;

	//UWAGA numer poziomu obliczamy z indeksu a nie podajemy bezposrednio
	//aby zmienic numer poziomu nalezy uzyc kalwiszy MoveUp i MoveDown
	//str.Printf( _T("%d"), m_pLevelsManager->GetLevel( iIndex ).iLevel );
	str.Printf( _T("%d"), iIndex + 1 );
	m_pTextCtrlNumberOfLevel->SetValue( str );

	//if( m_pLevelsManager->GetLevel( iIndex ).bWithBoss )
	//	m_pCheckBoxWithBoss->SetValue( true );
	//else
	//	m_pCheckBoxWithBoss->SetValue( false );

	str.Printf( _T("%d"), m_pLevelsManager->GetLevel( iIndex ).iIndexBoss );
	m_pTextCtrlIndexOfBoss->SetValue( str );

	str.Printf( _T("%d"), m_pLevelsManager->GetLevel( iIndex ).iMaxShipsOnScreen );
	m_pTextCtrlMaxObjOnScreen->SetValue( str );

	str.Printf( _T("%d"), m_pLevelsManager->GetLevel( iIndex ).iFrequencyShoot );
	m_pTextCtrlFrequencyOfShoot->SetValue( str );

	str.Printf( _T("%d"), m_pLevelsManager->GetLevel( iIndex ).iPercentChanceToShoot );
	m_pTextCtrlChanceOnShot->SetValue( str );

	//////////////////////////////////////////////////////
	//////////// ustaw emitery ///////////////////////////
	str.Printf( _T("%d"), m_pLevelsManager->GetLevel( iIndex ).iNumberGroups );
	m_pTextCtrlGroupsNumber->SetValue( str );

	FillGroupsList( iIndex );
	SetItemGroupList( iIndex, 0 );
}

void CMyPanel::FillAllControlsForGroup( int iIndexLevel, int iIndexGroup )
{
	wxString str;

	str.Printf( _T("%d"), m_pLevelsManager->GetLevel( iIndexLevel ).iNumberGroups );
	m_pTextCtrlGroupsNumber->SetValue( str );

	str.Printf( _T("%d"), m_pLevelsManager->GetLevel( iIndexLevel ).psShipsGroups[ iIndexGroup].iIndexShip );
	m_pTextCtrlGroupIndexOfObject->SetValue( str );

	str.Printf( _T("%d"), m_pLevelsManager->GetLevel( iIndexLevel ).psShipsGroups[ iIndexGroup].iNumberShips );
	m_pTextCtrlGroupNumberOfObjects->SetValue( str );

	m_pRadioGroupFormation->SetSelection( m_pLevelsManager->GetLevel( iIndexLevel ).psShipsGroups[ iIndexGroup].iFormation - 1 );

	str.Printf( _T("%d"), m_pLevelsManager->GetLevel( iIndexLevel ).psShipsGroups[ iIndexGroup].iPosXForRand );
	m_pTextCtrlGroupWidthOfFlying->SetValue( str );

	str.Printf( _T("%f"), m_pLevelsManager->GetLevel( iIndexLevel ).psShipsGroups[ iIndexGroup].fSpeed );
	m_pTextCtrlGroupSpeed->SetValue( str );

	if( m_pLevelsManager->GetLevel( iIndexLevel ).psShipsGroups[ iIndexGroup].bCosFly )
		m_pCheckGroupZigzagFlight->SetValue( true );
	else
		m_pCheckGroupZigzagFlight->SetValue( false );

	if( m_pLevelsManager->GetLevel( iIndexLevel ).psShipsGroups[ iIndexGroup].bSlantFly )
		m_pCheckBoxSlantFlight->SetValue( true );
	else
		m_pCheckBoxSlantFlight->SetValue( false );

	str.Printf( _T("%f"), m_pLevelsManager->GetLevel( iIndexLevel ).psShipsGroups[ iIndexGroup].fOffsetXPosForSlantFly );
	m_pTextCtrlOffsetForSlant->SetValue( str );

	m_pCheckGroupZigzagFlight->Show( true );
	m_pCheckBoxSlantFlight->Show( true );
	m_pLabelGroupOffsetForSlant->Show( true );
	m_pTextCtrlOffsetForSlant->Show( true );

	wxCommandEvent eEvent;
	OnCheckBoxZigzagFlight( eEvent );
	OnCheckBoxSlantFlight( eEvent );
}

void CMyPanel::ClearAllGroupsControls()
{
	m_pListBoxGroups->Clear();
	m_pTextCtrlGroupIndexOfObject->SetValue( _T("0") );
	m_pTextCtrlGroupNumberOfObjects->SetValue( _T("60") );
	m_pRadioGroupFormation->SetSelection( 0 );
	m_pTextCtrlGroupWidthOfFlying->SetValue( _T("242") );
	m_pTextCtrlGroupSpeed->SetValue( _T("120.0") );
	m_pCheckGroupZigzagFlight->SetValue( false );
	m_pCheckBoxSlantFlight->SetValue( false );
	m_pTextCtrlOffsetForSlant->SetValue( _T("605.0") );
}

void CMyPanel::OnListBoxLevels( wxCommandEvent &eEvent )
{
	SetItemLoadedLevelList( eEvent.GetInt() );
}

void CMyPanel::OnListBoxGroups( wxCommandEvent &eEvent )
{
	SetItemGroupList( m_pListBoxLevels->GetSelection(), eEvent.GetInt() );
}

void CMyPanel::OnCheckBoxZigzagFlight( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pCheckGroupZigzagFlight->GetValue() ) {
		m_pCheckBoxSlantFlight->Show( false );
		m_pCheckBoxSlantFlight->SetValue( false );
	}
	else {
		m_pCheckBoxSlantFlight->Show( true );
	}
}

void CMyPanel::OnCheckBoxSlantFlight( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pCheckBoxSlantFlight->GetValue() ) {
		m_pTextCtrlOffsetForSlant->Show( true );
		if(_wtof( m_pTextCtrlOffsetForSlant->GetValue() ) == 0.0f )
			m_pTextCtrlOffsetForSlant->SetValue( _T("605.0") );
		m_pLabelGroupOffsetForSlant->Show( true );
		m_pCheckGroupZigzagFlight->Show( false );
		m_pCheckGroupZigzagFlight->SetValue( false );
	}
	else {
		m_pTextCtrlOffsetForSlant->Show( false );
		m_pLabelGroupOffsetForSlant->Show( false );
		m_pCheckGroupZigzagFlight->Show( true );
	}
}

CSelectObjectDlg* CMyPanel::CreateDialog( int iActualIndex, const wxString& strName )
{
	m_strArrayChoices.Clear();
	for( int i = 0; i < m_pLevelsManager->GetShipsNumber(); ++i ) {
		wxString strIndex;
		strIndex.Printf( _T("%d. "), i );
		m_strArrayChoices.Add( strIndex + wxString( m_pLevelsManager->GetShipName( i ) ) );
	}
	return new CSelectObjectDlg( iActualIndex, m_strArrayChoices, this, -1, strName, wxPoint( -1, -1 ), wxSize( 240, 110 ), wxCAPTION|wxSYSTEM_MENU );
}

void CMyPanel::OnButtonIndexOfBoss( wxCommandEvent &WXUNUSED( eEvent ) )
{
	int iActualIndexOfBoss = _wtoi( m_pTextCtrlIndexOfBoss->GetValue() );
	m_pSelectObjDlg = CreateDialog( iActualIndexOfBoss, _T("Select object") );

	int iResult = m_pSelectObjDlg->ShowModal();
	if( iResult > -1 ) {
		wxString value;
		value.Printf( _T("%d"), iResult );
		m_pTextCtrlIndexOfBoss->SetValue( value );
	}

	m_pSelectObjDlg->Destroy();
	m_strArrayChoices.Clear();
}

void CMyPanel::OnButtonIndexOfObject( wxCommandEvent &WXUNUSED( eEvent ) )
{
	int iActualIndexOfObj = _wtoi( m_pTextCtrlGroupIndexOfObject->GetValue() );
	m_pSelectObjDlg = CreateDialog( iActualIndexOfObj, _T("Select object") );

	int iResult = m_pSelectObjDlg->ShowModal();
	if( iResult > -1 ) {
		wxString value;
		value.Printf( _T("%d"), iResult );
		m_pTextCtrlGroupIndexOfObject->SetValue( value );
	}

	m_pSelectObjDlg->Destroy();
	m_strArrayChoices.Clear();
}

void CMyPanel::OnButtonMoveUp( wxCommandEvent &WXUNUSED( eEvent ) )
{
	int iIndex = m_pListBoxLevels->GetSelection();
	if( m_pLevelsManager->MoveUpIndex( iIndex ) ) {
		FillLoadedLevelsList();
		SetItemLoadedLevelList( --iIndex );
	}
}

void CMyPanel::OnButtonMoveDown( wxCommandEvent &WXUNUSED( eEvent ) )
{
	int iIndex = m_pListBoxLevels->GetSelection();
	if( m_pLevelsManager->MoveDownIndex( iIndex ) ) {
		FillLoadedLevelsList();
		SetItemLoadedLevelList( ++iIndex );
	}
}

bool CMyPanel::CheckCorrectnessGroupControl()
{
	if( !CheckWhetherStringIsInteger( m_pTextCtrlGroupIndexOfObject->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsInteger( m_pTextCtrlGroupNumberOfObjects->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsInteger( m_pTextCtrlGroupWidthOfFlying->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsFloat( m_pTextCtrlGroupSpeed->GetValue() ) )
		return false;
	if( m_pCheckBoxSlantFlight->GetValue() ) {
		if( !CheckWhetherStringIsFloat( m_pTextCtrlOffsetForSlant->GetValue() ) )
			return false;
	}

	if(_wtoi( m_pTextCtrlGroupNumberOfObjects->GetValue() ) <= 0 ) {
		wxMessageBox( _T("Value for \"Number of objects in group\" must be larger from 0"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}

	if(_wtoi( m_pTextCtrlGroupWidthOfFlying->GetValue() ) <= 0 ) {
		wxMessageBox( _T("Value for \"Width of flying the objects\" must be larger from 0"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}

	return true;
}

void CMyPanel::FillStructGroup( SShipsGroups &sGroup )
{
	sGroup.iIndexShip = _wtoi( m_pTextCtrlGroupIndexOfObject->GetValue() );
	sGroup.iNumberShips = _wtoi( m_pTextCtrlGroupNumberOfObjects->GetValue() );
	sGroup.iFormation = m_pRadioGroupFormation->GetSelection() + 1;
	sGroup.iPosXForRand = _wtoi( m_pTextCtrlGroupWidthOfFlying->GetValue() );
	sGroup.fSpeed = _wtof( m_pTextCtrlGroupSpeed->GetValue() );
	sGroup.bCosFly = m_pCheckGroupZigzagFlight->GetValue();
	if( sGroup.bCosFly ) {
		sGroup.bSlantFly = 0;
		sGroup.fOffsetXPosForSlantFly = 0.0f;
	}
	else {
		sGroup.bSlantFly = m_pCheckBoxSlantFlight->GetValue();
		if( sGroup.bSlantFly )
			sGroup.fOffsetXPosForSlantFly = _wtof( m_pTextCtrlOffsetForSlant->GetValue() );
		else
			sGroup.fOffsetXPosForSlantFly = 0.0f;
	}
}

void CMyPanel::OnButtonGroupAdd( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( wxMessageBox( _T("From current data in controls will be created new group. Do you want to continue?"), _T("Add new group"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( !CheckCorrectnessGroupControl() )
		return;

	//wypelnij strukture danymi
	SShipsGroups sNewGroup;
	FillStructGroup( sNewGroup );

	if( m_pLevelsManager->AddNewGroup( m_pListBoxLevels->GetSelection(), sNewGroup ) ) {
		int iIndexLevel = m_pListBoxLevels->GetSelection();
		int iIndexGroup = m_pLevelsManager->GetLevel( iIndexLevel ).iNumberGroups - 1;
		FillGroupsList( iIndexLevel );
		SetItemGroupList( iIndexLevel, iIndexGroup );
	}
}

void CMyPanel::OnButtonGroupDelete( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pListBoxGroups->GetCount() <= 0 ) {
		wxMessageBox( _T("List of groups is empty."), _T("Delete Group"), wxOK | wxICON_INFORMATION );
		return;
	}
	if( wxMessageBox( _T("Do you want to delete selected group?"), _T("Delete group"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( m_pLevelsManager->DeleteGroup( m_pListBoxLevels->GetSelection(), m_pListBoxGroups->GetSelection() ) ) {
		int iIndexLevel = m_pListBoxLevels->GetSelection();
		FillGroupsList( iIndexLevel );
		SetItemGroupList( iIndexLevel, 0 );

		//ustaw ilosc emiterow na zero jezeli jest 0 to przy fill nie sutawia bo nie dojdzie
		if( m_pLevelsManager->GetLevel( iIndexLevel ).iNumberGroups == 0 )
			m_pTextCtrlGroupsNumber->SetValue( _T("0") );
	}
}

void CMyPanel::OnButtonGroupSaveMod( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pListBoxGroups->GetCount() <= 0 ) {
		wxMessageBox( _T("List of groups is empty."), _T("Save modify group"), wxOK | wxICON_INFORMATION );
		return;
	}
	if( wxMessageBox( _T("Do you want to save modify of selected group?"), _T("Save modify group"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( !CheckCorrectnessGroupControl() )
		return;

	//wypelnij strukture danymi
	SShipsGroups sNewGroup;
	FillStructGroup( sNewGroup );

	m_pLevelsManager->SaveModifyGroup( 
		m_pListBoxLevels->GetSelection(),
		m_pListBoxGroups->GetSelection(),
		sNewGroup );
}

bool CMyPanel::CheckCorrectnessLevelControl()
{
	if( !CheckWhetherStringIsInteger( m_pTextCtrlMaxObjOnScreen->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsInteger( m_pTextCtrlFrequencyOfShoot->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsInteger( m_pTextCtrlChanceOnShot->GetValue() ) )
		return false;

	if(_wtoi( m_pTextCtrlMaxObjOnScreen->GetValue() ) <= 0 ) {
		wxMessageBox( _T("Value for \"Max objects on screen\" must be larger from 0"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}

	if(_wtoi( m_pTextCtrlFrequencyOfShoot->GetValue() ) < 1 || _wtoi( m_pTextCtrlFrequencyOfShoot->GetValue() ) > 6 ) {
		wxMessageBox( _T("Value for \"Frequency of shoot\" must be between 1 and 6"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}
	
	return true;
}

void CMyPanel::FillStructLevel( SLevel &sLevel )
{
	wxString str;
	sLevel.iLevel = m_pListBoxLevels->GetSelection() + 1;
	//sLevel.bWithBoss = m_pCheckBoxWithBoss->GetValue();
	sLevel.iIndexBoss = _wtoi( m_pTextCtrlIndexOfBoss->GetValue() );
	sLevel.iMaxShipsOnScreen = _wtoi( m_pTextCtrlMaxObjOnScreen->GetValue() );
	sLevel.iFrequencyShoot = _wtoi( m_pTextCtrlFrequencyOfShoot->GetValue() );
	sLevel.iPercentChanceToShoot = _wtoi( m_pTextCtrlChanceOnShot->GetValue() );
}

void CMyPanel::OnButtonLevelAdd( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( wxMessageBox( _T("From current data in controls will be created new level. Do you want to continue?"), _T("Add new level"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( !CheckCorrectnessLevelControl() )
		return;

	//wypelnij strukture danymi
	SLevel sLevel;
	FillStructLevel( sLevel );

	sLevel.iNumberGroups = 0;
	sLevel.psShipsGroups = NULL;

	if( m_pLevelsManager->AddNewLevel( sLevel ) ) {
		FillLoadedLevelsList();
		SetItemLoadedLevelList( m_pListBoxLevels->GetCount() - 1 );
	}
}

void CMyPanel::OnButtonLevelDelete( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pListBoxLevels->GetCount() <= 0 ) {
		wxMessageBox( _T("List of levels is empty."), _T("Delete level"), wxOK | wxICON_INFORMATION );
		return;
	}
	if( wxMessageBox( _T("Do you want to delete selected level?"), _T("Delete level"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( m_pLevelsManager->DeleteLevel( m_pListBoxLevels->GetSelection() ) ) {
		FillLoadedLevelsList();
		SetItemLoadedLevelList( 0 );
	}
}

void CMyPanel::OnButtonLevelSaveMod( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pListBoxLevels->GetCount() <= 0 ) {
		wxMessageBox( _T("List of levels is empty."), _T("Save modify level"), wxOK | wxICON_INFORMATION );
		return;
	}
	if( wxMessageBox( _T("Do you want to save modify of selected level?"), _T("Save modify level"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( !CheckCorrectnessLevelControl() )
		return;

	SLevel sLevel;
	FillStructLevel( sLevel );

	int iIndexLevel = m_pListBoxLevels->GetSelection();
	if( m_pLevelsManager->SaveModifyLevel( iIndexLevel, sLevel ) ) {
		FillLoadedLevelsList();
		SetItemLoadedLevelList( iIndexLevel );
	}
}

bool CMyPanel::CheckWhetherStringIsFloat( wxString strSrc )
{
	for( size_t i = 0; i < strSrc.Length(); ++i ) {
		switch( strSrc[ i ] ) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '.':
			case '-':
				break;
			default:
				wxMessageBox( strSrc + _T(" - it is not float number!"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
				return false;
		}
	}
	return true;
}

bool CMyPanel::CheckWhetherStringIsInteger( wxString strSrc )
{
	for( size_t i = 0; i < strSrc.Length(); ++i ) {
		switch( strSrc[ i ] ) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
				break;
			default:
				wxMessageBox( strSrc + _T(" - it is not integer!"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
				return false;
		}
	}
	return true;
}