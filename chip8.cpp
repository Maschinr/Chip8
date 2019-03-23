#include "Chip8.hpp"

#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>
#include <memory.h>

using namespace std;
using namespace sf;

Chip8::Chip8() {
    this->loadFunctions();
    this->reset();
}

Chip8::~Chip8() {

}

void Chip8::reset() {
    this->pc = 0x200;
    this->opcode = 0;
    this->I = 0;
    this->sp = 0;
    this->drawFlag = false;
    this->dtimer = 0;
    this->stimer = 0;
    this->lastFrameKey = -1;

    //Clear memory
    memset(this->memory, 0, 4096);
    memset(this->pixels, 0, 64 * 32);
    memset(this->keys, 0, sizeof(bool) * 16);

    //Load fontset
    memcpy(&this->memory[0], &this->fontset[0], 80);

}

bool Chip8::loadFile(string path) {
    ifstream rom(path, ios::binary);

    if(rom.is_open()) {
        this->reset();
        //TODO change -> Second parameter of istreambuf_iterator describes that no traits should be used
        vector<unsigned char> buffer(istreambuf_iterator<char>(rom), {});

        // TODO cpp way
        for(size_t i = 0; i < buffer.size(); ++i) {
            this->memory[i + 512] = buffer[i];
        }

        
        return true;
    } else {
        return false;
    }
}

bool Chip8::cycle() {

    this->input();

    //Fetch opcode, current position + cur position + 1 because opcode is 16 bits
    this->opcode = this->memory[this->pc] << 8 | this->memory[this->pc + 1];

    cout << "Performing opcode: " << hex << uppercase << this->opcode << dec << nouppercase << endl; 
    
    if ((this->opcode & 0xF000) == 0x0000) { // Zero is special because of unclear identifier, crash if rca 1802 call
        if(this->opFunctions.find(this->opcode) != this->opFunctions.end()) {
            this->opFunctions[this->opcode]();
        } else {
            cerr << "RCA 1802 Call" << endl;
            return false;
        }
    } else if ((this->opcode & 0xF000) == 0x8000) { // 8 has 9 possible values found out by the first and last
        this->opFunctions[(this->opcode & 0xF00F)]();
    } else if ((this->opcode & 0xF000) == 0xF000 || (this->opcode & 0xF000) == 0xE000) { //E has 2 and F has 9 possible codes found out by the first and the last 2 digits
        this->opFunctions[(this->opcode & 0xF0FF)]();
    } else { // If its an exclusive opcode
        this->opFunctions[(this->opcode & 0xF000)]();
    }

    if(timerClock.getElapsedTime() > milliseconds(seconds(1).asMilliseconds() / 60)) {
        timerClock.restart();
        if( this->dtimer > 0) {
            --this->dtimer;
        }
            
        if(this->stimer > 0) {
            if(stimer == 1) {
                cout << "TODO SOUND" << endl;
            }

            --this->stimer;
        }  
    }
   
    return true;
}

