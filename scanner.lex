%{
#include BISON_FILE
#include <iostream>
#include <string>
#include <vector>

%}

%option noyywrap

%%

"def" { return DEF;}
"if" { return IF;}
"elif" { return ELIF;}
"else" { return ELSE;}

[0-9]+   { yylval.val = atoi(yytext); return NUM; }
"+"  { yylval.sym = yytext[0]; return ADD; }
"-"   { yylval.sym = yytext[0]; return MINUS; }
"*"   { yylval.sym = yytext[0]; return MULTIPLY; }
"/"   { yylval.sym = yytext[0]; return DIVIDE; }
"%"   { yylval.sym = yytext[0]; return MODULO; }

">"   { yylval.sym = yytext[0]; return GT; }
"<"   { yylval.sym = yytext[0]; return LT; }
">="   { yylval.sym = yytext[0]; return GT_EQ; }
"<="   { yylval.sym = yytext[0]; return LT_EQ; }
"=="   { yylval.sym = yytext[0]; return EQUAL; }
"!="   { yylval.sym = yytext[0]; return NE; }

"^"   { yylval.sym = yytext[0]; return XOR; }
"&"   { yylval.sym = yytext[0]; return AND; }
"~"   { yylval.sym = yytext[0]; return OR; }
">>"   { yylval.sym = yytext[0]; return SHIFT_RIGHT; }
"<<"   { yylval.sym = yytext[0]; return SHIFT_LEFT; }

"."   { yylval.sym = yytext[0]; return DOT; }
","   { yylval.sym = yytext[0]; return COMMA; }

"="   { yylval.sym = yytext[0]; return ASSIGN; }

[a-zA-Z]+[a-zA-A0-9]* { 
  yylval.string = new std::string(&yytext[0],yyleng); 
  return ID;
}

"("      { return LP; }
")"      { return RP; }
"{"      { return LC; }
"}"      { return RC; }



\#.*     { }
";"      { return SEMI; }
<<EOF>>  { return 0; }
[ \t\n]+ { }
.        { std::cerr << "Unrecognized token: " << yytext[0] << " " << yytext << std::endl; exit(1); }
%%
