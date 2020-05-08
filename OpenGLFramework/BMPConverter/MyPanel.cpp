#include "StdAfx.h"
#include "Convert.h"
#include "MyPanel.h"

//#include <vld.h>

CMyPanel::CMyPanel( wxFrame *frame, const wxPoint& pos, const wxSize& size, long style )
       : wxPanel( frame, wxID_ANY, pos, size, style )
{
	m_bReady = false;

	m_pLog = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxPoint( 5, 5 ), wxSize( 775, 255 ),
		wxHSCROLL|wxTE_READONLY|wxTE_MULTILINE );

	m_pConvert = new CConvert( m_pLog );
	
	m_bReady = true;
}

CMyPanel::~CMyPanel()
{
	delete m_pConvert;
}

bool CMyPanel::ConvertBMPFileToJEH( wxString strFileName )
{
	return m_pConvert->ConvertBMPtoJEH( strFileName.c_str() );
}

bool CMyPanel::ConvertJEHFileToBMP( wxString strFileName )
{
	return m_pConvert->ConvertJEHtoBMP( strFileName.c_str() );
}

bool CMyPanel::ConvertAllBMPtoJEH( wxString strPath )
{
	return m_pConvert->ConvertAllBMPtoJEH( strPath.c_str() );
}

bool CMyPanel::ConvertAllJEHtoBMP( wxString strPath )
{
	return m_pConvert->ConvertAllJEHtoBMP( strPath.c_str() );
}

void CMyPanel::AddLogText( wxString strText )
{
	*m_pLog << strText;
}