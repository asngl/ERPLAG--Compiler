#ifndef macrolexerh
#define macrolexerh
#include "grammar_InitDef.h"
#include <stdio.h>

extern void initLexer(char inputFile[]);
extern FILE *getStream(FILE *fp);
extern struct TOKEN_INFO getNextToken();
extern void removeComments(char *testcaseFile, char *cleanFile);
#endif

//Declaration of functions used in lexer.c