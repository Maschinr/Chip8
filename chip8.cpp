#include "Chip8.hpp"

#include <fstream>
#include <vector>
#include <iterator>
#include <imgui/imgui.h>

using namespace std;
using namespace sf;

Chip8::Chip8() {
    this->memoryViewerOpened = false;
    this->stackViewerOpened = false;
    this->registerViewerOpened = false;
    this->hardReset();
}

Chip8::~Chip8() {

}

void Chip8::hardReset() {
    this->softReset();
    this->romLoaded = false;
    memset(this->memory, 0, 4096);
    memcpy(&this->memory[0], &this->fontset[0], 80);

}

void Chip8::softReset() {
    this->pc = 0x200;
    this->opcode = 0;
    this->I = 0;
    this->sp = 0;
    this->drawFlag = false;
    this->dtimer = 0;
    this->stimer = 0;
    this->lastFrameKey = -1;
    memset(this->pixels, 0, 64 * 32);
    memset(this->keys, 0, sizeof(bool) * 16);
    memset(this->V, 0, 16);
}

void Chip8::drawMenuBarGUI() {
    if (ImGui::BeginMenu("Chip8")) {
        if(ImGui::MenuItem("Memory", /*TODO shortcut*/"")) {
           this->memoryViewerOpened = !this->memoryViewerOpened;
        }
        if(ImGui::MenuItem("Stack", /*TODO shortcut*/"")) {
            this->stackViewerOpened = !this->stackViewerOpened;
        }
        if(ImGui::MenuItem("Register", /*TODO shortcut*/"")) {
            this->registerViewerOpened = !this->registerViewerOpened;
        }
        ImGui::EndMenu();
    }
}

void Chip8::drawMiscGUI() {
    if(this->registerViewerOpened) {
        ImGui::Begin("Chip 8 Register's", &this->registerViewerOpened); 
        ImGui::BeginChild("Registers");
        for (int i = 0; i < 16; i++)
            ImGui::Text("%02d: %d", i + 1, this->V[i]);
        ImGui::EndChild();
        ImGui::End();
    }
}

bool Chip8::loadFile(string path) {
    ifstream rom(path, ios::binary);
    this->hardReset();
    if(rom.is_open()) {
        //TODO change -> Second parameter of istreambuf_iterator describes that no traits should be used
        vector<unsigned char> buffer(istreambuf_iterator<char>(rom), {});
        rom.close();

        // TODO cpp way
        for(size_t i = 0; i < buffer.size(); ++i) {
            this->memory[i + 512] = buffer[i];
        }

        this->romLoaded = true;
        return true;
    } else {
        return false;
    }
}

void Chip8::cycle() {
    if(this->romLoaded == true) {
        this->input();

        //Fetch opcode, current position + cur position + 1 because opcode is 16 bits
        this->opcode = this->memory[this->pc] << 8 | this->memory[this->pc + 1];

        //cout << "Performing opcode: " << hex << uppercase << this->opcode << dec << nouppercase << endl; 
        unsigned short X = (this->opcode & 0x0F00) >> 8;
        unsigned short Y = (this->opcode & 0x00F0) >> 4;
        unsigned short NNN = this->opcode & 0x0FFF;
        unsigned short NN = this->opcode & 0x00FF;
        unsigned short N = this->opcode & 0x000F;

        switch((this->opcode & 0xF000)) {
            case 0x0000:
                switch(this->opcode & 0x00FF) {
                    case 0x00E0:
                        this->clearDisplay();
                        break;

                    case 0x00EE:
                        this->returnSubRoutine();
                        break;

                    default:
                        cerr << "Not implemented " << hex << uppercase << this->opcode << dec << nouppercase;
                        break;
                }
                break;

            case 0x1000:
                this->jumpAddress(NNN);
                break;

            case 0x2000:
                this->callAddress(NNN);
                break;

            case 0x3000:
                this->skipEqual(X, NN);
                break;

            case 0x4000:
                this->skipNEqual(X, NN);
                break;

            case 0x5000:
                this->skipEqualRegister(X, Y);
                break;

            case 0x6000:
                this->set(X, NN);
                break;

            case 0x7000:
                this->addNC(X, NN);
                break;

            case 0x8000:
                switch(this->opcode & 0x000F) {
                    case 0x0000:
                        this->assignRegister(X, Y);
                        break;
                    
                    case 0x0001:
                        this->assignOrRegister(X, Y);
                        break;

                    case 0x0002:
                        this->assignAndRegister(X, Y);
                        break;

                    case 0x0003:
                        this->assignXorRegister(X, Y);
                        break;

                    case 0x0004:
                        this->add(X, Y);
                        break;

                    case 0x0005:
                        this->subY(X, Y);
                        break;

                    case 0x0006:
                        this->shiftRight(X);
                        break;

                    case 0x0007:
                        this->subX(X, Y);
                        break;

                    case 0x000E:
                        this->shiftLeft(X);
                        break;

                    default: 
                        cerr << "Not implemented " << hex << uppercase << this->opcode << dec << nouppercase;
                        break;
                }
                break;

            case 0x9000:
                this->skipNEqualRegister(X, Y);
                break;

            case 0xA000:
                this->setIndex(NNN);
                break;

            case 0xB000:
                this->jumpRegister(NNN);
                break;

            case 0xC000:
                this->random(X, NN);
                break;

            case 0xD000:
                this->draw(X, Y, N);
                break;

            case 0xE000:
                switch(this->opcode & 0x00FF) {
                    case 0x009E:
                        this->keyPressed(X);
                        break;

                    case 0x00A1:
                        this->keyNPressed(X);
                        break;

                    default:
                        cerr << "Not implemented " << hex << uppercase << this->opcode << dec << nouppercase;
                        break;
                }
                break;

            case 0xF000:
                switch(this->opcode & 0x00FF) {
                    case 0x0007:
                        this->getDelayTimer(X);
                        break;

                    case 0x000A:
                        this->awaitKeyPress(X);
                        break;

                    case 0x0015:
                        this->setDelayTimer(X);
                        break;

                    case 0x0018:
                        this->setSoundTimer(X);
                        break;

                    case 0x001E:
                        this->addToIndex(X);
                        break;

                    case 0x0029:
                        this->setIndexToChar(X);
                        break;

                    case 0x0033:
                        this->bcdTransform(X);
                        break;

                    case 0x0055:
                        this->saveRegister(X);
                        break;

                    case 0x0065:
                        this->loadRegister(X);
                        break;
                }
                break;

            default:
                cerr << "Not implemented " << hex << uppercase << this->opcode << dec << nouppercase;
                break;
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
    }
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