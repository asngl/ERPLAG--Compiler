/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _PARSERDEFH
#define _PARSERDEFH
#include "grammar_InitDef.h"
#include "lexerDef.h"

extern int ERROR_FLAG;
struct ParseTreeNode{
	struct TaggedSymbol s;
	struct ParseTreeNode *leftChild;// Leftmost child
	struct ParseTreeNode *rightSibling;
	struct ParseTreeNode *parent;
	struct TOKEN_INFO token_info;
	int errorFlag;
	int ruleNumber;
};

#endif
//Definition of structs used in parser.c
