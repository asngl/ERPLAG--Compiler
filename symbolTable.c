
#ifndef _SYMBOLTABLEC
#define _SYMBOLTABLEC
#include "symbolTable.h"
#include "symbolTableDef.h"
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


LocalTable *populateConditionNodeLocalTable(struct ASTNode *head,int baseOffset)
{
    LocalTable *parent=newLocalTable();
    LocalTable *child;
    int newOffset;
    int defaultDone=0;
    struct ASTNode *ptr;
    struct ASTNode *cases;
    struct ASTNode *root;
    cases=head->node.conditionNode.Case;
    while(1)
    {
        if(defaultDone==0)
            root=cases->node.caseNode.stmt;
        while(root!=NULL)
        {
            switch(root->tag)
            {
                case INPUT_NODE:root=root->node.inputNode.next;
                    //EMPTY
                    break;
                case OUTPUT_NODE:root=root->node.outputNode.next;
                    //EMPTY
                    break;
                case ASSIGN_NODE:root=root->node.assignNode.next;
                    //EMPTY
                    break;
                case MODULE_REUSE_NODE:root=root->node.moduleReuseNode.next;
                    //EMPTY
                    break;
                case CONDITION_NODE:
                    child=populateConditionNodeLocalTable(root,baseOffset);
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
                    child=populateLocalTable(root->node.forNode.stmt,baseOffset);
                    child->scope.startLine=root->node.forNode.startLine;
                    child->scope.endLine=root->node.forNode.endLine;        
                    baseOffset+=child->size;
                    addChild(parent,child);
                    parent->size+=child->size;
                    root=root->node.forNode.next;
                    break;
                case WHILE_NODE:
                    child=populateLocalTable(root->node.whileNode.stmt,baseOffset);
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
LocalTable *populateLocalTable(struct ASTNode *root,int baseOffset)
{
    LocalTable *parent=newLocalTable();
    LocalTable *child;
    int newOffset;
    struct ASTNode *ptr;
    while(root!=NULL)
    {
        switch(root->tag)
        {
            case INPUT_NODE:root=root->node.inputNode.next;
                //EMPTY
                break;
            case OUTPUT_NODE:root=root->node.outputNode.next;
                //EMPTY
                break;
            case ASSIGN_NODE:root=root->node.assignNode.next;
                //EMPTY
                break;
            case MODULE_REUSE_NODE:root=root->node.moduleReuseNode.next;
                //EMPTY
                break;
            case CONDITION_NODE:
                child=populateConditionNodeLocalTable(root,baseOffset);
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
                child=populateLocalTable(root->node.forNode.stmt,baseOffset);
                child->scope.startLine=root->node.forNode.startLine;
                child->scope.endLine=root->node.forNode.endLine;        
                baseOffset+=child->size;
                addChild(parent,child);
                parent->size+=child->size;
                root=root->node.forNode.next;
                break;
            case WHILE_NODE:
                child=populateLocalTable(root->node.whileNode.stmt,baseOffset);
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
			ptr->defineFlag=1;
			ptr->lineNumberDef=root->lineNumber;
			int hash;
			hash=computeHashFromString(root->node.moduleNode.moduleName);
			FunctionTable *tmp;
			tmp=symbolTable[hash].pointer;
			ptr->next=tmp;
			symbolTable[hash].pointer=ptr;
			return ptr;
		}
		if(ptr->defineFlag==1){
			printf("ERROR: on line number %d, Function %s already defined on line number: %d\n",root->lineNumber, root->node.moduleNode.moduleName,ptr->lineNumberDef);
			return NULL;
		}
		if(ptr->useFlag==-1){
			printf("ERROR: on line number %d, Function %s definition and declaration are redundant\n", root->lineNumber, root->node.moduleNode.moduleName);
			return NULL;
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

		ptr->localTable = populateLocalTable(root->node.moduleNode.body,offset);
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



#endif

