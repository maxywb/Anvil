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

visitorIn = visitor/TreeWalker.cpp
visitorOut = $(derived)/libAvlVisitor.so

irOut = $(derived)/libAvlIr.so

executable = anvil

includes = -I/usr/include -I$(base) -I$(base)/parser -I$(base)/ast -I$(base)/visitor -I$(base)/ir 

links = -L$(derived) -lAvlParser -lAvlVisitor -lAvlIr 

CXX_FLAGS = -g -std=c++0x

CXX=LD_LIBRARY_PATH=LD_LIBRARY_PATH:$(derived) g++

all: bison parser visitor ir main.cpp 
	$(CXX) $(CXX_FLAGS) -o anvil main.cpp $(includes) $(links) 

bison: 
	mkdir -p derived
	bison -d $(bisonIn) -o $(bisonOut) $(bisonFlags)
	flex --outfile=$(flexOut) $(flexIn)

parser: bison $(parserIn)
	g++ $(CXX_FLAGS) -shared -fPIC -o $(parserOut) $(parserIn) $(bisonOut) $(flexBisonDefines) $(includes) 

visitor: bison
	g++ $(CXX_FLAGS) -shared -fPIC -o $(visitorOut) $(visitorIn) $(includes) 

ir: bison
	g++ $(CXX_FLAGS) -shared -fPIC -o  $(irOut) ir/Term.cpp

clean:
	@rm -r $(derived)
	@rm $(executable)

run: all
	./anvil test.avl	

