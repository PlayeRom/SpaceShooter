#include "StdAfx.h"
#include "GameControl.h"
#include "WindowData.h"
#include "Console.h"
#include "Cursor.h"
#include "Joystick.h"

CJoystick::CJoystick()
{
	m_bJoySupported = GL_FALSE;
	m_bZCoorSupported = GL_FALSE;
	m_bRudderSupported = GL_FALSE;
	m_iRepeatX = 0;
	m_iRepeatY = 0;
	m_bBindKeyMode = GL_FALSE;
	m_iIndexKey = 0;
}

CJoystick::~CJoystick()
{
}

GLboolean CJoystick::IsJoysick()
{
	UINT uiNumDevs = ::joyGetNumDevs();
	if( uiNumDevs == 0 )
		return GL_FALSE; 

	JOYINFO joyinfo; 
	GLboolean bDev1Attached = ::joyGetPos( JOYSTICKID1, &joyinfo ) == JOYERR_NOERROR;
	GLboolean bDev2Attached = uiNumDevs == 2 && ::joyGetPos( JOYSTICKID2, &joyinfo ) == JOYERR_NOERROR;

	if( bDev1Attached || bDev2Attached )
		return GL_TRUE;
	
	return GL_FALSE;
}

GLboolean CJoystick::Initialize()
{
	m_bJoySupported = GL_FALSE;
	m_bZCoorSupported = GL_FALSE;
	m_bRudderSupported = GL_FALSE;
	m_iRepeatX = 0;
	m_iRepeatY = 0;
	::SecureZeroMemory( m_aStateButton, sizeof( m_aStateButton ) );

	CONSOLE_ADDTEXT_NORMAL( _T("Number of joystick devices: %d"), ::joyGetNumDevs() );

	m_bJoySupported = IsJoysick();

	if( m_bJoySupported ) {
		m_uiJoyID = ::joyGetDevCaps( JOYSTICKID1, &m_sJoyCaps, sizeof( JOYCAPS ) );

		if( m_uiJoyID != JOYERR_NOERROR ) {
			m_uiJoyID = ::joyGetDevCaps( JOYSTICKID2, &m_sJoyCaps, sizeof( JOYCAPS ) );
			if( m_uiJoyID != JOYERR_NOERROR )
				m_bJoySupported = GL_FALSE;
		}
	}

	if( m_bJoySupported ) {
		CONSOLE_ADDTEXT_NORMAL( _T("Joystick supported.") );
		CONSOLE_ADDTEXT_NORMAL( _T("Joystick product name: %s."), m_sJoyCaps.szPname );
		CONSOLE_ADDTEXT_NORMAL( _T("Maximum number of buttons supported by the joystick: %d."), m_sJoyCaps.wMaxButtons );
		CONSOLE_ADDTEXT_NORMAL( _T("Number of joystick buttons: %d."), m_sJoyCaps.wNumButtons );
		CONSOLE_ADDTEXT_NORMAL( _T("Maximum number of axes supported by the joystick: %d."), m_sJoyCaps.wMaxAxes );
		CONSOLE_ADDTEXT_NORMAL( _T("Number of axes currently in use by the joystick: %d."), m_sJoyCaps.wNumAxes );

		CONSOLE_ADDTEXT_NORMAL( _T("Min/Max X-coordinate: %d / %d."), m_sJoyCaps.wXmin, m_sJoyCaps.wXmax );
		CONSOLE_ADDTEXT_NORMAL( _T("Min/Max Y-coordinate: %d / %d."), m_sJoyCaps.wYmin, m_sJoyCaps.wYmax );

		if( m_sJoyCaps.wCaps & JOYCAPS_HASZ ) {
			m_bZCoorSupported = GL_TRUE;
			CONSOLE_ADDTEXT_NORMAL( _T("Joystick has z-coordinate information.") );
		}
		if( m_sJoyCaps.wCaps & JOYCAPS_HASR ) {
			m_bRudderSupported = GL_TRUE;
			CONSOLE_ADDTEXT_NORMAL( _T("Joystick has rudder information.") );
		}
	}
	else
		CONSOLE_ADDTEXT_NORMAL( _T("Joystick not supported.") );

	return m_bJoySupported;
}

GLboolean CJoystick::ServiceMouse()
{
	if( !m_bJoySupported )
		return GL_FALSE;
	
	POINT ptJoy = { 0 };
	POINT ptMouse = { 0 };

	::GetCursorPos( &ptMouse );
	if( GetJoyPos( ptJoy ) ) {
		ptMouse.x += ptJoy.x;
		ptMouse.y += ptJoy.y;
		::SetCursorPos( ptMouse.x, ptMouse.y );

		return GL_TRUE;
	}

	return GL_FALSE;
}

