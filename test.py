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

    simple3="1+2*(3*(x+y));"
    simple4 ="1+foo(x,y);"
    current = simple4



    print current
    tokens = Tokenizer.tokenize(current)

    tokenStream = Tokenizer.TokenStream(tokens)
    parser = Parser.Parser(tokenStream)
    root = parser.parse()

    print root






testParser()
