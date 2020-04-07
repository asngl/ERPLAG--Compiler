#ifndef _TYPECHECKERC
#define _TYPECHECKERC


int assertNotForbidden(Context context,char name[], int lineNumber)
{
    VariableEntry* ptr=context.forbiddenVariables;
    int correct=1;
    while(ptr!=NULL)
    {
        if(strcmp(ptr->varName,name)==0)
        {
            printf("Semantic Error on  line %d: Attempt to modify variable %s used in a for loop at line %d\n",lineNumber,name,ptr->lineNumber);correct=0;
        }
    }
    return correct;
}

int validateExpression(Context context,LocalTable *parent,struct ASTNode *root)
{
	return 0;
}

int setModifyFlagExpression(Context context,LocalTable *parent,struct ASTNode *root,int bit){
    if(root->tag==NUM_NODE || root->tag==RNUM_NODE || root->tag==BOOL_NODE)
        return 0;
    if(root->tag==ID_NODE){
        VariableEntry *ptr;
        ptr=checkDeclarationBeforeUse(context,parent,root->node.idNode.varName);
        if(bit == 0){
            ptr->initFlag=0;
            return 0;
        }else
        if(bit == -1){
            if(ptr->initFlag==1){
                ptr->initFlag=-1;
                return 1;
            }
            ptr->initFlag=-1;
            return 0;
            
        }
    }
    if(root->tag==UNARY_NODE){
        return setModifyFlagExpression(context,parent,root->node.unaryNode.expr,bit);
    }
    if(root->tag==BINARY_NODE)
        return (setModifyFlagExpression(context,parent,root->node.binaryNode.expr1,bit) | setModifyFlagExpression(context,parent,root->node.binaryNode.expr2,bit));
    else{
        printf("Wrong node encountered in Expression in ast\n");
        return 0;
    }
}

VariableEntry *checkDeclarationBeforeUse(Context context,LocalTable *parent, char name[], int lineNumber){
    VariableEntry *currVar;
    currVar=context.outputList;
    while(currVar!=NULL){
        if(strcmp(currVar->varName,name)==0){
            return currVar;
        }
        currVar=currVar->next;
    }
    for(int i=0;i<MOD;i++){
        currVar=(parent->variableTable)[i];
        while(currVar!=NULL){
            if(strcmp(currVar->varName,name)==0){
                return currVar;
            }
            currVar=currVar->next;
        }
    }
    currVar=context.inputList;
    while(currVar!=NULL){
        if(strcmp(currVar->varName,name)==0){
            return currVar;
        }
        currVar=currVar->next;
    }
    if(parent->parent==NULL){
        printf("Error: No declaration of variable:%s used on line number:%d found",name,lineNumber);
        return NULL;
    }
    return checkDeclarationBeforeUse(context,parent->parent,name,lineNumber);
    
}

#endif