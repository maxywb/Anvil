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

binaryOperators = ["Add","Minus","Multiply","Divide","Modulo"]
sentinel = "Sentinel"
operatorPrecedence = {"LeftParen":sys.maxint,sentinel:-1,"Add":1,"Minus":1,"Multiply":2,"Divide":2,"Modulo":2}

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

    def parseExpression(self,operands,operators,terminator):
        self.currentToken = self.tokenStream.getNextToken()

        mine= self.currentToken
        print self.currentToken
        if self.see(["Id","Number"]):

            if self.peek("LeftParen"):
                current = self.parseFunctionCall()
            else:
                current = self.makeNode()



            operands.append(current)

            self.parseExpression(operands,operators,terminator)

        elif self.see(binaryOperators):
            current = self.makeNode()

            printer(operands,"operands")
            printer(operators,"operators")


            if precedes(current,operators[-1]):
                operators.append(current)

                if operators[-1] is not sentinel and len(operands)> 2:
                    printer(operands)
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
            printer(operands)
            printer(operators)
            self.parseExpression(subOperands,subOperators,"RightParen")

            subtree = wireExpressionTree(subOperands,subOperators)
            operands.append(subtree)

        elif self.see(terminator):
            return Terminator(self.currentToken) 



    def parseStatement(self):
        self.ctr = 0
        operands = []
        operators = [sentinel]

        self.parseExpression(operands,operators,"Semicolon")

        while not operators[-1] is sentinel:
            current = wireExpressionTree(operands,operators)
            operands.append(current)


        printer(operands,"operands")
        printer(operators,"operators")

        root = operands.pop()

        return root
