
CXX = g++
CCOPT = -g -O0
CXXFLAGS = $(CCOPT) -std=c++11 -Wall -Werror
LDFLAGS = -lpthread -lm -ldl -lrt

all: example
example: example.cpp CodeManager.hpp
	$(CXX) $(CXXFLAGS) example.cpp -o $@

clean:
	rm -rf *.o example


