enum Terminals{INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER, PROGRAM, RECORD, TAGGED, UNION, GET_VALUE, PRINT, 
USE, WITH, PARAMETERS, TRUE, FALSE, TAKES, INPUT, RETURNS, AND, OR, FOR, IN, SWITCH, CASE, BREAK, DEFAULT, WHILE, PLUS, MINUS, MUL, DIV, LT, LE, GE, GT, EQ, NE, DEF, ENDDEF, COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP, SQBO, SQBC, BO, BC, COMMENTMARK, DRIVERDEF, DRIVERENDDEF, ID, FEOF, EPS, NUM, RNUM };
enum NonTerminals{program, moduleDeclarations, moduleDeclaration, otherModules, driverModule, module, ret, input_plist, more_input_plist, output_plist, more_output_plist, dataType, range_arrays, type, moduleDef, statements, statement, ioStmt, var2, var, whichId, whichIndex, simpleStmt, assignmentStmt, whichStmt, lvalueIDStmt, lvalueARRStmt, index, moduleReuseStmt, optional, idList, more_id, expression, posExpr, posExpr2, relExpr, relExpr2, addExpr, addExpr2, multExpr, multExpr2, basicExpr, logOp, pmOp, mdOp, relOp, declareStmt, conditionalStmt, caseStmt, caseStmts, value, Default, iterativeStmt, range};

typedef enum Terminals Terminal;
typedef enum NonTerminals NonTerminal;

// Hardcoded enum values for Symbols
