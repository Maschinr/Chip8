# Chip8
Chip8 Emulator/Interpreter written in Cpp

Still in the making, not working properly

Uses SFML (https://www.sfml-dev.org) for graphics and input 

## Instructions:

### Standard Chip-8 Instructions
 Hex | Mnemonic | Status  
 --- | --- | ---
 0x0NNN | SYS  | :x:               
 0x00E0 | CLS  | :heavy_check_mark:
 0x00EE | RET  | :heavy_check_mark: 
 0x1NNN | JP   | :heavy_check_mark:
 0x2NNN | CALL | :heavy_check_mark:
 0x3XNN | SE   | :heavy_check_mark:
 0x4XNN | SNE  | :heavy_check_mark:
 0x5XY0 | SE   | :heavy_check_mark:
 0x6XNN | LD   | :heavy_check_mark:
 0x7XNN | ADD  | :heavy_check_mark:
 0x8XY0 | LD   | :heavy_check_mark:
 0x8XY1 | OR   | :heavy_check_mark:
 0x8XY2 | AND  | :heavy_check_mark:
 0x8XY3 | XOR  | :heavy_check_mark:
 0x8XY4 | ADD  | :heavy_check_mark:
 0x8XY5 | SUB  | :heavy_check_mark:
 0x8XY6 | SHR  | :heavy_check_mark:
 0x8XY7 | SUBN | :heavy_check_mark:
 0x8XYE | SHL  | :heavy_check_mark:
 0x9XY0 | SNE  | :x:
 0xANNN | LD   | :heavy_check_mark:
 0xBNNN | JP   | :x:
 0xCXNN | RND  | :heavy_check_mark:
 0xDXYN | DRW  | :heavy_check_mark:
 0xEX9E | SKP  | :x:
 0xEXA1 | SKNP | :heavy_check_mark:
 0xFX07 | LD   | :heavy_check_mark:
 0xFX0A | LD   | :x:
 0xFX15 | LD   | :heavy_check_mark:
 0xFX18 | LD   | :heavy_check_mark:
 0xFX1E | ADD  | :heavy_check_mark:
 0xFX29 | LD   | :heavy_check_mark:
 0xFX33 | LD   | :heavy_check_mark:
 0xFX55 | LD   | :heavy_check_mark:
 0xFX65 | LD   | :heavy_check_mark:

### Super Chip-48 Instructions
   Hex  | Mnemonic | Status  
 ---    | ---  | ---
 0x00CN | SCD  | :x:
 0x00FB | SCR  | :x:
 0x00FC | SCL  | :x:
 0x00FD | EXIT | :x:
 0x00FE | LOW  | :x:
 0x00FF | HIGH | :x:
 0xDXY0 | DRW  | :x:
 0xFX30 | LD   | :x:
 0xFX75 | LD   | :x:
 0xFX85 | LD   | :x:
