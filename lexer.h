/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
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