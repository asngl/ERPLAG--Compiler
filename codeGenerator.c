#define MAX_NUM_FLOAT_CONSTANTS 1000
void assertNodeType(enum NodeType a,enum NodeType b)
{
	if(a!=b)
		printf("\n\tASSERTION ERROR, Expected %d Found %d \n",a,b);
}
int LABEL_COUNTER;
int TEMPORARY_COUNTER;
int NUM_FLOAT_CONSTANTS;
double floatConstants[MAX_NUM_FLOAT_CONSTANTS];
void initLabelGenerator()
{
	LABEL_COUNTER=0;
}
int createLabel()// 37  _label37
{
	LABEL_COUNTER++;
	return LABEL_COUNTER;
}
int createFloatConstant(double t)
{
	floatConstants[NUM_FLOAT_CONSTANTS]=t;
	return NUM_FLOAT_CONSTANTS++;
}

enum Register{RAX,RBX,RCX,RDX,RSI,RDI,R8};
char* regMap[]={"rax","rbx","rcx","rdx","rsi","rdi","r8"};
int ARRAY_POINTER_WIDTH=8;
int BOOLEAN_WIDTH=8;
int INT_WIDTH=16;
int FLOAT_WIDTH=32;
enum FloatRegister{XMM0,XMM1,XMM2,XMM3};
char *floatRegMap[]={"xmm0","xmm1","xmm2","xmm3"};

void getFloatValue(enum FloatRegister reg,struct ASTNode *root)
{
    VariableEntry *varptr=root->localTableEntry;
    if(varptr->isParameter==1)
    {
        fprintf(fp,"        movsd     %s,qword [rbp+16+%d]\n",floatRegMap[reg],varptr->offset);
    }
    else
    {
        fprintf(fp,"        movsd     %s,qword [rbp-8-%d]\n",floatRegMap[reg],varptr->offset);
    }
}

void setFloatValue(enum FloatRegister reg,struct ASTNode *root)
{
    VariableEntry *varptr=root->localTableEntry;
    if(varptr->isParameter==1)
    {
        fprintf(fp,"        movsd     qword [rbp+16+%d],%s\n",varptr->offset,floatRegMap[reg]);
    }
    else
    {
        fprintf(fp,"        movsd     qword [rbp-8-%d],%s\n",varptr->offset,floatRegMap[reg]);
    }
}

void PUSH(enum Register reg)
{
    fprintf(fp,"        push %s\n",regMap[reg]);
}
void POP(enum Register reg)
{
    fprintf(fp,"        pop %s\n",regMap[reg]);
}
void CMP(enum Register reg1,enum Register reg2)
{
    fprintf(fp,"        cmp %s,%s\n",regMap[reg1],regMap[reg2]);
}
void SUB(enum Register reg1,enum Register reg2)
{
    fprintf(fp,"        sub %s,%s\n",regMap[reg1],regMap[reg2]);
}
void getValue(enum Register reg,struct ASTNode *root)
{
    //assertNodeType(root->tag,ID_NODE);
    VariableEntry *varptr=root->localTableEntry;
    if(varptr->isParameter==1)
    {
        fprintf(fp,"        mov     %s,qword [rbp+16+%d]\n",regMap[reg],varptr->offset);
    }
    else
    {
        fprintf(fp,"        mov     %s,qword [rbp-8-%d]\n",regMap[reg],varptr->offset);
    }
}
void getIndex(enum Register reg,struct ASTNode *root)
{
    //assertNodeType(root->tag,ID_NODE);
    VariableEntry *varptr=root->localTableEntry;
    if(root->tag==NUM_NODE)
    {
        fprintf(fp,"        mov     %s,%d\n",regMap[reg],root->node.numNode.num);
    }
    else// ID_NODE
    {
        getValue(RAX,root);
    }
}
void setValue(enum Register reg,struct ASTNode *root)
{
    //assertNodeType(root->tag,ID_NODE);
    VariableEntry *varptr=root->localTableEntry;
    if(varptr->isParameter==1)
    {
        fprintf(fp,"        mov     qword [rbp+16+%d], %s\n",varptr->offset,regMap[reg]);
    }
    else
    {
        fprintf(fp,"        mov     qword [rbp-8-%d], %s\n",varptr->offset,regMap[reg]);
    }
}
void getAddress(enum Register reg,struct ASTNode *root)
{
    //assertNodeType(root->tag,ID_NODE);
    VariableEntry *varptr=root->localTableEntry;
    if(varptr->isParameter==1)
    {
        fprintf(fp,"        lea     %s,qword [rbp+16+%d]\n",regMap[reg],varptr->offset);
    }
    else
    {
        fprintf(fp,"        lea     %s,qword [rbp-8-%d]\n",regMap[reg],varptr->offset);
    }
}
void getLow(enum Register reg,struct ASTNode *root)
{
    //assertNodeType(root->tag,ID_NODE);
    VariableEntry *varptr=root->localTableEntry;
    VariableEntry *ptr;
    if(varptr->isParameter==1)
    {
        fprintf(fp,"       mov     %s,qword [rbp+16+%d]\n",regMap[reg],varptr->offset+ARRAY_POINTER_WIDTH);
    }
    else
    {
        if(varptr->type.tagLow==0)
            fprintf(fp,"       mov     %s,%d\n",regMap[reg],varptr->type.low.bound);
        else
        {
            ptr=varptr->type.lowPtr;
            getValue(reg,ptr);
            fprintf(fp,"       mov     %s,qword [rbp-8-%s]\n",regMap[reg],regMap[reg]);
        }
    }
}
void getHigh(enum Register reg,struct ASTNode* root)
{
    //assertNodeType(root->tag,ID_NODE);
    VariableEntry *varptr=root->localTableEntry;
    VariableEntry *ptr;
    if(varptr->isParameter==1)
    {
        fprintf(fp,"        mov     %s,qword [rbp+16+%d]\n",regMap[reg],varptr->offset+ARRAY_POINTER_WIDTH+2*INT_WIDTH);
    }
    else
    {
        if(varptr->type.tagLow==0)
            fprintf(fp,"        mov     %s,%d\n",regMap[reg],varptr->type.high.bound);
        else
        {
            ptr=varptr->type.highPtr;
            getValue(reg,ptr);
            fprintf(fp,"        mov     %s,qword [rbp-8-%s]\n",regMap[reg],regMap[reg]);
        }
    }
}

