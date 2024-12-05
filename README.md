# 2Pass Assembler Emulator

## Overview
This project consists of two main components:
- An assembler (`asm.cpp`) that converts assembly language code to machine code
- An emulator (`emu.cpp`) that executes the generated machine code
  
## System Architecture
* **Registers**: A, B (internal stack), PC (Program Counter), SP (Stack Pointer)
* **Register Size**: 32-bit
* **Instruction Encoding**: 8-bit opcode, 24-bit operand

## Features
- Assembler
  - Supports various assembly mnemonics
  - First and second pass compilation
  - Error and warning logging
  - Generates object code and listing files
  
  ![2_PASS_ASSEMBLER_FLOW](https://github.com/user-attachments/assets/3491bf75-6282-4bc4-98e4-844d28ed33c6)

- Emulator
  - Executes machine code hex instructions
  - Tracks program counter, stack pointer, and registers
  - Supports 18 different instruction types
  - Provides detailed execution trace and memory state
  ![EMULATOR_FLOW](https://github.com/user-attachments/assets/19a1edc8-f46c-4201-be4f-0cc2feba5e64)

## Supported Instructions
- Data manipulation: `ldc`, `adc`, `ldl`, `stl`, `ldnl`, `stnl`
- Arithmetic: `add`, `sub`, `shl`, `shr`
- Stack operations: `adj`, `a2sp`, `sp2a`
- Control flow: `call`, `return`, `brz`, `brlz`, `br`
- Halt instruction: `HALT`
- (For more details, see instructions_0.png and instructions_1.png)

## Usage

### Assembler
1. Compile the assembler: `g++ asm.cpp -o asm`
2. Run the assembler: `./asm`
3. Enter the input assembly file name when prompted

### Emulator
1. Compile the emulator: `g++ emu.cpp -o emu`
2. Run the emulator: `./emu`

## Output Files
- `list1.txt`: First pass listing
- `list2.txt`: Second pass listing with machine code
- `obj.o`: Object code file
- `error.log`: Compilation errors and warnings
- `emu_log.txt`: Execution trace log

## Images Description
### 1. File containing compilation errors and warnings log

![Screenshot 2024-12-04 222736](https://github.com/user-attachments/assets/a3342e05-5683-4ea5-9c5f-9522408fe5e7)

### 2. Bubble Sort algorithm execution trace

![Screenshot 2024-12-04 223419](https://github.com/user-attachments/assets/056a70aa-d091-4b94-b011-3fea9f9fbf7a)
![Screenshot 2024-12-04 223439](https://github.com/user-attachments/assets/0c7a086b-91fe-4d8e-a99d-259e5cabca03)

## Requirements
- C++ compiler with C++11 support
- Standard C++ libraries

## Author
- Aditya Sharma
- Department of Computer Science and Engineering
- IIT Patna

## License
Academic project - For educational purposes
