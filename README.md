# Chip8
Chip8 Emulator/Interpreter written in Cpp

Still in the making, not working properly


## Instructions:

### Standard Chip-8 Instructions
 Hex | Mnemonic | Status  
 --- | --- | ---
 0x0NNN | SYS  | :x:               
 0x00E0 | CLS  | :heavy_check_mark:
 0x00EE | RET  | :heavy_check_mark: 
 0x1NNN | JP   | :x:
 0x2NNN | CALL | :x:
 0x3XNN | SE   | :x:
 0x4XNN | SNE  | :x:
 0x5XY0 | SE   | :x:
 0x6XNN | LD   | :x:
 0x7XNN | ADD  | :x:
 0x8XY0 | LD   | :x:
 0x8XY1 | OR   | :x:
 0x8XY2 | AND  | :x:
 0x8XY3 | XOR  | :x:
 0x8XY4 | ADD  | :x:
 0x8XY5 | SUB  | :x:
 0x8XY6 | SHR  | :x:
 0x8XY7 | SUBN | :x:
 0x8XYE | SHL  | :x:
 0x9XY0 | SNE  | :x:
 0xANNN | LD   | :x:
 0xBNNN | JP   | :x:
 0xCXNN | RND  | :x:
 0xDXYN | DRW  | :x:
 0xEX9E | SKP  | :x:
 0xEXA1 | SKNP | :x:
 0xFX07 | LD   | :x:
 0xFX0A | LD   | :x:
 0xFX15 | LD   | :x:
 0xFX18 | LD   | :x:
 0xFX1E | ADD  | :x:
 0xFX29 | LD   | :x:
 0xFX33 | LD   | :x:
 0xFX55 | LD   | :x:
 0xFX65 | LD   | :x:

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
