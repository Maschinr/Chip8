#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include <memory.h>
#include <iostream>

class Chip8 {
public:
    Chip8();
    ~Chip8();
    void hardReset();
    void softReset();
    bool loadFile(std::string path);
    void cycle();
    void draw(sf::RenderTexture& texture);
    bool isRomLoaded() { return this->romLoaded; }
    void drawMenuBarGUI();
    void drawMiscGUI();
private:
    void input();
    sf::Clock timerClock;
    
    bool romLoaded;

    //GUI var's
    bool memoryViewerOpened;
    bool stackViewerOpened;
    bool registerViewerOpened;

    //Emulator var's
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
    short lastFrameKey;

    // Emulator functions TODO all
    void clearDisplay();
    void returnSubRoutine();
    void jumpAddress(unsigned short NNN);
    void callAddress(unsigned short NNN);
    void skipEqual(unsigned short X, unsigned short NN);
    void skipNEqual(unsigned short X, unsigned short NN);
    void skipEqualRegister(unsigned short X, unsigned short Y); //5XY0
    void set(unsigned short X, unsigned short NN); 
    void addNC(unsigned short X, unsigned short NN); // NC = Not Carry
    void assignRegister(unsigned short X, unsigned short Y);
    void assignOrRegister(unsigned short X, unsigned short Y);
    void assignAndRegister(unsigned short X, unsigned short Y);
    void assignXorRegister(unsigned short X, unsigned short Y);
    void add(unsigned short X, unsigned short Y);
    void subY(unsigned short X, unsigned short Y);
    void shiftRight(unsigned short X); // 8XY6
    void subX(unsigned short X, unsigned short Y);
    void shiftLeft(unsigned short X);
    void skipNEqualRegister(unsigned short X, unsigned short Y);
    void setIndex(unsigned short NNN);
    void jumpRegister(unsigned short NNN); // BNNN
    void random(unsigned short X, unsigned short NN);
    void draw(unsigned short X, unsigned short Y, unsigned short N); //DXYN
    void keyPressed(unsigned short X);
    void keyNPressed(unsigned short X);
    void getDelayTimer(unsigned short X);
    void awaitKeyPress(unsigned short X);
    void setDelayTimer(unsigned short X);
    void setSoundTimer(unsigned short X);
    void addToIndex(unsigned short X);
    void setIndexToChar(unsigned short X);
    void bcdTransform(unsigned short X);
    void saveRegister(unsigned short X);
    void loadRegister(unsigned short X);
};

//Inline Implementations
inline
void Chip8::clearDisplay() {
    memset(this->pixels, 0, 64 * 32);
    this->pc += 2;
}

inline
void Chip8::returnSubRoutine() {
    this->sp--;
    this->pc = this->stack[this->sp];
    this->pc += 2; // TODO maybe false
}

inline
void Chip8::jumpAddress(unsigned short NNN) {
    this->pc = NNN; 
}

inline
void Chip8::callAddress(unsigned short NNN) {
    this->stack[this->sp] = this->pc;
    ++this->sp;
    this->pc = NNN;
}

inline
void Chip8::skipEqual(unsigned short X, unsigned short NN) {
    if(this->V[X] == NN) {
        this->pc += 4;
    } else {
        this->pc += 2;
    }
}

inline
void Chip8::skipNEqual(unsigned short X, unsigned short NN) {
    if(this->V[X] != NN) {
        this->pc += 4;
    } else {
        this->pc += 2;
    }
}

inline
void Chip8::skipEqualRegister(unsigned short X, unsigned short Y) { 
    if(this->V[X] == this->V[Y]) {
        this->pc += 4;
    } else {
        this->pc += 2;
    }
}

inline
void Chip8::set(unsigned short X, unsigned short NN) {
    this->V[X] = NN;
    this->pc += 2;
}

inline
void Chip8::addNC(unsigned short X, unsigned short NN) {
    this->V[X] += NN;
    this->pc += 2;
}

inline
void Chip8::assignRegister(unsigned short X, unsigned short Y) {
    this->V[X] = this->V[Y];
    this->pc += 2;
}

inline
void Chip8::assignOrRegister(unsigned short X, unsigned short Y) {
    this->V[X] = this->V[X] | this->V[Y];
    this->pc += 2;
}

inline
void Chip8::assignAndRegister(unsigned short X, unsigned short Y) {
    this->V[X] = this->V[X] & this->V[Y];
    this->pc += 2;
}

inline
void Chip8::assignXorRegister(unsigned short X, unsigned short Y) {
    this->V[X] = this->V[X] ^ this->V[Y];
    this->pc += 2;
}

inline
void Chip8::add(unsigned short X, unsigned short Y) {
    //if vy is larger than vx - 255 e.g if the missing number to get to 255 from x is smaller than vy then theres an overflow
    if(this->V[X] > this->V[Y] - 0xFF) {
        this->V[0xF] = 1;
    } else {
        this->V[0xF] = 0;
    }
    this->V[X] += this->V[Y];
    this->pc += 2;  
}

