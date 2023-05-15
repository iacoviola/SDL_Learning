//
//  main.cpp
//  SDL_Textures
//
//  Created by Emiliano Iacopini on 2/22/23.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;

SDL_Surface* screenSurface = NULL;

SDL_Renderer* renderer = NULL;

SDL_Texture* texture = NULL;

bool init(){
    bool success = true;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SLD could not initialize, SDL error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Create window
        window = SDL_CreateWindow("First SDL application", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL){
            printf("Window could not be created, SDL error: %s\n", SDL_GetError());
            success = false;
        } else {
            
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer == NULL){
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                
                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init( imgFlags ) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                } else {
                    screenSurface = SDL_GetWindowSurface(window);
                }
            }
        }
    }
    
    return success;
}

SDL_Texture* loadTexture (std::string path){
    
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if(newTexture == NULL){
        printf("Unable to create texture from %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    
    return newTexture;
}

bool loadMedia(){
    bool success = true;
    
    texture = loadTexture("../res/texture.png");
    if(texture == NULL){
        printf( "Failed to load texture image!\n" );
        success = false;
    }
    
    return success;
}

void close(){
    
    SDL_DestroyTexture(texture);
    texture = NULL;
    
    SDL_DestroyWindow(window);
    window = NULL;
    
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]){
    
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
        }
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    
    }

    close();
    return 0;
    
}
