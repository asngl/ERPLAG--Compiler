#ifndef _PARSERDEFH
#define _PARSERDEFH
#include "grammar_InitDef.h"

struct ParseTreeNode{
	struct TaggedSymbol s;
	struct ParseTreeNode *leftChild;// Leftmost child
	struct ParseTreeNode *rightSibling;
	struct ParseTreeNode *parent;
};

#endif
