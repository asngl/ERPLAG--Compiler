#ifndef _LEXER
#define _LEXER
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *context;// Stores the fp of the file we are currently working on

void initLexer(char inputFile[])
{
	// Open file and store file pointer for parsing
	context=fopen(inputFile,"r");
	
	curr_lineno = 1;
	buffer_pointer=0;
	nonactivefilled_flag=0;
	
	// This ensures that the next buffer read is buffer 0. 
	active_buffer=1;	
	
	//Initialise buffer
	for(int i=0;i<=MAX_BUFFER_SIZE;i++)
	{
		buffer[0][i]='\0';
		buffer[1][i]='\0';
	}
	
	// Fill one buffer initially 
	context=getStream(context);
}

FILE *getStream(FILE *fp)// Reads the next MAX_BUFFER_SIZE characters into the non-active buffer
{
	
	buffer_pointer=0;
	nonactivefilled_flag=0;
	active_buffer=(active_buffer+1)%2; // Toggle the active buffer
	
	int i=0;
	for(i=0;i<MAX_BUFFER_SIZE;i++)
	{   
		char c=fgetc(fp);
		buffer[active_buffer][i]=c;
		if(c==EOF)
		{
			i++;
			break;
		}
	}
	
	buffer[active_buffer][i] = '\0';
	return fp;
}

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
				}
				break;
		}		
	}
	if(CURR_STATE==1)
			fprintf(writeFile,"%c",'*');
}

void popChar(char read_char)// Remove a character from the input stream
{
	buffer_pointer++;
	if(buffer_pointer==MAX_BUFFER_SIZE)
	{
		if(nonactivefilled_flag==0)// If the non active buffer is not filled
			context=getStream(context);
		else // If the non-active buffer is already filled
		{
			active_buffer=(active_buffer+1)%2;
			buffer_pointer=0;
		}
	}
}

void retractCharacters(int n)// Adds n character in front of the input stream
{
	buffer_pointer-=n;	//Go back n characters
	if(buffer_pointer<0)
	{
		nonactivefilled_flag=1;
		buffer_pointer+=MAX_BUFFER_SIZE; // Gets the pointer in the previous buffer
		active_buffer=(active_buffer+1)%2;
	}
}

enum Terminals lookupTable(char *str)
{
	// Match str with corresponding terminal
	//printf("\nTrying to lookup %s\n",str);
	if(strcmp(str,"integer")==0)
	{
		return INTEGER;
	}
	else if(strcmp(str,"real")==0)
	{
		return REAL;
	}
	else if(strcmp(str,"boolean")==0)
	{
		return BOOLEAN;
	}
	else if(strcmp(str,"of")==0)
	{
		return OF;
	}
	else if(strcmp(str,"array")==0)
	{
		return ARRAY;
	}
	else if(strcmp(str,"start")==0)
	{
		return START;
	}
	else if(strcmp(str,"end")==0)
	{
		return END;
	}
	else if(strcmp(str,"declare")==0)
	{
		return DECLARE;
	}
	else if(strcmp(str,"begin")==0)
	{
		return BEGIN;
	}
	else if(strcmp(str,"module")==0)
	{
		return MODULE;
	}
	else if(strcmp(str,"driver")==0)
	{
		return DRIVER;
	}
	else if(strcmp(str,"program")==0)
	{
		return PROGRAM;
	}
	else if(strcmp(str,"record")==0)
	{
		return RECORD;
	}
	else if(strcmp(str,"union")==0)
	{
		return UNION;
	}
	else if(strcmp(str,"tagged")==0)
	{
		return TAGGED;
	}
	else if(strcmp(str,"get_value")==0)
	{
		return GET_VALUE;
	}
	else if(strcmp(str,"print")==0)
	{
		return PRINT;
	}
	else if(strcmp(str,"use")==0)
	{
		return USE;
	}
	else if(strcmp(str,"with")==0)
	{
		return WITH;
	}
	else if(strcmp(str,"parameters")==0)
	{
		return PARAMETERS;
	}
	else if(strcmp(str,"true")==0)
	{
		return TRUE;
	}
	else if(strcmp(str,"false")==0)
	{
		return FALSE;
	}
	else if(strcmp(str,"takes")==0)
	{
		return TAKES;
	}
	else if(strcmp(str,"input")==0)
	{
		return INPUT;
	}
	else if(strcmp(str,"returns")==0)
	{
		return RETURNS;
	}
	else if(strcmp(str,"AND")==0)
	{
		return AND;
	}
	else if(strcmp(str,"OR")==0)
	{
		return OR;
	}
	else if(strcmp(str,"for")==0)
	{
		return FOR;
	}
	else if(strcmp(str,"in")==0)
	{
		return IN;
	}
	else if(strcmp(str,"switch")==0)
	{
		return SWITCH;
	}
	else if(strcmp(str,"case")==0)
	{
		return CASE;
	}
	else if(strcmp(str,"break")==0)
	{
		return BREAK;
	}
	else if(strcmp(str,"default")==0)
	{
		return DEFAULT;
	}
	else if(strcmp(str,"while")==0)
	{
		return WHILE;
	}
	else
		return ID;
	 
}

