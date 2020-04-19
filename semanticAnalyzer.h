#ifndef _SEMANTICANALYZERH
#define _SEMANTICANALYZERH
#include "ASTNodeDef.h"
#include "symbolTableDef.h"

extern LocalTable *newLocalTable();
extern LocalTable *populateLocalTable(struct Context context,LocalTable *parentOfparent,struct ASTNode *root,int baseOffset);
extern LocalTable *populateConditionNodeLocalTable(struct Context context,LocalTable *parentOfparent,struct ASTNode *head,int baseOffset);

extern int insertLocalTable(Context context,LocalTable *localTable,struct ASTNode *root,int baseOffset);
extern VariableEntry *searchLocalTable(LocalTable *localTable,char *string);

extern VariableEntry *checkDeclarationBeforeUse(Context context,LocalTable *parent, char name[], int lineNumber);    //return 1 if no error else 0
extern void secondPass(struct ASTNode *root, SymbolTable symbolTable,char funcName[]);

extern void resetStringList();
extern int checkStringList(char *name);

#endif