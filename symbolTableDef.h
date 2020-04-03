#include "ASTNodeDef.h"
#define MOD 127
//enum Datatype{DT_INTEGER,DT_REAL,DT_BOOLEAN,DT_ARRAY};

typedef union ArrayBound{
	int bound;
	char lexeme[25];
}ArrayBound;

typedef struct Type
{
	enum Datatype type;
	int arrayFlag;		//1 if variable is an array
	ArrayBound low;	//Lower bound for Array type
	int tagLow;		//0 for num, 1 for ID
	ArrayBound high;
	int tagHigh;
	int isStatic;	//1 for Static, 0 for Dynamic

}Type;

typedef struct VariableEntry
{
	char varName[25];
	Type type;
	int lineNumber;
	int offset; //Offset to be calculated for code generation
	int width;  //Length in bytes
	struct VariableEntry *next;// For collisions
}VariableEntry;
currVar->node.idListNode.varName
typedef VariableEntry[MOD] VariableEntryTable;

typedef struct ParameterList
{
	char varName[25];
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
	char funcName[25];
	int declareFlag;
	int defineFlag;
	int useFlag;
	int fsize;
	int lineNumber;			//Line number for function Declaration
	int lineNumberDef;		//Line number for function Defintion
	int controlBaseOffset;
	int staticVariableOffset;
	int dynamicVariableOffset;
	ParameterList *inputParaList;
	ParameterList *outputParaList;
	LocalTable *localTable;
	struct FunctionTable *next; // For Collisions
}FunctionTable;
/*
struct ParaListNode{
	char name[STRING_MAX_SIZE];
	enum Datatype type;
	struct ASTNode *next;
	struct ASTNode *Range;
};
*/
typedef struct SymbolTableEntry{
	FunctionTable *pointer;
}SymbolTableEntry;

typedef SymbolTableEntry[MOD] SymbolTable;