/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _STACK
#define _STACK
#include <stdlib.h>
#include <stdio.h>
#include "stackDef.h"

struct stackItem ERR_stackItem;

//Function to initialize stack
struct STACK *createStack(int capacity){
	struct STACK *s;
	s=(struct STACK *)malloc(sizeof(struct STACK));
	s->top = -1;
	s->capacity = capacity;
	s->array = (struct stackItem *)malloc(sizeof(struct stackItem)*capacity);
	return s;
}

//Function to check if stack is empty
int isEmpty(struct STACK *s){
	if(s->top == -1) return 1;
	return 0;
}

//Function to push item onto stack
int push(struct STACK *s, struct stackItem item) //0 for success and 1 for failure
{ 
    if (s->top+1 >= s->capacity) 
        return 1; 
    s->array[++s->top] = item;
    return 0; 
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
