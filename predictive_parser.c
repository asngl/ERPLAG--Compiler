#ifndef _PREDICTIVE_PARSER
#define _PREDICTIVE_PARSER
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "predictive_parser.h"
#include "stack.h"
#define STACK_CAPACITY 1000
#define MAX_RHS_LENGTH 100
int panicFlag;
struct ParseTreeNode *tree;
struct ParseTreeNode *currNode;
struct ParseTreeNode *endNode;
struct STACK *stack;

char *filename="Test1/t2.txt";
char *grammarFilename="grammar.txt";

struct ParseTreeNode *newTNode(enum Terminals terminal)
{
	struct ParseTreeNode *tree;
	tree=(struct ParseTreeNode *)malloc(sizeof(struct ParseTreeNode));
	tree->leftChild=NULL;
	tree->rightSibling=NULL;
	tree->parent=NULL;
	tree->s.tag=0;
	tree->s.symbol.T=terminal;
	return tree;
}
struct ParseTreeNode *newNTNode(enum NonTerminals nonterminal)
{
	struct ParseTreeNode *tree;
	tree=(struct ParseTreeNode *)malloc(sizeof(struct ParseTreeNode));
	tree->leftChild=NULL;
	tree->rightSibling=NULL;
	tree->parent=NULL;
	tree->s.tag=1;
	tree->s.symbol.NT=nonterminal;
	return tree;
}
void initParser()
{
	tree=newNTNode(program);
	endNode=newTNode(FEOF);
	currNode=tree;
	
	panicFlag=0;
	initLexer("Test1/t2.txt");
	initGrammar("grammar.txt");
	stack=createStack(STACK_CAPACITY);
	
	struct stackItem bottomOfStack;
	bottomOfStack.s.tag=0;
	bottomOfStack.s.symbol.T=FEOF;

	struct stackItem startSymbol;
	startSymbol.s.tag=1;
	startSymbol.s.symbol.NT=program;
	startSymbol.ptn=tree;

	push(stack,bottomOfStack);
	push(stack,startSymbol);
}

struct ParseTreeNode * createNodes(struct STACK *stack,struct ParseTreeNode* parentNode,struct rhsnode *head)
{   
	struct STACK *stemp=createStack(100);
	struct rhsnode *temp=head;
	while(temp!=NULL)
	{	
		struct stackItem item;
		struct ParseTreeNode *node;
		if(temp->tag==terminal)
		{
			node=newTNode(temp->s.T);
			item.ptn = node;
			item.s.tag = 0;
			item.s.symbol.T = temp->s.T;
		}
		else
		{
			node=newNTNode(temp->s.NT);
			item.ptn = node;
			item.s.tag = 1;
			item.s.symbol.NT = temp->s.NT;
		}
		node->parent=parentNode;
		push(stemp,item);
		temp=temp->next;
	}
	struct stackItem left;
	struct stackItem next;
	next.ptn = NULL;
	while(!isEmpty(stemp)){
		left=peek(stemp);
		(left.ptn)->rightSibling=next.ptn;
		next=peek(stemp);
		push(stack,next);
		pop(stemp);

	}
	free(stemp->array);
	free(stemp);
	return next.ptn;
}

void pushReverse(struct STACK *stack,struct rhsnode *head)
{
	if(head==NULL)return;
	pushReverse(stack,head->next);
	struct stackItem item;
	if(head->tag==terminal)
	{
		item.s.tag=0;
		item.s.symbol.T=head->s.T;
	}
	else
	{
		item.s.tag=1;
		item.s.symbol.NT=head->s.NT;
	}
	push(stack,item);
}
int main()
{
	//initGrammar(grammarFilename);
	initParser();
	printf("HI");
	int readNextTokenFlag=1;
	struct TOKEN_INFO token_info;
	while(1)
	{
		if(readNextTokenFlag==1)
		{
			token_info=getNextToken();
			//printf("TOKEN READ IS %d\n",token_info.token);
			readNextTokenFlag=0;
		}
		struct stackItem topOfStack=peek(stack);
		enum Terminals readTerminal=token_info.token;
		printf("NEW FRAME : %s",token_info.lexeme);
		if(topOfStack.s.tag==2)
		{
			//printf("Here %d",stack->top);
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
				readNextTokenFlag=1;
				if(currNode->rightSibling==NULL)
				{
					while(currNode->rightSibling==NULL)
						currNode=currNode->parent;
					currNode=currNode->rightSibling;
				}
				else
					currNode=currNode->rightSibling;
				continue;
			}
			else
			{
				//MAPPING TABLE NEEDED???
				printf("1:Encountered unexpected token while parsing.\n %d\t%s\t%s",token_info.lineno,"",token_info.lexeme);
				readNextTokenFlag=1;
				continue;
			}
		}
		else if(topOfStack.s.tag==1)// IF top of stack is a non terminal
		{
			enum NonTerminals stackTopNonTerminal=topOfStack.s.symbol.NT;
			printf("\n Trying to access parseTable %d %d \n",stackTopNonTerminal,readTerminal);
			int parsingTableEntry=parseTable[stackTopNonTerminal][readTerminal];
			printf("\n ACCESSED %d \n ",parsingTableEntry);
			if(parsingTableEntry==-1)
			{
				// HANDLE ERROR
				printf("2:Encountered unexpected token while parsing.\n %d\t%s\t%s ....\n",token_info.lineno,"",token_info.lexeme);
				readNextTokenFlag=1;
				continue;
			}
			else if(parsingTableEntry==-2)
			{
				// HANDLE SYN
				printf("3:Encountered unexpected token while parsing.\n %d\t%s\t%s",token_info.lineno,"",token_info.lexeme);
				pop(stack);
				if(currNode->rightSibling==NULL)
				{
					while(currNode->rightSibling==NULL)
						currNode=currNode->parent;
					currNode=currNode->rightSibling;
				}
				else
					currNode=currNode->rightSibling;
			}
			else
			{
				struct cell Rule=grammarRules[parsingTableEntry];
				enum NonTerminals LHS=Rule.sym;
				struct rhsnode *RHS=Rule.head;
				pop(stack);
				currNode->leftChild = createNodes(stack,currNode,RHS);
				currNode=currNode->leftChild;
				continue;
			}
		}
		else
		{
			printf("4:Stack returned invalid item: Error code 53");
			exit(0);
		}
		// Everything is OK. Proceed with parsing.
	}
}

#endif