// Reads and returns the next Valid Lexical Token from the Input Stream
// Lexical errors are printed on the standard output stream
// REFER TO DFA for STATE TRANSITIONS
struct TOKEN_INFO getNextToken()
{
	// Initialise state as start state
	int state=1;
	int final=0;
	char read_char;
	int lexeme_size=0;
	struct TOKEN_INFO token_info;
	token_info.lineno=curr_lineno;
	token_info.value.tag=2;
	while(!final)// Keep going till we encounter a final state
	{
		// Can be used to print state transitions for debugging purposes
		//printf("%d->",state);
		
		//Peeks at the next input character
		read_char=buffer[active_buffer][buffer_pointer];
		char c=read_char;// Alias variable for read_char
		
		switch(state)
		{
			case 1:// Currently at Start State 
				//Read the character from the stream, add it as a part of the lexeme and increment buffer pointer
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
					case ' ':case '\t':case '\r':
						state=45;
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
						else if(c==EOF)
						{
							state=46;
						}
						else
						{	
							// If we encounter a character for which no transitions are defined from the start state, Throw LEXICAL-ERROR
							printf("LEXICAL ERROR:Encountered unexpected character.\n %d\t%c",curr_lineno,read_char);
							lexeme_size=0;
							token_info.lexeme[0]='\0';
							token_info.lineno=curr_lineno;
							token_info.value.tag=2;
							state=1;
						}
						break;
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
				popChar(read_char);
				if(read_char=='*')
				{
					state=7;
				}
				else 
					state=6;
				break;
			case 7:
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
				{
					printf("LEXICAL ERROR:Encountered unexpected character.\n %d\t%c",curr_lineno,read_char);
					lexeme_size=0;
					token_info.lexeme[0]='\0';
					token_info.lineno=curr_lineno;
					token_info.value.tag=2;
					state=1;
				}
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
				{
					printf("LEXICAL ERROR:Encountered unexpected character.\n %d\t%c",curr_lineno,read_char);
					lexeme_size=0;
					token_info.lexeme[0]='\0';
					token_info.lineno=curr_lineno;
					token_info.value.tag=2;
					state=1;
				}
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
				{
					printf("LEXICAL ERROR:Encountered unexpected character.\n %d\t%c",curr_lineno,read_char);
					lexeme_size=0;
					token_info.lexeme[0]='\0';
					token_info.lineno=curr_lineno;
					token_info.value.tag=2;
					state=1;
				}
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
					if(lexeme_size<=19)
						token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
			     	state = 33;
			    }
			    else {
			    	state = 34;
			    }
				break;
			case 34:
				if(lexeme_size>20)
				{
					printf("LEXICAL ERROR:Encountered lexeme sequence of too much length.\n %d\t%s %c",curr_lineno,token_info.lexeme,read_char);
					lexeme_size=0;
					token_info.lexeme[0]='\0';
					token_info.lineno=curr_lineno;
					token_info.value.tag=2;
					state=1;
				}
				else
				{
					token_info.lexeme[lexeme_size]='\0';
					token_info.token=lookupTable(token_info.lexeme);
					final=1;
				}
				break;
			case 35:
			    if(c>='0'&&c<='9'){
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
			     	state = 35;
			    }
			    else if(c=='.'){
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
				if(read_char=='.')
				{
					retractCharacters(1);
					lexeme_size-=1;
					token_info.token=NUM;
					token_info.value.value.num = 0;
					token_info.value.tag = 0;
					for(int i = 0; i<lexeme_size; i++){
						token_info.value.value.num *= 10;
						token_info.value.value.num += token_info.lexeme[i]-'0'; 
					}
					final=1;
				}
				else if(read_char >='0' && read_char <='9')
				{
					token_info.lexeme[lexeme_size]=read_char;
					lexeme_size++;
					popChar(read_char);
					state=38;
				}
				else
				{
					printf("LEXICAL ERROR:Encountered unexpected character.\n %d\t%c",curr_lineno,read_char);
					lexeme_size=0;
					token_info.lexeme[0]='\0';
					token_info.lineno=curr_lineno;
					token_info.value.tag=2;
					state=1;
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
				{
					printf("LEXICAL ERROR:Encountered unexpected character.\n %d\t%c",curr_lineno,read_char);
					lexeme_size=0;
					token_info.lexeme[0]='\0';
					token_info.lineno=curr_lineno;
					token_info.value.tag=2;
					state=1;
				}
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
				{
					printf("LEXICAL ERROR:Encountered unexpected character.\n %d\t%c",curr_lineno,read_char);
					lexeme_size=0;
					token_info.lexeme[0]='\0';
					token_info.lineno=curr_lineno;
					token_info.value.tag=2;
					state=1;
				}
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
				token_info.token=FEOF;
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
	}
	//MARK THE END OF LEXEME
	token_info.lexeme[lexeme_size]='\0';
	return token_info;
}
#endif

