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
    }
    return result;
}

