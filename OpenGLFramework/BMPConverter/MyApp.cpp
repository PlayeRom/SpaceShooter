
#include "stdafx.h"
#include "MyFrame.h"
#include "MyApp.h"

IMPLEMENT_APP( CMyApp )

bool CMyApp::OnInit()
{
    CMyFrame *pFrame = new CMyFrame( _T("BMPConverter"),
		wxPoint( WINDOW_POS_X, WINDOW_POS_Y ),
		wxSize( WINDOW_SIZE_X, WINDOW_SIZE_Y ),
		wxMINIMIZE_BOX/*|wxMAXIMIZE_BOX|wxRESIZE_BORDER*/|wxSYSTEM_MENU|wxCAPTION|wxCLOSE_BOX|wxCLIP_CHILDREN );

    pFrame->Show( true );
	SetTopWindow( pFrame );

    return true;
}
