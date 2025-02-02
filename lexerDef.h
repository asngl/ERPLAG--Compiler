/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef macrolexerDefh
#define macrolexerDefh
#include "grammar_InitDef.h"
#define MAX_BUFFER_SIZE 40000
#define MAX_LOOK_BACK_SIZE 2
#define MAX_LEXEME_SIZE 25

extern int ERROR_FLAG;
unsigned int curr_lineno;

char buffer[2][MAX_BUFFER_SIZE+1];//buffer[MAX_BUFFER_SIZE]='\0'
int active_buffer;
int buffer_pointer;
int nonactivefilled_flag;

char look_back[MAX_LOOK_BACK_SIZE];//look_back[i] denotes the (i+1)'th character before the current character
int look_back_size;
int retract_size;
int retract_character_flag;

union UNTAGGED_VALUE{
	int num;
	float rnum;
};
struct TAGGED_VALUE{
	unsigned int tag;// tag=0 denotes num , tag=1 denotes rnum and tag=2 denotes undefined 
	union UNTAGGED_VALUE value;
};

struct TOKEN_INFO{
	char lexeme[MAX_LEXEME_SIZE+1];
	enum Terminals token;
	struct TAGGED_VALUE value;
	unsigned int lineno;
};
#endif
