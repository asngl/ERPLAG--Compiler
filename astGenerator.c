/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _ASTGENERATORC
#define _ASTGENERATORC
#include "ASTNodeDef.h"
#include "astGenerator.h"
#include "lexerDef.h"
#include "grammar_InitDef.h"
#include "parserDef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Converts enum used for operators in Parse Tree to enum used for operators in AST
enum Operator getOperator(struct ParseTreeNode *root)
{
	switch(root->leftChild->token_info.token)
	{
		case PLUS:
			return OP_PLUS;
		case MINUS:
			return OP_MINUS;
		case MUL:
			return OP_MUL;
		case DIV:
			return OP_DIV;
		case AND:
			return OP_AND;
		case OR:
			return OP_OR;
		case LT:
			return OP_LT;
		case LE:
			return OP_LE;
		case GE:
			return OP_GE;
		case GT:
			return OP_GT;
		case EQ:
			return OP_EQ;
		case NE:
			return OP_NE;
		default:
			printf("getOperator ERROR");
			break;
	}
	return OP_PLUS;
}


//Utility function to initialize struct ASTNode pointer 
struct ASTNode *createASTNode(enum NodeType nodeType)
{
    struct ASTNode *node=malloc(sizeof(struct ASTNode));
    node->tag=nodeType;
    node->lineNumber=-1;
    return node;
}


//Retrieves Nth child of Parse Tree Node while traversing its children starting from left
struct ParseTreeNode *getNthChild(struct ParseTreeNode *parent,int childNumber)
{
    struct ParseTreeNode *result=parent->leftChild;
    childNumber--;
    while(childNumber--)result=result->rightSibling;
    return result;
}


//Utility function to retrieve datatype while traversing output parameter list
//Note : Only works if the nonterminal expanded is 'datatype' not 'type'
enum Datatype getDatatype(struct ParseTreeNode* pt)
{
	if(pt->ruleNumber==16)return DT_INTEGER;
	else if(pt->ruleNumber==17)return DT_REAL;
	else if(pt->ruleNumber==18)return DT_BOOLEAN;
	else if(pt->ruleNumber==19)return DT_ARRAY;
	else
	{
		printf("getDatatype ERROR");
		return DT_INTEGER;
	}
}

//Utility function to retrieve datatype while traversing input parameter list
//Note : Only works if the nonterminal expanded is 'type' not 'datatype'
enum Datatype getType(struct ParseTreeNode* pt)
{
	if(pt->ruleNumber==21)return DT_INTEGER;
	else if(pt->ruleNumber==22)return DT_REAL;
	else if(pt->ruleNumber==23)return DT_BOOLEAN;
	else
	{
		printf("getType ERROR");
		return DT_INTEGER;
	}
}


//Utility fucntion to assign the next field of any of the statements node
void assignNext(struct ASTNode *left, struct ASTNode *right)
{
	switch(left->tag)
	{
		case INPUT_NODE:left->node.inputNode.next=right;
			return;
		case OUTPUT_NODE:left->node.outputNode.next=right;
			return;
		case MODULE_DECLARE_NODE:left->node.moduleDeclareNode.next=right;
			return;
		case MODULE_NODE:left->node.moduleNode.next=right;
			return;
		case ASSIGN_NODE:left->node.assignNode.next=right;
			return;
		case MODULE_REUSE_NODE:left->node.moduleReuseNode.next=right;
			return;
		case CONDITION_NODE:left->node.conditionNode.next=right;
			return;
		case CASE_NODE:left->node.caseNode.next=right;
			return;
		case FOR_NODE:left->node.forNode.next=right;
			return;
		case WHILE_NODE:left->node.whileNode.next=right;
			return;
		case DECLARE_NODE:left->node.declareNode.next=right;
			return;
	}
}



