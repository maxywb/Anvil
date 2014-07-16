

import sys
def printer(thing,message=""):
    print message,
    for t in thing:
        print t,
    print ""

def wireExpressionTree(operands,operators):
    op = operators.pop()

    right = operands.pop()
    left = operands.pop()

    op.left = left
    op.right= right
    left.parent = op
    right.parent = op
    return op

keywords = ["def","if","else","elif","for","while","struct"]
binaryOperators = ["Add","Minus","Multiply","Divide","Modulo"]
sentinel = "Sentinel"
operatorPrecedence = {"LeftParen":sys.maxint,sentinel:-1,"Add":1,"Minus":1,"Multiply":2,"Divide":2,"Modulo":2}

def precedes(this,that):
    if isinstance(that,str):
        return operatorPrecedence[this.token.kind] > operatorPrecedence[that]
    else:
        return operatorPrecedence[this.token.kind] > operatorPrecedence[that.token.kind]

        
        



## TODO change these to named tuples

class NodeFactory():
    def make(self,token):
        kind = token.kind
        if kind == "Id":
            return Id(token)
        elif kind == "Number":
            return Number(token)
        elif kind == "SemiColon":
            return SemiColon(token)
        elif kind == "LeftParen":
            return LeftParen(token)
        elif kind == "Assign":
            return Assignment(token)
        elif kind in binaryOperators:
            return BinaryOperator(token)

class Root:
    def __init__(self):
        self.child = None
    def __str__(self):
        return ">>\n%s"%self.child

class StatementList:
    def __init__(self):
        self.statements = None
    def __str__(self):
        tostring = ""
        for s in self.statements:
            tostring = tostring + ":%s\n"%s
        return tostring

class BinaryOperator:
    def __init__(self,token):
        self.token = token
        self.parent = None
        self.left = None
        self.right = None
    def __str__(self):
        return "([%s]%s,%s)"%(self.token.value,self.left,self.right)

class Assignment:
    def __init__(self,token):
        self.token = token
        self.parent = None
        self.lhs = None
        self.rhs = None
    def __str__(self):
        return "%s=%s"%(self.lhs,self.rhs)
    
class LeftParen:
    def __init__(self,token):
        self.token = token
    def __str__(self):
        return "("

class Id:
    def __init__(self,token):
        self.token = token
        self.parent = None
    def __str__(self):
        return "%s"%self.token.value

class Number:
    def __init__(self,token):
        self.token = token
        self.parent = None
    def __str__(self):
        return "%s"%self.token.value

class Terminator:
    def __init__(self,token):
        self.token = token
        self.parent = None
    def __str__(self):
        return "(%s)"%self.token.value

class FunctionCall:
    def __init__(self,token,arguments):
        self.token = token
        self.parent = None
        self.arguments = arguments
    def __str__(self):
        argStr = ""
        for arg in self.arguments:
            argStr = argStr+"%s,"%arg

        return "%s(%s)"%(self.token.value,argStr)

class Scope:
    def __init__(self,token,statements):
        self.statements = statements
        self.line = token.line
        self.column = token.column
    def __str__(self):
        tostring = "{\n"
        for s in self.statements:
            tostring = tostring + ":%s\n"%s
        tostring = tostring + "}"
        return tostring
    
####################################################################################################


class Parser:
    def __init__(self):
        self.nodeFactory = NodeFactory()

    def makeNode(self):
        return self.nodeFactory.make(self.currentToken)

    def see(self,anyOfThese):
        return self.currentToken.kind in anyOfThese

    def peek(self,anyOfThese,index=0):
        return self.tokenStream.peek(index).kind in anyOfThese

    def parseFunctionCall(self):
        funcId = self.currentToken
        args = []
        current = self.tokenStream.getNextToken() # nuke the (
        while not current.kind == "RightParen":
        
            operands = []
            operators = [sentinel]
            self.parseExpression(operands,operators,["Comma","RightParen"])

            if len(operators) > 1:
                curArg = wireExpressionTree(operands,operators)                
            else:
                curArg = operands[-1]

            args.append(curArg)

            current = self.currentToken

        return FunctionCall(funcId,args)

    def parseScope(self):

        statements = []
        current = self.currentToken
        start = current
        while not current.kind == "RightBrace":
        
            stmt = self.parseStatement()
            statements.append(stmt)
            current = self.currentToken#tokenStream.getNextToken() 
            current = self.tokenStream.peek(0)


        self.tokenStream.getNextToken()
        return Scope(start,statements)

    def parseExpression(self,operands,operators,terminator):
        self.currentToken = self.tokenStream.getNextToken()

        if self.see(["Id","Number"]):

            if self.peek("LeftParen"):
                current = self.parseFunctionCall()
            else:
                current = self.makeNode()

            operands.append(current)

            self.parseExpression(operands,operators,terminator)

        elif self.see(binaryOperators):
            current = self.makeNode()

            if precedes(current,operators[-1]):
                operators.append(current)

                if operators[-1] is not sentinel and len(operands)> 2:
                    prev = wireExpressionTree(operands,operators)
                    operands.append(prev)
            else:
                prev= current
                current = wireExpressionTree(operands,operators)
                operands.append(current)
                operators.append(prev)

            self.parseExpression(operands,operators,terminator)
            
        elif self.see("LeftParen"):

            lp = self.makeNode()
            
            subOperands= []
            subOperators = [sentinel]

            self.parseExpression(subOperands,subOperators,"RightParen")

            subtree = wireExpressionTree(subOperands,subOperators)
            operands.append(subtree)
            self.parseExpression(operands,operators,terminator)

        elif self.see("LeftBrace"):

            scope = self.parseScope()
            operands.append(scope)

        elif self.see(terminator):
            return Terminator(self.currentToken) 
        else:
            raise Exception("unexpected "+str(self.currentToken))

    def parseStatement(self):
        self.ctr = 0
        operands = []
        operators = [sentinel]

        if self.peek("Assign",1):
            self.currentToken = self.tokenStream.getNextToken()
            lhs = Id(self.currentToken)
            eq = self.tokenStream.getNextToken()
            assign = Assignment(eq)
            assign.lhs = lhs

            self.parseExpression(operands,operators,"Semicolon")

            while not operators[-1] is sentinel:
                current = wireExpressionTree(operands,operators)
                operands.append(current)


            assign.rhs = operands[-1]

            root = assign

        else:
            self.parseExpression(operands,operators,"Semicolon")

            while not operators[-1] is sentinel:
                current = wireExpressionTree(operands,operators)
                operands.append(current)

            root = operands.pop()

        return root

    def parseKeywords(self):
        if self.see("if"):
            self.parseConditional()
        elif self.see("def"):
            self.parseFunctionDefinition()
        elif self.see("for"):
            self.parseForLoop()
        elif self.see("while"):
            self.parseWhileLoop()
        elif self.see("struct"):
            self.parseStruct()
        else:
            ct = self.currentToken
            tt = (ct.kind,ct.value,ct.line,ct.column)
            raise Exception("Unexpected Token %s %s %d %d"%tt)

    def parseAny(self):
        statements = []
        while not self.peek("EOF"):
            if self.peek(keywords):
                parseKeywords()
            else:
                stmt = self.parseStatement()
                statements.append(stmt)
        

        stmts = StatementList()
        stmts.statements = statements
        return stmts

    def parse(self,tokenStream):
        self.tokenStream = tokenStream
        return self.parseAny()        
