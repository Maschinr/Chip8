#include "Chip8.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

using namespace std;
using namespace sf;

//TODO imgui gui where things like clockspeed rom and so on can be changed Titlebar and playarea is height - titlebar?
//TODO Change function map to many inline class member functions
//TODO implement all missing opcodes
int main(int argc, char* argv[]) {

    if(argc != 2) {
        cerr << "Error loading Rom" << endl;
        return -1;
    }

    RenderWindow window(VideoMode(1280, 720), "Chip8");
    unique_ptr<Chip8> emulated = make_unique<Chip8>(&window);
    if(!emulated->loadFile(argv[1])) {
        cerr << "Error loading Rom" << endl;
        return -1;
    }

    Clock cycleClock;

    while(window.isOpen()) {
        Event event;

        while(window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::Resized) {
                View view = View(FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y));
	            window.setView(view);
            }
        }

        if(cycleClock.getElapsedTime() > milliseconds(seconds(1).asMilliseconds() / 500)) {
            cycleClock.restart();
            if(!emulated->cycle()) {
                break;
            }
        }
        
    }

    return 0;
}
