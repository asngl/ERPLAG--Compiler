/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/

#ifndef _TYPECHECKERC
#define _TYPECHECKERC
#include "symbolTableDef.h"
#include "ASTNodeDef.h"
#include "symbolTable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "typeChecker.h"
#include "semanticAnalyzer.h"

//Function to check if a given variable is forbidden or not, that is, it is part of for loop condition or not
int assertNotForbidden(Context context,char name[], int lineNumber)
{
	VariableEntry* ptr=context.forbiddenVariables;
	int correct=1;
	while(ptr!=NULL)
	{
		if(strcmp(ptr->varName,name)==0)
		{
			printf("Line %d : Attempt to modify variable %s used in a for loop at line %d\n",lineNumber,name,ptr->lineNumber);
			ERROR_FLAG = 1;
			correct=0;
		}
		ptr=ptr->next;
	}
	return correct;
}

//Function to check type equality for two variables or expressions
int assertTypeEquality(Type type1, Type type2, int lineNumber)
{
	if(type1.type==DT_ERROR || type2.type==DT_ERROR)
		return 1;
	if(type1.arrayFlag == 1 && type2.arrayFlag == 1)
	{
		if(type1.type == type2.type)
		{
			if(type1.isStatic == 1 && type2.isStatic == 1)
			{
				if(type1.low.bound == type2.low.bound && type1.high.bound == type2.high.bound)	return 1;
			}
			if(type1.tagLow == 0 && type2.tagLow ==0 && type1.low.bound != type2.low.bound) return 0;
			if(type1.tagHigh == 0 && type2.tagHigh ==0 && type1.high.bound != type2.high.bound) return 0;
			return 1;
		}
		else return 0;
	}
	else if(type1.arrayFlag == 0 && type2.arrayFlag == 0)
	{
		if(type1.type == DT_INTEGER && type2.type == DT_INTEGER) return 1;
		if(type1.type == DT_REAL && type2.type == DT_REAL) return 1;
		if(type1.type == DT_BOOLEAN && type2.type == DT_BOOLEAN) return 1;
	}
	return 0;
}             

