/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _STACKDEFH
#define _STACKDEFH
#include "lexerDef.h"
#include "grammar_InitDef.h"
#include "parserDef.h"

struct stackItem{
	struct TaggedSymbol s;
	struct ParseTreeNode *ptn;
};

struct STACK{
	int top;
	int capacity;
	struct stackItem * array; 
};
typedef struct STACK STRUCTSTACK;
#endif

//Definition of Strcuts used in Stack.c