# Hack Assembler

## Overview

This Hack Assembler translates Hack assembly language programs into machine code as specified in the *Nand2Tetris* course. The assembler supports the Hack instruction set, including both A-type and C-type instructions.

## Files

- **main.c**: The driver program for the assembler.
- **parser.c / parser.h**: Functions for parsing the input Hack assembly file.
- **symtable.c / symtable.h**: Implementation of the symbol table for resolving labels and variables.
- **hack.h**: Definitions and constants for the Hack machine language.
- **error.c / error.h**: Error handling functions for the assembler.

## How to Build

To build the assembler, use the following command:

```bash
gcc -o assembler main.c parser.c symtable.c error.c