//Creates Abstract Syntax Tree in a recursive manner
struct ASTNode *createAST(struct ParseTreeNode *root)
{
	struct ASTNode *result;
	struct ASTNode *tempAST;
	struct ASTNode *ASTptr;
	struct ParseTreeNode *typeNode;
	enum Datatype dt;
	int tmp;
	switch(root->ruleNumber)
	{
		case 0:
			result=createASTNode(PROGRAM_NODE);
			result->lineNumber=1;
			result->node.programNode.moduleDeclarations=createAST(getNthChild(root,1));
			result->node.programNode.otherModules1=createAST(getNthChild(root,2));
			result->node.programNode.driverModule=createAST(getNthChild(root,3));
			result->node.programNode.otherModules2=createAST(getNthChild(root,4));
			break;
		case 1:
			result=createAST(getNthChild(root,1));
			result->node.moduleDeclareNode.next=createAST(getNthChild(root,2));
			break;
		case 2:
			result=NULL;
			break;
		case 3:
			result=createASTNode(MODULE_DECLARE_NODE);
			strcpy(result->node.moduleDeclareNode.moduleName,getNthChild(root,3)->token_info.lexeme);
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			//next=0.next DONE IN PARENT in case 2
			break;
		case 4:
			result=createAST(getNthChild(root,1));
			result->node.moduleNode.next=createAST(getNthChild(root,2));
			break;
		case 5:
			result=NULL;
			break;
		case 6:
			result=createASTNode(MODULE_NODE);
			strcpy(result->node.moduleNode.moduleName, "driver");
			result->node.moduleNode.inputList=NULL;
			result->node.moduleNode.ret=NULL;
			result->node.moduleNode.body=createAST(getNthChild(root,5));
			result->node.moduleNode.startLine=getNthChild(getNthChild(root,5),1)->token_info.lineno;
			result->node.moduleNode.endLine=getNthChild(getNthChild(root,5),3)->token_info.lineno;
			result->node.moduleNode.next=NULL;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
		case 7:
			result=createASTNode(MODULE_NODE);
			strcpy(result->node.moduleNode.moduleName, getNthChild(root,3)->token_info.lexeme);
			result->node.moduleNode.inputList=createAST(getNthChild(root,8));
			result->node.moduleNode.ret=createAST(getNthChild(root,11));
			result->node.moduleNode.body=createAST(getNthChild(root,12));
			result->node.moduleNode.startLine=getNthChild(getNthChild(root,12),1)->token_info.lineno;
			result->node.moduleNode.endLine=getNthChild(getNthChild(root,12),3)->token_info.lineno;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
		case 8:
			result=createAST(getNthChild(root,3));
			break;
		case 9:
			result=NULL;
			break;
		case 10:
			result=createASTNode(PARA_LIST_NODE);
			strcpy(result->node.paraListNode.name, getNthChild(root,1)->token_info.lexeme);
			typeNode=getNthChild(root,3);
			dt=getDatatype(typeNode);
			if(dt==DT_ARRAY)
				result->node.paraListNode.type=getType(getNthChild(typeNode,6));
			else
				result->node.paraListNode.type=dt;
			result->node.paraListNode.Range=createAST(typeNode);
			result->node.paraListNode.next=createAST(getNthChild(root,4));
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
		case 11:
			result=createASTNode(PARA_LIST_NODE);
			strcpy(result->node.paraListNode.name, getNthChild(root,2)->token_info.lexeme);
			typeNode=getNthChild(root,4);
			dt=getDatatype(typeNode);
			if(dt==DT_ARRAY)
				result->node.paraListNode.type=getType(getNthChild(typeNode,6));
			else
				result->node.paraListNode.type=dt;
			result->node.paraListNode.Range=createAST(typeNode);
			result->node.paraListNode.next=createAST(getNthChild(root,5));
			result->lineNumber=getNthChild(root,2)->token_info.lineno;
			break;
		case 12:
			result=NULL;
			break;
		case 13:
			result=createASTNode(PARA_LIST_NODE);
			strcpy(result->node.paraListNode.name, getNthChild(root,1)->token_info.lexeme);
			typeNode=getNthChild(root,3);
			result->node.paraListNode.type=getType(typeNode);
			result->node.paraListNode.Range=NULL;	
			result->node.paraListNode.next=createAST(getNthChild(root,4));
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
		case 14:
			result=createASTNode(PARA_LIST_NODE);
			strcpy(result->node.paraListNode.name, getNthChild(root,2)->token_info.lexeme);
			typeNode=getNthChild(root,4);
			result->node.paraListNode.type=getType(typeNode);
			result->node.paraListNode.Range=NULL;	
			result->node.paraListNode.next=createAST(getNthChild(root,5));
			result->lineNumber=getNthChild(root,2)->token_info.lineno;
			break;
		case 15:
			result=NULL;
			break;
		case 16:
		case 17:
		case 18:
			result=NULL;
			break;
		case 19:
			result=createAST(getNthChild(root,3));
			//HANDLED IN CASE 10
			break;
		case 20:
			result=createASTNode(RANGE_NODE);
			result->node.rangeNode.Range1=createAST(getNthChild(root,1));
			result->node.rangeNode.Range2=createAST(getNthChild(root,3));
			result->lineNumber=getNthChild(root,2)->token_info.lineno;
			break;
		case 21:
			break;
		case 22:
			break;
		case 23:
			break;
		case 24:
			result=createAST(getNthChild(root,2));
			break;
		case 25:
			result=createAST(getNthChild(root,1));
			tempAST=createAST(getNthChild(root,2));
			assignNext(result,tempAST);
			break;

		case 26:
			result=NULL;
			break;

		case 27:
			result=createAST(getNthChild(root,1));
			break;

		case 28:
			result=createAST(getNthChild(root,1));
			break;

		case 29:
			result=createAST(getNthChild(root,1));
			break;
			    
		case 30:
			result=createAST(getNthChild(root,1));
			break;

		case 31:
			result=createAST(getNthChild(root,1));
			break;

		case 32:
			result=createASTNode(INPUT_NODE);
			strcpy(result->node.inputNode.name,getNthChild(root,3)->token_info.lexeme);
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 33:
			result=createASTNode(OUTPUT_NODE);
			result->node.outputNode.value=createAST(getNthChild(root,3));
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 34:
			result=createAST(getNthChild(root,1));
			break;

		case 35:
			result=createASTNode(BOOL_NODE);
			result->node.boolNode.value=BOOL_TRUE;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 36:
			result=createASTNode(BOOL_NODE);
			result->node.boolNode.value=BOOL_FALSE;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 37:
			result=createAST(getNthChild(root,2));
			strcpy(result->node.idNode.varName,getNthChild(root,1)->token_info.lexeme);
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 38:
			result=createASTNode(NUM_NODE);
			result->node.numNode.num=getNthChild(root,1)->token_info.value.value.num;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 39:
			result=createASTNode(RNUM_NODE);
			result->node.rNumNode.rnum=getNthChild(root,1)->token_info.value.value.rnum;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 40:
			result=createASTNode(ID_NODE);
			result->node.idNode.index=createAST(getNthChild(root,2));
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 41:
			result=createASTNode(ID_NODE);
			result->node.idNode.index=NULL;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
		case 42:
			result=createASTNode(ID_NODE);
			strcpy(result->node.idNode.varName,getNthChild(root,1)->token_info.lexeme);
			result->node.idNode.index=NULL;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 43:
			result=createASTNode(NUM_NODE);
			result->node.numNode.num=getNthChild(root,1)->token_info.value.value.num;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 44:
			result=createAST(getNthChild(root,1));
			//1.next = 0.next
			break;

		case 45:
			result=createAST(getNthChild(root,1));
			//1.next = 0.next
			break;

		case 46:
			result=createAST(getNthChild(root,2));
			//1.next = 0.next
			strcpy(result->node.assignNode.LHS,getNthChild(root,1)->token_info.lexeme);
			break;

		case 47:
			result=createAST(getNthChild(root,1));
			//1.next = 0.next
			//
			break;

		case 48:
			result=createAST(getNthChild(root,1));
			//1.next = 0.next
			//
			break;

		case 49:
			result=createASTNode(ASSIGN_NODE);
			result->node.assignNode.expr=createAST(getNthChild(root,2));
			result->node.assignNode.index=NULL;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 50:
			result=createASTNode(ASSIGN_NODE);
			result->node.assignNode.expr=createAST(getNthChild(root,5));
			result->node.assignNode.index=createAST(getNthChild(root,2));
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 51:
			result=createASTNode(NUM_NODE);
			result->node.numNode.num=getNthChild(root,1)->token_info.value.value.num;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 52:
			result=createASTNode(ID_NODE);
			strcpy(result->node.idNode.varName,getNthChild(root,1)->token_info.lexeme);
			result->node.idNode.index=NULL;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 53:
			result=createASTNode(MODULE_REUSE_NODE);
			result->node.moduleReuseNode.optional=createAST(getNthChild(root,1));
			result->node.moduleReuseNode.idList=createAST(getNthChild(root,7));
			strcpy(result->node.moduleReuseNode.id,getNthChild(root,4)->token_info.lexeme);
			result->lineNumber=getNthChild(root,2)->token_info.lineno;
			break;

		case 54:
			result=createAST(getNthChild(root,2));
			break;

		case 55:
			result=NULL;
			break;

		case 56:
			result=createASTNode(ID_LIST_NODE);
			strcpy(result->node.idListNode.varName,getNthChild(root,1)->token_info.lexeme);
			result->node.idListNode.next=createAST(getNthChild(root,2));
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;

		case 57:
			result=createASTNode(ID_LIST_NODE);
			strcpy(result->node.idListNode.varName,getNthChild(root,2)->token_info.lexeme);
			result->node.idListNode.next=createAST(getNthChild(root,3));
			result->lineNumber=getNthChild(root,2)->token_info.lineno;
			break;

		case 58:
			result=NULL;
			break;

	    case 59:
		    	result=createAST(getNthChild(root,1));
		    	break;
	    case 60:
		    	result=createASTNode(UNARY_NODE);
		    	result->node.unaryNode.op=getOperator(getNthChild(root,1));
		    	result->node.unaryNode.expr=createAST(getNthChild(root,2));
		    	result->lineNumber=getNthChild(root,1)->leftChild->token_info.lineno;
		    	break;
	    case 61:
			result=createAST(getNthChild(root,2));
			if(result==NULL)
			{
				result=createAST(getNthChild(root,1));
			}
			else	
			{
				tempAST=result;
				while(tempAST->node.binaryNode.expr1!=NULL)tempAST=tempAST->node.binaryNode.expr1;
				tempAST->node.binaryNode.expr1=createAST(getNthChild(root,1));
			}
			break;
	    case 62:
			tempAST=createASTNode(BINARY_NODE);
			tempAST->node.binaryNode.op=getOperator(getNthChild(root,1));
			tempAST->node.binaryNode.expr1=NULL;
			tempAST->node.binaryNode.expr2=createAST(getNthChild(root,2));
		    	result=createAST(getNthChild(root,3));
		    	if(result==NULL)
			{
				result=tempAST;
			}
			else	
			{
				ASTptr=result;
				while(ASTptr->node.binaryNode.expr1!=NULL)ASTptr=ASTptr->node.binaryNode.expr1;
				ASTptr->node.binaryNode.expr1=tempAST;
			}
		    	tempAST->lineNumber=getNthChild(root,1)->leftChild->token_info.lineno;
		    	break;
	    case 63:
			result=NULL;
			break;
	    case 64:
			result=createAST(getNthChild(root,2));
			if(result==NULL)
				result=createAST(getNthChild(root,1));
			else	
				result->node.binaryNode.expr1=createAST(getNthChild(root,1));
			break;
	    case 65:
	    		result=createASTNode(BOOL_NODE);
		    	result->node.boolNode.value=BOOL_TRUE;
		    	result->lineNumber=getNthChild(root,1)->token_info.lineno;
		    	break;
	    case 66:
		    	result=createASTNode(BOOL_NODE);
		    	result->node.boolNode.value=BOOL_FALSE;
		    	result->lineNumber=getNthChild(root,1)->token_info.lineno;
		    	break;
	    case 67:
			result=createASTNode(BINARY_NODE);
			result->node.binaryNode.op=getOperator(getNthChild(root,1));
			result->node.binaryNode.expr2=createAST(getNthChild(root,2));
			result->lineNumber=getNthChild(root,1)->leftChild->token_info.lineno;
			break;
	    case 68:
			result=NULL;
			break;
	    case 69:
			result=createAST(getNthChild(root,2));
			if(result==NULL)
			{
				result=createAST(getNthChild(root,1));
			}
			else	
			{
				tempAST=result;
				while(tempAST->node.binaryNode.expr1!=NULL)tempAST=tempAST->node.binaryNode.expr1;
				tempAST->node.binaryNode.expr1=createAST(getNthChild(root,1));
			}
			break;
	    case 70:
			tempAST=createASTNode(BINARY_NODE);
			tempAST->node.binaryNode.op=getOperator(getNthChild(root,1));
			tempAST->node.binaryNode.expr1=NULL;
			tempAST->node.binaryNode.expr2=createAST(getNthChild(root,2));
	    		result=createAST(getNthChild(root,3));
	    		if(result==NULL)
			{
				result=tempAST;
			}
			else	
			{
				ASTptr=result;
				while(ASTptr->node.binaryNode.expr1!=NULL)ASTptr=ASTptr->node.binaryNode.expr1;
				ASTptr->node.binaryNode.expr1=tempAST;
			}
	    		tempAST->lineNumber=getNthChild(root,1)->leftChild->token_info.lineno;
			break;
	    case 71:
			result=NULL;
			break;
	    case 72:
			result=createAST(getNthChild(root,2));
			if(result==NULL)
			{
				result=createAST(getNthChild(root,1));
			}
			else	
			{
				tempAST=result;
				while(tempAST->node.binaryNode.expr1!=NULL)tempAST=tempAST->node.binaryNode.expr1;
				tempAST->node.binaryNode.expr1=createAST(getNthChild(root,1));
			}
			break;
	    case 73:
			tempAST=createASTNode(BINARY_NODE);
			tempAST->node.binaryNode.op=getOperator(getNthChild(root,1));
			tempAST->node.binaryNode.expr1=NULL;
			tempAST->node.binaryNode.expr2=createAST(getNthChild(root,2));
		    	result=createAST(getNthChild(root,3));
		    	if(result==NULL)
			{
				result=tempAST;
			}
			else	
			{
				ASTptr=result;
				while(ASTptr->node.binaryNode.expr1!=NULL)ASTptr=ASTptr->node.binaryNode.expr1;
				ASTptr->node.binaryNode.expr1=tempAST;
			}
		    	tempAST->lineNumber=getNthChild(root,1)->leftChild->token_info.lineno;
			break;
	    case 74:
			result=NULL;
			break;
	    case 75:
			result=createAST(getNthChild(root,1));
			break;
	    case 76:
			result=createAST(getNthChild(root,2));
			break;
	    //case 77-88 assigned in parent itself
	    case 89:
		    	result=createASTNode(DECLARE_NODE);
		    	result->node.declareNode.idList=createAST(getNthChild(root,2));

		       	typeNode=getNthChild(root,4);
				dt=getDatatype(typeNode);
				if(dt==DT_ARRAY)
					result->node.declareNode.dataType=getType(getNthChild(typeNode,6));
				else
					result->node.declareNode.dataType=dt;

			result->node.declareNode.Range=createAST(getNthChild(root,4));
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
	    case 90:
			result=createASTNode(CONDITION_NODE);
			strcpy(result->node.conditionNode.id,getNthChild(root,3)->token_info.lexeme);
			result->node.conditionNode.Case=createAST(getNthChild(root,6));
			result->node.conditionNode.Default=createAST(getNthChild(root,7));
			result->node.conditionNode.startLine=getNthChild(root,5)->token_info.lineno;
			result->node.conditionNode.endLine=getNthChild(root,8)->token_info.lineno;
			tmp=(getNthChild(root,7)->ruleNumber);
			if(tmp==98)
				result->node.conditionNode.presentDefault=0;
			else
				result->node.conditionNode.presentDefault=1;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
	    case 91:
			result=createASTNode(CASE_NODE);
			result->node.caseNode.value=createAST(getNthChild(root,2));
			result->node.caseNode.stmt=createAST(getNthChild(root,4));
			result->node.caseNode.next=createAST(getNthChild(root,7));
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
	    case 92:
			result=createASTNode(CASE_NODE);
			result->node.caseNode.value=createAST(getNthChild(root,2));
			result->node.caseNode.stmt=createAST(getNthChild(root,4));
			result->node.caseNode.next=createAST(getNthChild(root,7));
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
	    case 93:
			result=NULL;
			break;
	    case 94:
			result=createASTNode(NUM_NODE);
			result->node.numNode.num=getNthChild(root,1)->token_info.value.value.num;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
	    case 95:
			result=createASTNode(BOOL_NODE);
			result->node.boolNode.value=BOOL_TRUE;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
	    case 96:
			result=createASTNode(BOOL_NODE);
			result->node.boolNode.value=BOOL_FALSE;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
	    case 97:
			result=createAST(getNthChild(root,3));
			if(result!=NULL)
				result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
	    case 98:
			result=NULL;
			break;
	    case 99:
			result=createASTNode(FOR_NODE);
			strcpy(result->node.forNode.id,getNthChild(root,3)->token_info.lexeme);
			result->node.forNode.range=createAST(getNthChild(root,5));
			result->node.forNode.stmt=createAST(getNthChild(root,8));
			result->node.forNode.startLine=getNthChild(root,7)->token_info.lineno;
			result->node.forNode.endLine=getNthChild(root,9)->token_info.lineno;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
	    case 100:
			result=createASTNode(WHILE_NODE);
			result->node.whileNode.expr=createAST(getNthChild(root,3));
			result->node.whileNode.stmt=createAST(getNthChild(root,6));
			result->node.whileNode.startLine=getNthChild(root,5)->token_info.lineno;
			result->node.whileNode.endLine=getNthChild(root,7)->token_info.lineno;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
	    case 101:
			result=createASTNode(RANGE_NODE);
			result->node.rangeNode.Range1=createASTNode(NUM_NODE);
			result->node.rangeNode.Range1->lineNumber=getNthChild(root,1)->token_info.lineno;
			result->node.rangeNode.Range1->node.numNode.num=getNthChild(root,1)->token_info.value.value.num;

			result->node.rangeNode.Range2=createASTNode(NUM_NODE);
			result->node.rangeNode.Range2->node.numNode.num=getNthChild(root,3)->token_info.value.value.num;
			result->node.rangeNode.Range2->lineNumber=getNthChild(root,1)->token_info.lineno;
			result->lineNumber=getNthChild(root,1)->token_info.lineno;
			break;
	}
	return result;
}

