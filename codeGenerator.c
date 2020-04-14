void assertNodeType(enum NodeType a,enum NodeType b)
{
	if(a!=b)
		printf("\n\tASSERTION ERROR, Expected %d Found %d \n",a,b);
}
int LABEL_COUNTER;
int TEMPORARY_COUNTER;
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
            int width=8;
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
            int width=8;
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

void generateOutputCode(struct ASTNode *root){
    if(root->tag==BOOL_NODE){
        fprintf(fp,"mov   rax,%d",root->node.boolNode.value);
        int label,label2;
        label=createLabel();
        label2=createLabel();
            
        fprintf(fp,"cmp rax,0");
        fprintf(fp,"jz _label%d",label);
        fprintf(fp,"mov     rdi,_formatBooleanTrue");
        fprintf(fp,"jmp     _label%d",label2);
        fprintf(fp,"_label%d",label1);
        fprintf(fp,"mov     rdi,_formatBooleanFalse");
        fprintf(fp,"_label%d:",label2);
        fprintf(fp,"mov     rax,0");

        

        fprintf(fp,"call    printf");
    }else
    if(root->tag==NUM_NODE){
        frpintf(fp,"mov    rdi, _formatIntOutput");
        fprintf(fp,"mov    rsi,%d,root->node.numNode.num",root->node.numNode.num);
        fprintf(fp,"mov    rax,0");
        fprintf("call printf");
    }else
    if(root->tag==RNUM_NODE){
        fprintf(fp,"mov    rdi,_formatRealOutput");
        fprintf(fp,"movss    xmm0,%f",root->node.rNumNode.rnum);
        fprintf(fp,"mov rax,1");
        fprintf(fp,"call printf");
    }else
    {
        VariableEntry *ptr = root->localTableEntry;
        if(ptr->type.arrayFlag==1){
            if(root->node.idNode.index==NULL){
                if(ptr->type.type==DT_BOOLEAN){
                    
                }else
                if(ptr->type.type==DT_INTEGER){
                    
                }else
                {
                    
                }
            }else
            if(root->node.idNode.index==NUM_NODE){
                if(ptr->type.type==DT_BOOLEAN){
                    
                }else
                if(ptr->type.type==DT_INTEGER){
                    
                }else
                {
                    
                }
            }else
            {
                if(ptr->type.type==DT_BOOLEAN){
                    
                }else
                if(ptr->type.type==DT_INTEGER){
                    
                }else
                {
                    
                }
            }
        }
    }
}

