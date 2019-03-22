#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <string>
#include <map>
#include <functional>
#include <SFML/Graphics.hpp>

class Chip8 {
public:
    Chip8(sf::RenderWindow* window);
    ~Chip8();
    void reset();
    bool loadFile(std::string path);
    bool cycle();

private:
    void loadFunctions();
    void draw();
    void input();

    sf::RenderWindow* window;
    sf::Clock timerClock;
    
    //Emulator var's
    std::map<unsigned short, std::function<void()>> opFunctions;

    unsigned char fontset[80] =
    { 
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    bool drawFlag;
    unsigned short opcode;  //Current Opcode
    unsigned char memory[4096]; //Memory/4MiB   
    unsigned char V[16];//Registers, each 1 Byte
    unsigned short I; //Index register
    unsigned short pc;//Program Counter
    bool pixels[64][32];//Pixels
    unsigned char dtimer;//Delay Timer
    unsigned char stimer;//Sound Time
    unsigned short stack[16];//Stack
    unsigned short sp;//Stack Pointer
    bool keys[16];
};

#endif //CHIP8_HPP