//
//  LTexture.cpp
//  SDL_Alpha_Blending
//
//  Created by Emiliano Iacopini on 3/12/23.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

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
    
    return mTexture != NULL;
}

void LTexture::free(){
    if(mTexture != NULL){
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(int x, int y, SDL_Rect* clip){
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    //Set clip rendering dimensions
    if(clip != NULL){
        renderQuad.w = clip->w;
		renderQuad.h = clip->h;
    }

    SDL_RenderCopy(renderer, mTexture, clip, &renderQuad);
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
