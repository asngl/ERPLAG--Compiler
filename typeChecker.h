#ifndef _TYPECHECKERH
#define _TYPECHECKERH
#include "symbolTableDef.h"
#include "ASTNodeDef.h"

int assertTypeEquality(Type type1, Type type2, int lineNumber);
VariableEntry *checkDeclarationBeforeUse(Context context,LocalTable *parent, char name[], int lineNumber);    //return 1 if no error else 0
int assertNotForbidden(Context context,char name[], int lineNumber);

int setModifyFlagExpression(Context context,LocalTable *parent,struct ASTNode *root,int bit);
int validateExpression(Context context,LocalTable *parent,struct ASTNode *root);

#endif