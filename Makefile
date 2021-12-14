PROJECT_NAME=qif
PLATFORM=PLATFORM_DESKTOP
CC=g++ # Compiler for desktop version
EXT=.o
AR=ar

# Define all source files required
CPP_SOURCE=$(wildcard src/*.cpp)
H_SOURCE=$(wildcard src/*.h)

# Take de list with all .cpp files and rename the extension to .o, generating a new list
OBJS = $(patsubst %.cpp, %$(EXT), $(CPP_SOURCE))

CFLAGS=-c -std=c++11 -Wall -Wextra -Werror -pedantic
ifeq ($(PLATFORM),PLATFORM_WEB)
    CC=em++ # Libraries for web (HTML5) compiling
	EXT=.so
	AR=emar
else
	CFLAGS += -O4
endif

all: $(PROJECT_NAME) bin

$(PROJECT_NAME): $(OBJS)

%$(EXT): %.cpp %.h
	$(CC) -o $@ $< $(CFLAGS)

bin: $(PROJECT_NAME)
	mkdir -p ./bin
	mv src/*$(EXT) ./bin
	$(AR) rcs qif.a bin/*$(EXT)

clean:
	rm -rf ./bin
	rm -f ./qif.a