#ifndef _STACKH
#define _STACKH
struct stackItem{
	struct TaggedSymbol s;
};
struct STACK{
	int top;
	int capacity;
	struct stackItem * array; 
};

extern struct STACK createStack(int capacity);
extern int isEmpty(struct STACK s);
extern int push(struct STACK s, struct stackItem item);
extern struct stackItem pop(struct STACK s);
extern struct stackItem peek(struct STACK s);

#endif