//Utility function for printing AST
void printSpaces2(int n)
{
	while(n--)printf("--");
}


//Prints the Abstract Syntax tree using inorder traversal
void printInlineAstTree(struct ASTNode *root, int spaces){

	if(root == NULL)return;
	printf("%3d",root->lineNumber);
	printSpaces2(spaces);
	switch(root->tag){
		case PROGRAM_NODE:
			printf("Program Node\n");
			printInlineAstTree(root->node.programNode.moduleDeclarations,spaces+1);
			printInlineAstTree(root->node.programNode.otherModules1,spaces+1);
			printInlineAstTree(root->node.programNode.driverModule,spaces+1);
			printInlineAstTree(root->node.programNode.otherModules2,spaces+1);
			break;
		case MODULE_DECLARE_NODE:
			printf("ModuleDeclareNode, Name- %s\n",root->node.moduleDeclareNode.moduleName);
			printInlineAstTree(root->node.moduleDeclareNode.next,spaces);
			break;
		case ID_NODE:
			printf("ID Node, Name - %s\n",root->node.idNode.varName);
			printInlineAstTree(root->node.idNode.index, spaces+1);
			break;
		case MODULE_NODE:
			printf("Module_Node, Name - %s\n",root->node.moduleNode.moduleName);
			printInlineAstTree(root->node.moduleNode.inputList,spaces+1);
			printInlineAstTree(root->node.moduleNode.ret,spaces+1);
			printInlineAstTree(root->node.moduleNode.body,spaces+1);
			printInlineAstTree(root->node.moduleNode.next,spaces);
			break;
		case PARA_LIST_NODE:
			printf("ParaListNode, Name- %s\n", root->node.paraListNode.name);
			printInlineAstTree(root->node.paraListNode.Range,spaces+1);
			printInlineAstTree(root->node.paraListNode.next,spaces);
			break;
		case NUM_NODE:
			printf("NumNode, Num-%d\n",root->node.numNode.num );
			break;
		case RNUM_NODE:
			printf("Rnumnode, Rnum-%f\n",root->node.rNumNode.rnum);
			break;
		case BOOL_NODE:
			printf("BoolNode, Value- %d\n",root->node.boolNode.value );
			break;
		case INPUT_NODE:
			printf("Input Node, Name- %s\n",root->node.inputNode.name );
			printInlineAstTree(root->node.inputNode.next,spaces);
			break;
		case OUTPUT_NODE:
			printf("Output Node\n" );
			printInlineAstTree(root->node.outputNode.value,spaces+1);
			printInlineAstTree(root->node.outputNode.next,spaces);
			break;
		case RANGE_NODE:
			printf("Range Node\n");
			printInlineAstTree(root->node.rangeNode.Range1,spaces+1);
			printInlineAstTree(root->node.rangeNode.Range2,spaces+1);
			break;
		case ASSIGN_NODE:
			printf("Assign Node, LHS- %s\n",root->node.assignNode.LHS );
			printInlineAstTree(root->node.assignNode.index,spaces+1);
			printInlineAstTree(root->node.assignNode.expr,spaces+1);
			printInlineAstTree(root->node.assignNode.next,spaces);
			break;
		case MODULE_REUSE_NODE:
			printf("ModuleReuseNode, ID-%s\n",root->node.moduleReuseNode.id );
			printInlineAstTree(root->node.moduleReuseNode.optional,spaces+1);
			printInlineAstTree(root->node.moduleReuseNode.idList,spaces+1);
			printInlineAstTree(root->node.moduleReuseNode.next,spaces);
			break;
		case ID_LIST_NODE:
			printf("IDListNode, Name- %s\n",root->node.idListNode.varName );
			printInlineAstTree(root->node.idListNode.next,spaces);
			break;
		case DECLARE_NODE:
			printf("DeclareNode, type-%d\n",root->node.declareNode.dataType );
			printInlineAstTree(root->node.declareNode.idList,spaces+1);
			printInlineAstTree(root->node.declareNode.Range,spaces+1);
			printInlineAstTree(root->node.declareNode.next,spaces);
			break;
		case CONDITION_NODE:
			printf("Condition Node, ID-%s\n",root->node.conditionNode.id );
			printInlineAstTree(root->node.conditionNode.Case,spaces+1);
			printInlineAstTree(root->node.conditionNode.Default,spaces+1);
			printInlineAstTree(root->node.conditionNode.next,spaces);
			break;
		case CASE_NODE:
			printf("Case Node\n");
			printInlineAstTree(root->node.caseNode.value,spaces+1);
			printInlineAstTree(root->node.caseNode.stmt,spaces+1);
			printInlineAstTree(root->node.caseNode.next,spaces);
			break;
		case FOR_NODE:
			printf("For Node, ID-%s\n",root->node.forNode.id );
			printInlineAstTree(root->node.forNode.range,spaces+1);
			printInlineAstTree(root->node.forNode.stmt,spaces+1);
			printInlineAstTree(root->node.forNode.next,spaces);
			break;
		case WHILE_NODE:
			printf("While Node\n");
			printInlineAstTree(root->node.whileNode.expr,spaces+1);
			printInlineAstTree(root->node.whileNode.stmt,spaces+1);
			printInlineAstTree(root->node.whileNode.next,spaces);
			break;
		case UNARY_NODE:
			printf("Unary Node, Op- %d\n",root->node.unaryNode.op);
			printInlineAstTree(root->node.unaryNode.expr,spaces+1);
			break;
		case BINARY_NODE:
			printf("Binary Node, Op-%d\n", root->node.binaryNode.op);
			printInlineAstTree(root->node.binaryNode.expr1,spaces+1);
			printInlineAstTree(root->node.binaryNode.expr2,spaces+1);
			break;
		default:
			printf("No Node Found\n");

	}
}


