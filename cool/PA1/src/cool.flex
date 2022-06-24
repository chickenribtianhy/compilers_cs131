%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
  if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
    YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */

/*  from stringtab.h 
extern IdTable idtable;
extern IntTable inttable;
extern StrTable stringtable; */

int comment_level = 0;   /* denotes the nested comment level */

#define add_to_string(c) {if(string_buf_ptr-string_buf>=MAX_STR_CONST){BEGIN(STRING_RECOVERY);yylval.error_msg="String constant too long";return ERROR;}else{*string_buf_ptr = c;++string_buf_ptr;} }
                      

%}

%option noyywrap

/* %s INITIAL */
%x STRING
%x STRING_RECOVERY
/* %x REALLYEND */
%x COMMENT

/* Define names for regular expressions here. */

/* 10.1 integers,identifiers, and special notation */
/* // digit       [0-9] */
INTEGER     [0-9]+
TYPE_IDENTIFIER   [A-Z][A-Za-z0-9_]*  
OBJECT_IDENTIFIER   [a-z][A-Za-z0-9_]*  
/* // SELF        self
// self_type   SELF_TYPE */

/* 10.2 strings, and 10.3 comments would be implemented later */

/* 10.4 Keywords */
CLASS       (?i:class)
ELSE        (?i:else)
FALSE       f(?i:alse)
FI          (?i:fi)
IF          (?i:if)
IN          (?i:in)
INHERITS    (?i:inherits)
ISVOID      (?i:isvoid)
LET         (?i:let)
LOOP        (?i:loop)
POOL        (?i:pool)
THEN        (?i:then)
WHILE       (?i:while)
CASE        (?i:case)
ESAC        (?i:esac)
NEW         (?i:new)
OF          (?i:of)
NOT         (?i:not)
TRUE        t(?i:rue)

/* 10.5 white space */
WHITE_SPACE [ \t\r\f\v]+

/* union from paser, here from cool-parser.h */
/* typedef union
  {
    Boolean boolean;
    Symbol symbol;
    Program program;
    Class_ class_;
    Classes classes;
    Feature feature;
    Features features;
    Formal formal;
    Formals formals;
    Case case_;
    Cases cases;
    Expression expression;
    Expressions expressions;
    const char *error_msg;
    } yystype; 
*/

/* tokens are defined in cool-parser.h */
/* #define CLASS 258
#define ELSE 259
#define FI 260
#define IF 261
#define IN 262
#define INHERITS 263
#define LET 264
#define LOOP 265
#define POOL 266
#define THEN 267
#define WHILE 268
#define CASE 269
#define ESAC 270
#define OF 271
#define DARROW 272
#define NEW 273
#define ISVOID 274
#define STR_CONST 275
#define INT_CONST 276
#define BOOL_CONST 277
#define TYPEID 278
#define OBJECTID 279
#define ASSIGN 280
#define NOT 281
#define LE 282
#define ERROR 283 */

%%

 /*
  * Define regular expressions for the tokens of COOL here. Make sure, you
  * handle correctly special cases, like:
  *   - Nested comments
  *   - String constants: They use C like systax and can contain escape
  *     sequences. Escape sequence \c is accepted for all characters c. Except
  *     for \n \t \b \f, the result is c.
  *   - Keywords: They are case-insensitive except for the values true and
  *     false, which must begin with a lower-case letter.
  *   - Multiple-character operators (like <-): The scanner should produce a
  *     single token for every such operator.
  *   - Line counting: You should keep the global variable curr_lineno updated
  *     with the correct line number
  */

  /* parse comments first */
  /* 4. comments */
  /* 4.1 inline comments, do nothing */
<INITIAL>--.*      { }
<INITIAL>[\n]      {++curr_lineno;}
  /* 4.2 enclosing comments */
<INITIAL>"*)"     {yylval.error_msg = "Unmatched *)";return ERROR;}
<INITIAL,COMMENT>"(*"    {BEGIN(COMMENT);
               ++comment_level;}
<COMMENT>"*)"     {--comment_level;
                  if(comment_level==0)
                      BEGIN(INITIAL);
                  }     
