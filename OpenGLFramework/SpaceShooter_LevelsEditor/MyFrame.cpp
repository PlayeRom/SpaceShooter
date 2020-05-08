
#include "stdafx.h"
#include "MyPanel.h"
#include "MyFrame.h"

BEGIN_EVENT_TABLE( CMyFrame, wxFrame )
	EVT_MENU( MENU_FILE_OPENFILE,	CMyFrame::OnMenuFileOpenFile )
    EVT_MENU( MENU_FILE_EXIT,		CMyFrame::OnMenuFileExit )
    EVT_MENU( MENU_HELP_ABOUT,		CMyFrame::OnMenuHelpAbout )
END_EVENT_TABLE()

CMyFrame::CMyFrame( const wxString& title, const wxPoint& pos, const wxSize& size, long style )
       : wxFrame( NULL, wxID_ANY, title, pos, size, style )
{
	// set the frame icon
	//SetIcon( wxICON( sample ) );

	wxMenu *pMenuFile = new wxMenu;
	wxMenu *pMenuHelp = new wxMenu;
	pMenuFile->Append( MENU_FILE_OPENFILE, _T("&Open file...\tAlt-1"), _T("Open file") );
	pMenuFile->AppendSeparator();
	pMenuFile->Append( MENU_FILE_EXIT, _T("E&xit\tAlt-X"), _T("Quit this program") );
	pMenuHelp->Append( MENU_HELP_ABOUT, _T("&About...\tF1"), _T("Show about dialog") );

	// now append the freshly created menu to the menu bar...
	wxMenuBar *pMenuBar = new wxMenuBar();
	pMenuBar->Append( pMenuFile, _T("File") );
	pMenuBar->Append( pMenuHelp, _T("Help") );
	SetMenuBar( pMenuBar );

	// create a status bar just for fun ( by default with 1 pane only )
	CreateStatusBar( 1 );
	SetStatusText( FILE_NAME_LEVEL_EDIT2 );

	m_pFileDialog = new wxFileDialog( NULL, wxFileSelectorPromptStr, wxEmptyString, _T("levels.dat"),
		_T("DAT files (*.dat )|*.dat" ), wxOPEN, wxDefaultPosition );

	m_pMyPanel = new CMyPanel( this );
	m_pMyPanel->Initialize();
}

CMyFrame::~CMyFrame()
{
	delete m_pFileDialog;
}

void CMyFrame::OnMenuFileOpenFile( wxCommandEvent& WXUNUSED( eEvent ) )
{
	OpenFile();
}

void CMyFrame::OpenFile( bool bIfCancelThenExit /*= false*/ )
{
	if( m_pFileDialog->ShowModal() == wxID_CANCEL ) {
		if( bIfCancelThenExit )
			Close( true );
		return;
	}

	m_pMyPanel->LoadLevels( m_pFileDialog->GetPath() );
	SetStatusText( m_pFileDialog->GetPath() );
}

void CMyFrame::OnMenuFileExit( wxCommandEvent& WXUNUSED( eEvent ) )
{
	Close( true ); // true is to force the frame to close
}

void CMyFrame::OnMenuHelpAbout( wxCommandEvent& WXUNUSED( eEvent ) )
{
	wxString msgStr;
	msgStr.Printf( _T("LEVELS EDITOR for Space Shooter project.\nVersion: %s"), _T(VERSION ) );

	wxMessageBox( msgStr, _T("About"), wxOK | wxICON_INFORMATION, this );
}
