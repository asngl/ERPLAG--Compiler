/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _PARSERH
#define _PARSERH
#include "parserDef.h"

extern struct ParseTreeNode *parseInputSourceCode(char *);
extern void printInlineParseTree(struct ParseTreeNode *,int);
extern void countParseNodes(struct ParseTreeNode *, int *);
#endif


