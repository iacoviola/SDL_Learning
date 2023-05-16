//
//  LButton.hpp
//  SDL_Mouse_Events
//
//  Created by Emiliano Iacopini on 3/12/23.
//

#ifndef LButton_hpp
#define LButton_hpp

#include <SDL2/SDL.h>

#include "Constants.hpp"

class LButton {
    public:
        LButton();

        //Sets top left position
        void setPosition(int x, int y);

        //Handles mouse event
        void handleEvent(SDL_Event* e);

        //Shows button sprite
        void render();
    private:
        //Top left position
        SDL_Point mPosition;

        //Currently used global sprite;
        LButtonSprite mCurrentSprite;
};

#endif /* LButton_hpp */