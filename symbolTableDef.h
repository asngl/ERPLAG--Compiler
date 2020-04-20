/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _SYMBOLTABLEDEFH
#define _SYMBOLTABLEDEFH

#include "ASTNodeDef.h"
#define MAX_IDENTIFIER_LENGTH 25
#define MOD 127

extern int ERROR_FLAG;

//Union for storing bound of an array
typedef union ArrayBound{
	int bound;
	char lexeme[MAX_IDENTIFIER_LENGTH+1];
}ArrayBound;

//Structure for storing Type 
typedef struct Type
{
	enum Datatype type;
	int arrayFlag;		//1 if variable is an array
	ArrayBound low;	//Lower bound for Array type
	int tagLow;		//0 for num, 1 for ID
	struct VariableEntry *lowPtr;
	ArrayBound high;
	int tagHigh;
	struct VariableEntry *highPtr;
	int isStatic;	//1 for Static, 0 for Dynamic
}Type;

//Structure for storing a single variable entry
typedef struct VariableEntry
{
	char varName[25];
	Type type;
	int lineNumber;
	int offset; //Offset to be calculated for code generation
	int width;  //Length in bytes
	int initFlag;
	int isParameter;
	struct VariableEntry *next;// For collisions
}VariableEntry;

//Array of variable entry to implement hashed arrays
typedef VariableEntry *VariableEntryTable[MOD];

//Structure for storing parameter information
typedef struct ParameterList
{
	char varName[MAX_IDENTIFIER_LENGTH+1];
	Type type;
	int initFlag;
	int lineNumber;
	int offset;
	int width;
	//Line number
	struct ParameterList *next;
}ParameterList;

//Structure for storing scope
typedef struct Scope{
	int startLine;
	int endLine;
}Scope;

//Structure for storing all the variables in a given scope
typedef struct LocalTable
{
	struct LocalTable *parent;
	struct LocalTable *leftChild;
	struct LocalTable *rightSibling;
	struct LocalTable *lastChild;
	Scope scope;
	int size;
	VariableEntryTable variableTable;
}LocalTable;

//Stucture for storing all the local tables corresponding to a function
typedef struct FunctionTable{
	char funcName[MAX_IDENTIFIER_LENGTH+1];
	int declareFlag;
	int defineFlag;
	int useFlag;
	int fsize;
	int lineNumber;			//Line number for function Declaration
	int lineNumberDef;		//Line number for function Defintion

	int activationRecordSize;

	Scope scope;
	ParameterList *inputParaList;
	ParameterList *outputParaList;
	LocalTable *localTable;
	struct FunctionTable *next; // For Collisions
}FunctionTable;

//Structure to implement symbol table as hashed arrays
typedef struct SymbolTableEntry{
	FunctionTable *pointer;
}SymbolTableEntry;

typedef SymbolTableEntry SymbolTable[MOD];

//Structure to store a string to avoid error repetition of a given undeclared variable
typedef struct StringList{
	char varName[MAX_IDENTIFIER_LENGTH+1];
	struct StringList *next;
}StringList;

//Structure for passing information such as forbidden variables, etc. for type checking
typedef struct Context{
	SymbolTableEntry **symbolTable;
	char funcName[MAX_IDENTIFIER_LENGTH+1];
	VariableEntry *forbiddenVariables;
	VariableEntry *inputList;
	VariableEntry *outputList;
}Context;

#endif


