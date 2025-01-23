all: main

main: src/main.cpp
	g++ -o build/main.o src/main.cpp

clean: 
	-rm build/main.o