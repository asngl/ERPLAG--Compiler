#ifndef _SEMANTICANALYZERC
#define _SEMANTICANALYZERC

#include "symbolTable.h"
#include "symbolTableDef.h"
#include "typeChecker.h"
#include "ASTNodeDef.h"
#include "semanticAnalyzer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define arrayWidth 1
#define intWidth 2
#define realWidth 4
#define boolWidth 1
#define controlSize 20


StringList *undeclaredVariables;

void resetStringList()
{
	StringList *list,*ptr;
	list=undeclaredVariables;
	while(list!=NULL)
	{
		ptr=list->next;
		free(list);
		list=ptr;
	}
	undeclaredVariables=NULL;
}

int checkStringList(char *name)// returns 1 if name is present in string list
{
	StringList *list;
	list=undeclaredVariables;
	while(list!=NULL)
	{
		if(strcmp(list->varName,name)==0)return 1;
		list=list->next;
	}
	StringList *newList;
	newList=(StringList *)malloc(sizeof(StringList));
	strcpy(newList->varName,name);
	newList->next=undeclaredVariables;
	undeclaredVariables=newList;
	return 0;
}

LocalTable *newLocalTable(){
	LocalTable *node;
	node=(LocalTable *)malloc(sizeof(LocalTable));
	node->size=0;
	node->parent=NULL;
	node->leftChild=NULL;
	node->rightSibling=NULL;
	node->lastChild=NULL;
	for (int i = 0; i < MOD; ++i)
	{
		node->variableTable[i]=NULL;
	}
	return node;
}


int getWidthLocal(Type type){
	if(type.arrayFlag ==1){
		if(type.isStatic==0)
		return arrayWidth;
		else
		{
			switch(type.type){
				case DT_INTEGER:
					return (type.high.bound-type.low.bound+1)*intWidth+1;
				case DT_REAL:
					return (type.high.bound-type.low.bound+1)*realWidth+1;
				case DT_BOOLEAN:
					return (type.high.bound-type.low.bound+1)*boolWidth+1;
			}
		}
	}
	switch(type.type){
		case DT_INTEGER:
			return intWidth;
		case DT_REAL:
			return realWidth;
		case DT_BOOLEAN:
			return boolWidth;
	}
	return 0;
}


VariableEntry *checkDeclarationBeforeUse(Context context,LocalTable *parent, char name[], int lineNumber){
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
	if(parent->parent==NULL)
	{
		if(checkStringList(name)==0)
			printf("Line %d : Variable %s is not declared\n",lineNumber,name);
		ERROR_FLAG = 1;
		return NULL;
	}
	return checkDeclarationBeforeUse(context,parent->parent,name,lineNumber);
    
}

VariableEntry *searchLocalTable(LocalTable *localTable,char *string)
{
	int hash;
	hash = computeHashFromString(string);
	VariableEntry *ptr;
	ptr = (localTable->variableTable)[hash];
	while(ptr!=NULL)
	{
		if(strcmp(string,ptr->varName)==0)	return ptr;
		ptr=ptr->next;
	}
	return NULL;

}



