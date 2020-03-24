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
        case 61:
            result=createAST(getNthChild(root,2));
            result->node.binaryNode.expr1=getNthChild(root,1);
            break;
        case 62:
            result=createASTNode(BINARY_NODE);
            strcpy(result->node.binaryNode.op,getNthChild(root,1)->token_info.lexeme);
        	result->node.binaryNode.expr2=getNthChild(root,3);
        	break;
        case 63:
            result=result->node.binaryNode.expr1;
            break;
        case 64:
            result=createAST(root,2);
            getNthChild(root,2)->node.binaryNode.expr1=getNthChild(root,1);
            break;
        case 65:
        	result=createASTNode(BOOL_NODE);
        	result->node.boolNode.value=BOOL_TRUE;
        	break;
        case 66:
        	result=createASTNode(BOOL_NODE);
        	result->node.boolNode.value=BOOL_FALSE;
        	break;
        case 67:
            result=createASTNode(BINARY_NODE);
            strcpy(result->node.binaryNode.op,getNthChild(root,1)->token_info.lexeme);
            result->node.binaryNode.expr2=getNthChild(root,2);
            break;
        case 68:
            result=result->node.binaryNode.expr1;
            break;
        case 69:
            result=createAST(getNthChild(root,2));
            getNthChild(root,2)->node.addExpr2=createAST(root,1);
            break;
        case 70:
            result=createASTNode(BINARY_NODE);
            strcpy(result->node.binaryNode.op,getNthChild(root,1)->token_info.lexeme);
            result->node.binaryNode.expr2=getNthChild(root,3);
            getNthChild(root,3)->node.expr1=getNthChild(root,2);
            break;
        case 71:
            result=result->node.binaryNode.expr1;
            break;
        case 72:
            result=getNthChild(root,2);
            getNthChild(root,2)->node.binaryNode.expr1=getNthChild(root,1);
            break;
        case 73:
            result=createASTNode(BINARY_NODE);
            strcpy(result->node.binaryNode.op,getNthChild(root,1)->token_info.lexeme);
            result->node.binaryNode.expr2=getNthChild(root,3);
            getNthChild(root,3)->node.expr1=getNthChild(root,2);
            break;
        case 74:
            result=result->node.binaryNode.expr1;
            break;
        case 75:
            result=createAST(getNthChild(root,1));
            break;
        case 76:
            result=createAST(getNthChild(root,2));
            break;
        //case 77-88 assigned in parent itself
        //case 89:
          //  result=createASTNode(DECLARE_NODE);
           // result->node.declareNode.idList=getNthChild(root,2);
           // result->node.declareNode.dataType=createAST(getNthChild(root,4))->node.;
            //result->node.declareNode.Range=createAST(getNthChild(root,4));


    }
    return result;
}

