all: qif.o distribution.o channel.o gain.o hyper.o vulnerability.o entropy.o leakage.o move

qif.o:
	g++ -std=c++11 -c qif.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

distribution.o:
	g++ -std=c++11 -c ./src/distribution.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

channel.o:
	g++ -std=c++11 -c ./src/channel.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

gain.o:
	g++ -std=c++11 -c ./src/gain.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

hyper.o:
	g++ -std=c++11 -c ./src/hyper.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

vulnerability.o:
	g++ -std=c++11 -c ./src/vulnerability.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

entropy.o:
	g++ -std=c++11 -c ./src/entropy.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

leakage.o:
	g++ -std=c++11 -c ./src/leakage.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -pedantic

move:
	mv *.o ./bin/
	mv ./bin/qif.o ./