<COMMENT>[^*\n]*    { } /* ignore anything but \n and *, warning * */
<COMMENT>"*"[^)\n]*  { } /* ignore *, but *) */
<COMMENT>[\n]       {++curr_lineno;}
<COMMENT><<EOF>> {BEGIN(INITIAL);
                  yylval.error_msg = "EOF in comment";return ERROR;}

  /* string next */
  /* 5. strings */
<INITIAL>(\")   {BEGIN(STRING);
                string_buf_ptr = string_buf;}
<STRING>(\")    {BEGIN(INITIAL);
                for(char* ptr = string_buf; ptr < string_buf_ptr; ++ptr){
                    if( (*ptr) == 0 ){
                    yylval.error_msg="String contains null character.";
                    return ERROR;}
                    }
                    yylval.symbol = stringtable.add_string(string_buf, string_buf_ptr-string_buf);
                    return STR_CONST;
                }
<STRING>[\n]    {  ++curr_lineno;
              BEGIN(INITIAL);
            yylval.error_msg = "Unterminated string constant";
            return ERROR;}
<STRING>\\n   {add_to_string('\n'); }
<STRING>\\t   {add_to_string('\t'); }
<STRING>\\r   {add_to_string('\r'); }
<STRING>\\b   {add_to_string('\b'); }
<STRING>\\f   {add_to_string('\f'); }             
<STRING><<EOF>>   {BEGIN(INITIAL);
                  yylval.error_msg = "EOF in string";return ERROR;}
<STRING>\\\r?\n  {add_to_string(yytext[1]);++curr_lineno;}                         
<STRING>\\[^btnf\0]   {add_to_string(yytext[1]);}   
<STRING>[^\\\n\"]        {add_to_string(yytext[0]);   
                            }
  /* <STRING>[\0]      {BEGIN(INITIAL);
                    yylval.error_msg = "String contains null character";return ERROR; }  */                           
<STRING_RECOVERY>(\") {BEGIN(INITIAL);}
<STRING_RECOVERY>[^\"]* { }



  /* 1. do nothing to white spaces */
{WHITE_SPACE}     {}

  /* 2. keywords */
{CLASS}       {return CLASS;}
{ELSE}        {return ELSE;}
  /* {FALSE}       {return FALSE;} */
{FI}          {return FI;}
{IF}          {return IF;}
{IN}          {return IN;}
{INHERITS}    {return INHERITS;}
{ISVOID}      {return ISVOID;}
{LET}         {return LET;}
{LOOP}        {return LOOP;}
{POOL}        {return POOL;}
{THEN}        {return THEN;}
{WHILE}       {return WHILE;}
{CASE}        {return CASE;}
{ESAC}        {return ESAC;}
{NEW}         {return NEW;}
{OF}          {return OF;}
{NOT}         {return NOT;}
  /* {TRUE}        {return TRUE;} */

  /* 3. identifiers */
{TRUE}      {yylval.boolean = 1;
              return BOOL_CONST;}
{FALSE}      {yylval.boolean = 0;
              return BOOL_CONST;}
{INTEGER}     {//yylval.symbol = inttable.add_int(atoi(yytext));
                yylval.symbol = inttable.add_string(yytext);
              return INT_CONST;}
{TYPE_IDENTIFIER}   {yylval.symbol = idtable.add_string(yytext);
              return TYPEID;}
{OBJECT_IDENTIFIER} {yylval.symbol = idtable.add_string(yytext);
              return OBJECTID;}

  /* 6. operators */
"{"       { return (int)yytext[0]; }
"}"       { return (int)yytext[0]; }
"("       { return (int)yytext[0]; }
")"       { return (int)yytext[0]; }
";"       { return (int)yytext[0]; }
":"       { return (int)yytext[0]; }
"@"       { return (int)yytext[0]; }
","       { return (int)yytext[0]; }

"<-"      { return ASSIGN; }
"<="      { return LE; }
"=>"      { return DARROW; }

"+"       { return (int)yytext[0]; }
"-"       { return (int)yytext[0]; }
"*"       { return (int)yytext[0]; }
"/"       { return (int)yytext[0]; }
"="       { return (int)yytext[0]; }
"<"       { return (int)yytext[0]; }
"."       { return (int)yytext[0]; }
"~"       { return (int)yytext[0]; }

. { yylval.error_msg = yytext;
    return ERROR;
}

%%
