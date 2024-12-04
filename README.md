# Simple Assembly Emulator

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

- Emulator
  - Executes machine code hex instructions
  - Tracks program counter, stack pointer, and registers
  - Supports 18 different instruction types
  - Provides detailed execution trace and memory state

## Supported Instructions
- Data manipulation: `ldc`, `adc`, `ldl`, `stl`, `ldnl`, `stnl`
- Arithmetic: `add`, `sub`, `shl`, `shr`
- Stack operations: `adj`, `a2sp`, `sp2a`
- Control flow: `call`, `return`, `brz`, `brlz`, `br`
- Halt instruction: `HALT`

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

## Requirements
- C++ compiler with C++11 support
- Standard C++ libraries

## Author
Aditya Sharma (Roll No: 2301AI01)
Department of Computer Science and Engineering
IIT Patna

## License
Academic project - For educational purposes
