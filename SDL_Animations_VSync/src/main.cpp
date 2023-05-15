//
//  main.cpp
//  SDL_Keying
//
//  Created by Emiliano Iacopini on 3/12/23.
//

#include <stdio.h>

#include "LTexture.hpp"
#include "Globals.hpp"
#include "Constants.hpp"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect spriteClips[WALKING_ANIMATION_FRAMES];
LTexture spriteTexture;

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
    
    if(!spriteTexture.loadFromFile("../res/foo.png")){
        printf("Failed to load front texture image!\n");
        return false;
    } else {
        spriteClips[0].x = 0;
        spriteClips[0].y = 0;
        spriteClips[0].w = 64;
        spriteClips[0].h = 205;

        spriteClips[1].x = 64;
        spriteClips[1].y = 0;
        spriteClips[1].w = 64;
        spriteClips[1].h = 205;

        spriteClips[2].x = 128;
        spriteClips[2].y = 0;
        spriteClips[2].w = 64;
        spriteClips[2].h = 205;
        
        spriteClips[3].x = 192;
        spriteClips[3].y = 0;
        spriteClips[3].w = 64;
        spriteClips[3].h = 205;
    }
    
    return true;
}

void close(){
    spriteTexture.free();
    
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
    int frame = 0;
    while( quit == false ){
        while( SDL_PollEvent( &e ) ){
            if( e.type == SDL_QUIT){
                quit = true;
            } else if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        
        SDL_Rect* currentClip = &spriteClips[frame / 4];
        spriteTexture.render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
                
        SDL_RenderPresent(renderer);

        frame++;
        if(frame / 4 >= WALKING_ANIMATION_FRAMES){
            frame = 0;
        }
    }

    close();
    return 0;
}
