import Parser,Tokenizer
import tests.parser.testParser as parseTest


parser = Parser.Parser()
tokenizer = Tokenizer.Tokenizer()
print "calling"
parseTest.testParserPositive()
print "done"
