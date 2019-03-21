INCLUDE = ./include/SDL2
LIBRARY_PATH = -L./lib
LIBRARIES = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

prog.exe: main.o chip8.o
	g++ -o prog.exe main.o chip8.o $(LIBRARY_PATH) $(LIBRARIES)

main.o: main.cpp chip8.hpp
	g++ -c -I $(INCLUDE) main.cpp

chip8.o: chip8.cpp chip8.hpp
	g++ -c -I $(INCLUDE) chip8.cpp

clear: 
	del *.o prog.exe