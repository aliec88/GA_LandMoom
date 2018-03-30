// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <sstream>
using namespace std;
// TODO: reference additional headers your program requires here
#include "define.h"
#include "Timer.h"
#include "tools.h"
#include "Vector2D.h"
#include "Matrix2D.h"
#include "Ga.h"
#include "Lander.h"
#include "Controller.h"