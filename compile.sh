#!/bin/sh
gcc -O2 -Wall src/*.c src/*/*.c src/*/*/*.c -lSDL3_image -lSDL3
