#ifndef _PREDICTIVE_PARSER
#define _PREDICTIVE_PARSER
#include <stdlib.h>
#include "lexerDef.h"
#include "lexer.h"
#include "grammar_InitDef.h"
#include "grammar_Init.h"
#include "parser.h"
#include "parserDef.h"
#include "stack.h"
#include "stackDef.h"
#define STACK_CAPACITY 1000
#define MAX_RHS_LENGTH 100

struct ParseTreeNode{
	struct TaggedSymbol s;
	struct ParseTreeNode *leftChild;// Leftmost child
	struct ParseTreeNode *rightSibling;
	struct ParseTreeNode *parent;
	struct TOKEN_INFO token_info;
	int errorFlag;
};
//Structure used for Parse Tree creation


struct ParseTreeNode *getParseTree(char *filename);
//Function name for processing parse tree of given testFile


int panicFlag;
struct ParseTreeNode *tree;
struct ParseTreeNode *currNode;
struct ParseTreeNode *endNode;
struct STACK *stack;

char *filename="t6.txt";
char *grammarFilename="grammar.txt";
//Default values of filename and grammarFilename in case it is not provided

//Prints a particular node in the inorder travesal
void printNode(struct ParseTreeNode * root, FILE * fp){
	
	int isLeafNode=1-root->s.tag;
	int isEPS=(root->s.tag==0)&&(root->s.symbol.T==EPS);
	if(isLeafNode && !isEPS ) { //leaf
		fprintf(fp,"%20s\t",root->token_info.lexeme);
	}
	else fprintf(fp,"%20s\t","----");


	//lineno
	if(isLeafNode && !isEPS )
		fprintf(fp,"%5d\t",root->token_info.lineno);
	else fprintf(fp,"%5s\t","----");

	//tokenName
	if(root->s.tag == 0)//terminal
		fprintf(fp,"%18s\t",mapping[root->s.symbol.T].str);
	else //non terminal
		fprintf(fp,"%18s\t",mapping[root->s.symbol.NT+63].str);


	//valueIfNumber
	if(isLeafNode )
	{
		if(root->token_info.token == NUM) //num
			fprintf(fp,"%8d\t",root->token_info.value.value.num);		
		else if(root->token_info.token == RNUM) //rnum
			fprintf(fp,"%8f\t",root->token_info.value.value.rnum);
		else
			fprintf(fp,"%8s\t","----");
	}
	else //undefined
		fprintf(fp,"%8s\t","----");


	//parentNodeSymbol
	if(root->parent == NULL) 
		fprintf(fp,"%18s\t","ROOT");
	else 
		fprintf(fp,"%18s\t",mapping[root->parent->s.symbol.NT+63].str);


	//isLeafNode
	if(isLeafNode) 
		fprintf(fp,"%8s\t","yes");
	else
		fprintf(fp,"%8s\t","no");


	//nodeSymbol
	if(isLeafNode) 
		fprintf(fp,"%18s\t","----");
	else
		fprintf(fp,"%18s\t",mapping[root->s.symbol.NT+63].str);


	fprintf(fp,"\n");
}


//Inorder travesal of the parse tree
void inorder(struct ParseTreeNode * root, FILE * fp){
	if(root == NULL) return;
	inorder(root->leftChild,fp);
	printNode(root,fp);
	if(root->leftChild == NULL) return;
	struct ParseTreeNode * temp = root->leftChild->rightSibling;
	while(temp != NULL){
		inorder(temp,fp);
		temp = temp->rightSibling;
	}
}

//Main function for printing Parse tree in output file
void printFullParseTree(struct ParseTreeNode * root, char * outfile){
FILE * fp = fopen(outfile,"w");
fprintf(fp,"%20s\t","lexeme");
fprintf(fp,"%5s\t","line");
fprintf(fp,"%18s\t","tokenName");
fprintf(fp,"%8s\t","value");
fprintf(fp,"%18s\t","parentSymbol");
fprintf(fp,"%8s\t","isLeaf");
fprintf(fp,"%18s\t","NodeSymbol");
fprintf(fp,"\n\n");
inorder(root,fp);
fclose(fp);
}





