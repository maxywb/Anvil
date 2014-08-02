base = $(shell pwd)

derived = $(base)/derived

bisonFlags = -DYYDEBUG=

bisonIn = parser.y
flexIn = scanner.lex

bisonOut = $(derived)/parser.c
bisonOutHeader = $(derived)/parser.h
flexOut = $(derived)/scanner.h

flexBisonDefines = -DFLEX_FILE=\"$(flexOut)\" -DBISON_FILE=\"$(bisonOutHeader)\" 

executable = anvil

includes = -I$(base) -I$(base)/Node

default:
	@echo $(base)
	mkdir -p derived
	bison -d $(bisonIn) -o $(bisonOut) 
	flex --outfile=$(flexOut) $(flexIn)

	g++ -o $(executable) main.cpp $(bisonOut) $(flexBisonDefines) $(includes)

clean:
	@rm -r $(derived)
	@rm $(executable)

run: default
	./anvil test.avl	