void generateScopeCode(LabelGenerator *lg,struct ASTNode *root)
{
    VariableEntry *varptr;
    struct ASTNode *currVar;
    int allocatedSpace;
    fprintf(fp, "mov 	rdx,0");
    while(root!=NULL)
    {
    	switch(root->tag)
    	{
	        case INPUT_NODE:
	            generateInputCode(root);
	            root=root->node.inputNode.next;
	            break;
	        case OUTPUT_NODE:
	            generateOutputCode(root->node.outputNode.value);
	            root=root->node.outputNode.next;
	            break;
	        case ASSIGN_NODE:
	        	generateAssignmentCode(root);
	            root=root->node.assignNode.next;
	            break;
	        case MODULE_REUSE_NODE:
	            root=root->node.moduleReuseNode.next;
	            break;
	        case CONDITION_NODE:
	            root=root->node.conditionNode.next;
	            break;
	        case CASE_NODE:
	            root=root->node.caseNode.next;
	            break;
	        case FOR_NODE:
	            root=root->node.forNode.next;
	            break;
	        case WHILE_NODE:
	            root=root->node.whileNode.next;
	            break;
	        case DECLARE_NODE:
	        	currVar=root->node.declareNode.idList;
	        	varptr=currVar->localTableEntry;
	        	if(root->node.declareNode.Range!=NULL)
	        	{
	        		if(varptr->type.tagLow==0)
	        		{
	        			mov 	rsi,varptr->type.low.bound
	        		}
	        		else
	        		{
	        			mov 	rsi,
	        		}
	        		if(varptr->type.tagHigh==0)
	        		{
	        			mov 	rdi,varptr->type.high.bound
	        		}
	        		else
	        		{
	        			
	        		}
	        		mov 	rsi,start
	        		mov 	rdi,end

	        		mov 	rcx,rdi
	        		sub		rcx,rsi
	        		inc 	rcx
	        		mul     rcx,8
	        	}
	        	while(currVar!=NULL)
	        	{
	        		varptr=currVar->localTableEntry;
	        		if(varptr->type.isArrayFlag==1)
	        		{
	        			if(varptr->type.isStatic==1)
	        			{
	        				lea 	rax,[ebp-8-(varptr->offset+24)]
	        				mov 	qword [ebp-8-varptr->offset],rax
	        			}
	        			else
	        			{
	        				sub		rsp,rcx
	        				add		rdx,rcx

	        				mov 	rax,rsp
	        				add		rax,8
	        				mov 	qword [ebp-8-varptr->offset],rax
	        			}
	        			mov 	qword [ebp-8-(varptr->offset+8)],rsi
	        			mov 	qword [ebp-8-(varptr->offset+16)],rdi
	        		}
	        		currVar=currVar->next;
	        	}
	            root=root->node.declareNode.next;
	            break;
	        default:break;
	    }
    }
    fprintf(fp, "add 	rsp,rdx");
    return;
}
void printStartingCode()
{
	fprintf(fp,"global  main ;nasm -felf64 sample.asm && gcc -no-pie sample.o && ./a.out\n");
	fprintf(fp,"        extern  printf\n");
	fprintf(fp,"        extern  scanf\n");
	fprintf(fp,"        SECTION .text\n");
	fprintf(fp,"main:\n");
	fprintf(fp,"        push    rbp\n");
	fprintf(fp,"        mov     rbp,rsp\n");
	fprintf(fp,"        and     rsp,0xfffffff0\n");
	fprintf(fp,"\n");
	fprintf(fp,"        ;PUSH PARAMETERS NOW\n");
	fprintf(fp,"\n");
	fprintf(fp,"        call    driver\n");
	fprintf(fp,"\n");
	fprintf(fp,"        ;POP PARAMETERS NOW\n");
	fprintf(fp,"        \n");
	fprintf(fp,"        ;Fill output parameters while poppping\n");
	fprintf(fp,"\n");
	fprintf(fp,"        mov     rsp,rbp\n");
	fprintf(fp,"        pop     rbp\n");
	fprintf(fp,"        \n");
	fprintf(fp,"        ret\n");
}
void  generateErrorHandlingCode()
{
	fprintf(fp,"_arrayerrorlabel_:\n");
	fprintf(fp,"        and     rsp,0xfffffff0\n");
	fprintf(fp,"        mov     rdi,_errorString\n");
	fprintf(fp,"        mov     rax,0\n");
	fprintf(fp,"        call    printf\n");
	fprintf(fp,"        mov     rax,1\n");
	fprintf(fp,"        ret\n");
	fprintf(fp,"\n");
	fprintf(fp,"_errorString:\n");
	fprintf(fp,"        db  \"Array Index Out of Bounds \"\n");
	fprintf(fp,"_errorArray:\n");
	fprintf(fp,"        db  \"                    \",10\n");
	fprintf(fp,"_errorString2:\n");
	fprintf(fp,"        db  \" Index %d was out of bounds %d and %d\",10,0\n");
fprintf(fp,"\n");
}
void generateDataCode()
{
	// printTemporaries
	fprintf(fp,"_formatIntArray:\n");
	fprintf(fp,"        db  \"Enter %hd numbers for integer array from %hi to %hi \", 10, 0\n");
	fprintf(fp,"\n");
	fprintf(fp,"_formatBooleanArray:\n");
	fprintf(fp,"        db  \"Enter %hi numbers for boolean array from %hi to %hi \", 10, 0\n");
	fprintf(fp,"\n");
	fprintf(fp,"_formatRealArray:\n");
	fprintf(fp,"        db  \"Enter %hi numbers for real array from %hi to %hi \", 10, 0\n");
	fprintf(fp,"\n");
	fprintf(fp,"_formatIntInput:\n");
	fprintf(fp,"        db  \"%d\",0\n");
	fprintf(fp,"\n");
	fprintf(fp,"_formatIntOutput:\n");
	fprintf(fp,"        db  \"%d\",10,0\n");
	fprintf(fp,"\n");
	fprintf(fp,"_formatBooleanInput:\n");
	fprintf(fp,"        db  \"%d\",0\n");
	fprintf(fp,"\n");
	fprintf(fp,"_formatBooleanOutput:\n");
	fprintf(fp,"        db  \"%s\",10,0\n");
	fprintf(fp,"\n");
	fprintf(fp,"_formatRealInput:\n");
	fprintf(fp,"        db  \"%lf\",0\n");
	fprintf(fp,"\n");
	fprintf(fp,"_formatRealOutput:\n");
	fprintf(fp,"        db  \"%lf\",10,\n");
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

void generateProgramCode(struct ASTNode *root,char *filename)
{
	struct ASTNode *ASTptr;

	assertNodeType(PROGRAM_NODE,root->tag);
	fp=fopen(filename,"w+");
	initLabelGenerator();
	TEMPORARY_COUNTER=0;
	printStartingCode();

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
	generateErrorHandlingCode();
	generateDataCode();
	return;
}