struct ParseTreeNode *newTNode(enum Terminals terminal)
{
	struct ParseTreeNode *node;
	node=(struct ParseTreeNode *)malloc(sizeof(struct ParseTreeNode));
	node->leftChild=NULL;
	node->rightSibling=NULL;
	node->parent=NULL;
	node->s.tag=0;
	node->s.symbol.T=terminal;
	node->errorFlag=0;
	return node;
}
struct ParseTreeNode *newNTNode(enum NonTerminals nonterminal)
{
	struct ParseTreeNode *node;
	node=(struct ParseTreeNode *)malloc(sizeof(struct ParseTreeNode));
	node->leftChild=NULL;
	node->rightSibling=NULL;
	node->parent=NULL;
	node->s.tag=1;
	node->s.symbol.NT=nonterminal;
	node->errorFlag=0;
	return node;
}
void initParser()
{
	tree=newNTNode(program);
	endNode=newTNode(FEOF);
	currNode=tree;
	
	panicFlag=0;
	initLexer(filename);
	initGrammar(grammarFilename);
	stack=createStack(STACK_CAPACITY);
	
	struct stackItem bottomOfStack;
	bottomOfStack.s.tag=0;
	bottomOfStack.s.symbol.T=FEOF;

	struct stackItem startSymbol;
	startSymbol.s.tag=1;
	startSymbol.s.symbol.NT=program;
	startSymbol.ptn=tree;
	startSymbol.ptn->rightSibling=endNode;
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
void printStack(struct STACK *s)
{
    printf("Stack State:");
    for(int i=0;i<=s->top;i++)
    {
    	if(s->array[i].s.tag==1)
        	printf(" %s,",mapping[s->array[i].s.symbol.NT+63].str);
        else
        	printf(" %s,",mapping[s->array[i].s.symbol.NT].str);
    }
    printf("\n");
}
void printSpaces(int n)
{
	while(n--)printf("--");
}
void printParseTree(struct ParseTreeNode *root,int spaces)
{
	if(root==NULL)return;
	struct ParseTreeNode *node=root->leftChild;
	printSpaces(spaces);
	if(root->s.tag==0)
		printf("%s\n",mapping[root->s.symbol.T].str);
	else
		printf("%s\n",mapping[root->s.symbol.NT+63].str);
	while(node!=NULL)
	{
		printParseTree(node,spaces+1);
		node=node->rightSibling;
	}
}
struct ParseTreeNode *getParseTree(char *testFile)
{
	//initGrammar(grammarFilename);
	struct TOKEN_INFO emptyToken;
	//emptyToken->;
	filename=testFile;
	initParser();
	//printf("HI");
	int debugMode=0;
	int readNextTokenFlag=1;
	struct TOKEN_INFO token_info;
	int count=0;
	while(1)
	{
		//if(count++==200)break;
		if(readNextTokenFlag==1)
		{
			token_info=getNextToken();
			while(token_info.token==EPS||token_info.token==COMMENTMARK)
				token_info=getNextToken();
			//printf("TOKEN READ IS %d\n",token_info.token);
			readNextTokenFlag=0;
		}
		struct stackItem topOfStack=peek(stack);
		enum Terminals readTerminal=token_info.token;
		if(topOfStack.s.tag==0 && topOfStack.s.symbol.T==EPS)
		{
			if(debugMode)
			{
				printf("\nNEW FRAME :%s %s\n  ","EPS",mapping[token_info.token].str);
				printStack(stack);
			}
			currNode->token_info.lexeme[0]='\0';
			pop(stack);
			while(currNode->rightSibling==NULL)
				currNode=currNode->parent;
			currNode=currNode->rightSibling;
			continue;
		}
		if(debugMode)
		{
			if(topOfStack.s.tag==1)
				printf("\nNEW FRAME :%s %s\n  ",mapping[topOfStack.s.symbol.NT+63].str,mapping[token_info.token].str);
			else
				printf("\nNEW FRAME :%s %s\n  ",mapping[topOfStack.s.symbol.T].str,mapping[token_info.token].str);
			printStack(stack);
		}
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
				printf("Finished Parsing Successfully.\n");
				break;
			}
			else if((topOfStack.s.tag==1 && parseTable[topOfStack.s.symbol.NT][readTerminal]<0)||(topOfStack.s.tag==0))
			{
				pop(stack);
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
		}
		if(topOfStack.s.tag==0 && topOfStack.s.symbol.T == FEOF)
		{
			
			printf("SYNTAX ERROR:Encountered invalid token at end of file\n");
			break;
			//exit(0);
		}
		if(topOfStack.s.tag==0)// IF top of stack is a terminal
		{
			enum Terminals stackTopTerminal=topOfStack.s.symbol.T;
			//printf("%s %s %s\n",);
			if(readTerminal==stackTopTerminal)
			{
				currNode->token_info=token_info;
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
				pop(stack);
				if(currNode->rightSibling==NULL)
				{
					while(currNode->rightSibling==NULL)
						currNode=currNode->parent;
					currNode=currNode->rightSibling;
				}
				else
					currNode=currNode->rightSibling;
				printf("SYNTAX ERROR1:Encountered unexpected token while parsing.\n\tLINE NUMBER %d: %s\n",token_info.lineno,token_info.lexeme);
				readNextTokenFlag=0;
				continue;
			}
		}
		else if(topOfStack.s.tag==1)// IF top of stack is a non terminal
		{
			enum NonTerminals stackTopNonTerminal=topOfStack.s.symbol.NT;
			int parsingTableEntry=parseTable[stackTopNonTerminal][readTerminal];
			//printf("    Trying to access parseTable:: %s,%s -> USE RULE %d\n",mapping[stackTopNonTerminal+63].str,mapping[readTerminal].str,parsingTableEntry+1);
			if(parsingTableEntry==-1)
			{
				// HANDLE ERROR
				printf("SYNTAX ERROR2:Encountered unexpected token while parsing.\n\tLINE NUMBER %d: %s\n",token_info.lineno,token_info.lexeme);
				readNextTokenFlag=1;
				continue;
			}
			else if(parsingTableEntry==-2)
			{
				// HANDLE SYN
				if(debugMode)
					printf("    Trying to access parseTable:: %s,%s -> USE RULE %d\n",mapping[stackTopNonTerminal+63].str,mapping[readTerminal].str,parsingTableEntry+1);
				printf("SYNTAX ERROR3:Encountered unexpected token while parsing.\n\tLINE NUMBER %d: %s\n",token_info.lineno,token_info.lexeme);
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
	tree->rightSibling=NULL;// Remove FEOF from the parse tree
	//printf("\n\t\t\t-----PARSE TREE-----\n");
	//printParseTree(tree,0);
	return tree;
}

#endif
