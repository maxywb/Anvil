base = $(shell pwd)

derived = $(base)/derived

bisonFlags = -t -v

bisonIn = parser/anvil.y
flexIn = parser/anvil.lex
parserIn = parser/Parser.cpp
parserOut = $(derived)/libAvlParser.so

bisonOut = $(derived)/parser.c
bisonOutH = $(derived)/parser.h
bisonOuts = $(bisonOut) $(bisonOutH)
flexOut = $(derived)/scanner.h
flexBisonDefines = -DFLEX_FILE=\"$(flexOut)\" -DBISON_FILE=\"$(bisonOutH)\" 

visitorIn = visitor/TreeWalker.cpp
visitorOut = $(derived)/libAvlVisitor.so

executable = anvil

includes = -I/usr/include -I$(base) -I$(base)/parser -I$(base)/ast -I$(base)/visitor -I$(derived)

links = -L$(derived) -lAvlParser -lAvlVisitor -lpthread -lm -ldl 

llvmFlags = -I/mnt/raid/prog/src/llvm/include -I/mnt/raid/prog/src/llvm/include  -DNDEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -fomit-frame-pointer -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fno-rtti -fPIC -Woverloaded-virtual -ffunction-sections -fdata-sections -Wcast-qual -L/mnt/raid/prog/src/llvm/Release/lib -lLLVMLTO -lLLVMObjCARCOpts -lLLVMLinker -lLLVMipo -lLLVMVectorize -lLLVMBitWriter -lLLVMIRReader -lLLVMAsmParser -lLLVMR600CodeGen -lLLVMR600Desc -lLLVMR600Info -lLLVMR600AsmPrinter -lLLVMSystemZDisassembler -lLLVMSystemZCodeGen -lLLVMSystemZAsmParser -lLLVMSystemZDesc -lLLVMSystemZInfo -lLLVMSystemZAsmPrinter -lLLVMHexagonCodeGen -lLLVMHexagonAsmPrinter -lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMNVPTXAsmPrinter -lLLVMCppBackendCodeGen -lLLVMCppBackendInfo -lLLVMMSP430CodeGen -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMSP430AsmPrinter -lLLVMXCoreDisassembler -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreInfo -lLLVMXCoreAsmPrinter -lLLVMMipsDisassembler -lLLVMMipsCodeGen -lLLVMMipsAsmParser -lLLVMMipsDesc -lLLVMMipsInfo -lLLVMMipsAsmPrinter -lLLVMAArch64Disassembler -lLLVMAArch64CodeGen -lLLVMAArch64AsmParser -lLLVMAArch64Desc -lLLVMAArch64Info -lLLVMAArch64AsmPrinter -lLLVMAArch64Utils -lLLVMARMDisassembler -lLLVMARMCodeGen -lLLVMARMAsmParser -lLLVMARMDesc -lLLVMARMInfo -lLLVMARMAsmPrinter -lLLVMPowerPCDisassembler -lLLVMPowerPCCodeGen -lLLVMPowerPCAsmParser -lLLVMPowerPCDesc -lLLVMPowerPCInfo -lLLVMPowerPCAsmPrinter -lLLVMSparcDisassembler -lLLVMSparcCodeGen -lLLVMSparcAsmParser -lLLVMSparcDesc -lLLVMSparcInfo -lLLVMSparcAsmPrinter -lgtest_main -lgtest -lLLVMTableGen -lLLVMDebugInfo -lLLVMOption -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMJIT -lLLVMLineEditor -lLLVMMCAnalysis -lLLVMInstrumentation -lLLVMInterpreter -lLLVMCodeGen -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMProfileData -lLLVMMCJIT -lLLVMTarget -lLLVMRuntimeDyld -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMExecutionEngine -lLLVMMC -lLLVMCore -lLLVMSupport -lz -lpthread -ltinfo -lrt -ldl -lm 

CXX_FLAGS =  -std=c++11 -O3 

CXX=LD_LIBRARY_PATH=LD_LIBRARY_PATH:$(derived) g++

bad = -fpermissive

VPATH = derived 
all: main/main.cpp $(parserOut) $(visitorOut) $(bisonOut) setup
	@echo make all
	$(CXX) $(CXX_FLAGS) -o anvil $< $(includes) $(links) $(llvmFlags)

$(flexOut):  $(flexIn) | setup
	@echo make flex
	flex --outfile=$@ $(flexIn)

$(bisonOut): $(bisonIn) | setup
	@echo make bison
	bison -d $(bisonIn) -o $(bisonOut) $(bisonFlags)

$(parserOut): $(parserIn) $(bisonOut) $(flexOut) | setup
	@echo make parser
	g++ $(CXX_FLAGS) -shared -fPIC -o $@ $(parserIn) $(bisonOut) $(flexBisonDefines) $(includes) $(llvmFlags)

$(visitorOut): $(visitorIn) | setup
	@echo make visitor
	g++ $(CXX_FLAGS) -shared -fPIC -o $@ $(visitorIn) $(includes) $(llvmFlags) $(bad)

testing: bison testing/testing.cpp 
	g++ -g -o  testing/testing.out testing/testing.cpp $(bad) $(CXX_FLAGS)

.PHONY: setup
setup:
	mkdir -p derived

.PHONY: clean
clean:
	@rm -r $(derived)
	@rm $(executable)

.PHONY: run
run: all
	./anvil demo.avl	








