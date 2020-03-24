#include "ASTNodeDef.h"
#include "parserDef.h"
#include <stdlib.h>
#include <string.h>


struct ASTNode *createASTNode(enum NodeType nodeType)
{
    struct ASTNode *node=malloc(sizeof(struct ASTNode));
    node->tag=nodeType;
    return node;
}
struct ParseTreeNode *getNthChild(struct ParseTreeNode *parent,int childNumber)
{
    struct ParseTreeNode *result=parent->leftChild;
    childNumber--;
    while(childNumber--)result=result->rightSibling;
    return result;
}
enum Datatype getDatatype(struct ParseTreeNode* pt)//Only works if the nonterminal expanded is 'datatype' not 'type'
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
enum Datatype getType(struct ParseTreeNode* pt)//Only works if the nonterminal expanded is 'type' not 'datatype'
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
	case 10:
		result=createASTNode(PARA_LIST_NODE);
		strcpy(result->node.paraListNode.name, getNthChild(root,1)->token_info.lexeme);
		struct ParseTreeNode *typeNode=getNthChild(root,3);
		enum Datatype dt=getDatatype(typeNode);
		if(dt==DT_ARRAY)
		{
			result->node.paraListNode.type=getType(getNthChild(typeNode,6));
			result->node.paraListNode.Range=createAST(getNthChild(typeNode,3));
		}
		else
		{
			result->node.paraListNode.type=dt;
			result->node.paraListNode.Range=NULL;
		}		
		result->node.paraListNode.next=createAST(getNthChild(root,4));
		break;
	case 11:
		result=createASTNode(PARA_LIST_NODE);
		strcpy(result->node.paraListNode.name, getNthChild(root,2)->token_info.lexeme);
		struct ParseTreeNode *typeNode=getNthChild(root,4);
		enum Datatype dt=getDatatype(typeNode);
		if(dt==DT_ARRAY)
		{
			result->node.paraListNode.type=getType(getNthChild(typeNode,6));
			result->node.paraListNode.Range=createAST(getNthChild(typeNode,3));
		}typeNode
		else
		{
			result->node.paraListNode.type=dt;
			result->node.paraListNode.Range=NULL;
		}		
		result->node.paraListNode.next=createAST(getNthChild(root,5));
		break;
	case 12:
		result=NULL;
		break;
	case 13:
		result=createASTNode(PARA_LIST_NODE);
		strcpy(result->node.paraListNode.name, getNthChild(root,1)->token_info.lexeme);
		struct ParseTreeNode *typeNode=getNthChild(root,3);
		result->node.paraListNode.type=getType(typeNode);
		result->node.paraListNode.Range=NULL;	
		result->node.paraListNode.next=createAST(getNthChild(root,4));
		break;
	case 14:
		result=createASTNode(PARA_LIST_NODE);
		strcpy(result->node.paraListNode.name, getNthChild(root,1)->token_info.lexeme);
		struct ParseTreeNode *typeNode=getNthChild(root,4);
		result->node.paraListNode.type=getType(typeNode);
		result->node.paraListNode.Range=NULL;	
		result->node.paraListNode.next=createAST(getNthChild(root,4));
		break;
	case 15:
		result=NULL;
		break;
	case 16:case 17:case 18:case 19:
		//HANDLED IN CASE 10
		break;
	case 20:
		result=createASTNode(RANGE_NODE);
		result->node.rangeNode.Range1=createAST(getNthChild(root,1));
		result->node.rangeNode.Range2=createAST(getNthChild(root,3));
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
		result->next=createAST(getNthChild(root,2));
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
		break;

	case 33:
		result=createASTNode(OUTPUT_NODE);
		result->node.outputNode.value=createAST(getNthChild(root,3));
		break;

	case 34:
		result=createAST(getNthChild(root,1));
		break;

	case 35:
		result=createASTNode(BOOL_NODE);
		result->node.boolNode.value=BOOL_TRUE;
		break;

	case 36:
		result=createASTNode(BOOL_NODE);
		result->node.boolNode.value=BOOL_FALSE;
		break;

	case 37:
		result=createAST(getNthChild(root,2));
		strcpy(result->node.idNode.varName,getNthChild(root,1)->token_info.lexeme);
		break;

	case 38:
		result=createASTNode(NUM_NODE);
		result->node.numNode.num=getNthChild(root,1)->token_info.value.value.num;
		break;

	case 39:
		result=createASTNode(RNUM_NODE);
		result->node.rNumNode.rnum=getNthChild(root,1)->token_info.value.value.rnum;
		break;

	case 40:
		result=createASTNode(ID_NODE);
		result->node.idNode.index=createAST(getNthChild(root,2));
		break;

	case 41:
		result=createASTNode(ID_NODE);
		result->node.idNode.index=NULL;
		break;
	case 42:
		result=createASTNode(ID_NODE);
		strcpy(result->node.idNode.varName,getNthChild(root,1)->token_info.lexeme);
		result->node.idNode.index=NULL;
		break;

	case 43:
		result=createASTNode(NUM_NODE);
		result->node.numNode.num=getNthChild(root,1)->token_info.value.value.num;
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
		//
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
		//result->node.assignNode->LHS=
		break;

	case 50:
		break;

	case 51:
		result=createASTNode(NUM_NODE);
		result-node.numNode.num=getNthChild(root,1)->value;
		break;

	case 52:
		result=createASTNode(ID_NODE);
		strcpy(result->node.idNode.varName,getNthChild(root,1)->token_info.lexeme);
		result-node.idNode.index=NULL;
		break;

	case 53:
		//result=createASTNode()
		break;

	case 54:
		result=createAST(getNthChild(root,2));
		break;

	case 55:
		result=NULL;
		break;

	case 56:
		result=createASTNode(ID_LIST_NODE);
		strcpy(result->node.idListNode.varname,getNthChild(root,1)->token_info.lexeme);
		result->node.idListNode.next=createAST(getNthChild(root,2));
		break;

	case 57:
		result=createASTNode(ID_LIST_NODE);
		strcpy(result->node.idListNode.varname,getNthChild(root,2)->token_info.lexeme);
		result->node.idListNode.next=createAST(getNthChild(root,3));
		break;

	case 58:
		result=NULL;
		break;

	//case 30-58
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
		result=createAST(getNthChild(root,2));
		result->node.binaryNode.expr1=createAST(getNthChild(root,1));
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
		result->node.binaryNode.expr2=createAST(getNthChild(root,2));
		break;
        case 68:
		result=result->node.binaryNode.expr1;
		break;
        case 69:
            result=createAST(getNthChild(root,2));
            result->node.binaryNode.expr1=createAST(getNthChild(root,1));
            break;
        case 70:
            result=createASTNode(BINARY_NODE);
            strcpy(result->node.binaryNode.op,getNthChild(root,1)->token_info.lexeme);
            result->node.binaryNode.expr2=createAST(getNthChild(root,3));
            result->node.binaryNode.expr2->node.binaryNode.expr1=createAST(getNthChild(root,2));
            break;
        case 71:
            result=result->node.binaryNode.expr1;
            break;
        case 72:
            result=createAST(getNthChild(root,2));
            result->node.binaryNode.expr1=createAST(getNthChild(root,1));
            break;
        case 73:
            result=createASTNode(BINARY_NODE);
            strcpy(result->node.binaryNode.op,getNthChild(root,1)->token_info.lexeme);
            result->node.binaryNode.expr2=createAST(getNthChild(root,3));
            result->node.binaryNode.expr2->node.binaryNode.expr1=createAST(getNthChild(root,2));
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
        case 90:
            result=createASTNode(CONDITION_NODE);
            strcpy(result->node.conditionNode.id,getNthChild(root,3)->token_info.lexeme);
            result->node.conditionNode.Case=createAST(getNthChild(root,6));
            result->node.conditionNode.Default=createAST(getNthChild(root,7));
            break;
        case 91:
            result=createASTNode(CASE_NODE);
            result->node.caseNode.value=createAST(getNthChild(root,2));
            result->node.caseNode.stmt=createAST(getNthChild(root,4));
            result->node.caseNode.next=createAST(getNthChild(root,7));
            break;
        case 92:
            result=createASTNode(CASE_NODE);
            result->node.caseNode.value=createAST(getNthChild(root,2));
            result->node.caseNode.stmt=createAST(getNthChild(root,4));
            result->node.caseNode.next=createAST(getNthChild(root,7));
            break;
        case 93:
            result=NULL;
        case 94:
            result=createASTNode(NUM_NODE);
            result->node.numNode.num=atoi(root->token_info.lexeme);
        case 95:
            result=createASTNode(BOOL_NODE);
            result->node.boolNode.value=BOOL_TRUE;
            break;
        case 96:
            result=createASTNode(BOOL_NODE);
            result->node.boolNode.value=BOOL_FALSE;
            break;
        case 97:
            result=createAST(getNthChild(root,3))
            break;
        case 98:
            result=NULL;
        case 99:
            result=createASTNode(FOR_NODE);
            strcpy(result->node.forNode.id,getNthChild(root,3)->token_info.lexeme);
            result->node.forNode.range=createAST(getNthChild(root,5));
            result->node.forNode.stmt=createAST(getNthChild(root,8));
            break;
        case 100:
            result=createASTNode(WHILE_NODE);
            result->node.whileNode.expr=createAST(getNthChild(root,3));
            result->node.whileNode.stmt=createAST(getNthChild(root,6));
            break;
        case 101:
            result=createASTNode(RANGE_NODE);
            result->node.rangeNode.Range1=getType(getNthChild(root,1));
            result->node.rangeNode.Range2=getType(getNthChild(root,3));
            break;
    }
    return result;
}

