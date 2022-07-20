#pragma once
#include <iostream>
#include <tuple>

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

class Window_Init {
private:
    std::string Window_Name_ = "Window";
    SDL_Window* Window_ = NULL;
    SDL_Renderer* Renderer_ = NULL;

public:
    // Default
    Window_Init()
    {
    };

    // Custom
    Window_Init(std::string name)
        : Window_Name_(name)
    {
        this->Sdl_Init();
        this->Take_window_size();
        this->Sdl_Build();
        GetName();
    };

    ~Window_Init()
    {
        SDL_DestroyRenderer(Renderer_);
        SDL_DestroyWindow(Window_);
    };

    
    SDL_Renderer* GetRenderer()
    {
        return Renderer_;
    };

    SDL_Window* GetWindow()
    {
        return Window_;
    };

    std::string GetName()
    {
        std::cout << Window_Name_ << "\n";
        return Window_Name_;
    };

    std::tuple<int, int> return_w_h()
    {
        return std::make_tuple(SCREEN_WIDTH, SCREEN_HEIGHT);
    };

    // Initialize SDL
    bool Sdl_Init();

    // Build up window and renderer
    void Sdl_Build();

    // Initializing window for different resoultions and changing it if needed.
    void Take_window_size();

private:
    //Screen size
    int SCREEN_WIDTH = 0,
        SCREEN_HEIGHT = 0;
};