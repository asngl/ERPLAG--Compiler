#ifndef _PARSERH
#define _PARSERH
#define NUM_OF_TERMINALS 63
#define NUM_OF_NON_TERMINALS 54 
enum Terminals{INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER, PROGRAM, RECORD, TAGGED, UNION, GET_VALUE, PRINT, USE, WITH, PARAMETERS, TRUE, FALSE, TAKES, INPUT, RETURNS, AND, OR, FOR, IN, SWITCH, CASE, BREAK, DEFAULT, WHILE, PLUS, MINUS, MUL, DIV, LT, LE, GE, GT, EQ, NE, DEF, ENDDEF, DRIVERDEF, DRIVERENDDEF, COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP, SQBO, SQBC, BO, BC, COMMENTMARK, ID, NUM, RNUM, EPS , FEOF, BEGIN};
enum NonTerminals{program, moduleDeclarations, moduleDeclaration, otherModules, driverModule, module, ret, input_plist, more_input_plist, output_plist, more_output_plist, dataType, range_arrays, type, moduleDef, statements, statement, ioStmt, var2, var, whichId, whichIndex, simpleStmt, assignmentStmt, whichStmt, lvalueIDStmt, lvalueARRStmt, Index, moduleReuseStmt, optional, idList, more_id, expression, posExpr, posExpr2, relExpr, relExpr2, addExpr, addExpr2, multExpr, multExpr2, basicExpr, logOp, pmOp, mdOp, relOp, declareStmt, conditionalStmt, caseStmt, caseStmts, value, Default, iterativeStmt, range};

typedef enum Terminals Terminal;
typedef enum NonTerminals NonTerminal;

union Symb{
	enum Terminals T;
	enum NonTerminals NT;
};
typedef union Symb Symbol;
// Union for enumeration of terminals and nonterminals
struct TaggedSymbol
{
	union Symb symbol;
	int tag;// tag=0 for terminal,1 for non terminal,2 for not defined
};
// Hardcoded enum values for Symbols

enum type_of_symbol{terminal, nonTerminal};

struct MT{
	Symbol s;
	char str[Max_Token_Size];
	enum type_of_symbol tag;
	int flag; //To determine whether element is present or not, 1 if present
};

struct rhsnode{
	Symbol s;
	enum type_of_symbol tag;
	struct rhsnode *next;
};

typedef struct rhsnode RHSNODE;
struct cell{
	enum NonTerminals sym;
	RHSNODE *head;
};



typedef struct MT MappingTable[No_Of_Tokens];



typedef struct cell GRAMMAR[No_Of_Rules];
//int parseTable[NUM_OF_NON_TERMINALS][NUM_OF_TERMINALS]; // Stores rule number to be applied ,-1 for error ,-2 for syn

#endif


// IN RULES FILE , use Index instead of index

