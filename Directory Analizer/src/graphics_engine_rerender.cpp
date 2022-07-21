#pragma once
#include <iostream>
#include <filesystem>
#include "graphics_engine.h"
#include "algorithms.h"

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
	std::cout << "Entering app loop" << std::endl;
	// Graphics loading to textures
	Texture ButtonTexture(window_inst);
	Texture Background(window_inst);
	//Texture Arrow(window_inst);
	ButtonTexture.loadFromFile("Graphics/Pngs/button.png");
	Background.loadFromFile("Graphics/Pngs/background.png");
	//Arrow.loadFromFile("Graphics/Pngs/arrow.png");

	// Building buttons
	button Folder(window_inst, "Folder", { 40, 40, 180, 45 }, "Select Folder", { 55, 47 });
	button Analyze(window_inst, "Analyze", { 260, 40, 180, 45 }, "Analyze", { 305, 47 });
	//button Stats(window_inst, "Stats", { 480, 40, 180, 45 }, "Statistics", { 520, 47 });
	//button PerFile(window_inst, "PerFile", { 100, 40, 180, 45 }, "In File", { 145, 47 });
	//button PerFolder(window_inst, "PerFolder", { 320, 40, 180, 45 }, "In Folder", { 355, 47 });
	//button General(window_inst, "General", { 540, 40, 180, 45 }, "General", { 585, 47 });
	//button Back(window_inst, "Back", { 40, 40, 45, 45 });
	button Exit(window_inst, "Exit", { window_inst->GetWidth() - 240, 40, 180, 45 }, "Exit", { window_inst->GetWidth() - 175, 47 });

	// 1st time menu rendering
	SDL_RenderClear((*window_inst).GetRenderer());
	SDL_RenderCopy(window_inst->GetRenderer(), Background.getTexture(), NULL, NULL);
	Folder.Rerender(&ButtonTexture);
	//Stats.Rerender(&ButtonTexture);
	Analyze.Rerender(&ButtonTexture);
	Exit.Rerender(&ButtonTexture);
	SDL_RenderPresent((*window_inst).GetRenderer());


	// Main app loop
	bool exit = 1;
	std::string path = "";

	//std::cout << "Pre main loop" << "\n";
	while (exit)
	{
		SDL_Event e;

		while (SDL_WaitEvent(&e))
		{
			SDL_Log("Event");
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_Log("Click");
				{
					int x, y;
					SDL_GetMouseState(&x, &y);

					if(checkPosition(x, y, &Folder))
					{
						path = OpenFolder();
					}
					/*else if (checkPosition(x, y, &Stats))
					{
						SDL_RenderClear(window_inst->GetRenderer());
						SDL_RenderCopy(window_inst->GetRenderer(), Background.getTexture(), NULL, NULL);


						Folder.setZero();
						Stats.setZero();
						Analyze.setZero();

						Back.Rerender(&Arrow);
					}*/
					else if (checkPosition(x, y, &Analyze))
					{
						if (path.size() != 0)
						{
							std::cout << "Size != 0" << "\n";
							algo Anal(path);
						}
					}
					/*else if (checkPosition(x, y, &Back))
					{
						SDL_RenderClear(window_inst->GetRenderer());
						SDL_RenderCopy(window_inst->GetRenderer(), Background.getTexture(), NULL, NULL);
						
						Back.setZero();

						Analyze.Rerender(&ButtonTexture);
						Folder.Rerender(&ButtonTexture);
						Stats.Rerender(&ButtonTexture);
					}*/
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

std::string rerender::pwstr_to_str(PWSTR win_str)
{
	wchar_t* tmp = win_str;
	std::wstring ws(tmp);
	std::string str;
	size_t size;
	str.resize(ws.length());
	wcstombs_s(&size, &str[0], str.size() + 1, ws.c_str(), ws.size());
	return str;
}

std::string rerender::OpenFolder()
{
#ifdef _WIN32
	IFileOpenDialog* pFileOpen;
	PWSTR pszFilePath = NULL;

	// Create the FileOpenDialog object.
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
	if (SUCCEEDED(hr))
	{
		// Set to folder selection
		DWORD dwOptions;
		if (SUCCEEDED(pFileOpen->GetOptions(&dwOptions)))
		{
			pFileOpen->SetOptions(dwOptions | FOS_PICKFOLDERS);
		}
		pFileOpen->Show(NULL);

		// Get path
		if (SUCCEEDED(hr))
		{
			IShellItem* pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
				
				return pwstr_to_str(pszFilePath);
			}
		}
		pFileOpen->Release();
		return "";
	}
#elif __linux__	

#endif
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