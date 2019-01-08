all: clean distribution.o channels.o gain.o hyper.o

clean:
	rm -f *.o
	rm -f *.out

distribution.o:
	g++ -std=c++11 -c ./structs/distribution.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

channels.o:
	g++ -std=c++11 -c ./structs/channels.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

gain.o:
	g++ -std=c++11 -c ./structs/gain.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

hyper.o:
	g++ -std=c++11 -c ./structs/hyper.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

# functions.o:
# 	g++ -std=c++11 -c functions.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic
