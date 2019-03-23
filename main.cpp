#include "Chip8.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <imgui-filedialogue/ImGuiFileDialog.h>

using namespace std;
using namespace sf;

//TODO imgui gui where things like clockspeed rom and so on can be changed Titlebar and playarea is height - titlebar?
//TODO save imgui preferences to file
//TODO Change function map to many inline class member functions
//TODO implement all missing opcodes
//TODO pixels are one off?
//TODO some opcodes are wrong, build debugger with imgui
//TODO license stuff
//TODO performance

//UI Stuff todo
//Clock speed ? wrong or performance issue?
//Choose active emulator
//Menubar item for active emulator -> assembler, memory viewer and everything the emulator can do
int main(int argc, char* argv[]) {

    RenderWindow window(VideoMode(1280, 720), "Chip8");
    ImGui::SFML::Init(window);

    unique_ptr<Chip8> emulated = make_unique<Chip8>();

    Clock cycleClock;
    Clock deltaClock;

    bool generalMenuOpened = false;
    bool loadRomMenuOpened = false;
    float bgColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    int clockSpeed = 500;
    int menuBarHeight = 0;

    while(window.isOpen()) {
        Event event;

        while(window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::Resized) {
                View view = View(FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y));
	            window.setView(view);
            }
        }

        ////// LOGIC
        if(cycleClock.getElapsedTime() > milliseconds(seconds(1).asMilliseconds() / clockSpeed)) {
            cycleClock.restart();
            emulated->cycle();
        }


        ////// DRAWING
        ImGui::SFML::Update(window, deltaClock.restart());

        //Build General UI
        //Menu Bar
        if (ImGui::BeginMainMenuBar()) {
            menuBarHeight = ImGui::GetWindowSize().y;
            if (ImGui::Button("General")) {
                //TODO no highlight
                generalMenuOpened = true;
            }

            if (ImGui::BeginMenu("Rom")) {
                if(ImGui::MenuItem("Load", /*TODO shortcut*/"")) {
                    loadRomMenuOpened = true;
                }
                if(ImGui::MenuItem("Close", /*TODO shortcut*/"")) {
                    emulated->reset();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();

            if (loadRomMenuOpened)
			{
                //TODO underhang menu
				if (ImGuiFileDialog::Instance()->FileDialog("Choose File"))
				{
					if (ImGuiFileDialog::Instance()->IsOk == true)
					{
						string filePathName = ImGuiFileDialog::Instance()->GetFilepathName();
                        cout << filePathName << endl;
                        if(!emulated->loadFile(filePathName)) {
                            cerr << "Error loading Rom" << endl;
                        }
					}
					loadRomMenuOpened = false;
				}
			}
        }
        ImGui::End();

        if(generalMenuOpened == true) {
            //TODO Fixed size, not moveable, directly under general button
            ImGui::Begin("General", &generalMenuOpened);
            ImGui::ColorEdit4("Color", bgColor); // TODO configure
            ImGui::SliderInt("Clock Speed", &clockSpeed, 1, 1000);
            //TODO Set Emulator
            ImGui::End();
        }

        window.clear(Color(bgColor[0] * 255, bgColor[1] * 255, bgColor[2] * 255, bgColor[3] * 255)); 

        //Draw playfield from emulator
        RenderTexture texture;
        emulated->draw(texture);
        
        Sprite emuGfx(texture.getTexture());
        emuGfx.setOrigin(emuGfx.getTexture()->getSize().x / 2, emuGfx.getTexture()->getSize().y  / 2);
        emuGfx.setPosition(window.getSize().x / 2, (window.getSize().y + menuBarHeight) / 2);

        //Todo emu get aspect ratio, and emu get resolution for math
        unsigned int w_width = window.getSize().x;
        w_width = ((int)(w_width / 64)) * 64;
        unsigned int w_height = window.getSize().y - menuBarHeight;
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

        //Draw General UI
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
