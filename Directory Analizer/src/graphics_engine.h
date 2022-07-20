#pragma once
#include <iostream>
#include "window_mng.h"

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


class Texture {
private:
    Window_Init* window_inst;
protected:
    SDL_Texture* mTexture = NULL;
    TTF_Font* Orbitron = NULL;

    int mWidth;
    int mHeight;
public:
    Texture();

    // Shallow copy constructor
    Texture(Window_Init* inst);

    ~Texture();

    bool loadFromFile(std::string path);

#if defined(SDL_TTF_MAJOR_VERSION)
    bool loadFromRenderedText(TTF_Font* font, std::string textureText, SDL_Color textColor);
#endif

    void free();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    void setBlendMode(SDL_BlendMode blending);

    void setAlpha(Uint8 alpha);

    void render(int x, int y, int w, int h, SDL_Rect* clip = NULL);

    // Button for text
    void renderButton(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    
    int getWidth();
    int getHeight();

    SDL_Texture* getTexture();
};

class rerender
{
private:
    Window_Init* window_inst;
public:
    // Rerendering require window control object.
    rerender(Window_Init* inst)
    {
        window_inst = inst;
    };

    void AppLoop();

    SDL_Renderer* getRenderer()
    {
        return (*window_inst).GetRenderer();
    }
};