#include <SDL2/SDL.h>

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

bool is_jumping = false;
bool is_left = false;
bool is_right = false;

namespace world {
    static const float gravity = 0.2f;
    static const float drag = 0.999f;
    static const float ground_drag = 0.9f;
}

class Cube {
    public:
        Cube();
        void draw(SDL_Renderer* renderer);
        void update();

    private:
        int jump_strength = -5;
        int move_speed = 2;
        float size = 20.0f;
        float dy = 0.0f; //Delta y
        float dx = 0.0f; //Delta x
        float y = (WINDOW_HEIGHT / 2.0f) - this->size; //y position
        float x = (WINDOW_WIDTH - this->size) / 2.0f; //x position
        bool isGrounded = true;
};

Cube::Cube(){

}

void Cube::draw(SDL_Renderer* renderer){
    SDL_FRect shape = {this->x, this->y, this->size, this->size};
    SDL_RenderFillRectF(renderer, &shape);
}

void Cube::update(){
    if(is_jumping && this->isGrounded){
        this->dy = this->jump_strength;
    }
    if(is_right){
        this->dx = this->move_speed;
    }
    if(is_left){
        this->dx = -this->move_speed;
    }


    this->dy += world::gravity;
    this->dy *= world::drag;
    this->dx *= isGrounded ? world::ground_drag : world::drag;
    this->y += this->dy;
    this->x += this->dx;

    if(this->y + this->size >= (WINDOW_HEIGHT) / 2.0f){
        this->y = (WINDOW_HEIGHT / 2.0f) - this->size;
        this->dy = 0;
        is_jumping = false;
        this->isGrounded = true;
    } else {
        this->isGrounded = false;
    }

    if(this->x > WINDOW_WIDTH){
        this->x = 0;
    } 
    else if(this->x + this->size < 0){
        this->x = WINDOW_WIDTH;
    }

}

bool init(){
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Could not initialize SDL, SDL_Error: %s", SDL_GetError());
        success = false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
        puts("Warning: linear texture filtering not enabled!");
    }

    gWindow = SDL_CreateWindow((char*)"Cube Jump Testing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if(gWindow == NULL){
        printf("Could not create SDL window, SDL_Error: %s", SDL_GetError());
        success = false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(gRenderer == NULL){
        printf("Could not create SDL renderer, SDL_Error: %s", SDL_GetError());
        success = false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return success;

}

void close(){
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gRenderer = NULL;
    gWindow = NULL;

    SDL_Quit();
}

int main(int argc, char* argv[]){
    if(!init()){
        puts("Error initializing SDL");
        return 0;
    }

    SDL_Event e;
    bool quit = false;
    Cube character;

    while(!quit){
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = true;
            }if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_UP:
                    case SDLK_SPACE:
                        is_jumping = true;
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        is_right = true;
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        is_left = true;
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
            } else if(e.type == SDL_KEYUP){
                switch(e.key.keysym.sym){
                    case SDLK_d:
                    case SDLK_RIGHT:
                        is_right = false;
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        is_left = false;
                        break;
                }
            }
        }
        
        SDL_RenderClear(gRenderer);

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);

        character.update();
        character.draw(gRenderer);

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        SDL_RenderPresent(gRenderer);

    }

    return 0;
}