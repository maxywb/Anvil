%{
#define YYDEBUG 1

  /*
   * have to include AllNodes.hpp first otherwise compile errors
   */
#include "ast/ast.hpp"
#include "parser/Parser.hpp"
#include FLEX_FILE   

#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <list>

  extern FILE * yyin;
  int yylex(); 
  int yyerror(const char *p) { std::cerr << p << " Error!" << std::endl; }

  std::list<anvil::Statement *> * s_root;

%}

%debug
%error-verbose



%union {
  anvil::Expression * expr;
  anvil::Statement * stmt;
  anvil::BinaryOperator * binaryOperator;
  anvil::FunctionCall * functionCall;
  anvil::Assignment * assignment;
  anvil::FunctionDefinition * functionDefinition;

  anvil::ConditionalBlock * conditionalBlock;
  anvil::ConditionalBranch * conditionalBranch;
  std::list<anvil::ConditionalBranch *> * conditionalList;

  anvil::ForLoop * forLoop;
  anvil::WhileLoop * whileLoop;

  char sym;
  double doubleValue;
  int intValue;
  std::string * string;

  std::list<anvil::Id * > * idList;
  std::list<anvil::Expression * > * exprList;
  std::list<anvil::Statement *> * stmtList;

};


//terminals
%type <expr> literal
%type <expr> expression 
%type <stmt> statement
%type <conditionalBlock> conditional
%type <functionCall> function_call
%type <assignment> assignment
%type <exprList> function_arguments
%type <idList> function_parameters
%type <functionDefinition> function_definition
%type <stmtList> statement_list

%type <conditionalBranch> if_branch
%type <conditionalBranch> elif_branch
%type <conditionalBranch> else_branch
%type <conditionalList> elif_list

%type <stmt> loop
%type <forLoop> for_loop
%type <whileLoop> while_loop

//non-terminals
%token <sym>  ADD MINUS MULTIPLY DIVIDE MODULO GT LT GT_EQ LT_EQ EQUAL NE XOR OR AND SHIFT_RIGHT
%token <sym>  SHIFT_LEFT DOT LC RC LP RP SEMI COMMA ASSIGN DEF IF ELSE ELIF FOR WHILE BREAK RETURN
%token <sym>  LS RS 
%token <string> ID 
%token <doubleValue> DOUBLE
%token <intValue> INT

//associativity
%left ADD MINUS LT
%left MULTIPLY DIVIDE 
%left COMMA
%%

top : statement_list
{
  s_root = $1;
}


statement_list: statement statement_list
{
  $2->emplace_front($1);
  $$ = $2;
}
| statement
{
  $$ = new std::list<anvil::Statement *>();
  $$->emplace_front($1);
}

statement: expression SEMI
{
  $$ = $1;
}
|
function_definition
{
  $$ = $1;
}
|
conditional
{
  $$ = $1;
}
|
loop
{
  $$ = $1;
}
|
RETURN expression SEMI
{
  $$ = new anvil::ReturnStatement($2);
}
|
SEMI
{
  $$ = NULL;
}

conditional: if_branch elif_list else_branch
{
    $$ = new anvil::ConditionalBlock($1,$2,$3);
}
| if_branch
{
    $$ = NULL;
}
| if_branch else_branch
{
    $$ = NULL;
}

if_branch: IF LP expression RP LC statement_list RC
{
    $$ = new anvil::ConditionalBranch($3,$6);
}
elif_branch: ELIF LP expression RP LC statement_list RC
{
    $$ = new anvil::ConditionalBranch($3,$6);
}
else_branch: ELSE LC statement_list RC
{
    $$ = new anvil::ConditionalBranch(NULL,$3);
}

elif_list: elif_branch elif_list
{
  $$ = $2;
  $$->push_front($1);
}
| elif_branch
{
    $$ = new std::list<anvil::ConditionalBranch *>();
    $$->push_front($1);
}

expression: literal
{
  $$ = $1;
}
| expression ADD expression
{
  $$ = new anvil::BinaryOperator(anvil::operators::Add,$1,$3);			   
}
| expression MULTIPLY expression
{
  $$ = new anvil::BinaryOperator(anvil::operators::Multiply,$1,$3);
}
| expression LT expression
{
  $$ = new anvil::BinaryOperator(anvil::operators::LessThan,$1,$3);
}
| LP expression RP
{
  $$ = $2;
}
| function_call 
{
  $$ = $1;
}
|
assignment
{
  $$ = $1;
}
| ID
{
  $$ = new anvil::Id($1);
}
literal: INT
{
  $$ = new anvil::Integer($1);
  
}
|
DOUBLE
{
  $$ = new anvil::Double($1);
  
}
function_call: ID LP RP
{
  $$ = new anvil::FunctionCall(*$1,NULL);
}
|
ID LP function_arguments RP
{
  $$ = new anvil::FunctionCall(*$1,$3);
}

function_arguments: expression COMMA function_arguments
{
  $3->emplace_front($1);
  $$ = $3;
}
| expression
{
  $$ = new std::list<anvil::Expression *>();
  $$->emplace_front($1);
}

assignment: ID ASSIGN expression
{
  $$ = new anvil::Assignment($1,$3);
}

function_definition: DEF ID LP function_parameters RP LC statement_list RC
{
  $$ = new anvil::FunctionDefinition($2,$4,$7);
}
| DEF ID LP RP LC statement_list RC
{
  $$ = new anvil::FunctionDefinition($2, new std::list<anvil::Id *>(), $6);
}
function_parameters: ID COMMA function_parameters
{
  $3->emplace_front(new anvil::Id($1));
  $$ = $3;
}
| ID
{
  $$ = new std::list<anvil::Id *>();
  $$->emplace_front(new anvil::Id($1));
}


loop: for_loop
{
  $$ = $1;
}
|
while_loop
{
  $$ = $1;
}

for_loop: FOR LP expression SEMI expression SEMI expression RP LC statement_list RC
{
  $$ = new anvil::ForLoop($3,$5,$7,$10);
}

while_loop: WHILE LP expression RP LC statement_list RC
{
  $$ = new anvil::WhileLoop($3,$6);
}
%%
