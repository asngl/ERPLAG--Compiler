#ifndef _DRIVER
#define _DRIVER
#include "lexer.h"
#include <time.h>
// #ifndef macrolexerc
// #define macrolexerc
// #include "lexer.c"
// #endif
int main(int argc, char *argv[])
{	
	initLexer(argv[1]);
	int input;
	int flag=1;
	printf("Implementation Details:-\n");
	printf("(a) The First and Follow sets generation is automated\n");
	printf("(b) Both lexical and syntactical modules are implemented in entirety\n");
	while(flag){
		printf("Choose option: ");
		scan("%d\n",&input);
		switch(input)
		{
		case 0:
			flag=0;
			break;
		case 1:
			char cleanFile[]="Clean code";
			removeComments(argv[1],cleanFile)
			FILE *fp=fopen(cleanFile,"r");
			char c=fgetc(fp);
			while(c!=EOF){
				printf("%c",c);
				c=fgetc(fp);
			}
			printf("\n");
			fclose(fp);
			break;
		case 2:
			char *filename="input.txt";
			initLexer(filename);
			while(1)
			{
				struct TOKEN_INFO token_info=getNextToken();
				printf("%d\t%s\t%s\n",token_info.lineno,token_info.lexeme,mapping[token_info.token].str);
				if(token_info.token==FEOF)break;
			}
			break;
		case 3:
			main();////////////////////////////////////////////////////////rename according to predictive
			break;
		case 4:
			clock_t start_time, end_time;
		        double total_CPU_time, total_CPU_time_in_seconds;
		        start_time = clock();
		        main();////////////////////////////////////////////////////////rename according to predictive                           // invoke your lexer and parser here
			end_time = clock();
		        total_CPU_time  =  (double) (end_time - start_time);
		        total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
		     	printf("Total CPU time = %d\n",total_CPU_time);// Print both total_CPU_time and total_CPU_time_in_seconds 
			printf("Total CPU time in seconds = %d\n", total_CPU_time_in_seconds);
			break;
		default:
		}
	}
}
#endif
