
#ifndef _SYMBOLTABLEH
#define _SYMBOLTABLEH
#include "ASTNodeDef.h"
#include "symbolTableDef.h"


extern int computeHashFromString(char *);// Copy from grammarInitDef.h

extern ParameterList *populateParaList(struct ASTNode *root,int baseOffset);
extern LocalTable *populateLocalTable(struct Context context,LocalTable *parentOfparent,struct ASTNode *root,int baseOffset);

extern FunctionTable *insertSymbolTable(SymbolTable symbolTable,struct ASTNode *root);
extern FunctionTable *searchSymbolTable(SymbolTable symbolTable,char *string);
extern LocalTable *populateConditionNodeLocalTable(struct Context context,LocalTable *parentOfparent,struct ASTNode *head,int baseOffset);

extern int insertLocalTable(Context context,LocalTable *localTable,struct ASTNode *root,int baseOffset);
extern VariableEntry *searchLocalTable(LocalTable *localTable,char *string);

extern void addChild(LocalTable *localTable, LocalTable *siblingTable);
extern LocalTable *newLocalTable();
extern FunctionTable *newFunctionNode(char *funcName);
extern int getWidth(Type type);
extern int getWidthLocal(Type type);
extern SymbolTable *populateSymbolTable(struct ASTNode *root);
extern void printSymbolTable(SymbolTable *symbolTable);

#endif