void Chip8::input() {
    //Check key presses
    //TODO maybe map or somewhat like that?
    this->lastFrameKey = -1;
    if (Keyboard::isKeyPressed(Keyboard::X)) {
        this->keys[0] = true;
        this->lastFrameKey = 0;
    } else {
        this->keys[0] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::Num1)) {
        this->keys[1] = true;
        this->lastFrameKey = 1;
    } else {
        this->keys[1] = false;
    }
    
    if (Keyboard::isKeyPressed(Keyboard::Num2)) {
        this->keys[2] = true;
        this->lastFrameKey = 2;
    } else {
        this->keys[2] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::Num3)) {
        this->keys[3] = true;
        this->lastFrameKey = 3;
    } else {
        this->keys[3] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::Q)) {
        this->keys[4] = true;
        this->lastFrameKey = 4;
    } else {
        this->keys[4] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::W)) {
        this->keys[5] = true;
        this->lastFrameKey = 5;
    } else {
        this->keys[5] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::E)) {
        this->keys[6] = true;
        this->lastFrameKey = 6;
    } else {
        this->keys[6] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::A)) {
        this->keys[7] = true;
        this->lastFrameKey = 7;
    } else {
        this->keys[7] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::S)) {
        this->keys[8] = true;
        this->lastFrameKey = 8;
    } else {
        this->keys[8] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::D)) {
        this->keys[9] = true;
        this->lastFrameKey = 9;
    } else {
        this->keys[9] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::Y)) {
        this->keys[10] = true;
        this->lastFrameKey = 10;
    } else {
        this->keys[10] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::C)) {
        this->keys[11] = true;
        this->lastFrameKey = 11;
    } else {
        this->keys[11] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::Num4)) {
        this->keys[12] = true;
        this->lastFrameKey = 12;
    } else {
        this->keys[12] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::R)) {
        this->keys[13] = true;
        this->lastFrameKey = 13;
    } else {
        this->keys[13] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::F)) {
        this->keys[14] = true;
        this->lastFrameKey = 14;
    } else {
        this->keys[14] = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::V)) {
        this->keys[15] = true;
        this->lastFrameKey = 15;
    } else {
        this->keys[15] = false;
    }
}

