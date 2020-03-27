#include<stdio.h>

typedef struct VariableEntry
{
	char varName[25];
	char type[25];
	//Line number?
	//Offset?
	struct VariableEntry *next; 
}VariableEntry;

typedef VariableEntry[43] VariableEntryTable;

typedef struct ParameterList
{
	char varName[25];
	char type[25];
	//Line number
	struct ParameterList *next;
}ParameterList;

typedef struct LocalTable
{
	struct LocalTable *parent;
	struct LocalTable *leftChild;
	struct LocalTable *rightSibling;
	VariableEntryTable variableTable;	//Pointer or static?
}LocalTable;

typedef struct FunctionTable{
	char funcName[25];
	int fsize;
	ParameterList *inputParaList;
	ParameterList *outputParaList;
	LocalTable *localTable;
	struct FunctionTable *next;
}FunctionTable;


typedef struct SymbolTableEntry{
	char funcName[25];
	FunctionTable *pointer;
}SymbolTableEntry;

typedef SymbolTableEntry[127] SymbolTable;