int insertLocalTable(Context context,LocalTable *localTable,struct ASTNode *root,int baseOffset)	//returns offset after allocation
{
	struct ASTNode *currVar;
	VariableEntry *varptr;
	currVar = root->node.declareNode.idList;
	while(currVar!=NULL)
	{
		VariableEntry *search;
		search=searchLocalTable(localTable,currVar->node.idListNode.varName);
		if(search!=NULL)
		{
			printf("Line %d : Variable %s is already declared on line %d\n",currVar->lineNumber,currVar->node.idListNode.varName,search->lineNumber);
			ERROR_FLAG=1;
			currVar=currVar->node.idListNode.next;
			continue;
		}
		VariableEntry *initNode;
		initNode = (VariableEntry *)malloc(sizeof(VariableEntry));
		strcpy(initNode->varName,currVar->node.idListNode.varName);
		initNode->isParameter=0;
		initNode->type.type=root->node.declareNode.dataType;
		if(root->node.declareNode.Range==NULL)
		{
			initNode->type.arrayFlag=0;
		}
		else
		{
			initNode->type.arrayFlag=1;
			if(root->node.declareNode.Range->node.rangeNode.Range1->tag == ID_NODE)
			{
				initNode->type.tagLow=1;
				strcpy(initNode->type.low.lexeme,root->node.declareNode.Range->node.rangeNode.Range1->node.idNode.varName);
				varptr=checkDeclarationBeforeUse(context,localTable,initNode->type.low.lexeme,root->lineNumber);
				initNode->type.lowPtr=varptr;
				if(varptr!=NULL)
				{
					if(varptr->type.type!=DT_INTEGER)
					{
						printf("Line %d : Lower Bound Variable %s is not of Integer type\n",root->lineNumber,initNode->type.low.lexeme);
						ERROR_FLAG=1;
					}
					else if(varptr->type.arrayFlag==1)
					{
						printf("Line %d : Lower Bound Variable %s is of Array type\n",root->lineNumber,initNode->type.low.lexeme);
						ERROR_FLAG=1;
					}
					else if(varptr->initFlag==0)
					{
						printf("Line %d : Lower Bound Variable %s not initialised\n",root->lineNumber,initNode->type.low.lexeme);
						ERROR_FLAG=1;
					}

				}
				root->node.declareNode.Range->node.rangeNode.Range1->localTableEntry=varptr;
			}
			else
			{
				initNode->type.tagLow=0;
				initNode->type.low.bound=root->node.declareNode.Range->node.rangeNode.Range1->node.numNode.num;
				initNode->type.lowPtr=NULL;
			}
			
			if(root->node.declareNode.Range->node.rangeNode.Range2->tag == ID_NODE)
			{
				initNode->type.tagHigh=1;
				strcpy(initNode->type.high.lexeme,root->node.declareNode.Range->node.rangeNode.Range2->node.idNode.varName);
				varptr=checkDeclarationBeforeUse(context,localTable,initNode->type.high.lexeme,root->lineNumber);
				initNode->type.highPtr=varptr;
				if(varptr!=NULL)
				{
					if(varptr->type.type!=DT_INTEGER)
					{
						printf("Line %d : Higher Bound Variable %s is not of Integer type\n",root->lineNumber,initNode->type.high.lexeme);
						ERROR_FLAG=1;
					}
					else if(varptr->type.arrayFlag==1)
					{
						printf("Line %d : Higher Bound Variable %s is of Array type\n",root->lineNumber,initNode->type.high.lexeme);
						ERROR_FLAG=1;
					}
					else if(varptr->initFlag==0)
					{
						printf("Line %d : Higher Bound Variable %s not initialised\n",root->lineNumber,initNode->type.high.lexeme);
						ERROR_FLAG=1;
					}
				}
				root->node.declareNode.Range->node.rangeNode.Range2->localTableEntry=varptr;
			}
			else
			{
				initNode->type.tagHigh=0;
				initNode->type.high.bound=root->node.declareNode.Range->node.rangeNode.Range2->node.numNode.num;
				initNode->type.highPtr=NULL;
			}
			
			if(initNode->type.tagLow==0 && initNode->type.tagHigh==0)
			{
				initNode->type.isStatic =1;
				if(initNode->type.low.bound > initNode->type.high.bound)
				{
					printf("Line %d : Lower bound of an array cannot be greater than the upper bound\n",root->lineNumber);
					ERROR_FLAG=1;
				}
			}
			else{
				initNode->type.isStatic =0;
			}
		}

		initNode->lineNumber=currVar->lineNumber;
		initNode->offset=baseOffset;
		initNode->width=getWidthLocal(initNode->type);
		baseOffset+=initNode->width;
		int hash;
		hash = computeHashFromString(currVar->node.idListNode.varName);
		initNode->next=(localTable->variableTable)[hash];
		(localTable->variableTable)[hash] = initNode;
		currVar->localTableEntry=initNode;
		currVar=currVar->node.idListNode.next;
	}
	return baseOffset;
}



void addChild(LocalTable *parent, LocalTable *child)
{
	LocalTable *ptr;
	ptr=parent->leftChild;
	if(ptr==NULL)
	{
		parent->leftChild=child;
		parent->lastChild=child;
		child->parent=parent;
		return;
	}
	ptr=parent->lastChild;
	ptr->rightSibling=child;
	parent->lastChild=child;
	child->parent=parent;
	return;
}


