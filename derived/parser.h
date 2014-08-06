/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ADD = 258,
     MINUS = 259,
     MULTIPLY = 260,
     DIVIDE = 261,
     MODULO = 262,
     GT = 263,
     LT = 264,
     GT_EQ = 265,
     LT_EQ = 266,
     EQUAL = 267,
     NE = 268,
     XOR = 269,
     OR = 270,
     AND = 271,
     SHIFT_RIGHT = 272,
     SHIFT_LEFT = 273,
     DOT = 274,
     LC = 275,
     RC = 276,
     LP = 277,
     RP = 278,
     SEMI = 279,
     COMMA = 280,
     ASSIGN = 281,
     DEF = 282,
     IF = 283,
     ELSE = 284,
     ELIF = 285,
     ID = 286,
     NUM = 287
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 31 "parser.y"

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




/* Line 2068 of yacc.c  */
#line 106 "/home/meatwad/Dropbox/src/AnViL/derived/parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


