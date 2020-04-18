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
            printf("Line %d : Attempt to modify variable %s used in a for loop at line %d\n",lineNumber,name,ptr->lineNumber);
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
            if(type1.tagLow == 0 && type2.tagLow ==0 && type1.low.bound != type2.low.bound) return 0;
            if(type1.tagHigh == 0 && type2.tagHigh ==0 && type1.high.bound != type2.high.bound) return 0;
            return 1;
        }
        else return 0;
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
            root->localTableEntry=varptr;
			if(varptr==NULL)
			{
				type.type=DT_ERROR;
				type.arrayFlag=0;
				return type;
			}
			leftType=varptr->type;
			if(root->node.idNode.index!=NULL){
			    if(leftType.arrayFlag==0){
			        printf("Line %d : Cannot index a non-array variable %s\n",root->lineNumber,root->node.idNode.varName);
			        type.type=DT_ERROR;
					type.arrayFlag=0;
					return type;
			    }
			    if(leftType.isStatic==1&&root->node.idNode.index->tag==NUM_NODE)
			    {
			        if(leftType.low.bound > root->node.idNode.index->node.numNode.num)
			        {
			            printf("Line %d : index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.idNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
				        type.type=DT_ERROR;
						type.arrayFlag=0;
						return type;
			        }
			        if(leftType.high.bound < root->node.idNode.index->node.numNode.num)
			        {
			            printf("Line %d : index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.idNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
			            type.type=DT_ERROR;
						type.arrayFlag=0;
						return type;
			        }
			        leftType.arrayFlag=0;
			    }
                if(root->node.idNode.index->tag==ID_NODE)
                {
                    varptr=checkDeclarationBeforeUse(context,parent,root->node.idNode.index->node.idNode.varName,root->lineNumber);
                    root->node.idNode.index->localTableEntry=varptr;
                    if(varptr!=NULL)
                    {
                        if(varptr->type.type!=DT_INTEGER)
                        {
                            printf("Line %d : Index Variable %s is not of Integer type\n",root->lineNumber,root->node.idNode.index->node.idNode.varName);
                        }
                        else if(varptr->type.arrayFlag==1)
                        {
                            printf("Line %d : Index Variable %s cannot be of Array type\n",root->lineNumber,root->node.idNode.index->node.idNode.varName);
                        }
                        else if(varptr->initFlag==0)
                        {
                            printf("Line %d : Index Variable %s not initialised\n",root->lineNumber,root->node.idNode.index->node.idNode.varName);
                        }
                    }
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
            root->node.unaryNode.type=type.type;
			if(type.type==DT_ERROR)return type;
			if(type.type == DT_BOOLEAN){
				printf("Line %d : unary operations cannot be applied on Boolean expressions\n",root->lineNumber);
				type.type=DT_ERROR;
				type.arrayFlag=0;
				return type;
			}
			if(type.arrayFlag==1){
				printf("Line %d : unary operations cannot be applied on an array\n",root->lineNumber);
				type.type=DT_ERROR;
				type.arrayFlag=0;
				return type;
			}
            return type;
		case BINARY_NODE:
			type=validateExpression(context,parent,root->node.binaryNode.expr1);
			type2=validateExpression(context,parent,root->node.binaryNode.expr2);
            root->node.binaryNode.childType=type.type;
			if(type.type==DT_ERROR || type2.type==DT_ERROR)
			{
				type.type=DT_ERROR;
				type.arrayFlag=0;
				return type;
			}
			if(type.arrayFlag==1 || type2.arrayFlag==1 )
			{
			    printf("Line %d : binary operations cannot be applied on an array\n",root->lineNumber);
			    return type;
			}
			switch(root->node.binaryNode.op) //enum Operator{OP_PLUS,OP_MINUS,OP_MUL,OP_DIV,OP_AND,OP_OR,OP_LT, OP_LE, OP_GE, OP_GT, OP_EQ, OP_NE};
			{
			    case OP_PLUS: case OP_MINUS: case OP_MUL: case OP_DIV:
			        if(type.type==DT_BOOLEAN || type2.type==DT_BOOLEAN)
			        {
			            printf("Line %d : Cannot perform arithmetic operation on boolean type operands.\n",root->lineNumber);
                        type.type=DT_ERROR;
                        type.arrayFlag=0;
			            return type;
			        }
			        else if(type.type!=type2.type)
			        {
			            printf("Line %d : Cannot perform arithmetic operation with one int and one real operand.\n",root->lineNumber);
                        type.type=DT_ERROR;
                        type.arrayFlag=0;
			            return type;
			        }
                    root->node.binaryNode.type=type.type;
			        return type;
			    case OP_AND:case OP_OR:
			        if(type.type!=DT_BOOLEAN || type2.type!=DT_BOOLEAN)
			        {
			            printf("Line %d : Cannot perform logical operation on non-boolean type operands.\n",root->lineNumber);
                        type.type=DT_ERROR;
                        type.arrayFlag=0;
			            return type;
			        }
                    root->node.binaryNode.type=type.type;
			        return type;
			    case OP_LT:case OP_LE:case OP_GT:case OP_GE:
			        if(type.type==DT_BOOLEAN || type2.type==DT_BOOLEAN)
			        {
			            printf("Line %d : Cannot perform inequality operations on boolean type operands.\n",root->lineNumber);
                        type.type=DT_ERROR;
                        type.arrayFlag=0;
			            return type;
			        }
			        else if(type.type!=type2.type)
			        {
			            printf("Line %d : Cannot perform relational operations with one int and one real operand.\n",root->lineNumber);
                        type.type=DT_ERROR;
                        type.arrayFlag=0;
			            return type;
			        }
			        type.type=DT_BOOLEAN;
                    root->node.binaryNode.type=type.type;
			        return type;
			    case OP_EQ:case OP_NE:
			        if(type.type!=type2.type)
			        {
			            printf("Line %d : Cannot perform equality operations with differing datatype operands.\n",root->lineNumber);
                        type.type=DT_ERROR;
                        type.arrayFlag=0;
			            return type;
			        }
			        type.type=DT_BOOLEAN;
                    root->node.binaryNode.type=type.type;
			        return type;
			            
			}  
      		return type;    
    }
    return type;
}

VariableEntry *getVariableEntry(Context context,LocalTable *parent, char name[], int lineNumber){
    VariableEntry *currVar;
    currVar=context.outputList;
    while(currVar!=NULL){
        //printf("%s",currVar->varName);
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
        
        return NULL;
    }
    return checkDeclarationBeforeUse(context,parent->parent,name,lineNumber);
    
}
          
int setModifyFlagExpression(Context context,LocalTable *parent,struct ASTNode *root,int bit){
    if(root->tag==NUM_NODE || root->tag==RNUM_NODE || root->tag==BOOL_NODE)
        return 0;
    if(root->tag==ID_NODE){
        VariableEntry *ptr;
        ptr=getVariableEntry(context,parent,root->node.idNode.varName,root->lineNumber);
        if(ptr==NULL)
            return 0;
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
        //printf("%s",currVar->varName);
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
        printf("Line %d : Variable %s is not declared\n",lineNumber,name);
        return NULL;
    }
    return checkDeclarationBeforeUse(context,parent->parent,name,lineNumber);
    
}

void secondPass(struct ASTNode *root, SymbolTable symbolTable,char funcName[]){

    if(root==NULL)
        return;
    Context context;
    context.symbolTable = &symbolTable;
    strcpy(context.funcName,funcName);
    context.inputList=NULL;
    context.outputList=NULL;
    FunctionTable *funcptr, *currfunc;
    struct ASTNode *ptr;
    ParameterList *paraptr;
    VariableEntry *varptr,*para_var_ptr;
    switch(root->tag){
        case PROGRAM_NODE:
            secondPass(root->node.programNode.moduleDeclarations,symbolTable,funcName);
            secondPass(root->node.programNode.otherModules1,symbolTable,funcName);
            secondPass(root->node.programNode.driverModule,symbolTable,"driver");
            secondPass(root->node.programNode.otherModules2,symbolTable,funcName);
            break;
        case MODULE_DECLARE_NODE:
            secondPass(root->node.moduleDeclareNode.next,symbolTable,funcName);
            break;
        case ID_NODE:
            secondPass(root->node.idNode.index, symbolTable,funcName);
            break;
        case MODULE_NODE:
            secondPass(root->node.moduleNode.inputList,symbolTable,funcName);
            secondPass(root->node.moduleNode.ret,symbolTable,funcName);
            secondPass(root->node.moduleNode.body,symbolTable,root->node.moduleNode.moduleName);
            secondPass(root->node.moduleNode.next,symbolTable,funcName);
            break;
        case PARA_LIST_NODE:
            secondPass(root->node.paraListNode.Range,symbolTable,funcName);
            secondPass(root->node.paraListNode.next,symbolTable,funcName);
            break;
        case NUM_NODE:
            
            break;
        case RNUM_NODE:
            
            break;
        case BOOL_NODE:
           
            break;
        case INPUT_NODE:
           
            secondPass(root->node.inputNode.next,symbolTable,funcName);
            break;
        case OUTPUT_NODE:
           
            secondPass(root->node.outputNode.value,symbolTable,funcName);
            secondPass(root->node.outputNode.next,symbolTable,funcName);
            break;
        case RANGE_NODE:
           
            secondPass(root->node.rangeNode.Range1,symbolTable,funcName);
            secondPass(root->node.rangeNode.Range2,symbolTable,funcName);
            break;
        case ASSIGN_NODE:
          
            secondPass(root->node.assignNode.index,symbolTable,funcName);
            secondPass(root->node.assignNode.expr,symbolTable,funcName);
            secondPass(root->node.assignNode.next,symbolTable,funcName);
            break;
        case MODULE_REUSE_NODE:
           
            secondPass(root->node.moduleReuseNode.optional,symbolTable,funcName);
            secondPass(root->node.moduleReuseNode.idList,symbolTable,funcName);
            funcptr=searchSymbolTable(symbolTable,root->node.moduleReuseNode.id);
            currfunc=searchSymbolTable(symbolTable,funcName);
            if(currfunc==NULL){
            	printf("Cannot access current function \n");
            	return;
            }
            if(funcptr==NULL)
                return;
            if(funcptr->defineFlag!=1){
                printf("Line %d : Function : %s is declared but not defined\n",root->lineNumber,root->node.moduleReuseNode.id);
                return;
            }
            ptr = root->node.moduleReuseNode.idList;
            paraptr = funcptr->inputParaList;
            while(ptr!=NULL && paraptr!=NULL){
                if(ptr->localTableEntry==NULL){
                	ptr=ptr->node.idListNode.next;
                	paraptr=paraptr->next;
                	continue;
                }
                if(assertTypeEquality(paraptr->type,ptr->localTableEntry->type,ptr->lineNumber)==0){
                    printf("Line %d : type mismatch for input variable %s and parameter %s\n",ptr->lineNumber,ptr->node.idListNode.varName,paraptr->varName);
                }
                paraptr=paraptr->next;
                ptr=ptr->node.idListNode.next;
            }
            if(ptr!=NULL){
                printf("Line %d : more input variables used in the statement than actual needed\n",root->lineNumber);
            } 
            if(paraptr!=NULL){
                printf("Line %d : less input variables used in the statement than actual needed\n",root->lineNumber);
            }      
            ptr = root->node.moduleReuseNode.optional;
            paraptr = funcptr->outputParaList;
            while(ptr!=NULL && paraptr!=NULL){
                if(ptr->localTableEntry==NULL){
                	ptr=ptr->node.idListNode.next;
                	paraptr=paraptr->next;
                	continue;
                }
                if(assertTypeEquality(paraptr->type,ptr->localTableEntry->type,ptr->lineNumber)==0){
                    printf("Line %d : type mismatch for input variable %s and parameter %s\n",ptr->lineNumber,ptr->node.idListNode.varName,paraptr->varName);
                }
                paraptr=paraptr->next;
                ptr=ptr->node.idListNode.next;
            }
            if(ptr!=NULL){
                printf("Line %d : more output variables used in the statement than actual needed\n",root->lineNumber);
            } 
            if(paraptr!=NULL){
                printf("Line %d : less output variables used in the statement than actual needed\n",root->lineNumber);
            }
            secondPass(root->node.moduleReuseNode.next,symbolTable,funcName);
            break;
        case ID_LIST_NODE:
           
            secondPass(root->node.idListNode.next,symbolTable,funcName);
            break;
        case DECLARE_NODE:
           
            secondPass(root->node.declareNode.idList,symbolTable,funcName);
            secondPass(root->node.declareNode.Range,symbolTable,funcName);
            secondPass(root->node.declareNode.next,symbolTable,funcName);
            break;
        case CONDITION_NODE:
           
            secondPass(root->node.conditionNode.Case,symbolTable,funcName);
            secondPass(root->node.conditionNode.Default,symbolTable,funcName);
            secondPass(root->node.conditionNode.next,symbolTable,funcName);
            break;
        case CASE_NODE:
            
            secondPass(root->node.caseNode.value,symbolTable,funcName);
            secondPass(root->node.caseNode.stmt,symbolTable,funcName);
            secondPass(root->node.caseNode.next,symbolTable,funcName);
            break;
        case FOR_NODE:
           
            secondPass(root->node.forNode.range,symbolTable,funcName);
            secondPass(root->node.forNode.stmt,symbolTable,funcName);
            secondPass(root->node.forNode.next,symbolTable,funcName);
            break;
        case WHILE_NODE:
           
            secondPass(root->node.whileNode.expr,symbolTable,funcName);
            secondPass(root->node.whileNode.stmt,symbolTable,funcName);
            secondPass(root->node.whileNode.next,symbolTable,funcName);
            break;
        case UNARY_NODE:
           
            secondPass(root->node.unaryNode.expr,symbolTable,funcName);
            break;
        case BINARY_NODE:
            
            secondPass(root->node.binaryNode.expr1,symbolTable,funcName);
            secondPass(root->node.binaryNode.expr2,symbolTable,funcName);
            break;
        default:
           break;
    }
}

#endif
