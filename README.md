# 2Pass Assembler and Emulator

## Project Overview
A two-pass assembler and emulator for a custom SIMPLE instruction set, implementing a minimal assembly language with a specialized machine architecture.

## System Architecture
- **Registers**: A, B (internal stack), PC (Program Counter), SP (Stack Pointer)
- **Register Size**: 32-bit
- **Instruction Encoding**: 8-bit opcode, 24-bit operand

## Key Features
- Two-pass assembler with error diagnosis
- Object file and listing file generation
- Custom instruction set implementation
- Complete machine emulation

## Supported Instructions
- Arithmetic: `add`, `sub`, `adc`
- Load/Store: `ldc`, `ldl`, `stl`, `ldnl`, `stnl`
- Stack Management: `adj`, `a2sp`, `sp2a`
- Control Flow: `call`, `return`, `brz`, `brlz`, `br`
- Bitwise: `shl`, `shr`

## Development
- **Language**: ISO C89
- **Compilation**: `gcc asm.c -o asm` and `gcc emu.c -o emu`

## Project Components
- Assembler source code
- Emulator source code
- Test assembly files
- Output logs and listing files

## Additional Implementations
- Bubble sort program
- Multiple algorithmic test cases

## Development Standard
Adherence to strict compilation standards with pedantic error checking.
