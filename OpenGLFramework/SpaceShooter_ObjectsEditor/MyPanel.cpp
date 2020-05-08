#include "StdAfx.h"
#include "MyFrame.h"
#include "ShipsManager.h"
#include "MyPanel.h"

//#include <vld.h>

BEGIN_EVENT_TABLE( CMyPanel, wxPanel )
	EVT_LISTBOX( ID_EVENT_LISTBOX_OBJECTS,		CMyPanel::OnListBoxObjects )
	EVT_LISTBOX( ID_EVENT_LISTBOX_EMITTERS,		CMyPanel::OnListBoxEmitters )
	EVT_CHECKBOX( ID_EVENT_CHECKBOX_PLAYER,		CMyPanel::OnCheckBoxPlayer )
	EVT_CHECKBOX( ID_EVENT_CHECKBOX_BUMP_TEX,	CMyPanel::OnCheckBoxBumpTex )
	EVT_RADIOBOX( ID_EVENT_RADIOBOX_TEX_TYPE,	CMyPanel::OnRadioBoxTypeOfTexture )
	EVT_BUTTON( ID_EVENT_BUTTON_COLOUR_TEXTURE,	CMyPanel::OnButtonColourOfTexture )
	EVT_BUTTON( ID_EVENT_BUTTON_MOVEUP,			CMyPanel::OnButtonMoveUp )
	EVT_BUTTON( ID_EVENT_BUTTON_MOVEDOWN,		CMyPanel::OnButtonMoveDown )
	EVT_BUTTON( ID_EVENT_BUTTON_EMITTER_ADD,		CMyPanel::OnButtonEmitterAdd )
	EVT_BUTTON( ID_EVENT_BUTTON_EMITTER_DELETE,	CMyPanel::OnButtonEmitterDelete )
	EVT_BUTTON( ID_EVENT_BUTTON_EMITTER_SAVEMOD,	CMyPanel::OnButtonEmitterSaveMod )
	EVT_BUTTON( ID_EVENT_BUTTON_OBJ_ADD,			CMyPanel::OnButtonObjectAdd )
	EVT_BUTTON( ID_EVENT_BUTTON_OBJ_DELETE,		CMyPanel::OnButtonObjectDelete )
	EVT_BUTTON( ID_EVENT_BUTTON_OBJ_SAVEMOD,		CMyPanel::OnButtonObjectSaveMod )
	EVT_TEXT( ID_EVENT_TEXT_NAME_OF_TEX,			CMyPanel::OnTextNameOfTexture )
	EVT_TEXT( ID_EVENT_TEXT_NAME_OF_LIFETEX,		CMyPanel::OnTextNameOfLifeTexture )
END_EVENT_TABLE()

