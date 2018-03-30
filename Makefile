MUDUO_DIRECTORY ?= $(HOME)/build/release-cpp11
#MUDUO_DIRECTORY ?= $(HOME)/build/install
MUDUO_INCLUDE = $(HOME)/muduo
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
SOURCE = $(wildcard *.cc)

CXXFLAGS = -g -std=c++11 -Wall -Wextra -Werror \
		   -Wno-unused-parameter  \
		   -Wold-style-cast -Woverloaded-virtual \
		   -Wpointer-arith -Wshadow -Wwrite-strings \
		   -march=native -rdynamic \
		   -I$(MUDUO_INCLUDE)

LDFLAGS = -L$(MUDUO_LIBRARY) -L/usr/local/lib -lmuduo_net -lmuduo_base -lpthread -lhiredis -lrt

all: spell
clean:
	rm -f spell core

spell: $(SOURCE)
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean

