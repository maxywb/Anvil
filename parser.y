%{
#define YYDEBUG 1
#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <list>

  /*
   * have to include Nodes.hpp first otherwise compile errors
   */
#include "Node/AllNodes.hpp"
#include "Node/operators.hpp"
#include FLEX_FILE


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
  anvil::Number * number;
  anvil::BinaryOperator * binaryOperator;
  anvil::FunctionCall * functionCall;
  anvil::Assignment * assignment;
  anvil::FunctionDefinition * functionDefinition;

  char sym;
  double val;
  std::string * string;

  std::list<std::string * > * vecString;

};


//terminals
%type <number> literal
%type <expr> expression 
%type <stmt> statement statement_list
%type <functionCall> function_call
%type <assignment> assignment_statement
%type <vecString> function_parameters
%type <functionDefinition> function_definition
 //tokens
%token <sym>  ADD MINUS MULTIPLY DIVIDE MODULO GT LT GT_EQ LT_EQ EQUAL NE XOR OR AND SHIFT_RIGHT SHIFT_LEFT DOT LC RC LP RP SEMI COMMA ASSIGN DEF
%token <string> ID 
%token <val> NUM

 //associativity
%right DEF
%left COMMA
%left ADD MINUS
%left MULTIPLY DIVIDE

%%

top : statement_list
{
  root = $1;
}


statement_list: statement statement_list
{
  $1->setNext($2);
}
| statement

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
| SEMI
{
  $$ = NULL;
}


expression: expression COMMA expression
{
  $$ = new anvil::Comma($1,$3);
}
|
literal
{
  $$ = $1;
}
| expression ADD expression
{
  $$ = new anvil::Addition($1,$3);			   
}
| expression MULTIPLY expression
{
  $$ = new anvil::Multiplication($1,$3);

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

function_definition: DEF ID LP function_parameters RP
{
  $$ = new anvil::FunctionDefinition($2,$4);
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
