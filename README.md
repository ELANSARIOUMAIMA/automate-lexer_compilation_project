# automate-lexer_compilation_project

## Description
This project implements a **lexical analyzer (lexer)** for a simple finite automaton definition language.  
The lexer reads a text file describing an automaton, including its **states, alphabet, initial and final states, and transitions**, and then recognizes the corresponding tokens.

---

## Features

* Recognizes **keywords**: `etats`, `alphabet`, `initial`, `final`, `transitions`.
* Recognizes **identifiers** for states and symbols.
* Handles **special symbols**: `=`, `,`, `;`, `{`, `}`, `->`, `:`.
* Skips whitespaces and optional comments (`//`).
* Outputs a **list of lexemes and their token types**.

---

## Files Included

* `lexer.h` — Header file with token definitions and function declarations.
* `lexer.c` — Implementation of the lexer functions.
* `main_lexer.c` — Main program to test the lexer on a file.
* `automate.txt` — Sample input file describing an automaton.

---

## Usage

1. Compile the project with GCC:

```bash
gcc lexer.c main_lexer.c -o main_lexer
```

2. Run the program:

```bash
./main_lexer
```
