#ifndef _TYPECHECKERH
#define _TYPECHECKERH
#include "symbolTableDef.h"
#include "ASTNodeDef.h"

extern int assertTypeEquality(Type type1, Type type2, int lineNumber);
extern VariableEntry *checkDeclarationBeforeUse(Context context,LocalTable *parent, char name[], int lineNumber);    //return 1 if no error else 0
extern int assertNotForbidden(Context context,char name[], int lineNumber);

extern int setModifyFlagExpression(Context context,LocalTable *parent,struct ASTNode *root,int bit);
extern Type validateExpression(Context context,LocalTable *parent,struct ASTNode *root);
extern void secondPass(struct ASTNode *root, Context context);

#endif