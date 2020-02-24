#ifndef _STACKDEFH
#define _STACKDEFH
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