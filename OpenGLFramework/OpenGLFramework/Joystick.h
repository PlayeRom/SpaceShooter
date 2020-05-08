#pragma once

const GLint MAX_JOY_BUTTONS = 32;
const GLint MAX_KEYBOARD_ACTION = 9;

// definicaje klawiszy klawiatury jako stale
const DWORD g_aConstKeyboardAction[ MAX_KEYBOARD_ACTION ] = { VK_SPACE,		// strzal
															  VK_RETURN,	// zdalna rakieta
															  ( DWORD ) -1,	// zmiana broni jako MouseWheel, tj. 1,2,3,4,5 i od nowa, 1,2,3 itd.
															  VK_CONTROL,	// unik
															  'C',			// kamera
															  'K',			// kolor celownika
															  'P',			// pauza
															  VK_F1,		// pomoc
															  VK_ESCAPE };	// wyjscie

class CJoystick
{
public:
	CJoystick();
	~CJoystick();

	GLboolean Initialize();
	inline GLboolean IsSupported() { return m_bJoySupported; }
	GLboolean ServiceMouse();
	GLboolean ServiceButtons();
	GLboolean IsJoysick();

	GLvoid SetBindKeyMode( GLboolean bMode, GLint iIndexKye = 0 ) { m_bBindKeyMode = bMode; m_iIndexKey = iIndexKye; }
	GLboolean IsBindKeyMode() { return m_bBindKeyMode; }
	GLint GetIndexBindKey() { return m_iIndexKey; }

	GLint GetBindButtonNumFromKey( GLint iIndexKey );

private:
	GLboolean m_bJoySupported;
	GLboolean m_bZCoorSupported;
	GLboolean m_bRudderSupported;

	JOYCAPS		m_sJoyCaps;
	MMRESULT	m_uiJoyID;

	GLboolean	m_bBindKeyMode;	// joy pracuje w trybie przypisania klawisza
	GLint		m_iIndexKey;	// identyfikator klawisza, wg kolejnosci 0 - strzal, 1 - Odpalenie / detonacja zdalnej rakiety, itd.

	GLboolean GetJoyPos( POINT &ptJoy );
	GLvoid SetVariable( LONG &x, GLint &iRepeat, GLint iSpeedCtrlIndex, GLint iResolution, GLfloat fResolutionBase );

	GLvoid ServiceButtonsForBind( DWORD dwButtons );
	GLvoid ServiceButtonsForUse( DWORD dwButtons );

	GLint m_iRepeatX;
	GLint m_iRepeatY;

	GLboolean m_aStateButton[ MAX_JOY_BUTTONS ];
	GLint GetIdFromButton( DWORD dwButton );
	DWORD GetButtonFromId( GLint iIndex );
};
