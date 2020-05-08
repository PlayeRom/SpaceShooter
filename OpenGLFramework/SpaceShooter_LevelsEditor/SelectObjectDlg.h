#pragma once
//#include "wx/dialog.h"

class CSelectObjectDlg : public wxDialog
{
public:
	CSelectObjectDlg(
					int iActualIndexOfBoss,
					const wxArrayString &strArrayChoices,
					wxWindow* parent,
					wxWindowID id,
					const wxString& title,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxDEFAULT_DIALOG_STYLE,
					const wxString& name = _T("dialogBox"));
	virtual ~CSelectObjectDlg();

	int GetIndexObject()	{ return m_iIndexObject; }

private:
	enum{
		ID_EVENT_BUTTON_OK = 200,
		ID_EVENT_BUTTON_CANCEL
	};

	void OnOK(wxCommandEvent &eEvent);
	void OnCancel(wxCommandEvent &eEvent);

	int m_iIndexObject;

	wxComboBox		*m_pComboSelect;
	wxButton		*m_pButtonOK;
	wxButton		*m_pButtonCancel;

	DECLARE_EVENT_TABLE()
};
