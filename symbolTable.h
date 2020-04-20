/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _SYMBOLTABLEH
#define _SYMBOLTABLEH
#include "ASTNodeDef.h"
#include "symbolTableDef.h"


extern int computeHashFromString(char *);// Copy from grammarInitDef.h

extern ParameterList *populateParaList(struct ASTNode *root,int baseOffset);

extern FunctionTable *insertSymbolTable(SymbolTable symbolTable,struct ASTNode *root);
extern FunctionTable *searchSymbolTable(SymbolTable symbolTable,char *string);


extern FunctionTable *newFunctionNode(char *funcName);
extern int getWidth(Type type);
extern SymbolTable *populateSymbolTable(struct ASTNode *root);
extern void printSymbolTable(SymbolTable *);
extern void printRecordWidth(SymbolTable *);
extern void printArrayVariables(SymbolTable *);

#endif
