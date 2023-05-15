//
//  main.cpp
//  SDL_Keying
//
//  Created by Emiliano Iacopini on 3/12/23.
//

#include <stdio.h>
#include <SDL2_image/SDL_image.h>

#include "LTexture.hpp"
#include "Globals.hpp"
#include "Constants.hpp"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Rect spriteClips[4];
LTexture spriteSheetTexture;

bool init(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return false;
    }
    
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
        printf("Warning: linear texture filtering not enabled!");
    }
    
    window = SDL_CreateWindow("SLD Color Keying", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if(window == NULL){
        printf("Window could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
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
    if(!spriteSheetTexture.loadFromFile("dots.png")){
        printf("Failed to load Foo's texture image!\n");
        return false;
    }
    
    //Set top left sprite
    spriteClips[0].x =   0;
    spriteClips[0].y =   0;
    spriteClips[0].w = 100;
    spriteClips[0].h = 100;

    //Set top right sprite
    spriteClips[1].x = 100;
    spriteClips[1].y =   0;
    spriteClips[1].w = 100;
    spriteClips[1].h = 100;
    
    //Set bottom left sprite
    spriteClips[2].x =   0;
    spriteClips[2].y = 100;
    spriteClips[2].w = 100;
    spriteClips[2].h = 100;

    //Set bottom right sprite
    spriteClips[3].x = 100;
    spriteClips[3].y = 100;
    spriteClips[3].w = 100;
    spriteClips[3].h = 100;
    
    return true;
}

void close(){
    spriteSheetTexture.free();
    
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
    while(quit == false){
        while(SDL_PollEvent(&e)){
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
        
        //Render top left sprite
        spriteSheetTexture.render(0, 0, &spriteClips[0]);

        //Render top right sprite
        spriteSheetTexture.render(SCREEN_WIDTH - spriteClips[1].w, 0, &spriteClips[1]);

        //Render bottom left sprite
        spriteSheetTexture.render(0, SCREEN_HEIGHT - spriteClips[2].h, &spriteClips[2]);

        //Render bottom right sprite
        spriteSheetTexture.render(SCREEN_WIDTH - spriteClips[3].w, SCREEN_HEIGHT - spriteClips[3].h, &spriteClips[3]);

                
        SDL_RenderPresent(renderer);
    }
}
