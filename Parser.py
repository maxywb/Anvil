binaryOperators = ["Add","Minus","Multiply","Divide","Modulo"]
keywords = ["asdf"]
scope = ["asdf"]

## TODO change these to named tuples

class NodeFactory():
    def make(self,token,parent):
        kind = token.kind
        if kind == "Id":
            return Id(token,parent)
        elif kind == "Number":
            return Number(token,parent)
        elif kind == "SemiColon":
            return SemiColon(token,parent)
        elif kind == "LeftParen":
            return LeftParen(token,parent)
        elif kind == "Assign":
            return LeftParen(token,parent)
        elif kind in binaryOperators:
            return BinaryOperator(token,parent)

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
    def __init__(self,token,parent):
        self.token = token
        self.parent = parent
        self.left = None
        self.right = None
    def __str__(self):
        return "([%s]%s,%s)"%(self.token.value,self.left,self.right)

class Assignment:
    def __init__(self,token,parent):
        self.token = token
        self.parent = parent
        self.lhs = None
        self.rhs = None
    def __str__(self):
        return "([%s]%s=%s)"%(self.token.value,self.lhs,self.rhs)
    
class Id:
    def __init__(self,token,parent):
        self.token = token
        self.parent = parent
    def __str__(self):
        return "(%s)"%self.token.value

class Number:
    def __init__(self,token,parent):
        self.token = token
        self.parent = parent
    def __str__(self):
        return "(%s)"%self.token.value

class SemiColon:
    def __init__(self,token,parent):
        self.token = token
        self.parent = parent
    def __str__(self):
        return "(%s)"%self.token.value

class FunctionCall:
    def __init__(self,token,parent,arguments):
        self.token = token
        self.parent = parent
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

    def accepts(self,token):
        if type(token) is list:
            return self.currentToken.kind in token
        else:
            return self.currentToken.kind == token

    def parseFunctionArguments(self):
        arguments = list()
        while self.currentToken.kind != "RightParen":
            arguments.append(self.parseExpression(["Comma","RightParen"]))
        return arguments

    def parseFunctionCall(self):

        functionName = self.currentToken
        leftParen = self.tokenStream.getNextToken()
        if self.tokenStream.peek().kind == "RightParen":
            arguments = list()
        else:
            arguments = self.parseFunctionArguments()


        
        return FunctionCall(None,None,arguments)
        

    def parseAssignment(self):
        
        return "asdf"

    def parseExpression(self,terminator):

        self.currentToken = self.tokenStream.getNextToken()

        myToken = self.currentToken
        myRoot = self.currentNode

        if self.accepts(terminator):
            return self.currentToken
        elif self.accepts(binaryOperators):
            myNode = self.nodeFactory.make(self.currentToken,myRoot)
            subtree = self.parseExpression(terminator)
            
            myNode.right = subtree
            subtree.parent = myNode
            return myNode

        elif self.accepts("Id") or self.accepts("Number"):
            myNode = self.nodeFactory.make(self.currentToken,myRoot)
            if self.tokenStream.peek().kind == "LeftParen":
                subtree =  self.parseFunctionCall()

            subtree = self.parseExpression(terminator)

        elif self.accepts("Assign"):
            assign = Assignment(self.currentToken,myRoot)
            subtree = self.parseExpression(terminator)
            assign.rhs = subtree
            subtree.root = assign
            return assign
        else:
            subtree = self.parseExpression(terminator)

        if isinstance(subtree,BinaryOperator):
            myRoot.child = subtree 
            subtree.left = myNode
            myNode.parent = subtree
            return subtree
        elif isinstance(subtree,FunctionCall):
            subtree.token = myToken
            return subtree
        elif isinstance(subtree,Assignment):
            myNode.root = subtree
            subtree.lhs = myNode
            return subtree
        else:
            return myNode

        

    def parseAny(self):
        self.currentToken = self.tokenStream.peek();
        myRoot = StatementList()
        self.currentNode.child = myRoot
        self.currentNode = myRoot
        while(True):
            self.currentToken = self.tokenStream.peek();

            if self.accepts(["Id","Number"]):
                subtree = self.parseExpression("Semicolon")

            elif self.accepts("LeftBrace"):
                subtree = self.parseScope()
            elif self.accepts("Semicolon") :
                self.tokenStream.getNextToken();


            myRoot.statements.append(subtree)

            if self.tokenStream.peek().kind == "EOF":
                break


    def parse(self):
        root = Root()
        self.currentNode = root
        self.parseAny()

        return root
