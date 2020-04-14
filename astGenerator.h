/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _ASTH
#define _ASTH
#include "parserDef.h"
#include "ASTNodeDef.h"
extern struct ASTNode *createAST(struct ParseTreeNode *);
extern void printInlineAstTree(struct ASTNode *,int); 
extern void countASTNodes(struct ASTNode *,int *);
#endif

