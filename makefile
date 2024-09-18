compile: src/main.cpp
	g++ -c src/main.cpp -I ./include/
	g++ main.o -o flappy -I ./include/ -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio

run:
	./flappy
