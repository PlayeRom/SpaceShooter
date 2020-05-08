
#include "stdafx.h"
#include "MyPanel.h"
#include "MyFrame.h"

BEGIN_EVENT_TABLE( CMyFrame, wxFrame )
	EVT_MENU( MENU_FILE_OPENFILE_BMP,	CMyFrame::OnMenuFileOpenFile )
	EVT_MENU( MENU_FILE_OPENFILE_JEH,	CMyFrame::OnMenuFileOpenFile )
	EVT_MENU( MENU_FILE_OPENDIR_BMP,	CMyFrame::OnMenuFileOpenDir )
	EVT_MENU( MENU_FILE_OPENDIR_JEH,	CMyFrame::OnMenuFileOpenDir )
    EVT_MENU( MENU_FILE_EXIT,			CMyFrame::OnMenuFileExit )
    EVT_MENU( MENU_HELP_ABOUT,			CMyFrame::OnMenuHelpAbout )
END_EVENT_TABLE()

CMyFrame::CMyFrame( const wxString& title, const wxPoint& pos, const wxSize& size, long style )
       : wxFrame( NULL, wxID_ANY, title, pos, size, style )
{
	// set the frame icon
	//SetIcon( wxICON( sample ) );

	wxMenu *pMenuFile = new wxMenu;
	wxMenu *pMenuHelp = new wxMenu;
	pMenuFile->Append( MENU_FILE_OPENFILE_BMP, _T("Open BMP file and convert to JEH...\tAlt-1"), _T("Open BMP file") );
	pMenuFile->Append( MENU_FILE_OPENFILE_JEH, _T("Open JEH file and convert to BMP...\tAlt-2"), _T("Open JEH file") );
	pMenuFile->AppendSeparator();
	pMenuFile->Append( MENU_FILE_OPENDIR_BMP, _T("Open dir and convert all to JEH...\tAlt-3"), _T("Open BMP file") );
	pMenuFile->Append( MENU_FILE_OPENDIR_JEH, _T("Open dir and convert all to BMP...\tAlt-4"), _T("Open JEH file") );
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
	SetStatusText( _T("") );

	m_pMyPanel = new CMyPanel( this );

	m_pOpenFile = new wxFileDialog( NULL, wxFileSelectorPromptStr,
		_T("C:\\Documents and Settings\\admin\\Moje dokumenty\\Visual Studio 2005\\Projects\\OpenGLFramework\\SpaceShooter\\OpenGLFramework\\OpenGLFramework\\textures"), wxEmptyString,
		_T("BMP files (*.bmp)|*.bmp"), wxOPEN, wxDefaultPosition );

	m_pOpenDir = new wxDirDialog( NULL, wxDirSelectorPromptStr, _T("C:\\Documents and Settings\\admin\\Moje dokumenty\\Visual Studio 2005\\Projects\\OpenGLFramework\\SpaceShooter\\OpenGLFramework\\OpenGLFramework\\textures") );
}

CMyFrame::~CMyFrame()
{
	delete m_pOpenFile;
	delete m_pOpenDir;
}

void CMyFrame::OnMenuFileOpenFile( wxCommandEvent& eEvent )
{
	switch( eEvent.GetId() ) {
		case MENU_FILE_OPENFILE_BMP:
			m_pOpenFile->SetWildcard( _T("BMP files (*.bmp)|*.bmp") );
			if( m_pOpenFile->ShowModal() == wxID_OK) {
				m_pMyPanel->AddLogText( _T("-------------------------\n") );
				m_pMyPanel->ConvertBMPFileToJEH( m_pOpenFile->GetPath() );
				SetStatusText( _T("Convert BMP file to JEH") );
			}
			break;
		case MENU_FILE_OPENFILE_JEH:
			m_pOpenFile->SetWildcard( _T("JEH files (*.jeh)|*.jeh") );
			if( m_pOpenFile->ShowModal() == wxID_OK) {
				m_pMyPanel->AddLogText( _T("-------------------------\n") );
				m_pMyPanel->ConvertJEHFileToBMP( m_pOpenFile->GetPath() );
				SetStatusText( _T("Convert JEH file to BMP") );
			}
			break;
	}
}

void CMyFrame::OnMenuFileOpenDir( wxCommandEvent& eEvent )
{
	switch( eEvent.GetId() ) {
		case MENU_FILE_OPENDIR_BMP:
			if( m_pOpenDir->ShowModal() == wxID_OK) {
				m_pMyPanel->AddLogText( _T("-------------------------\n") );
				m_pMyPanel->ConvertAllBMPtoJEH( m_pOpenDir->GetPath() );
				SetStatusText( _T("Convert all BMP files to JEH") );
			}
			break;
		case MENU_FILE_OPENDIR_JEH:
			if( m_pOpenDir->ShowModal() == wxID_OK) {
				m_pMyPanel->AddLogText( _T("-------------------------\n") );
				m_pMyPanel->ConvertAllJEHtoBMP( m_pOpenDir->GetPath() );
				SetStatusText( _T("Convert all JEH files to BMP") );
			}
			break;
	}
}

void CMyFrame::OnMenuFileExit( wxCommandEvent& WXUNUSED( eEvent ) )
{
	Close( true ); // true is to force the frame to close
}

void CMyFrame::OnMenuHelpAbout( wxCommandEvent& WXUNUSED( eEvent ) )
{
	wxString msgStr;
	msgStr.Printf( _T("BMPConverter.\nVersion: %s"), _T(VERSION ) );

	wxMessageBox( msgStr, _T("About"), wxOK | wxICON_INFORMATION, this );
}
