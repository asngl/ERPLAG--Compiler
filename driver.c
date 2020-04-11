/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/

#ifndef _DRIVER
#define _DRIVER
#include "lexerDef.h"
#include "lexer.h"
#include "grammar_Init.h" 
#include "grammar_InitDef.h"
#include "parserDef.h"
#include "parser.h"
#include "ASTNodeDef.h"
#include <time.h>
#include "symbolTableDef.h"
#include "symbolTable.h"
#include "typeChecker.h"

struct ParseTreeNode *parseInputSourceCode(char *);
void printParseTree(struct ParseTreeNode * root, char * outfile);
void printInlineParseTree(struct ParseTreeNode * root, int spaces);
struct ASTNode *createAST(struct ParseTreeNode *root);
void printInlineAstTree(struct ASTNode *root, int spaces);
//Main Function
int main(int argc, char *argv[])
{	
	initLexer(argv[1]);
	int input;
	int flag=1;
	char *outfile="Clean_code.txt";
	FILE *fp;
	char c;
	Context context;
	SymbolTable *mainTable;
	struct ParseTreeNode *root;
	struct ASTNode *ASTroot;
	clock_t start_time, end_time;
	double total_CPU_time, total_CPU_time_in_seconds;	
	struct TOKEN_INFO token_info;		    
	printf("Implementation Details:-\n");
	printf("(a) The First and Follow sets generation is automated\n");
	printf("(b) Both lexical and syntactical modules are implemented in entirety\n");
	printf("(c) All testcases parsed correctly.\n");
	printf("(d) Parse Tree constructed successfully for all testcases.\n");
	printf("(e) Error Detection and Recovery done successfully.\n");
	while(flag){
		printf("Choose option: ");
		scanf("%d",&input);
		//printf("\n%d\n",input);
		switch(input)
		{
			case 0:
				//Option to exit the program
				flag=0;
				break;
			case 1:
				//Option to Remove comments from our Input File
				removeComments(argv[1],outfile);
				fp=fopen(outfile,"r");
				c=fgetc(fp);
				while(c!=EOF){
					printf("%c",c);
					c=fgetc(fp);
				}
				printf("\n");
				fclose(fp);
				break;
			case 2:
				//Option to Print tokens in our input file according to given specifications and print lexical errors in case there is one
				initLexer(argv[1]);
				initGrammar("grammar.txt");
				while(1)
				{	
					token_info=getNextToken();
					if(token_info.token==EPS)continue;
					printf("\t%d\t%s\t\t%s\n",token_info.lineno,token_info.lexeme,mapping[token_info.token].str);
					if(token_info.token==FEOF)break;
				}
				break;
			case 3:
				//Option to output parse tree in output file and print errors on the screen
				root=parseInputSourceCode(argv[1]);
				//printInlineParseTree(root,0);
				//printf("\n Printing AST: \n");
				ASTroot=createAST(root);
				printInlineAstTree(ASTroot,0);
				mainTable=populateSymbolTable(ASTroot);
				//printSymbolTable(mainTable);
				context.symbolTable=mainTable;
				secondPass(ASTroot,context);
				
				break;
			case 4:
				//Option to check the amount of time taken by compiler to process the parse tree
			    start_time = clock();
			    root=parseInputSourceCode(argv[1]);
				end_time = clock();
			    
			    total_CPU_time  =  (double) (end_time - start_time);
			    total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;

			    // Print both total_CPU_time and total_CPU_time_in_seconds 
			    printf("Total CPU time = %F\n",total_CPU_time);
				printf("Total CPU time in seconds = %F\n", total_CPU_time_in_seconds);
				break;
			default:
				break;
		}
	}
}
#endif
