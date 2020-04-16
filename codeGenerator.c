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

enum Register{RAX,RBX,RCX,RDX,RSI,RDI};
char* regMap={"rax","rbx","rcx","rdx","rsi","rdi"};
int ARRAY_POINTER_WIDTH=8;
int BOOLEAN_WIDTH=8;
int INT_WIDTH=16;
int FLOAT_WIDTH=32;

void getValue(enum Register reg,struct ASTNode *root)
{
	assertNodeType(root->tag,ID_NODE);
	VariableEntry *varptr=root->localTableEntry;
	if(varptr->isParameter==1)
	{
		fprintf("		mov 	%s,qword [rbp+16+%d]\n",regMap[reg],varptr->offset);
	}
	else
	{
		fprintf("		mov 	%s,qword [rbp-8-%d]\n",regMap[reg],varptr->offset);
	}
}
void getAddress(enum Register reg,struct ASTNode *root)
{
	assertNodeType(root->tag,ID_NODE);
	VariableEntry *varptr=root->localTableEntry;
	if(varptr->isParameter==1)
	{
		fprintf("		lea 	%s,qword [rbp+16+%d\n]",regMap[reg],varptr->offset);
	}
	else
	{
		fprintf("		lea 	%s,qword [rbp-8-%d\n]",regMap[reg],varptr->offset);
	}
}
void getLow(enum Register reg,struct ASTNode *root)
{
	assertNodeType(root->tag,ID_NODE);
	VariableEntry *varptr=root->localTableEntry;
	VariableEntry *ptr;
	if(varptr->isParameter==1)
	{
		fprintf("		mov 	%s,qword [rbp+16+%d\n]",regMap[reg],varptr->offset+ARRAY_POINTER_WIDTH);
	}
	else
	{
		if(varptr->type.tagLow==0)
			fprintf("		mov 	%s,%d\n",regMap[reg],varptr->type.low.bound);
		else
		{
			ptr=root->node.idNode.Range->node.rangeNode.Range1->localTableEntry;
			getValue(reg,ptr);
			fprintf("		mov 	%s,qword [rbp-8-%s]\n",regMap[reg],regMap[reg]);
		}
	}
}
void getHigh(enum Register reg,struct ASTNode* root)
{
	assertNodeType(root->tag,ID_NODE);
	VariableEntry *varptr=root->localTableEntry;
	VariableEntry *ptr;
	if(varptr->isParameter==1)
	{
		fprintf("		mov 	%s,qword [rbp+16+%d]\n",regMap[reg],varptr->offset+ARRAY_POINTER_WIDTH+INT_WIDTH);
	}
	else
	{
		if(varptr->type.tagLow==0)
			fprintf("		mov 	%s,%d\n",regMap[reg],varptr->type.high.bound);
		else
		{
			ptr=root->node.idNode.Range->node.rangeNode.Range1->localTableEntry;
			getValue(reg,ptr);
			fprintf("		mov 	%s,qword [rbp-8-%s]\n",regMap[reg],regMap[reg]);
		}
	}
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
        fprintf(fp,"_label%d",label);
        fprintf(fp,"mov     rdi,_formatBooleanFalse");
        fprintf(fp,"_label%d:",label2);
        fprintf(fp,"mov     rax,0");
        fprintf(fp,"call    printf");
    }else
    if(root->tag==NUM_NODE){
        fprintf(fp,"mov    rdi, _formatIntOutput");
        fprintf(fp,"mov    rsi,%d",root->node.numNode.num);
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
                    fprintf(fp,"mov    rbx,0");
                    fprintf(fp,"mov    rcx,[rbp-8-%d-8]",ptr->offset);
                    fprintf(fp,"mov    rbx,[rbp-8-%d-16]",ptr->offset);
                    fprintf(fp,"mov    rsi,[rbp-8-%d]",ptr->offset);
                    int label,label1,label2;
                    label=createLabel();
                    label1=createLabel();
                    label2=createLabel();
                    fprintf(fp,"_label%d:",label);
                    fprintf(fp,"push    rbp");
                    fprintf(fp,"push    rax");
                    fprintf(fp,"push    rcx");
                    fprintf(fp,"push    rbx");
                    fprintf(fp,"push    rsi");
                    fprintf(fp,"movsx   rax,byte [rsi-rbx*1]");    //Change???????
                    fprintf(fp,"cmp    rax,0");
                    fprintf(fp,"jz _label%d",label1);
                    fprintf(fp,"mov    rdi,_formatBooleanTrue");
                    fprintf(fp,"jmp    _label%d",label2);
                    fprintf(fp,"_label%d",label1);
                    fprintf(fp,"mov    rdi_formatBooleanFalse");
                    fprintf(fp,"_label%d:",label2);
                    fprintf(fp,"mov    rax,0");
                    fprintf(fp,"call    printf");
                    fprintf(fp,"pop    rsi");
                    fprintf(fp,"pop    rbx");
                    fprintf(fp,"pop    rcx");
                    fprintf(fp,"pop    rax");
                    fprintf(fp,"pop    rbp");
                    fprintf(fp,"inc rbx");
                    fprintf(fp,"inc rcx");
                    fprintf(fp,"cmp    rcx,rbx");
                    fprintf(fp,"jnz    _label%d",label);
                }else
                if(ptr->type.type==DT_INTEGER){
                    fprintf(fp,"mov    rax,0");
                    fprintf(fp,"mov    rcx,[rbp-8-%d-8]",ptr->offset);
                    fprintf(fp,"mov    rbx,[rbp-8-%d-16]",ptr->offset);
                    fprintf(fp,"mov    r8,[rbp-8-%d]",ptr->offset);
                    int label;
                    label=createLabel();
                    fprintf(fp,"_label%d:",label);
                    fprintf(fp,"push    rbp");
                    fprintf(fp,"push    rax");
                    fprintf(fp,"push    rcx");
                    fprintf(fp,"push    rbx");
                    fprintf(fp,"push    r8");
                    fprintf(fp,"mov    rdi, _formatIntOutput");
                    fprintf(fp,"movsx    rsi,word [r8-rax*2]");    //Change????
                    fprintf(fp,"mov    rax,0");
                    fprintf("call printf");
                    fprintf(fp,"pop    r8");
                    fprintf(fp,"pop    rbx");
                    fprintf(fp,"pop    rcx");
                    fprintf(fp,"pop    rax");
                    fprintf(fp,"pop    rbp");
                    fprintf(fp,"inc rax");
                    fprintf(fp,"inc rcx");
                    fprintf(fp,"cmp    rcx,rbx");
                    fprintf(fp,"jnz    _label%d",label);
                }else
                {
                    fprintf(fp,"mov    rax,0");
                    fprintf(fp,"mov    rcx,[rbp-8-%d-8]",ptr->offset);
                    fprintf(fp,"mov    rbx,[rbp-8-%d-16]",ptr->offset);
                    fprintf(fp,"mov    r8,[rbp-8-%d]",ptr->offset);
                    int label;
                    label=createLabel();
                    fprintf(fp,"_label%d:",label);
                    fprintf(fp,"push    rbp");
                    fprintf(fp,"push    rax");
                    fprintf(fp,"push    rcx");
                    fprintf(fp,"push    rbx");
                    fprintf(fp,"push    r8");
                    fprintf(fp,"mov    rdi, _formatRealOutput");
                    fprintf(fp,"movss   xmm0,qword [r8-rax*4]");    //Change????
                    fprintf(fp,"mov    rax,1");
                    fprintf("call printf");
                    fprintf(fp,"pop    r8");
                    fprintf(fp,"pop    rbx");
                    fprintf(fp,"pop    rcx");
                    fprintf(fp,"pop    rax");
                    fprintf(fp,"pop    rbp");
                    fprintf(fp,"inc rax");
                    fprintf(fp,"inc rcx");
                    fprintf(fp,"cmp    rcx,rbx");
                    fprintf(fp,"jnz    _label%d",label);
                }
            }else
            if(root->node.idNode.index->tag==NUM_NODE){
                if(ptr->type.type==DT_BOOLEAN){
                    fprintf(fp,"mov    rdx,[rbp-8-%d-8]",ptr->offset);
                    fprintf(fp,"mov    rcx,[rbp-8-%d-16]",ptr->offset);
                    fprintf(fp,"mov    r8,[rbp-8-%d]",ptr->offset);
                    fprintf(fp,"mov    rsi,%d",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"cmp    rdx,%d",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"jg    _arrayerrorlabel_");
                    fprintf(fp,"cmp    rcx,%d",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"jl    _arrayerrorlabel_");
                    fprintf(fp,"sub    rsi,rdx");
                    fprintf(fp,"movsx   rax,byte [r8-rsi]");
                    int label,label2;
                    label=createLabel();
                    label2=createLabel();
                    fprintf(fp,"cmp rax,0");
                    fprintf(fp,"jz _label%d",label);
                    fprintf(fp,"mov     rdi,_formatBooleanTrue");
                    fprintf(fp,"jmp     _label%d",label2);
                    fprintf(fp,"_label%d",label);
                    fprintf(fp,"mov     rdi,_formatBooleanFalse");
                    fprintf(fp,"_label%d:",label2);
                    fprintf(fp,"mov     rax,0");
                    fprintf(fp,"call    printf");
                }else
                if(ptr->type.type==DT_INTEGER){
                    fprintf(fp,"mov    rdx,[rbp-8-%d-8]",ptr->offset);
                    fprintf(fp,"mov    rcx,[rbp-8-%d-16]",ptr->offset);
                    fprintf(fp,"mov    r8,[rbp-8-%d]",ptr->offset);
                    fprintf(fp,"mov    rsi,%d",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"cmp    rdx,%d",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"jg    _arrayerrorlabel_");
                    fprintf(fp,"cmp    rcx,%d",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"jl    _arrayerrorlabel_");
                    fprintf(fp,"mov    rdi, _formatIntOutput");
                    fprintf(fp,"sub    rsi,rdx");
                    fprintf(fp,"movsx   rsi,word [r8-rsi*2]");
                    fprintf(fp,"mov    rax,0");
                    fprintf("call printf");
                }else
                {
                    fprintf(fp,"mov    rdx,[rbp-8-%d-8]",ptr->offset);
                    fprintf(fp,"mov    rcx,[rbp-8-%d-16]",ptr->offset);
                    fprintf(fp,"mov    r8,[rbp-8-%d]",ptr->offset);
                    fprintf(fp,"mov    rsi,%d",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"cmp    rdx,%d",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"jg    _arrayerrorlabel_");
                    fprintf(fp,"cmp    rcx,%d",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"jl    _arrayerrorlabel_");
                    fprintf(fp,"mov    rdi, _formatRealOutput");
                    fprintf(fp,"sub    rsi,rdx");
                    fprintf(fp,"movss   xmm0,qword [r8-rsi*4]");
                    fprintf(fp,"mov    rax,1");
                    fprintf("call printf");
                }
            }else
            {
                VariableEntry *indexptr;
                indexptr=root->node.idNode.index->localTableEntry;
                if (ptr->type.type==DT_BOOLEAN)
                {
                    fprintf(fp,"mov    rdx,[rbp-8-%d-8]",ptr->offset);
                    fprintf(fp,"mov    rcx,[rbp-8-%d-16]",ptr->offset);
                    fprintf(fp,"mov    r8,[rbp-8-%d]",ptr->offset);
                    fprintf(fp,"mov    rsi,word [rbp-8-%d]",indexptr->offset);
                    fprintf(fp,"cmp    rdx,rsi");
                    fprintf(fp,"jg    _arrayerrorlabel_");
                    fprintf(fp,"cmp    rcx,rsi");
                    fprintf(fp,"jl    _arrayerrorlabel_");
                    fprintf(fp,"sub    rsi,rdx");
                    fprintf(fp,"movsx   rax,byte [r8-rsi]");
                    int label,label2;
                    label=createLabel();
                    label2=createLabel();
                    fprintf(fp,"cmp rax,0");
                    fprintf(fp,"jz _label%d",label);
                    fprintf(fp,"mov     rdi,_formatBooleanTrue");
                    fprintf(fp,"jmp     _label%d",label2);
                    fprintf(fp,"_label%d",label);
                    fprintf(fp,"mov     rdi,_formatBooleanFalse");
                    fprintf(fp,"_label%d:",label2);
                    fprintf(fp,"mov     rax,0");
                    fprintf(fp,"call    printf");
                }else
                if (ptr->type.type == DT_INTEGER)
                {
                    fprintf(fp,"mov    rdx,[rbp-8-%d-8]",ptr->offset);
                    fprintf(fp,"mov    rcx,[rbp-8-%d-16]",ptr->offset);
                    fprintf(fp,"mov    r8,[rbp-8-%d]",ptr->offset);
                    fprintf(fp,"mov    rsi,word [rbp-8-%d]",indexptr->offset);
                    fprintf(fp,"cmp    rdx,rsi");
                    fprintf(fp,"jg    _arrayerrorlabel_");
                    fprintf(fp,"cmp    rcx,rsi");
                    fprintf(fp,"jl    _arrayerrorlabel_");
                    fprintf(fp,"mov    rdi, _formatIntOutput");
                    fprintf(fp,"sub    rsi,rdx");
                    fprintf(fp,"movsx   rsi,word [r8-rsi*2]");
                    fprintf(fp,"mov    rax,0");
                    fprintf("call printf");
                }else{
                    fprintf(fp,"mov    rdx,[rbp-8-%d-8]",ptr->offset);
                    fprintf(fp,"mov    rcx,[rbp-8-%d-16]",ptr->offset);
                    fprintf(fp,"mov    r8,[rbp-8-%d]",ptr->offset);
                    fprintf(fp,"mov    rsi,word [rbp-8-%d]",indexptr->offset);
                    fprintf(fp,"cmp    rdx,rsi");
                    fprintf(fp,"jg    _arrayerrorlabel_");
                    fprintf(fp,"cmp    rcx,rsi");
                    fprintf(fp,"jl    _arrayerrorlabel_");
                    fprintf(fp,"mov    rdi, _formatRealOutput");
                    fprintf(fp,"sub    rsi,rdx");
                    fprintf(fp,"movss   xmm0,qword [r8-rsi*4]");
                    fprintf(fp,"mov    rax,1");
                    fprintf("call printf");
                }
            }
        }else
        {
            if(ptr->type.type==DT_BOOLEAN){
                fprintf(fp,"movsx   rax,byte [rbp-8-%d]",ptr->offset);
                int label,label2;
                label=createLabel();
                label2=createLabel();
                fprintf(fp,"cmp rax,0");
                fprintf(fp,"jz _label%d",label);
                fprintf(fp,"mov     rdi,_formatBooleanTrue");
                fprintf(fp,"jmp     _label%d",label2);
                fprintf(fp,"_label%d",label);
                fprintf(fp,"mov     rdi,_formatBooleanFalse");
                fprintf(fp,"_label%d:",label2);
                fprintf(fp,"mov     rax,0");
                fprintf(fp,"call    printf");
            }else
            if(ptr->type.type==DT_INTEGER){
                fprintf(fp,"mov    rdi, _formatIntOutput");
                fprintf(fp,"movsx    rsi,word [rbp-8-%d]",ptr->offset);
                fprintf(fp,"mov    rax,0");
                fprintf("call printf");
            }else
            {
                fprintf(fp,"mov    rdi,_formatRealOutput");
                fprintf(fp,"movss    xmm0,qword [rbp-8-%d]",ptr->offset);
                fprintf(fp,"mov rax,1");
                fprintf(fp,"call printf");
            }
        }
    }
}

