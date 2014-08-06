%{
#define YYDEBUG 1

  /*
   * have to include AllNodes.hpp first otherwise compile errors
   */
#include "ast/ast.hpp"

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

  anvil::Node * root;

%}

%debug
%error-verbose



%union {
  anvil::Expression * expr;
  anvil::Statement * stmt;
  anvil::StatementList * statementList;
  anvil::Number * number;
  anvil::BinaryOperator * binaryOperator;
  anvil::FunctionCall * functionCall;
  anvil::Assignment * assignment;
  anvil::FunctionDefinition * functionDefinition;

  anvil::ConditionalBlock * conditionalBlock;
  anvil::ConditionalBranch * conditionalBranch;
  std::list<anvil::ConditionalBranch *> * conditionalList;

  char sym;
  double val;
  std::string * string;

  std::list<std::string * > * stringList;

};


//terminals
%type <number> literal
%type <expr> expression 
%type <stmt> statement
%type <conditionalBlock> conditional
%type <functionCall> function_call
%type <assignment> assignment_statement
%type <stringList> function_parameters
%type <functionDefinition> function_definition
%type <statementList> statement_list

%type <conditionalBranch> if_branch
%type <conditionalBranch> elif_branch
%type <conditionalBranch> else_branch
%type <conditionalList> elif_list



 //tokens
%token <sym>  ADD MINUS MULTIPLY DIVIDE MODULO GT LT GT_EQ LT_EQ EQUAL NE XOR OR AND SHIFT_RIGHT SHIFT_LEFT DOT LC RC LP RP SEMI COMMA ASSIGN DEF IF ELSE ELIF
%token <string> ID 
%token <val> NUM

 //associativity


%left ADD MINUS
%left MULTIPLY DIVIDE

%%

top : statement_list
{
  root = $1;
}


statement_list: statement statement_list
{
  $2->push_front($1);
  $$ = $2;
}
| statement
{
    $$ = new anvil::StatementList();
    $$->push_front($1);
}
statement: expression SEMI
{
  $$ = $1;
}
| assignment_statement SEMI
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

| SEMI
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

expression: expression COMMA expression
{
  $$ = new anvil::BinaryOperator(anvil::operators::Comma,$1,$3);
}
|
literal
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
| LP expression RP
{
  $$ = $2;
}
| function_call 
{
  $$ = $1;
}

literal: NUM
{
  $$ = new anvil::Number(static_cast<double>($1));
  
}

function_call: ID LP RP
{
  $$ = new anvil::FunctionCall(*$1,NULL);
}
|
ID LP expression RP
{
  $$ = new anvil::FunctionCall(*$1,$3);
}

assignment_statement: ID ASSIGN expression
{
  $$ = new anvil::Assignment($1,$3);
}

function_definition: DEF ID LP function_parameters RP LC statement_list RC
{
  $$ = new anvil::FunctionDefinition($2,$4,$7);
}

function_parameters: ID COMMA function_parameters
{
  $3->insert($3->begin(),$1);
  $$ = $3;
}
| ID 
{
  $$ = new std::list<std::string *>();
  $$->insert($$->begin(),$1);
}

%%
