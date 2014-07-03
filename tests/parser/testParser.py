#!/usr/bin/env python
import glob
import Parser,Tokenizer



def testParserPositive():
    print "testParser"
    avlFiles = glob.glob("tests/parser/positiveFiles/*.avl")
    goldFiles = glob.glob("./*.gold")

    tokenizer = Tokenizer.Tokenizer()
    parser = Parser.Parser()

    for currentAvl in avlFiles:

        fp = open(currentAvl)
        avlText = fp.read()

        tokens = tokenizer.tokenize(avlText)
        tokenStream = Tokenizer.TokenStream(tokens)



        print currentAvl

        print parser.parse(tokenStream)