GLboolean CJoystick::GetJoyPos( POINT &ptJoy )
{
	ptJoy.x = ptJoy.y = 0;

//	POINT ptJoy2 = { 0 };

	JOYINFOEX sJoyInfoEx;
	sJoyInfoEx.dwSize = sizeof( JOYINFOEX );
	sJoyInfoEx.dwFlags = JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR;

	if( ::joyGetPosEx( m_uiJoyID, &sJoyInfoEx ) == JOYERR_NOERROR ) {
		ptJoy.x = sJoyInfoEx.dwXpos - ( m_sJoyCaps.wXmax / 2 );
		ptJoy.y = sJoyInfoEx.dwYpos - ( m_sJoyCaps.wYmax / 2 );

//		ptJoy2.x = sJoyInfoEx.dwZpos;// - ( m_sJoyCaps.wZmax / 2 ); // drugi joy gora/dol
//		ptJoy2.y = sJoyInfoEx.dwRpos;// - ( m_sJoyCaps.wRmax / 2 ); // drugi joy lewo/prawo <->

//		CONSOLE_ADDTEXT_NORMAL( _T("Joystick X = %d"), ptJoy.x );
//		CONSOLE_ADDTEXT_NORMAL( _T("Joystick Y = %d"), ptJoy.y );

//		CONSOLE_ADDTEXT_NORMAL( _T("Joystick Z = %d"), ptJoy2.x );
//		CONSOLE_ADDTEXT_NORMAL( _T("Joystick R = %d"), ptJoy2.y );

		SetVariable( ptJoy.x, m_iRepeatX, 17, CWindowData::GetInstance()->SettingFile().iWidth, 1024.0f );
		SetVariable( ptJoy.y, m_iRepeatY, 18, CWindowData::GetInstance()->SettingFile().iHeight, 768.0f );

		return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CJoystick::SetVariable( LONG &lJoyCoord, GLint &iRepeat, GLint iSpeedCtrlIndex, GLint iResolution, GLfloat fResolutionBase )
{
	GLint iSigned = ( lJoyCoord < 0 ) ? -1 : 1;

	lJoyCoord = abs( lJoyCoord );

	//jako ze wywoluje sie co klatek musi byc uzaleznione od czau
	GLboolean bRepeat = GL_FALSE;
	if( CGameControl::GetInstance()->GetSpeedCtrl()->CheckAnimationTime( 50, iSpeedCtrlIndex ) ) {
		bRepeat = GL_TRUE;
		++iRepeat;
	}

	for( GLint i = 1500, j = 0; i < 47000; ++j, i += ( j * 500 ) ) {
		if( lJoyCoord < i ) {
			lJoyCoord = j;
			if( lJoyCoord == 0 && bRepeat )
				iRepeat = 0;
			break;
		}
	}

/*	if( lJoyCoord < 1500 ) {
		lJoyCoord = 0;
		if( bRepeat )
			iRepeat = 0;
	}
	else if( lJoyCoord < 2000 )//+500
		lJoyCoord = 1;
	else if( lJoyCoord < 3000 )//+ 1000
		lJoyCoord = 2;
	else if( lJoyCoord < 4500 )//+ 1500
		lJoyCoord = 3;
	else if( lJoyCoord < 6500 )//+2000
		lJoyCoord = 4;
	else if( lJoyCoord < 9000 )//+2500
		lJoyCoord = 5;
	else if( lJoyCoord < 12000 )//+3000
		lJoyCoord = 6;
	else if( lJoyCoord < 15500 )//+3500
		lJoyCoord = 7;
	else if( lJoyCoord < 19500 )//+4000
		lJoyCoord = 8;
	else if( lJoyCoord < 24000 )//+4500
		lJoyCoord = 9;
	else if( lJoyCoord < 29000 )//+5000
		lJoyCoord = 10;
	else if( lJoyCoord < 34500 )//+5500
		lJoyCoord = 11;
	else
		lJoyCoord = 12;
*/
	lJoyCoord += iRepeat;

	//dla rozdzielczosci 1024x768 - rozdzielczosc bazowa - bez zmian
	//dla wiekszych rozdzielczosci trzeba zwiekszyc ilosc przesuwanych pikseli
	//dla mniejszych rozdzielczosci trzeba odpowiednio zmniejszyc

	if( iResolution != static_cast< GLint >( fResolutionBase ) ) {
		//oblicz procent rozdzielczosci z bazowej
		GLfloat fPercent = ( static_cast< GLfloat >( iResolution ) * 100.0f ) / fResolutionBase;
		//teraz lJoyCoord trzeba zmniejszyc/zwiekszyc o obliczony procent
		GLfloat fNewCoord = ( static_cast< GLfloat >( lJoyCoord ) * fPercent ) / 100.0f;
		lJoyCoord = static_cast< GLint >( fNewCoord );
	}

	//dla menu poruszaj wolniej
	if( CGameControl::GetInstance()->GetCursor()->GetMyCursor() == CCursor::EMenu && lJoyCoord > 3 )
		lJoyCoord /= 2;

	// przywroc znak
	lJoyCoord *= iSigned;
}

GLboolean CJoystick::ServiceButtons()
{
	if( !m_bJoySupported )
		return GL_FALSE;

	JOYINFOEX sJoyInfoEx;
	sJoyInfoEx.dwSize = sizeof( JOYINFOEX );
	sJoyInfoEx.dwFlags = JOY_RETURNBUTTONS;

	if( ::joyGetPosEx( m_uiJoyID, &sJoyInfoEx ) != JOYERR_NOERROR )
		return GL_FALSE;

	if( IsBindKeyMode() ) // tryb przypisania przycisku
		ServiceButtonsForBind( sJoyInfoEx.dwButtons );
	else // tryb reakcji na nacisniecie przycisków
		ServiceButtonsForUse( sJoyInfoEx.dwButtons );

	return GL_TRUE;
}

GLvoid CJoystick::ServiceButtonsForBind( DWORD dwButtons )
{
	if( m_iIndexKey >= 0 && m_iIndexKey < MAX_KEYBOARD_ACTION ) {

		// wez kod klawisza z g_aConstKeyboardAction[ m_iIndexKey ]
		// i wstaw do CWindowData::GetInstance()->SettingFile().aButtonBindKey[ 'ktory button nacisnieto' ]

		// nie mozna uzyc i = GetIdFromButton( sJoyInfoEx.dwButtons )
		// bo w sJoyInfoEx.dwButtons moze byc kilka klawiszy
		for( GLint i = 0; i < MAX_JOY_BUTTONS; ++i ) {
			if( dwButtons & GetButtonFromId( i ) ) {
				if( !m_aStateButton[ i ] ) {
					m_aStateButton[ i ] = GL_TRUE;

					// usun aktualny klawisz g_aConstKeyboardAction[ m_iIndexKey ] z
					// CWindowData::GetInstance()->SettingFile().aButtonBindKey[ x ]
					for( GLint j = 0; j < MAX_JOY_BUTTONS; ++j ) {
						if( g_aConstKeyboardAction[ m_iIndexKey ] == CWindowData::GetInstance()->SettingFile().aButtonBindKey[ j ] )
							CWindowData::GetInstance()->SettingFile().aButtonBindKey[ j ] = 0;
					}

					// przypisz klawisz
					CWindowData::GetInstance()->SettingFile().aButtonBindKey[ i ] = g_aConstKeyboardAction[ m_iIndexKey ];
					SetBindKeyMode( GL_FALSE );
				}
			}
			else {
				if( m_aStateButton[ i ] )
					m_aStateButton[ i ] = GL_FALSE;
			}
		}
	}
}

GLvoid CJoystick::ServiceButtonsForUse( DWORD dwButtons )
{
	POINT ptMouse = { 0 };
	::GetCursorPos( &ptMouse );

	for( GLint i = 0; i < MAX_JOY_BUTTONS; ++i ) {
		if( CWindowData::GetInstance()->SettingFile().aButtonBindKey[ i ] == 0 )
			continue;

		if( dwButtons & GetButtonFromId( i ) ) {
			if( !m_aStateButton[ i ] ) {
				m_aStateButton[ i ] = GL_TRUE;

				//jezeli nacisnieto jakikolwiek przycisk, i jestesmy w menu ( mamy kursor = 1 )
				//to zawsze reagujemy klinieciem
				if( CGameControl::GetInstance()->GetCursor()->GetMyCursor() == CCursor::EMenu ) {
					CGameControl::GetInstance()->MouseLButtonDown( ptMouse.x, ptMouse.y );
					CGameControl::GetInstance()->MouseLButtonUp( ptMouse.x, ptMouse.y );
				}
				else {
					// jezeli akcja "zmiana broni" to wywolujemy MouseWheel
					if( CWindowData::GetInstance()->SettingFile().aButtonBindKey[ i ] == g_aConstKeyboardAction[ 2 ] )
						CGameControl::GetInstance()->MouseWheel( -1 );
					else
						CGameControl::GetInstance()->KeyDown( CWindowData::GetInstance()->SettingFile().aButtonBindKey[ i ] );
				}
			}
		}
		else {
			if( m_aStateButton[ i ] ) {
				m_aStateButton[ i ] = GL_FALSE;

				if( CWindowData::GetInstance()->SettingFile().aButtonBindKey[ i ] != g_aConstKeyboardAction[ 2 ] )
					CGameControl::GetInstance()->KeyUp( CWindowData::GetInstance()->SettingFile().aButtonBindKey[ i ] );
			}
		}
	}
}

GLint CJoystick::GetBindButtonNumFromKey( GLint iIndexKey )
{
	if( iIndexKey < 0 || iIndexKey >= MAX_KEYBOARD_ACTION )
		return 0;

	for( GLint i = 0; i < MAX_JOY_BUTTONS; ++i ) {
		if( CWindowData::GetInstance()->SettingFile().aButtonBindKey[ i ] == g_aConstKeyboardAction[ iIndexKey ] )
			return i + 1; // zwróc numer klawisza na joyu
	}

	return 0; // zaden
}

GLint CJoystick::GetIdFromButton( DWORD dwButton )
{
	switch( dwButton ) {
		case JOY_BUTTON1:
			return 0;
		case JOY_BUTTON2:
			return 1;
		case JOY_BUTTON3:
			return 2;
		case JOY_BUTTON4:
			return 3;
		case JOY_BUTTON5:
			return 4;
		case JOY_BUTTON6:
			return 5;
		case JOY_BUTTON7:
			return 6;
		case JOY_BUTTON8:
			return 7;
		case JOY_BUTTON9:
			return 8;
		case JOY_BUTTON10:
			return 9;
		case JOY_BUTTON11:
			return 10;
		case JOY_BUTTON12:
			return 11;
		case JOY_BUTTON13:
			return 12;
		case JOY_BUTTON14:
			return 13;
		case JOY_BUTTON15:
			return 14;
		case JOY_BUTTON16:
			return 15;
		case JOY_BUTTON17:
			return 16;
		case JOY_BUTTON18:
			return 17;
		case JOY_BUTTON19:
			return 18;
		case JOY_BUTTON20:
			return 19;
		case JOY_BUTTON21:
			return 20;
		case JOY_BUTTON22:
			return 21;
		case JOY_BUTTON23:
			return 22;
		case JOY_BUTTON24:
			return 23;
		case JOY_BUTTON25:
			return 24;
		case JOY_BUTTON26:
			return 25;
		case JOY_BUTTON27:
			return 26;
		case JOY_BUTTON28:
			return 27;
		case JOY_BUTTON29:
			return 28;
		case JOY_BUTTON30:
			return 29;
		case JOY_BUTTON31:
			return 30;
		case JOY_BUTTON32:
			return 31;
	}

	return -1;
}

DWORD CJoystick::GetButtonFromId( GLint iIndex )
{
	switch( iIndex ) {
		case 0:
			return JOY_BUTTON1;
		case 1:
			return JOY_BUTTON2;
		case 2:
			return JOY_BUTTON3;
		case 3:
			return JOY_BUTTON4;
		case 4:
			return JOY_BUTTON5;
		case 5:
			return JOY_BUTTON6;
		case 6:
			return JOY_BUTTON7;
		case 7:
			return JOY_BUTTON8;
		case 8:
			return JOY_BUTTON9;
		case 9:
			return JOY_BUTTON10;
		case 10:
			return JOY_BUTTON11;
		case 11:
			return JOY_BUTTON12;
		case 12:
			return JOY_BUTTON13;
		case 13:
			return JOY_BUTTON14;
		case 14:
			return JOY_BUTTON15;
		case 15:
			return JOY_BUTTON16;
		case 16:
			return JOY_BUTTON17;
		case 17:
			return JOY_BUTTON18;
		case 18:
			return JOY_BUTTON19;
		case 19:
			return JOY_BUTTON20;
		case 20:
			return JOY_BUTTON21;
		case 21:
			return JOY_BUTTON22;
		case 22:
			return JOY_BUTTON23;
		case 23:
			return JOY_BUTTON24;
		case 24:
			return JOY_BUTTON25;
		case 25:
			return JOY_BUTTON26;
		case 26:
			return JOY_BUTTON27;
		case 27:
			return JOY_BUTTON28;
		case 28:
			return JOY_BUTTON29;
		case 29:
			return JOY_BUTTON30;
		case 30:
			return JOY_BUTTON31;
		case 31:
			return JOY_BUTTON32;
	}

	return 0;
}