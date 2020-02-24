#ifndef _STACK
#define _STACK
#include <stdlib.h>
#include <stdio.h>
#include "stackDef.h"

struct stackItem ERR_stackItem;

struct STACK *createStack(int capacity){
	struct STACK *s;
    //printf("IN0\n");
	s=(struct STACK *)malloc(sizeof(struct STACK));
    //printf("IN1\n");
	s->top = -1;
	s->capacity = capacity;
    //printf("IN2\n");
	s->array = (struct stackItem *)malloc(sizeof(struct stackItem)*capacity);
	//ERR_stackItem.s.tag=2;
	return s;
}

int isEmpty(struct STACK *s){
	if(s->top == -1) return 1;
	return 0;
}

int push(struct STACK *s, struct stackItem item) //0 for success and 1 for failure
{ 
    if (s->top+1 >= s->capacity) 
        return 1; 
    s->array[++s->top] = item;
    return 0; 
    //printf("%d pushed to stack\n", item); 
} 
  
// Function to remove an item from stack.  It decreases top by 1 
struct stackItem pop(struct STACK *s) 
{   
    if (isEmpty(s)) 
        return ERR_stackItem; //return stackItem error 
    return s->array[s->top--]; 
} 
  
// Function to return the top from stack without removing it 
struct stackItem peek(struct STACK *s) 
{ 
    if (isEmpty(s)) 
        return ERR_stackItem; 
    return s->array[s->top]; 
} 

#endif
