#include "Chip8.hpp"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <memory>

using namespace std;

bool init(SDL_Window** window, SDL_Renderer** renderer, string title, unsigned int width, unsigned int height) {
    *window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    
    if(*window != NULL) {
        *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
        if(*renderer != NULL) {
            int imgFlags = IMG_INIT_PNG;
            if((IMG_Init(imgFlags) & imgFlags)) {
                return true;
            }
        }
    }
    
    return false;
}

void cleanup(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    if(init(&window, &renderer, "Test", 1280, 720) == false) {
        cerr << "Error initializing graphics";
        return -1;
    }

    //Later generic class Emulator
    unique_ptr<Chip8> emulated = make_unique<Chip8>(renderer);
    if(!emulated->loadFile("./roms/BC_test.ch8")) {
        cerr << "Error loading Rom" << endl;
    }

    bool running = true;
    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
               running = false; /* Game ended */
            }
            emulated->event(event);
        }
        
        //Simulation Code
        if(!emulated->cycle()) {
            break;
        }
    }
    
    cleanup(window, renderer);

    return 0;
}
