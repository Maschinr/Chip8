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
//TODO pixels are one off?
//TODO some opcodes are worng, build debugger with imgui
int main(int argc, char* argv[]) {

    if(argc != 2) {
        cerr << "Error loading Rom" << endl;
        return -1;
    }

    RenderWindow window(VideoMode(1280, 720), "Chip8");
    unique_ptr<Chip8> emulated = make_unique<Chip8>();
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

         
            window.clear();

            //Draw playfield from emulator
            RenderTexture texture;
            emulated->draw(texture);
            
            Sprite emuGfx(texture.getTexture());
            emuGfx.setOrigin(emuGfx.getTexture()->getSize().x / 2, emuGfx.getTexture()->getSize().y / 2);
            emuGfx.setPosition(window.getSize().x / 2, window.getSize().y / 2);

            //Todo emu get aspect ratio, and emu get resolution for math
            unsigned int w_width = window.getSize().x;
            w_width = ((int)(w_width / 64)) * 64;
            unsigned int w_height = window.getSize().y;
            w_height = ((int)(w_height / 32)) * 32;
            unsigned int width, height;

            if(w_width / 2 > w_height) { // The playground can't be stretched all the way because height is not enough
                height = w_height;
                width = height * 2;
            } else {
                width = w_width;
                height = width / 2;
            }

            // Width and height are the maximum size
            emuGfx.setScale(width / emuGfx.getTexture()->getSize().x, height / emuGfx.getTexture()->getSize().y);
            window.draw(emuGfx);

            window.display();
        }
    }

    return 0;
}