void generateInputCode(struct ASTNode *root)
{
    VariableEntry *ptr=root->localTableEntry;
    
    if(ptr->type.arrayFlag==1)
    {
        getValue(RAX,root);
        getLow(RDX,root);
        getHigh(RCX,root);
        
        fprintf(fp,"        mov rsi,rcx\n");
        fprintf(fp,"        sub rsi,rdx\n");
        fprintf(fp,"        inc rsi\n");
        
        if(ptr->type.type==DT_BOOLEAN)
        {
            
            fprintf(fp,"        mov     rdi,_formatBooleanArray\n");
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    printf\n");
            fprintf(fp,"        mov     rbp,rax\n");
            fprintf(fp,"        mov     rbx,0\n");
            fprintf(fp,"        mov     rcx,rsi\n");
            int label=createLabel();
            fprintf(fp,"_label%d:\n",label);
            fprintf(fp,"        push    rbp\n");
            fprintf(fp,"        push    rbx\n");
            fprintf(fp,"        push    rcx\n");
            fprintf(fp,"        mov     rdi,_formatBooleanInput\n");
            fprintf(fp,"        lea     rsi,qword [rbp-rbx*%d]\n",BOOLEAN_WIDTH);
            fprintf(fp,"        mov     rbx,0\n");
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    scanf\n");
            fprintf(fp,"        pop     rcx\n");
            fprintf(fp,"        pop     rbx\n");
            fprintf(fp,"        pop     rbp\n");
            fprintf(fp,"        inc     rbx\n");
            fprintf(fp,"        dec     rcx\n");
            fprintf(fp,"        jnz     _label%d\n",label);
        }
        else if(ptr->type.type == DT_INTEGER)
        {
            fprintf(fp,"        mov     rdi,_formatIntArray\n");
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    printf\n");
            fprintf(fp,"        mov     rbp,rax\n");
            fprintf(fp,"        mov     rbx,0\n");
            fprintf(fp,"        mov     rcx,rsi\n");
            int label=createLabel();
            fprintf(fp,"_label%d:\n",label);
            fprintf(fp,"        push    rbp\n");
            fprintf(fp,"        push    rbx\n");
            fprintf(fp,"        push    rcx\n");
            fprintf(fp,"        mov     rdi,_formatIntInput\n");
            fprintf(fp,"        lea     rsi,qword [rbp-rbx*%d]\n",INT_WIDTH);
            fprintf(fp,"        mov     rbx,0\n");
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    scanf\n");
            fprintf(fp,"        pop     rcx\n");
            fprintf(fp,"        pop     rbx\n");
            fprintf(fp,"        pop     rbp\n");
            fprintf(fp,"        inc     rbx\n");
            fprintf(fp,"        dec     rcx\n");
            fprintf(fp,"        jnz     _label%d\n",label);
        }
        else
        {
            fprintf(fp,"        mov     rdi,_formatRealArray\n");
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    printf\n");
            fprintf(fp,"        mov     rbp,rax\n");
            fprintf(fp,"        mov     rbx,0\n");
            fprintf(fp,"        mov     rcx,rsi\n");
            int label=createLabel();
            fprintf(fp,"_label%d:\n",label);
            fprintf(fp,"        push    rbp\n");
            fprintf(fp,"        push    rbx\n");
            fprintf(fp,"        push    rcx\n");
            fprintf(fp,"        mov     rdi,_formatRealInput\n");
            fprintf(fp,"        lea     rsi,qword [rbp-rbx*%d]\n",REAL_WIDTH);
            fprintf(fp,"        mov     rbx,0\n");
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    scanf\n");
            fprintf(fp,"        pop     rcx\n");
            fprintf(fp,"        pop     rbx\n");
            fprintf(fp,"        pop     rbp\n");
            fprintf(fp,"        inc     rbx\n");
            fprintf(fp,"        dec     rcx\n");
            fprintf(fp,"        jnz     _label%d\n",label);
        }
    }
    else
    {
        if(ptr->type.type==DT_BOOLEAN)
        {

            fprintf(fp,"        mov     rdi,_formatBooleanSingle\n");
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    printf\n");

            fprintf(fp,"        mov     rdi,_formatBooleanInput\n");
            getAddress(RSI,root);
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    scanf\n");

        }
        else if(ptr->type.type == DT_INTEGER)
        {
            fprintf(fp,"        mov     rdi,_formatIntSingle\n");
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    printf\n");

            fprintf(fp,"        mov     rdi,_formatIntInput\n");
            getAddress(RSI,root);
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    scanf\n");
        }
        else
        {
            fprintf(fp,"        mov     rdi,_formatRealSingle\n");
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    printf\n");

            fprintf(fp,"        mov     rdi,_formatRealInput\n");
            getAddress(RSI,root);
            fprintf(fp,"        mov     rax,0\n");
            fprintf(fp,"        call    scanf\n");
        }
    }
}
void generateOutputCode(struct ASTNode *root){
    if(root->tag==BOOL_NODE){
        fprintf(fp,"        mov   rax,%d\n",root->node.boolNode.value);
        int label,label2;
        label=createLabel();
        label2=createLabel();
            
        fprintf(fp,"        cmp    rax,0\n");
        fprintf(fp,"        jz    _label%d\n",label);
        fprintf(fp,"        mov    rdi,_formatBooleanTrue\n");
        fprintf(fp,"        jmp    _label%d\n",label2);
        fprintf(fp,"_label%d:\n",label);
        fprintf(fp,"        mov    rdi,_formatBooleanFalse\n");
        fprintf(fp,"_label%d:\n",label2);
        fprintf(fp,"        mov     rax,0\n");
        fprintf(fp,"        call    printf\n");
    }else
    if(root->tag==NUM_NODE){
        fprintf(fp,"        mov    rdi, _formatIntOutput\n");
        fprintf(fp,"        mov    rsi,%d\n",root->node.numNode.num);
        fprintf(fp,"        mov    rax,0\n");
        fprintf(fp,"        call    printf\n");
    }else
    if(root->tag==RNUM_NODE){
        fprintf(fp,"        mov    rdi,_formatRealOutput\n");
        int flt1=createFloatConstant(root->node.rNumNode.rnum);
        fprintf(fp,"        movsd    xmm0,qword [_flt%d]\n",flt1);
        fprintf(fp,"        mov    rax,1\n");
        fprintf(fp,"        push    rbp\n");
        fprintf(fp,"        mov     rbp,rsp\n");
        fprintf(fp,"        and     rsp,0xFFFFFFFFFFFFFFF0\n");
        fprintf(fp,"        call    printf\n");
        fprintf(fp,"        mov     rsp,rbp\n");
        fprintf(fp,"        pop     rbp\n");
    }else
    {
        VariableEntry *ptr = root->localTableEntry;
        if(ptr->type.arrayFlag==1){
            if(root->node.idNode.index==NULL){
                if(ptr->type.type==DT_BOOLEAN){
                    fprintf(fp,"        mov    rbx,0\n");
                    getLow(RCX,root);
                    getHigh(R8,root);
                    getValue(RSI,root);
                    int label,label1,label2;
                    label=createLabel();
                    label1=createLabel();
                    label2=createLabel();
                    fprintf(fp,"_label%d:\n",label);
                    fprintf(fp,"        push    rbp\n");
                    fprintf(fp,"        push    rax\n");
                    fprintf(fp,"        push    rcx\n");
                    fprintf(fp,"        push    rbx\n");
                    fprintf(fp,"        push    rsi\n");
                    fprintf(fp,"        mov    rax,qword [rsi-rbx*%d]\n",BOOLEAN_WIDTH);    
                    fprintf(fp,"        cmp    rax,0\n");
                    fprintf(fp,"        jz    _label%d\n",label1);
                    fprintf(fp,"        mov    rdi,_formatBooleanTrue\n");
                    fprintf(fp,"        jmp    _label%d\n",label2);
                    fprintf(fp,"_label%d:\n",label1);
                    fprintf(fp,"        mov    rdi_formatBooleanFalse\n");
                    fprintf(fp,"_label%d:\n",label2);
                    fprintf(fp,"        mov    rax,0\n");
                    fprintf(fp,"        call    printf\n");
                    fprintf(fp,"        pop    rsi\n");
                    fprintf(fp,"        pop    rbx\n");
                    fprintf(fp,"        pop    rcx\n");
                    fprintf(fp,"        pop    rax\n");
                    fprintf(fp,"        pop    rbp\n");
                    fprintf(fp,"        inc    rbx\n");
                    fprintf(fp,"        inc    rcx\n");
                    fprintf(fp,"        cmp    rcx,r8\n");
                    fprintf(fp,"        jle    _label%d\n",label);
                }else
                if(ptr->type.type==DT_INTEGER){
                    fprintf(fp,"        mov    rax,0\n");
                    getLow(RCX,root);
                    getHigh(RBX,root);
                    getValue(R8,root);
                    int label;
                    label=createLabel();
                    fprintf(fp,"_label%d:\n",label);
                    fprintf(fp,"        push    rbp\n");
                    fprintf(fp,"        push    rax\n");
                    fprintf(fp,"        push    rcx\n");
                    fprintf(fp,"        push    rbx\n");
                    fprintf(fp,"        push    r8\n");
                    fprintf(fp,"        mov    rdi, _formatIntOutput\n");
                    fprintf(fp,"        mov    rsi,qword [r8-rax*%d]\n",INT_WIDTH);    //Change????
                    fprintf(fp,"        mov    rax,0\n");
                    fprintf(fp,"        call    printf\n");
                    fprintf(fp,"        pop    r8\n");
                    fprintf(fp,"        pop    rbx\n");
                    fprintf(fp,"        pop    rcx\n");
                    fprintf(fp,"        pop    rax\n");
                    fprintf(fp,"        pop    rbp\n");
                    fprintf(fp,"        inc    rax\n");
                    fprintf(fp,"        inc    rcx\n");
                    fprintf(fp,"        cmp    rcx,rbx\n");
                    fprintf(fp,"        jle    _label%d\n",label);
                }else
                {
                    fprintf(fp,"        mov    rax,0\n");
                    getLow(RCX,root);
                    getHigh(RBX,root);
                    getValue(R8,root);
                    int label;
                    label=createLabel();
                    fprintf(fp,"_label%d:\n",label);
                    fprintf(fp,"        push    rbp\n");
                    fprintf(fp,"        push    rax\n");
                    fprintf(fp,"        push    rcx\n");
                    fprintf(fp,"        push    rbx\n");
                    fprintf(fp,"        push    r8\n");
                    fprintf(fp,"        mov    rdi, _formatRealOutput\n");
                    fprintf(fp,"        movsd    xmm0,qword [r8-rax*%d]\n",FLOAT_WIDTH);    
                    fprintf(fp,"        mov    rax,1\n");
                    fprintf(fp,"        push    rbp\n");
                    fprintf(fp,"        mov     rbp,rsp\n");
                    fprintf(fp,"        and     rsp,0xFFFFFFFFFFFFFFF0\n");
                    fprintf(fp,"        call    printf\n");
                    fprintf(fp,"        mov     rsp,rbp\n");
                    fprintf(fp,"        pop     rbp\n");
                    fprintf(fp,"        pop    r8\n");
                    fprintf(fp,"        pop    rbx\n");
                    fprintf(fp,"        pop    rcx\n");
                    fprintf(fp,"        pop    rax\n");
                    fprintf(fp,"        pop    rbp\n");
                    fprintf(fp,"        inc    rax\n");
                    fprintf(fp,"        inc    rcx\n");
                    fprintf(fp,"        cmp    rcx,rbx\n");
                    fprintf(fp,"        jle    _label%d\n",label);
                }
            }else
            if(root->node.idNode.index->tag==NUM_NODE){
                if(ptr->type.type==DT_BOOLEAN){
                    getLow(RDX,root);
                    getHigh(RCX,root);
                    getValue(R8,root);
                    fprintf(fp,"        mov    rsi,%d\n",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"        cmp    rdx,%d\n",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"        jg    _arrayerrorlabel_\n");
                    fprintf(fp,"        cmp    rcx,%d\n",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"        jl    _arrayerrorlabel_\n");
                    fprintf(fp,"        sub    rsi,rdx\n");
                    fprintf(fp,"        mov   rax,qword [r8-rsi*%d]\n",BOOLEAN_WIDTH);
                    int label,label2;
                    label=createLabel();
                    label2=createLabel();
                    fprintf(fp,"        cmp rax,0\n");
                    fprintf(fp,"        jz _label%d\n",label);
                    fprintf(fp,"        mov     rdi,_formatBooleanTrue\n");
                    fprintf(fp,"        jmp     _label%d\n",label2);
                    fprintf(fp,"_label%d:\n",label);
                    fprintf(fp,"        mov     rdi,_formatBooleanFalse\n");
                    fprintf(fp,"_label%d:\n",label2);
                    fprintf(fp,"        mov     rax,0\n");
                    fprintf(fp,"        call    printf\n");
                }else
                if(ptr->type.type==DT_INTEGER){
                    getLow(RDX,root);
                    getHigh(RCX,root);
                    getValue(R8,root);
                    fprintf(fp,"        mov    rsi,%d\n",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"        cmp    rdx,%d\n",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"        jg    _arrayerrorlabel_\n");
                    fprintf(fp,"        cmp    rcx,%d\n",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"        jl    _arrayerrorlabel_\n");
                    fprintf(fp,"        mov    rdi, _formatIntOutput\n");
                    fprintf(fp,"        sub    rsi,rdx\n");
                    fprintf(fp,"        mov   rsi,qword [r8-rsi*%d]\n",INT_WIDTH);
                    fprintf(fp,"        mov    rax,0\n");
                    fprintf(fp,"        call    printf\n");
                }else
                {
                    getLow(RDX,root);
                    getHigh(RCX,root);
                    getValue(R8,root);
                    fprintf(fp,"        mov    rsi,%d\n",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"        cmp    rdx,%d\n",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"        jg    _arrayerrorlabel_\n");
                    fprintf(fp,"        cmp    rcx,%d\n",root->node.idNode.index->node.numNode.num);
                    fprintf(fp,"        jl    _arrayerrorlabel_\n");
                    fprintf(fp,"        mov    rdi, _formatRealOutput\n");
                    fprintf(fp,"        sub    rsi,rdx\n");
                    fprintf(fp,"        movsd   xmm0,qword [r8-rsi*%d]\n",FLOAT_WIDTH);
                    fprintf(fp,"        mov    rax,1\n");
                    fprintf(fp,"        push    rbp\n");
                    fprintf(fp,"        mov     rbp,rsp\n");
                    fprintf(fp,"        and     rsp,0xFFFFFFFFFFFFFFF0\n");
                    fprintf(fp,"        call    printf\n");
                    fprintf(fp,"        mov     rsp,rbp\n");
                    fprintf(fp,"        pop     rbp\n");
                }
            }else
            {
                VariableEntry *indexptr;
                indexptr=root->node.idNode.index->localTableEntry;
                if (ptr->type.type==DT_BOOLEAN)
                {
                    getLow(RDX,root);
                    getHigh(RCX,root);
                    getValue(R8,root);
                    getValue(RSI,root->node.idNode.index);
                    fprintf(fp,"        cmp    rdx,rsi\n");
                    fprintf(fp,"        jg    _arrayerrorlabel_\n");
                    fprintf(fp,"        cmp    rcx,rsi\n");
                    fprintf(fp,"        jl    _arrayerrorlabel_\n");
                    fprintf(fp,"        sub    rsi,rdx\n");
                    fprintf(fp,"        mov   rax,qword [r8-rsi*%d]\n",BOOLEAN_WIDTH);
                    int label,label2;
                    label=createLabel();
                    label2=createLabel();
                    fprintf(fp,"        cmp    rax,0\n");
                    fprintf(fp,"        jz    _label%d\n",label);
                    fprintf(fp,"        mov     rdi,_formatBooleanTrue\n");
                    fprintf(fp,"        jmp     _label%d\n",label2);
                    fprintf(fp,"_label%d:\n",label);
                    fprintf(fp,"        mov     rdi,_formatBooleanFalse\n");
                    fprintf(fp,"        _label%d:\n",label2);
                    fprintf(fp,"        mov     rax,0\n");
                    fprintf(fp,"        call    printf\n");
                }else
                if (ptr->type.type == DT_INTEGER)
                {
                    getLow(RDX,root);
                    getHigh(RCX,root);
                    getValue(R8,root);
                    getValue(RSI,root->node.idNode.index);
                    fprintf(fp,"        cmp    rdx,rsi\n");
                    fprintf(fp,"        jg    _arrayerrorlabel_\n");
                    fprintf(fp,"        cmp    rcx,rsi\n");
                    fprintf(fp,"        jl    _arrayerrorlabel_\n");
                    fprintf(fp,"        mov    rdi, _formatIntOutput\n");
                    fprintf(fp,"        sub    rsi,rdx\n");
                    fprintf(fp,"        mov   rsi,qword [r8-rsi*%d]\n",INT_WIDTH);
                    fprintf(fp,"        mov    rax,0\n");
                    fprintf(fp,"        call    printf\n");
                }else{
                    getLow(RDX,root);
                    getHigh(RCX,root);
                    getValue(R8,root);
                    getValue(RSI,root->node.idNode.index);
                    fprintf(fp,"        cmp    rdx,rsi\n");
                    fprintf(fp,"        jg    _arrayerrorlabel_\n");
                    fprintf(fp,"        cmp    rcx,rsi\n");
                    fprintf(fp,"        jl    _arrayerrorlabel_\n");
                    fprintf(fp,"        mov    rdi, _formatRealOutput\n");
                    fprintf(fp,"        sub    rsi,rdx\n");
                    fprintf(fp,"        movsd   xmm0,qword [r8-rsi*%d]\n",FLOAT_WIDTH);
                    fprintf(fp,"        mov    rax,1\n");
                    fprintf(fp,"        push    rbp\n");
                    fprintf(fp,"        mov     rbp,rsp\n");
                    fprintf(fp,"        and     rsp,0xFFFFFFFFFFFFFFF0\n");
                    fprintf(fp,"        call    printf\n");
                    fprintf(fp,"        mov     rsp,rbp\n");
                    fprintf(fp,"        pop     rbp\n");
                }
            }
        }else
        {
            if(ptr->type.type==DT_BOOLEAN){
                getValue(RAX,root);
                int label,label2;
                label=createLabel();
                label2=createLabel();
                fprintf(fp,"        cmp    rax,0\n");
                fprintf(fp,"        jz    _label%d\n",label);
                fprintf(fp,"        mov     rdi,_formatBooleanTrue\n");
                fprintf(fp,"        jmp     _label%d\n",label2);
                fprintf(fp,"_label%d:\n",label);
                fprintf(fp,"        mov     rdi,_formatBooleanFalse\n");
                fprintf(fp,"_label%d:\n",label2);
                fprintf(fp,"        mov     rax,0\n");
                fprintf(fp,"        call    printf\n");
            }else
            if(ptr->type.type==DT_INTEGER){
                fprintf(fp,"        mov    rdi, _formatIntOutput\n");
                getValue(RSI,root);
                fprintf(fp,"        mov    rax,0\n");
                fprintf(fp,"        call    printf\n");
            }else
            {
                fprintf(fp,"        mov    rdi,_formatRealOutput\n");
                getFloatValue(XMM0,root);
                fprintf(fp,"        mov    rax,1\n");
                fprintf(fp,"        push    rbp\n");
                fprintf(fp,"        mov     rbp,rsp\n");
                fprintf(fp,"        and     rsp,0xFFFFFFFFFFFFFFF0\n");
                fprintf(fp,"        call    printf\n");
                fprintf(fp,"        mov     rsp,rbp\n");
                fprintf(fp,"        pop     rbp\n");
            }
        }
    }
}
void generateConditionCode(struct AST * root){ 
    VariableEntry *ptr;
    ptr=root->localTableEntry;
    struct ASTNode * cases = root->node.conditionNode.Case;
    int endlabel = createLabel(); 
    if(root->node.conditionNode.presentDefault == 0){ //BOOLEAN 
        while(cases != NULL){   
            int labe11 = createLabel();
            int label2 = createLabel();
            fprintf(fp,"        push    rbp\n");
            fprintf(fp,"        push    rax\n");
            getValue(RAX,root);
            fprintf(fp,"        cmp    rax,%d\n",cases->node.caseNode.value->node.boolNode.value);
            fprintf(fp,"        jz    _label%d\n",label1);
            fprintf(fp,"        pop    rax\n");
            fprintf(fp,"        pop    rbp\n");
            fprintf(fp,"        jump    _label%d\n",label2);
            fprintf(fp,"        _label%d:\n",label1);
            generateScopeCode(cases->node.caseNode.stmt);
            fprintf(fp,"        jump    _label%d\n",endlabel);
            fprintf(fp,"        _label%d:\n",label2);
            cases = cases->node.caseNode.next;
        }
    }
    
    else{   //INTEGER
        int defaultlabel =  createLabel();
        while(cases != NULL){   
            int labe11 = createLabel();
            int label2 = createLabel();
            fprintf(fp,"        push    rbp\n");
            fprintf(fp,"        push    rax\n");
            fprintf(fp,"        cmp    rax,%d\n",cases->node.caseNode.value->node.numNode.num);
            fprintf(fp,"        jz    _label%d\n",label1);
            fprintf(fp,"        push    rbp\n");
            fprintf(fp,"        push    rax\n");
            fprintf(fp,"        jump    _label%d\n",label2);
            fprintf(fp,"        _label%d:\n",label1);
            generateScopeCode(cases->node.caseNode.stmt);
            fprintf(fp,"        jump    _label%d\n",endlabel);
            fprintf(fp,"        _label%d:\n",label2);
            cases = cases->node.caseNode.next;                  
        }
        
        fprintf(fp,"        _label%d:\n",defaultlabel);
        generateScopeCode(cases->node.caseNode.default);
        fprintf(fp,"        jump    _label%d\n",endlabel);
    }
    fprintf(fp,"        _label%d:\n",endlabel);
}


