INCLUDE = -I./include
LIBRARY_PATH = -L./lib
LIBRARIES = -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lwinmm -lgdi32 -lopengl32

prog.exe: main.o chip8.o
	g++ -o prog.exe main.o chip8.o $(LIBRARY_PATH) $(LIBRARIES) 

main.o: main.cpp chip8.hpp
	g++ -c $(INCLUDE) main.cpp

chip8.o: chip8.cpp chip8.hpp
	g++ -c $(INCLUDE) chip8.cpp

clear: 
	del *.o prog.exe