#pragma once
#include <iostream>
#include <tuple>

// Graphics libs
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

class Window_Init {
private:
    std::string Window_Name_ = "";
    SDL_Window* Window_ = NULL;
    SDL_Renderer* Renderer_;

public:
    Window_Init(std::string name)
        : Window_Name_(name)
    {
        this->Sdl_Init();
        this->Take_window_size();
        this->Sdl_Build();
    }

    ~Window_Init()
    {
        SDL_DestroyRenderer(Renderer_);
        SDL_DestroyWindow(Window_);
    }

    
    SDL_Renderer* GetRenderer()
    {
        return Renderer_;
    }

    SDL_Window* GetWindow()
    {
        return Window_;
    }

    std::tuple<int, int> return_w_h()
    {
        return std::make_tuple(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

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