inline
void Chip8::subY(unsigned short X, unsigned short Y) {
      //if vx is smaller than vy then there is a borrow
    if(this->V[X] < this->V[Y]) {
        this->V[0xF] = 0;
    } else {
        this->V[0xF] = 1;
    }

    this->V[X] -= this->V[Y];
    this->pc += 2;  
}

inline
void Chip8::shiftRight(unsigned short X) {
    this->V[0xF] = this->V[X] & 0x01;
    this->V[X] >>= 1;
    this->pc += 2;  
}

inline
void Chip8::subX(unsigned short X, unsigned short Y) {
    //if vx is larger than vy then there is a borrow
    if(this->V[X] > this->V[Y]) {
        this->V[0xF] = 0;
    } else {
        this->V[0xF] = 1;
    }

    this->V[X] = this->V[Y] - this->V[X];
    this->pc += 2;  
}

inline
void Chip8::shiftLeft(unsigned short X) {
    this->V[0xF] = (this->V[X] & 0x80) >> 7;
    this->V[X] <<= 1;
    this->pc += 2;  
}

inline
void Chip8::skipNEqualRegister(unsigned short X, unsigned short Y) {
    if(this->V[X] != this->V[Y]) {
        this->pc += 4;
    } else {
        this->pc += 2;
    }
}

inline
void Chip8::setIndex(unsigned short NNN) {
    this->I = NNN;
    this->pc += 2;
}

inline
void Chip8::jumpRegister(unsigned short NNN) {
    this->pc = NNN + this->V[0];
}

inline
void Chip8::random(unsigned short X, unsigned short NN) {
    unsigned short randNum = rand() % 255;
    this->V[X] = randNum & NN;
    this->pc += 2;
}

inline
void Chip8::draw(unsigned short X, unsigned short Y, unsigned short N) {
    unsigned char pixel;
    this->V[0xF] = 0; //Collision carry flag

    for(int row = 0; row < N; row++) {
        //Get row pixel values
        pixel = this->memory[this->I + row];
        for(int column = 0; column < 8; column++) { // Draw width of 8
            if((pixel & (0x80 >> column)) != 0) { // if its getting set active
                if(this->pixels[this->V[X] + column][this->V[Y] + row] == 1) { // if it is already active
                    //Set collision flag
                    this->V[0xF] = 1;
                }
                // -1 is necessary because in draw call the number 64 is the most right pxiel and the number one the most left, it doesnt start from 0 zero but from one
                this->pixels[this->V[X] + column][this->V[Y] + row] ^= 1; //Set pixel with xor (active if different, off if same)
            }
        }
    }
    this->drawFlag = true;
    this->pc += 2;
}

inline
void Chip8::keyPressed(unsigned short X) {
    if(this->keys[this->V[X]] == true) {
        this->pc += 4;
    } else {
        this->pc += 2;
    }
}

inline
void Chip8::keyNPressed(unsigned short X) {
    if(this->keys[this->V[X]] != true) {
        this->pc += 4;
    } else {
        this->pc += 2;
    }
}

inline
void Chip8::getDelayTimer(unsigned short X) {
    this->V[X] = this->dtimer;
    this->pc += 2;
}

inline
void Chip8::awaitKeyPress(unsigned short X) {
    if(this->lastFrameKey != -1) {
        this->V[X] = this->lastFrameKey;
        this->pc += 2;
    } //else
    //NOP
}

inline
void Chip8::setDelayTimer(unsigned short X) {
    this->dtimer = this->V[X];
    this->pc += 2;
}

inline
void Chip8::setSoundTimer(unsigned short X) {
    this->stimer = this->V[X];
    this->pc += 2;
}

inline
void Chip8::addToIndex(unsigned short X) {
    this->I += this->V[X];
    this->pc += 2;
}

inline
void Chip8::setIndexToChar(unsigned short X) {
    this->I = this->V[X] * 5;
    this->pc += 2;
}

inline
void Chip8::bcdTransform(unsigned short X) {
    //Solution from http://www.multigesture.net/wp-content/uploads/mirror/goldroad/chip8.shtml
    //TODO own version
    this->memory[I] = this->V[X] / 100;
    this->memory[I + 1] = (this->V[X] / 10) % 10;
    this->memory[I + 2] = (this->V[X] % 100) % 10;
    this->pc += 2;
}

inline
void Chip8::saveRegister(unsigned short X) {
    for(int i = 0; i <= X; i++) {
        this->memory[this->I + i] = this->V[i];
    }
    this->pc += 2;
}

inline
void Chip8::loadRegister(unsigned short X) {
    for(int i = 0; i <= X; i++) {
        this->V[i] = this->memory[this->I + i];
    }
    this->pc += 2;
}

#endif //CHIP8_HPP