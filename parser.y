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

  anvil::ConditionalStatement * conditionalStatement;
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
  $$ = NULL;
}

| SEMI
{
  $$ = NULL;
}

conditional: if_branch elif_list else_branch
{
  std::cout << "if elif_list else" << std::endl;
}
| if_branch
{
    std::cout << "if" << std::endl;
}
| if_branch else_branch
{
    std::cout << "if else" << std::endl;
}

if_branch: IF LP expression RP LC statement_list RC
{
  $$ = NULL;
}
elif_branch: ELIF LP expression RP LC statement_list RC
{
  $$ = NULL;
}
else_branch: ELSE LC statement_list RC
{
  $$ = NULL;
}

elif_list: elif_branch elif_list
{
  $$ = NULL;
}
| elif_branch
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
