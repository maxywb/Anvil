%{
#define YYDEBUG 1

  /*
   * have to include Nodes.hpp first otherwise compile errors
   */
#include "Node/AllNodes.hpp"
#include "Node/operators.hpp"
#include FLEX_FILE

#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <cstdio>


  extern FILE * yyin;
  int yylex(); 
  int yyerror(const char *p) { cerr << p << " Error!" << endl; }
%}

%debug
%error-verbose


%union {
  anvil::Expression * expr;
  anvil::Number * number;
  anvil::BinaryOperator * binaryOperator;
  anvil::FunctionCall * functionCall;
  char sym;
  double val;
  std::string * str;
};

%type <number> literal
%type <expr> expression 
%type <sym> statement  
%type <functionCall> function_call


%token <sym>  ADD MINUS MULTIPLY DIVIDE MODULO GT LT GT_EQ LT_EQ EQUAL NE XOR OR AND SHIFT_RIGHT SHIFT_LEFT DOT LP RP SEMI COMMA
%token <str> ID 
%token <val> NUM


%left ADD MINUS
%left MULTIPLY DIVIDE


%%
statement_list: statement statement_list  | statement

statement: expression SEMI
{
  std::cout << $1->print() << std::endl;
}

| SEMI

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

%%
