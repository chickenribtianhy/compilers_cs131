%{
#include<stdio.h>
#include<stdlib.h>
int symbols[52];
int symbolVal(char symbol);
void updateSymbolVal(char symbol, int val);   
void yyerror(char *s);
int yylex();
%}

%union {int num; char id;}
%start line
%token print
%token exit_commond
%token <num> number
%token <id> identifier
%type <num> line exp term
%type <id> assignment
%%
/* declaration                          corresponding actions*/
line    : assignment ';'                {;}
        | exit_commond ';'              {exit(EXIT_SUCCESS);}
        | print exp ';'                 {printf("Printing %d\n", $2);}
        | line assignment ';'           {;}
        | line print exp ';'            {printf("Printing %d\n", $3);}
        | line exit_commond ';'         {exit(EXIT_SUCCESS);}
        ;
assignment  : identifier '=' exp        {updateSymbolVal($1, $3);}
            ;
exp     : term                          {$$ = $1;}
        | exp '+' term                  {$$ = $1 + $3;}
        | exp '-' term                  {$$ = $1 - $3;}
        ;
term    : number                        {$$ = $1;}
        | identifier                    {$$ = symbolVal($1);}
        ;
%%
int computeSymbolIndex(char token)
{
    int idx = -1;
    /* if(islower(token))
        idx = token-'a'+26;
    else if(isupper(token))
        idx = token-'A'; */
    if(token >= 'A' && token <= 'Z')
        idx = token - 'A';
    else if(token >= 'a' && token <= 'z')
        idx = token -'a'+26;
    /* printf("token = %c, idx = %d\n",token,idx); */
    return idx;
}
/* term    : identifier                    {$$ = symbolVal($1);} */
int symbolVal(char symbol)
{
    int bucket = computeSymbolIndex(symbol);
    return symbols[bucket];
}

/* assignment  : identifier '=' exp        {updateSymbolVal($1,$3);} */
void updateSymbolVal(char symbol, int val)
{
    int bucket = computeSymbolIndex(symbol);
    /* printf("%c = %d\n", symbol, val); */
    symbols[bucket] = val;
}

int main()
{
    /* init symbol table */
    int i;
    for(i = 0; i < 52; ++i)
    {
        symbols[i] = 0;
    }
    return yyparse();
}
void yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}