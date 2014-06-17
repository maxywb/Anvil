#!/usr/bin/env python



import Tokenizer
import Parser


def testParser():
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

    error = "y = asdf = 12;"

    tokens = Tokenizer.tokenize(error)

    tokenStream = Tokenizer.TokenStream(tokens)
    parser = Parser.Parser(tokenStream)
    root = parser.parse()

    print root






testParser()
