base = $(shell pwd)

derived = $(base)/derived

bisonFlags = -t --graph -v

bisonIn = parser/anvil.y
flexIn = parser/anvil.lex
parserIn = parser/Parser.cpp
parserOut = $(derived)/libAvlParser.so

bisonOut = $(derived)/parser.c
bisonOutHeader = $(derived)/parser.h
flexOut = $(derived)/scanner.h

flexBisonDefines = -DFLEX_FILE=\"$(flexOut)\" -DBISON_FILE=\"$(bisonOutHeader)\" 

executable = anvil

includes = -I$(base) -I$(base)/parser -I$(base)/ast

links = -L$(derived) -lAvlParser


CXX=LD_LIBRARY_PATH=LD_LIBRARY_PATH:$(derived) g++

all: bison parser main.cpp 
	$(CXX) -o anvil main.cpp $(includes) $(links) 

bison:
	mkdir -p derived
	bison -d $(bisonIn) -o $(bisonOut) $(bisonFlags)
	flex --outfile=$(flexOut) $(flexIn)

parser: bison 
	g++ -shared -o $(parserOut) $(parserIn) $(bisonOut) $(flexBisonDefines) $(includes) 

clean:
	@rm -r $(derived)
	@rm $(executable)

run: default
	./anvil test.avl	

dot: default
	dot -o /mnt/raid/anvil.svg derived/parser.dot -Tsvg
