#ifndef _DRIVER
#define _DRIVER
#include "lexer.h"
// #ifndef macrolexerc
// #define macrolexerc
// #include "lexer.c"
// #endif
int main()
{	
	char *in="in.txt";
	//char *out="out.txt";
	//removeComments(in,out);
	FILE *fp=fopen(in,"r");
	printf("HI\n");
	fp=getStream(fp);
	printf("HI\n");
	for(int i=1;i<=6;i++)
	{
		struct TOKEN_INFO token_info=getNextToken();
		printf(":%s",token_info.lexeme);
		printf("::%d\n",token_info.token);
	}	
	//token_info=getNextToken();
	//printf("%s",token_info.lexeme);
	/*int end=0;
	while(!end)
	{
		fp=getStream(fp);
		while(buffer_pointer<MAX_BUFFER_SIZE)
		{
			char c=buffer[buffer_pointer];
			if(c=='\0') //Buffer ended
				break;
			if(c==EOF)  //File ended
			{
				end=1;
				break;
			}
			buffer_pointer++;
			printf("%c",c);
		}
	}
	return 0;*/
}
#endif
