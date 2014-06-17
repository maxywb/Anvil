#!/usr/bin/env python


import GrammarUtils
import Tokenizer
import Parser

def testGrammar():
    rules = GrammarUtils.readGrammar("test/simpleGrammar.py")

    symbols = GrammarUtils.findAllSymbols(rules)
    terminalSymbols = GrammarUtils.findTerminalSymbols(symbols,rules)
    nonTerminalSymbols = GrammarUtils.findNonTerminalSymbols(symbols,rules)

    firsts = GrammarUtils.findFirstSets(symbols,rules,terminalSymbols,nonTerminalSymbols)
    follows = GrammarUtils.findFollowSets(symbols,rules,terminalSymbols,nonTerminalSymbols,firsts)
    print firsts
    print follows


def testLexer():
    allSstatements = '''
    IF quantity {
        x = y
        x + y
        x - y
        x * y
        x / y
        x % y
        x < y
        x > y
        x << y
        x >> y
        x >= y
        x <= y
        x != y
        x ^ y
        x == y
        x ** y
        foo()
        x[] 
        $ // this is a comment

  "asdfasdf+_)(*&^%$#@!~`"
     }

'''
    simpleStatements = '''
a + b - c * 2 % foo(d,3,4+5);
x = 12;
y = m*x+b;
'''
    simple2 = "y = m*x+b;"


    tokens = Tokenizer.tokenize(simpleStatements)

    tokenStream = Tokenizer.TokenStream(tokens)
    parser = Parser.Parser(tokenStream)
    root = parser.parse()

    print root






testLexer()
