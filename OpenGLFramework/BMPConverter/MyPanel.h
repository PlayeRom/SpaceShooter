#pragma once

class CConvert;

class CMyPanel : public wxPanel
{
public:
	CMyPanel(wxFrame *frame,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL);
    virtual ~CMyPanel();

	bool ConvertBMPFileToJEH(wxString strFileName);
	bool ConvertJEHFileToBMP(wxString strFileName);
	bool ConvertAllBMPtoJEH(wxString strPath);
	bool ConvertAllJEHtoBMP(wxString strPath);
	void AddLogText(wxString strText);

private:
	wxTextCtrl		*m_pLog;

	CConvert	*m_pConvert;
	bool m_bReady;
};