void Chip8::loadFunctions() {
    //Load all Functions into map TODO use mnemonics in map and "decode" the opcode check speed
    //this->opFunctions[0x0000] = [](){};           // 0x0NNN Calls RCA 1802 program at address NNN. Not necessary for most ROMs.
    this->opFunctions[0x00E0] = [this] () {         // 0x00E0 Clears the screen.
        //cout << "0x00E0 Clears the screen." << endl;
        memset(this->pixels, 0, 64 * 32);
        this->pc += 2;
    }; 

    this->opFunctions[0x00EE] = [this] () {         // 0x00EE Returns from a subroutine.
        //cout << "0x00EE Returns from a subroutine." << endl;
        this->sp--;
        this->pc = this->stack[this->sp];
        this->pc += 2; // TODO maybe false
    }; 

    this->opFunctions[0x1000] = [this] () {         // 0x1NNN Jumps to address NNN.
        //cout << "0x1NNN Jumps to address NNN." << endl;
        this->pc = this->opcode & 0x0FFF; 
    };

    this->opFunctions[0x2000] = [this] () {         // 0x2NNN Calls subroutine at NNN.
        //cout << "0x2NNN Calls subroutine at NNN." << endl;
        this->stack[this->sp] = this->pc;
        ++this->sp;
        this->pc = this->opcode & 0x0FFF;
    }; 

    this->opFunctions[0x3000] = [this] () {         // 0x3XNN Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
        //cout << "0x3XNN Skips the next instruction if VX equals NN." << endl;
        
        if(this->V[((this->opcode & 0x0F00) >> 8)] == (this->opcode & 0x00FF)) {
            this->pc += 4;
        } else {
            this->pc += 2;
        }
    }; 

    this->opFunctions[0x4000] = [this] () {         // 0x4XNN Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
        //cout << "0x4XNN Skips the next instruction if VX doesn't equal NN." << endl;
        
        if(this->V[((this->opcode & 0x0F00) >> 8)] != (this->opcode & 0x00FF)) {
            this->pc += 4;
        } else {
            this->pc += 2;
        }
    }; 

    this->opFunctions[0x5000] = [this] () {         // 0x5XY0 Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
        //cout << "0x5XY0 Skips the next instruction if VX equals VY." << endl;
        
        if(this->V[((this->opcode & 0x0F00) >> 8)] == this->V[((this->opcode & 0x00F0) >> 4)]) {
            this->pc += 4;
        } else {
            this->pc += 2;
        }
    }; 

    this->opFunctions[0x6000] = [this] () {         // 0x6XNN Sets VX to NN.
        //cout << "0x6XNN Sets VX to NN." << endl;
        this->V[((this->opcode & 0x0F00) >> 8)] = this->opcode & 0x00FF;
        this->pc += 2;
    }; 

    this->opFunctions[0x7000] = [this] () {         // 0x7XNN Adds NN to VX. (Carry flag is not changed)
        //cout << "0x7XNN Adds NN to VX." << endl;
        this->V[((this->opcode & 0x0F00) >> 8)] += this->opcode & 0x00FF;
        this->pc += 2;
    };

    this->opFunctions[0x8000] = [this] () {         // 0x8XY0 Sets VX to the value of VY.
        //cout << "0x8XY0 Sets VX to the value of VY." << endl;
        this->V[((this->opcode & 0x0F00) >> 8)] = this->V[((this->opcode & 0x00F0) >> 4)];
        this->pc += 2;
    };

    this->opFunctions[0x8001] = [this] () {         // 0x8XY1 Sets VX to VX or VY. (Bitwise OR operation)
        //cout << "0x8XY1 Sets VX to VX or VY. (Bitwise OR operation)" << endl;
        this->V[((this->opcode & 0x0F00) >> 8)] = (this->V[((this->opcode & 0x0F00) >> 8)] | this->V[((this->opcode & 0x00F0) >> 4)]);
        this->pc += 2;
    };

    this->opFunctions[0x8002] = [this] () {         // 0x8XY2 Sets VX to VX and VY. (Bitwise AND operation)
        //cout << "0x8XY2 Sets VX to VX and VY." << endl;
        this->V[((this->opcode & 0x0F00) >> 8)] = (this->V[((this->opcode & 0x0F00) >> 8)] & this->V[((this->opcode & 0x00F0) >> 4)]);
        this->pc += 2;
    };

    this->opFunctions[0x8003] = [this] () {         // 0x8XY3 Sets VX to VX xor VY.
        //cout << "0x8XY3 Sets VX to VX xor VY." << endl;
        this->V[((this->opcode & 0x0F00) >> 8)] = (this->V[((this->opcode & 0x0F00) >> 8)] ^ this->V[((this->opcode & 0x00F0) >> 4)]);
        this->pc += 2;
    };

    this->opFunctions[0x8004] = [this] () {         // 0x8XY4 Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
        //cout << "0x8XY4 Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't." << endl;

        //if vy is larger than vx - 255 e.g if vx is the missing number to get to 255 is smaller than vy then theres an overflow
        if(this->V[((opcode & 0x00F0) >> 4)] > (V[((this->opcode & 0x0F00) >> 8)] - 0xFF)) {
            this->V[0xF] = 1;
        } else {
            this->V[0xF] = 0;
        }
        //VX                                    VY
        this->V[((opcode & 0x0F00) >> 8)] += this->V[((opcode & 0x00F0) >> 4)];
        this->pc += 2;  
    };

    this->opFunctions[0x8005] = [this] () {         // 0x8XY5 VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
        //cout << "0x8XY5 VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't." << endl;
        
        //if vy is larger than vx then there is a borrow
        //VY                                        VX
        if(this->V[((opcode & 0x00F0) >> 4)] > (V[((this->opcode & 0x0F00) >> 8)])) {
            this->V[0xF] = 0;
        } else {
            this->V[0xF] = 1;
        }

        //VX                                    VY
        this->V[((opcode & 0x0F00) >> 8)] -= this->V[((opcode & 0x00F0) >> 4)];
        this->pc += 2;  
    };

    this->opFunctions[0x8006] = [this] () {         // 0x8XY6 Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
        //cout << "0x8XY6 Stores the least significant bit of VX in VF and then shifts VX to the right by 1." << endl;
        
        this->V[0xF] = (this->V[((this->opcode & 0x0F00) >> 8)] & 0x01);
        this->V[((this->opcode & 0x0F00) >> 8)] = this->V[((this->opcode & 0x0F00) >> 8)] >> 1;
        this->pc += 2;   
    };

    this->opFunctions[0x8007] = [this] () {         // 0x8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
        //cout << "0x8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't." << endl;
        
        //if vy is larger than vx then there is a borrow
        //VY                                        VX
        if(this->V[((opcode & 0x00F0) >> 4)] < (V[((this->opcode & 0x0F00) >> 8)])) {
            this->V[0xF] = 0;
        } else {
            this->V[0xF] = 1;
        }

        //VX                                    VY                                  VX
        this->V[((opcode & 0x0F00) >> 8)] = this->V[((opcode & 0x00F0) >> 4)] - this->V[((opcode & 0x0F00) >> 8)];
        this->pc += 2;  
    };

    this->opFunctions[0x800E] = [this] () {         // 0x8XYE Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
        //cout << "0x8XYE Stores the most significant bit of VX in VF and then shifts VX to the left by 1." << endl;
        
        this->V[0xF] = ((this->V[((this->opcode & 0x0F00) >> 8)] & 0x80)) >> 7;
        this->V[((this->opcode & 0x0F00) >> 8)] <<= 1;
        this->pc += 2;  
    };

    this->opFunctions[0x9000] = [this] () {         // 0x9XY0 Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
        //cout << "0x9XY0 Skips the next instruction if VX doesn't equal VY." << endl;
        
        if(this->V[((this->opcode & 0x0F00) >> 8)] != this->V[((this->opcode & 0x00F0) >> 4)]) {
            this->pc += 4;
        } else {
            this->pc += 2;
        }
    };

    this->opFunctions[0xA000] = [this] () {         // 0xANNN Sets I to the address NNN.
        //cout << "0xANNN Sets I to the address NNN." << endl;
        this->I = this->opcode & 0x0FFF;
        this->pc += 2;
    };

    this->opFunctions[0xC000] = [this] () {         // 0xCXNN Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
        //cout << "0xCXNN Sets VX to the result of a bitwise and operation on a random number and NN." << endl;
        unsigned short randNum = rand() % 255;
        
        this->V[((this->opcode & 0x0F00) >> 8)] = (randNum & (this->opcode & 0x00FF));
        this->pc += 2;
    };

    this->opFunctions[0xD000] = [this] () {         // 0xDXYN Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
        //cout << "0xDXYN Draw." << endl;
        unsigned short x = this->V[(this->opcode & 0x0F00) >> 8]; //Xpos
        unsigned short y = this->V[(this->opcode & 0x00F0) >> 4]; //Ypos
        unsigned short n = this->opcode & 0x000F; //Height
        unsigned char pixel;

        this->V[0xF] = 0; //Collision carry flag

        for(int row = 0; row < n; row++) {
            //Get row pixel values
            pixel = this->memory[this->I + row];
            for(int column = 0; column < 8; column++) { // Draw width of 8
                if((pixel & (0x80 >> column)) != 0) { // if its getting set active
                    if(this->pixels[x + column][y + row] == 1) { // if it is already active
                        //Set collision flag
                        //cout << "collision" << x << "!" << y << endl;
                        this->V[0xF] = 1;
                    }
                    this->pixels[x + column][y + row] ^= 1; //Set pixel with xor (active if different, off if same)
                }
            }
        }
        this->drawFlag = true;
        this->pc += 2;
    };

    this->opFunctions[0xE09E] = [this] () {         // 0xEX9E Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
        //cout << "0xEX9E Skips the next instruction if the key stored in VX is pressed." << endl;

        if(this->keys[this->V[((this->opcode & 0x0F00) >> 8)]] == true) {
            this->pc += 4;
        } else {
            this->pc += 2;
        }
    };

    this->opFunctions[0xE0A1] = [this] () {         // 0xEXA1 Skips the next instruction if the key stored in VX isn't pressed.
        //cout << "0xEXA1 Skips the next instruction if the key stored in VX isn't pressed." << endl;

        if(this->keys[this->V[((this->opcode & 0x0F00) >> 8)]] != true) {
            this->pc += 4;
        } else {
            this->pc += 2;
        }
    };

    this->opFunctions[0xF007] = [this] () {         // 0xFX07 Sets VX to the value of the delay timer.
        //cout << "0xFX07 Sets VX to the value of the delay timer." << endl;

        this->V[((this->opcode & 0x0F00) >> 8)] = this->dtimer;
        this->pc += 2;
    };

    this->opFunctions[0xF00A] = [this] () {         // 0xFX0A A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
        //cout << "0xFX0A A key press is awaited, and then stored in VX." << endl;

        if(this->lastFrameKey != -1) {
            this->V[(this->opcode & 0x0F00) >> 8] = this->lastFrameKey;
            this->pc += 2;
        }
    };

    this->opFunctions[0xF015] = [this] () {         // 0xFX15 Sets the delay timer to VX.
        //cout << "0xFX15 Sets the delay timer to VX." << endl;

        this->dtimer = ((this->opcode & 0x0F00) >> 8);
        this->pc += 2;
    };

    this->opFunctions[0xF018] = [this] () {         // 0xFX18 Sets the sound timer to VX.
        //cout << "0xFX18 Sets the sound timer to VX.." << endl;

        this->stimer = ((this->opcode & 0x0F00) >> 8);
        this->pc += 2;
    };

    this->opFunctions[0xF01E] = [this] () {         // 0xFX1E Adds VX to I.
        //cout << "0xFX1E Adds VX to I." << endl;
        this->I += this->V[((this->opcode & 0x0F00) >> 8)];
        this->pc += 2;
    };

    this->opFunctions[0xF029] = [this] () {         // 0xFX29 Font loading.
        //cout << "0xFX29 Font loading." << endl;
        unsigned short x = ((this->opcode & 0x0F00) >> 8);
        this->I = this->V[x] * 5;
        this->pc += 2;
    };

    this->opFunctions[0xF033] = [this] () {         // 0xFX33 BCD Transform.
        //cout << "0xFX33 BCD Transform." << endl;
        //Solution from http://www.multigesture.net/wp-content/uploads/mirror/goldroad/chip8.shtml
        //TODO own version
        this->memory[I] = this->V[(this->opcode & 0x0F00) >> 8] / 100;
        this->memory[I + 1] = (this->V[(this->opcode & 0x0F00) >> 8] / 10) % 10;
        this->memory[I + 2] = (this->V[(this->opcode & 0x0F00) >> 8] % 100) % 10;
        this->pc += 2;
    };

    this->opFunctions[0xF055] = [this] () {         // 0xFX55 Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
        //cout << "0xFX55 Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified." << endl;
        unsigned short x = ((this->opcode & 0x0F00) >> 8); //Last Register to fill
        for(int i = 0; i <= x; i++) {
            this->memory[this->I + i] = this->V[i];
        }
        this->pc += 2;
    };

    this->opFunctions[0xF065] = [this] () {         // 0xFX65 Register Fill.
        //cout << "0xFX65 Register Fill." << endl;
        unsigned short x = ((this->opcode & 0x0F00) >> 8); //Last Register to fill
        for(int i = 0; i <= x; i++) {
            this->V[i] = this->memory[this->I + i];
        }
        this->pc += 2;
    };
}

void Chip8::draw(RenderTexture& texture) {
    if(!texture.create(64, 32)) {
        throw "Failed to create texture!";
    }

    texture.clear(Color::Red);

    RectangleShape rect(Vector2f(1, 1)); // Draw one pixel
    rect.setFillColor(Color::White);
    
    for(int x = 0; x < 64; x++) {
        for(int y = 0; y < 32; y++) {
            if(this->pixels[x][y] == true) {
                rect.setPosition(Vector2f(x, y));
                texture.draw(rect);
            }
        }
    }
    texture.display();
}