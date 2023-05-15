//
//  main.cpp
//  SDL_tutorial
//
//  Created by Emiliano Iacopini on 2/22/23.
//

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

enum{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_S,
    KEY_PRESS_SURFACE_P,    //This one is a PNG, cannot be handled inside the for loop
    KEY_PRESS_SURFACE_TOTAL
};

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* currentSurface = NULL;
SDL_Surface* keyPressSurface[KEY_PRESS_SURFACE_TOTAL];

std::string path = "../res/";
std::string imgs[] = {"press.bmp", "up.bmp", "down.bmp", "left.bmp", "right.bmp", "stretch.bmp", "loaded.png"};

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
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init( imgFlags ) & imgFlags)) {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                success = false;
            } else {
                screenSurface = SDL_GetWindowSurface(window);
            }
        }
    }
    
    return success;
}

bool loadMedia(){
    for(int i = 0; i < KEY_PRESS_SURFACE_TOTAL - 1; i++){
        SDL_Surface* loadedSurface = SDL_LoadBMP((path + imgs[i]).c_str());
        if(loadedSurface == NULL){
            printf("Unable to load image \"%s%s\", SDL error: %s\n", path.c_str(), imgs[i].c_str(), SDL_GetError());
            return false;
        } else {
            keyPressSurface[i] = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
            if(keyPressSurface[i] == NULL) {
                printf("Unable to optimize image \"%s%s\", SDL error: %s\n", path.c_str(), imgs[i].c_str(), SDL_GetError());
            }
            SDL_FreeSurface(loadedSurface);
        }
    }
    
    keyPressSurface[KEY_PRESS_SURFACE_P] = IMG_Load((path + imgs[KEY_PRESS_SURFACE_P]).c_str());
    if(keyPressSurface[KEY_PRESS_SURFACE_P] == NULL){
        printf("Unable to load image \"%s%s\", SDL error: %s\n", path.c_str(), imgs[KEY_PRESS_SURFACE_P].c_str(), IMG_GetError());
        return false;
    }
    
    return true;
}

/*SDL_Surface* loadSurface(char* path){
    SDL_Surface* optimizedSurface = NULL;
    
    SDL_Surface* loadedSurface = SDL_LoadBMP(path);
    if(currentSurface == NULL){
        printf("Unable to load image \"%s\", SDL error: %s\n", path, SDL_GetError());
    } else {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
        if(optimizedSurface == NULL) {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
        }
        
        SDL_FreeSurface(loadedSurface);
    }
    return optimizedSurface;
}*/

void close(){
    SDL_FreeSurface(currentSurface);
    currentSurface = NULL;
    
    SDL_DestroyWindow(window);
    window = NULL;
    
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
    
    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH;
    stretchRect.h = SCREEN_HEIGHT;
    
    //Hack to get window to stay up
    SDL_Event e;
    bool quit = false;
    while( quit == false ){
        while( SDL_PollEvent( &e ) ){
            if( e.type == SDL_QUIT){
                quit = true;
            } else if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        currentSurface = keyPressSurface[KEY_PRESS_SURFACE_UP];
                        break;
                    case SDLK_DOWN:
                        currentSurface = keyPressSurface[KEY_PRESS_SURFACE_DOWN];
                        break;
                    case SDLK_LEFT:
                        currentSurface = keyPressSurface[KEY_PRESS_SURFACE_LEFT];
                        break;
                    case SDLK_RIGHT:
                        currentSurface = keyPressSurface[KEY_PRESS_SURFACE_RIGHT];
                        break;
                    case SDLK_s:
                        currentSurface = keyPressSurface[KEY_PRESS_SURFACE_S];
                        break;
                    case SDLK_p:
                        currentSurface = keyPressSurface[KEY_PRESS_SURFACE_P];
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        currentSurface = keyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
                        break;
                }
            }
        }
        
        SDL_BlitScaled(currentSurface, NULL, screenSurface, &stretchRect);
        SDL_UpdateWindowSurface(window);
    }

    close();
    return 0;
    
}
