//
//  main.cpp
//  SDL_Force_Feedback
//
//  Created by Emiliano Iacopini on 3/12/23.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include <cmath>
#include <SDL2/SDL_image.h>

#include "LTexture.hpp"
#include "Globals.hpp"
#include "Constants.hpp"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_GameController* gGameController = NULL;

SDL_Joystick* gJoystick = NULL;
SDL_Haptic* gJoyHaptic = NULL;

LTexture gSplashTexture;

bool init(){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0){
        printf("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return false;
    }
    
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
        printf("Warning: linear texture filtering not enabled!");
    }
    //Check for joysticks
    if(SDL_NumJoysticks() < 1){
        printf("Warning: no joysticks connected!");
    } else {
        //Check if first joystick is game controller interface capable
        if(!SDL_IsGameController(0)){
            printf("Warning: Joystick is not game controller interface compatible! SDL Error: %s\n", SDL_GetError());
        }
        else {
            gGameController = SDL_GameControllerOpen(0);
            if(!SDL_GameControllerHasRumble(gGameController)){
                printf("Warning: Game controller does not have rumble! SDL Error: %s\n", SDL_GetError());
            }
        }
        if(gGameController == NULL){
            gJoystick = SDL_JoystickOpen(0);
            if(gJoystick == NULL){
                printf("Warning: unable to open joystick! SDL Error: %s\n", SDL_GetError());
            }
            else {
                //Check if joystick supports haptics
                if(!SDL_JoystickIsHaptic(gJoystick)){
                    printf( "Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError() );
                }
                else {
                    gJoyHaptic = SDL_HapticOpenFromJoystick(gJoystick);
                    if(gJoyHaptic == NULL) {
                        printf("Warning: Unable to get joystick haptics! SDL Error: %s\n", SDL_GetError());
                    }
                    else {
                        if(SDL_HapticRumbleInit(gJoyHaptic) < 0){
                            printf("Warning: Unable to initialize haptic rumble! SDL Error: %s\n", SDL_GetError());
                        }
                    }
                }
            }
        }
    }
    
    window = SDL_CreateWindow("SLD Gamepads Joysticks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
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
    
    if(!gSplashTexture.loadFromFile("../res/splash.png")){
        printf("Failed to load front texture image!\n");
        return false;
    }
    
    return true;
}

void close(){
    gSplashTexture.free();

    //Close game controller
    if(gGameController != NULL){
        SDL_GameControllerClose(gGameController);
    }
    if(gJoystick != NULL){
        SDL_JoystickClose(gJoystick);
    }
    if(gJoyHaptic != NULL){
        SDL_HapticClose(gJoyHaptic);
    }
    gGameController = NULL;
    gJoystick = NULL;
    gJoyHaptic = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    renderer = NULL;
    window = NULL;
    
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char * argv[]) {
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
    int xDir = 0, yDir = 0;
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
            } else if(e.type == SDL_JOYBUTTONDOWN){
                if(gGameController != NULL){
                    if(SDL_GameControllerRumble(gGameController, 0xFFFF * 3/4, 0xFFFF * 3/4, 500) != 0){
                        printf("Warning: Unable to play game contoller rumble! %s\n", SDL_GetError());
                    }
                }
                else if(gJoyHaptic != NULL){
                    if(SDL_HapticRumblePlay(gJoyHaptic, 0xFFFF * 3/4, 500) != 0){
                        printf("Warning: Unable to play haptic rumble! %s\n", SDL_GetError());
                    }
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        gSplashTexture.render((SCREEN_WIDTH - gSplashTexture.getWidth()) / 2, (SCREEN_HEIGHT - gSplashTexture.getHeight()) / 2);
                
        SDL_RenderPresent(renderer);
    }

    close();
    return 0;
}
