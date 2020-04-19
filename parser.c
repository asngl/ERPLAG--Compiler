/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
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
	if(root->errorFlag==1)
		fprintf(fp,"%20s\t","ERROR");
	else if(isLeafNode && !isEPS ) { //leaf
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
		fprintf(fp,"%18s\t",mapping[root->s.symbol.NT+NUM_OF_TERMINALS].str);


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
		fprintf(fp,"%18s\t",mapping[root->parent->s.symbol.NT+NUM_OF_TERMINALS].str);


	//isLeafNode
	if(isLeafNode) 
		fprintf(fp,"%8s\t","yes");
	else
		fprintf(fp,"%8s\t","no");


	//nodeSymbol
	if(isLeafNode) 
		fprintf(fp,"%18s\t","----");
	else
		fprintf(fp,"%18s\t",mapping[root->s.symbol.NT+NUM_OF_TERMINALS].str);


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
void printParseTree(struct ParseTreeNode * root, char * outfile){
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






//Function to generate new node for given Terminal
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
	node->ruleNumber = -1;
	return node;
}

//Function to generate new node for given Non Terminal
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
	node->ruleNumber = -1;
	return node;
}

//Function for initialising parser
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


// Expands a non-terminal using a rule and generates a list of children nodes and returns the head pointer of the list
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


//Pushes the given grammar rule in reverse order in the stack
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

//Utility function to print current state of stack
void printStack(struct STACK *s)
{
    printf("Stack State:");
    for(int i=0;i<=s->top;i++)
    {
    	if(s->array[i].s.tag==1)
        	printf(" %s,",mapping[s->array[i].s.symbol.NT+NUM_OF_TERMINALS].str);
        else
        	printf(" %s,",mapping[s->array[i].s.symbol.T].str);
    }
    printf("\n");
}

//Utility Function
void printSpaces(int n)
{
	while(n--)printf("--");
}
void printInlineParseTree(struct ParseTreeNode *root,int spaces)
{
	if(root==NULL)return;
	struct ParseTreeNode *node=root->leftChild;
	printSpaces(spaces);
	if(root->s.tag==0)
		printf("%s, Rule=%d\n",mapping[root->s.symbol.T].str, root->ruleNumber);
	else
		printf("%s, Rule=%d\n",mapping[root->s.symbol.NT+NUM_OF_TERMINALS].str, root->ruleNumber);
	while(node!=NULL)
	{
		printInlineParseTree(node,spaces+1);
		node=node->rightSibling;
	}
}


void countParseNodes(struct ParseTreeNode *root, int *count)
{
	if(root==NULL) return;
	*count+=1;
	struct ParseTreeNode *ptr=root->leftChild;
	while(ptr!=NULL)
	{
		countParseNodes(ptr,count);
		ptr=ptr->rightSibling;
	}
}


//Returns the generated parse tree from given testFile as a structure
struct ParseTreeNode *parseInputSourceCode(char *testFile)
{
	struct TOKEN_INFO emptyToken;
	filename=testFile;
	initParser();
	int debugMode=0;
	int readNextTokenFlag=1;
	struct TOKEN_INFO token_info;
	int count=0;
	while(1)
	{	
		//Reads the next valid token
		if(readNextTokenFlag==1)
		{
			token_info=getNextToken();
			while(token_info.token==EPS||token_info.token==COMMENTMARK)
				token_info=getNextToken();
			readNextTokenFlag=0;
		}
		struct stackItem topOfStack=peek(stack);
		enum Terminals readTerminal=token_info.token;
		if(topOfStack.s.tag==0 && topOfStack.s.symbol.T==EPS) //If there is a EPS on top of stack, pop stack
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
				printf("\nNEW FRAME :%s %s\n  ",mapping[topOfStack.s.symbol.NT+NUM_OF_TERMINALS].str,mapping[token_info.token].str);
			else
				printf("\nNEW FRAME :%s %s\n  ",mapping[topOfStack.s.symbol.T].str,mapping[token_info.token].str);
			printStack(stack);
		}
		

		if(readTerminal==FEOF)		//If we encounter End of file in input stream
		{
			if(topOfStack.s.tag==0 && topOfStack.s.symbol.T == FEOF)
			{
				printf("Finished Parsing Successfully.\n");
				break;
			}
			else if((topOfStack.s.tag==1 && parseTable[topOfStack.s.symbol.NT][readTerminal]<0)||(topOfStack.s.tag==0)) //If top of stack is not Nullable
			{
				ERROR_FLAG=1;
				pop(stack);
				currNode->errorFlag=1;
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
			ERROR_FLAG=1;
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
				pop(stack);
				currNode->errorFlag=1;
				if(currNode->rightSibling==NULL)
				{
					while(currNode->rightSibling==NULL)
						currNode=currNode->parent;
					currNode=currNode->rightSibling;
				}
				else
					currNode=currNode->rightSibling;
				printf("SYNTAX ERROR:Encountered unexpected token while parsing.\n\tLINE NUMBER %d: %s\n",token_info.lineno,token_info.lexeme);
				ERROR_FLAG=1;
				readNextTokenFlag=0;
				continue;
			}
		}
		else if(topOfStack.s.tag==1)// IF top of stack is a non terminal
		{
			enum NonTerminals stackTopNonTerminal=topOfStack.s.symbol.NT;
			int parsingTableEntry=parseTable[stackTopNonTerminal][readTerminal];
			if(parsingTableEntry==-1)
			{
				// HANDLE ERROR
				printf("SYNTAX ERROR:Encountered unexpected token while parsing.\n\tLINE NUMBER %d: %s\n",token_info.lineno,token_info.lexeme);
				ERROR_FLAG=1;
				readNextTokenFlag=1;
				continue;
			}
			else if(parsingTableEntry==-2)
			{
				// HANDLE SYN
				if(debugMode)
					printf("Trying to access parseTable:: %s,%s -> USE RULE %d\n",mapping[stackTopNonTerminal+NUM_OF_TERMINALS].str,mapping[readTerminal].str,parsingTableEntry+1);
				printf("SYNTAX ERROR:Encountered unexpected token while parsing.\n\tLINE NUMBER %d: %s\n",token_info.lineno,token_info.lexeme);
				ERROR_FLAG=1;
				pop(stack);
				currNode->errorFlag=1;
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
				currNode->ruleNumber = parsingTableEntry; 	//parsingTableEntry is 0 indexed
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
