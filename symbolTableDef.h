
#ifndef _SYMBOLTABLEDEFH
#define _SYMBOLTABLEDEFH

#include "ASTNodeDef.h"
#define MAX_IDENTIFIER_LENGTH 25
#define MOD 127
//enum Datatype{DT_INTEGER,DT_REAL,DT_BOOLEAN,DT_ARRAY};
extern int ERROR_FLAG;
typedef union ArrayBound{
	int bound;
	char lexeme[MAX_IDENTIFIER_LENGTH+1];
}ArrayBound;

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


typedef VariableEntry *VariableEntryTable[MOD];

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

typedef struct Scope{
	int startLine;
	int endLine;
}Scope;
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

typedef struct SymbolTableEntry{
	FunctionTable *pointer;
}SymbolTableEntry;

typedef SymbolTableEntry SymbolTable[MOD];

typedef struct StringList{
	char varName[MAX_IDENTIFIER_LENGTH+1];
	struct StringList *next;
}StringList;

typedef struct Context{
	SymbolTableEntry **symbolTable;
	char funcName[MAX_IDENTIFIER_LENGTH+1];
	VariableEntry *forbiddenVariables;
	VariableEntry *inputList;
	VariableEntry *outputList;
}Context;

#endif


