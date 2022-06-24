lex myscanner.l
gcc myscanner.c lex.yy.c -o myscanner
./myscanner <conf.in