#pragma once

class CMyPanel;

class CMyFrame : public wxFrame
{
public:
	CMyFrame(const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE);
	~CMyFrame();

	void OpenFile(bool bIfCancelThenExit = false);

	void OnMenuFileOpenFile(wxCommandEvent& eEvent);
	void OnMenuFileExit(wxCommandEvent& eEvent);
	void OnMenuHelpAbout(wxCommandEvent& eEvent);

private:
	enum{
		MENU_FILE_OPENFILE = 200,
		MENU_FILE_EXIT = wxID_EXIT,
		MENU_HELP_ABOUT = wxID_ABOUT
	};

	CMyPanel *m_pMyPanel;
	wxFileDialog	*m_pFileDialog;

	DECLARE_EVENT_TABLE()
};
