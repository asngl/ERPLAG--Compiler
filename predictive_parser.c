#ifndef _PREDICTIVE_PARSER
#define _PREDICTIVE_PARSER
#include "lexer.h"
#include "parser.h"
#include "stack.h"
#define STACK_CAPACITY 1000

int panicFlag;
void initParser()
{
	panicFlag=0;
	initLexer(filename);
	struct STACK stack=createStack(STACK_CAPACITY);
	struct StackItem bottomOfStack;
	bottomOfStack.s.tag=0;
	bottomOfStack.s.symbol=FEOF;
	struct StackItem startSymbol;
	startSymbol.s.tag=1;
	startSymbol.s.symbol=program;
	push(stack,bottomOfStack);
	push(stack,startSymbol);
}
int main()
{
	char *filename="Test1/t2.txt";
	initParser();
	while(1)
	{
		struct TOKEN_INFO token_info=getNextToken();
		printf("TOKEN READ IS %d\n",token_info.token);

		struct stackItem topOfStack=peek(stack);
		enum Terminals readTerminal=token_info.token;

		if(topOfStack.s.tag==2)
		{
			printf("Invalid operation on stack: Exit with error code 1");
			exit(0);
		}
		if(readTerminal==FEOF)
		{
			if(topOfStack.s.tag==0 && topOfStack.s.symbol.T == FEOF)
			{
				printf("Finished Parsing Successfully.");
				break;
			}
			else
			{
				printf("Encountered end of file while parsing: Exit with error code 2");
				exit(0);
			}
		}

		if(topOfStack.s.tag==0)// IF top of stack is a terminal
		{
			enum Terminals stackTopTerminal=topOfStack.s.symbol.T;
			if(readTerminal==stackTopTerminal)
			{
				pop(stack);
				continue;
			}
			else
			{
				// PANICCCCCCC ???????? O-O
			}
		}
		else if(topOfStack.s.tag==1)
		{
			enum NonTerminals stackTopNonTerminal=topOfStack.s.symbol.NT;
			int parsingTableEntry=parseTable[stackTopNonTerminal][readTerminal];
			if(parsingTableEntry==-1)
			{
				// HANDLE ERROR
			}
			else if(parsingTableEntry==-2)
			{
				// HANDLE SYN
			}
			else
			{
				struct cell Rule=grammarRules[parsingTableEntry];
			}
		}
		else
		{
			printf("Stack returned invalid item: Error code 53");
			exit(0);
		}
		// Everything is OK. Proceed with parsing.
	}
}

#endif