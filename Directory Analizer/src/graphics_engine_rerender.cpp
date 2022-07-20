#pragma once
#include <iostream>
#include <filesystem>
#include "graphics_engine.h"

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


// Primitive version. I didn't separate correctly a lot of things like buttons management, graphics loading e.t.c. My ui functions and classes aren't ready yet 
// (pseudo ui engine) so i just write out a simplific version to manage this app (Code quality is terrible in this point).
void rerender::AppLoop()
{
	Texture loadBackground(window_inst);
	loadBackground.loadFromFile("Graphics/Pngs/test.png");

	SDL_RenderClear((*window_inst).GetRenderer());
	SDL_RenderCopy((*window_inst).GetRenderer(), loadBackground.getTexture(), NULL, NULL);
	SDL_RenderPresent((*window_inst).GetRenderer());
	SDL_Delay(12312);
	std::cin.get();




}