void generateExpressionCode(int depth,struct ASTNode *root)// Stores result in temporary number depth _inttmp0
{
    if(depth>=TEMPORARY_COUNTER)
    {
        TEMPORARY_COUNTER=depth;
    }
    int flt1,flt2,flt3;
    Type type,type2;
    int label;
    switch(root->tag)
    {
        case BOOL_NODE:
            fprintf(fp,"        mov qword [_booltmp%d],%d\n",depth,root->node.boolNode.value);
            break;
        case NUM_NODE:
            fprintf(fp,"        mov qword [_inttmp%d],%d\n",depth,root->node.numNode.num);
            break;
        case RNUM_NODE:
            int flt1=createFloatConstant(root->node.rnumNode.rnum);
            fprintf(fp,"        movsd   xmm0,qword [_flt%d]\n",flt1);
            fprintf(fp,"        movsd qword [_flttmp%d],xmm0\n",depth);
            break;
        case ID_NODE:
            if(root->node.idNode.index==NULL){
                switch(root->localTableEntry->type.type)
                {
                    case DT_INTEGER:
                        getValue(RAX,root);
                        fprintf(fp,"        mov qword [_inttmp%d],rax\n",depth);
                        break;
                    case DT_REAL:
                        getFloatValue(XMM0,root);
                        fprintf(fp,"        movsd qword [_flttmp%d],xmm0\n",depth);
                        break;
                    case DT_BOOLEAN:
                        getValue(RAX,root);
                        fprintf(fp,"        mov qword [_booltmp%d],rax\n",depth);
                        break;
                }
            }else{
                getLow(RSI,root);
                getHigh(RDI,root);
                getIndex(RAX,root->node.idNode.index);
                CMP(RAX,RSI);
                fprintf(fp,"        jl      _indexerrorlabel_\n");
                CMP(RDI,RAX);
                fprintf(fp,"        jg      _indexerrorlabel_\n");
                SUB(RAX,RSI);
                getValue(RSI,root);
                switch(root->localTableEntry->type.type){
                    case DT_INTEGER:
                        fprintf(fp,"        mov    rax,qword [rsi-rax*16]\n");
                        fprintf(fp,"        mov    qword [_inttmp%d],rax\n",depth);
                        break;
                    case DT_REAL:
                        fprintf(fp,"        movsd    xmm0,qword [rsi-rax*32]\n");
                        fprintf(fp,"        movsd qword [_flttmp%d],xmm0\n",depth);
                        break;
                    case DT_BOOLEAN:
                        fprintf(fp,"        mov    rax,qword [rsi-rax*8]\n");
                        fprintf(fp,"        mov qword [_booltmp%d],rax\n",depth);
                        break;
                }            
            }
            break;
        case UNARY_NODE:
            generateExpressionCode(depth+1,root->node.unaryNode.expr);
            if(root->node.unaryNode.type==DT_INTEGER)
            {
                fprintf(fp,"        mov rax,-1\n");
                fprintf(fp,"        mov rbx,qword [_inttmp%d]\n",depth+1);
                fprintf(fp,"        mul rax,rbx\n");
                fprintf(fp,"        mov [_inttmp%d],rax\n",depth);
            }
            else
            {
                fprintf(fp,"        movsd xmm0,qword [_flt0]\n");
                fprintf(fp,"        movsd xmm1,qword [_flttmp%d]\n",depth+1);
                fprintf(fp,"        mulsd xmm0,xmm1\n");
                fprintf(fp,"        movsd qword [_flttmp%d],xmm0\n",depth);
            }
            break;
        case BINARY_NODE:
            generateExpressionCode(depth+1,root->node.binaryNode.expr1);
            generateExpressionCode(depth+2,root->node.binaryNode.expr2);
            switch(root->node.binaryNode.op) //enum Operator{OP_PLUS,OP_MINUS,OP_MUL,OP_DIV,OP_AND,OP_OR,OP_LT, OP_LE, OP_GE, OP_GT, OP_EQ, OP_NE};
            {
                case OP_PLUS: 
                    if(root->node.binaryNode.childType==DT_INTEGER){
                        fprintf(fp,"        mov rax,qword [_inttmp%d]\n",depth+1);
                        fprintf(fp,"        mov rbx,qword [_inttmp%d]\n",depth+2);
                        fprintf(fp,"        add rax,rbx\n");
                        fprintf(fp,"        mov [_inttmp%d],rax\n",depth);                        
                        
                    }else{
                        fprintf(fp,"        movsd xmm0,qword [_flttmp%d]\n",depth+1);
                        fprintf(fp,"        movsd xmm1,qword [_flttmp%d]\n",depth+2);
                        fprintf(fp,"        addsd xmm0,xmm1\n");
                        fprintf(fp,"        movsd qword [_flttmp%d],xmm0\n",depth);
                    }
                    break;
                case OP_MINUS:
                    if(root->node.binaryNode.childType==DT_INTEGER){
                        fprintf(fp,"        mov rax,qword [_inttmp%d]\n",depth+1);
                        fprintf(fp,"        mov rbx,qword [_inttmp%d]\n",depth+2);
                        fprintf(fp,"        sub rax,rbx\n");
                        fprintf(fp,"        mov [_inttmp%d],rax\n",depth);
                    }else{
                        fprintf(fp,"        movsd xmm0,qword [_flttmp%d]\n",depth+1);
                        fprintf(fp,"        movsd xmm1,qword [_flttmp%d]\n",depth+2);
                        fprintf(fp,"        subsd xmm0,xmm1\n");
                        fprintf(fp,"        movsd qword [_flttmp%d],xmm0\n",depth);
                    }
                    break; 
                case OP_MUL:
                    if(root->node.binaryNode.childType==DT_INTEGER){
                        fprintf(fp,"        mov rax,qword [_inttmp%d]\n",depth+1);
                        fprintf(fp,"        mov rbx,qword [_inttmp%d]\n",depth+2);
                        fprintf(fp,"        mul rbx\n");
                        fprintf(fp,"        mov [_inttmp%d],rax\n",depth);
                    }else{
                        fprintf(fp,"        movsd xmm0,qword [_flttmp%d]\n",depth+1);
                        fprintf(fp,"        movsd xmm1,qword [_flttmp%d]\n",depth+2);
                        fprintf(fp,"        mulsd xmm0,xmm1\n");
                        fprintf(fp,"        movsd qword [_flttmp%d],xmm0\n",depth);
                    }
                    break; 
                case OP_DIV:
                    if(root->node.binaryNode.childType==DT_INTEGER){
                        fprintf(fp,"        mov rax,qword [_inttmp%d]\n",depth+1);
                        fprintf(fp,"        mov rbx,qword [_inttmp%d]\n",depth+2);
                        fprintf(fp,"        mov rdx,0\n");
                        fprintf(fp,"        div rbx\n");
                        fprintf(fp,"        mov [_inttmp%d],rax\n",depth);
                    }else{
                        fprintf(fp,"        movsd xmm0,qword [_flttmp%d]\n",depth+1);
                        fprintf(fp,"        movsd xmm1,qword [_flttmp%d]\n",depth+2);
                        fprintf(fp,"        divsd xmm0,xmm1\n");
                        fprintf(fp,"        movsd qword [_flttmp%d],xmm0\n",depth);
                    }
                    break;
                case OP_AND:
                    fprintf(fp,"    mov rax,qword [_booltmp%d]\n",depth+1);
                    fprintf(fp,"    mov rbx,qword [_booltmp%d]\n",depth+2);
                    fprintf(fp,"    and rax,rbx\n");
                    fprintf(fp,"    mov [_booltmp%d],rax\n",depth);
                    break;
                case OP_OR:
                    fprintf(fp,"    mov rax,qword [_booltmp%d]\n",depth+1);
                    fprintf(fp,"    mov rbx,qword [_booltmp%d]\n",depth+2);
                    fprintf(fp,"    or rax,rbx\n");
                    fprintf(fp,"    mov [_booltmp%d],rax\n",depth);
                    break;
                case OP_LT:
                    if(root->node.binaryNode.childType==DT_INTEGER){
                        fprintf(fp,"    mov rax,qword [_inttmp%d]\n",depth+1);
                        fprintf(fp,"    mov rbx,qword [_inttmp%d]\n",depth+2);
                        fprintf(fp,"    mov rcx,0");
                        fprintf(fp,"    cmp rax,rbx");
                        label=createLabel();
                        fprintf(fp,"    jge _label%d",label);
                        fprintf(fp,"    mov rcx,1");
                        fprintff(fp,"    _label%d",label);
                        fprintf(fp,"    mov [_booltmp%d],rcx\n",depth);
                    }else{
                        fprintf(fp,"    movsd xmm0,qword [_flttmp%d]\n",depth+1);
                        fprintf(fp,"    movsd xmm1,qword [_flttmp%d]\n",depth+2);
                        fprintf(fp,"    cmpltsd xmm0,xmm1\n");
                        fprintf(fp,"    movq    rax,xmm0\n");
                        fprintf(fp,"    mov     rbx,-1\n");
                        fprintf(fp,"    mul     rbx\n");
                        fprintf(fp,"    mov [_booltmp%d],rax\n",depth);
                    }
                    break;
                case OP_LE:
                    if(root->node.binaryNode.childType==DT_INTEGER){
                        fprintf(fp,"    mov rax,qword [_inttmp%d]\n",depth+1);
                        fprintf(fp,"    mov rbx,qword [_inttmp%d]\n",depth+2);
                        fprintf(fp,"    mov rcx,0");
                        fprintf(fp,"    cmp rax,rbx");
                        label=createLabel();
                        fprintf(fp,"    jg _label%d",label);
                        fprintf(fp,"    mov rcx,1");
                        fprintff(fp,"    _label%d",label);
                        fprintf(fp,"    mov [_booltmp%d],rcx\n",depth);
                    }else{
                        fprintf(fp,"    movsd xmm0,qword [_flttmp%d]\n",depth+1);
                        fprintf(fp,"    movsd xmm1,qword [_flttmp%d]\n",depth+2);
                        fprintf(fp,"    cmplesd xmm0,xmm1\n");
                        fprintf(fp,"    movq    rax,xmm0\n");
                        fprintf(fp,"    mov     rbx,-1\n");
                        fprintf(fp,"    mul     rbx\n");
                        fprintf(fp,"    mov [_booltmp%d],rax\n",depth);
                    }
                    break;
                case OP_GT:
                    if(root->node.binaryNode.childType==DT_INTEGER){
                        fprintf(fp,"    mov rax,qword [_inttmp%d]\n",depth+1);
                        fprintf(fp,"    mov rbx,qword [_inttmp%d]\n",depth+2);
                        fprintf(fp,"    mov rcx,0");
                        fprintf(fp,"    cmp rax,rbx");
                        label=createLabel();
                        fprintf(fp,"    jle _label%d",label);
                        fprintf(fp,"    mov rcx,1");
                        fprintff(fp,"    _label%d",label);
                        fprintf(fp,"    mov [_booltmp%d],rcx\n",depth);
                    }else{
                        fprintf(fp,"    movsd xmm0,qword [_flttmp%d]\n",depth+1);
                        fprintf(fp,"    movsd xmm1,qword [_flttmp%d]\n",depth+2);
                        fprintf(fp,"    cmpnlesd xmm0,xmm1\n");
                        fprintf(fp,"    movq    rax,xmm0\n");
                        fprintf(fp,"    mov     rbx,-1\n");
                        fprintf(fp,"    mul     rbx\n");
                        fprintf(fp,"    mov [_booltmp%d],rax\n",depth);
                    }
                    break;
                case OP_GE:
                    if(root->node.binaryNode.childType==DT_INTEGER){
                        fprintf(fp,"    mov rax,qword [_inttmp%d]\n",depth+1);
                        fprintf(fp,"    mov rbx,qword [_inttmp%d]\n",depth+2);
                        fprintf(fp,"    mov rcx,0");
                        fprintf(fp,"    cmp rax,rbx");
                        label=createLabel();
                        fprintf(fp,"    jl _label%d",label);
                        fprintf(fp,"    mov rcx,1");
                        fprintff(fp,"    _label%d",label);
                        fprintf(fp,"    mov [_booltmp%d],rcx\n",depth);
                    }else{
                        fprintf(fp,"    movsd xmm0,qword [_flttmp%d]\n",depth+1);
                        fprintf(fp,"    movsd xmm1,qword [_flttmp%d]\n",depth+2);
                        fprintf(fp,"    cmpnltsd xmm0,xmm1\n");
                        fprintf(fp,"    movq    rax,xmm0\n");
                        fprintf(fp,"    mov     rbx,-1\n");
                        fprintf(fp,"    mul     rbx\n");
                        fprintf(fp,"    mov [_booltmp%d],rax\n",depth);
                    }
                    break;                    
                case OP_EQ:
                    if(root->node.binaryNode.childType==DT_INTEGER){
                        fprintf(fp,"    mov rax,qword [_inttmp%d]\n",depth+1);
                        fprintf(fp,"    mov rbx,qword [_inttmp%d]\n",depth+2);
                        fprintf(fp,"    mov rcx,0");
                        fprintf(fp,"    cmp rax,rbx");
                        label=createLabel();
                        fprintf(fp,"    jnz _label%d",label);
                        fprintf(fp,"    mov rcx,1");
                        fprintff(fp,"    _label%d",label);
                        fprintf(fp,"    mov [_booltmp%d],rcx\n",depth);
                    }else{
                        fprintf(fp,"    movsd xmm0,qword [_flttmp%d]\n",depth+1);
                        fprintf(fp,"    movsd xmm1,qword [_flttmp%d]\n",depth+2);
                        fprintf(fp,"    cmpeqsd xmm0,xmm1\n");
                        fprintf(fp,"    movq    rax,xmm0\n");
                        fprintf(fp,"    mov     rbx,-1\n");
                        fprintf(fp,"    mul     rbx\n");
                        fprintf(fp,"    mov [_booltmp%d],rax\n",depth);
                    }
                    break;
                case OP_NE:
                    if(root->node.binaryNode.childType==DT_INTEGER){
                       fprintf(fp,"    mov rax,qword [_inttmp%d]\n",depth+1);
                        fprintf(fp,"    mov rbx,qword [_inttmp%d]\n",depth+2);
                        fprintf(fp,"    mov rcx,0");
                        fprintf(fp,"    cmp rax,rbx");
                        label=createLabel();
                        fprintf(fp,"    jz _label%d",label);
                        fprintf(fp,"    mov rcx,1");
                        fprintff(fp,"    _label%d",label);
                        fprintf(fp,"    mov [_booltmp%d],rcx\n",depth); 
                    }else{
                        fprintf(fp,"    movsd xmm0,qword [_flttmp%d]\n",depth+1);
                        fprintf(fp,"    movsd xmm1,qword [_flttmp%d]\n",depth+2);
                        fprintf(fp,"    cmpnesd xmm0,xmm1\n");
                        fprintf(fp,"    movq    rax,xmm0\n");
                        fprintf(fp,"    mov     rbx,-1\n");
                        fprintf(fp,"    mul     rbx\n");
                        fprintf(fp,"    mov [_booltmp%d],rax\n",depth);
                    }
                    break;
                   
            }
            
            break;
    }
}
void generateAssignmentCode(struct ASTNode* root)
{
    VariableEntry *varptr;
    varptr=root->localTableEntry;
    struct ASTNode *right;
    right=root->node.assignNode.expr;
    VariableEntry *varptr2;
    varptr2=right->localTableEntry;
    if(varptr->type.arrayFlag==1)
    {
        if(root->node.assignNode.index==NULL)
        {
            if(ptr->type.isStatic==0||varptr->type.isStatic==0)
            {
                // Compare low and high for equality
                getLow(RSI,root);
                getLow(RDI,right);
                CMP(RSI,RDI);
                fprintf(fp,"    jnz     _boundserrorlabel_\n");
                getHigh(RSI,root);
                getHigh(RDI,right);
                CMP(RSI,RDI);
                fprintf(fp,"    jnz     _boundserrorlabel_\n");

            }
            getValue(RAX,right);
            setValue(RAX,root);
            return;
        }
        getLow(RSI,root);
        getHigh(RDI,root);
        getIndex(RAX,root->node.assignNode.index);
        CMP(RAX,RSI);
        fprintf(fp,"    jl      _indexerrorlabel_\n");
        CMP(RDI,RAX);
        fprintf(fp,"    jg      _indexerrorlabel_\n");
        SUB(RAX,RSI);
        getValue(RSI,root);
        generateExpressionCode(0,right);
        switch(varptr->type.type)
        {
            case DT_INTEGER:
                fprintf(fp,"    mov rbx,qword [_inttmp0]\n");
                fprintf(fp,"    mov [rsi-rax*16],rbx\n");
                break;
            case DT_BOOLEAN:
                fprintf(fp,"    mov rbx,qword [_booltmp0]\n");
                fprintf(fp,"    mov [rsi-rax*8],rbx\n");
                break;
            case DT_REAL:
                fprintf(fp,"    movsd xmm0,qword [_flttmp0]\n");
                fprintf(fp,"    mov qword [rsi-rax*32],xmm0\n");
                break;
        }
        return;
    }
    generateExpressionCode(0,right);
    switch(varptr->type.type)
    {
        case DT_INTEGER:
            fprintf(fp,"    mov rbx,qword [_inttmp0]\n");
            setValue(RBX,root);
            break;
        case DT_BOOLEAN:
            fprintf(fp,"    mov rbx,qword [_booltmp0]\n");
            setValue(RBX,root);
            break;
        case DT_REAL:
            fprintf(fp,"    movsd xmm0,qword [_flttmp0]\n");
            setFloatValue(RBX,root);
            break;
    }
}

