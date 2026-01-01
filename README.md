# automate-lexer_compilation_project

## Description
This project implements a **lexical analyzer (lexer)** and **syntax parser** for a simple finite automaton definition language.  
The lexer reads a text file describing an automaton, including its **states, alphabet, initial and final states, and transitions**, and recognizes the corresponding tokens.
The parser validates the syntax according to grammar rules and builds the automaton data structure in memory.

---

## Features

### Lexer Features
* Recognizes **keywords**: `etats`, `alphabet`, `initial`, `final`, `transitions`.
* Recognizes **identifiers** for states and symbols.
* Handles **special symbols**: `=`, `,`, `;`, `{`, `}`, `->`, `:`.
* Skips whitespaces and optional comments (`//`).
* Outputs a **list of lexemes and their token types**.

### Parser Features
* Validates syntax rules: correct order and format of automaton sections
* Detects missing sections: states, alphabet, initial, final, transitions
* Parses state definitions: `etats = q0,q1,q2;`
* Parses alphabet definitions: `alphabet = a,b;`
* Parses initial state: `initial = q0;`
* Parses final states: `final = q2;`
* Parses transitions: `q0 -> q1 : a;`
* Provides clear error messages for syntax violations

---

## Files Included

* `automate.txt` — Sample input file describing an automaton.
* `lexer.h` — Header file with token definitions and function declarations.
* `lexer.c` — Implementation of the lexer functions.
* `main_lexer.c` — Main program to test the lexer on a file.
* `parser.h` — Parser function declarations and structures
* `parser.c` — Syntax parser implementation with error detection
* `automate.h` — Automaton data structure definitions
* `automate.c` — Automaton creation and manipulation functions
* `main_parser.c` — Main program to test the complete parser

---

## Usage

### Compile the Lexer Only
```bash
gcc lexer.c main_lexer.c -o main_lexer
./main_lexer
```

### Compile the Complete Parser
1. Compile all files:
```bash
gcc -c lexer.c -o lexer.o
gcc -c automate.c -o automate.o
gcc -c parser.c -o parser.o
gcc -c main_parser.c -o main_parser.o
```

2. Link all objects:
```bash
gcc lexer.o automate.o parser.o main_parser.o -o main_parser
```

3. Run:
```bash
./main_parser
```
