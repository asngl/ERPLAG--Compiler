
#ifndef _SYMBOLTABLEC
#define _SYMBOLTABLEC
#include "symbolTable.h"
#include "symbolTableDef.h"
#include "typeChecker.h"
#include "ASTNodeDef.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define arrayWidth 10
#define intWidth 4
#define realWidth 8
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
	newNode->inputParaList=NULL;
	newNode->outputParaList=NULL;
	newNode->localTable=newLocalTable();
	newNode->next=NULL;
	return newNode;
}

int getWidth(Type type){
	if(type.arrayFlag ==1){
		return arrayWidth;
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
					return (type.high.bound-type.low.bound+1)*intWidth;
				case DT_REAL:
					return (type.high.bound-type.low.bound+1)*realWidth;
				case DT_BOOLEAN:
					return (type.high.bound-type.low.bound+1)*boolWidth;
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

int insertLocalTable(LocalTable *localTable,struct ASTNode *root,int baseOffset)	//returns offset after allocation
{
	struct ASTNode *currVar;
	currVar = root->node.declareNode.idList;
	while(currVar!=NULL){
		VariableEntry *search;
		search=searchLocalTable(localTable,currVar->node.idListNode.varName);
		if(search!=NULL){
			printf("Error on line number: %d, %s already declared on line number: %d",currVar->lineNumber,currVar->node.idListNode.varName,search->lineNumber);
			continue;
		}
		VariableEntry *initNode;
		initNode = (VariableEntry *)malloc(sizeof(VariableEntry));
		strcpy(initNode->varName,currVar->node.idListNode.varName);
		initNode->type.type=root->node.declareNode.dataType;
		if(root->node.declareNode.Range==NULL){
			initNode->type.arrayFlag=0;
		}
		else{
			initNode->type.arrayFlag=1;
			if(root->node.declareNode.Range->node.rangeNode.Range1->tag == ID_NODE){
				initNode->type.tagLow=1;
				strcpy(initNode->type.low.lexeme,root->node.declareNode.Range->node.rangeNode.Range1->node.idNode.varName);
			}else{
				initNode->type.tagLow=0;
				initNode->type.low.bound=root->node.declareNode.Range->node.rangeNode.Range1->node.numNode.num;
			}
			if(root->node.declareNode.Range->node.rangeNode.Range2->tag == ID_NODE){
				initNode->type.tagHigh=1;
				strcpy(initNode->type.high.lexeme,root->node.declareNode.Range->node.rangeNode.Range2->node.idNode.varName);
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
	                    if(varptr!=NULL)
	                        varptr->initFlag=1;
	                }
	                root=root->node.inputNode.next;
	                break;
	            case OUTPUT_NODE:
	            	if(root->node.outputNode.value->tag == ID_NODE){
	                	ptr=root->node.outputNode.value;
	                	varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idNode.varName,root->lineNumber);
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
	                	}

	            	}
	                root=root->node.outputNode.next;
	                break;
	            case ASSIGN_NODE:
					rightType=validateExpression(context,parent,root->node.assignNode.expr);   
	                if(assertNotForbidden(context,root->node.assignNode.LHS,root->lineNumber)==1)
	                {
	                    varptr=checkDeclarationBeforeUse(context,parent,root->node.assignNode.LHS,root->lineNumber);
	                    if(varptr==NULL)break;
	                    varptr->initFlag=1;
	                    leftType=varptr->type;
	                    if(root->node.assignNode.index!=NULL)
	                    {
							if(leftType.arrayFlag==0)
							{
								printf("Semantic Error on line %d: Cannot index a non-array variable %s\n",root->lineNumber,root->node.assignNode.LHS);
								break;
							}
							if(leftType.isStatic==1&&root->node.assignNode.index->tag==NUM_NODE)
							{
								if(leftType.low.bound > root->node.assignNode.index->node.numNode.num)
								{
								  printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.assignNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
								  break;
								}
								if(leftType.high.bound < root->node.assignNode.index->node.numNode.num)
								{
								  printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.assignNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
								  break;
								}
								leftType.arrayFlag=0;
							}
                        }
                        assertTypeEquality(leftType,rightType,root->lineNumber);
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
	                    checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
	                    ptr=ptr->node.idListNode.next;
	                }
	                ptr=root->node.moduleReuseNode.optional;
	                while(ptr!=NULL)
	                {
	                    if(assertNotForbidden(context,ptr->node.idListNode.varName,root->lineNumber)==1){
	                        varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
	                        if(varptr!=NULL)
	                            varptr->initFlag=1;
	                    }
	                    ptr=ptr->node.idListNode.next;
	                }
	                root=root->node.moduleReuseNode.next;
	                break;
	            case CONDITION_NODE:
	                varptr=checkDeclarationBeforeUse(context,parent,root->node.conditionNode.id,root->lineNumber);
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
	                        case DT_REAL:
	                            printf("Error on line number:%d, value inside switch is of type real",root->lineNumber);
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
	                setModifyFlagExpression(context,parent,root,0);
	                rightType=validateExpression(context,parent,root->node.whileNode.expr);
					leftType.type=DT_BOOLEAN;
					leftType.arrayFlag=0;
					assertTypeEquality(leftType,rightType,root->lineNumber);
	                child=populateLocalTable(context,parent,root->node.whileNode.stmt,baseOffset);
	                if(setModifyFlagExpression(context,parent,root,-1)==0){
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
	                newOffset=insertLocalTable(parent,root,baseOffset);
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
                    if(varptr!=NULL)
                        varptr->initFlag=1;
                }
                root=root->node.inputNode.next;
                break;
            case OUTPUT_NODE:
	            	if(root->node.outputNode.value->tag == ID_NODE){
	                	ptr=root->node.outputNode.value;
	                	varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idNode.varName,root->lineNumber);
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
	                	}

	            	}
	                root=root->node.outputNode.next;
	                break;
            case ASSIGN_NODE:
				rightType=validateExpression(context,parent,root->node.assignNode.expr);   
                if(assertNotForbidden(context,root->node.assignNode.LHS,root->lineNumber)==1)
                {
                    varptr=checkDeclarationBeforeUse(context,parent,root->node.assignNode.LHS,root->lineNumber);
                    if(varptr==NULL)break;
                    varptr->initFlag=1;
                    leftType=varptr->type;
                    if(root->node.assignNode.index!=NULL)
                    {
						if(leftType.arrayFlag==0)
						{
							printf("Semantic Error on line %d: Cannot index a non-array variable %s\n",root->lineNumber,root->node.assignNode.LHS);
							break;
						}
						if(leftType.isStatic==1&&root->node.assignNode.index->tag==NUM_NODE)
						{
							if(leftType.low.bound > root->node.assignNode.index->node.numNode.num)
							{
							  printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.assignNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
							  break;
							}
							if(leftType.high.bound < root->node.assignNode.index->node.numNode.num)
							{
							  printf("Error on line number:%d, index %d used is out of bounds [%d,%d]\n",root->lineNumber,root->node.assignNode.index->node.numNode.num,leftType.low.bound,leftType.high.bound);
							  break;
							}
							leftType.arrayFlag=0;
						}
                    }    
                    assertTypeEquality(leftType,rightType,root->lineNumber);
                    
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
                    checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
                    ptr=ptr->node.idListNode.next;
                }
                ptr=root->node.moduleReuseNode.optional;
                while(ptr!=NULL)
                {
                    if(assertNotForbidden(context,ptr->node.idListNode.varName,root->lineNumber)==1){
                        varptr=checkDeclarationBeforeUse(context,parent,ptr->node.idListNode.varName,root->lineNumber);
                        if(varptr!=NULL)
                            varptr->initFlag=1;
                    }
                    ptr=ptr->node.idListNode.next;
                }
                root=root->node.moduleReuseNode.next;
                break;
            case CONDITION_NODE:
                varptr=checkDeclarationBeforeUse(context,parent,root->node.conditionNode.id,root->lineNumber);
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
                        case DT_REAL:
                            printf("Error on line number:%d, value inside switch is of type real",root->lineNumber);
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
                setModifyFlagExpression(context,parent,root,0);
                rightType=validateExpression(context,parent,root->node.whileNode.expr);
				leftType.type=DT_BOOLEAN;
				leftType.arrayFlag=0;
				assertTypeEquality(leftType,rightType,root->lineNumber);
                child=populateLocalTable(context,parent,root->node.whileNode.stmt,baseOffset);
                if(setModifyFlagExpression(context,parent,root,-1)==0){
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
                newOffset=insertLocalTable(parent,root,baseOffset);
                parent->size+=newOffset-baseOffset;
                baseOffset=newOffset;
                root=root->node.declareNode.next;
                break;
        }
    }
    return parent;
}
VariableEntry *cloneParaListAsVariables(ParameterList *list)
{
	if(list==NULL)
		return NULL;
	VariableEntry *varptr=(VariableEntry *)malloc(sizeof(VariableEntry));
	strcpy(varptr->varName,list->varName);
	varptr->type=list->type;
	varptr->offset=list->offset;
	varptr->width=list->width;
	varptr->initFlag=0;
	varptr->lineNumber=list->lineNumber;
	varptr->next=cloneParaListAsVariables(list->next);
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
		context.inputList=cloneParaListAsVariables(ptr->inputParaList);
		context.outputList=cloneParaListAsVariables(ptr->outputParaList);
		context.forbiddenVariables=NULL;
		ptr->localTable = populateLocalTable(context,NULL,root->node.moduleNode.body,offset);
		
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


void printType(Type type){
    if(type.arrayFlag==1){
        if(type.isStatic==1){
            printf("Static array of Type:%d with lower bound: %d and upper bound: %d\n ", type.type, type.low.bound, type.high.bound);
        }
        else{
            printf("Dynamic array of Type:%d", type.type);
            if(type.tagLow==0){
                printf("with lower bound: %d",type.low.bound);
            }
            else
                printf("with lower bound: %s ",type.low.lexeme);
        
            if(type.tagHigh==0){
                printf("with higher bound: %d\n",type.high.bound);
            }
            else
                printf("with higher bound: %s \n",type.high.lexeme);
        }        
    }else{
    	printf("Type is %d\n",type.type );
    }
}

void printTabs(int tabs)
{
    while(tabs--)printf("\t");
}

void printVariableTable(VariableEntryTable variableEntryTable,int tabs)
{
    VariableEntry *entry;
    for(int i=0;i<MOD;i++)
    {
        entry=variableEntryTable[i];
        while(entry!=NULL)
        {
            printTabs(tabs);
            printf("%s :: Line %d :: Offset %d :: Width %d\n",entry->varName,entry->lineNumber,entry->offset,entry->width);
            printTabs(tabs+1);
            printType(entry->type);
            entry=entry->next;
        }
    }
}


void printLocalTable(LocalTable *localTable,int tabs)
{
    printTabs(tabs);
    printf("Scope: Line %d to Line %d :: ActRecordSize %d\n",localTable->scope.startLine,localTable->scope.endLine,localTable->size);
    printTabs(tabs);
    printf("Local Variables\n");
    printVariableTable(localTable->variableTable,tabs+1);
    printTabs(tabs);
    printf("Children\n");
    LocalTable *child=localTable->leftChild;
    while(child!=NULL)
    {
        printLocalTable(child,tabs+1);
        child=child->rightSibling;
    }
}


void printParameterList(ParameterList *list)
{
    while(list!=NULL){
        printf("VarName: %s\n",list->varName);
        printType(list->type);
        printf("Line Number: %d \n", list->lineNumber);
        printf("Offset: %d\n", list->offset);
        printf("Width: %d\n",list->width);
        list=list->next;
    }
}
void printFunctionTable(FunctionTable *funTable)
{
    if(funTable == NULL) return;
    printf("\nFunction %s \n",funTable->funcName);
    printf("\tDeclaration Line number %d \n",funTable->lineNumber);
    printf("\tDefinition Line number %d \n",funTable->lineNumberDef);
    printf("\tInput parameterList \n");
    printParameterList(funTable->inputParaList);
    printf("\tOutput parameterList \n");
    printParameterList(funTable->outputParaList);
    printf("\tLocal Table structure \n");
    printLocalTable(funTable->localTable,1);
}

void printSymbolTable(SymbolTable *symbolTable)
{
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

