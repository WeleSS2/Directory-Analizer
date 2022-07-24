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
	// Graphics loading to textures
	Texture Empty(window_inst);
	Texture ButtonTexture(window_inst);
	Texture Background(window_inst);
	//Texture Arrow(window_inst);
	ButtonTexture.loadFromFile("Graphics/Pngs/button.png");
	Background.loadFromFile("Graphics/Pngs/background.png");
	//Arrow.loadFromFile("Graphics/Pngs/arrow.png");

	// Building buttons
	button Folder(window_inst, "Folder", { 40, 40, 180, 45 }, "Select Folder", { 55, 47 });
	button Analyze(window_inst, "Analyze", { 260, 40, 180, 45 }, "Analyze", { 305, 47 });
	button Exit(window_inst, "Exit", { window_inst->GetWidth() - 240, 40, 180, 45 }, "Exit", { window_inst->GetWidth() - 175, 47 });
	button CurrentFolder(window_inst, "CurrentFolder", { 40, 100, 10, 10 }, "Current Folder: ", { 40, 100 });

	//button Stats(window_inst, "Stats", { 480, 40, 180, 45 }, "Statistics", { 520, 47 });
	//button PerFile(window_inst, "PerFile", { 100, 40, 180, 45 }, "In File", { 145, 47 });
	//button PerFolder(window_inst, "PerFolder", { 320, 40, 180, 45 }, "In Folder", { 355, 47 });
	//button General(window_inst, "General", { 540, 40, 180, 45 }, "General", { 585, 47 });
	//button Back(window_inst, "Back", { 40, 40, 45, 45 });

	button Files(window_inst, "Files", { 40, 150, 10, 10}, "Files amount: ", {40, 150});
	button NonEmptyLines(window_inst, "NonEmptyLines", { 40, 200, 10, 10 }, "Filled lines amount: ", { 40, 200 });
	button EmptyLines(window_inst, "EmptyLines", { 40, 250, 10, 10 }, "Empty lines amount: ", { 40, 250 });
	button Words(window_inst, "Words", { 40, 300, 10, 10 }, "Words amount: ", { 40, 300 });
	button Letters(window_inst, "Letters", { 40, 350, 10, 10 }, "Letters amount: ", { 40, 350 });


	// 1st time menu rendering
	SDL_RenderClear((*window_inst).GetRenderer());
	SDL_RenderCopy(window_inst->GetRenderer(), Background.getTexture(), NULL, NULL);
	Folder.Rerender(&ButtonTexture);
	Analyze.Rerender(&ButtonTexture);
	Exit.Rerender(&ButtonTexture);
	SDL_RenderPresent((*window_inst).GetRenderer());

	// Main app loop variables
	bool exit = 1;
	std::string path = "";
	Algo Anal;


	// Main app loop
	while (exit)
	{
		SDL_RenderClear(window_inst->GetRenderer());

		SDL_Event e;

		// Event loop
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				{
					int x, y;
					SDL_GetMouseState(&x, &y);

					if(checkPosition(x, y, &Folder))
					{
						path = OpenFolder();

						CurrentFolder.ChangeText("Current Folder: " + path);
					}
					if (checkPosition(x, y, &Analyze))
					{
						if (path.size() != 0)
						{
							Anal.setPatch(path);
							Anal.resetStats();
							Threads.push_task([&] {
								Anal.mapFolder(path);
								}
							);
						}
					}
					if (checkPosition(x, y, &Exit))
					{
						exit = 0;
						break;
					}
				}
			}
		}


		// Graphics rendering part
		SDL_RenderCopy(window_inst->GetRenderer(), Background.getTexture(), NULL, NULL);
		Folder.Rerender(&ButtonTexture);
		Analyze.Rerender(&ButtonTexture);

		if (!path.empty() || (Anal.getFilesAmount() != 0))
		{
			CurrentFolder.Rerender(&Empty);
			Files.ChangeText("Files amount: " + std::to_string(Anal.getFilesAmount()));
			Files.Rerender(&Empty);
			NonEmptyLines.ChangeText("Filled lines amount: " + std::to_string(Anal.getLinesAmount()));
			NonEmptyLines.Rerender(&Empty);
			EmptyLines.ChangeText("Empty lines amount: " + std::to_string(Anal.getEmptyLinesAmount()));
			EmptyLines.Rerender(&Empty);
			Words.ChangeText("Words amount: " + std::to_string(Anal.getWordsAmount()));
			Words.Rerender(&Empty);
			Letters.ChangeText("Letters amount: " + std::to_string(Anal.getLettersAmount()));
			Letters.Rerender(&Empty);
		}

		Exit.Rerender(&ButtonTexture);
		SDL_RenderPresent(window_inst->GetRenderer());
	}
}

std::string rerender::pwstr_to_str(PWSTR win_str)
{
	// Changing windows pw string into normal string...
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
	PWSTR FilePath = NULL;

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
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &FilePath);
				
				return pwstr_to_str(FilePath);
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