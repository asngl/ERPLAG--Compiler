#ifndef _STACKH
#define _STACKH
#include "predictive_parser.h"
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
extern struct STACK *createStack(int capacity);
extern int isEmpty(struct STACK *s);
extern int push(struct STACK *s, struct stackItem item);
extern struct stackItem pop(struct STACK *s);
extern struct stackItem peek(struct STACK *s);
#endif
