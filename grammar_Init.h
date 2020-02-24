/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _PARSERH
#define _PARSERH

extern void initGrammar(char *filename);//Function to read and store grammarRules
extern int ComputeFirstAndFollowSets();	//Function to compute first and follow sets together
extern int CreateParseTable();			//Function for creation of parse table

#endif