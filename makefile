INCLUDE = -I./include
LIBRARY_PATH = -L./lib
LIBRARIES = -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lwinmm -lgdi32 -lopengl32

prog.exe: main.o chip8.o imgui.o imgui_draw.o imgui_widgets.o imgui-SFML.o ImGuiFileDialog.o
	g++ -o prog.exe main.o chip8.o imgui.o imgui_draw.o imgui_widgets.o imgui-SFML.o ImGuiFileDialog.o $(LIBRARY_PATH) $(LIBRARIES) 

main.o: main.cpp chip8.hpp
	g++ -c $(INCLUDE) main.cpp

chip8.o: chip8.cpp chip8.hpp
	g++ -c $(INCLUDE) chip8.cpp

imgui.o: ./include/imgui/imgui.cpp
	g++ -c $(INCLUDE) ./include/imgui/imgui.cpp

imgui_draw.o: ./include/imgui/imgui_draw.cpp
	g++ -c $(INCLUDE) ./include/imgui/imgui_draw.cpp

imgui_widgets.o: ./include/imgui/imgui_widgets.cpp
	g++ -c $(INCLUDE) ./include/imgui/imgui_widgets.cpp

imgui-SFML.o: ./include/imgui-sfml/imgui-SFML.cpp
	g++ -c $(INCLUDE) ./include/imgui-sfml/imgui-SFML.cpp

ImGuiFileDialog.o: ./include/imgui-filedialogue/ImGuiFileDialog.cpp
	g++ -c $(INCLUDE) ./include/imgui-filedialogue/ImGuiFileDialog.cpp

clear: 
	del *.o prog.exe