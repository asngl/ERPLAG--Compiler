void assertNodeType(enum NodeType a,enum NodeType b)
{
	if(a!=b)
		printf("\n\tASSERTION ERROR, Expected %d Found %d \n",a,b);
}


void generateScopeCode(LabelGenerator *lg,struct ASTNode *root)
{
	Code finalCode=newEmptyCode();
	Code temp1,temp2,temp3;
	Code endOfScopeCode=newEmptyCode();

	while(root!=NULL)
	{
		case INPUT_NODE:
			root=root->node.inputNode.next;
			return;
		case OUTPUT_NODE:
			root=root->node.outputNode.next;
			return;
		case ASSIGN_NODE:
			root=root->node.assignNode.next;
			return;
		case MODULE_REUSE_NODE:
			root=root->node.moduleReuseNode.next;
			return;
		case CONDITION_NODE:
			root=root->node.conditionNode.next;
			return;
		case CASE_NODE:
			root=root->node.caseNode.next;
			return;
		case FOR_NODE:
			root=root->node.forNode.next;
			return;
		case WHILE_NODE:
			root=root->node.whileNode.next;
			return;
		case DECLARE_NODE:
			root=root->node.declareNode.next;
			return;
	}
	finalCode=mergeCode(finalCode,endOfScopeCode);
	return finalCode;
}

void generateModuleCode(LabelGenerator *lg,struct ASTNode *root)
{
	assert(MODULE_NODE,root->tag);
	Code finalCode=newEmptyCode();
	Code temp;

	temp=newCode(newCustomLabelLine(root->node.moduleNode.moduleName));
	finalCode=mergeCode(finalCode,temp);

	temp=generateScopeCode(lg,root->body);
	finalCode=mergeCode(finalCode,temp);

	return finalCode;
}

void generateProgramCode(struct ASTNode *root)
{
	struct ASTNode *ASTptr;

	assertNodeType(PROGRAM_NODE,root->tag);

	initLabelGenerator();

	Code finalCode=generateModuleCode(lg,root->node.programNode.driverModule);
	
	Code temp=newEmptyCode();

	ASTptr=root->node.programNode.otherModules1;

	while(ASTptr!=NULL)
	{
		temp=generateModuleCode(lg,ASTptr);
		finalCode=mergeCode(finalCode,temp);
		ASTptr=ASTptr->node.moduleNode.next;
	}

	ASTptr=root->node.programNode.otherModules2;

	while(ASTptr!=NULL)
	{
		temp=generateModuleCode(lg,ASTptr);
		finalCode=mergeCode(finalCode,temp);
		ASTptr=ASTptr->node.moduleNode.next;
	}
	return finalCode;
}