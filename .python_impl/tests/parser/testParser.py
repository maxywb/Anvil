#!/usr/bin/env python
import glob
import Parser,Tokenizer



def testParserPositive():
    print "testParser"
    avlFiles = glob.glob("tests/parser/positiveFiles/*.avl")
    goldFiles = glob.glob("tests/parser/positiveFiles/*.gold")

    avlFiles.sort()
    goldFiles.sort()



    tokenizer = Tokenizer.Tokenizer()
    parser = Parser.Parser()



    for itr in range(len(avlFiles)):
        avlFilename = avlFiles[itr]
        avlFp = open(avlFilename)
        avlText = avlFp.read()
        tokens = tokenizer.tokenize(avlText)
        tokenStream = Tokenizer.TokenStream(tokens)
        avlOutput = parser.parse(tokenStream)
        try:
            goldFilename = goldFiles[itr]
            goldFp = open(goldFilename)
            goldText = goldFp.read()
            print str(avlOutput) == goldText
        
        except IndexError:
            print "%s does not have a .gold file"%avlFilename            
            print avlOutput
