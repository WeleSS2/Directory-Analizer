#pragma once
#include "window_mng.h"

//Creating rendring window with custom name
void Window_Init::Sdl_Build()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Could not initilize SDL!", SDL_GetError();
    }
    else
    {
        // Check did linear filtering is enabled
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
            std::cout << "Filtering to linear not enabled" << "\n";

        //Create window
        Window_ = SDL_CreateWindow(Window_Name_.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (Window_ == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            // Create render surface
            Renderer_ = SDL_CreateRenderer(Window_ , 0, SDL_RENDERER_ACCELERATED);
            if (Renderer_ == NULL)
                std::cout << "Error while creating renderer!" << "\n";

            else
            {
                SDL_SetRenderDrawColor(Renderer_, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                
                // Initialize SDL_image
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                }

                // Initialize SDL_ttf
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                }
            }
        }
    }
}

// Setting up window size depending on current screen resolution
void Window_Init::Take_window_size()
{
    // Take screen
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    SCREEN_HEIGHT = DM.h;
    SCREEN_WIDTH = DM.w;
}

// Initialization of SDL
bool Window_Init::Sdl_Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        system("pause");
        return 1;
    }
}