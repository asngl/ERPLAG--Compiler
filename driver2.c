#ifndef _DRIVER
#define _DRIVER
#include "lexerDef.h"
#include "lexer.h"
#include "grammar_Init.h" 
#include "grammar_InitDef.h"
#include "parser.h"
#include "parserDef.h"
#include "astGenerator.h"
#include "symbolTable.h"
#include "typeChecker.h"
#include "codeGenerator.h"
#include <stdlib.h>
#include <time.h>

int ERROR_FLAG=0;

int main(int argc, char *argv[])
{	
	printf("\nLEVEL 4 : Symbol table, type Checking, Semantic rules modules work, handled static and dynamic arrays in type checking and code generation along with floating point operations implemented sucessfully\n\n");
	
	struct TOKEN_INFO token_info;
	struct ParseTreeNode *root;
	struct ASTNode *AST_root;
	SymbolTable *mainTable;
	
	root=parseInputSourceCode(argv[1]);
	int stage1_errorflag=ERROR_FLAG;
	if(!stage1_errorflag)
	{
		AST_root=createAST(root);
		mainTable=populateSymbolTable(AST_root);
	}
	if(!ERROR_FLAG)
	{
		printf("\nCode compiles succesfully...\n");
	}
	
	int input, flag=1;
	int parseNodes=0, astNodes=0;
	int parseMem, astMem;
	clock_t start_time, end_time;
	double total_CPU_time, total_CPU_time_in_seconds;
	while(flag){
		printf("Choose option: ");
		scanf("%d",&input);
		switch(input)
		{
			case 0:
				//Option to exit the program
				flag=0;
				break;
			case 1:
				//Print tokens as generated by lexer
				initLexer(argv[1]);
				initGrammar("grammar.txt");
				while(1)
				{	
					token_info=getNextToken();
					if(token_info.token==EPS)	continue;
					printf("\t%-5d %-25s%s\n",token_info.lineno,token_info.lexeme,mapping[token_info.token].str);
					if(token_info.token==FEOF)	break;
				}
				break;
			case 2:
				//Verify syntactic correctness and print parse tree
				printInlineParseTree(root,1);
				break;
			case 3:
				//Print AST
				if(stage1_errorflag)
				{
					printf("Lexical/Syntactical errors present---Invalid option");
					break; 
				}
				printf("The Abstract Syntax Tree is traversed inorder\n");
				printInlineAstTree(AST_root,1);
				break;
			case 4:
				//Allocated Memory
				if(stage1_errorflag)
				{
					printf("Lexical/Syntactical errors present---Invalid option");
					break; 
				}
				countParseNodes(root,&parseNodes);
				countASTNodes(AST_root,&astNodes);
				parseMem=parseNodes*sizeof(struct ParseTreeNode);				    
				astMem=astNodes*sizeof(struct ASTNode);
				printf("Parse tree Number of nodes=%d\n",parseNodes);
				printf("Allocated Memory=%d Bytes\n",parseMem);
				printf("AST Number of nodes=%d\n",astNodes);
				printf("Allocated Memory=%d Bytes\n",astMem);
				printf("Compression percentage=%f\n",((float)(parseMem-astMem)*100)/parseMem);
				break;
			case 5:
				//populate Symbol Table and print
				if(stage1_errorflag)
				{
					printf("Lexical/Syntactical errors present---Invalid option");
					break; 
				}
				printSymbolTable(mainTable);
				break;
			case 6:
				//Print activation records and their width
				if(stage1_errorflag)
				{
					printf("Lexical/Syntactical errors present---Invalid option");
					break; 
				}
				printRecordWidth(mainTable);	
				break;
			case 7:
				//Print array variables and their info
				if(stage1_errorflag)
				{
					printf("Lexical/Syntactical errors present---Invalid option");
					break; 
				}
				printArrayVariables(mainTable);
				break;
			case 8:
				// Print both total_CPU_time and total_CPU_time_in_seconds 
				start_time = clock();
			    	root=parseInputSourceCode(argv[1]);
				if(!stage1_errorflag)
				{	
					AST_root=createAST(root);
					mainTable=populateSymbolTable(AST_root);
				}
				end_time = clock();
			    
			   	total_CPU_time  =  (double) (end_time - start_time);
			    	total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;

			  	printf("\nTotal CPU time = %F\n",total_CPU_time);
				printf("Total CPU time in seconds = %F\n", total_CPU_time_in_seconds);
				break;
			case 9:
				//Generate ASM code
				generateProgramCode(AST_root,argv[2]);
				printf("Code generated.\nRun with:\n");
				printf("	nasm -felf64 %s -o out.o && gcc -no-pie out.o && ./a.out\n",argv[2]);
			default:
				break;
		}
		printf("\n\n\n");
	}
}
#endif
