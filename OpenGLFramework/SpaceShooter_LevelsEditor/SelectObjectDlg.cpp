#include "StdAfx.h"
#include "SelectObjectDlg.h"

BEGIN_EVENT_TABLE( CSelectObjectDlg, wxDialog )
	EVT_BUTTON( ID_EVENT_BUTTON_OK,		CSelectObjectDlg::OnOK)
	EVT_BUTTON( ID_EVENT_BUTTON_CANCEL,	CSelectObjectDlg::OnCancel )
END_EVENT_TABLE()

CSelectObjectDlg::CSelectObjectDlg(
								   int iActualIndexOfBoss,
								   const wxArrayString &strArrayChoices,
								   wxWindow* parent,
								   wxWindowID id,
								   const wxString& title,
								   const wxPoint& pos,
								   const wxSize& size,
								   long style,
								   const wxString& name )
								   : wxDialog( parent, id, title, pos, size, style, name )
{
	wxString strValue = wxEmptyString;
	if( strArrayChoices.Count() > 0 ) {
		if( iActualIndexOfBoss == -1 )
			strValue = strArrayChoices[ 0 ];
		else if( iActualIndexOfBoss > -1 && iActualIndexOfBoss < static_cast<int>( strArrayChoices.Count() ) )
			strValue = strArrayChoices[ iActualIndexOfBoss ];
	}

	m_pComboSelect = new wxComboBox( this, -1, strValue, wxPoint( 10, 10 ), wxSize( 220, 24 ), strArrayChoices, wxCB_DROPDOWN | wxCB_READONLY );

	m_pButtonOK = new wxButton( this, ID_EVENT_BUTTON_OK, _T("OK"), wxPoint( 10, 50 ), wxSize( 100, 24 ) );
	m_pButtonCancel = new wxButton( this, ID_EVENT_BUTTON_CANCEL, _T("Cancel"), wxPoint( 120, 50 ), wxSize( 100, 24 ) );

	SetAffirmativeId( ID_EVENT_BUTTON_OK );
	SetEscapeId( ID_EVENT_BUTTON_CANCEL );
}

CSelectObjectDlg::~CSelectObjectDlg()
{
}

void CSelectObjectDlg::OnOK( wxCommandEvent &WXUNUSED( eEvent ) )
{
	EndModal( m_pComboSelect->GetCurrentSelection() );
}

void CSelectObjectDlg::OnCancel( wxCommandEvent &WXUNUSED( eEvent ) )
{
	EndModal( -1 );
}