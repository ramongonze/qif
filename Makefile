all: clean main

clean:
	rm -f *.o
	rm -f *.out

main: structs.o functions.o
	g++ -std=c++11 main.cpp structs.o functions.o -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

structs.o:
	g++ -std=c++11 -c structs.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

functions.o:
	g++ -std=c++11 -c functions.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic
