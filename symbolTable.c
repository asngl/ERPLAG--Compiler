
#ifndef _SYMBOLTABLEC
#define _SYMBOLTABLEC
#include "symbolTable.h"
#include "symbolTableDef.h"
#include "typeChecker.h"
#include "ASTNodeDef.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define arrayWidth 1
#define intWidth 2
#define realWidth 4
#define boolWidth 1
#define controlSize 20

int computeHashFromString(char *str){
	int sum=0,i=0;
	while(str[i]!='\0'){
		sum+=str[i];
		i+=1;
	}
	sum=sum%MOD;
	return sum;
}

FunctionTable *searchSymbolTable(SymbolTable symbolTable,char *string){
	int hash;
	hash=computeHashFromString(string);
	FunctionTable *ptr;
	ptr=symbolTable[hash].pointer;
	while(ptr!=NULL){
		if(strcmp(string,ptr->funcName)==0){
			return ptr;
		}
		ptr=ptr->next;
	}
	return NULL;
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

FunctionTable *newFunctionNode(char *funcName){
	FunctionTable *newNode;
	newNode=(FunctionTable *)malloc(sizeof(FunctionTable));
	strcpy(newNode->funcName,funcName);
	newNode->declareFlag=-1;
	newNode->defineFlag=-1;
	newNode->useFlag=-1;
	newNode->fsize=-1;
	newNode->lineNumber=-1;
	newNode->lineNumberDef=-1;
	newNode->controlBaseOffset=-1;
	newNode->staticVariableOffset=-1;
	newNode->dynamicVariableOffset=-1;
	newNode->scope.startLine=-1;
	newNode->scope.endLine=-1;
	newNode->inputParaList=NULL;
	newNode->outputParaList=NULL;
	newNode->localTable=newLocalTable();
	newNode->next=NULL;
	return newNode;
}

int getWidth(Type type){
	if(type.arrayFlag ==1){
		return arrayWidth+2*intWidth;
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



ParameterList *populateParaList(struct ASTNode *root,int baseOffset){
	
	if (root==NULL)
	{
		return NULL;
	}
	ParameterList *initNode;
	initNode=(ParameterList *)malloc(sizeof(ParameterList));
	strcpy(initNode->varName,root->node.paraListNode.name);
	initNode->type.type=root->node.paraListNode.type;
	if(root->node.paraListNode.Range==NULL){
		initNode->type.arrayFlag=0;
	}
	else{
		initNode->type.arrayFlag=1;
		if(root->node.paraListNode.Range->node.rangeNode.Range1->tag == ID_NODE){
			initNode->type.tagLow=1;
			strcpy(initNode->type.low.lexeme,root->node.paraListNode.Range->node.rangeNode.Range1->node.idNode.varName);
		}else{
			initNode->type.tagLow=0;
			initNode->type.low.bound=root->node.paraListNode.Range->node.rangeNode.Range1->node.numNode.num;
		}
		if(root->node.paraListNode.Range->node.rangeNode.Range2->tag == ID_NODE){
			initNode->type.tagHigh=1;
			strcpy(initNode->type.high.lexeme,root->node.paraListNode.Range->node.rangeNode.Range2->node.idNode.varName);
		}else{
			initNode->type.tagHigh=0;
			initNode->type.high.bound=root->node.paraListNode.Range->node.rangeNode.Range2->node.numNode.num;
		}	
		if(initNode->type.tagLow==0 && initNode->type.tagHigh==0){
			initNode->type.isStatic =1;
		}else{
			initNode->type.isStatic =0;
		}
	}
	initNode->initFlag=0;
	initNode->lineNumber=root->lineNumber;
	initNode->offset=baseOffset;
	initNode->width = getWidth(initNode->type);
	initNode->next=populateParaList(root->node.paraListNode.next,baseOffset+initNode->width);
	return initNode;
}

VariableEntry *searchLocalTable(LocalTable *localTable,char *string){
	int hash;
	hash = computeHashFromString(string);
	VariableEntry *ptr;
	ptr = (localTable->variableTable)[hash];
	while(ptr!=NULL){
		if(strcmp(string,ptr->varName)==0){
			return ptr;
		}
		ptr=ptr->next;
	}
	return NULL;

}

int insertLocalTable(Context context,LocalTable *localTable,struct ASTNode *root,int baseOffset)	//returns offset after allocation
{
	struct ASTNode *currVar;
	VariableEntry *varptr;
	currVar = root->node.declareNode.idList;
	while(currVar!=NULL){
		VariableEntry *search;
		search=searchLocalTable(localTable,currVar->node.idListNode.varName);
		if(search!=NULL){
			printf("Error on line number: %d, %s already declared on line number: %d\n",currVar->lineNumber,currVar->node.idListNode.varName,search->lineNumber);
			currVar=currVar->node.idListNode.next;
			continue;
		}
		VariableEntry *initNode;
		initNode = (VariableEntry *)malloc(sizeof(VariableEntry));
		strcpy(initNode->varName,currVar->node.idListNode.varName);
		initNode->isParameter=0;
		initNode->type.type=root->node.declareNode.dataType;
		if(root->node.declareNode.Range==NULL){
			initNode->type.arrayFlag=0;
		}
		else{
			initNode->type.arrayFlag=1;
			if(root->node.declareNode.Range->node.rangeNode.Range1->tag == ID_NODE){
				initNode->type.tagLow=1;
				strcpy(initNode->type.low.lexeme,root->node.declareNode.Range->node.rangeNode.Range1->node.idNode.varName);
				varptr=checkDeclarationBeforeUse(context,localTable,initNode->type.low.lexeme,root->lineNumber);
				if(varptr!=NULL)
				{
					if(varptr->type.type!=DT_INTEGER)
					{
						printf("Error on line number %d: Lower Bound Variable %s is not of Integer type\n",root->lineNumber,initNode->type.low.lexeme);
					}
					else if(varptr->type.arrayFlag==1)
					{
						printf("Error on line number %d: Lower Bound Variable %s is of Array type\n",root->lineNumber,initNode->type.low.lexeme);
					}
					else if(varptr->initFlag==0)
					{
						printf("Error on line number %d: Lower Bound Variable %s not initialised\n",root->lineNumber,initNode->type.low.lexeme);
					}

				}
				root->node.declareNode.Range->node.rangeNode.Range1->localTableEntry=varptr;
			}else{
				initNode->type.tagLow=0;
				initNode->type.low.bound=root->node.declareNode.Range->node.rangeNode.Range1->node.numNode.num;
			}
			if(root->node.declareNode.Range->node.rangeNode.Range2->tag == ID_NODE){
				initNode->type.tagHigh=1;
				strcpy(initNode->type.high.lexeme,root->node.declareNode.Range->node.rangeNode.Range2->node.idNode.varName);
				varptr=checkDeclarationBeforeUse(context,localTable,initNode->type.high.lexeme,root->lineNumber);
				if(varptr!=NULL)
				{
					if(varptr->type.type!=DT_INTEGER)
					{
						printf("Error on line number %d: Higher Bound Variable %s is not of Integer type\n",root->lineNumber,initNode->type.high.lexeme);
					}
					else if(varptr->type.arrayFlag==1)
					{
						printf("Error on line number %d: Higher Bound Variable %s is of Array type\n",root->lineNumber,initNode->type.high.lexeme);
					}
					else if(varptr->initFlag==0)
					{
						printf("Error on line number %d: Higher Bound Variable %s not initialised\n",root->lineNumber,initNode->type.high.lexeme);
					}
				}
				root->node.declareNode.Range->node.rangeNode.Range2->localTableEntry=varptr;
			}else{
				initNode->type.tagHigh=0;
				initNode->type.high.bound=root->node.declareNode.Range->node.rangeNode.Range2->node.numNode.num;
			}
			if(initNode->type.tagLow==0 && initNode->type.tagHigh==0){
				initNode->type.isStatic =1;
			}else{
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



void addChild(LocalTable *parent, LocalTable *child){
    LocalTable *ptr;
    ptr=parent->leftChild;
    if(ptr==NULL){
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
        if(defaultDone==0)
            root=cases->node.caseNode.stmt;
        while(root!=NULL)
        {
            switch(root->tag)
	        {
	            case INPUT_NODE:
	                if(assertNotForbidden(context,root->node.inputNode.name,root->lineNumber)==1){
	                    varptr=checkDeclarationBeforeUse(context,parent,root->node.inputNode.name,root->lineNumber);
	                    root->localTableEntry=varptr;
	                    if(varptr!=NULL)
	                        varptr->initFlag=1;
	                }
	                root=root->node.inputNode.next;
	                break;
	            case OUTPUT_NODE:
		            	if(root->node.outputNode.value->tag == ID_NODE){
		                	ptr=root->node.outputNode.value;
		                	varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idNode.varName,root->lineNumber);
		                	ptr->localTableEntry=varptr;
		                	if(varptr==NULL){
		                		root=root->node.outputNode.next;
		                		break;
		                	}
		                	leftType=varptr->type;
		                	if(ptr->node.idNode.index!=NULL){
		                	    if(leftType.arrayFlag==0){
		                	        printf("Semantic Error on line %d: Cannot index a non-array variable %s\n",ptr->lineNumber,ptr->node.idNode.varName);
		                	    }
		                	    if(leftType.isStatic==1&&ptr->node.idNode.index->tag==NUM_NODE)
		                	    {
		                	        if(leftType.low.bound > ptr->node.idNode.index->node.numNode.num)
		                	        {
		                	              printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",ptr->lineNumber,ptr->node.idNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
		                	        }
		                	        if(leftType.high.bound < ptr->node.idNode.index->node.numNode.num)
		                	        {
		                	              printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",ptr->lineNumber,ptr->node.idNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
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
											printf("Error on line number %d: Index Variable %s is not of Integer type\n",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);
										}
										else if(varptr->type.arrayFlag==1)
										{
											printf("Error on line number %d: Index Variable %s cannot be of Array type\n",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);
										}
										else if(varptr->initFlag==0)
										{
											printf("Error on line number %d: Index Variable %s not initialised\n",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);
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
								printf("Semantic Error on line %d: Cannot index a non-array variable %s\n",root->lineNumber,root->node.assignNode.LHS);
								root=root->node.assignNode.next;
								break;
							}
							if(leftType.isStatic==1&&root->node.assignNode.index->tag==NUM_NODE)
							{
								if(leftType.low.bound > root->node.assignNode.index->node.numNode.num)
								{
								  printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.assignNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
								  root=root->node.assignNode.next;
								  break;
								}
								if(leftType.high.bound < root->node.assignNode.index->node.numNode.num)
								{
								  printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.assignNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
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
										printf("Error on line number %d: Index Variable %s is not of Integer type\n",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
									}
									else if(varptr->type.arrayFlag==1)
									{
										printf("Error on line number %d: Index Variable %s cannot be of Array type\n",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
									}
									else if(varptr->initFlag==0)
									{
										printf("Error on line number %d: Index Variable %s not initialised\n",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
									}
								}
			               	}
							leftType.arrayFlag=0;
	                    }    
	                    if(assertTypeEquality(leftType,rightType,root->lineNumber)==0)
	                    {
	                    	printf("Type Mismatch Error on line number %d: types of lhs and rhs of assignment statement do not match\n",root->lineNumber);
	                    }
	                }
	                root=root->node.assignNode.next;
	                break;
	            case MODULE_REUSE_NODE:
	                //Set use flag
	                if(strcmp(root->node.moduleReuseNode.id,context.funcName)==0)
	                {
	                    printf("Error on line number:%d , Recursion attempt on function %s\n",root->lineNumber, root->node.moduleReuseNode.id);
	                    root=root->node.moduleReuseNode.next;
	                    break;
	                }
	                funcptr=searchSymbolTable(*(context.symbolTable),root->node.moduleReuseNode.id);
	                if(funcptr==NULL){                   
	                    printf("Error on line number:%d , Function %s not declared\n",root->lineNumber, root->node.moduleReuseNode.id);
	                    root=root->node.moduleReuseNode.next;
	                    break;
	                }
	                

	                funcptr->useFlag=1;
	                ptr=root->node.moduleReuseNode.idList;
	                while(ptr!=NULL)
	                {
	                    ptr->localTableEntry=checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
	                    ptr=ptr->node.idListNode.next;
	                }
	                ptr=root->node.moduleReuseNode.optional;
	                while(ptr!=NULL)
	                {
	                    if(assertNotForbidden(context,ptr->node.idListNode.varName,root->lineNumber)==1){
	                        varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
	                        ptr->localTableEntry=varptr;
	                        if(varptr!=NULL)
	                            varptr->initFlag=1;
	                    }
	                    ptr=ptr->node.idListNode.next;
	                }
	                root=root->node.moduleReuseNode.next;
	                break;
	            case CONDITION_NODE:
	                varptr=checkDeclarationBeforeUse(context,parent,root->node.conditionNode.id,root->lineNumber);
	                root->localTableEntry=varptr;
	                if(varptr->type.arrayFlag==1){
	                    printf("Error on line number:%d, usage of %s is forbidden inside switch as it is an array\n",root->lineNumber,varptr->varName);
	                }
	                else{
	                    switch(varptr->type.type){
	                        case DT_INTEGER:
	                            ptr=root->node.conditionNode.Case;
	                            while(ptr!=NULL){
	                                if(ptr->node.caseNode.value->tag!=NUM_NODE){
	                                    printf("Error on line number:%d, Case value defined is not an integer\n",ptr->lineNumber);
	                                }
	                                ptr=ptr->node.caseNode.next;
	                            }
	                            if(root->node.conditionNode.Default==NULL)
	                                printf("Error on line number:%d, Default not declared for integer switch \n",root->lineNumber);
	                            break;
	                        case DT_REAL:
	                            printf("Error on line number:%d, value inside switch is of type real",root->lineNumber);
	                            break;
	                        case DT_BOOLEAN:
	                            ptr=root->node.conditionNode.Case;
	                            while(ptr!=NULL){
	                                if(ptr->node.caseNode.value->tag!=BOOL_NODE){
	                                    printf("Error on line number:%d, Case value defined is not a boolean\n",ptr->lineNumber);
	                                }
	                                ptr=ptr->node.caseNode.next;
	                            }
	                            if(root->node.conditionNode.Default!=NULL)
	                                printf("Error on line number:%d, Default not declared for boolean switch \n",root->lineNumber);
	                    }
	                }
	                child=populateConditionNodeLocalTable(context,parent,root,baseOffset);
	                child->scope.startLine=root->node.conditionNode.startLine;
	                child->scope.endLine=root->node.conditionNode.endLine;        
	                baseOffset+=child->size;
	                addChild(parent,child);
	                parent->size+=child->size;
	                root=root->node.conditionNode.next;
	                /*ptr = root->node.conditionNode.Case;
	                child = populateLocalTable(ptr->node.caseNode.stmt,baseOffset);
	                baseOffset+=child->size;
	                ptr=ptr->node.caseNode.next;
	                while(ptr!=NULL){
	                    newOffset=insertLocalTable(child,ptr->node.caseNode)
	                }*/
	                break;
	            case FOR_NODE:
	                if(assertNotForbidden(context,root->node.forNode.id,root->lineNumber)==1){
	                        varptr=checkDeclarationBeforeUse(context,parent,root->node.forNode.id,root->lineNumber);
	                        root->localTableEntry=varptr;
	                        if(varptr!=NULL)
	                            varptr->initFlag=1;
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
	                if(setModifyFlagExpression(context,parent,root->node.whileNode.expr,-1)==0){
	                    printf("Error at line Number:%d, no variables inside while loop are being modified\n",root->lineNumber);
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
        if(cases!=NULL)
            cases=cases->node.caseNode.next;
        if(defaultDone==0&&cases==NULL){
            root=head->node.conditionNode.Default;
            defaultDone=1;
        }
        else if(cases==NULL)break;
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
                if(assertNotForbidden(context,root->node.inputNode.name,root->lineNumber)==1){
                    varptr=checkDeclarationBeforeUse(context,parent,root->node.inputNode.name,root->lineNumber);
                    root->localTableEntry=varptr;
                    if(varptr!=NULL)
                        varptr->initFlag=1;
                }
                root=root->node.inputNode.next;
                break;
            case OUTPUT_NODE:
	            	if(root->node.outputNode.value->tag == ID_NODE){
	                	ptr=root->node.outputNode.value;
	                	varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idNode.varName,root->lineNumber);
	                	ptr->localTableEntry=varptr;
	                	if(varptr==NULL){
	                		root=root->node.outputNode.next;
	                		break;
	                	}
	                	leftType=varptr->type;
	                	if(ptr->node.idNode.index!=NULL){
	                	    if(leftType.arrayFlag==0){
	                	        printf("Semantic Error on line %d: Cannot index a non-array variable %s\n",ptr->lineNumber,ptr->node.idNode.varName);
	                	    }
	                	    if(leftType.isStatic==1&&ptr->node.idNode.index->tag==NUM_NODE)
	                	    {
	                	        if(leftType.low.bound > ptr->node.idNode.index->node.numNode.num)
	                	        {
	                	              printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",ptr->lineNumber,ptr->node.idNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
	                	        }
	                	        if(leftType.high.bound < ptr->node.idNode.index->node.numNode.num)
	                	        {
	                	              printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",ptr->lineNumber,ptr->node.idNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
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
										printf("Error on line number %d: Index Variable %s is not of Integer type\n",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);
									}
									else if(varptr->type.arrayFlag==1)
									{
										printf("Error on line number %d: Index Variable %s cannot be of Array type\n",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);
									}
									else if(varptr->initFlag==0)
									{
										printf("Error on line number %d: Index Variable %s not initialised\n",root->lineNumber,ptr->node.idNode.index->node.idNode.varName);
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
							printf("Semantic Error on line %d: Cannot index a non-array variable %s\n",root->lineNumber,root->node.assignNode.LHS);
							root=root->node.assignNode.next;
							break;
						}
						if(leftType.isStatic==1&&root->node.assignNode.index->tag==NUM_NODE)
						{
							if(leftType.low.bound > root->node.assignNode.index->node.numNode.num)
							{
							  printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.assignNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
							  root=root->node.assignNode.next;
							  break;
							}
							if(leftType.high.bound < root->node.assignNode.index->node.numNode.num)
							{
							  printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.assignNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
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
									printf("Error on line number %d: Index Variable %s is not of Integer type\n",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
								}
								else if(varptr->type.arrayFlag==1)
								{
									printf("Error on line number %d: Index Variable %s cannot be of Array type\n",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
								}
								else if(varptr->initFlag==0)
								{
									printf("Error on line number %d: Index Variable %s not initialised\n",root->lineNumber,root->node.assignNode.index->node.idNode.varName);
								}
							}
		               	}
						leftType.arrayFlag=0;
                    }    
                    if(assertTypeEquality(leftType,rightType,root->lineNumber)==0)
                    {
                    	printf("Type Mismatch Error on line number %d: types of lhs and rhs of assignment statement do not match\n",root->lineNumber);
                    }
                }
                root=root->node.assignNode.next;
                break;
            case MODULE_REUSE_NODE:
                //Set use flag
                if(strcmp(root->node.moduleReuseNode.id,context.funcName)==0)
                {
                    printf("Error on line number:%d , Recursion attempt on function %s\n",root->lineNumber, root->node.moduleReuseNode.id);
                    root=root->node.moduleReuseNode.next;
                    break;
                }
                funcptr=searchSymbolTable(*(context.symbolTable),root->node.moduleReuseNode.id);
                if(funcptr==NULL){                   
                    printf("Error on line number:%d , Function %s not declared\n",root->lineNumber, root->node.moduleReuseNode.id);
                    root=root->node.moduleReuseNode.next;
                    break;
                }
                

                funcptr->useFlag=1;
                ptr=root->node.moduleReuseNode.idList;
                while(ptr!=NULL)
                {
                    ptr->localTableEntry=checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
                    ptr=ptr->node.idListNode.next;
                }
                ptr=root->node.moduleReuseNode.optional;
                while(ptr!=NULL)
                {
                    if(assertNotForbidden(context,ptr->node.idListNode.varName,root->lineNumber)==1){
                        varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
                        ptr->localTableEntry=varptr;
                        if(varptr!=NULL)
                            varptr->initFlag=1;
                    }
                    ptr=ptr->node.idListNode.next;
                }
                root=root->node.moduleReuseNode.next;
                break;
            case CONDITION_NODE:
                varptr=checkDeclarationBeforeUse(context,parent,root->node.conditionNode.id,root->lineNumber);
                root->localTableEntry=varptr;
                if(varptr->type.arrayFlag==1){
                    printf("Error on line number:%d, usage of %s is forbidden inside switch as it is an array\n",root->lineNumber,varptr->varName);
                }
                else{
                    switch(varptr->type.type){
                        case DT_INTEGER:
                            ptr=root->node.conditionNode.Case;
                            while(ptr!=NULL){
                                if(ptr->node.caseNode.value->tag!=NUM_NODE){
                                    printf("Error on line number:%d, Case value defined is not an integer\n",ptr->lineNumber);
                                }
                                ptr=ptr->node.caseNode.next;
                            }
                            if(root->node.conditionNode.Default==NULL)
                                printf("Error on line number:%d, Default not declared for integer switch \n",root->lineNumber);
                            break;
                        case DT_REAL:
                            printf("Error on line number:%d, value inside switch is of type real",root->lineNumber);
                            break;
                        case DT_BOOLEAN:
                            ptr=root->node.conditionNode.Case;
                            while(ptr!=NULL){
                                if(ptr->node.caseNode.value->tag!=BOOL_NODE){
                                    printf("Error on line number:%d, Case value defined is not a boolean\n",ptr->lineNumber);
                                }
                                ptr=ptr->node.caseNode.next;
                            }
                            if(root->node.conditionNode.Default!=NULL)
                                printf("Error on line number:%d, Default not declared for boolean switch \n",root->lineNumber);
                    }
                }
                child=populateConditionNodeLocalTable(context,parent,root,baseOffset);
                child->scope.startLine=root->node.conditionNode.startLine;
                child->scope.endLine=root->node.conditionNode.endLine;        
                baseOffset+=child->size;
                addChild(parent,child);
                parent->size+=child->size;
                root=root->node.conditionNode.next;
                /*ptr = root->node.conditionNode.Case;
                child = populateLocalTable(ptr->node.caseNode.stmt,baseOffset);
                baseOffset+=child->size;
                ptr=ptr->node.caseNode.next;
                while(ptr!=NULL){
                    newOffset=insertLocalTable(child,ptr->node.caseNode)
                }*/
                break;
            case FOR_NODE:
                if(assertNotForbidden(context,root->node.forNode.id,root->lineNumber)==1){
                        varptr=checkDeclarationBeforeUse(context,parent,root->node.forNode.id,root->lineNumber);
                        root->localTableEntry=varptr;
                        if(varptr!=NULL)
                            varptr->initFlag=1;
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
                if(setModifyFlagExpression(context,parent,root->node.whileNode.expr,-1)==0){
                    printf("Error at line Number:%d, no variables inside while loop are being modified\n",root->lineNumber);
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
VariableEntry *cloneParaListAsVariables(ParameterList *list,int bit)
{
	if(list==NULL)
		return NULL;
	VariableEntry *varptr=(VariableEntry *)malloc(sizeof(VariableEntry));
	strcpy(varptr->varName,list->varName);
	varptr->type=list->type;
	varptr->offset=list->offset;
	varptr->width=list->width;
	varptr->initFlag=bit;
	varptr->isParameter=1;
	varptr->lineNumber=list->lineNumber;
	varptr->next=cloneParaListAsVariables(list->next,bit);
	return varptr;
}

FunctionTable *insertSymbolTable(SymbolTable symbolTable,struct ASTNode *root){
	if(root->tag==MODULE_DECLARE_NODE){
		FunctionTable *ptr;
		ptr=searchSymbolTable(symbolTable,root->node.moduleDeclareNode.moduleName);
		if(ptr!=NULL){
			printf("ERROR:on Line number:%d, %s already declared on %d",root->lineNumber,root->node.moduleDeclareNode.moduleName,ptr->lineNumber);
			return NULL;
		}
		ptr = newFunctionNode(root->node.moduleDeclareNode.moduleName);
		ptr->declareFlag=1;
		ptr->lineNumber=root->lineNumber;
		int hash;
		hash=computeHashFromString(root->node.moduleDeclareNode.moduleName);
		FunctionTable *tmp;
		tmp=symbolTable[hash].pointer;
		ptr->next=tmp;
		symbolTable[hash].pointer=ptr;
		return ptr;

	}else
	if (root->tag==MODULE_NODE)
	{
		FunctionTable *ptr;
		ptr=searchSymbolTable(symbolTable,root->node.moduleNode.moduleName);
		if(ptr==NULL){
			ptr = newFunctionNode(root->node.moduleNode.moduleName);
			ptr->lineNumberDef=root->lineNumber;
			int hash;
			hash=computeHashFromString(root->node.moduleNode.moduleName);
			FunctionTable *tmp;
			tmp=symbolTable[hash].pointer;
			ptr->next=tmp;
			symbolTable[hash].pointer=ptr;
		}
		if(ptr->defineFlag==1){
			printf("ERROR: on line number %d, Function %s already defined on line number: %d\n",root->lineNumber, root->node.moduleNode.moduleName,ptr->lineNumberDef);
			return NULL;
		}
		if(ptr->declareFlag==1 && ptr->useFlag==-1){
			printf("ERROR: on line number %d, Function %s definition and declaration are redundant\n", root->lineNumber, root->node.moduleNode.moduleName);
		}
		ptr->defineFlag=1;
		ptr->lineNumberDef=root->lineNumber;
		ptr->scope.startLine=root->node.moduleNode.startLine;
		ptr->scope.endLine=root->node.moduleNode.endLine;
		ptr->inputParaList = populateParaList(root->node.moduleNode.inputList,0);
		int offset=0;
		if(ptr->inputParaList==NULL){
			offset=0;
		}
		else{
			ParameterList *tmp;
			tmp=ptr->inputParaList;
			while(tmp->next!=NULL)
				tmp=tmp->next;
			offset=tmp->offset+tmp->width;
		}
		ptr->outputParaList = populateParaList(root->node.moduleNode.ret,offset);//Offset change
		if(ptr->outputParaList==NULL){}
		else{
			ParameterList *tmp;
			tmp=ptr->outputParaList;
			while(tmp->next!=NULL)
				tmp=tmp->next;
			offset=tmp->offset+tmp->width;
		}
		ptr->controlBaseOffset=offset;
		offset+=controlSize;
		ptr->staticVariableOffset=offset;

		struct Context context;
		context.symbolTable=&symbolTable;
		strcpy(context.funcName, ptr->funcName);
		context.inputList=cloneParaListAsVariables(ptr->inputParaList,1);
		context.outputList=cloneParaListAsVariables(ptr->outputParaList,0);
		context.forbiddenVariables=NULL;
		ptr->localTable = populateLocalTable(context,NULL,root->node.moduleNode.body,0);
		
		VariableEntry *varptr=context.outputList;
		while(varptr!=NULL)
		{
			if(varptr->initFlag==0)
			{
				printf("Output paramter %s of function %s at line %d is not assigned any value inside function definition.\n",varptr->varName,context.funcName,varptr->lineNumber);
			}
			varptr=varptr->next;
		}

		ptr->localTable->scope.startLine=root->node.moduleNode.startLine;
		ptr->localTable->scope.endLine=root->node.moduleNode.endLine;
		ptr->dynamicVariableOffset = offset + ptr->localTable->size;
		return ptr;
	}
	else{
		printf("Error in insertSymbolTable\n");
		return NULL;
	}
}

SymbolTable *populateSymbolTable(struct ASTNode *root){
    SymbolTable *mainTable;
    mainTable=(SymbolTable *)malloc(sizeof(SymbolTable));
    for (int i = 0; i < MOD; ++i)
    {
    	(*mainTable)[i].pointer =NULL;
    }
    struct ASTNode *currNode;
    currNode=root->node.programNode.moduleDeclarations;
    while(currNode!=NULL){
        insertSymbolTable(*mainTable,currNode);
        currNode=currNode->node.moduleDeclareNode.next;
    }
    currNode=root->node.programNode.otherModules1;
    while(currNode!=NULL){        
        insertSymbolTable(*mainTable,currNode);
        currNode=currNode->node.moduleNode.next;
    }
    currNode=root->node.programNode.driverModule;
    insertSymbolTable(*mainTable,currNode);
    currNode=root->node.programNode.otherModules2;
    while(currNode!=NULL){        
        insertSymbolTable(*mainTable,currNode);
        currNode=currNode->node.moduleNode.next;
    }
    return mainTable;
}


char *to_string(int n,char *s)
{
	int i=0;
	if(n==0)
	{
		s[0]='0';
		s[1]='\0';
		return s;
	}	
	while(n!=0)
	{
		s[i++]=(char)('0'+n%10);
		n=n/10;
	}
	s[i]='\0';
	int len=i-1;
	i=0;
	while(i<=len/2)
	{
		int temp=s[i];
		s[i]=s[len-i];
		s[len-i]=temp;
		i++;
	}
	return s;
}

char *scopeToString(char *s,int start,int end)
{
	char int1[10],int2[10];
	s=to_string(start,int1);
	strncat(s,"-",25);
	strncat(s,to_string(end,int2),25);
	return s;
}

void printArrayVar(VariableEntryTable varTable, char *funcName,Scope scope)
{
	VariableEntry *ptr;
	for(int i=0;i<MOD;i++)
	{
		ptr=varTable[i];
		while(ptr!=NULL)
		{
			if(ptr->type.arrayFlag)
			{
				printf("\t%-21s%d-%d\t%-21s%-20s\t",funcName,scope.startLine,scope.endLine,ptr->varName,ptr->type.isStatic?"static array":"dynamic array");
				char s[25];
				s[0]='[';
				s[1]='\0';
				if(ptr->type.tagLow==0)
				{
					char int1[10];
					strncat(s,to_string(ptr->type.low.bound,int1),25);
				}
				else
					strncat(s,ptr->type.low.lexeme,25);
				strncat(s,",",25);
				if(ptr->type.tagHigh==0){
					char int2[10];
					strncat(s,to_string(ptr->type.low.bound,int2),25);
				}
				else
					strncat(s,ptr->type.high.lexeme,25);
				strncat(s,"]",25);
				printf("%-30s",s);
				switch(ptr->type.type)
				{
					case DT_INTEGER:
						printf("%-20s\n","integer");
						break;
					case DT_REAL:
						printf("%-20s\n","real");
						break;
					case DT_BOOLEAN:
						printf("%-20s\n","boolean");
						break;
					default:
						break;
				}
			}
			ptr=ptr->next;
		}
	}
}




void printArrayLT(LocalTable *node, char *funcName)
{
	if(node==NULL)	return;
	printArrayVar(node->variableTable,funcName, node->scope);
	node=node->leftChild;
	while(node!=NULL)
	{
		printArrayLT(node, funcName);
		node=node->rightSibling;
	}
}
void printArrayPL(ParameterList *list,char *funcName,Scope scope)
{
	while(list!=NULL)
	{
		if(list->type.arrayFlag)
		{
			printf("\t%-21s%d-%d\t%-21s%-20s\t",funcName,scope.startLine,scope.endLine,list->varName,"static array");
			char s[25];
			char int1[10],int2[10];
			s[0]='[';
			s[1]='\0';
			strncat(s,to_string(list->type.low.bound,int1),25);
			strncat(s,",",25);
			strncat(s,to_string(list->type.high.bound,int2),25);
			strncat(s,"]",25);
			printf("%-30s",s);
			switch(list->type.type)
			{
				case DT_INTEGER:
					printf("%-20s\n","integer");
					break;
				case DT_REAL:
					printf("%-20s\n","real");
					break;
				case DT_BOOLEAN:
					printf("%-20s\n","boolean");
					break;
				default:
					break;
			}
		}
		list=list->next;
		
	}
}
	


void printArrayFT(FunctionTable *table)
{
	if(table==NULL)	return;
	printArrayPL(table->inputParaList, table->funcName, table->scope);
	printArrayPL(table->outputParaList, table->funcName, table->scope);
	printArrayLT(table->localTable,table->funcName);
}

void printArrayVariables(SymbolTable *symbolTable)
{
	for(int i=0;i<MOD;i++)
	{
		FunctionTable* funTable=(*symbolTable)[i].pointer;
		while(funTable!=NULL)
		{
		    printArrayFT(funTable);
		    funTable=funTable->next;
		}
	}
}

int widthVariableEntryTable(VariableEntryTable table)
{
	int w=0;
	VariableEntry *ptr;
	for(int i=0;i<MOD;i++)
	{
		ptr=table[i];
		while(ptr!=NULL)
		{
			w+=ptr->width;
			ptr=ptr->next;
		}
	}
	return w;
}

int widthLocalTable(LocalTable *table)
{
	int w=widthVariableEntryTable(table->variableTable);
	table=table->leftChild;
	while(table!=NULL)
	{
		w+=widthLocalTable(table);
		table=table->rightSibling;
	}
	return w;
}

int widthParameterList(ParameterList *list)
{
	int w=0;
	while(list!=NULL)
	{
		w+=list->width;
		list=list->next;
	}
	return w;	
}

int calculateWidth(FunctionTable *fun)
{
	int w=widthParameterList(fun->inputParaList);
	w+=widthParameterList(fun->outputParaList);
	w+=widthLocalTable(fun->localTable);
	return w;
}

void printRecordWidth(SymbolTable *symbolTable)
{
	for(int i=0;i<MOD;i++)
	{
		FunctionTable *funTable=(*symbolTable)[i].pointer;
		while(funTable!=NULL)
		{
			int w=calculateWidth(funTable);
			printf("\t%-25s%d\n",funTable->funcName,w);
			funTable=funTable->next;
		}
	}	
} 

void printType(Type type)
{
    printf("%-10s",type.arrayFlag?"yes":"no");
    if(type.arrayFlag==1)
    {
    	char s[25];
	char int1[10],int2[10];
	s[0]='[';
	s[1]='\0';
    	if(type.isStatic==1)
    	{
		printf("%-15s","static array");
		strncat(s,to_string(type.low.bound,int1),25);
		strncat(s,",",25);
		strncat(s,to_string(type.high.bound,int2),25);
		strncat(s,"]",25);
		printf("%-30s",s);;
        }
        else
        {
		printf("%-15s","dynamic array");
		if(type.tagLow==0)
		{
			char int1[10];
			strncat(s,to_string(type.low.bound,int1),25);
		}
		else
			strncat(s,type.low.lexeme,25);
		strncat(s,",",25);
		if(type.tagHigh==0)
		{
			char int2[10];
			strncat(s,to_string(type.low.bound,int2),25);
		}
		else
			strncat(s,type.high.lexeme,25);
		strncat(s,"]",25);
		printf("%-30s",s);
        }        
    }
    else{
    	printf("%-15s%-30s","---","---");
    }
    switch(type.type)
    {
    	case 0:
    		printf("%-20s","integer");
    		break;
    	case 1:
    		printf("%-20s","real");
    		break;
    	case 2:
    		printf("%-20s","boolean");
    		break;
    	default:
    		break;
    }
}

void printVariableTable(VariableEntryTable variableEntryTable,char *funcName, Scope scope,int depth)
{
    VariableEntry *entry;
    for(int i=0;i<MOD;i++)
    {
        entry=variableEntryTable[i];
        while(entry!=NULL)
        {
        	char s[25];
        	char width[10];
        	printf("\t%-21s%-21s%-25s%-10s",entry->varName,funcName,scopeToString(s,scope.startLine,scope.endLine),to_string(entry->width,width));
        	printType(entry->type);
        	char depth_string[10],offset[10];
        	printf("%-11s%-11s",to_string(entry->offset,offset),to_string(depth,depth_string));        
        	printf("\n");
        	entry=entry->next;
        }
    }
}


void printLocalTable(LocalTable *localTable,char *funcName,int depth)
{
    printVariableTable(localTable->variableTable,funcName,localTable->scope,depth);
    LocalTable *child=localTable->leftChild;
    while(child!=NULL)
    {
        printLocalTable(child,funcName,depth+1);
        child=child->rightSibling;
    }
}

void printParameterList(ParameterList *list, char *funcName,Scope scope)
{
    while(list!=NULL){
    	char scope_string[25];
        char width[10];
        printf("\t%-21s%-21s%-25s%-10s",list->varName,funcName,scopeToString(scope_string,scope.startLine,scope.endLine),to_string(list->width,width));
        printType(list->type);
        char offset[10];
        printf("%-11s%-11s",to_string(list->offset,offset),"0");  
        printf("\n");
        list=list->next;
    }
}
void printFunctionTable(FunctionTable *funTable)
{
    if(funTable == NULL) return;
    printParameterList(funTable->inputParaList,funTable->funcName,funTable->scope);
    printParameterList(funTable->outputParaList,funTable->funcName,funTable->scope);
    printLocalTable(funTable->localTable,funTable->funcName,1);
    
}

void printSymbolTable(SymbolTable *symbolTable)
{
    printf("\n\nPRINTING SYMBOL TABLE:\n");
    for(int i=0;i<MOD;i++)
    {
        FunctionTable* funTable=(*symbolTable)[i].pointer;
        while(funTable!=NULL)
        {
            printFunctionTable(funTable);
            funTable=funTable->next;
        }
    }
}

#endif

