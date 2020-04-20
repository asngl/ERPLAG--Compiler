/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _SYMBOLTABLEC
#define _SYMBOLTABLEC
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

//finds the bucket in hash table corresponding to function str
//used in searchSymbolTable and insertSymbolTable
int computeHashFromString(char *str){ 
	int sum=0,i=0;
	while(str[i]!='\0'){
		sum+=str[i];
		i+=1;
	}
	sum=sum%MOD;
	return sum;
}

//search a function in symbol table
//returns FunctionTable pointer if found, else NULL
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

//Initializes a FunctionTable node
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
	newNode->activationRecordSize=-1;
	newNode->scope.startLine=-1;
	newNode->scope.endLine=-1;
	newNode->inputParaList=NULL;
	newNode->outputParaList=NULL;
	newNode->localTable=newLocalTable();
	newNode->next=NULL;
	return newNode;
}

//returns the width of datatype
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



//populates input and output parameter list and returns its pointer
ParameterList *populateParaList(struct ASTNode *root,int baseOffset){
	if (root==NULL)
	{
		return NULL;
	}
	ParameterList *initNode;
	initNode=(ParameterList *)malloc(sizeof(ParameterList));
	strcpy(initNode->varName,root->node.paraListNode.name);
	initNode->type.type=root->node.paraListNode.type;
	if(root->node.paraListNode.Range==NULL){ //non-array type
		initNode->type.arrayFlag=0;
	}
	else{
		initNode->type.arrayFlag=1;
		if(root->node.paraListNode.Range->node.rangeNode.Range1->tag == ID_NODE){
			initNode->type.tagLow=1; //lower bound is dynamic
			strcpy(initNode->type.low.lexeme,root->node.paraListNode.Range->node.rangeNode.Range1->node.idNode.varName);
		}else{
			initNode->type.tagLow=0; //lower bound is static
			initNode->type.low.bound=root->node.paraListNode.Range->node.rangeNode.Range1->node.numNode.num;
		}
		if(root->node.paraListNode.Range->node.rangeNode.Range2->tag == ID_NODE){
			initNode->type.tagHigh=1; //high bound is dynamic
			strcpy(initNode->type.high.lexeme,root->node.paraListNode.Range->node.rangeNode.Range2->node.idNode.varName);
		}else{
			initNode->type.tagHigh=0; //high bound is static
			initNode->type.high.bound=root->node.paraListNode.Range->node.rangeNode.Range2->node.numNode.num;
		}	
		if(initNode->type.tagLow==0 && initNode->type.tagHigh==0){
			initNode->type.isStatic =1; //if both low and high bound is static, isStatic equals 1
		}else{
			initNode->type.isStatic =0;
		}
	}
	initNode->initFlag=0; //used to check if all output parameters are initialized 
	initNode->lineNumber=root->lineNumber;
	initNode->offset=baseOffset;
	initNode->width = getWidth(initNode->type);
	initNode->next=populateParaList(root->node.paraListNode.next,baseOffset+initNode->width); //recusive call to populate next paraList node
	return initNode; 
}

//clones ParameterList as VariableEntry 
VariableEntry *cloneParaListAsVariables(ParameterList *list,int bit)
{
	if(list==NULL)
		return NULL;
	VariableEntry *varptr=(VariableEntry *)malloc(sizeof(VariableEntry));
	strcpy(varptr->varName,list->varName);
	varptr->type=list->type;
	varptr->offset=list->offset;
	varptr->width=list->width;
	varptr->initFlag=bit; //1 if variable is initialized
	varptr->isParameter=1; //1 for parameter
	varptr->lineNumber=list->lineNumber;
	varptr->next=cloneParaListAsVariables(list->next,bit); //recursive call to clone next paraList node to variableEntry node
	return varptr;
}

