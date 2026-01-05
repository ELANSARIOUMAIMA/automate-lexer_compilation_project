# automate-lexer_compilation_project

## Description
This project implements a compiler for a finite automaton description language.
The compiler is composed of three main phases:
**Lexical Analysis** – tokenizing the input text,
**Syntax Analysis** – validating the grammar and structure,
**Semantic Analysis** – verifying the logical correctness of the automaton.

The goal of the project is to ensure that an automaton described by the user is syntactically correct, semantically valid, and formally consistent.

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

### Semantic Analyzer

The semantic analyzer ensures the logical and formal correctness of the automaton after parsing.

**Responsibilities:**
* Verify that the initial state exists
* Verify that all final states are defined
* Ensure transitions use existing states
* Ensure transition symbols belong to the alphabet
* Detect non-deterministic automata
  
**Semantic Rules Checked:**
* The initial state must belong to the set of states
* Final states must be defined states
* All transition symbols must be declared in the alphabet
* Each transition must link valid states
* A warning is generated in case of non-determinism
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
* `semantic.h` — Header file declaring semantic analysis functions for validating the automaton.
* `semantic.c` — Implementation of the semantic analyzer with logical consistency checks.
* `main_parser.c` — Main program to test the complete parser

---

## Usage

### Compile the Lexer Only
```bash
gcc lexer.c main_lexer.c -o main_lexer
./main_lexer
```

### Compile Full Compiler (Lexer + Parser + Semantic Analyzer)
1. Compile all files:
```bash
gcc -c lexer.c -o lexer.o
gcc -c automate.c -o automate.o
gcc -c parser.c -o parser.o
gcc -c semantic.c -o semantic.o
gcc -c main_parser.c -o main_parser.o
```

2. Link all objects:
```bash
gcc lexer.o automate.o parser.o semantic.o main_parser.o -o main_parser
```

3. Run:
```bash
./main_parser
```
## Output
* Valid automata are accepted and analyzed successfully
* Syntax errors stop compilation with clear messages
* Semantic errors are detected and reported
* Non-deterministic automata generate warnings
