#ifndef _STACK
#define _STACK
#include <stdlib.h>
#include "stack.h"
#include "parser.h"

struct stackItem ERR_stackItem;

struct STACK createStack(int capacity){
	struct STACK s;
	s.top = -1;
	s.capacity = capacity;
	s.array = (struct stackItem *)malloc(sizeof(struct stackItem *));
	ERR_stackItem.s.tag=2;
	return s;
}

int isEmpty(struct STACK s){
	if(s.top == -1) return 1;
	return 0;
}

int push(struct STACK s, struct stackItem item) //0 for success and 1 for failure
{ 
    if (s.top+1 >= s.capacity) 
        return 1; 
    s.array[++s.top] = item;
    return 0; 
    //printf("%d pushed to stack\n", item); 
} 
  
// Function to remove an item from stack.  It decreases top by 1 
struct stackItem pop(struct STACK s) 
{   
    if (isEmpty(s)) 
        return ERR_stackItem; //return stackItem error 
    return s.array[s.top--]; 
} 
  
// Function to return the top from stack without removing it 
struct stackItem peek(struct STACK s) 
{ 
    if (isEmpty(s)) 
        return ERR_stackItem; 
    return s.array[s.top]; 
} 


#endif