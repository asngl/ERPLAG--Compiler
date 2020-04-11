#ifndef _TYPECHECKERC
#define _TYPECHECKERC
#include "symbolTableDef.h"
#include "ASTNodeDef.h"
#include "symbolTable.h"
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
	if(type1.type==DT_ERROR || type2.type==DT_ERROR)
		return 1;
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
			if(varptr==NULL)
			{
				type.type=DT_ERROR;
				type.arrayFlag=0;
				return type;
			}
			leftType=varptr->type;
			if(root->node.idNode.index!=NULL){
			    if(leftType.arrayFlag==0){
			        printf("Semantic Error on line %d: Cannot index a non-array variable %s\n",root->lineNumber,root->node.idNode.varName);
			        type.type=DT_ERROR;
					type.arrayFlag=0;
					return type;
			    }
			    if(leftType.isStatic==1&&root->node.idNode.index->tag==NUM_NODE)
			    {
			        if(leftType.low.bound > root->node.idNode.index->node.numNode.num)
			        {
			            printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.idNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
				        type.type=DT_ERROR;
						type.arrayFlag=0;
						return type;
			        }
			        if(leftType.high.bound < root->node.idNode.index->node.numNode.num)
			        {
			            printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.idNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
			            type.type=DT_ERROR;
						type.arrayFlag=0;
						return type;
			        }
			        leftType.arrayFlag=0;
			    }
                leftType.arrayFlag=0;
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
			if(type.type==DT_ERROR)return type;
			if(type.type == DT_BOOLEAN){
				printf("Error on line number:%d, unary operations cannot be applied on Boolean expressions",root->lineNumber);
				type.type=DT_ERROR;
				type.arrayFlag=0;
				return type;
			}
			if(type.arrayFlag==1){
				printf("Error on line number:%d, unary operations cannot be applied on an array",root->lineNumber);
				type.type=DT_ERROR;
				type.arrayFlag=0;
				return type;
			}
            return type;
		case BINARY_NODE:
			type=validateExpression(context,parent,root->node.binaryNode.expr1);
			type2=validateExpression(context,parent,root->node.binaryNode.expr2);
			if(type.type==DT_ERROR || type2.type==DT_ERROR)
			{
				type.type=DT_ERROR;
				type.arrayFlag=0;
				return type;
			}
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
        printf("Error: No declaration of variable:%s used on line number:%d found\n",name,lineNumber);
        return NULL;
    }
    return checkDeclarationBeforeUse(context,parent->parent,name,lineNumber);
    
}