void pushReverse(Struct ASTNode *root){
    if(root==NULL){
        return;
    }
    pushReverse(root->node.idListNode.next);
    VariableEntry *ptr;
    ptr=root->localTableEntry;
    if(ptr.arrayFlag==1){
        getHigh(RAX,root);
        PUSH(RAX);
        PUSH(RAX);
        getLow(RAX,root);
        PUSH(RAX);
        PUSH(RAX);
        getValue(RAX,root);
        PUSH(RAX);
    }else
    {
        switch(ptr->type.type){
            case DT_BOOLEAN:
                getValue(RAX,root);
                PUSH(RAX);
                break;
            case DT_INTEGER:
                getValue(RAX,root);
                PUSH(RAX);
                PUSH(RAX);
                break;
            case DT_REAL:
                getValue(RAX,root);
                PUSH(RAX);
                PUSH(RAX);
                PUSH(RAX);
                PUSH(RAX);                       
        }
    }
}


void generateModuleReuseCode(struct ASTNode *root){
    struct ASTNode *nodeptr;
    fprintf(fp,"push    rbp\n");
    fprintf(fp,"mov    rbp,rsp\n");
    nodeptr=root->node.moduleReuseNode.optional;
    pushReverse(nodeptr);
    nodeptr=root->node.moduleReuseNode.idList;
    pushReverse(nodeptr);
    fprintf(fp,"call %s\n",root->node.moduleReuseNode.id);
    nodeptr=root->node.moduleReuseNode.idList;
    while(nodeptr!=NULL){
        VariableEntry *ptr;
        ptr=nodeptr->localTableEntry;
        if(ptr.arrayFlag==1){
            POP(RAX);
            POP(RAX);
            POP(RAX);
            POP(RAX);
            POP(RAX);
        }else
        {
            switch(ptr->type.type){
                case DT_BOOLEAN:
                    POP(RAX);
                    break;
                case DT_INTEGER:
                    POP(RAX);
                    POP(RAX);
                    break;
                case DT_REAL:
                    POP(RAX);
                    POP(RAX);
                    POP(RAX);
                    POP(RAX);
                    break;                       
            }
        }
        
        nodeptr=nodeptr->node.idListNode.next;
    }
    nodeptr=root->node.moduleReuseNode.optional;
    while(nodeptr!=NULL){
        VariableEntry *ptr;
        ptr=nodeptr->localTableEntry;
        if(ptr.arrayFlag==1){
            POP(RAX);
            POP(RAX);
            POP(RAX);
            POP(RAX);
            POP(RAX);
        }else
        {
            switch(ptr->type.type){
                case DT_BOOLEAN:
                    POP(RAX);
                    setValue(RAX,nodeptr);
                    break;
                case DT_INTEGER:
                    POP(RAX);
                    setValue(RAX,nodeptr);
                    POP(RAX);
                    break;
                case DT_REAL:
                    POP(RAX);
                    setValue(RAX,nodeptr);
                    POP(RAX);
                    POP(RAX);
                    POP(RAX);  
                    break;                     
            }
        }
        
        nodeptr=nodeptr->node.idListNode.next;
    }
    fprintf(fp,"mov     rsp,rbp\n");
    fprintf(fp,"pop     rbp\n");
}


