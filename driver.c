#ifndef _DRIVER
#define _DRIVER
#include "lexer.h"
// #ifndef macrolexerc
// #define macrolexerc
// #include "lexer.c"
// #endif
int main(int argc, char *argv[])
{	
	//char *out="out.txt";
	//removeComments(in,out);
	//printf("HI\n");
	//printf("%d %d",EPS,FEOF);
	initLexer(argv[1]);
	//printf("HI\n");
	while(1)
	{
		struct TOKEN_INFO token_info=getNextToken();
		printf("%d, ",token_info.token);
		if(token_info.token==FEOF)break;
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
