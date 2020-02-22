#ifndef _PREDICTIVE_PARSER
#define _PREDICTIVE_PARSER
#include "lexer.h"
#include "parser.h"
#include "stack.h"
#define STACK_CAPACITY 1000
#define MAX_RHS_LENGTH 100
int panicFlag;
struct ParseTreeNode *tree;
struct ParseTreeNode *currNode;
struct ParseTreeNode *endNode;
struct ParseTreeNode *newTNode(enum Terminals terminal)
{
	tree=(ParseTreeNode *)malloc(sizeof(ParseTreeNode));
	tree->leftChild=NULL;
	tree->rightSibling=NULL;
	tree->parent=NULL;
	tree->s.tag=0;
	tree->s.symbol.T=terminal;
}
struct ParseTreeNode *newNTNode(enum NonTerminals nonterminal)
{
	tree=(ParseTreeNode *)malloc(sizeof(ParseTreeNode));
	tree->leftChild=NULL;
	tree->rightSibling=NULL;
	tree->parent=NULL;
	tree->s.tag=1;
	tree->s.symbol.NT=nonterminal;
}
void initParser()
{
	tree=newNTNode(program);
	endNode=newTNode(FEOF);
	currNode=tree;
	
	panicFlag=0;
	initLexer(filename);
	struct STACK stack=createStack(STACK_CAPACITY);
	
	struct StackItem bottomOfStack;
	bottomOfStack.s.tag=0;
	bottomOfStack.s.symbol=FEOF;

	struct StackItem startSymbol;
	startSymbol.s.tag=1;
	startSymbol.s.symbol=program;
	startSymbol.ptn=tree;

	push(stack,bottomOfStack);
	push(stack,startSymbol);
}

struct ParseTreeNode * createNodes(struct STACK stack,struct ParseTreeNode* parentNode,struct rhsnode *head)
{   
	struct STACK stemp=createStack(MAX_RHS_LENGTH);
	struct rhsnode *temp=head;
	while(temp!=NULL)
	{	
		struct stackItem item;
		struct ParseTreeNode *node;
		node->parent=parentNode;
		if(temp->tag==terminal)
		{
			node=newTNode(temp->symbol.T);
			item.ptn = node;
			item.s.tag = 0;
			item.s.symbol.T = temp->symbol.T;
		}
		else
		{
			node=newNTNode(temp->symbol.NT);
			item.ptn = node;
			item.s.tag = 1;
			item.s.symbol.NT = temp->symbol.NT;
		}
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
	return next.ptn;
}

void pushReverse(struct STACK stack,struct rhsnode *head)
{
	if(head==NULL)return;
	pushReverse(stack,head->next);
	struct stackItem item;
	if(head.tag==terminal)
	{
		item.s.tag=0;
		item.s.symbol.T=head.s.T;
	}
	else
	{
		item.s.tag=1;
		item.s.symbol.NT=head.s.NT;
	}
	push(stack,item);
}
int main()
{
	char *filename="Test1/t2.txt";
	initParser();
	int readNextTokenFlag=1;
	while(1)
	{
		if(readNextTokenFlag==1)
		{
			struct TOKEN_INFO token_info=getNextToken();
			printf("TOKEN READ IS %d\n",token_info.token);
			readNextTokenFlag=0;
		}
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
				readNextTokenFlag=1;
				continue;
			}
			else if(parsingTableEntry==-2)
			{
				// HANDLE SYN
			}
			else
			{
				struct cell Rule=grammarRules[parsingTableEntry];
				enum NonTerminals LHS=Rule.sym;
				struct rhsnode *RHS=Rule.rhsnode;

				pop(stack);
				currNode->leftChild = createNodes(stack,currNode,RHS);
				// pushReverse(stack,currNode,RHS);
				currNode=currNode->leftChild;
				continue;
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