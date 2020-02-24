#ifndef _DRIVER
#define _DRIVER
#include "lexerDef.h"
#include "lexer.h"
#include "grammar_Init.h" 
#include "grammar_InitDef.h"
#include "parserDef.h"
#include "parser.h"
#include <time.h>

struct ParseTreeNode *getParseTree(char *);

int main(int argc, char *argv[])
{	
	initLexer(argv[1]);
	int input;
	int flag=1;
	char *outfile="Clean_code.txt";
	FILE *fp;
	char c;
	struct ParseTreeNode *root;
	clock_t start_time, end_time;
	double total_CPU_time, total_CPU_time_in_seconds;	
	struct TOKEN_INFO token_info;		    
	printf("Implementation Details:-\n");
	printf("(a) The First and Follow sets generation is automated\n");
	printf("(b) Both lexical and syntactical modules are implemented in entirety\n");
	while(flag){
		printf("Choose option: ");
		scanf("%d",&input);
		//printf("\n%d\n",input);
		switch(input)
		{
			case 0:
				flag=0;
				break;
			case 1:
				//printf("Processing Comments...\n");
				removeComments(argv[1],outfile);
				//printf("Processed.\n");
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
				root=getParseTree(argv[1]);
				printf("PRINT PARSE TREE\n");
				//printFullParseTree(root,argv[2]);
				break;
			case 4:
				
			    start_time = clock();
			    getParseTree(argv[1]);
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