void getValueRAX(VariableEntry *varptr)
{

}
void generateAssignmentCode(struct ASTNode* root)
{
	VariableEntry *varptr;
	varptr=root->localTableEntry;
	if(varptr->type.arrayFlag==1)
	{
		getValueRAX(varptr);
		getValueRAX(varptr);
		
	}
}

void generateScopeCode(struct ASTNode *root)//Must maintain RBP and RDX
{
    VariableEntry *varptr;
    struct ASTNode *currVar;
    int allocatedSpace;
    int label1,label2,label3;
    int num1,num2;
    fprintf(fp,"	push 	rdx\n");
    fprintf(fp,"	mov 	rdx,0\n");
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
	        	varptr=root->localTableEntry;
	        	num1=root->node.forNode.Range->node.rangeNode.Range1->node.numNode.num;
	        	num2=root->node.forNode.Range->node.rangeNode.Range2->node.numNode.num;
	            fprintf(fp,"    mov     rcx,%d\n",num1);
	            label1=createLabel();
				fprintf(fp,"_label%d:\n",label1);
				fprintf(fp,"    push    rcx\n");
				fprintf(fp,"    push    rbx\n");
				fprintf(fp,"    mov     qword [rbp-8-%d],rcx\n",varptr->offset);
				generateScopeCode(root->node.forNode.stmts);
				fprintf(fp,"    pop     rbx\n");
				fprintf(fp,"    pop     rcx\n");
				fprintf(fp,"    inc     rcx\n");
				fprintf(fp,"    cmp     rcx,%d\n",num2+1);
				fprintf(fp,"    jnz     _label%d\n",label1);
				root=root->node.forNode.next;
	            break;
	        case WHILE_NODE:
	        	label1=createLabel();
	        	label2=createLabel();
	        	fprintf(fp,"_label%d:\n",label1);
				
				generateExpressionCode(0,root->node.whileNode.expr);   // This will generate output of a Expression and store it in rax or xmm0
				
				fprintf(fp,"    movsx   rax,byte [_booltmp0_]\n");
				fprintf(fp,"    cmp     rax,0\n");
				fprintf(fp,"    jz      _label%d\n",label2);
				
				generateScopeCode(root->node.whileNode.stmts);
				
				fprintf(fp,"    jmp     _label%d\n",label1);
				fprintf(fp,"_label%d:\n",label2);
	            root=root->node.whileNode.next;
	            break;
	        case DECLARE_NODE:
	        	currVar=root->node.declareNode.idList;
	        	varptr=currVar->localTableEntry;
	        	if(root->node.declareNode.Range!=NULL)
	        	{
	        		if(varptr->type.tagLow==0)
	        		{
	        			fprintf(fp,"mov 	rsi,%d\n",varptr->type.low.bound);
	        		}
	        		else
	        		{
	        			fprintf(fp,"mov 	rsi,[rbp-8-%d]\n",varptr->offset);
	        		}
	        		if(varptr->type.tagHigh==0)
	        		{
	        			fprintf(fp,"mov 	rdi,%d\n",varptr->type.high.bound);
	        		}
	        		else
	        		{
	        			fprintf(fp,"mov 	rsi,[rbp-8-%d]\n",varptr->offset);
	        		}

	        		fprintf(fp,"mov 	rcx,rdi\n");
	        		fprintf(fp,"sub		rcx,rsi\n");
	        		fprintf(fp,"inc 	rcx\n");
	        		fprintf(fp,"mul     rcx,8\n");
	        	}
	        	while(currVar!=NULL)
	        	{
	        		varptr=currVar->localTableEntry;
	        		if(varptr->type.isArrayFlag==1)
	        		{
	        			if(varptr->type.isStatic==1)
	        			{
	        				fprintf(fp,"lea 	rax,[ebp-8-(varptr->offset+24)]\n");
	        				fprintf(fp,"mov 	qword [ebp-8-varptr->offset],rax\n");
	        			}
	        			else
	        			{
	        				fprintf(fp,"sub		rsp,rcx\n");
	        				fprintf(fp,"add		rdx,rcx\n");

	        				fprintf(fp,"mov 	rax,rsp\n");
	        				fprintf(fp,"add		rax,8\n");
	        				fprintf(fp,"mov 	qword [ebp-8-varptr->offset],rax\n");
	        			}
	        			fprintf(fp,"mov 	qword [ebp-8-(varptr->offset+8)],rsi\n");
	        			fprintf(fp,"mov 	qword [ebp-8-(varptr->offset+16)],rdi\n");
	        		}
	        		currVar=currVar->next;
	        	}
	            root=root->node.declareNode.next;
	            break;
	        default:break;
	    }
    }
    fprintf(fp, "add 	rsp,rdx\n");
    fprintf(fp, "pop 	rdx\n");
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
void generateModuleCode(struct ASTNode *root)
{
	assert(MODULE_NODE,root->tag);
	//int activationRecordSize=?;
	//activationRecordSize+=16-(activationRecordSize%16);
	fprintf(fp,"%s:\n",root->node.moduleNode.moduleName);
	fprintf(fp,"        push    rbp\n");
	fprintf(fp,"        mov     rbp,rsp\n");
	fprintf(fp,"        sub 	rsp,%d\n",activationRecordSize);
	generateScopeCode(root->body);
	fprintf(fp,"        mov     rsp,rbp\n");
	fprintf(fp,"        pop     rbp\n");
	fprintf(fp,"	ret\n\n");
	return;
}

void generateProgramCode(struct ASTNode *root,char *filename)
{
	struct ASTNode *ASTptr;

	assertNodeType(PROGRAM_NODE,root->tag);
	fp=fopen(filename,"w+");
	initLabelGenerator();
	TEMPORARY_COUNTER=0;
	printStartingCode();

	Code finalCode=generateModuleCode(root->node.programNode.driverModule);
	
	Code temp=newEmptyCode();

	ASTptr=root->node.programNode.otherModules1;

	while(ASTptr!=NULL)
	{
		generateModuleCode(ASTptr);
		ASTptr=ASTptr->node.moduleNode.next;
	}

	ASTptr=root->node.programNode.otherModules2;

	while(ASTptr!=NULL)
	{
		generateModuleCode(lg,ASTptr);
		ASTptr=ASTptr->node.moduleNode.next;
	}
	generateErrorHandlingCode();
	generateDataCode();
	return;
}