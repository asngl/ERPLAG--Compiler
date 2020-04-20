/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _TYPECHECKERH
#define _TYPECHECKERH
#include "symbolTableDef.h"
#include "ASTNodeDef.h"

extern int assertTypeEquality(Type type1, Type type2, int lineNumber);
extern int assertNotForbidden(Context context,char name[], int lineNumber);

extern int setModifyFlagExpression(Context context,LocalTable *parent,struct ASTNode *root,int bit);
extern Type validateExpression(Context context,LocalTable *parent,struct ASTNode *root);

#endif