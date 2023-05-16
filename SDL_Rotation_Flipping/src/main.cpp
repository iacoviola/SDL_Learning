//
//  main.cpp
//  SDL_Rotation_Flipping
//
//  Created by Emiliano Iacopini on 3/12/23.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "LTexture.hpp"
#include "Globals.hpp"
#include "Constants.hpp"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

LTexture arrowTexture;

bool init(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return false;
    }
    
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
        printf("Warning: linear texture filtering not enabled!");
    }
    
    window = SDL_CreateWindow("SLD Animations VSync", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if(window == NULL){
        printf("Window could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if(renderer == NULL){
        printf("Renderer could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    
    int imgflags = IMG_INIT_PNG;
    if(!(IMG_Init(imgflags) & imgflags)){
        printf("SDL_image could not initialize! SDL_image error: %s\n", IMG_GetError());
        return false;
    }
    
    return true;
}

bool loadMedia(){
    
    if(!arrowTexture.loadFromFile("../res/arrow.png")){
        printf("Failed to load front texture image!\n");
        return false;
    }
    
    return true;
}

void close(){
    arrowTexture.free();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    renderer = NULL;
    window = NULL;
    
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, const char * argv[]) {
    if(!init()){
        printf("Failed to initialize!\n");
        return 0;
    }
    if(!loadMedia()){
        printf("Failed to load media!\n");
        return 0;
    }
    
    //Hack to get window to stay up
    SDL_Event e;
    bool quit = false;
    double degrees = 0.0;
    SDL_RendererFlip flipType = SDL_FLIP_NONE;
    while( quit == false ){
        while( SDL_PollEvent( &e ) ){
            if( e.type == SDL_QUIT){
                quit = true;
            } else if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_a:
                        degrees -= 60;
                        break;
                        
                    case SDLK_d:
                        degrees += 60;
                        break;

                    case SDLK_q:
                        flipType = SDL_FLIP_HORIZONTAL;
                        break;

                    case SDLK_w:
                        flipType = SDL_FLIP_NONE;
                        break;

                    case SDLK_e:
                        flipType = SDL_FLIP_VERTICAL;
                        break;   
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        
        arrowTexture.render((SCREEN_WIDTH - arrowTexture.getWidth()) / 2, (SCREEN_HEIGHT - arrowTexture.getHeight()) / 2, NULL, degrees, NULL, flipType);
                
        SDL_RenderPresent(renderer);
    }

    close();
    return 0;
}
