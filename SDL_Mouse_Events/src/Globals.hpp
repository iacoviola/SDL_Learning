//
//  Globals.hpp
//  SDL_Mouse_Events
//
//  Created by Emiliano Iacopini on 3/12/23.
//

#ifndef Globals_h
#define Globals_h

#include <SDL2/SDL.h>

#include "Constants.hpp"
#include "LTexture.hpp"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern LTexture buttonSpriteTexture;
extern SDL_Rect spriteClips[BUTTON_SPRITE_TOTAL];


#endif /* Globals_h */
