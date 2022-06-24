%option noyywrap
%option yylineno

/* The prefix for imports in c syntax */
%{
/* For Indentation */
#include <stack>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#if __cplusplus > 199711L
#define register      // Deprecated in C++11.
#endif  // #if __cplusplus > 199711L

using namespace std;
std::stack<int> stack_indent;

#include <chocopy_parse.hpp>
#include "chocopy.tab.h"

int e1 = 1, e2 = 1,global_leng=0;
string info = "";

#define LITERAL(type) { \
    LOG(DEBUG) << #type << " " <<  yytext << " location: "<< yylloc.first_line<<" " << yylloc.first_column<< " " << (e1 = yylloc.last_line) <<" " << (e2 = yylloc.last_column); \
    return TOKEN_##type; \
}

#define LITERAL_END(type) { \
    LOG(DEBUG) << #type << " " <<  yytext << " location: "<< yylloc.first_line<<" " << yylloc.first_column<< " " << (e1 = yylloc.last_line) <<" " << (e2 = yylloc.last_column); \
    yylloc.last_column=1; yylloc.last_line+=1;  \
    return TOKEN_##type; \
}

int ColNum = 1;
int LineNum = 1;
#define SET_ACTION                                                           \
  yylloc.first_line = LineNum;                                               \
  yylloc.first_column = ColNum;                                              \
  ColNum += yyleng;                                                          \
  yylloc.last_line = LineNum;                                                \
  yylloc.last_column = ColNum;

#define ANOTHER_LINE                                                         \
  ColNum = 1;                                                                \
  LineNum += 1;
#define CLEAR_ONE                                                         \
  ColNum -= 4;                                                                \

#define YY_ANOTHER_TAB                                                       \
  ColNum += 4;
#define YY_ANOTHER_SPACE                                                     \
  ColNum += 1;
#define YY_ANOTHER_STR(i)                                                    \
  ColNum += (i);
%}

%x INDENT
%x STRING
%x REALLYEND

%%

%{
  /* Init indent stack_indent */
  if (stack_indent.empty()) {
    stack_indent.push(0);
  }

  int current_indent = 0;
%}
/* The Lexical rules Your Code Here */

[0-9]+                            { yylval.raw_int = atof(yytext); return return TOKEN_INTEGER; }
[a-zA-Z_][a-zA-Z0-9_]*             { yylval.raw_str = yytext; return TOKEN_IDENTIFIER; }
/* \"(\\.|[^"\\])*\"                 { yylval.raw_str = yytext; return TOKEN_STRING; } */
\"                                {BEGIN(STRING);}
<STRING>{
  \n                        yyerror("the string misses \" to termiate before newline");
  <<EOF>>                   yyerror("the string misses \" to terminate before EOF");
  ([^\\\"]|\\.)*            { yylval.raw_str = yytext; }
  \"                        { BEGIN(INITIAL);  return TOKEN_STRING; }
  .                         ;
}
"True"                            { yylval.raw_str = yytext; return TOKEN_TRUE; }
"False"                           { yylval.raw_str = yytext; return TOKEN_FALSE; }
"and"                             { yylval.raw_str = yytext; return TOKEN_AND; }
"break"                           { yylval.raw_str = yytext; return TOKEN_BREAK; }
"def"                             { yylval.raw_str = yytext; return TOKEN_DEF; }
"elif"                            { yylval.raw_str = yytext; return TOKEN_ELIF; }
"else"                            { yylval.raw_str = yytext; return TOKEN_ELSE; }
"for"                             { yylval.raw_str = yytext; return TOKEN_FOR; }
"if"                              { yylval.raw_str = yytext; return TOKEN_IF; }
"not"                             { yylval.raw_str = yytext; return TOKEN_NOT; }
"or"                              { yylval.raw_str = yytext; return TOKEN_OR; }
"return"                          { yylval.raw_str = yytext; return TOKEN_RETURN; }
"while"                           { yylval.raw_str = yytext; return TOKEN_WHILE; }
"None"                            { yylval.raw_str = yytext; return TOKEN_NONE; }
"as"                              { yylval.raw_str = yytext; return TOKEN_AS; }
"class"                           { yylval.raw_str = yytext; return TOKEN_CLASS; }
"global"                          { yylval.raw_str = yytext; return TOKEN_GLOBAL; }
"in"                              { yylval.raw_str = yytext; return TOKEN_IN; }
"is"                              { yylval.raw_str = yytext; return TOKEN_IS; }
"nonlocal"                        { yylval.raw_str = yytext; return TOKEN_NONLOCAL; }
"pass"                            { yylval.raw_str = yytext; return TOKEN_PASS; }

"+"                               { yylval.raw_str = yytext; return TOKEN_plus; }
"-"                               { yylval.raw_str = yytext; return TOKEN_minus; }
"*"                               { yylval.raw_str = yytext; return TOKEN_star; }
"//"                              { yylval.raw_str = yytext; return TOKEN_slash; }
"%"                               { yylval.raw_str = yytext; return TOKEN_percent; }
"<"                               { yylval.raw_str = yytext; return TOKEN_less; }
">"                               { yylval.raw_str = yytext; return TOKEN_greater; }
"<="                              { yylval.raw_str = yytext; return TOKEN_lessequal; }
">="                              { yylval.raw_str = yytext; return TOKEN_greaterequal; }
"=="                              { yylval.raw_str = yytext; return TOKEN_equalequal; }
"!="                              { yylval.raw_str = yytext; return TOKEN_exclaimequal; }
"="                               { yylval.raw_str = yytext; return TOKEN_equal; }
"("                               { yylval.raw_str = yytext; return TOKEN_l_paren; }
")"                               { yylval.raw_str = yytext; return TOKEN_r_paren; }
"["                               { yylval.raw_str = yytext; return TOKEN_l_square; }
"]"                               { yylval.raw_str = yytext; return TOKEN_r_square; }
","                               { yylval.raw_str = yytext; return TOKEN_r_comma; }
":"                               { yylval.raw_str = yytext; return TOKEN_r_colon; }
"."                               { yylval.raw_str = yytext; return TOKEN_r_period; }
"->"                              { yylval.raw_str = yytext; return TOKEN_r_rarrow; }

<INITIAL> [\n]       { current_indent = 0; BEGIN(INDENT); }
<INDENT> [\t]           { current_indent += 8; current_indent = current_indent & (~7);}
<INDENT> [ ]             { current_indent += 1; }
<INDENT> [\n]       { current_indent = 0; } 
<INDENT> .            { 
                          int now_indent_level = stack_indent.top();
                          if ( current_indent == now_indent_level ){
                              BEGIN(INITIAL);
                          }else if ( current_indent > now_indent_level ){
                              stack_indent.push( current_indent );
                              return TOKEN_INDENT;
                          }else {
                              while( current_indent < stack_indent.top()){
                                stack_indent.pop();
                                return TOKEN_DEDENT;
                              }
                          }
                      }

<INITIAL> <<EOF>>                 { BEGIN(REALLYEND); }
<REALLYEND> <<EOF>>               { return 0; }
.                                 {
                                      LOG(ERROR) << "Error: Unrecognized token on line " << yylineno << ": " << yytext;
                                      return 1;
                                  }
%%

int yywrap(void)
{
    return 1;
}