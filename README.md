# Infix to Postfix Calculator in C

This project implements a command-line calculator in C that:

- Converts mathematical infix expressions to postfix (Reverse Polish Notation).
- Evaluates postfix expressions using a custom stack implementation.
- Supports:
  - Floating-point numbers (e.g., `3.14`, `-2.7`)
  - Operators: `+`, `-`, `*`, `/`, `^`
  - Parentheses and operator precedence
  - Error handling with colored console messages

## Features

- Robust stack implementation using linked list
- Handles division by zero and malformed expressions
- Supports continuous input loop until 'q' is entered

## How to Use

1. Compile the code:
   ```bash
   gcc postfix.c -o calculator -lm
