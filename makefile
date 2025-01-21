compile: src/main.cpp
	g++ -c src/main.cpp -std=c++20 -I ./include/ 
	g++ main.o -o flappy -std=c++20 -I ./include/ -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio

run:
	./flappy
