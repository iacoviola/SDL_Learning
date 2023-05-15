//
//  main.h
//  SDL_Textures
//
//  Created by Emiliano Iacopini on 3/1/23.
//

#ifndef main_h
#define main_h

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>

bool init();
SDL_Texture* loadTexture(std::string path);
bool loadMedia();
void close();

#endif /* main_h */
