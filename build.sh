#!/bin/sh

set -e

echo "Building Project..."

if (g++ src/*.cpp -o flappy -I ./include -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio); then
  echo "Compiled successfully. Use ./flappy to run."
else
  echo "Error occured while compiling. Exiting."
fi
