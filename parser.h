#ifndef _PARSERH
#define _PARSERH
enum Terminals{INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER, PROGRAM, RECORD, TAGGED, UNION, GET_VALUE, PRINT, USE, WITH, PARAMETERS, TRUE, FALSE, TAKES, INPUT, RETURNS, AND, OR, FOR, IN, SWITCH, CASE, BREAK, DEFAULT, WHILE, PLUS, MINUS, MUL, DIV, LT, LE, GE, GT, EQ, NE, DEF, ENDDEF, DRIVERDEF, DRIVERENDDEF, COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP, SQBO, SQBC, BO, BC, COMMENTMARK, ID, NUM, RNUM, EPS , FEOF, BEGIN};
enum NonTerminals{program, moduleDeclarations, moduleDeclaration, otherModules, driverModule, module, ret, input_plist, more_input_plist, output_plist, more_output_plist, dataType, range_arrays, type, moduleDef, statements, statement, ioStmt, var2, var, whichId, whichIndex, simpleStmt, assignmentStmt, whichStmt, lvalueIDStmt, lvalueARRStmt, Index, moduleReuseStmt, optional, idList, more_id, expression, posExpr, posExpr2, relExpr, relExpr2, addExpr, addExpr2, multExpr, multExpr2, basicExpr, logOp, pmOp, mdOp, relOp, declareStmt, conditionalStmt, caseStmt, caseStmts, value, Default, iterativeStmt, range};

typedef enum Terminals Terminal;
typedef enum NonTerminals NonTerminal;


// Union for enumeration of terminals and nonterminals
// Hardcoded enum values for Symbols
#endif


// IN RULES FILE , use Index instead of index

