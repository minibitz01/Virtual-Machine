# Virtual Machine & PL/0 Compiler Components

Implementation of a PM/0 stack-based virtual machine and lexical analyzer written in C.

## Overview

This project implements core components of a simple compiler system, including a lexical analyzer (scanner) and a PM/0 virtual machine.

The scanner processes source code and generates tokens, which can then be used by later stages of a compiler. The PM/0 virtual machine executes instructions using a stack-based architecture.

## Features

- Lexical analyzer for tokenizing source code
- Identification of:
  - identifiers
  - numbers
  - reserved words
  - special symbols
- PM/0 stack-based virtual machine
- Execution of low-level instructions using a stack model

## Components

### Scanner (Lexical Analyzer)
The scanner reads source code and converts it into tokens that represent identifiers, numbers, reserved words, and symbols.

### PM/0 Virtual Machine
The PM/0 virtual machine executes generated instructions using a stack-based architecture.

## Language

C

## Sample Input
```
7 0 45
7 0 6
6 0 4
1 0 4
1 0 3
2 0 4
4 1 4
1 0 14
3 1 4
2 0 8
8 0 39
1 0 7
7 0 42
1 0 5
2 0 0
6 0 5
9 0 2
5 0 3
9 0 1
9 0 3

## Sample Output

```
L   M   PC   BP   SP   stack
Initial values:      0   480   481

JMP   0   45   45   480   481
INC   0   5    48   480   476   0 0 0 0 0
Please Enter an Integer: 8
SYS   0   2    51   480   475   0 0 0 0 0 8
CAL   0   3    3    474   475   0 0 0 0 0 8
JMP   0   6    6    474   475   0 0 0 0 0 8
INC   0   4    9    474   471   0 0 0 0 0 8 | 480 480 54 0
LIT   0   4    12   474   470   0 0 0 0 0 8 | 480 480 54 0 4
LIT   0   3    15   474   469   0 0 0 0 0 8 | 480 480 54 0 4 3
MUL   0   4    18   474   470   0 0 0 0 0 8 | 480 480 54 0 12
STO   1   4    21   474   471   0 0 0 0 12 8 | 480 480 54 0
LIT   0   14   24   474   470   0 0 0 0 12 8 | 480 480 54 0 14
LOD   1   4    27   474   469   0 0 0 0 12 8 | 480 480 54 0 14 12
LSS   0   8    30   474   470   0 0 0 0 12 8 | 480 480 54 0 0
JPC   0   39   39   474   471   0 0 0 0 12 8 | 480 480 54 0
LIT   0   5    42   474   470   0 0 0 0 12 8 | 480 480 54 0 5
RTN   0   0    54   480   475   0 0 0 0 12 8

Output result is: 8
SYS   0   1    57   480   476   0 0 0 0 12
SYS   0   3    60   480   476   0 0 0 0 12
```
