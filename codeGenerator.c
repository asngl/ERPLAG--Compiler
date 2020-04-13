void assertNodeType(enum NodeType a,enum NodeType b)
{
	if(a!=b)
		printf("\n\tASSERTION ERROR, Expected %d Found %d \n",a,b);
}
int LABEL_COUNTER;
void initLabelGenerator()
{
	LABEL_COUNTER=0;
}
int createLabel()// 37  _label37
{
	LABEL_COUNTER++;
	return LABEL_COUNTER;
}
void generateInputCode(struct ASTNode *root)
{
    VariableEntry *ptr=root->localTableEntry;
    
    if(ptr->type.arrayFlag==1)
    {
        if(ptr->type.type==DT_BOOLEAN)
        {
        	int width=1;
            fprintf(fp,"		mov rax,qword [rbp+%d]\n",ptr->offset);
            fprintf(fp,"		mov rdx,word [rbp+%d]\n",ptr->offset+8);
            fprintf(fp,"		mov rcx,word [rbp+%d]\n",ptr->offset+10);
            
            fprintf(fp,"        mov rsi,rcx");
			fprintf(fp,"        sub rsi,rdx");
			fprintf(fp,"        inc rsi");
			fprintf(fp,"        push    rax");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        push    rsi");
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        mov     rdi,_formatBooleanArray");
			fprintf(fp,"        ;mov     rsi,rsi");
			fprintf(fp,"        ;mov     rdx,rdx");
			fprintf(fp,"        ;mov     rcx,rcx");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    printf");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        pop     rsi");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rax");
			fprintf(fp,"        mov     rbp,rax");
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rcx,rsi");
			int label=createLabel();
			fprintf(fp,"_label%d:",label);
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        mov     rdi,_formatBooleanInput");
			fprintf(fp,"        lea     rsi,[rbp+rbx*%d]",width);
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    scanf");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        inc     rbx");
			fprintf(fp,"        dec     rcx");
			fprintf(fp,"        jnz     _label%d",label);
        }
        else if(ptr->type.type == DT_INTEGER)
        {
            int width=2;
            fprintf(fp,"		mov rax,qword [rbp+%d]\n",ptr->offset);
            fprintf(fp,"		mov rdx,word [rbp+%d]\n",ptr->offset+8);
            fprintf(fp,"		mov rcx,word [rbp+%d]\n",ptr->offset+10);
            
            fprintf(fp,"        mov rsi,rcx");
			fprintf(fp,"        sub rsi,rdx");
			fprintf(fp,"        inc rsi");
			fprintf(fp,"        push    rax");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        push    rsi");
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        mov     rdi,_formatIntArray");
			fprintf(fp,"        ;mov     rsi,rsi");
			fprintf(fp,"        ;mov     rdx,rdx");
			fprintf(fp,"        ;mov     rcx,rcx");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    printf");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        pop     rsi");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rax");
			fprintf(fp,"        mov     rbp,rax");
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rcx,rsi");
			int label=createLabel();
			fprintf(fp,"_label%d:",label);
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        mov     rdi,_formatIntInput");
			fprintf(fp,"        lea     rsi,[rbp+rbx*%d]",width);
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    scanf");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        inc     rbx");
			fprintf(fp,"        dec     rcx");
			fprintf(fp,"        jnz     _label%d",label);
        }
        else
        {
        	///TODO
            int width=4;
            fprintf(fp,"		mov rax,qword [rbp+%d]\n",ptr->offset);
            fprintf(fp,"		mov rdx,word [rbp+%d]\n",ptr->offset+8);
            fprintf(fp,"		mov rcx,word [rbp+%d]\n",ptr->offset+10);
            
            fprintf(fp,"        mov rsi,rcx");
			fprintf(fp,"        sub rsi,rdx");
			fprintf(fp,"        inc rsi");
			fprintf(fp,"        push    rax");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        push    rsi");
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        mov     rdi,_formatRealArray");
			fprintf(fp,"        ;mov     rsi,rsi");
			fprintf(fp,"        ;mov     rdx,rdx");
			fprintf(fp,"        ;mov     rcx,rcx");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    printf");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        pop     rsi");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rax");
			fprintf(fp,"        mov     rbp,rax");
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rcx,rsi");
			int label=createLabel();
			fprintf(fp,"_label%d:",label);
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        mov     rdi,_formatRealInput");
			fprintf(fp,"        lea     rsi,[rbp+rbx*%d]",width);
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    scanf");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        inc     rbx");
			fprintf(fp,"        dec     rcx");
			fprintf(fp,"        jnz     _label%d",label);
        }
    }
    else
    {
        if(ptr->type.type==DT_BOOLEAN)
        {
        	int width=1;
            fprintf(fp,"		movsx rsi,byte [rbp+%d]\n",ptr->offset);
			fprintf(fp,"        push    rax");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        push    rsi");
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        mov     rdi,_formatBooleanSingle");
			fprintf(fp,"        ;mov     rsi,rsi");
			fprintf(fp,"        ;mov     rdx,rdx");
			fprintf(fp,"        ;mov     rcx,rcx");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    printf");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        pop     rsi");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rax");
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rcx,rsi");
			int label=createLabel();
			fprintf(fp,"_label%d:",label);
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        mov     rdi,_formatBooleanInput");
			fprintf(fp,"        lea     rsi,[rbp+rbx*%d+%d]",width,ptr->offset);
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    scanf");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        inc     rbx");
			fprintf(fp,"        dec     rcx");
			fprintf(fp,"        jnz     _label%d",label);
        }
        else if(ptr->type.type == DT_INTEGER)
        {
            int width=2;
            fprintf(fp,"		movsx rsi,word [rbp+%d]\n",ptr->offset);

			fprintf(fp,"        push    rax");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        push    rsi");
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        mov     rdi,_formatIntSingle");
			fprintf(fp,"        ;mov     rsi,rsi");
			fprintf(fp,"        ;mov     rdx,rdx");
			fprintf(fp,"        ;mov     rcx,rcx");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    printf");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        pop     rsi");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rax");
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rcx,rsi");
			int label=createLabel();
			fprintf(fp,"_label%d:",label);
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        mov     rdi,_formatIntInput");
			fprintf(fp,"        lea     rsi,[rbp+rbx*%d+%d]",width,ptr->offset);
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    scanf");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        inc     rbx");
			fprintf(fp,"        dec     rcx");
			fprintf(fp,"        jnz     _label%d",label);
        }
        else
        {
        	///TODO
            int width=4;
            fprintf(fp,"		movsx rsi,dword [rbp+%d]\n",ptr->offset);
            
			fprintf(fp,"        push    rax");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        push    rsi");
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        mov     rdi,_formatRealSingle");
			fprintf(fp,"        ;mov     rsi,rsi");
			fprintf(fp,"        ;mov     rdx,rdx");
			fprintf(fp,"        ;mov     rcx,rcx");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    printf");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        pop     rsi");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rax");
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rcx,rsi");
			int label=createLabel();
			fprintf(fp,"_label%d:",label);
			fprintf(fp,"        push    rbp");
			fprintf(fp,"        push    rbx");
			fprintf(fp,"        push    rcx");
			fprintf(fp,"        mov     rdi,_formatRealInput");
			fprintf(fp,"        lea     rsi,[rbp+rbx*%d+%d]",width,ptr->offset);
			fprintf(fp,"        mov     rbx,0");
			fprintf(fp,"        mov     rax,0");
			fprintf(fp,"        call    scanf");
			fprintf(fp,"        pop     rcx");
			fprintf(fp,"        pop     rbx");
			fprintf(fp,"        pop     rbp");
			fprintf(fp,"        inc     rbx");
			fprintf(fp,"        dec     rcx");
			fprintf(fp,"        jnz     _label%d",label);
        }
    }
}
void generateScopeCode(LabelGenerator *lg,struct ASTNode *root)
{
	while(root!=NULL)
	{
		case INPUT_NODE:
			generateInputCode(root);
			root=root->node.inputNode.next;
			return;
		case OUTPUT_NODE:
			generateOutputCode(root);
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