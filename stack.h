/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _STACKH
#define _STACKH
#include "stackDef.h"

extern struct STACK *createStack(int capacity);
extern int isEmpty(struct STACK *s);
extern int push(struct STACK *s, struct stackItem item);
extern struct stackItem pop(struct STACK *s);
extern struct stackItem peek(struct STACK *s);
#endif
//Declaration of functions used in Stack.c