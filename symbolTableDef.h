#include<stdio.h>

typedef struct VariableEntry
{
	char varName[25];
	char type[25];
	//Offset?
	int offset; //Offset to be calculated for code generation
	int width;  //Length in bytes
	struct VariableEntry *next;// For collisions
}VariableEntry;

typedef VariableEntry[43] VariableEntryTable;

typedef struct ParameterList
{
	char varName[25];
	char type[25];
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
	struct LocalTable *lastSibling;
	Scope scope;
	VariableEntryTable variableTable;	//Pointer or static??
}LocalTable;

typedef struct FunctionTable{
	char funcName[25];
	int fsize;
	ParameterList *inputParaList;
	ParameterList *outputParaList;
	LocalTable *localTable;
	struct FunctionTable *next; // For Collisions
}FunctionTable;


typedef struct SymbolTableEntry{
	FunctionTable *pointer;
}SymbolTableEntry;

typedef SymbolTableEntry[127] SymbolTable;