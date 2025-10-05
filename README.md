# Custom Shell in C

A simple custom shell implementation in C that supports basic commands, built-in functions, and piping/redirects.

## Table of Contents

- [Overview](#overview)  
- [Features](#features)  
- [Dependencies](#dependencies)  
- [Building / Compilation](#building--compilation)  
- [Usage](#usage)  
- [Supported Commands & Behavior](#supported-commands--behavior)  
- [Limitations / TODOs](#limitations--todos)  

## Overview

This project implements a minimal Unix-style shell in C. The shell reads user commands, parses arguments, handles built-ins, supports external program execution, input/output redirection, and simple piping between commands.

The goal is to learn how shells work internally (parsing, `fork`, `exec`, file descriptors, etc.).

## Features

- Command parsing (splitting input into command + arguments)  
- Execution of external programs (via `fork` + `execvp`)  
- Built-in commands like `cd`, `exit`  
- Input (`<`) and output (`>`, `>>`) redirection  
- Simple pipes (one pipe between two commands)  
- Error handling (e.g. command not found, file open errors)  

(If your code supports more features like job control, background execution, signal handling, expand them here.)

## Dependencies

- A C compiler (gcc, clang)  
- Unix / POSIX-compatible environment (Linux, macOS)  
- Standard C library  

## Building / Compilation

Assuming you have GCC installed, you can compile like this:
bash
gcc -o viveks_shell main.c
1.After compilation run
./viveks_shell
2.You will see a prompt (for example, > or shell> ). Enter commands like you would in bash, e.g.:
> ls -l
> grep “main” main.c
> cat file.txt > out.txt
> cat file1.txt | grep hello
> cd src
> exit
3.The shell will parse your input, execute built-ins or external programs, handle redirections or pipes, and then return to the prompt.

#Limitations / TODOs

-> Only supports one pipe (i.e. chaining two commands), not multiple pipes

-> No job control (background &, signal handling)

-> No support for command chaining (&&, ;, ||)

-> No wildcard / glob expansion (*)

-> Minimal quoting / escaping support

-> Limited error handling and input validation

#Feel free to extend these in future:

-> Support multiple pipes

-> Background/foreground job control

-> Command chaining and logical operators

-> Environment variable expansion

-> Signal handling (Ctrl+C, Ctrl+Z)

-> History / line editing
