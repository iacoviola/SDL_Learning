//
//  main.cpp
//  SDL_Mouse_Events
//
//  Created by Emiliano Iacopini on 3/12/23.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "LTexture.hpp"
#include "Globals.hpp"
#include "Constants.hpp"
#include "LButton.hpp"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

LTexture buttonSpriteTexture;
SDL_Rect spriteClips[BUTTON_SPRITE_TOTAL];

LButton buttons[BUTTON_SPRITE_TOTAL];

bool init(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return false;
    }
    
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
        printf("Warning: linear texture filtering not enabled!");
    }
    
    window = SDL_CreateWindow("SLD Mouse Events", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
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

    SDL_Color textColor = {0, 0, 0};

    if(!buttonSpriteTexture.loadFromFile("../res/button.png")){
        printf("Failed to load button texture image!\n");
        return false;
    }

    for(int i = 0; i < BUTTON_SPRITE_TOTAL; i++){
        spriteClips[i].x = 0;
        spriteClips[i].y = i * BUTTON_HEIGHT;
        spriteClips[i].w = BUTTON_WIDTH;
        spriteClips[i].h = BUTTON_HEIGHT;
    }

    buttons[0].setPosition(0, 0);
    buttons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
    buttons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
    buttons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);

    return true;
}

void close(){
    buttonSpriteTexture.free();
    
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

            for(int i = 0; i < BUTTON_SPRITE_TOTAL; i++){
                buttons[i].handleEvent(&e);
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
                
        for(int i = 0; i < BUTTON_SPRITE_TOTAL; i++){
            buttons[i].render();
        }
        
        SDL_RenderPresent(renderer);
    }

    close();
    return 0;
}
