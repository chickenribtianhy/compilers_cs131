# generate y.tab.c
yacc -d calc.y
# generate lex.yy.c
lex calc.l
gcc lex.yy.c y.tab.c -o calc
./calc