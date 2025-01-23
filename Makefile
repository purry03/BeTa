all: main

main: src/main.cpp
	g++ -std=c++20 -o build/main.o src/main.cpp

clean: 
	-rm build/main.o