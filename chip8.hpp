#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <string>
#include <SDL.h>
#include <map>
#include <functional>

class Chip8 {
public:
    Chip8(SDL_Renderer* renderer);
    ~Chip8();
    void reset();
    bool loadFile(std::string path);
    bool cycle();
    void event(SDL_Event& event);

private:
    void loadFunctions();
    void draw();

    SDL_Renderer* renderer;
    bool allKeys[322];

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


    //Op Switch TODO Replace?
    //Descriptions from https://en.wikipedia.org/wiki/CHIP-8#Opcode_table
    /*switch(this->opcode & 0xF000) { //Switch the first 4 bits

        case 0x0000:
            switch(this->opcode) {

                case 0x00E0: //Clears the screen.
                    break;

                case 0x00EE: //Returns from a subroutine.
                    this->sp--;
                    this->pc = this->stack[this->sp];
                    break;

                default: //Calls RCA 1802 program at address NNN. Not necessary for most ROMs.
                    cerr << "RCA 1802 Call" << endl;
                    return false;
                    break;
            }
            break;

        case 0x1000: //1NNN Jumps to address NNN.
            this->pc = this->opcode & 0x0FFF;
            break;

        case 0x2000: //2NNN Calls subroutine at NNN.
            this->stack[this->sp] = this->pc;
            ++this->sp;
            this->pc = this->opcode & 0x0FFF;
            break;

        case 0x3000: //3XNN Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
            if(this->V[(this->opcode & 0x0F00) >> 8] == this->opcode & 0x00FF) {
                this->pc += 4;
            } else {
                this->pc += 2;
            }
            break;

        case 0x6000: //6XNN Sets VX to NN.
            this->V[(this->opcode & 0x0F00) >> 8] = this->opcode & 0x00FF;
            this->pc += 2;
            break;

        case 0x7000: //7XNN Adds NN to VX. (Carry flag is not changed)
            this->V[(this->opcode & 0x0F00) >> 8] += this->opcode & 0x00FF;
            this->pc += 2;
            break;

        case 0xA000: //Sets I to the address NNN.
            this->I = this->opcode & 0x0FFF;
            this->pc += 2;
            break;
        
        case 0xD000: 
            //Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
            //Each row of 8 pixels is read as bit-coded starting from memory location I; 
            //I value doesn’t change after the execution of this instruction. 
            //As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
            { // Scope for variables
                unsigned short x = this->V[(this->opcode & 0x0F00) >> 8];
                unsigned short y = this->V[(this->opcode & 0x00F0) >> 4];
                unsigned short n = this->opcode & 0x000F; //Height
                unsigned char pixel;

                //cout << "Drawing Sprite at " << x << " " <<  y << " height " << n << endl;

                this->V[0xF] = 0; //Collision carry flag

                for(int row = 0; row < n; row++) {
                    //Get row pixel values
                    pixel = this->memory[this->I + row];
                    for(int column = 0; column < 8; column++) { // Draw width of 8
                        if((pixel & (0x80 >> column)) != 0) { // if its getting set active
                            if(this->pixels[x + column][y + row] == 1) { // if it is already active
                                //Set collision flag
                                this->V[0xF] = 1;
                            }
                            this->pixels[x + column][y + row] ^= 1; //Set pixel with xor (active if different, off if same)
                        }
                    }
                }
            }
            this->drawFlag = true;
            this->pc += 2;
            
            break;

        case 0xF000:
            switch(this->opcode & 0x00FF) {
                case 0x0029: //FX29 Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
                    {
                        unsigned short x = ((this->opcode & 0x0F00) >> 8);
                        cout << "Font " << this->V[x] << endl;
                        this->I = this->memory[this->V[x]];
                    }
                    this->pc += 2;
                    break;
                case 0x0033: 
                    //Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, 
                    //the middle digit at I plus 1, and the least significant digit at I plus 2. 
                    //(In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, 
                    //the tens digit at location I+1, and the ones digit at location I+2.)

                    
                    break;

                case 0x0065: 
                    //Fills V0 to VX (including VX) with values from memory starting at address I. 
                    //The offset from I is increased by 1 for each value written, but I itself is left unmodified.
                    {// Scope for variables
                        unsigned short x = ((this->opcode & 0x0F00) >> 8); //Last Register to fill
                        for(int i = 0; i <= x; i++) {
                            this->V[i] = this->memory[this->I + i];
                        }
                        this->pc += 2;
                    }
                    break;

                default:
                    cerr << "Unknown opcode: " << hex << uppercase << this->opcode << dec << nouppercase << endl; 
                    return false;
                    break;
            }
            break;

        default:
            cerr << "Unknown opcode: " << hex << uppercase << this->opcode << dec << nouppercase << endl; 
            return false;
            break;
    }*/