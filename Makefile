all: clean main

clean:
	rm -f *.o
	rm -f *.out

main: structs.o functions.o
	g++ -std=c++11 main.cpp structs.o functions.o

structs.o:
	g++ -std=c++11 -c structs.cpp

functions.o:
	g++ -std=c++11 -c functions.cpp
