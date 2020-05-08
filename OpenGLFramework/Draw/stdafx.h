// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

//#define _3DSLOADER_

// TODO: reference additional headers your program requires here
#include "../OpenGLFramework/define.h"
#include <windows.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include "../OpenGLFramework/include/wglext.h"		//WGL extensions
#include "../OpenGLFramework/include/glext.h"		//GL extensions
#include <vector>
#include <tchar.h>
#include <math.h>
#include "Vector.h"
#include "StencilShadow.h"
#include "../OpenGLFramework/UnicodeHelperFunc.h"
#include "../OpenGLFramework/WindowData.h"
#include "../Logger/Logger.h"
#include "../SpeedControl/Profiler.h"