#ifndef _PARSERH1
#define _PARSERH1
#include "parser.h"

void init_mappingtable(){
	mapping[0].s.T = INTEGER;
	strcpy(str, "INTEGER");
	mapping[0].tag = terminal;
	mapping[0].flag = 1;

	mapping[1].s.T = REAL;
	strcpy(str, "REAL");
	mapping[1].tag = terminal;
	mapping[1].flag = 1;

	mapping[2].s.T = BOOLEAN;
	strcpy(str, "BOOLEAN");
	mapping[2].tag = terminal;
	mapping[2].flag = 1;

	mapping[3].s.T = OF;
	strcpy(str, "OF");
	mapping[3].tag = terminal;
	mapping[3].flag = 1;

	mapping[4].s.T = ARRAY;
	strcpy(str, "ARRAY");
	mapping[4].tag = terminal;
	mapping[4].flag = 1;

	mapping[5].s.T = START;
	strcpy(str, "START");
	mapping[5].tag = terminal;
	mapping[5].flag = 1;

	mapping[6].s.T = END;
	strcpy(str, "END");
	mapping[6].tag = terminal;
	mapping[6].flag = 1;

	mapping[7].s.T = DECLARE;
	strcpy(str, "DECLARE");
	mapping[7].tag = terminal;
	mapping[7].flag = 1;

	mapping[8].s.T = MODULE;
	strcpy(str, "MODULE");
	mapping[8].tag = terminal;
	mapping[8].flag = 1;

	mapping[9].s.T = DRIVER;
	strcpy(str, "DRIVER");
	mapping[9].tag = terminal;
	mapping[9].flag = 1;

	mapping[10].s.T = PROGRAM;
	strcpy(str, "PROGRAM");
	mapping[10].tag = terminal;
	mapping[10].flag = 1;

	mapping[11].s.T = RECORD;
	strcpy(str, "RECORD");
	mapping[11].tag = terminal;
	mapping[11].flag = 1;

	mapping[12].s.T = TAGGED;
	strcpy(str, "TAGGED");
	mapping[12].tag = terminal;
	mapping[12].flag = 1;

	mapping[13].s.T = UNION;
	strcpy(str, "UNION");
	mapping[13].tag = terminal;
	mapping[13].flag = 1;

	mapping[14].s.T = GET_VALUE;
	strcpy(str, "GET_VALUE");
	mapping[14].tag = terminal;
	mapping[14].flag = 1;

	mapping[15].s.T = PRINT;
	strcpy(str, "PRINT");
	mapping[15].tag = terminal;
	mapping[15].flag = 1;

	mapping[16].s.T = USE;
	strcpy(str, "USE");
	mapping[16].tag = terminal;
	mapping[16].flag = 1;

	mapping[17].s.T = WITH;
	strcpy(str, "WITH");
	mapping[17].tag = terminal;
	mapping[17].flag = 1;

	mapping[18].s.T = PARAMETERS;
	strcpy(str, "PARAMETERS");
	mapping[18].tag = terminal;
	mapping[18].flag = 1;

	mapping[19].s.T = TRUE;
	strcpy(str, "TRUE");
	mapping[19].tag = terminal;
	mapping[19].flag = 1;

	mapping[20].s.T = FALSE;
	strcpy(str, "FALSE");
	mapping[20].tag = terminal;
	mapping[20].flag = 1;

	mapping[21].s.T = TAKES;
	strcpy(str, "TAKES");
	mapping[21].tag = terminal;
	mapping[21].flag = 1;

	mapping[22].s.T = INPUT;
	strcpy(str, "INPUT");
	mapping[22].tag = terminal;
	mapping[22].flag = 1;

	mapping[23].s.T = RETURNS;
	strcpy(str, "RETURNS");
	mapping[23].tag = terminal;
	mapping[23].flag = 1;

	mapping[24].s.T = AND;
	strcpy(str, "AND");
	mapping[24].tag = terminal;
	mapping[24].flag = 1;

	mapping[25].s.T = OR;
	strcpy(str, "OR");
	mapping[25].tag = terminal;
	mapping[25].flag = 1;

	mapping[26].s.T = FOR;
	strcpy(str, "FOR");
	mapping[26].tag = terminal;
	mapping[26].flag = 1;

	mapping[27].s.T = IN;
	strcpy(str, "IN");
	mapping[27].tag = terminal;
	mapping[27].flag = 1;

	mapping[28].s.T = SWITCH;
	strcpy(str, "SWITCH");
	mapping[28].tag = terminal;
	mapping[28].flag = 1;

	mapping[29].s.T = CASE;
	strcpy(str, "CASE");
	mapping[29].tag = terminal;
	mapping[29].flag = 1;

	mapping[30].s.T = BREAK;
	strcpy(str, "BREAK");
	mapping[30].tag = terminal;
	mapping[30].flag = 1;

	mapping[31].s.T = DEFAULT;
	strcpy(str, "DEFAULT");
	mapping[31].tag = terminal;
	mapping[31].flag = 1;

	mapping[32].s.T = WHILE;
	strcpy(str, "WHILE");
	mapping[32].tag = terminal;
	mapping[32].flag = 1;

	mapping[33].s.T = PLUS;
	strcpy(str, "PLUS");
	mapping[33].tag = terminal;
	mapping[33].flag = 1;

	mapping[34].s.T = MINUS;
	strcpy(str, "MINUS");
	mapping[34].tag = terminal;
	mapping[34].flag = 1;

	mapping[35].s.T = MUL;
	strcpy(str, "MUL");
	mapping[35].tag = terminal;
	mapping[35].flag = 1;

	mapping[36].s.T = DIV;
	strcpy(str, "DIV");
	mapping[36].tag = terminal;
	mapping[36].flag = 1;

	mapping[37].s.T = LT;
	strcpy(str, "LT");
	mapping[37].tag = terminal;
	mapping[37].flag = 1;

	mapping[38].s.T = LE;
	strcpy(str, "LE");
	mapping[38].tag = terminal;
	mapping[38].flag = 1;

	mapping[39].s.T = GE;
	strcpy(str, "GE");
	mapping[39].tag = terminal;
	mapping[39].flag = 1;

	mapping[40].s.T = GT;
	strcpy(str, "GT");
	mapping[40].tag = terminal;
	mapping[40].flag = 1;

	mapping[41].s.T = EQ;
	strcpy(str, "EQ");
	mapping[41].tag = terminal;
	mapping[41].flag = 1;

	mapping[42].s.T = NE;
	strcpy(str, "NE");
	mapping[42].tag = terminal;
	mapping[42].flag = 1;

	mapping[43].s.T = DEF;
	strcpy(str, "DEF");
	mapping[43].tag = terminal;
	mapping[43].flag = 1;

	mapping[44].s.T = ENDDEF;
	strcpy(str, "ENDDEF");
	mapping[44].tag = terminal;
	mapping[44].flag = 1;

	mapping[45].s.T = DRIVERDEF;
	strcpy(str, "DRIVERDEF");
	mapping[45].tag = terminal;
	mapping[45].flag = 1;

	mapping[46].s.T = DRIVERENDDEF;
	strcpy(str, "DRIVERENDDEF");
	mapping[46].tag = terminal;
	mapping[46].flag = 1;

	mapping[47].s.T = COLON;
	strcpy(str, "COLON");
	mapping[47].tag = terminal;
	mapping[47].flag = 1;

	mapping[48].s.T = RANGEOP;
	strcpy(str, "RANGEOP");
	mapping[48].tag = terminal;
	mapping[48].flag = 1;

	mapping[49].s.T = SEMICOL;
	strcpy(str, "SEMICOL");
	mapping[49].tag = terminal;
	mapping[49].flag = 1;

	mapping[50].s.T = COMMA;
	strcpy(str, "COMMA");
	mapping[50].tag = terminal;
	mapping[50].flag = 1;

	mapping[51].s.T = ASSIGNOP;
	strcpy(str, "ASSIGNOP");
	mapping[51].tag = terminal;
	mapping[51].flag = 1;

	mapping[52].s.T = SQBO;
	strcpy(str, "SQBO");
	mapping[52].tag = terminal;
	mapping[52].flag = 1;

	mapping[53].s.T = SQBC;
	strcpy(str, "SQBC");
	mapping[53].tag = terminal;
	mapping[53].flag = 1;

	mapping[54].s.T = BO;
	strcpy(str, "BO");
	mapping[54].tag = terminal;
	mapping[54].flag = 1;

	mapping[55].s.T = BC;
	strcpy(str, "BC");
	mapping[55].tag = terminal;
	mapping[55].flag = 1;

	mapping[56].s.T = COMMENTMARK;
	strcpy(str, "COMMENTMARK");
	mapping[56].tag = terminal;
	mapping[56].flag = 1;

	mapping[57].s.T = ID;
	strcpy(str, "ID");
	mapping[57].tag = terminal;
	mapping[57].flag = 1;

	mapping[58].s.T = NUM;
	strcpy(str, "NUM");
	mapping[58].tag = terminal;
	mapping[58].flag = 1;

	mapping[59].s.T = RNUM;
	strcpy(str, "RNUM");
	mapping[59].tag = terminal;
	mapping[59].flag = 1;

	mapping[60].s.T = EPS ;
	strcpy(str, "EPS ");
	mapping[60].tag = terminal;
	mapping[60].flag = 1;

	mapping[61].s.T = FEOF;
	strcpy(str, "FEOF");
	mapping[61].tag = terminal;
	mapping[61].flag = 1;

	mapping[62].s.T = BEGIN;
	strcpy(str, "BEGIN");
	mapping[62].tag = terminal;
	mapping[62].flag = 1;

	mapping[63].s.T = program;
	strcpy(str, "program");
	mapping[63].tag = nonTerminal;
	mapping[63].flag = 1;

	mapping[64].s.T = moduleDeclarations;
	strcpy(str, "moduleDeclarations");
	mapping[64].tag = nonTerminal;
	mapping[64].flag = 1;

	mapping[65].s.T = moduleDeclaration;
	strcpy(str, "moduleDeclaration");
	mapping[65].tag = nonTerminal;
	mapping[65].flag = 1;

	mapping[66].s.T = otherModules;
	strcpy(str, "otherModules");
	mapping[66].tag = nonTerminal;
	mapping[66].flag = 1;

	mapping[67].s.T = driverModule;
	strcpy(str, "driverModule");
	mapping[67].tag = nonTerminal;
	mapping[67].flag = 1;

	mapping[68].s.T = module;
	strcpy(str, "module");
	mapping[68].tag = nonTerminal;
	mapping[68].flag = 1;

	mapping[69].s.T = ret;
	strcpy(str, "ret");
	mapping[69].tag = nonTerminal;
	mapping[69].flag = 1;

	mapping[70].s.T = input_plist;
	strcpy(str, "input_plist");
	mapping[70].tag = nonTerminal;
	mapping[70].flag = 1;

	mapping[71].s.T = more_input_plist;
	strcpy(str, "more_input_plist");
	mapping[71].tag = nonTerminal;
	mapping[71].flag = 1;

	mapping[72].s.T = output_plist;
	strcpy(str, "output_plist");
	mapping[72].tag = nonTerminal;
	mapping[72].flag = 1;

	mapping[73].s.T = more_output_plist;
	strcpy(str, "more_output_plist");
	mapping[73].tag = nonTerminal;
	mapping[73].flag = 1;

	mapping[74].s.T = dataType;
	strcpy(str, "dataType");
	mapping[74].tag = nonTerminal;
	mapping[74].flag = 1;

	mapping[75].s.T = range_arrays;
	strcpy(str, "range_arrays");
	mapping[75].tag = nonTerminal;
	mapping[75].flag = 1;

	mapping[76].s.T = type;
	strcpy(str, "type");
	mapping[76].tag = nonTerminal;
	mapping[76].flag = 1;

	mapping[77].s.T = moduleDef;
	strcpy(str, "moduleDef");
	mapping[77].tag = nonTerminal;
	mapping[77].flag = 1;

	mapping[78].s.T = statements;
	strcpy(str, "statements");
	mapping[78].tag = nonTerminal;
	mapping[78].flag = 1;

	mapping[79].s.T = statement;
	strcpy(str, "statement");
	mapping[79].tag = nonTerminal;
	mapping[79].flag = 1;

	mapping[80].s.T = ioStmt;
	strcpy(str, "ioStmt");
	mapping[80].tag = nonTerminal;
	mapping[80].flag = 1;

	mapping[81].s.T = var2;
	strcpy(str, "var2");
	mapping[81].tag = nonTerminal;
	mapping[81].flag = 1;

	mapping[82].s.T = var;
	strcpy(str, "var");
	mapping[82].tag = nonTerminal;
	mapping[82].flag = 1;

	mapping[83].s.T = whichId;
	strcpy(str, "whichId");
	mapping[83].tag = nonTerminal;
	mapping[83].flag = 1;

	mapping[84].s.T = whichIndex;
	strcpy(str, "whichIndex");
	mapping[84].tag = nonTerminal;
	mapping[84].flag = 1;

	mapping[85].s.T = simpleStmt;
	strcpy(str, "simpleStmt");
	mapping[85].tag = nonTerminal;
	mapping[85].flag = 1;

	mapping[86].s.T = assignmentStmt;
	strcpy(str, "assignmentStmt");
	mapping[86].tag = nonTerminal;
	mapping[86].flag = 1;

	mapping[87].s.T = whichStmt;
	strcpy(str, "whichStmt");
	mapping[87].tag = nonTerminal;
	mapping[87].flag = 1;

	mapping[88].s.T = lvalueIDStmt;
	strcpy(str, "lvalueIDStmt");
	mapping[88].tag = nonTerminal;
	mapping[88].flag = 1;

	mapping[89].s.T = lvalueARRStmt;
	strcpy(str, "lvalueARRStmt");
	mapping[89].tag = nonTerminal;
	mapping[89].flag = 1;

	mapping[90].s.T = Index;
	strcpy(str, "Index");
	mapping[90].tag = nonTerminal;
	mapping[90].flag = 1;

	mapping[91].s.T = moduleReuseStmt;
	strcpy(str, "moduleReuseStmt");
	mapping[91].tag = nonTerminal;
	mapping[91].flag = 1;

	mapping[92].s.T = optional;
	strcpy(str, "optional");
	mapping[92].tag = nonTerminal;
	mapping[92].flag = 1;

	mapping[93].s.T = idList;
	strcpy(str, "idList");
	mapping[93].tag = nonTerminal;
	mapping[93].flag = 1;

	mapping[94].s.T = more_id;
	strcpy(str, "more_id");
	mapping[94].tag = nonTerminal;
	mapping[94].flag = 1;

	mapping[95].s.T = expression;
	strcpy(str, "expression");
	mapping[95].tag = nonTerminal;
	mapping[95].flag = 1;

	mapping[96].s.T = posExpr;
	strcpy(str, "posExpr");
	mapping[96].tag = nonTerminal;
	mapping[96].flag = 1;

	mapping[97].s.T = posExpr2;
	strcpy(str, "posExpr2");
	mapping[97].tag = nonTerminal;
	mapping[97].flag = 1;

	mapping[98].s.T = relExpr;
	strcpy(str, "relExpr");
	mapping[98].tag = nonTerminal;
	mapping[98].flag = 1;

	mapping[99].s.T = relExpr2;
	strcpy(str, "relExpr2");
	mapping[99].tag = nonTerminal;
	mapping[99].flag = 1;

	mapping[100].s.T = addExpr;
	strcpy(str, "addExpr");
	mapping[100].tag = nonTerminal;
	mapping[100].flag = 1;

	mapping[101].s.T = addExpr2;
	strcpy(str, "addExpr2");
	mapping[101].tag = nonTerminal;
	mapping[101].flag = 1;

	mapping[102].s.T = multExpr;
	strcpy(str, "multExpr");
	mapping[102].tag = nonTerminal;
	mapping[102].flag = 1;

	mapping[103].s.T = multExpr2;
	strcpy(str, "multExpr2");
	mapping[103].tag = nonTerminal;
	mapping[103].flag = 1;

	mapping[104].s.T = basicExpr;
	strcpy(str, "basicExpr");
	mapping[104].tag = nonTerminal;
	mapping[104].flag = 1;

	mapping[105].s.T = logOp;
	strcpy(str, "logOp");
	mapping[105].tag = nonTerminal;
	mapping[105].flag = 1;

	mapping[106].s.T = pmOp;
	strcpy(str, "pmOp");
	mapping[106].tag = nonTerminal;
	mapping[106].flag = 1;

	mapping[107].s.T = mdOp;
	strcpy(str, "mdOp");
	mapping[107].tag = nonTerminal;
	mapping[107].flag = 1;

	mapping[108].s.T = relOp;
	strcpy(str, "relOp");
	mapping[108].tag = nonTerminal;
	mapping[108].flag = 1;

	mapping[109].s.T = declareStmt;
	strcpy(str, "declareStmt");
	mapping[109].tag = nonTerminal;
	mapping[109].flag = 1;

	mapping[110].s.T = conditionalStmt;
	strcpy(str, "conditionalStmt");
	mapping[110].tag = nonTerminal;
	mapping[110].flag = 1;

	mapping[111].s.T = caseStmt;
	strcpy(str, "caseStmt");
	mapping[111].tag = nonTerminal;
	mapping[111].flag = 1;

	mapping[112].s.T = caseStmts;
	strcpy(str, "caseStmts");
	mapping[112].tag = nonTerminal;
	mapping[112].flag = 1;

	mapping[113].s.T = value;
	strcpy(str, "value");
	mapping[113].tag = nonTerminal;
	mapping[113].flag = 1;

	mapping[114].s.T = Default;
	strcpy(str, "Default");
	mapping[114].tag = nonTerminal;
	mapping[114].flag = 1;

	mapping[115].s.T = iterativeStmt;
	strcpy(str, "iterativeStmt");
	mapping[115].tag = nonTerminal;
	mapping[115].flag = 1;

	mapping[116].s.T = range;
	strcpy(str, "range");
	mapping[116].tag = nonTerminal;
	mapping[116].flag = 1;
}














