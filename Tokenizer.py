import collections
import re

Token = collections.namedtuple('Token', ['kind', 'value', 'line', 'column'])

tokens = collections.deque()

def tokenize(s):
    keywords = {'IF', 'THEN', 'WHILE', 'FOR', 'ELSE', 'ELIF', 'CONTINUE', 'BREAK'\
                    'FUNC', 'RETURN', 'GOTO'}
    token_specification = [
        ('Comment',r'//(.+?)'),

        ('ShiftLeft',r'<<'),
        ('ShiftRight',r'>>'),
        ('Equal',r'=='),
        ('Or',r'\|\|'),
        ('And',r'&&'),
        ('Power',r'\*\*'),
        ('Add',r'\+'),
        ('Minus',r'\-'),
        ('Multiply',r'\*'),
        ('Divide',r'/'),
        ('Modulo',r'%'),
        ('Bang',r'!'),
        ('Tilde',r'~'),
        ('Carat',r'\^'),
        ('NotEqual',r'!='),
        ('LT',r'<'),
        ('GT',r'>'),
        ('LTEQ',r'<='),
        ('GTEQ',r'>='),
        ('Assign',r'='),

        ('Dollar',r'\$'),
        ('Colon',r':'),
        ('Semicolon',  r';'),

        ('Comma',r','),
        ('Dot',r'\.'),
        ('LeftParen',r'\('),
        ('RightParen',r'\)'),
        ('LeftBrace',r'\{'),
        ('RightBrace',r'\}'),
        ('LeftBracket',r'\['),
        ('RightBracket',r'\]'),

        ('Number',     r'\d+(\.\d*)?'), # Integer or decimal number
        ('Id',         r'[A-Za-z]+'),   # Identifiers
        ('String',r'\"(.+?)\"'),
        ('EOL',        r'\n'),       # Line endings
        ('Whitespace', r'[ \t]'),       # Skip over spaces and tabs
    ]
    tok_regex = '|'.join('(?P<%s>%s)' % pair for pair in token_specification)
    get_token = re.compile(tok_regex).match
    line = 1
    pos = line_start = 0
    mo = get_token(s)
    while mo is not None:
        typ = mo.lastgroup
        if typ == 'EOL':
            line_start = pos
            line += 1
        elif typ != 'Whitespace':
            val = mo.group(typ)
            if typ == 'ID' and val in keywords:
                typ = val
            tokens.append(Token(typ, val, line, mo.start()-line_start))
        pos = mo.end()
        mo = get_token(s, pos)
    if pos != len(s):
        raise RuntimeError('Unexpected character %r on line %d' %(s[pos], line))
    
    tokens.append(Token("EOF","EOF","EOF","EOF"))
    return tokens
class TokenStream:
    def __init__(self,tokens):
        self.tokens = tokens
        self.pointer = 0
        
    def __getitem__(self,index):
        try:
            return tokens[index]
        except IndexError:
            return None
    
    def current(self):
        return self[self.pointer]

    def getNextToken(self):
        ret = self.current()
        self.pointer = self.pointer + 1
        return ret

    def peek(self):
        return self[self.pointer]
