#ifndef _TYPECHECKERC
#define _TYPECHECKERC
#include "symbolTableDef.h"
#include "ASTNodeDef.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "typeChecker.h"
int assertNotForbidden(Context context,char name[], int lineNumber)
{
    VariableEntry* ptr=context.forbiddenVariables;
    int correct=1;
    while(ptr!=NULL)
    {
        if(strcmp(ptr->varName,name)==0)
        {
            printf("Semantic Error on  line %d: Attempt to modify variable %s used in a for loop at line %d\n",lineNumber,name,ptr->lineNumber);
            correct=0;
        }
        ptr=ptr->next;
    }
    return correct;
}

int assertTypeEquality(Type type1, Type type2, int lineNumber){
    if(type1.arrayFlag == 1 && type2.arrayFlag == 1){
        if(type1.type == type2.type){
            if(type1.isStatic == 1 && type2.isStatic == 1){
                if(type1.low.bound == type2.low.bound && type1.high.bound == type2.high.bound) return 1;
            }

            else if(type1.isStatic == 1 || type2.isStatic == 1){ //if only one is static
                    if(type1.tagLow == 0 && type2.tagLow ==0 && type1.low.bound != type2.low.bound) return 0;
                    if(type1.tagHigh == 0 && type2.tagHigh ==0 && type1.high.bound != type2.high.bound) return 0;
            }
            return 1;
        }
    }
    else if(type1.arrayFlag == 0 && type2.arrayFlag == 0){
    if(type1.type == DT_INTEGER && type2.type == DT_INTEGER) return 1;
    if(type1.type == DT_REAL && type2.type == DT_REAL) return 1;
    if(type1.type == DT_BOOLEAN && type2.type == DT_BOOLEAN) return 1;
    }
    return 0;
}             

Type validateExpression(Context context,LocalTable *parent,struct ASTNode *root)
{    
    Type type,leftType,type2;
    VariableEntry *varptr;
    switch(root->tag)
    {   
		case BOOL_NODE:
			type.type=DT_BOOLEAN;
			type.arrayFlag=0;
			return type;
		case ID_NODE:
			varptr=checkDeclarationBeforeUse(context,parent,root->node.idNode.varName,root->lineNumber);
			leftType=varptr->type;
			if(root->node.idNode.index!=NULL){
			    if(leftType.arrayFlag==0){
			        printf("Semantic Error on line %d: Cannot index a non-array variable %s\n",root->lineNumber,root->node.idNode.varName);
			    }
			    if(leftType.isStatic==1&&root->node.idNode.index->tag==NUM_NODE)
			    {
			        if(leftType.low.bound > root->node.idNode.index->node.numNode.num)
			        {
			              printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.idNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
			        }
			        if(leftType.high.bound < root->node.idNode.index->node.numNode.num)
			        {
			              printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.idNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
			        }
			        leftType.arrayFlag=0;
			    }
			}        
			return leftType;
		case NUM_NODE:
			type.type=DT_INTEGER;
			type.arrayFlag=0;
			return type;
			case RNUM_NODE:
			type.type=DT_REAL;
			type.arrayFlag=0;
			return type;
		case UNARY_NODE:
			type=validateExpression(context,parent,root->node.unaryNode.expr);
			if(type.type == DT_BOOLEAN){
			 printf("Error on line number:%d, unary operations cannot be applied on Boolean expressions",root->lineNumber);
			}
			if(type.arrayFlag==1){
			 printf("Error on line number:%d, unary operations cannot be applied on an array",root->lineNumber);
			}
            return type;
		case BINARY_NODE:
			type=validateExpression(context,parent,root->node.binaryNode.expr1);
			type2=validateExpression(context,parent,root->node.binaryNode.expr2);
			if(type.arrayFlag==1 || type2.arrayFlag==1 )
			{
			    printf("Error on line number:%d, binary operations cannot be applied on an array",root->lineNumber);
			    return type;
			}
			switch(root->node.binaryNode.op) //enum Operator{OP_PLUS,OP_MINUS,OP_MUL,OP_DIV,OP_AND,OP_OR,OP_LT, OP_LE, OP_GE, OP_GT, OP_EQ, OP_NE};
			{
			    case OP_PLUS: case OP_MINUS: case OP_MUL: case OP_DIV:
			        if(type.type==DT_BOOLEAN || type2.type==DT_BOOLEAN)
			        {
			            printf("Semantic error on line %d: Cannot perform arithmetic operation on boolean type operands.\n",root->lineNumber);
			            return type;
			        }
			        else if(type.type!=type2.type)
			        {
			            printf("Semantic error on line %d: Cannot perform arithmetic operation with one int and one real operand.\n",root->lineNumber);
			            return type;
			        }
			        return type;
			    case OP_AND:case OP_OR:
			        if(type.type!=DT_BOOLEAN || type2.type!=DT_BOOLEAN)
			        {
			            printf("Semantic error on line %d: Cannot perform logical operation on non-boolean type operands.\n",root->lineNumber);
			            return type;
			        }
			        return type;
			    case OP_LT:case OP_LE:case OP_GT:case OP_GE:
			        if(type.type==DT_BOOLEAN || type2.type==DT_BOOLEAN)
			        {
			            printf("Semantic error on line %d: Cannot perform inequality operations on boolean type operands.\n",root->lineNumber);
			            return type;
			        }
			        else if(type.type!=type2.type)
			        {
			            printf("Semantic error on line %d: Cannot perform relational operations with one int and one real operand.\n",root->lineNumber);
			            return type;
			        }
			        type.type=DT_BOOLEAN;
			        return type;
			    case OP_EQ:case OP_NE:
			        if(type.type!=type2.type)
			        {
			            printf("Semantic error on line %d: Cannot perform equality operations with differing datatype operands.\n",root->lineNumber);
			            return type;
			        }
			        type.type=DT_BOOLEAN;
			        return type;
			            
			}  
      		return type;    
    }
    return type;
}

          
int setModifyFlagExpression(Context context,LocalTable *parent,struct ASTNode *root,int bit){
    if(root->tag==NUM_NODE || root->tag==RNUM_NODE || root->tag==BOOL_NODE)
        return 0;
    if(root->tag==ID_NODE){
        VariableEntry *ptr;
        ptr=checkDeclarationBeforeUse(context,parent,root->node.idNode.varName,root->lineNumber);
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