//inserts a function in symbolTable
FunctionTable *insertSymbolTable(SymbolTable symbolTable,struct ASTNode *root){
	if(root->tag==MODULE_DECLARE_NODE){ 
		FunctionTable *ptr;
		ptr=searchSymbolTable(symbolTable,root->node.moduleDeclareNode.moduleName);
		if(ptr!=NULL){ //re-declaration of a function
			printf("Line %d : Variable %s is already declared on line %d",root->lineNumber,root->node.moduleDeclareNode.moduleName,ptr->lineNumber);
			ERROR_FLAG=1;
			return NULL;
		}
		ptr = newFunctionNode(root->node.moduleDeclareNode.moduleName); //new FunctionTable node
		ptr->declareFlag=1; //function is decalared
		ptr->lineNumber=root->lineNumber;
		int hash;
		hash=computeHashFromString(root->node.moduleDeclareNode.moduleName); //compute the bucket for hashing 
		FunctionTable *tmp;
		tmp=symbolTable[hash].pointer; //retrieve the head of linked list
		ptr->next=tmp;
		symbolTable[hash].pointer=ptr; //insert at beginning of linked list
		return ptr; //returns FunctionTable pointer 

	}else
	if (root->tag==MODULE_NODE) //module definition
	{
		FunctionTable *ptr; 
		ptr=searchSymbolTable(symbolTable,root->node.moduleNode.moduleName);
		if(ptr==NULL){ //not declared or defined before
			ptr = newFunctionNode(root->node.moduleNode.moduleName);
			ptr->lineNumberDef=root->lineNumber;
			int hash;
			hash=computeHashFromString(root->node.moduleNode.moduleName); //compute the bucket for hashing 
			FunctionTable *tmp;
			tmp=symbolTable[hash].pointer;
			ptr->next=tmp;
			symbolTable[hash].pointer=ptr; //insert at beginning of linked list
		}
		if(ptr->defineFlag==1){ //already defined 
			printf("Line %d : Module %s is already defined on line %d\n",root->lineNumber, root->node.moduleNode.moduleName,ptr->lineNumberDef);
			ERROR_FLAG=1;
			return NULL;
		}
		if(ptr->declareFlag==1 && ptr->useFlag==-1){ //declared and defined but not used in between
			printf("Line %d : Module %s definition and declaration are redundant\n", root->lineNumber, root->node.moduleNode.moduleName);
			ERROR_FLAG=1;
		}
		ptr->defineFlag=1;
		ptr->lineNumberDef=root->lineNumber;
		ptr->scope.startLine=root->node.moduleNode.startLine;
		ptr->scope.endLine=root->node.moduleNode.endLine;
		ptr->inputParaList = populateParaList(root->node.moduleNode.inputList,0); //populate inputParaList
		int offset=0;
		if(ptr->inputParaList==NULL){
			offset=0;
		}
		else{
			ParameterList *tmp;
			tmp=ptr->inputParaList;
			while(tmp->next!=NULL)
				tmp=tmp->next;
			offset=tmp->offset+tmp->width; //Offset change
		}
		ptr->outputParaList = populateParaList(root->node.moduleNode.ret,offset); //populate outputParaList
		if(ptr->outputParaList==NULL){}
		else{
			ParameterList *tmp;
			tmp=ptr->outputParaList;
			while(tmp->next!=NULL)
				tmp=tmp->next;
			offset=tmp->offset+tmp->width;
		}
		ptr->fsize=offset;//offset stores length of fucntion parameters if needed
		struct Context context;
		context.symbolTable=&symbolTable;
		strcpy(context.funcName, ptr->funcName);
		context.inputList=cloneParaListAsVariables(ptr->inputParaList,1);
		context.outputList=cloneParaListAsVariables(ptr->outputParaList,0);
		context.forbiddenVariables=NULL;
		resetStringList();
		ptr->localTable = populateLocalTable(context,NULL,root->node.moduleNode.body,0);
		VariableEntry *varptr=context.outputList;
		while(varptr!=NULL)
		{	//check initFlag to see if output parameter is assigned a value inside function definition or not
			if(varptr->initFlag==0) 
			{
				printf("Line %d : Output paramter %s of Module %s is not assigned any value inside function definition.\n",ptr->scope.endLine,varptr->varName,context.funcName);
				ERROR_FLAG=1;
			}
			varptr=varptr->next;
		}

		ptr->localTable->scope.startLine=root->node.moduleNode.startLine;
		ptr->localTable->scope.endLine=root->node.moduleNode.endLine;
		ptr->activationRecordSize = ptr->localTable->size;
		ptr->fsize+=ptr->activationRecordSize;
		root->node.moduleNode.localVariablesSize= ptr->localTable->size;
		return ptr; //returns FunctionTable pointer
	}
	else{
		printf("Error in insertSymbolTable\n");
		ERROR_FLAG=1;
		return NULL;
	}
}