void generateScopeCode(struct ASTNode *root)//Must maintain RBP and RDX
{
    VariableEntry *varptr;
    struct ASTNode *currVar;
    int allocatedSpace;
    int label1,label2,label3;
    int num1,num2;
    fprintf(fp,"    push    rdx\n");
    fprintf(fp,"    mov     rdx,0\n");
    while(root!=NULL)
    {
        switch(root->tag)
        {
            case INPUT_NODE:
                PUSH(RBP);PUSH(RDX);
                generateInputCode(root);
                POP(RDX);POP(RBP);
                root=root->node.inputNode.next;
                break;
            case OUTPUT_NODE:
                PUSH(RBP);PUSH(RDX);
                generateOutputCode(root->node.outputNode.value);
                POP(RDX);POP(RBP);
                root=root->node.outputNode.next;
                break;
            case ASSIGN_NODE:
                PUSH(RBP);PUSH(RDX);
                generateAssignmentCode(root);
                POP(RDX);POP(RBP);
                root=root->node.assignNode.next;
                break;
            case MODULE_REUSE_NODE:
                PUSH(RBP);PUSH(RDX);
                generateModuleReuseCode(root);
                POP(RDX);POP(RBP);
                root=root->node.moduleReuseNode.next;
                break;
            case CONDITION_NODE:
                PUSH(RBP);PUSH(RDX);
                generateConditionCode(root);
                POP(RDX);POP(RBP);
                root=root->node.conditionNode.next;
                break;
            case FOR_NODE:
                varptr=root->localTableEntry;
                num1=root->node.forNode.Range->node.rangeNode.Range1->node.numNode.num;
                num2=root->node.forNode.Range->node.rangeNode.Range2->node.numNode.num;
                PUSH(RBP);PUSH(RDX);
                fprintf(fp,"    mov     rcx,%d\n",num1);
                label1=createLabel();
                fprintf(fp,"_label%d:\n",label1);
                fprintf(fp,"    push    rcx\n");
                fprintf(fp,"    push    rbx\n");
                setValue(RCX,root);
                generateScopeCode(root->node.forNode.stmts);
                fprintf(fp,"    pop     rbx\n");
                fprintf(fp,"    pop     rcx\n");
                fprintf(fp,"    inc     rcx\n");
                fprintf(fp,"    cmp     rcx,%d\n",num2+1);
                fprintf(fp,"    jnz     _label%d\n",label1);
                root=root->node.forNode.next;
                POP(RDX);POP(RBP);
                break;
            case WHILE_NODE:
                PUSH(RBP);PUSH(RDX);
                label1=createLabel();
                label2=createLabel();
                fprintf(fp,"_label%d:\n",label1);
                
                generateExpressionCode(0,root->node.whileNode.expr);   // This will generate output of a Expression and store it in rax or xmm0
                
                fprintf(fp,"    mov   rax,qword [_booltmp0]\n");
                fprintf(fp,"    cmp     rax,0\n");
                fprintf(fp,"    jz      _label%d\n",label2);
                
                generateScopeCode(root->node.whileNode.stmts);
                
                fprintf(fp,"    jmp     _label%d\n",label1);
                fprintf(fp,"_label%d:\n",label2);
                POP(RDX);POP(RBP);
                root=root->node.whileNode.next;
                break;
            case DECLARE_NODE:
                currVar=root->node.declareNode.idList;
                varptr=currVar->localTableEntry;
                if(root->node.declareNode.Range!=NULL)
                {
                    getLow(RSI,currVar);
                    getHigh(RDI,currVar);

                    fprintf(fp,"mov     rcx,rdi\n");
                    fprintf(fp,"sub     rcx,rsi\n");
                    fprintf(fp,"inc     rcx\n");
                    switch(varptr->type.type)
                    {
                        case DT_BOOLEAN:
                            fprintf(fp,"mov     rax,8\n");
                            break;
                        case DT_INTEGER:
                            fprintf(fp,"mov     rax,16\n");
                            break;
                        case DT_REAL:
                            fprintf(fp,"mov     rax,32\n");
                            break;
                    }
                    fprintf(fp,"mul     rcx\n");
                    fprintf(fp,"mov     rcx,rax\n");
                }
                while(currVar!=NULL)
                {
                    varptr=currVar->localTableEntry;
                    if(varptr->type.isArrayFlag==1)
                    {
                        if(varptr->type.isStatic==1)
                        {
                            getAddress(RAX,currVar);
                            fprintf(fp,"add     rax,%d\n",ARRAY_POINTER_WIDTH);
                            setValue(RAX,currVar);
                        }
                        else
                        {
                            fprintf(fp,"mov     rax,rsp\n");
                            fprintf(fp,"add     rax,8\n");
                            setValue(RAX,currVar);

                            fprintf(fp,"sub     rsp,rcx\n");  // ALLOCATING DYNAMIC MEMORY 
                            fprintf(fp,"add     rdx,rcx\n");
                        }
                    }
                    currVar=currVar->next;
                }
                root=root->node.declareNode.next;
                break;
            default:break;
        }
    }
    fprintf(fp, "add    rsp,rdx\n");
    fprintf(fp, "pop    rdx\n");
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
	fprintf(fp,"	SECTION .data\n");
	// printTemporaries
	// printFloatConstants of type _flt0,_flt1, till NUM_FLOAT_CONSTANTS-1 with _flt<i>: dq floatConstants[i]
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
	fprintf(fp,"        sub 	rsp,%d\n",8*activationRecordSize);
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
	createFloatConstant(-1);
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