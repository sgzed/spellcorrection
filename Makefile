MUDUO_DIRECTORY ?= $(HOME)/build/release-install
#MUDUO_DIRECTORY ?= $(HOME)/build/install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
SOURCE = $(wildcard *.cc)

CXXFLAGS = -g -O0 -Wall -Wextra -Werror \
		   -Wconversion -Wno-unused-parameter \
		   -Wold-style-cast -Woverloaded-virtual \
		   -Wpointer-arith -Wshadow -Wwrite-strings \
		   -march=native -rdynamic \
		   -I$(MUDUO_INCLUDE)

LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_net -lmuduo_base -lpthread -lrt

all: spell
clean:
	rm -f spell core

spell: $(SOURCE)
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean

