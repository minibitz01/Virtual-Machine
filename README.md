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