//count total nodes present in the AST in a recursive fashion
void countASTNodes(struct ASTNode *root, int *count)
{
	if(root == NULL)	return;
	*count+=1;
	switch(root->tag){
		case PROGRAM_NODE:
			countASTNodes(root->node.programNode.moduleDeclarations,count);
			countASTNodes(root->node.programNode.otherModules1,count);
			countASTNodes(root->node.programNode.driverModule,count);
			countASTNodes(root->node.programNode.otherModules2,count);
			break;
		case MODULE_DECLARE_NODE:
			countASTNodes(root->node.moduleDeclareNode.next,count);
			break;
		case ID_NODE:
			countASTNodes(root->node.idNode.index, count);
			break;
		case MODULE_NODE:
			countASTNodes(root->node.moduleNode.inputList,count);
			countASTNodes(root->node.moduleNode.ret,count);
			countASTNodes(root->node.moduleNode.body,count);
			countASTNodes(root->node.moduleNode.next,count);
			break;
		case PARA_LIST_NODE:
			countASTNodes(root->node.paraListNode.Range,count);
			countASTNodes(root->node.paraListNode.next,count);
			break;
		case NUM_NODE:
			break;
		case RNUM_NODE:
			break;
		case BOOL_NODE:
			break;
		case INPUT_NODE:
			countASTNodes(root->node.inputNode.next,count);
			break;
		case OUTPUT_NODE:
			countASTNodes(root->node.outputNode.value,count);
			countASTNodes(root->node.outputNode.next,count);
			break;
		case RANGE_NODE:
			countASTNodes(root->node.rangeNode.Range1,count);
			countASTNodes(root->node.rangeNode.Range2,count);
			break;
		case ASSIGN_NODE:
			countASTNodes(root->node.assignNode.index,count);
			countASTNodes(root->node.assignNode.expr,count);
			countASTNodes(root->node.assignNode.next,count);
			break;
		case MODULE_REUSE_NODE:
			countASTNodes(root->node.moduleReuseNode.optional,count);
			countASTNodes(root->node.moduleReuseNode.idList,count);
			countASTNodes(root->node.moduleReuseNode.next,count);
			break;
		case ID_LIST_NODE:
			countASTNodes(root->node.idListNode.next,count);
			break;
		case DECLARE_NODE:
			countASTNodes(root->node.declareNode.idList,count);
			countASTNodes(root->node.declareNode.Range,count);
			countASTNodes(root->node.declareNode.next,count);
			break;
		case CONDITION_NODE:
			countASTNodes(root->node.conditionNode.Case,count);
			countASTNodes(root->node.conditionNode.Default,count);
			countASTNodes(root->node.conditionNode.next,count);
			break;
		case CASE_NODE:
			countASTNodes(root->node.caseNode.value,count);
			countASTNodes(root->node.caseNode.stmt,count);
			countASTNodes(root->node.caseNode.next,count);
			break;
		case FOR_NODE:
			countASTNodes(root->node.forNode.range,count);
			countASTNodes(root->node.forNode.stmt,count);
			countASTNodes(root->node.forNode.next,count);
			break;
		case WHILE_NODE:
			countASTNodes(root->node.whileNode.expr,count);
			countASTNodes(root->node.whileNode.stmt,count);
			countASTNodes(root->node.whileNode.next,count);
			break;
		case UNARY_NODE:
			countASTNodes(root->node.unaryNode.expr,count);
			break;
		case BINARY_NODE:
			countASTNodes(root->node.binaryNode.expr1,count);
			countASTNodes(root->node.binaryNode.expr2,count);
			break;
		default:
			printf("No Node Found\n");

	}
}

#endif
