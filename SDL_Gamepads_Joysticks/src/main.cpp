//
//  main.cpp
//  SDL_Gamepads_Joysticks
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
SDL_Joystick* gameController = NULL;

LTexture arrowTexture;

bool init(){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0){
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
        //Load joystick
        gameController = SDL_JoystickOpen(0);
        if(gameController == NULL){
            printf("Warning: unable to open game controller! SDL Error: %s\n", SDL_GetError());
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
    
    if(!arrowTexture.loadFromFile("../res/arrow.png")){
        printf("Failed to load front texture image!\n");
        return false;
    }
    
    return true;
}

void close(){
    arrowTexture.free();

    //Close game controller
    SDL_JoystickClose(gameController);
    gameController = NULL;
    
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
            } else if(e.type == SDL_JOYAXISMOTION){
                //Motion on controller 0
                if(e.jaxis.which == 0){
                    //X axis motion
                    if(e.jaxis.axis == 0){
                        //Left of dead zone
                        if(e.jaxis.value < - JOYSTICK_DEAD_ZONE){
                            xDir = -1;
                        }
                        //Right of dead zone
                        else if(e.jaxis.value > JOYSTICK_DEAD_ZONE){
                            xDir = 1;
                        }
                        else {
                            xDir = 0;
                        }
                    }
                    //Y axis motion
                    if(e.jaxis.axis == 1){
                        //Above dead zone
                        if(e.jaxis.value > JOYSTICK_DEAD_ZONE){
                            yDir = 1;
                        }
                        //Below dead zone
                        else if(e.jaxis.value < -JOYSTICK_DEAD_ZONE){
                            yDir = -1;
                        } else {
                            yDir = 0;
                        }
                    }
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        
        //Calculate angle
        double joystickAngle = atan2((double)yDir, (double)xDir) * (180.0 / M_PI);

        //Correct angle
        if(xDir == 0 && yDir == 0){
            joystickAngle = 0;
        }

        printf("xDir: %d, yDir: %d\n", xDir, yDir);
        arrowTexture.render((SCREEN_WIDTH - arrowTexture.getWidth()) / 2, (SCREEN_HEIGHT - arrowTexture.getHeight()) / 2, NULL, joystickAngle);
                
        SDL_RenderPresent(renderer);
    }

    close();
    return 0;
}
