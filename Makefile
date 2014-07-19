derived = `pwd`/derived

bisonIn = parser.y
flexIn = scanner.lex

bisonOut = $(derived)/parser.c
bisonOutHeader = $(derived)/parser.h
flexOut = $(derived)/scanner.h

flexBisonDefines = -DFLEX_FILE=\"$(flexOut)\" -DBISON_FILE=\"$(bisonOutHeader)\"

executable = testing

default:

	mkdir -p derived
	bison -d $(bisonIn) -o $(bisonOut)
	flex --outfile=$(flexOut) $(flexIn)

	g++ -o $(executable) main.cpp $(bisonOut) $(flexBisonDefines)

clean:
	@rm -r $(derived)
	@rm $(executable)
