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


// Main app loop, not divided correctly (primitive)
void rerender::AppLoop()
{
	// Graphics loading to textures
	Texture ButtonTexture(window_inst);
	Texture Background(window_inst);
	Texture Arrow(window_inst);
	ButtonTexture.loadFromFile("Graphics/Pngs/button.png");
	Background.loadFromFile("Graphics/Pngs/background.png");
	Arrow.loadFromFile("Graphics/Pngs/arrow.png");

	// Building buttons
	button Folder(window_inst, "Folder", { 40, 40, 180, 45 }, "Select Folder", { 55, 47 });
	button Stats(window_inst, "Stats", { 260, 40, 180, 45 }, "Statistics", { 305, 47 });
	button PerFile(window_inst, "PerFile", { 100, 40, 180, 45 }, "In File", { 145, 47 });
	button PerFolder(window_inst, "PerFolder", { 320, 40, 180, 45 }, "In Folder", { 355, 47 });
	button General(window_inst, "General", { 540, 40, 180, 45 }, "General", { 585, 47 });
	button Back(window_inst, "Back", { 40, 40, 45, 45 });
	button Exit(window_inst, "Exit", { window_inst->GetWidth() - 240, 40, 180, 45 }, "Exit", { window_inst->GetWidth() - 175, 47 });

	// 1st time menu rendering
	SDL_RenderClear((*window_inst).GetRenderer());
	SDL_RenderCopy(window_inst->GetRenderer(), Background.getTexture(), NULL, NULL);
	Folder.render_button(&ButtonTexture);
	Stats.render_button(&ButtonTexture);
	Exit.render_button(&ButtonTexture);
	SDL_RenderPresent((*window_inst).GetRenderer());


	// Main app loop
	bool exit = 1;
	while (exit)
	{
		SDL_Delay(5);
		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				{
					int x, y;
					SDL_GetMouseState(&x, &y);

					if(checkPosition(x, y, &Folder))
					{
						OpenFolder();
					}
					else if (checkPosition(x, y, &Stats))
					{
						SDL_RenderClear(window_inst->GetRenderer());
						SDL_RenderCopy(window_inst->GetRenderer(), Background.getTexture(), NULL, NULL);


						Folder.setZero();
						Stats.setZero();
						Back.Rerender(&Arrow);
						PerFile.Rerender(&ButtonTexture);
						PerFolder.Rerender(&ButtonTexture);
						General.Rerender(&ButtonTexture);
					}
					else if (checkPosition(x, y, &Back))
					{
						SDL_RenderClear(window_inst->GetRenderer());
						SDL_RenderCopy(window_inst->GetRenderer(), Background.getTexture(), NULL, NULL);
						
						Back.setZero();
						PerFile.setZero();
						PerFolder.setZero();
						General.setZero();


						Folder.Rerender(&ButtonTexture);
						Stats.Rerender(&ButtonTexture);
					}
					else if (checkPosition(x, y, &PerFile))
					{
						
					}
					else if (checkPosition(x, y, &Exit))
					{
						exit = 0;
					}

				}

				Exit.Rerender(&ButtonTexture);
				SDL_RenderPresent(window_inst->GetRenderer());
			}
		}
	}
}

void rerender::OpenFolder()
{
	std::cout << "Work" << "\n";
}

bool rerender::checkPosition(int& x, int& y, button* obj)
{
	if (x < obj->GetPosition().x + obj->GetPosition().w && x > obj->GetPosition().x)
	{
		if (y < obj->GetPosition().y + obj->GetPosition().h && y > obj->GetPosition().y)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

void rerender::Stats()
{

}