//populates symbol table and returns its pointer
SymbolTable *populateSymbolTable(struct ASTNode *root){
    SymbolTable *mainTable;
    mainTable=(SymbolTable *)malloc(sizeof(SymbolTable)); //allocate memory for symbol table
    for (int i = 0; i < MOD; ++i)
    {
    	(*mainTable)[i].pointer =NULL;
    }
    //insert FunctionTable for each function in symbol table
    struct ASTNode *currNode;
    currNode=root->node.programNode.moduleDeclarations;
    while(currNode!=NULL){
        insertSymbolTable(*mainTable,currNode); //module declarations
        currNode=currNode->node.moduleDeclareNode.next;
    }
    currNode=root->node.programNode.otherModules1;
    while(currNode!=NULL){        
        insertSymbolTable(*mainTable,currNode); //module definitions
        currNode=currNode->node.moduleNode.next;
    }
    currNode=root->node.programNode.driverModule;
    insertSymbolTable(*mainTable,currNode);  //driver module
    currNode=root->node.programNode.otherModules2;
    while(currNode!=NULL){        
        insertSymbolTable(*mainTable,currNode); //module definitions
        currNode=currNode->node.moduleNode.next;
    }
    secondPass(root,*mainTable,"");
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

//used in printArrayLT to print local array variables 
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
				char scope_string[25];
				printf("\t%-21s%-25s%-21s%-20s",funcName,scopeToString(scope_string,scope.startLine,scope.endLine),ptr->varName,ptr->type.isStatic?"static array":"dynamic array");
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
					strncat(s,to_string(ptr->type.high.bound,int2),25);
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



//print local array variables of function funcName
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

//prints array variables in parameter list for function funcName
void printArrayPL(ParameterList *list,char *funcName,Scope scope)
{
	while(list!=NULL)
	{
		if(list->type.arrayFlag) //array type variable
		{
			char scope_string[25];
			printf("\t%-21s%-25s%-21s%-20s",funcName,scopeToString(scope_string,scope.startLine,scope.endLine),list->varName,list->type.isStatic?"static array":"dynamic array");
			char s[25];
			s[0]='[';
			s[1]='\0';
			if(list->type.tagLow==0)
			{
				char int1[10];
				strncat(s,to_string(list->type.low.bound,int1),25);
			}
			else
				strncat(s,list->type.low.lexeme,25);
			strncat(s,",",25);
			if(list->type.tagHigh==0){
				char int2[10];
				strncat(s,to_string(list->type.high.bound,int2),25);
			}
			else
				strncat(s,list->type.high.lexeme,25);
			strncat(s,"]",25);
			printf("%-30s",s);
			switch(list->type.type) //type of array variable
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

//prints the array variables
void printArrayVariables(SymbolTable *symbolTable)
{
	printf("\nPRINTING ARRAY VARIABLES:\n");
	printf("\t%-21s%-25s%-21s%-20s%-30s%-20s\n","Scope(module_name)","Scope(line_numbers)","Array name","Static/Dynamic","Range","Type");
	for(int i=0;i<MOD;i++) //traverse complete symbolTable
	{
		FunctionTable* funTable=(*symbolTable)[i].pointer;
		while(funTable!=NULL) //traverse the linked list for given bucket
		{
		    printArrayFT(funTable);
		    funTable=funTable->next;
		}
	}
}

//prints activation record width for all functions 
void printRecordWidth(SymbolTable *symbolTable)
{
	printf("\nPRINTING ACTIVATION RECORD WIDTHS:\n");
	printf("\t%-25s%s\n","Activation record","Width");
	for(int i=0;i<MOD;i++)
	{
		FunctionTable *funTable=(*symbolTable)[i].pointer;
		while(funTable!=NULL)
		{
			printf("\t%-25s%d\n",funTable->funcName,funTable->activationRecordSize);
			funTable=funTable->next;
		}
	}	
} 

//prints Type
void printType(Type type)
{
    printf("%-10s",type.arrayFlag?"yes":"no");
    if(type.arrayFlag==1) //array datatype
    {
    	char s[25];
	char int1[10],int2[10];
	s[0]='[';
	s[1]='\0';
    	if(type.isStatic==1) //static array
    	{
		printf("%-15s","static array");
		strncat(s,to_string(type.low.bound,int1),25);
		strncat(s,",",25);
		strncat(s,to_string(type.high.bound,int2),25);
		strncat(s,"]",25);
		printf("%-30s",s);;
        }
        else //dynamic array
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
			strncat(s,to_string(type.high.bound,int2),25);
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

//prints VariableTable for function funcName in a given scope
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

//prints LocalTable for function funcName
//called in printFunctionTable
void printLocalTable(LocalTable *localTable,char *funcName,int depth)
{
    printVariableTable(localTable->variableTable,funcName,localTable->scope,depth);
    LocalTable *child=localTable->leftChild;
    while(child!=NULL)
    {
        printLocalTable(child,funcName,depth+1); //recursive call to print localTable with depth+1
        child=child->rightSibling;
    }
}

//prints ParameterList, called in printFunctionTable
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

//prints FunctionTable, called in printSymbolTable
void printFunctionTable(FunctionTable *funTable)
{
    if(funTable == NULL) return;
    printParameterList(funTable->inputParaList,funTable->funcName,funTable->scope);
    printParameterList(funTable->outputParaList,funTable->funcName,funTable->scope);
    printLocalTable(funTable->localTable,funTable->funcName,1);
    
}

//prints symbol table
void printSymbolTable(SymbolTable *symbolTable)
{
    printf("\nPRINTING SYMBOL TABLE:\n");
    printf("\t%-21s%-21s%-25s%-10s%-10s%-15s%-30s%-20s%-11s%-11s\n", "variable_name","scope(module_name)","scope(line_numbers)","width","isArray", "static/dynamic", "range_lexemes","type_of_element", "offset","nesting_level");
    for(int i=0;i<MOD;i++)
    {
        FunctionTable* funTable=(*symbolTable)[i].pointer;
        while(funTable!=NULL) //traverse the complete linked list
        {
            printFunctionTable(funTable); //prints FunctionTable
            funTable=funTable->next;
        }
    }
}

#endif

