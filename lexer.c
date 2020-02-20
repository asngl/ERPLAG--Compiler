#ifndef _LEXER
#define _LEXER
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>


/*  DESIGN ISSUES  */
/*
	what do you return on lexical error 
	what do you return on end of file 
	getStream() in getNextToken()??? BUFFER ERROR what if we dont have fp
	Do you denote space and empty characters as EPS token on NULL


*/
void initLexer()
{
	curr_lineno = 1;
	buffer_pointer=0;
	look_back_size=0;
	retract_size=0;
	retract_character_flag=0;
	for(int i=0;i<=MAX_LOOK_BACK_SIZE;i++)
		look_back[i]='\0';
}
FILE *getStream(FILE *fp)
{
	buffer_pointer=0;
	int i=0;
	for(i=0;i<MAX_BUFFER_SIZE;i++)
	{   
		char c=fgetc(fp);
		buffer[i]=c;
		if(c==EOF)
		{
			i++;
			break;
		}
	}
	buffer[i] = '\0';
	return fp;
}
// struct TOKEN_INFO getNextToken()
// {
// 	return NULL;
// }
void removeComments(char *testcaseFile, char *cleanFile)
{
	FILE *readFile=fopen(testcaseFile,"r");
	FILE *writeFile=fopen(cleanFile,"w"); 
	int CURR_STATE=0;
	//STATE 0: NORMAL CODE
	//STATE 1: 1 star 
	//STATE 2: 2 star 
	//STATE 3: 3 star 
	char character;
	while((character=fgetc(readFile))!=EOF){
		switch(CURR_STATE)
		{
			case 0:
				if(character=='*')
					CURR_STATE=1;
				else
					fprintf(writeFile,"%c",character);
				break;
			case 1:
				if(character=='*')
					CURR_STATE=2;
				else
				{
					CURR_STATE=0;
					fprintf(writeFile,"%c%c",'*',character);
				}
				break;
			case 2:
				if(character=='*')
					CURR_STATE=3;
				break;
			case 3:
				if(character=='*')
					CURR_STATE=0;
				else
				{
					CURR_STATE=2;
					//fprintf(writeFile,"%c%c",'*',character);
				}
				break;
		}		
	}
	if(CURR_STATE==1)
			fprintf(writeFile,"%c",'*');
}
void updateLookBack(char c)
{
	for(int i=MAX_LOOK_BACK_SIZE-1;i>0;i--)
		look_back[i]=look_back[i-1];
	look_back[0]=c;
}
void popChar(char read_char)
{
	if(retract_character_flag==1)
		retract_character_flag=0;
	else
	{
		buffer_pointer++;
		updateLookBack(read_char);
	}
}
struct TOKEN_INFO getNextToken()
{
	int state=1;
	int final=0;
	char read_char;
	int end=0;
	int lexeme_size=0;
	struct TOKEN_INFO token_info;
	token_info.lineno=curr_lineno;
	token_info.value.tag=2;
	printf("\nCALLED\n");
	while(!end)
	{
		printf("HERE%d->",state);
		if(retract_size>0)
		{
			retract_character_flag=1;
			retract_size--;
			read_char=look_back[retract_size];
		}
		else
			read_char=buffer[buffer_pointer];
		//if end==1 TODO
		// if lexeme_size >20 TODO
		if(read_char=='\0')
		{
			//TODO getStream()?? 
		}
		else if(read_char==EOF)
			end=1;
		char c=read_char;
		switch(state)
		{
			case 1:
				token_info.lexeme[lexeme_size]=read_char;
				lexeme_size++;
				popChar(read_char);
				switch(read_char)
				{
					case '+':
						state=2;
					break;
					case '-':
						state=3;
					break;
					case '*':
						state=4;
					break;
					case '/':
						state=9;
					break;
					case '<':
						state=10;
					break;
					case '[':
						state=14;
					break;
					case ']':
						state=15;
					break;
					case '(':
						state=16;
					break;
					case ')':
						state=17;
					break;
					case '>':
						state=18;
					break;
					case '=':
						state=22;
					break;
					case '!':
						state=24;
					break;
					case '.':
						state=26;
					break;
					case ';':
						state=28;
					break;
					case ',':
						state=29;
					break;
					case ':':
						state=30;
					break;
					case '\n':
						state=44;
					break;
					case ' ':case '\t':
						state=45;
					break;
					case EOF:
						state=46;
					break;
					default:
						if(((c>='a')&&(c<='z'))||((c>='A')&&(c<='Z')))
						{
							state=33;
						}
						else if((c>='0')&&(c<='9'))
						{
							state=35;
						}
						else
						{	
							printf("LEXICAL_ERROR: Read incorrect character %c at line %d at state %d",read_char,curr_lineno,state);
						}

				}
				break;
			case 2:
				token_info.token=PLUS;
				final=1;
				break;
			case 3:
				token_info.token=MINUS;
				final=1;
				break;
			case 4:
				if(read_char=='*')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=6;
				}
				else
					state=5;
				break;
			case 5:
				token_info.token=MUL;
				final=1;
				break;
			case 6:
				token_info.lexeme[lexeme_size]=read_char;
				lexeme_size++;
				popChar(read_char);
				if(read_char=='*')
				{
					state=7;
				}
				else 
					state=6;
				break;
			case 7:
				token_info.lexeme[lexeme_size]=read_char;
				lexeme_size++;
				popChar(read_char);
				if(read_char=='*')
				{
					state=8;
				}
				else 
					state=6;
				break;
			case 8:
				token_info.token=COMMENTMARK;
				final=1;
				break;
			case 9:
				token_info.token=DIV;
				final=1;
				break;
			case 10:
				if(read_char=='<')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=11;
				}
				else if (read_char=='=')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=12;
				}
				else
					state=13;
				break;
			case 11:
				if(read_char=='<')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=47;
				}
				else
				{
					state=48;
				}
				break;
			case 12:
				token_info.token=LE;
				final=1;
				break;
			case 13:
				token_info.token=LT;
				final=1;
				break;
			case 14:
				token_info.token=SQBO;
				final=1;
				break;
			case 15:
				token_info.token=SQBC;
				final=1;
				break;
			case 16:
				token_info.token=BO;
				final=1;
				break;
			case 17:
				token_info.token=BC;
				final=1;
				break;
			case 18:
				if(read_char=='>')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=19;
				}
				else if (read_char=='=')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);	
					state=20;
				}
				else
					state=21;
				break;
			case 19:
				if(read_char=='>')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=49;
				}
				else
				{
					state=50;
				}
				break;
			case 20:
				token_info.token=GE;
				final=1;
				break;
			case 21:
				token_info.token=GT;
				final=1;
				break;
			case 22:
				if(read_char=='=')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=23;
				}
				else
					printf("LEX");
				break;
			case 23:
				token_info.token=EQ;
				final=1;
				break;
			case 24:
				if(read_char=='=')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=23;
				}
				else
					printf("LEX");
				break;
			case 25:
				token_info.token=NE;
				final=1;
				break;
			case 26:
				if(read_char=='.')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=27;
				}
				else
					printf("LEX");
				break;
			case 27:
				token_info.token=RANGEOP;
				final=1;
				break;
			case 28:
				token_info.token=SEMICOL;
				final=1;
				break;
			case 29:
				token_info.token=COMMA;
				final=1;
				break;
			case 30:
				if(read_char=='=')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=31;
				}
				else
					state=32;
				break;
			case 31:
				token_info.token=ASSIGNOP;
				final=1;
				break;
			case 32:
				token_info.token=COLON;
				final=1;
				break;
			case 33:
			    if(c>='a'&&c<='z' || c>='A'&&c<='Z' || c>='0'&&c<='9'|| c=='_'){
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					//if lexeme_size>20 TODO
			     	state = 33;
			    }
			    else {
			    	state = 34;
			    }
				break;
			case 34:
				token_info.token=ID;
				//LOOKUP TABLE, action 34 TODO
				final=1;
				break;
			case 35:
			    if(c>='0'&&c<='9'){
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
			     	state = 35;
			    }
			    else if(c=='.'){
			    	//look back TODO
			 		token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
			     	state = 37;
			    }
			    else {
			    	state = 36;
			    }
				break;
			case 36:
				token_info.token=NUM;
				token_info.value.value.num = 0;
				token_info.value.tag = 0;
				for(int i = 0; i<lexeme_size; i++){
					token_info.value.value.num *= 10;
					token_info.value.value.num += token_info.lexeme[i]-'0'; 
				}
				final=1;
				break;
			case 37:
				// check for look ahead
				if(read_char=='.')
				{
					//CONFUSED TODO
				}
				else if(read_char >='0' && read_char <='9')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=37;
				}
				else
				{
					printf("LEX");
				}
				break;
			case 38:
				if(read_char >='0' && read_char <='9')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=38;
				}
				else if(read_char=='e'||read_char=='E')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=40;
				}
				else 
					state=39;
				break;
			case 39:
				token_info.token=RNUM;
				token_info.value.value.rnum = 0;
				token_info.value.tag = 1;
				int i=0;
				for(i = 0; i<lexeme_size; i++){
					if(token_info.lexeme[i]=='.')
						break;
					token_info.value.value.rnum *= 10;
					token_info.value.value.rnum += token_info.lexeme[i]-'0'; 
				}
				i++;
				float place_value=1;
				int decimal_value=0;
				for(; i<lexeme_size; i++){
					decimal_value*=10;
					decimal_value += token_info.lexeme[i]-'0'; 
					place_value*=10;
				}
				token_info.value.value.rnum+=((float)decimal_value)/place_value;
				final=1;
				break;
			case 40:
				if(read_char=='+'||read_char=='-')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=41;
				}
				else if(read_char >='0' && read_char <='9')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=42;
				}
				else
					printf("LEX");
				break;
			case 41:
				if(read_char >='0' && read_char <='9')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=42;
				}
				else
					printf("LEX");
				break;
			case 42:
				if(read_char >='0' && read_char <='9')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=42;
				}
				else
					state=43;
				break;
			case 43:
				token_info.token=RNUM;
				token_info.value.tag=1;
				token_info.value.value.rnum=atof(token_info.lexeme);
				final=1;
				break;
			case 44:
				token_info.token=EPS;
				curr_lineno=curr_lineno+1;
				final=1;
				break;
			case 45:
				token_info.token=EPS;
				final=1;
				break;
			case 46:
				token_info.token=EPS;
				end=1;// return NULL????? TODO
				final=1;
				break;
			case 47:
				token_info.token=DRIVERDEF;
				final=1;
				break;
			case 48:
				token_info.token=DEF;
				final=1;
				break;
			case 49:
				token_info.token=DRIVERENDDEF;
				final=1;
				break;
			case 50:
				token_info.token=ENDDEF;
				final=1;
				break;
		}
		if(final==1)break;
	}
	printf("\n");
	token_info.lexeme[lexeme_size]='\0';
	return token_info;
}
#endif

// struct TOKEN_INFO{
// 	char lexeme[MAX_LEXEME_SIZE+1];
// 	enum TOKENS token;
// 	struct TAGGED_VALUE value;
// 	unsigned int lineno;
// };
