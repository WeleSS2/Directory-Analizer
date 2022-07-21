#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include "window_mng.h"

// Graphics libs
#ifdef __linux__	
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"	

#elif _WIN32
#include <windows.h>
#include <ShObjIdl.h>
#include <ShlObj.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#endif

class Texture;
class rerender;
class button;

class Texture {
private:
    std::shared_ptr<Window_Init> window_inst;
protected:
    SDL_Texture* mTexture = NULL;
    TTF_Font* font = NULL;

    int mWidth;
    int mHeight;
public:
    Texture()
    {
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    };

    Texture(std::shared_ptr<Window_Init> inst)
        : window_inst(inst)
    {
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    };

    ~Texture();

    bool loadFromFile(std::string path);

    void free();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    void setBlendMode(SDL_BlendMode blending);

    void setAlpha(Uint8 alpha);

    void render(int x, int y, int w, int h, SDL_Rect* clip = NULL);

    // Button for text
    void renderButton(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
    
    int getWidth();
    int getHeight();

    SDL_Texture* getTexture();

    TTF_Font* getFont()
    {
        return font;
    };

    void loadFont(std::string path, int size)
    {
        font = TTF_OpenFont(path.c_str(), size);
        if (font == NULL)
        {
            std::cout << "Not loaded " << TTF_GetError() << std::endl;
        }
    };

    void setFont(TTF_Font* inp)
    {
        font = inp;
    };
};

class rerender
{
private:
    std::shared_ptr<Window_Init> window_inst;
public:
    // Rerendering require window control object.
    rerender(std::shared_ptr<Window_Init> inst)
        : window_inst(inst)
    {
    };

    void AppLoop();

    SDL_Renderer* getRenderer()
    {
        return (*window_inst).GetRenderer();
    }

    bool checkPosition(int& x, int& y, button* obj);

    void createMainMenu();
    void renderMainMenu();

    void createStatsMenu();
    void renderStatsMenu();

    std::string pwstr_to_str(PWSTR str);
    std::string OpenFolder();

    void Stats();

    void PerFile();

    void PerFolder();

    void General();
};

class button
{
private:
    std::shared_ptr<Window_Init> window_inst;
    SDL_Rect mPosition; // Button position, width and height
    SDL_Point prevPos = { -10000, -10000 };
    SDL_Point fontPosition; // Text position
    SDL_Point prevFontPos = { -10000, -10000 };
    std::string name, text; // Button text
    Texture fontTexture;

public:
    // Constructor for simple buttons
    button(std::shared_ptr<Window_Init> inst, std::string c_name, SDL_Rect c_mPosition)
        : window_inst(inst),
        name(c_name),
        mPosition(c_mPosition)
    {
    }


    // Constructor for buttons with text
    button(std::shared_ptr<Window_Init> inst, std::string c_name, SDL_Rect c_mPosition, std::string c_text, SDL_Point c_text_cord = { -1000, -1000 })
        : window_inst(inst), 
        name(c_name),
        mPosition(c_mPosition),
        text(c_text),
        fontPosition(c_text_cord)
    {
        Texture temp(window_inst);
        fontTexture = temp;
        fontTexture.loadFont("Graphics/Fonts/Bodoni.ttf", 28);
        fontTexture.loadFromRenderedText(text, { 15, 20, 28, 0xFF });
    }

    ~button()
    {
    }

    void setZero()
    {
        mPosition.x = -10000;
        mPosition.y = -10000;
    }
    
    void Rerender(Texture* texture)
    {
        if (mPosition.x == -10000)
        {
            SetPosition(prevPos);
            render_button(texture);
        }
        else
        {
            render_button(texture);
            prevPos = { mPosition.x, mPosition.y };
        }
    }

    void SetPosition(SDL_Point cords)
    {
        mPosition.x = cords.x;
        mPosition.y = cords.y;
    }

    void render_button(Texture* texture)
    {
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { mPosition.x, mPosition.y, mPosition.w, mPosition.h };

        // Fillers
        SDL_Rect* clip = NULL;
        double angle = 0.0;
        SDL_Point* center = NULL;
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        //Render to screen
        SDL_RenderCopyEx(window_inst->GetRenderer(), texture->getTexture(), clip, &renderQuad, angle, center, flip);
        if (text.size() != 0)
        {
            fontTexture.renderButton(fontPosition.x, fontPosition.y);
        }
    };

    SDL_Rect GetPosition()
    {
        return mPosition;
    }

    std::string GetName()
    {
        return name;
    }
    
    std::string GetText()
    {
        return text;
    }
};