void secondPass(struct ASTNode *root, Context context){

    if(root==NULL)
        return;
    FunctionTable *funcptr;
    struct ASTNode *ptr;
    ParameterList *paraptr;
    VariableEntry *varptr,*para_var_ptr;
    switch(root->tag){
        case PROGRAM_NODE:
            secondPass(root->node.programNode.moduleDeclarations,context);
            secondPass(root->node.programNode.otherModules1,context);
            secondPass(root->node.programNode.driverModule,context);
            secondPass(root->node.programNode.otherModules2,context);
            break;
        case MODULE_DECLARE_NODE:
            secondPass(root->node.moduleDeclareNode.next,context);
            break;
        case ID_NODE:
            secondPass(root->node.idNode.index, context);
            break;
        case MODULE_NODE:
            secondPass(root->node.moduleNode.inputList,context);
            secondPass(root->node.moduleNode.ret,context);
            secondPass(root->node.moduleNode.body,context);
            secondPass(root->node.moduleNode.next,context);
            break;
        case PARA_LIST_NODE:
            secondPass(root->node.paraListNode.Range,context);
            secondPass(root->node.paraListNode.next,context);
            break;
        case NUM_NODE:
            
            break;
        case RNUM_NODE:
            
            break;
        case BOOL_NODE:
           
            break;
        case INPUT_NODE:
           
            secondPass(root->node.inputNode.next,context);
            break;
        case OUTPUT_NODE:
           
            secondPass(root->node.outputNode.value,context);
            secondPass(root->node.outputNode.next,context);
            break;
        case RANGE_NODE:
           
            secondPass(root->node.rangeNode.Range1,context);
            secondPass(root->node.rangeNode.Range2,context);
            break;
        case ASSIGN_NODE:
          
            secondPass(root->node.assignNode.index,context);
            secondPass(root->node.assignNode.expr,context);
            secondPass(root->node.assignNode.next,context);
            break;
        case MODULE_REUSE_NODE:
           
            secondPass(root->node.moduleReuseNode.optional,context);
            secondPass(root->node.moduleReuseNode.idList,context);
            secondPass(root->node.moduleReuseNode.next,context);
            funcptr=searchSymbolTable(*(context.symbolTable),root->node.moduleReuseNode.id);
            if(funcptr==NULL)
                return;
            if(funcptr->defineFlag!=1){
                printf("Error on line number: %d, Function : %s is declared but not defined",root->lineNumber,root->node.moduleReuseNode.id);
                return;
            }
            ptr = root->node.moduleReuseNode.idList;
            paraptr = funcptr->inputParaList;
            while(ptr!=NULL || paraptr!=NULL){
                varptr = checkDeclarationBeforeUse(context,funcptr->localTable, ptr->node.idListNode.varName,ptr->lineNumber);
                if(varptr==NULL){
                    paraptr=paraptr->next;
                    ptr=ptr->node.idListNode.next;
                    continue;
                }
                if(assertTypeEquality(varptr->type,paraptr->type,ptr->lineNumber)==0){
                    printf("Error on line number:%d , type mismatch for input variable %s and parameter %s\n",ptr->lineNumber,varptr->varName,paraptr->varName);
                }
                paraptr=paraptr->next;
                ptr=ptr->node.idListNode.next;
            }
            if(ptr!=NULL){
                printf("Error on line number:%d, more input variables used in the statement than actual needed\n",root->lineNumber);
            } 
            if(paraptr!=NULL){
                printf("Error on line number:%d, less input variables used in the statement than actual needed\n",root->lineNumber);
            }      
            if(paraptr)
            ptr = root->node.moduleReuseNode.optional;
            paraptr = funcptr->outputParaList;
            while(ptr!=NULL || paraptr!=NULL){
                varptr = checkDeclarationBeforeUse(context,funcptr->localTable, ptr->node.idListNode.varName,ptr->lineNumber);
                if(varptr==NULL){
                    paraptr=paraptr->next;
                    ptr=ptr->node.idListNode.next;
                    continue;
                }
                if(assertTypeEquality(varptr->type,paraptr->type,ptr->lineNumber)==0){
                    printf("Error on line number:%d , type mismatch for output variable %s and parameter %s \n",ptr->lineNumber,varptr->varName,paraptr->varName);
                }
                paraptr=paraptr->next;
                ptr=ptr->node.idListNode.next;
            }
            if(ptr!=NULL){
                printf("Error on line number:%d, more output variables used in the statement than actual needed\n",root->lineNumber);
            } 
            if(paraptr!=NULL){
                printf("Error on line number:%d, less output variables used in the statement than actual needed\n",root->lineNumber);
            }
            break;
        case ID_LIST_NODE:
           
            secondPass(root->node.idListNode.next,context);
            break;
        case DECLARE_NODE:
           
            secondPass(root->node.declareNode.idList,context);
            secondPass(root->node.declareNode.Range,context);
            secondPass(root->node.declareNode.next,context);
            break;
        case CONDITION_NODE:
           
            secondPass(root->node.conditionNode.Case,context);
            secondPass(root->node.conditionNode.Default,context);
            secondPass(root->node.conditionNode.next,context);
            break;
        case CASE_NODE:
            
            secondPass(root->node.caseNode.value,context);
            secondPass(root->node.caseNode.stmt,context);
            secondPass(root->node.caseNode.next,context);
            break;
        case FOR_NODE:
           
            secondPass(root->node.forNode.range,context);
            secondPass(root->node.forNode.stmt,context);
            secondPass(root->node.forNode.next,context);
            break;
        case WHILE_NODE:
           
            secondPass(root->node.whileNode.expr,context);
            secondPass(root->node.whileNode.stmt,context);
            secondPass(root->node.whileNode.next,context);
            break;
        case UNARY_NODE:
           
            secondPass(root->node.unaryNode.expr,context);
            break;
        case BINARY_NODE:
            
            secondPass(root->node.binaryNode.expr1,context);
            secondPass(root->node.binaryNode.expr2,context);
            break;
        default:
           break;
    }
}

#endif