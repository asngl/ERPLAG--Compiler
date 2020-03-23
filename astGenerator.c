#include "ASTNodeDef.h"
#include "lexerDef.h"
#include "parserDef.h"
#include <stdlib.h>
#include <string.h>


struct ASTNode *createASTNode(enum NodeType nodeType)
{   
    struct ASTNode *node=malloc(sizeof(struct ASTNode));
    node->tag=nodeType;
}
struct ParseTreeNode *getNthChild(struct ParseTreeNode *parent,int childNumber)
{
    struct ParseTreeNode *result=parent->leftChild;
    childNumber--;
    while(childNumber--)result=result->rightSibling;
    return result;
}
struct ASTNode *createAST(struct ParseTreeNode *root)
{
    struct ASTNode *result;
    switch(root->ruleNumber)
    {
        case 0:
		result=createASTNode(PROGRAM_NODE);
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
		result=createAST(getNthChild(root,5));
		break;
	case 7:
		result=createASTNode(MODULE_NODE);
		strcpy(result->node.moduleNode.moduleName, getNthChild(root,3)->token_info.lexeme);
		result->node.moduleNode.inputList=createAST(getNthChild(root,8));
		result->node.moduleNode.ret=createAST(getNthChild(root,11));
		result->node.moduleNode.body=createAST(getNthChild(root,12));
		break;
	case 8:
		result=createAST(getNthChild(root,3));
		break;
	case 9:
		result=NULL;
		break;
//	case 10:
//		result=createASTNode(PARA_LIST_NODE);
//		strcpy(result->node.paraListNode.name, getNthChild(root,1)->token_info.lexeme);
//		result->node.paraListNode.type=getNthChild(root,3);
//		result->node.paraListNode.next=getNthChild(root,4);
//		result->node.paraListNode.index=getNthChild(root,);
//		break;
//	cases 10-14
	case 15:
		result=NULL;
		break;
	case 16:
		result=createASTNode(NUM_NODE);///////////////check 16-18
		break;
	case 17:
		result=createASTNode(RNUM_NODE);
		break;
	case 18:
		result=createASTNode(BOOL_NODE);
		break;
        case 59:
        	result=createAST(getNthChild(root,1));
        	break;
        case 60:    	
        	result=createASTNode(UNARY_NODE);
        	strcpy(result->node.unaryNode.op,getNthChild(root,1)->token_info.lexeme);
        	result->node.unaryNode.expr=createAST(getNthChild(root,2));
        	break;
        //case 61:
        //	result=createAST(getNthChild(root,2));
        //cases 61-64
        case 65:
        	result=createASTNode(BOOL_NODE);
        	result->node.boolNode.value=BOOL_TRUE;
        	break;
        case 66:
        	result=createASTNode(BOOL_NODE);
        	result->node.boolNode.value=BOOL_FALSE;
        	break;
        
        	
    }
    return result;
}

