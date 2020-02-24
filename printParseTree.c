#ifndef _PRINT_PARSE_TREE
#define _PRINT_PARSE_TREE

#include "parser.h"

/*
struct ParseTreeNode{
	struct TaggedSymbol s;
	struct ParseTreeNode *leftChild;// Leftmost child
	struct ParseTreeNode *rightSibling;
	struct ParseTreeNode *parent;
	
	struct TAGGED_VALUE value;
	unsigned int lineno;
};
*/
void printNode(struct ParseTreeNode * node, FILE * fp){
	
	//lexeme
	if(root->leftChild == NULL) { //leaf
		fprintf(fp,"%s\t",);
	}
	else fprintf(fp,"----\t");


	//lineno
	fprintf(fp,"%d\t",root->lineno);


	//tokenName
	if(root->s.tag == 0)//terminal
		fprintf(fp,"%s\t",mapping[root->s.symbol.T].str);
	else //non terminal
		fprintf(fp,"%s\t",mapping[root->s.symbol.NT+63].str);


	//valueIfNumber
	if(root->value.tag == 0) //num
		fprintf(fp,"%s\t",root->value.value.num);		
	else if(root->value.tag == 1) //rnum
		fprintf(fp,"%s\t",root->value.value.rnum);
	else //undefined
		fprintf(fp,"----\t");


	//parentNodeSymbol
	if(root->parent == NULL) 
		fprintf(fp,"ROOT\t");
	else 
		fprintf(fp,"%s\t",mapping[root->parent->s.symbol.NT+63].str);


	//isLeafNode
	if(root->leftChild == NULL) 
		fprintf(fp,"yes\t");
	else
		fprintf(fp,"no\t");


	//nodeSymbol
	if(root->leftChild == NULL) 
		fprintf(fp,"----\t");
	else
		fprintf(fp,"%s\t",mapping[root->s.symbol.NT+63].str);


	fprintf(fp,"\n");
}

void inorder(struct ParseTreeNode * root, FILE * fp){
	if(root == NULL) return;
	inorder(root->leftChild,fp);
	printNode(root,fp);
	if(root->leftChild == NULL) return;
	struct ParseTreeNode * temp = root->leftChild->rightSibling;
	while(temp != NULL){
		inorder(temp,fp);
		temp = temp->rightSibling;
	}
}


void printFullParseTree(struct ParseTreeNode * root, char * outfile){
FILE * fp = fopen(outfile,"w");
inorder(root,outfile);
fp.close();
}

#endif