//Function to check whether a given expression is valid or not checking for type and other constraints
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
			if(root->node.idNode.index!=NULL)
			{
				if(leftType.arrayFlag==0)
				{
			        	printf("Line %d : Cannot index a non-array variable %s\n",root->lineNumber,root->node.idNode.varName);
			       		ERROR_FLAG = 1;
			        	type.type=DT_ERROR;
					type.arrayFlag=0;
					return type;
				}
				if(leftType.isStatic==1&&root->node.idNode.index->tag==NUM_NODE)
			    	{	
			        	if(leftType.low.bound > root->node.idNode.index->node.numNode.num)
			        	{
					 	printf("Line %d : index %d used is out of bounds ",root->lineNumber,root->node.idNode.index->node.numNode.num);
						printf("[%d,%d]\n", leftType.low.bound,leftType.high.bound);
						ERROR_FLAG = 1;
						type.type=DT_ERROR;
						type.arrayFlag=0;
						return type;
				        }
			        	if(leftType.high.bound < root->node.idNode.index->node.numNode.num)
			        	{
				            	printf("Line %d : index %d used is out of bounds ",root->lineNumber,root->node.idNode.index->node.numNode.num);
						printf("[%d,%d]\n", leftType.low.bound,leftType.high.bound);
						ERROR_FLAG = 1;
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
							ERROR_FLAG = 1;
						}
						else if(varptr->type.arrayFlag==1)
						{
							printf("Line %d : Index Variable %s cannot be of Array type\n",root->lineNumber,root->node.idNode.index->node.idNode.varName);
							ERROR_FLAG = 1;
						}
						else if(varptr->initFlag==0)
						{
							printf("Line %d : Index Variable %s not initialised\n",root->lineNumber,root->node.idNode.index->node.idNode.varName);
							ERROR_FLAG = 1;
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
			
			if(type.type == DT_BOOLEAN)
			{
				printf("Line %d : unary operations cannot be applied on Boolean expressions\n",root->lineNumber);
				ERROR_FLAG = 1;
				type.type=DT_ERROR;
				type.arrayFlag=0;
				return type;
			}
			if(type.arrayFlag==1)
			{
				printf("Line %d : unary operations cannot be applied on an array\n",root->lineNumber);
				ERROR_FLAG = 1;
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
				ERROR_FLAG = 1;
				return type;
			}
			switch(root->node.binaryNode.op) //enum Operator{OP_PLUS,OP_MINUS,OP_MUL,OP_DIV,OP_AND,OP_OR,OP_LT, OP_LE, OP_GE, OP_GT, OP_EQ, OP_NE};
			{
				case OP_PLUS: case OP_MINUS: case OP_MUL: case OP_DIV:
			    		if(type.type==DT_BOOLEAN || type2.type==DT_BOOLEAN)
					{
					 	printf("Line %d : Cannot perform arithmetic operation on boolean type operands.\n",root->lineNumber);
						ERROR_FLAG = 1;
						type.type=DT_ERROR;
						type.arrayFlag=0;
						return type;
					}
			     		else if(type.type!=type2.type)
					{
						printf("Line %d : Cannot perform arithmetic operation with one int and one real operand.\n",root->lineNumber);
						ERROR_FLAG = 1;
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
						ERROR_FLAG = 1;
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
						ERROR_FLAG = 1;
						type.type=DT_ERROR;
						type.arrayFlag=0;
						return type;
					}
					else if(type.type!=type2.type)
					{
						printf("Line %d : Cannot perform relational operations with one int and one real operand.\n",root->lineNumber);
						ERROR_FLAG = 1;
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
						ERROR_FLAG = 1;
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

//Function to return the symbol table entry pointer for a given variable
VariableEntry *getVariableEntry(Context context,LocalTable *parent, char name[], int lineNumber){
	VariableEntry *currVar;
	currVar=context.outputList;
	while(currVar!=NULL)
	{
		if(strcmp(currVar->varName,name)==0)	return currVar;
		currVar=currVar->next;
	}
	for(int i=0;i<MOD;i++)
	{
		currVar=(parent->variableTable)[i];
		while(currVar!=NULL)
		{
			if(strcmp(currVar->varName,name)==0)	return currVar;
			currVar=currVar->next;
		}
	}
	currVar=context.inputList;
	while(currVar!=NULL)
	{
		if(strcmp(currVar->varName,name)==0)	return currVar;
		currVar=currVar->next;
	}
	if(parent->parent==NULL)	return NULL;
	return checkDeclarationBeforeUse(context,parent->parent,name,lineNumber);
    
}
    
//Function to check and set the modify flag for the given while loop variables to check the semantic that while loop variables are modified     
int setModifyFlagExpression(Context context,LocalTable *parent,struct ASTNode *root,int bit)
{
	if(root->tag==NUM_NODE || root->tag==RNUM_NODE || root->tag==BOOL_NODE)
		return 0;
	if(root->tag==ID_NODE)
	{
		VariableEntry *ptr;
		ptr=getVariableEntry(context,parent,root->node.idNode.varName,root->lineNumber);
		if(ptr==NULL)
			return 0;
		if(bit == 0)
		{
			ptr->initFlag=0;
			return 0;
		}
		else if(bit == -1)
		{
			if(ptr->initFlag==1)
			{
				ptr->initFlag=-1;
				return 1;
			}
			ptr->initFlag=-1;
			return 0;
		}
	}
	if(root->tag==UNARY_NODE)
	{
		return setModifyFlagExpression(context,parent,root->node.unaryNode.expr,bit);
	}
	if(root->tag==BINARY_NODE)
		return (setModifyFlagExpression(context,parent,root->node.binaryNode.expr1,bit) | setModifyFlagExpression(context,parent,root->node.binaryNode.expr2,bit));
	else
	{
		printf("Wrong node encountered in Expression in ast\n");
		ERROR_FLAG = 1;
		return 0;
	}
}

#endif
