//
//  LTexture.hpp
//  SDL_Key_States
//
//  Created by Emiliano Iacopini on 3/12/23.
//

#ifndef LTexture_hpp
#define LTexture_hpp

#include <string>
#include <SDL2/SDL.h>

//Texture wrapper class
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile(std::string path);

        #if defined(SDL_TTF_MAJOR_VERSION)
        //Creates image from font string
        bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
        #endif

        //Deallocates texture
        void free();

        //Renders texture at given point
        void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
        //set color modulation
        void setColor(Uint8 red, Uint8 green, Uint8 blue);

        //set blending
        void setBlendMode(SDL_BlendMode blending);

        //set alpha modulation
        void setAlpha(Uint8 alpha);

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;
};

#endif /* LTexture_hpp */
