//
//  main.cpp
//  SDL_Key_States
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

LTexture upTexture;
LTexture downTexture;
LTexture pressTexture;
LTexture rightTexture;
LTexture leftTexture;

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

    if(!upTexture.loadFromFile("../res/up.png")){
        printf("Failed to load button texture image!\n");
        return false;
    }

    if(!downTexture.loadFromFile("../res/down.png")){
        printf("Failed to load button texture image!\n");
        return false;
    }

    if(!pressTexture.loadFromFile("../res/press.png")){
        printf("Failed to load button texture image!\n");
        return false;
    }
    
    if(!rightTexture.loadFromFile("../res/right.png")){
        printf("Failed to load button texture image!\n");
        return false;
    }

    if(!leftTexture.loadFromFile("../res/left.png")){
        printf("Failed to load button texture image!\n");
        return false;
    }

    return true;
}

void close(){
    upTexture.free();
    downTexture.free();
    pressTexture.free();
    rightTexture.free();
    leftTexture.free();
    
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
    LTexture* currentTexture = NULL;
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

            const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
            if(currentKeyStates[SDL_SCANCODE_UP]){
                currentTexture = &upTexture;
            } else if (currentKeyStates[SDL_SCANCODE_DOWN]){
                currentTexture = &downTexture;
            } else if (currentKeyStates[SDL_SCANCODE_LEFT]){
                currentTexture = &leftTexture;
            } else if (currentKeyStates[SDL_SCANCODE_RIGHT]){
                currentTexture = &rightTexture;
            } else {
                currentTexture = &pressTexture;
            }

        }
        
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        currentTexture->render(0 , 0);
        
        SDL_RenderPresent(renderer);
    }

    close();
    return 0;
}
