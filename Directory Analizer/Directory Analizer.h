// Directory Analizer.h : Include file for standard system include files,
// or project specific include files.

#pragma once

// Basic libs
#include <iostream>

// Graphics libs
#ifdef __linux__	
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"	

#elif _WIN32
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#endif

// Additional project files
#include "src/window_mng.h"
#include "src/graphics_engine.h"

// TODO: Reference additional headers your program requires here.
