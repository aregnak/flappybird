#!/bin/sh

if (g++ src/main.cpp -o flappy -I ./include -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio) then
	echo "Compiled successfully. Use ./flappy to run."
else
	echo "Error occured while compiling. Exiting."
fi
