//
//  LTexture.cpp
//  SDL_Key_States
//
//  Created by Emiliano Iacopini on 3/12/23.
//

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "LTexture.hpp"
#include "Globals.hpp"

LTexture::LTexture(){
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture(){
    free();
}

bool LTexture::loadFromFile(std::string path){
    free();
    
    SDL_Texture* tmpTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL){
        printf("Unable to load image %s! SDL_image error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }
    
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    
    tmpTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(tmpTexture == NULL){
        printf("Unable to create texture from %s! SDL error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }
    
    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;
    
    SDL_FreeSurface(loadedSurface);
    
    mTexture = tmpTexture;
    
    return true;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor){
    //Remove preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if(textSurface == NULL){
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    //Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(mTexture == NULL){
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Get image dimensions
    mWidth = textSurface->w;
    mHeight = textSurface->h;

    SDL_FreeSurface(textSurface);

    return true;
}
#endif

void LTexture::free(){
    if(mTexture != NULL){
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    //Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

    SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending){
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha){
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight;
}
