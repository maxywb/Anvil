%{
#define YYDEBUG 1
#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <cstdio>
#include FLEX_FILE


  extern FILE * yyin;


  //-- Lexer prototype required by bison, aka getNextToken()
  int yylex(); 
  int yyerror(const char *p) { cerr << "Error!" << endl; }
%}
%debug
%error-verbose
 //-- SYMBOL SEMANTIC VALUES -----------------------------
%union {
  int val; 
  char sym;
};
%token <val> NUM
%token <sym> ADD MULTIPLY LP RP SEMI
%type <val> statement expression literal

%%
statement_list: statement statement_list  | statement

statement: expression SEMI
{
  std::cout << $$ << std::endl;
}

| SEMI

expression: literal
{
  $$ = $1;
}
| expression ADD expression
{
  $$ = $1 + $3;
}
| expression MULTIPLY expression
{
  $$ = $1 * $3;
}
| LP expression RP
{
  $$ = $2;
}

literal: NUM
{
  $$ = $1;
}


%%
