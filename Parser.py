binaryOperators = ["Add","Minus","Multiply","Divide","Modulo"]
sentinel = "Sentinel"
operatorPrecedence = {sentinel:-1,"Add":1,"Minus":1,"Multiply":2,"Divide":2,"Modulo":2}

def precedes(this,that):
    if isinstance(that,str):
        return operatorPrecedence[this.token.kind] > operatorPrecedence[that]
    else:
        return operatorPrecedence[this.token.kind] > operatorPrecedence[that.token.kind]

keywords = ["asdf"]
scope = ["asdf"]

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
            return LeftParen(token)
        elif kind in binaryOperators:
            return BinaryOperator(token)

class Root:
    def __init__(self):
        self.child = None
    def __str__(self):
        return ">>\n%s"%self.child

class StatementList:
    def __init__(self):
        self.statements = list()
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
        return "([%s]%s=%s)"%(self.token.value,self.lhs,self.rhs)
    
class Id:
    def __init__(self,token):
        self.token = token
        self.parent = None
    def __str__(self):
        return "(%s)"%self.token.value

class Number:
    def __init__(self,token):
        self.token = token
        self.parent = None
    def __str__(self):
        return "(%s)"%self.token.value

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

        return "(%s(%s))"%(self.token.value,argStr)

    
####################################################################################################


class Parser:
    def __init__(self,tokenStream):
        self.tokenStream = tokenStream
        self.nodeFactory = NodeFactory()

    def makeNode(self):
        return self.nodeFactory.make(self.currentToken)

    def see(self,anyOfThese):
        return self.currentToken.kind in anyOfThese

    def peek(self,anyOfThese):
        return self.tokenStream.peek().kind in anyOfThese

    def parseFunctionCall(self):
        raise Exception("not supported yet")

    def parseExpression(self,terminator):
        self.currentToken = self.tokenStream.getNextToken()
        print self.currentToken

        if self.see(["Id","Number"]):

            if self.peek("LeftParen"):
                current = self.parseFunctionCall()
            else:
                current = self.makeNode()
            self.operands.append(current)

        elif self.see(binaryOperators):
            current = self.makeNode()

            self.parseExpression(terminator)


            if precedes(current,self.operators[-1]):
                print "precedes"
                self.wire(current)
            elif self.see("LeftParen"):
                lp = self.make()
                self.operators.append()
                self.parseExpression("RightParen")

                current = None
                while not current is lp:
                    current = self.wire()
            else:
                self.operators.append(current)

            return current

        elif self.see(terminator):
            return Terminator(self.currentToken) 
        
        self.parseExpression(terminator)

    def wire(self,current=None):
        if not current:
            current =self.operators.pop()
        right = self.operands.pop()
        left = self.operands.pop()

        print current,left,right

        current.left = left
        current.right = right
        left.parent = current
        right.parent = current
        self.operands.append(current)
        return current

    def parse(self):
        self.operators = [sentinel]
        self.operands = []
        root = Root()
        self.currentNode = root
        self.parseExpression("Semicolon")

        print "operands"
        for i in self.operands:
            print i,
        print ""
        print "operators"
        for i in self.operators:
            print i,
        print ""

        return root