CMyPanel::CMyPanel( wxFrame *frame, const wxPoint& pos, const wxSize& size, long style )
       : wxPanel( frame, wxID_ANY, pos, size, style )
{
	m_bReady = false;

	m_pMyFrame = ( CMyFrame* )frame;

	( void )new wxStaticBox( this, wxID_ANY, _T("Loaded objects"), wxPoint( 5, 5 ), wxSize( 120, 500 ) );
	m_pListBoxObjects = new wxListBox( this, ID_EVENT_LISTBOX_OBJECTS, wxPoint( 15, 25 ), wxSize( 100, 400 ), 0, 0 );
	m_pButtonMoveUp = new wxButton( this, ID_EVENT_BUTTON_MOVEUP, _T("Move up"), wxPoint( 15, 435 ), wxSize( 100, 24 ) );
	m_pButtonMoveDown = new wxButton( this, ID_EVENT_BUTTON_MOVEDOWN, _T("Move down"), wxPoint( 15, 469 ), wxSize( 100, 24 ) );

	wxColour colourReadOnly( 192, 192, 192 );
	int iStartPosY = 5; //pozycja Y pierwszych kontrolek
	int iOffsetPosY = 25;
	int iLabelLength = 200; //dlugosc napisow przed edit boxami
	int iLabelPosX = 5 + 130; //pozycja napisow przed edit boxami
	int iLabel2PosX = iLabelPosX + iLabelLength + 105; //310; //pozycja napisow po edit boxach, np dla ".3DObj" itp.
	int iEditPosX = iLabelPosX + iLabelLength + 5;//210; //pozycja X edit boxow

	int iPosY = iStartPosY;

	m_pLabelIndex = new wxStaticText( this, wxID_ANY, _T("&Index:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlIndex = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX, iPosY ), wxDefaultSize, wxTE_READONLY );
	m_pTextCtrlIndex->SetBackgroundColour( colourReadOnly );
	iPosY += iOffsetPosY;
	m_pLabelNameOfShip = new wxStaticText( this, wxID_ANY, _T("Name of &object:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlNameOfShip = new wxTextCtrl( this, wxID_ANY, _T("Text"), wxPoint( iEditPosX, iPosY ) );
	iPosY += iOffsetPosY;
	m_pLabelNameOfFile1 = new wxStaticText( this, wxID_ANY, _T("Name of &file: objects/"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlNameOfFile = new wxTextCtrl( this, wxID_ANY, _T("Text"), wxPoint( iEditPosX, iPosY ) );
	m_pLabelNameOfFile2 = new wxStaticText( this, wxID_ANY, _T(".3DObj"), wxPoint( iLabel2PosX, iPosY ) );
	iPosY += iOffsetPosY;
	m_pCheckBoxPlayer = new wxCheckBox( this, ID_EVENT_CHECKBOX_PLAYER, _T("&Player's ship"), wxPoint( iEditPosX, iPosY ) );
	iPosY += iOffsetPosY;
	wxString strTypesOfTexture[] = {
			_T("Normal"),
			_T("Multi-mask with colour"),
			_T("None")
		};
	m_pRadioTypeOfTexture = new wxRadioBox( this, ID_EVENT_RADIOBOX_TEX_TYPE, _T("Type of texture"), wxPoint( iEditPosX, iPosY ), wxDefaultSize, WXSIZEOF( strTypesOfTexture ), strTypesOfTexture, 1 );

	// jezeli typ tekstury = "normal" || "Multi-mask with COLORREF"
	iPosY += ( iOffsetPosY * 4 );
	m_pLabelNameOfTexture1 = new wxStaticText( this, wxID_ANY, _T("File name of &texture: textures/"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlNameOfTexture = new wxTextCtrl( this, ID_EVENT_TEXT_NAME_OF_TEX, _T("Text"), wxPoint( iEditPosX, iPosY ) );
	m_pLabelNameOfTexture2 = new wxStaticText( this, wxID_ANY, _T(".jeh"), wxPoint( iLabel2PosX, iPosY ) );

	// jezeli typ tekstury = "normal"
	iPosY += iOffsetPosY;
	m_pCheckBoxIsBumpTexture = new wxCheckBox( this, ID_EVENT_CHECKBOX_BUMP_TEX, _T("&Bump texture"), wxPoint( iEditPosX, iPosY ) );
	iPosY += iOffsetPosY;
	m_pLabelNameOfBumpTexture1 = new wxStaticText( this, wxID_ANY, _T("File name of bump texture: textures/"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlNameOfBumpTexture = new wxTextCtrl( this, wxID_ANY, _T("Text"), wxPoint( iEditPosX, iPosY ), wxDefaultSize, wxTE_READONLY );
	m_pTextCtrlNameOfBumpTexture->SetBackgroundColour( colourReadOnly );
	m_pLabelNameOfBumpTexture2 = new wxStaticText( this, wxID_ANY, _T(".jeh"), wxPoint( iLabel2PosX, iPosY ) );

	//jezeli typ textury = "Multi-mask with COLORREF"
	//ta sama pozycja co m_pCheckBoxIsBumpTexture
	iPosY -= iOffsetPosY;
	m_pButtonColourOfMask = new wxButton( this, ID_EVENT_BUTTON_COLOUR_TEXTURE, _T("Colour of texture:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength-20, 20 ) );
	m_pLabelColourOfMaskR = new wxStaticText( this, wxID_ANY, _T("R:"), wxPoint( iLabelPosX + iLabelLength - 20, iPosY ), wxSize( 20, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlColourOfTextureR = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX, iPosY ), wxSize( 30, 20 ) );
	m_pTextCtrlColourOfTextureR->SetToolTip( _T("Expect integer between 0 and 255") );
	m_pTextCtrlColourOfTextureR->SetMaxLength( 3 );
	m_pLabelColourOfMaskG = new wxStaticText( this, wxID_ANY, _T("G:"), wxPoint( iLabelPosX + iLabelLength + 30, iPosY ), wxSize( 20, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlColourOfTextureG = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX + 30 + 20, iPosY ), wxSize( 30, 20 ) );
	m_pTextCtrlColourOfTextureG->SetToolTip( _T("Expect integer between 0 and 255") );
	m_pTextCtrlColourOfTextureG->SetMaxLength( 3 );
	m_pLabelColourOfMaskB = new wxStaticText( this, wxID_ANY, _T("B:"), wxPoint( iLabelPosX + iLabelLength + 30 + 20 + 30, iPosY ), wxSize( 20, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlColourOfTextureB = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX + 30 + 20 + 30 + 20, iPosY ), wxSize( 30, 20 ) );
	m_pTextCtrlColourOfTextureB->SetToolTip( _T("Expect integer between 0 and 255") );
	m_pTextCtrlColourOfTextureB->SetMaxLength( 3 );
	iPosY += iOffsetPosY; //powrot do wlasciwej pozycji Y

	// jezli statek gracza
	iPosY += iOffsetPosY;
	m_pLabelNameOfLifeTexture1 = new wxStaticText( this, wxID_ANY, _T("File name of &life texture: textures/"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlNameOfLifeTexture = new wxTextCtrl( this, ID_EVENT_TEXT_NAME_OF_LIFETEX, _T("Text"), wxPoint( iEditPosX, iPosY ) );
	m_pLabelNameOfLifeTexture2 = new wxStaticText( this, wxID_ANY, _T(".jeh"), wxPoint( iLabel2PosX, iPosY ) );
	iPosY += iOffsetPosY;
	m_pLabelNameOfLifeTextureMask1 = new wxStaticText( this, wxID_ANY, _T("File name of life texture mask: textures/"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlNameOfLifeTextureMask = new wxTextCtrl( this, wxID_ANY, _T("Text"), wxPoint( iEditPosX, iPosY ), wxDefaultSize, wxTE_READONLY );
	m_pTextCtrlNameOfLifeTextureMask->SetBackgroundColour( colourReadOnly );
	m_pLabelNameOfLifeTextureMask2 = new wxStaticText( this, wxID_ANY, _T(".jeh"), wxPoint( iLabel2PosX, iPosY ) );

	iPosY += iOffsetPosY;
	m_pLabelCollisionA = new wxStaticText( this, wxID_ANY, _T("&Collision A:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlCollisionA = new wxTextCtrl( this, wxID_ANY, _T("0.0"), wxPoint( iEditPosX, iPosY ) );
	m_pTextCtrlCollisionA->SetToolTip( _T("Expect float number") );

	iPosY += iOffsetPosY;
	m_pLabelCollisionB = new wxStaticText( this, wxID_ANY, _T("Colli&sion B:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlCollisionB = new wxTextCtrl( this, wxID_ANY, _T("0.0"), wxPoint( iEditPosX, iPosY ) );
	m_pTextCtrlCollisionB->SetToolTip( _T("Expect float number") );

	iPosY += iOffsetPosY;
	m_pCheckBoxCanFire = new wxCheckBox( this, wxID_ANY, _T("It be able to shoot"), wxPoint( iEditPosX, iPosY ) );

	iPosY += iOffsetPosY;
	m_pCheckBoxCanManoeuvre = new wxCheckBox( this, wxID_ANY, _T("It be able to manoeuvre"), wxPoint( iEditPosX, iPosY ) );

	iPosY += iOffsetPosY;
	m_pLabelStarsShield = new wxStaticText( this, wxID_ANY, _T("Number of stars of shield:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlStarsShield = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX, iPosY ) );
	m_pTextCtrlStarsShield->SetToolTip( _T("Expect integer between 0 and 3") );

	iPosY += iOffsetPosY;
	m_pLabelStarsArmour = new wxStaticText( this, wxID_ANY, _T("Number of stars of armour:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlStarsArmour = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX, iPosY ) );
	m_pTextCtrlStarsArmour->SetToolTip( _T("Expect integer between 0 and 3") );

	iPosY += iOffsetPosY;
	m_pLabelStarsArmament = new wxStaticText( this, wxID_ANY, _T("Number of stars of armament:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlStarsArmament = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX, iPosY ) );
	m_pTextCtrlStarsArmament->SetToolTip( _T("Expect integer between 0 and 3") );

	iPosY += iOffsetPosY;
	m_pLabelStarsManoeuvring = new wxStaticText( this, wxID_ANY, _T("Number of stars of manoeuvring:"), wxPoint( iLabelPosX, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlStarsManoeuvring = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX, iPosY ) );
	m_pTextCtrlStarsManoeuvring->SetToolTip( _T("Expect integer between 0 and 3") );

	iPosY = iStartPosY;
	int iLabelPosX2 = iLabelPosX + iLabelLength + 100 + 100; //100 - dlugosc exit boxa
	iLabelLength = 100;// nowa dlugosc label-a
	int iEditPosX2 = iLabelPosX2 + iLabelLength + 5;

	( void )new wxStaticBox( this, wxID_ANY, _T("Emitters of particles"), wxPoint( iLabelPosX2 - 10, iPosY ), wxSize( 315, 350 ) );

	iPosY += iOffsetPosY;
	m_pLabelEmitterNumber = new wxStaticText( this, wxID_ANY, _T("Number of emitters:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlEmitterNumber = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX2, iPosY ), wxDefaultSize, wxTE_READONLY );
	m_pTextCtrlEmitterNumber->SetBackgroundColour( colourReadOnly );

	iPosY += iOffsetPosY;
	m_pListBoxEmitters = new wxListBox( this, ID_EVENT_LISTBOX_EMITTERS, wxPoint( iEditPosX2, iPosY ), wxSize( 100, 60 ), 0, 0 );

	iPosY += ( iOffsetPosY * 3 );
	m_pLabelEmitterIndexOfEmitter = new wxStaticText( this, wxID_ANY, _T("Index of emitter:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlEmitterIndexOfEmitter = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX2, iPosY ), wxDefaultSize, wxTE_READONLY );
	m_pTextCtrlEmitterIndexOfEmitter->SetBackgroundColour( colourReadOnly );
	m_pLabelEmitterIndexOfEmitter->Disable();
	m_pTextCtrlEmitterIndexOfEmitter->Disable();

	iPosY += iOffsetPosY;
	m_pLabelEmitterPosX = new wxStaticText( this, wxID_ANY, _T("Position:  X:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlEmitterPosX = new wxTextCtrl( this, wxID_ANY, _T("0.0"), wxPoint( iEditPosX2, iPosY ), wxSize( 50, 20 ) );
	m_pTextCtrlEmitterPosX->SetToolTip( _T("Expect float number") );
	m_pLabelEmitterPosY = new wxStaticText( this, wxID_ANY, _T("Y:"), wxPoint( iLabelPosX2 + iLabelLength + 50, iPosY ), wxSize( 20, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlEmitterPosY = new wxTextCtrl( this, wxID_ANY, _T("0.0"), wxPoint( iEditPosX2 + 50 + 20, iPosY ), wxSize( 50, 20 ) );
	m_pTextCtrlEmitterPosY->SetToolTip( _T("Expect float number") );
	m_pLabelEmitterPosZ = new wxStaticText( this, wxID_ANY, _T("Z:"), wxPoint( iLabelPosX2 + iLabelLength + 50 + 20 + 50, iPosY ), wxSize( 20, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlEmitterPosZ = new wxTextCtrl( this, wxID_ANY, _T("0.0"), wxPoint( iEditPosX2 + 50 + 20 + 50 + 20, iPosY ), wxSize( 50, 20 ) );
	m_pTextCtrlEmitterPosZ->SetToolTip( _T("Expect float number") );

	iPosY += iOffsetPosY;
	m_pLabelEmitterIndexOfTexture = new wxStaticText( this, wxID_ANY, _T("Index of texture:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlEmitterIndexOfTexture = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX2, iPosY ) );

	iPosY += iOffsetPosY;
	m_pLabelEmitterColourR = new wxStaticText( this, wxID_ANY, _T("Colour:  R:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlEmitterColourR = new wxTextCtrl( this, wxID_ANY, _T("0.0"), wxPoint( iEditPosX2, iPosY ), wxSize( 50, 20 ) );
	m_pTextCtrlEmitterColourR->SetToolTip( _T("Expect float number between 0.0f and 1.0f") );
	m_pLabelEmitterColourG = new wxStaticText( this, wxID_ANY, _T("G:"), wxPoint( iLabelPosX2 + iLabelLength + 50, iPosY ), wxSize( 20, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlEmitterColourG = new wxTextCtrl( this, wxID_ANY, _T("0.0"), wxPoint( iEditPosX2 + 50 + 20, iPosY ), wxSize( 50, 20 ) );
	m_pTextCtrlEmitterColourG->SetToolTip( _T("Expect float number between 0.0f and 1.0f") );
	m_pLabelEmitterColourB = new wxStaticText( this, wxID_ANY, _T("B:"), wxPoint( iLabelPosX2 + iLabelLength + 50 + 20 + 50, iPosY ), wxSize( 20, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlEmitterColourB = new wxTextCtrl( this, wxID_ANY, _T("0.0"), wxPoint( iEditPosX2 + 50 + 20 + 50 + 20, iPosY ), wxSize( 50, 20 ) );
	m_pTextCtrlEmitterColourB->SetToolTip( _T("Expect float number between 0.0f and 1.0f") );

	iPosY += iOffsetPosY;
	m_pLabelEmitterNumberOfParticles = new wxStaticText( this, wxID_ANY, _T("Number of particles:"), wxPoint( iLabelPosX2, iPosY ), wxSize( iLabelLength, 20 ), wxALIGN_RIGHT );
	m_pTextCtrlEmitterNumberOfParticles = new wxTextCtrl( this, wxID_ANY, _T("0"), wxPoint( iEditPosX2, iPosY ) );

	iPosY += iOffsetPosY + 5;
	m_pButtonEmitterAdd = new wxButton( this, ID_EVENT_BUTTON_EMITTER_ADD, _T("Add new"), wxPoint( iEditPosX2, iPosY ), wxSize( 100, 24 ) );

	iPosY += iOffsetPosY + 5;
	m_pButtonEmitterDelete = new wxButton( this, ID_EVENT_BUTTON_EMITTER_DELETE, _T("Delete selected"), wxPoint( iEditPosX2, iPosY ), wxSize( 100, 24 ) );

	iPosY += iOffsetPosY + 5;
	m_pButtonEmitterSaveModify = new wxButton( this, ID_EVENT_BUTTON_EMITTER_SAVEMOD, _T("Save modify"), wxPoint( iEditPosX2, iPosY ), wxSize( 100, 24 ) );

	///////////////////////////////////
	iPosY += ( iOffsetPosY * 4 );
	m_pButtonAddObj = new wxButton( this, ID_EVENT_BUTTON_OBJ_ADD, _T("Add new object"), wxPoint( iEditPosX2, iPosY ), wxSize( 150, 24 ) );

	iPosY += iOffsetPosY + 5;
	m_pButtonDeleteObj = new wxButton( this, ID_EVENT_BUTTON_OBJ_DELETE, _T("Delete selected object"), wxPoint( iEditPosX2, iPosY ), wxSize( 150, 24 ) );

	iPosY += iOffsetPosY + 5;
	m_pButtonSaveModifyObj = new wxButton( this, ID_EVENT_BUTTON_OBJ_SAVEMOD, _T("Save modify for object"), wxPoint( iEditPosX2, iPosY ), wxSize( 150, 24 ) );

	m_pShipManager = new CShipsManager();
	//m_pShipManager->ConvertStruct();
	m_strFileName = FILE_NAME_SHIPS_EDIT2;
	
	m_bReady = true;
}

CMyPanel::~CMyPanel()
{
	delete m_pShipManager;
}

void CMyPanel::Initialize()
{
	if( !FillLoadedObjectsList() ) {
		m_pMyFrame->OpenFile( true );
	}
	SetItemLoadedObjectsList( 0 );
}

void CMyPanel::LoadUniqueShips( wxString strFileName )
{
	m_strFileName = strFileName;
	FillLoadedObjectsList();
	SetItemLoadedObjectsList( 0 );
}

bool CMyPanel::FillLoadedObjectsList()
{
	bool bLoadOK = m_pShipManager->LoadUniqueShips( m_strFileName.c_str() );
	m_pListBoxObjects->Clear();
	for( int i = 0; i < m_pShipManager->GetUniqueShipSize(); ++i ) {
		wxString str;
		str.Printf( _T("%2d %s"), i, m_pShipManager->GetUniqueShip( i ).cShipName );
		m_pListBoxObjects->Append( str );
	}
	return bLoadOK;
}

void CMyPanel::FillEmittersList( int iIndexObj /*= -1*/ )
{
	ClearAllEmitterControls();
	if( iIndexObj == -1 )
		iIndexObj = m_pListBoxObjects->GetSelection();
	if( iIndexObj == -1 )
		return;
	wxString str;
	for( int i = 0; i < m_pShipManager->GetUniqueShip( iIndexObj ).iEmitterNumber; ++i ) {
		str.Printf( _T("%2d emitter"), i );
		m_pListBoxEmitters->Append( str );
	}
}

void CMyPanel::SetItemLoadedObjectsList( int iIndex )
{
	if( iIndex < 0 || iIndex >= m_pShipManager->GetUniqueShipSize() )
		return;

	m_pListBoxObjects->SetSelection( iIndex );
	FillAllControls( iIndex );
}

void CMyPanel::SetItemEmitterList( int iIndexObj, int iIndexEmitter )
{
	if( iIndexObj < 0 || iIndexObj >= m_pShipManager->GetUniqueShipSize() )
		return;

	if( iIndexEmitter < 0 || iIndexEmitter >= m_pShipManager->GetUniqueShip( iIndexObj ).iEmitterNumber )
		return;

	m_pListBoxEmitters->SetSelection( iIndexEmitter );
	FillAllControlsForEmitter( iIndexObj, iIndexEmitter );
}

void CMyPanel::FillAllControls( int iIndex )
{
	wxString str;

	str.Printf( _T("%d"), iIndex );
	m_pTextCtrlIndex->SetValue( str );

	m_pTextCtrlNameOfShip->SetValue( m_pShipManager->GetUniqueShip( iIndex ).cShipName );

	str = CutString( _T("objects/"), _T(".3dobj"), m_pShipManager->GetUniqueShip( iIndex ).cObjectName );
	m_pTextCtrlNameOfFile->SetValue( str );

	if( m_pShipManager->GetUniqueShip( iIndex ).bPlayer )
		m_pCheckBoxPlayer->SetValue( true );
	else
		m_pCheckBoxPlayer->SetValue( false );

	switch( m_pShipManager->GetUniqueShip( iIndex ).byTexType ) {
		case 0:
			m_pRadioTypeOfTexture->SetSelection( 0 );
			break;
		case 1:
			m_pRadioTypeOfTexture->SetSelection( 1 );
			break;
		default:
			m_pRadioTypeOfTexture->SetSelection( 2 );
			break;
	}

	str.Printf( _T("%d"), GetRValue( m_pShipManager->GetUniqueShip( iIndex ).uiCOLORREF ) );
	m_pTextCtrlColourOfTextureR->SetValue( str );

	str.Printf( _T("%d"), GetGValue( m_pShipManager->GetUniqueShip( iIndex ).uiCOLORREF ) );
	m_pTextCtrlColourOfTextureG->SetValue( str );

	str.Printf( _T("%d"), GetBValue( m_pShipManager->GetUniqueShip( iIndex ).uiCOLORREF ) );
	m_pTextCtrlColourOfTextureB->SetValue( str );

	if( m_pShipManager->GetUniqueShip( iIndex ).cTexBumpName[ 0 ] == 0 )
		m_pCheckBoxIsBumpTexture->SetValue( false );
	else m_pCheckBoxIsBumpTexture->SetValue( true );

	SetVisibleControlsForTextureMode();

	str = CutString( _T("textures/"), _T(".jeh"), m_pShipManager->GetUniqueShip( iIndex ).cTexName );
	m_pTextCtrlNameOfTexture->SetValue( str );

	str = CutString( _T("textures/"), _T(".jeh"), m_pShipManager->GetUniqueShip( iIndex ).cTexBumpName );
	m_pTextCtrlNameOfBumpTexture->SetValue( str );

	str = CutString( _T("textures/"), _T(".jeh"), m_pShipManager->GetUniqueShip( iIndex ).cTexLife );
	m_pTextCtrlNameOfLifeTexture->SetValue( str );

	str = CutString( _T("textures/"), _T(".jeh"), m_pShipManager->GetUniqueShip( iIndex ).cTexLifeMask );
	m_pTextCtrlNameOfLifeTextureMask->SetValue( str );

	str.Printf( _T("%f"), m_pShipManager->GetUniqueShip( iIndex ).fCollisionA );
	m_pTextCtrlCollisionA->SetValue( str );

	str.Printf( _T("%f"), m_pShipManager->GetUniqueShip( iIndex ).fCollisionB );
	m_pTextCtrlCollisionB->SetValue( str );

	if( m_pShipManager->GetUniqueShip( iIndex ).bCanFire )
		m_pCheckBoxCanFire->SetValue( true );
	else
		m_pCheckBoxCanFire->SetValue( false );

	if( m_pShipManager->GetUniqueShip( iIndex ).bCanManoeuvring )
		m_pCheckBoxCanManoeuvre->SetValue( true );
	else
		m_pCheckBoxCanManoeuvre->SetValue( false );

	str.Printf( _T("%d"), m_pShipManager->GetUniqueShip( iIndex ).iStarsShield );
	m_pTextCtrlStarsShield->SetValue( str );

	str.Printf( _T("%d"), m_pShipManager->GetUniqueShip( iIndex ).iStarsArmour );
	m_pTextCtrlStarsArmour->SetValue( str );

	str.Printf( _T("%d"), m_pShipManager->GetUniqueShip( iIndex ).iStarsArmament );
	m_pTextCtrlStarsArmament->SetValue( str );

	str.Printf( _T("%d"), m_pShipManager->GetUniqueShip( iIndex ).iStarsManoeuvring );
	m_pTextCtrlStarsManoeuvring->SetValue( str );

	//////////////////////////////////////////////////////
	//////////// ustaw emitery ///////////////////////////
	str.Printf( _T("%d"), m_pShipManager->GetUniqueShip( iIndex ).iEmitterNumber );
	m_pTextCtrlEmitterNumber->SetValue( str );

	FillEmittersList( iIndex );
	SetItemEmitterList( iIndex, 0 );
}

void CMyPanel::FillAllControlsForEmitter( int iIndexObj, int iIndexEmitter )
{
	wxString str;

	str.Printf( _T("%d"), m_pShipManager->GetUniqueShip( iIndexObj ).iEmitterNumber );
	m_pTextCtrlEmitterNumber->SetValue( str );

	str.Printf( _T("%d"), m_pShipManager->GetUniqueShip( iIndexObj ).psEmitterData[ iIndexEmitter ].iIndex );
	m_pTextCtrlEmitterIndexOfEmitter->SetValue( str );

	str.Printf( _T("%f"), m_pShipManager->GetUniqueShip( iIndexObj ).psEmitterData[ iIndexEmitter ].fEmitterPosX );
	m_pTextCtrlEmitterPosX->SetValue( str );

	str.Printf( _T("%f"), m_pShipManager->GetUniqueShip( iIndexObj ).psEmitterData[ iIndexEmitter ].fEmitterPosY );
	m_pTextCtrlEmitterPosY->SetValue( str );

	str.Printf( _T("%f"), m_pShipManager->GetUniqueShip( iIndexObj ).psEmitterData[ iIndexEmitter ].fEmitterPosZ );
	m_pTextCtrlEmitterPosZ->SetValue( str );

	str.Printf( _T("%d"), m_pShipManager->GetUniqueShip( iIndexObj ).psEmitterData[ iIndexEmitter ].iTexEmitter );
	m_pTextCtrlEmitterIndexOfTexture->SetValue( str );

	str.Printf( _T("%f"), m_pShipManager->GetUniqueShip( iIndexObj ).psEmitterData[ iIndexEmitter ].fR );
	m_pTextCtrlEmitterColourR->SetValue( str );

	str.Printf( _T("%f"), m_pShipManager->GetUniqueShip( iIndexObj ).psEmitterData[ iIndexEmitter ].fG );
	m_pTextCtrlEmitterColourG->SetValue( str );

	str.Printf( _T("%f"), m_pShipManager->GetUniqueShip( iIndexObj ).psEmitterData[ iIndexEmitter ].fB );
	m_pTextCtrlEmitterColourB->SetValue( str );
	
	str.Printf( _T("%d"), m_pShipManager->GetUniqueShip( iIndexObj ).psEmitterData[ iIndexEmitter ].iParticlesNumber );
	m_pTextCtrlEmitterNumberOfParticles->SetValue( str );
}

void CMyPanel::ClearAllEmitterControls()
{
	m_pListBoxEmitters->Clear();
	m_pTextCtrlEmitterIndexOfEmitter->SetValue( _T("0") );
	m_pTextCtrlEmitterPosX->SetValue( _T("0.0") );
	m_pTextCtrlEmitterPosY->SetValue( _T("0.0") );
	m_pTextCtrlEmitterPosZ->SetValue( _T("0.0") );
	m_pTextCtrlEmitterIndexOfTexture->SetValue( _T("0") );
	m_pTextCtrlEmitterColourR->SetValue( _T("0") );
	m_pTextCtrlEmitterColourG->SetValue( _T("0") );
	m_pTextCtrlEmitterColourB->SetValue( _T("0") );
	m_pTextCtrlEmitterNumberOfParticles->SetValue( _T("0") );
}

wxString CMyPanel::CutString( wxString strFirst, wxString strLast, wxString strSrc )
{
	strSrc.LowerCase();
	strFirst.LowerCase();
	strLast.LowerCase();
	int iPos = strSrc.Find( strFirst );
	if( iPos > -1 ) 
		strSrc = strSrc.Mid( iPos + strFirst.Length() );
	iPos = strSrc.Find( strLast );
	if( iPos > -1 ) 
		strSrc = strSrc.Left( iPos );

	return strSrc;
}

void CMyPanel::SetVisibleControlsForTextureMode()
{
	wxCommandEvent eEvent;
	switch( m_pRadioTypeOfTexture->GetSelection() ) {
		case 0:
			m_pLabelNameOfTexture1->Show( true );
			m_pTextCtrlNameOfTexture->Show( true );
			m_pLabelNameOfTexture2->Show( true );

			m_pCheckBoxIsBumpTexture->Show( true );

			eEvent.SetId( ID_EVENT_CHECKBOX_BUMP_TEX );
			OnCheckBoxBumpTex( eEvent );
			
			m_pButtonColourOfMask->Show( false );
			m_pLabelColourOfMaskR->Show( false );
			m_pTextCtrlColourOfTextureR->Show( false );
			m_pLabelColourOfMaskG->Show( false );
			m_pTextCtrlColourOfTextureG->Show( false );
			m_pLabelColourOfMaskB->Show( false );
			m_pTextCtrlColourOfTextureB->Show( false );
			break;
		case 1:
			m_pLabelNameOfTexture1->Show( true );
			m_pTextCtrlNameOfTexture->Show( true );
			m_pLabelNameOfTexture2->Show( true );

			m_pCheckBoxIsBumpTexture->Show( false );

			m_pLabelNameOfBumpTexture1->Show( false );
			m_pTextCtrlNameOfBumpTexture->Show( false );
			m_pLabelNameOfBumpTexture2->Show( false );
			
			m_pButtonColourOfMask->Show( true );
			m_pLabelColourOfMaskR->Show( true );
			m_pTextCtrlColourOfTextureR->Show( true );
			m_pLabelColourOfMaskG->Show( true );
			m_pTextCtrlColourOfTextureG->Show( true );
			m_pLabelColourOfMaskB->Show( true );
			m_pTextCtrlColourOfTextureB->Show( true );
			break;
		default:
			m_pLabelNameOfTexture1->Show( false );
			m_pTextCtrlNameOfTexture->Show( false );
			m_pLabelNameOfTexture2->Show( false );

			m_pCheckBoxIsBumpTexture->Show( false );

			m_pLabelNameOfBumpTexture1->Show( false );
			m_pTextCtrlNameOfBumpTexture->Show( false );
			m_pLabelNameOfBumpTexture2->Show( false );
			
			m_pButtonColourOfMask->Show( false );
			m_pLabelColourOfMaskR->Show( false );
			m_pTextCtrlColourOfTextureR->Show( false );
			m_pLabelColourOfMaskG->Show( false );
			m_pTextCtrlColourOfTextureG->Show( false );
			m_pLabelColourOfMaskB->Show( false );
			m_pTextCtrlColourOfTextureB->Show( false );
			break;
	}
	eEvent.SetId( ID_EVENT_CHECKBOX_PLAYER );
	OnCheckBoxPlayer( eEvent );
}

void CMyPanel::OnListBoxObjects( wxCommandEvent &eEvent )
{
	SetItemLoadedObjectsList( eEvent.GetInt() );
}

void CMyPanel::OnListBoxEmitters( wxCommandEvent &eEvent )
{
	SetItemEmitterList( m_pListBoxObjects->GetSelection(), eEvent.GetInt() );
}

void CMyPanel::OnCheckBoxPlayer( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pCheckBoxPlayer->GetValue() ) {
		m_pLabelNameOfLifeTexture1->Show( true );
		m_pTextCtrlNameOfLifeTexture->Show( true );
		m_pLabelNameOfLifeTexture2->Show( true );

		m_pLabelNameOfLifeTextureMask1->Show( true );
		m_pTextCtrlNameOfLifeTextureMask->Show( true );
		m_pLabelNameOfLifeTextureMask2->Show( true );
	}
	else {
		m_pLabelNameOfLifeTexture1->Show( false );
		m_pTextCtrlNameOfLifeTexture->Show( false );
		m_pLabelNameOfLifeTexture2->Show( false );

		m_pLabelNameOfLifeTextureMask1->Show( false );
		m_pTextCtrlNameOfLifeTextureMask->Show( false );
		m_pLabelNameOfLifeTextureMask2->Show( false );
	}
}

void CMyPanel::OnCheckBoxBumpTex( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pCheckBoxIsBumpTexture->GetValue() ) {
		m_pLabelNameOfBumpTexture1->Show( true );
		m_pTextCtrlNameOfBumpTexture->Show( true );
		m_pLabelNameOfBumpTexture2->Show( true );

		wxString str = m_pTextCtrlNameOfTexture->GetValue() + _T("bump");
		m_pTextCtrlNameOfBumpTexture->SetValue( str );
	}
	else {
		m_pLabelNameOfBumpTexture1->Show( false );
		m_pTextCtrlNameOfBumpTexture->Show( false );
		m_pLabelNameOfBumpTexture2->Show( false );
	}
}

void CMyPanel::OnRadioBoxTypeOfTexture( wxCommandEvent &WXUNUSED( eEvent ) )
{
	SetVisibleControlsForTextureMode();
}

void CMyPanel::OnButtonMoveUp( wxCommandEvent &WXUNUSED( eEvent ) )
{
	int iIndex = m_pListBoxObjects->GetSelection();
	if( m_pShipManager->MoveUpIndex( iIndex ) ) {
		FillLoadedObjectsList();
		SetItemLoadedObjectsList( --iIndex );
	}
}

bool CMyPanel::CheckCorrectnessEmitterControl()
{
	if( !CheckWhetherStringIsFloat( m_pTextCtrlEmitterPosX->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsFloat( m_pTextCtrlEmitterPosY->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsFloat( m_pTextCtrlEmitterPosZ->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsFloat( m_pTextCtrlEmitterColourR->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsFloat( m_pTextCtrlEmitterColourG->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsFloat( m_pTextCtrlEmitterColourB->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsInteger( m_pTextCtrlEmitterIndexOfTexture->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsInteger( m_pTextCtrlEmitterNumberOfParticles->GetValue() ) )
		return false;

	float fR = _wtof( m_pTextCtrlEmitterColourR->GetValue() );
	float fG = _wtof( m_pTextCtrlEmitterColourG->GetValue() );
	float fB = _wtof( m_pTextCtrlEmitterColourB->GetValue() );

	if( fR < 0.0f || fR > 1.0f ) {
		wxMessageBox( _T("Value R for colour of emitter must be float number between 0.0f and 1.0f"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}
	if( fG < 0.0f || fG > 1.0f ) {
		wxMessageBox( _T("Value G for colour of emitter must be float number between 0.0f and 1.0f"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}
	if( fB < 0.0f || fB > 1.0f ) {
		wxMessageBox( _T("Value B for colour of emitter must be float number between 0.0f and 1.0f"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}

	return true;
}

void CMyPanel::FillStructEmitter( SEmitterData &sEmitter )
{
	sEmitter.iIndex = 0;
	sEmitter.fEmitterPosX = _wtof( m_pTextCtrlEmitterPosX->GetValue() );
	sEmitter.fEmitterPosY = _wtof( m_pTextCtrlEmitterPosY->GetValue() );
	sEmitter.fEmitterPosZ = _wtof( m_pTextCtrlEmitterPosZ->GetValue() );
	sEmitter.fR = _wtof( m_pTextCtrlEmitterColourR->GetValue() );
	sEmitter.fG = _wtof( m_pTextCtrlEmitterColourG->GetValue() );
	sEmitter.fB = _wtof( m_pTextCtrlEmitterColourB->GetValue() );
	sEmitter.iTexEmitter = _wtoi( m_pTextCtrlEmitterIndexOfTexture->GetValue() );
	sEmitter.iParticlesNumber = _wtoi( m_pTextCtrlEmitterNumberOfParticles->GetValue() );
}

void CMyPanel::OnButtonMoveDown( wxCommandEvent &WXUNUSED( eEvent ) )
{
	int iIndex = m_pListBoxObjects->GetSelection();
	if( m_pShipManager->MoveDownIndex( iIndex ) ) {
		FillLoadedObjectsList();
		SetItemLoadedObjectsList( ++iIndex );
	}
}

void CMyPanel::OnButtonEmitterAdd( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( wxMessageBox( _T("From current data in controls will be created new emitter. Do you want to continue?"), _T("Add new emitter"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( !CheckCorrectnessEmitterControl() )
		return;

	//wypelnij strukture danymi
	SEmitterData sNewEmitter;
	FillStructEmitter( sNewEmitter );

	if( m_pShipManager->AddNewEmitter( m_pListBoxObjects->GetSelection(), sNewEmitter ) ) {
		int iIndexObj = m_pListBoxObjects->GetSelection();
		int iIndexEmitter = m_pShipManager->GetUniqueShip( iIndexObj ).iEmitterNumber - 1;
		FillEmittersList( iIndexObj );
		SetItemEmitterList( iIndexObj, iIndexEmitter );
	}
}

void CMyPanel::OnButtonEmitterDelete( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pListBoxEmitters->GetCount() <= 0 ) {
		wxMessageBox( _T("List of emitters is empty."), _T("Delete emitter"), wxOK | wxICON_INFORMATION );
		return;
	}
	if( wxMessageBox( _T("Do you want to delete selected emitter?"), _T("Delete emitter"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( m_pShipManager->DeleteEmitter( m_pListBoxObjects->GetSelection(), m_pListBoxEmitters->GetSelection() ) ) {
		int iIndexObj = m_pListBoxObjects->GetSelection();
		FillEmittersList( iIndexObj );
		SetItemEmitterList( iIndexObj, 0 );

		//ustaw ilosc emiterow na zero jezeli jest 0 to przy fill nie sutawia bo nie dojdzie
		if( m_pShipManager->GetUniqueShip( iIndexObj ).iEmitterNumber == 0 )
			m_pTextCtrlEmitterNumber->SetValue( _T("0") );
	}
}

void CMyPanel::OnButtonEmitterSaveMod( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pListBoxEmitters->GetCount() <= 0 ) {
		wxMessageBox( _T("List of emitters is empty."), _T("Save modify emitter"), wxOK | wxICON_INFORMATION );
		return;
	}
	if( wxMessageBox( _T("Do you want to save modify of selected emitter?"), _T("Save modify emitter"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( !CheckCorrectnessEmitterControl() )
		return;

	//wypelnij strukture danymi
	SEmitterData sNewEmitter;
	FillStructEmitter( sNewEmitter );

	m_pShipManager->SaveModifyEmitter( 
		m_pListBoxObjects->GetSelection(),
		m_pListBoxEmitters->GetSelection(),
		sNewEmitter );
}

bool CMyPanel::CheckCorrectnessObjectControl()
{
	//sprawdz czy dane sa puste
	if( m_pTextCtrlNameOfShip->GetValue().Length() <= 0 ) {
		wxMessageBox( _T("Name of object cannot be empty."), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}
	if( m_pTextCtrlNameOfFile->GetValue().Length() <= 0 ) {
		wxMessageBox( _T("Name of file cannot be empty."), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}
	switch( m_pRadioTypeOfTexture->GetSelection() ) {
		case 0:
			if( m_pTextCtrlNameOfTexture->GetValue().Length() <= 0 ) {
				wxMessageBox( _T("File name of texture cannot be empty."), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
				return false;
			}
			break;
		case 1:
			if( m_pTextCtrlNameOfTexture->GetValue().Length() <= 0 ) {
				wxMessageBox( _T("File name of texture cannot be empty."), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
				return false;
			}
			break;
		default:
			break;
	}

	if( !CheckWhetherStringIsFloat( m_pTextCtrlCollisionA->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsFloat( m_pTextCtrlCollisionB->GetValue() ) )
		return false;
	if( m_pRadioTypeOfTexture->GetSelection() == 1 ) {
		if( !CheckWhetherStringIsInteger( m_pTextCtrlColourOfTextureR->GetValue() ) )
			return false;
		if( !CheckWhetherStringIsInteger( m_pTextCtrlColourOfTextureG->GetValue() ) )
			return false;
		if( !CheckWhetherStringIsInteger( m_pTextCtrlColourOfTextureB->GetValue() ) )
			return false;
		int iR = _wtoi( m_pTextCtrlColourOfTextureR->GetValue() );
		int iG = _wtoi( m_pTextCtrlColourOfTextureG->GetValue() );
		int iB = _wtoi( m_pTextCtrlColourOfTextureB->GetValue() );

		if( iR < 0 || iR > 255 ) {
			wxMessageBox( _T("Value R for colour of texture must be integer between 0 and 255"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
			return false;
		}
		if( iG < 0 || iG > 255 ) {
			wxMessageBox( _T("Value G for colour of texture must be integer between 0 and 255"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
			return false;
		}
		if( iB < 0 || iB > 255 ) {
			wxMessageBox( _T("Value B for colour of texture must be integer between 0 and 255"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
			return false;
		}
	}
	if( !CheckWhetherStringIsInteger( m_pTextCtrlStarsShield->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsInteger( m_pTextCtrlStarsArmour->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsInteger( m_pTextCtrlStarsArmament->GetValue() ) )
		return false;
	if( !CheckWhetherStringIsInteger( m_pTextCtrlStarsManoeuvring->GetValue() ) )
		return false;

	int iStarsShield = _wtoi( m_pTextCtrlStarsShield->GetValue() );
	int iStarsArmour = _wtoi( m_pTextCtrlStarsArmour->GetValue() );
	int iStarsArmament = _wtoi( m_pTextCtrlStarsArmament->GetValue() );
	int iStarsManoeuvring = _wtoi( m_pTextCtrlStarsManoeuvring->GetValue() );

	if( iStarsShield < 0 || iStarsShield > 3 ) {
		wxMessageBox( _T("Value for star of shield must be integer between 0 and 3"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}
	if( iStarsArmour < 0 || iStarsArmour > 3 ) {
		wxMessageBox( _T("Value for star of armour must be integer between 0 and 3"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}
	if( iStarsArmament < 0 || iStarsArmament > 3 ) {
		wxMessageBox( _T("Value for star of armament must be integer between 0 and 3"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}
	if( iStarsManoeuvring < 0 || iStarsManoeuvring > 3 ) {
		wxMessageBox( _T("Value for star of manoeuvring must be integer between 0 and 3"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
		return false;
	}

	return true;
}

void CMyPanel::FillStructUniqueShip( SUniqueShip &sShip )
{
	sShip.i3DObjIndex = 0;
	sShip.iTexIndex = 0;
	sShip.iTexBumpIndex = 0;
	sShip.iTexLife = 0;
	sShip.byTexType = static_cast<unsigned char>( m_pRadioTypeOfTexture->GetSelection() );
	switch( sShip.byTexType ) {
		case 0:
			wcscpy_s( sShip.cTexName, SIZE_STRING, _T("textures/") );
			wcscat_s( sShip.cTexName, SIZE_STRING, m_pTextCtrlNameOfTexture->GetValue() );
			wcscat_s( sShip.cTexName, SIZE_STRING, _T(".jeh") );
			if( m_pCheckBoxIsBumpTexture->GetValue() ) {
				wcscpy_s( sShip.cTexBumpName, SIZE_STRING, _T("textures/") );
				//wcscat_s( cTemp, SIZE_STRING, m_pTextCtrlNameOfBumpTexture->GetValue() );
				wcscat_s( sShip.cTexBumpName, SIZE_STRING, m_pTextCtrlNameOfTexture->GetValue() );
				wcscat_s( sShip.cTexBumpName, SIZE_STRING, _T("bump.jeh") );
			}
			else
				sShip.cTexBumpName[ 0 ] = 0; //bez tekstury bump
			sShip.uiCOLORREF = 0;
			break;
		case 1:
		{
			wcscpy_s( sShip.cTexName, SIZE_STRING, _T("textures/") );
			wcscat_s( sShip.cTexName, SIZE_STRING, m_pTextCtrlNameOfTexture->GetValue() );
			wcscat_s( sShip.cTexName, SIZE_STRING, _T(".jeh") );
			sShip.cTexBumpName[ 0 ] = 0; //bez tekstury bump
			int iR = _wtoi( m_pTextCtrlColourOfTextureR->GetValue() );
			int iG = _wtoi( m_pTextCtrlColourOfTextureG->GetValue() );
			int iB = _wtoi( m_pTextCtrlColourOfTextureB->GetValue() );
			sShip.uiCOLORREF = RGB( BYTE( iR ), BYTE( iG ), BYTE( iB ) );
			break;
		}
		default:
			sShip.cTexName[ 0 ] = 0; //bez tekstury
			sShip.cTexBumpName[ 0 ] = 0; //bez tekstury bump
			sShip.uiCOLORREF = 0;
			break;
	}

	if( m_pCheckBoxPlayer->GetValue() ) {
		wcscpy_s( sShip.cTexLife, SIZE_STRING, _T("textures/") );
		wcscat_s( sShip.cTexLife, SIZE_STRING, m_pTextCtrlNameOfLifeTexture->GetValue() );
		wcscat_s( sShip.cTexLife, SIZE_STRING, _T(".jeh") );

		wcscpy_s( sShip.cTexLifeMask, SIZE_STRING, _T("textures/") );
		wcscat_s( sShip.cTexLifeMask, SIZE_STRING, m_pTextCtrlNameOfLifeTexture->GetValue() );
		wcscat_s( sShip.cTexLifeMask, SIZE_STRING, _T("mask.jeh") );
	}
	else {
		sShip.cTexLife[ 0 ] = 0;
		sShip.cTexLifeMask[ 0 ] = 0;
	}

	wcscpy_s( sShip.cObjectName, SIZE_STRING, _T("objects/") );
	wcscat_s( sShip.cObjectName, SIZE_STRING, m_pTextCtrlNameOfFile->GetValue() );
	wcscat_s( sShip.cObjectName, SIZE_STRING, _T(".3DObj") );

	sShip.bPlayer = static_cast<unsigned char>( m_pCheckBoxPlayer->GetValue() );
	wcscpy_s( sShip.cShipName, SIZE_STRING, m_pTextCtrlNameOfShip->GetValue() );
	sShip.bCanFire = static_cast<unsigned char>( m_pCheckBoxCanFire->GetValue() );
	sShip.bCanManoeuvring = static_cast<unsigned char>( m_pCheckBoxCanManoeuvre->GetValue() );
	sShip.iStarsShield = _wtoi( m_pTextCtrlStarsShield->GetValue() );
	sShip.iStarsArmour = _wtoi( m_pTextCtrlStarsArmour->GetValue() );
	sShip.iStarsArmament = _wtoi( m_pTextCtrlStarsArmament->GetValue() );
	sShip.iStarsManoeuvring = _wtoi( m_pTextCtrlStarsManoeuvring->GetValue() );
	sShip.fCollisionA = _wtof( m_pTextCtrlCollisionA->GetValue() );
	sShip.fCollisionB = _wtof( m_pTextCtrlCollisionB->GetValue() );
}

void CMyPanel::OnButtonObjectAdd( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( wxMessageBox( _T("From current data in controls will be created new object. Do you want to continue?"), _T("Add new object"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( !CheckCorrectnessObjectControl() )
		return;

	//wypelnij strukture danymi
	SUniqueShip sShip;
	FillStructUniqueShip( sShip );

	sShip.iEmitterNumber = 0;
	sShip.psEmitterData = NULL;

	if( m_pShipManager->AddNewObject( sShip ) ) {
		FillLoadedObjectsList();
		SetItemLoadedObjectsList( m_pListBoxObjects->GetCount() - 1 );
	}
}

void CMyPanel::OnButtonObjectDelete( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pListBoxObjects->GetCount() <= 0 ) {
		wxMessageBox( _T("List of objects is empty."), _T("Delete object"), wxOK | wxICON_INFORMATION );
		return;
	}
	if( wxMessageBox( _T("Do you want to delete selected object?"), _T("Delete object"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( m_pShipManager->DeleteObject( m_pListBoxObjects->GetSelection() ) ) {
		FillLoadedObjectsList();
		SetItemLoadedObjectsList( 0 );
	}
}

void CMyPanel::OnButtonObjectSaveMod( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( m_pListBoxObjects->GetCount() <= 0 ) {
		wxMessageBox( _T("List of objects is empty."), _T("Save modify object"), wxOK | wxICON_INFORMATION );
		return;
	}
	if( wxMessageBox( _T("Do you want to save modify of selected object?"), _T("Save modify object"), wxYES_NO|wxICON_QUESTION ) == wxNO )
		return;

	if( !CheckCorrectnessObjectControl() )
		return;

	SUniqueShip sShip;
	FillStructUniqueShip( sShip );

	int iIndexObj = m_pListBoxObjects->GetSelection();
	if( m_pShipManager->SaveModifyObject( iIndexObj, sShip ) ) {
		FillLoadedObjectsList();
		SetItemLoadedObjectsList( iIndexObj );
	}
}

void CMyPanel::OnTextNameOfTexture( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( !m_bReady )
		return;
	wxString str = m_pTextCtrlNameOfTexture->GetValue();
	m_pTextCtrlNameOfBumpTexture->SetValue( str + _T("bump") );	
}

void CMyPanel::OnTextNameOfLifeTexture( wxCommandEvent &WXUNUSED( eEvent ) )
{
	if( !m_bReady )
		return;
	wxString str = m_pTextCtrlNameOfLifeTexture->GetValue();
	m_pTextCtrlNameOfLifeTextureMask->SetValue( str + _T("mask") );	
}

void CMyPanel::OnButtonColourOfTexture( wxCommandEvent &WXUNUSED( eEvent ) )
{
	wxColourData colourData;
	wxColour colour( _wtoi( m_pTextCtrlColourOfTextureR->GetValue() ),
					 _wtoi( m_pTextCtrlColourOfTextureG->GetValue() ),
					 _wtoi( m_pTextCtrlColourOfTextureB->GetValue() ) );
	colourData.SetColour( colour );
	wxColourDialog pColourDialog( this, &colourData);
	if( pColourDialog.ShowModal() == wxID_OK )
	{
		colourData = pColourDialog.GetColourData();
		colour = colourData.GetColour();
		wxString str;
		str.Printf( _T("%d"), colour.Red() );
		m_pTextCtrlColourOfTextureR->SetValue( str );
		str.Printf( _T("%d"), colour.Green() );
		m_pTextCtrlColourOfTextureG->SetValue( str );
		str.Printf( _T("%d"), colour.Blue() );
		m_pTextCtrlColourOfTextureB->SetValue( str );
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
				break;
			default:
				wxMessageBox( strSrc + _T(" - it is not integer!"), _T("Incorrect data"), wxOK | wxICON_EXCLAMATION );
				return false;
		}
	}
	return true;
}