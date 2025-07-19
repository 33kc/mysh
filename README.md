# mysh  – MY SHell in C

`mysh` is a personal project i'm building to replicate the basic behavior of unix shells, <br> 
this isn't production-ready, this is purely for learning purposes.

---

##  Features

- basic command execution using `fork()` and `execvp()`
- built-in commands:
  - `cd` (supports `cd ~` and `cd -`)
  - `clear`
  - `exit`
- readline-based input with command history
- prompt displays the current working directory

---

##  Build Instructions

make sure you have `gcc` and `readline` development libraries installed

### Install dependencies (Debian/Ubuntu):

    sudo apt update
    sudo apt install build-essential libreadline-dev

### Compile:

    make

### Clean build files:

    make clean

---

##  Run the Shell

    ./mysh

---