LocalTable *populateConditionNodeLocalTable(struct Context context,LocalTable *parentOfparent,struct ASTNode *head,int baseOffset)
{
	LocalTable *parent=newLocalTable();
	parent->parent=parentOfparent;
	LocalTable *child;
	int newOffset;
	int defaultDone=0;
	struct ASTNode *ptr;
	struct ASTNode *cases;
	struct ASTNode *root;
	FunctionTable *funcptr;
	VariableEntry *varptr;
	Type leftType,rightType;
	cases=head->node.conditionNode.Case;
	while(1)
	{
		if(defaultDone==0)	root=cases->node.caseNode.stmt;
		while(root!=NULL)
		{
			switch(root->tag)
			{
				case INPUT_NODE:
					if(assertNotForbidden(context,root->node.inputNode.name,root->lineNumber)==1)
					{
						varptr=checkDeclarationBeforeUse(context,parent,root->node.inputNode.name,root->lineNumber);
						root->localTableEntry=varptr;
						if(varptr!=NULL)	varptr->initFlag=1;
					}
					root=root->node.inputNode.next;
					break;
					
				case OUTPUT_NODE:
				    	if(root->node.outputNode.value->tag == ID_NODE)
				    	{
						ptr=root->node.outputNode.value;
						varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idNode.varName,root->lineNumber);
						ptr->localTableEntry=varptr;
						if(varptr==NULL)
						{
							root=root->node.outputNode.next;
							break;
						}
						leftType=varptr->type;
						if(ptr->node.idNode.index!=NULL)
						{
							if(leftType.arrayFlag==0)
							{
								printf("Line %d : Cannot index a non-array variable %s\n",ptr->lineNumber,ptr->node.idNode.varName);
								ERROR_FLAG=1;
					    		}
					    		if(leftType.isStatic==1&&ptr->node.idNode.index->tag==NUM_NODE)
					    		{	
					    			if(leftType.low.bound > ptr->node.idNode.index->node.numNode.num)
					    			{
									printf("Line %d : index %d used is out of bounds ",ptr->lineNumber,ptr->node.idNode.index->node.numNode.num);
									printf("[%d,%d]\n",leftType.low.bound,leftType.high.bound);
									ERROR_FLAG=1;
					        		}
								if(leftType.high.bound < ptr->node.idNode.index->node.numNode.num)
								{
									printf("Line %d : index %d used is out of bounds ",ptr->lineNumber,ptr->node.idNode.index->node.numNode.num);
									printf("[%d,%d]\n",leftType.low.bound,leftType.high.bound);
									ERROR_FLAG=1;
								}
					        		leftType.arrayFlag=0;
					    		}
					    		if(ptr->node.idNode.index->tag==ID_NODE)
					   		{
						    		varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idNode.index->node.idNode.varName,root->lineNumber);
						    		ptr->node.idNode.index->localTableEntry=varptr;
								if(varptr!=NULL)
								{
									if(varptr->type.type!=DT_INTEGER)
									{
										printf("Line %d : Index Variable %s ",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);	
										printf("is not of Integer type\n");
										ERROR_FLAG=1;
									}
									else if(varptr->type.arrayFlag==1)
									{
										printf("Line %d : Index Variable %s ",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);	
										printf("cannot be of Array type\n");
										ERROR_FLAG=1;
									}
									else if(varptr->initFlag==0)
									{
										printf("Line %d : Index Variable %s not initialised\n",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);
										ERROR_FLAG=1;
									}
								}
					 		}	
						}
					}
					root=root->node.outputNode.next;
					break;
				
				case ASSIGN_NODE:
					rightType=validateExpression(context,parent,root->node.assignNode.expr);   
					if(assertNotForbidden(context,root->node.assignNode.LHS,root->lineNumber)==1)
					{
						varptr=checkDeclarationBeforeUse(context,parent,root->node.assignNode.LHS,root->lineNumber);
						root->localTableEntry=varptr;
						if(varptr==NULL)
						{
							root=root->node.assignNode.next;
							break;
						}
						varptr->initFlag=1;
						leftType=varptr->type;
						if(root->node.assignNode.index!=NULL)
						{
							if(leftType.arrayFlag==0)
							{
								printf("Line %d : Cannot index a non-array variable %s\n",root->lineNumber,root->node.assignNode.LHS);
								ERROR_FLAG=1;
								root=root->node.assignNode.next;
								break;
							}
							if(leftType.isStatic==1&&root->node.assignNode.index->tag==NUM_NODE)
							{
								if(leftType.low.bound > root->node.assignNode.index->node.numNode.num)
								{
									printf("Line %d : index %d used is out of bounds ",root->lineNumber,root->node.assignNode.index->node.numNode.num);
									printf("[%d,%d]\n",leftType.low.bound,leftType.high.bound);
									ERROR_FLAG=1;
									root=root->node.assignNode.next;
									break;
								}
								if(leftType.high.bound < root->node.assignNode.index->node.numNode.num)
								{
									printf("Line %d : index %d used is out of bounds ",root->lineNumber,root->node.assignNode.index->node.numNode.num);
									printf("[%d,%d]\n",leftType.low.bound,leftType.high.bound);
									ERROR_FLAG=1;
									root=root->node.assignNode.next;
									break;
								}
								leftType.arrayFlag=0;
							}
							if(root->node.assignNode.index->tag==ID_NODE)
			    				{
							    	varptr=checkDeclarationBeforeUse(context,parent,root->node.assignNode.index->node.idNode.varName,root->lineNumber);
							    	root->node.assignNode.index->localTableEntry=varptr;
								if(varptr!=NULL)
								{
									if(varptr->type.type!=DT_INTEGER)
									{
										printf("Line %d : Index Variable %s ",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
										printf("is not of Integer type\n");
										ERROR_FLAG=1;
									}
									else if(varptr->type.arrayFlag==1)
									{
										printf("Line %d : Index Variable %s ",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
										printf("cannot be of Array type\n");
										ERROR_FLAG=1;
									}
									else if(varptr->initFlag==0)
									{
										printf("Line %d : Index Variable %s ",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
										printf("not initialised\n");
										ERROR_FLAG=1;
									}
								}
				       			}
							leftType.arrayFlag=0;
			    			}
						if(assertTypeEquality(leftType,rightType,root->lineNumber)==0)
						{
							printf("Line %d : Types of lhs and rhs of assignment statement do not match (Type Mismatch Error)\n",root->lineNumber);
							ERROR_FLAG=1;
						}
					}
					root=root->node.assignNode.next;
					break;
				case MODULE_REUSE_NODE:
				//Set use flag
					if(strcmp(root->node.moduleReuseNode.id,context.funcName)==0)
					{
						printf("Line %d : Recursion attempt on Module %s\n",root->lineNumber, root->node.moduleReuseNode.id);
						ERROR_FLAG=1;
					}
					else
					{
						funcptr=searchSymbolTable(*(context.symbolTable),root->node.moduleReuseNode.id);
						if(funcptr==NULL)
						{                   
							printf("Line %d : Module %s not declared\n",root->lineNumber, root->node.moduleReuseNode.id);
							ERROR_FLAG=1;
						}
						else
							funcptr->useFlag=1;
					}
					ptr=root->node.moduleReuseNode.idList;
					while(ptr!=NULL)
					{
						ptr->localTableEntry=checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
						ptr=ptr->node.idListNode.next;
					}
					ptr=root->node.moduleReuseNode.optional;
					while(ptr!=NULL)
					{
						if(assertNotForbidden(context,ptr->node.idListNode.varName,root->lineNumber)==1)
						{
							varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
							ptr->localTableEntry=varptr;
							if(varptr!=NULL)	varptr->initFlag=1;
						}
						ptr=ptr->node.idListNode.next;
					}
					root=root->node.moduleReuseNode.next;
					break;
				
				case CONDITION_NODE:
					varptr=checkDeclarationBeforeUse(context,parent,root->node.conditionNode.id,root->lineNumber);
					root->localTableEntry=varptr;
					if(varptr->type.arrayFlag==1)
					{
						printf("Line %d : usage of %s is forbidden inside switch as it is an array\n",root->lineNumber,varptr->varName);
						ERROR_FLAG=1;
					}
					else
					{
						switch(varptr->type.type)
						{
							case DT_INTEGER:
								ptr=root->node.conditionNode.Case;
								while(ptr!=NULL)
								{
									if(ptr->node.caseNode.value->tag!=NUM_NODE)
									{
										printf("Line %d : Case value defined is not an integer\n",ptr->lineNumber);
										ERROR_FLAG=1;
									}
									ptr=ptr->node.caseNode.next;
								}
								if(root->node.conditionNode.Default==NULL)
								{
									printf("Line %d : Default not declared for integer switch \n",root->node.conditionNode.endLine);
									ERROR_FLAG=1;
								}
								break;
							
							case DT_REAL:
								printf("Line %d : value inside switch is of type real\n",root->lineNumber);
								ERROR_FLAG=1;
								break;
							
							case DT_BOOLEAN:
								ptr=root->node.conditionNode.Case;
								while(ptr!=NULL)
								{
									if(ptr->node.caseNode.value->tag!=BOOL_NODE)
									{
										printf("Line %d : Case value defined is not a boolean\n",ptr->lineNumber);
										ERROR_FLAG=1;
									}
									ptr=ptr->node.caseNode.next;
								}
								if(root->node.conditionNode.Default!=NULL)
								{
									printf("Line %d : Default declared for boolean switch \n",root->node.conditionNode.Default->lineNumber);
									ERROR_FLAG=1;
								}
								break;
						}
					}
					child=populateConditionNodeLocalTable(context,parent,root,baseOffset);
					child->scope.startLine=root->node.conditionNode.startLine;
					child->scope.endLine=root->node.conditionNode.endLine;        
					baseOffset+=child->size;
					addChild(parent,child);
					parent->size+=child->size;
					root=root->node.conditionNode.next;
					break;
		    
				case FOR_NODE:
					if(assertNotForbidden(context,root->node.forNode.id,root->lineNumber)==1)
					{
						varptr=checkDeclarationBeforeUse(context,parent,root->node.forNode.id,root->lineNumber);
						root->localTableEntry=varptr;
						if(varptr!=NULL)	varptr->initFlag=1;
					}
					varptr=(VariableEntry *)malloc(sizeof(VariableEntry));
					strcpy(varptr->varName,root->node.forNode.id);
					varptr->lineNumber=root->lineNumber;
					varptr->next=context.forbiddenVariables;
					context.forbiddenVariables=varptr;
					child=populateLocalTable(context,parent,root->node.forNode.stmt,baseOffset);
					child->scope.startLine=root->node.forNode.startLine;
					child->scope.endLine=root->node.forNode.endLine;        
					baseOffset+=child->size;
					addChild(parent,child);
					parent->size+=child->size;
					root=root->node.forNode.next;
					context.forbiddenVariables=varptr->next;
					free(varptr);
					break;
	
				case WHILE_NODE:
					setModifyFlagExpression(context,parent,root->node.whileNode.expr,0);
					rightType=validateExpression(context,parent,root->node.whileNode.expr);
					leftType.type=DT_BOOLEAN;
					leftType.arrayFlag=0;
					assertTypeEquality(leftType,rightType,root->lineNumber);
					child=populateLocalTable(context,parent,root->node.whileNode.stmt,baseOffset);
					if(setModifyFlagExpression(context,parent,root->node.whileNode.expr,-1)==0)
					{
					    printf("Line %d : no variables inside while loop are being modified\n",root->lineNumber);
					    ERROR_FLAG=1;
					}
					child->scope.startLine=root->node.whileNode.startLine;
					child->scope.endLine=root->node.whileNode.endLine;              
					baseOffset+=child->size;
					addChild(parent,child);
					parent->size+=child->size;
					root=root->node.whileNode.next;
					break;
					
				case DECLARE_NODE:
					newOffset=insertLocalTable(context,parent,root,baseOffset);
					parent->size+=newOffset-baseOffset;
					baseOffset=newOffset;
					root=root->node.declareNode.next;
					break;
			}
		}
		if(cases!=NULL)	cases=cases->node.caseNode.next;
		if(defaultDone==0&&cases==NULL)
		{
			root=head->node.conditionNode.Default;
			defaultDone=1;
		}
		else if(cases==NULL)	break;
	}
	return parent;
}


LocalTable *populateLocalTable(Context context,LocalTable *parentOfparent,struct ASTNode *root,int baseOffset)
{
	LocalTable *parent=newLocalTable();
	parent->parent=parentOfparent;
	LocalTable *child;
	int newOffset;
	struct ASTNode *ptr;
	FunctionTable *funcptr;
	VariableEntry *varptr;
	Type leftType,rightType;
	while(root!=NULL)
	{
		switch(root->tag)
		{
            		case INPUT_NODE:
                		if(assertNotForbidden(context,root->node.inputNode.name,root->lineNumber)==1)
                		{
					varptr=checkDeclarationBeforeUse(context,parent,root->node.inputNode.name,root->lineNumber);
					root->localTableEntry=varptr;
					if(varptr!=NULL)	varptr->initFlag=1;
				}
				root=root->node.inputNode.next;
				break;
        		
        		case OUTPUT_NODE:
	            		if(root->node.outputNode.value->tag == ID_NODE)
	            		{
			        	ptr=root->node.outputNode.value;
			        	varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idNode.varName,root->lineNumber);
			        	ptr->localTableEntry=varptr;
			        	if(varptr==NULL)
			        	{
			        		root=root->node.outputNode.next;
			        		break;
			        	}
					leftType=varptr->type;
					if(ptr->node.idNode.index!=NULL)
					{
						if(leftType.arrayFlag==0)
						{
							printf("Line %d : Cannot index a non-array variable %s\n",ptr->lineNumber,ptr->node.idNode.varName);
							ERROR_FLAG=1;
						}
						if(leftType.isStatic==1&&ptr->node.idNode.index->tag==NUM_NODE)
						{
							if(leftType.low.bound > ptr->node.idNode.index->node.numNode.num)
							{
								printf("Line %d : index %d used is out of bounds ",ptr->lineNumber,ptr->node.idNode.index->node.numNode.num);
								printf("[%d,%d]\n",leftType.low.bound,leftType.high.bound);
								ERROR_FLAG=1;
							}
							if(leftType.high.bound < ptr->node.idNode.index->node.numNode.num)
							{
								printf("Line %d : index %d used is out of bounds [%d,%d]\n",ptr->lineNumber,ptr->node.idNode.index->node.numNode.num);
								printf("[%d,%d]\n",leftType.low.bound,leftType.high.bound);
								ERROR_FLAG=1;
							}
							leftType.arrayFlag=0;
						}
						if(ptr->node.idNode.index->tag==ID_NODE)
						{
							varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idNode.index->node.idNode.varName,root->lineNumber);
							ptr->node.idNode.index->localTableEntry=varptr;
							if(varptr!=NULL)
							{
								if(varptr->type.type!=DT_INTEGER)
								{
									printf("Line %d : Index Variable %s is not of Integer type\n",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);
									ERROR_FLAG=1;
								}
								else if(varptr->type.arrayFlag==1)
								{
									printf("Line %d : Index Variable %s cannot be of Array type\n",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);
									ERROR_FLAG=1;
								}
								else if(varptr->initFlag==0)
								{
									printf("Line %d : Index Variable %s not initialised\n",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);
									ERROR_FLAG=1;
								}
							}
	                	    		}
	                		}
	            		}
			        root=root->node.outputNode.next;
			        break;
			case ASSIGN_NODE:
				rightType=validateExpression(context,parent,root->node.assignNode.expr);   
				if(assertNotForbidden(context,root->node.assignNode.LHS,root->lineNumber)==1)
				{
					varptr=checkDeclarationBeforeUse(context,parent,root->node.assignNode.LHS,root->lineNumber);
					root->localTableEntry=varptr;
					if(varptr==NULL)
					{
						root=root->node.assignNode.next;
						break;
					}
					varptr->initFlag=1;
					leftType=varptr->type;
					if(root->node.assignNode.index!=NULL)
					{
						if(leftType.arrayFlag==0)
						{
							printf("Line %d : Cannot index a non-array variable %s\n",root->lineNumber,root->node.assignNode.LHS);
							ERROR_FLAG=1;
							root=root->node.assignNode.next;
							break;
						}
						if(leftType.isStatic==1&&root->node.assignNode.index->tag==NUM_NODE)
						{
							if(leftType.low.bound > root->node.assignNode.index->node.numNode.num)
							{
								printf("Line %d : index %d used is out of bounds ",root->lineNumber,root->node.assignNode.index->node.numNode.num);
								printf("[%d,%d]\n",leftType.low.bound,leftType.high.bound);
								ERROR_FLAG=1;
								root=root->node.assignNode.next;
								break;
							}
							if(leftType.high.bound < root->node.assignNode.index->node.numNode.num)
							{
								printf("Line %d : index %d used is out of bounds ",root->lineNumber,root->node.assignNode.index->node.numNode.num);
								printf("[%d,%d]\n",leftType.low.bound,leftType.high.bound);
								ERROR_FLAG=1;
								root=root->node.assignNode.next;
								break;
							}
							leftType.arrayFlag=0;
						}
						if(root->node.assignNode.index->tag==ID_NODE)
						{
							varptr=checkDeclarationBeforeUse(context,parent,root->node.assignNode.index->node.idNode.varName,root->lineNumber);
							root->node.assignNode.index->localTableEntry=varptr;
							if(varptr!=NULL)
							{
								if(varptr->type.type!=DT_INTEGER)
								{
									printf("Line %d : Index Variable %s ",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
									printf("is not of Integer type\n");
									ERROR_FLAG=1;
								}
								else if(varptr->type.arrayFlag==1)
								{
									printf("Line %d : Index Variable %s ",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
									printf("cannot be of Array type\n");
									ERROR_FLAG=1;
								}
								else if(varptr->initFlag==0)
								{
									printf("Line %d : Index Variable %s not initialised\n",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
									ERROR_FLAG=1;
								}
							}
						}
						leftType.arrayFlag=0;
					}    
					if(assertTypeEquality(leftType,rightType,root->lineNumber)==0)
					{
						printf("Line %d : Types of lhs and rhs of assignment statement do not match (Type Mismatch Error)\n",root->lineNumber);
						ERROR_FLAG=1;
					}
				}
				root=root->node.assignNode.next;
				break;
				
			case MODULE_REUSE_NODE:
				//Set use flag
				if(strcmp(root->node.moduleReuseNode.id,context.funcName)==0)
				{
					printf("Line %d : Recursion attempt on Module %s\n",root->lineNumber, root->node.moduleReuseNode.id);
					ERROR_FLAG=1;
				}
				else
				{
					funcptr=searchSymbolTable(*(context.symbolTable),root->node.moduleReuseNode.id);
					if(funcptr==NULL)
					{                   
					    printf("Line %d : Module %s not declared\n",root->lineNumber, root->node.moduleReuseNode.id);
					    ERROR_FLAG=1;
					}
					else
						funcptr->useFlag=1;
				}
				ptr=root->node.moduleReuseNode.idList;
				while(ptr!=NULL)
				{
					ptr->localTableEntry=checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
					ptr=ptr->node.idListNode.next;
				}
				ptr=root->node.moduleReuseNode.optional;
				while(ptr!=NULL)
				{
					if(assertNotForbidden(context,ptr->node.idListNode.varName,root->lineNumber)==1)
					{
						varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
						ptr->localTableEntry=varptr;
						if(varptr!=NULL)	varptr->initFlag=1;
					}
					ptr=ptr->node.idListNode.next;
				}
				root=root->node.moduleReuseNode.next;
				break;
			case CONDITION_NODE:
				varptr=checkDeclarationBeforeUse(context,parent,root->node.conditionNode.id,root->lineNumber);
				root->localTableEntry=varptr;
				if(varptr->type.arrayFlag==1)
				{
					printf("Line %d : usage of %s is forbidden inside switch as it is an array\n",root->lineNumber,varptr->varName);
					ERROR_FLAG=1;
				}
				else
				{
					switch(varptr->type.type)
					{
						case DT_INTEGER:
							ptr=root->node.conditionNode.Case;
							while(ptr!=NULL)
							{
								if(ptr->node.caseNode.value->tag!=NUM_NODE)
								{
									printf("Line %d : Case value defined is not an integer\n",ptr->lineNumber);
									ERROR_FLAG=1;
								}
								ptr=ptr->node.caseNode.next;
							}
							if(root->node.conditionNode.Default==NULL)
							{
								printf("Line %d : Default not declared for integer switch \n",root->node.conditionNode.endLine);
								ERROR_FLAG=1;
							}
							break;
							
						case DT_REAL:
							printf("Line %d : value inside switch is of type real\n",root->lineNumber);
							ERROR_FLAG=1;
							break;
							
						case DT_BOOLEAN:
							ptr=root->node.conditionNode.Case;
							while(ptr!=NULL)
							{
								if(ptr->node.caseNode.value->tag!=BOOL_NODE)
								{
									printf("Line %d : Case value defined is not a boolean\n",ptr->lineNumber);
									ERROR_FLAG=1;
								}
								ptr=ptr->node.caseNode.next;
							}
							if(root->node.conditionNode.Default!=NULL)
							{
								printf("Line %d : Default declared for boolean switch \n",root->node.conditionNode.Default->lineNumber);
								ERROR_FLAG=1;
							}
							break;
					}
		                }
				child=populateConditionNodeLocalTable(context,parent,root,baseOffset);
				child->scope.startLine=root->node.conditionNode.startLine;
				child->scope.endLine=root->node.conditionNode.endLine;        
				baseOffset+=child->size;
				addChild(parent,child);
				parent->size+=child->size;
				root=root->node.conditionNode.next;
                      		break;
			case FOR_NODE:
				if(assertNotForbidden(context,root->node.forNode.id,root->lineNumber)==1)
				{
					varptr=checkDeclarationBeforeUse(context,parent,root->node.forNode.id,root->lineNumber);
					root->localTableEntry=varptr;
					if(varptr!=NULL)	varptr->initFlag=1;
				}
				varptr=(VariableEntry *)malloc(sizeof(VariableEntry));
				strcpy(varptr->varName,root->node.forNode.id);
				varptr->lineNumber=root->lineNumber;
				varptr->next=context.forbiddenVariables;
				context.forbiddenVariables=varptr;
				child=populateLocalTable(context,parent,root->node.forNode.stmt,baseOffset);
				child->scope.startLine=root->node.forNode.startLine;
				child->scope.endLine=root->node.forNode.endLine;        
				baseOffset+=child->size;
				addChild(parent,child);
				parent->size+=child->size;
				root=root->node.forNode.next;
				context.forbiddenVariables=varptr->next;
				free(varptr);
				break;
			
			case WHILE_NODE:
			setModifyFlagExpression(context,parent,root->node.whileNode.expr,0);
			rightType=validateExpression(context,parent,root->node.whileNode.expr);
			leftType.type=DT_BOOLEAN;
			leftType.arrayFlag=0;
			assertTypeEquality(leftType,rightType,root->lineNumber);
			child=populateLocalTable(context,parent,root->node.whileNode.stmt,baseOffset);
			if(setModifyFlagExpression(context,parent,root->node.whileNode.expr,-1)==0)
			{
				printf("Line %d : no variables inside while loop are being modified\n",root->lineNumber);
				ERROR_FLAG=1;
			}
			child->scope.startLine=root->node.whileNode.startLine;
			child->scope.endLine=root->node.whileNode.endLine;              
			baseOffset+=child->size;
			addChild(parent,child);
			parent->size+=child->size;
			root=root->node.whileNode.next;
			break;
			
			case DECLARE_NODE:
			newOffset=insertLocalTable(context,parent,root,baseOffset);
			parent->size+=newOffset-baseOffset;
			baseOffset=newOffset;
			root=root->node.declareNode.next;
			break;
        	}
	}
	return parent;
}

void secondPass(struct ASTNode *root, SymbolTable symbolTable,char funcName[])
{

	if(root==NULL)	return;
	Context context;
	context.symbolTable = &symbolTable;
	strcpy(context.funcName,funcName);
	context.inputList=NULL;
	context.outputList=NULL;
	FunctionTable *funcptr, *currfunc;
	struct ASTNode *ptr;
	ParameterList *paraptr;
	VariableEntry *varptr,*para_var_ptr;
	switch(root->tag)
	{
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
			if(currfunc==NULL)
			{
				printf("Cannot access current function \n");
				ERROR_FLAG = 1;
				return;
			}
			if(funcptr==NULL)
			return;
			if(funcptr->defineFlag!=1)
			{
				printf("Line %d : Function : %s is declared but not defined\n",root->lineNumber,root->node.moduleReuseNode.id);
				ERROR_FLAG = 1;
				return;
			}
			ptr = root->node.moduleReuseNode.idList;
			paraptr = funcptr->inputParaList;
			while(ptr!=NULL && paraptr!=NULL)
			{
				if(ptr->localTableEntry==NULL)
				{
					ptr=ptr->node.idListNode.next;
					paraptr=paraptr->next;
					continue;
				}
				if(assertTypeEquality(paraptr->type,ptr->localTableEntry->type,ptr->lineNumber)==0)
				{
					ERROR_FLAG = 1;
					printf("Line %d : type mismatch for input variable %s and parameter %s\n",ptr->lineNumber,ptr->node.idListNode.varName,paraptr->varName);
				}
				paraptr=paraptr->next;
				ptr=ptr->node.idListNode.next;
			}
			if(ptr!=NULL)
			{
				printf("Line %d : more input variables used in the statement than actual needed\n",root->lineNumber);
				ERROR_FLAG = 1;
			} 
			if(paraptr!=NULL)
			{
				printf("Line %d : less input variables used in the statement than actual needed\n",root->lineNumber);
				ERROR_FLAG = 1;
			}      
			ptr = root->node.moduleReuseNode.optional;
			paraptr = funcptr->outputParaList;
			while(ptr!=NULL && paraptr!=NULL)
			{
				if(ptr->localTableEntry==NULL)
				{
					ptr=ptr->node.idListNode.next;
					paraptr=paraptr->next;
					continue;
				}
				if(assertTypeEquality(paraptr->type,ptr->localTableEntry->type,ptr->lineNumber)==0)
				{
					printf("Line %d : type mismatch for input variable %s and parameter %s\n",ptr->lineNumber,ptr->node.idListNode.varName,paraptr->varName);
					ERROR_FLAG = 1;
				}
				paraptr=paraptr->next;
				ptr=ptr->node.idListNode.next;
			}
			if(ptr!=NULL)
			{
				printf("Line %d : more output variables used in the statement than actual needed\n",root->lineNumber);
				ERROR_FLAG = 1;
			} 
			if(paraptr!=NULL)
			{
				printf("Line %d : less output variables used in the statement than actual needed\n",root->lineNumber);
				ERROR_FLAG = 1;            
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
