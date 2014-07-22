%{
#define YYDEBUG 1

  /*
   * have to include Nodes.hpp first otherwise compile errors
   */
#include "Nodes.hpp"
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
  char sym;
  double val;
};

%type <number> literal
%type <expr> expression 
%token <sym>  ADD MULTIPLY LP RP SEMI

%type <sym> statement  
%token <val> NUM


%left ADD
%left MULTIPLY


%%
statement_list: statement statement_list  | statement

statement: expression SEMI
{
  std::cout << $1->print() << std::endl;
}

| SEMI

expression: literal
{
  $$ = static_cast<anvil::Expression *>($1);
}
| expression ADD expression
{
  $$ = new anvil::BinaryOperator(
			     static_cast<anvil::Expression *>($1),
			     static_cast<anvil::Expression *>($3),
			     anvil::operators::ADD);

  std::cout << "expr: " << $$->print() << std::endl;



}
| expression MULTIPLY expression
{
  $$ = new anvil::BinaryOperator(
			     static_cast<anvil::Expression *>($1),
			     static_cast<anvil::Expression *>($3),
			     anvil::operators::MULTIPLY);
}
| LP expression RP
{
  $$ = $2;
}

literal: NUM
{
  $$ = new anvil::Number(static_cast<size_t>($1));
  std::cout << $1 << std::endl;
}


%%
