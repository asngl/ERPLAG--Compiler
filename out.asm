        ;nasm -felf64 out.asm -o out.o && gcc -no-pie out.o && ./a.out
        global  main
        extern  printf
        extern  scanf
        SECTION .text
main:
        push    rbp
        mov     rbp,rsp
        and     rsp,0xfffffffffffffff0

        ;PUSH PARAMETERS NOW

        call    driver

        ;POP PARAMETERS NOW
        
        ;Fill output parameters while poppping

        mov     rsp,rbp
        pop     rbp
        
        ret
driver:
        push    rbp
        mov     rbp,rsp
        sub 	rsp,176
        push    rdx
        mov     rdx,0


        push    rbp
        push    rdx
        mov     rdi,_formatIntSingle
        mov     rax,0
        call    printf
        mov     rdi,_formatIntInput
        lea     rsi,[rbp-8-0]
        mov     rax,0
        push    rbp
        mov     rbp,rsp
        and     rsp,0xFFFFFFFFFFFFFFF0
        call    scanf
        mov     rsp,rbp
        pop     rbp
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rdi,_formatIntSingle
        mov     rax,0
        call    printf
        mov     rdi,_formatIntInput
        lea     rsi,[rbp-8-16]
        mov     rax,0
        push    rbp
        mov     rbp,rsp
        and     rsp,0xFFFFFFFFFFFFFFF0
        call    scanf
        mov     rsp,rbp
        pop     rbp
        pop     rdx
        pop     rbp

       mov     rsi,10
        mov     rdi,14
        cmp     rsi,rdi
        jg     _indexordererror_
        mov     rcx,rdi
        sub     rcx,rsi
        inc     rcx
        mov     rax,16
        mul     rcx
        mov     rcx,rax
        lea     rax,[rbp-8-80]
        sub     rax,8
        mov     qword [rbp-8-80], rax

        mov     rsi,qword [rbp-8-0]
        mov     rdi,qword [rbp-8-16]
        mov     rcx,rdi
        sub     rcx,rsi
        inc     rcx
        mov     rax,16
        mul     rcx
        mov     rcx,rax
        mov     rax,rsp
        sub     rax,8
        mov     qword [rbp-8-168], rax
        sub     rsp,rcx
        add     rdx,rcx

        push    rbp
        push    rdx
        mov     rax,qword [rbp-8-80]
       mov     rdx,10
        mov     rcx,14
        mov rsi,rcx
        sub rsi,rdx
        inc rsi
        push    rax
        push    rsi
        mov     rdi,_formatIntArray
        mov     rax,0
        call    printf
        pop     rsi
        pop     rax
        mov     rbp,rax
        mov     rbx,0
        mov     rcx,rsi
_label1:
        push    rbp
        push    rbx
        push    rcx
        mov     rdi,_formatIntInput
        imul    rbx,rbx,16
        sub     rbp,rbx
        lea     rsi,[rbp]
        mov     rbx,0
        mov     rax,0
        push    rbp
        mov     rbp,rsp
        and     rsp,0xFFFFFFFFFFFFFFF0
        call    scanf
        mov     rsp,rbp
        pop     rbp
        pop     rcx
        pop     rbx
        pop     rbp
        inc     rbx
        dec     rcx
        jnz     _label1
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rax,qword [rbp-8-168]
        mov     rdx,qword [rbp-8-0]
        mov     rcx,qword [rbp-8-16]
        mov rsi,rcx
        sub rsi,rdx
        inc rsi
        push    rax
        push    rsi
        mov     rdi,_formatIntArray
        mov     rax,0
        call    printf
        pop     rsi
        pop     rax
        mov     rbp,rax
        mov     rbx,0
        mov     rcx,rsi
_label2:
        push    rbp
        push    rbx
        push    rcx
        mov     rdi,_formatIntInput
        imul    rbx,rbx,16
        sub     rbp,rbx
        lea     rsi,[rbp]
        mov     rbx,0
        mov     rax,0
        push    rbp
        mov     rbp,rsp
        and     rsp,0xFFFFFFFFFFFFFFF0
        call    scanf
        mov     rsp,rbp
        pop     rbp
        pop     rcx
        pop     rbx
        pop     rbp
        inc     rbx
        dec     rcx
        jnz     _label2
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rax,qword [rbp-8-64]
        push    rax
        push    rax
        mov     rax,qword [rbp-8-48]
        push    rax
        push    rax
        mov     rax,qword [rbp-8-32]
        push    rax
        push    rax
        mov     rax,qword [rbp-8-16]
        push    rax
        push    rax
        mov     rax,qword [rbp-8-0]
        push    rax
        push    rax
        mov     rax,qword [rbp-8-168]
        push    rax
        mov     rax,14
        push    rax
        push    rax
       mov     rax,10
        push    rax
        push    rax
        mov     rax,qword [rbp-8-80]
        push    rax
        call array_parameter_demo
        pop     rax
        pop     rax
        pop     rax
        pop     rax
        pop     rax
        pop     rax
        pop     rax
        pop     rax
        pop     rax
        pop     rax
        pop     rax
        mov     qword [rbp-8-32], rax
        pop     rax
        pop     rax
        mov     qword [rbp-8-48], rax
        pop     rax
        pop     rax
        mov     qword [rbp-8-64], rax
        pop     rax
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rdi, _formatIntOutput
        mov     rsi,qword [rbp-8-32]
        mov    rax,0
        call    printf
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rdi, _formatIntOutput
        mov     rsi,qword [rbp-8-48]
        mov    rax,0
        call    printf
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rdi, _formatIntOutput
        mov     rsi,qword [rbp-8-64]
        mov    rax,0
        call    printf
        call    _newline_
        pop     rdx
        pop     rbp

        add     rsp,rdx
        pop     rdx
        mov     rsp,rbp
        pop     rbp
	    ret

array_parameter_demo:
        push    rbp
        mov     rbp,rsp
        sub 	rsp,280
        push    rdx
        mov     rdx,0
        push    rbp
        push    rdx
        call    _output_
        mov    rax,0
       mov     rcx,qword [rbp+16+8]
        mov     rbx,qword [rbp+16+24]
        mov     r8,qword [rbp+16+0]
_label3:
        push    rbp
        push    rax
        push    rcx
        push    rbx
        push    r8
        mov    rdi, _formatIntOutput
        imul    rax,rax,16
        sub     r8,rax
        mov    rsi,qword [r8]
        mov    rax,0
        call    printf
        pop    r8
        pop    rbx
        pop    rcx
        pop    rax
        pop    rbp
        inc    rax
        inc    rcx
        cmp    rcx,rbx
        jle    _label3
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rax,0
       mov     rcx,qword [rbp+16+48]
        mov     rbx,qword [rbp+16+64]
        mov     r8,qword [rbp+16+40]
_label4:
        push    rbp
        push    rax
        push    rcx
        push    rbx
        push    r8
        mov    rdi, _formatIntOutput
        imul    rax,rax,16
        sub     r8,rax
        mov    rsi,qword [r8]
        mov    rax,0
        call    printf
        pop    r8
        pop    rbx
        pop    rcx
        pop    rax
        pop    rbp
        inc    rax
        inc    rcx
        cmp    rcx,rbx
        jle    _label4
        call    _newline_
        pop     rdx
        pop     rbp


        push    rbp
        push    rdx
        mov     rdi,_formatIntSingle
        mov     rax,0
        call    printf
        mov     rdi,_formatIntInput
        lea     rsi,[rbp-8-32]
        mov     rax,0
        push    rbp
        mov     rbp,rsp
        and     rsp,0xFFFFFFFFFFFFFFF0
        call    scanf
        mov     rsp,rbp
        pop     rbp
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rdi,_formatIntSingle
        mov     rax,0
        call    printf
        mov     rdi,_formatIntInput
        lea     rsi,[rbp-8-48]
        mov     rax,0
        push    rbp
        mov     rbp,rsp
        and     rsp,0xFFFFFFFFFFFFFFF0
        call    scanf
        mov     rsp,rbp
        pop     rbp
        pop     rdx
        pop     rbp

       mov     rsi,10
        mov     rdi,14
        cmp     rsi,rdi
        jg     _indexordererror_
        mov     rcx,rdi
        sub     rcx,rsi
        inc     rcx
        mov     rax,16
        mul     rcx
        mov     rcx,rax
        lea     rax,[rbp-8-96]
        sub     rax,8
        mov     qword [rbp-8-96], rax
        lea     rax,[rbp-8-184]
        sub     rax,8
        mov     qword [rbp-8-184], rax

        mov     rsi,qword [rbp-8-32]
        mov     rdi,qword [rbp-8-48]
        mov     rcx,rdi
        sub     rcx,rsi
        inc     rcx
        mov     rax,16
        mul     rcx
        mov     rcx,rax
        mov     rax,rsp
        sub     rax,8
        mov     qword [rbp-8-272], rax
        sub     rsp,rcx
        add     rdx,rcx

        push    rbp
        push    rdx
        mov     rax,qword [rbp-8-96]
       mov     rdx,10
        mov     rcx,14
        mov rsi,rcx
        sub rsi,rdx
        inc rsi
        push    rax
        push    rsi
        mov     rdi,_formatIntArray
        mov     rax,0
        call    printf
        pop     rsi
        pop     rax
        mov     rbp,rax
        mov     rbx,0
        mov     rcx,rsi
_label5:
        push    rbp
        push    rbx
        push    rcx
        mov     rdi,_formatIntInput
        imul    rbx,rbx,16
        sub     rbp,rbx
        lea     rsi,[rbp]
        mov     rbx,0
        mov     rax,0
        push    rbp
        mov     rbp,rsp
        and     rsp,0xFFFFFFFFFFFFFFF0
        call    scanf
        mov     rsp,rbp
        pop     rbp
        pop     rcx
        pop     rbx
        pop     rbp
        inc     rbx
        dec     rcx
        jnz     _label5
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rax,qword [rbp-8-272]
        mov     rdx,qword [rbp-8-32]
        mov     rcx,qword [rbp-8-48]
        mov rsi,rcx
        sub rsi,rdx
        inc rsi
        push    rax
        push    rsi
        mov     rdi,_formatIntArray
        mov     rax,0
        call    printf
        pop     rsi
        pop     rax
        mov     rbp,rax
        mov     rbx,0
        mov     rcx,rsi
_label6:
        push    rbp
        push    rbx
        push    rcx
        mov     rdi,_formatIntInput
        imul    rbx,rbx,16
        sub     rbp,rbx
        lea     rsi,[rbp]
        mov     rbx,0
        mov     rax,0
        push    rbp
        mov     rbp,rsp
        and     rsp,0xFFFFFFFFFFFFFFF0
        call    scanf
        mov     rsp,rbp
        pop     rbp
        pop     rcx
        pop     rbx
        pop     rbp
        inc     rbx
        dec     rcx
        jnz     _label6
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rax,0
       mov     rcx,10
        mov     rbx,14
        mov     r8,qword [rbp-8-96]
_label7:
        push    rbp
        push    rax
        push    rcx
        push    rbx
        push    r8
        mov    rdi, _formatIntOutput
        imul    rax,rax,16
        sub     r8,rax
        mov    rsi,qword [r8]
        mov    rax,0
        call    printf
        pop    r8
        pop    rbx
        pop    rcx
        pop    rax
        pop    rbp
        inc    rax
        inc    rcx
        cmp    rcx,rbx
        jle    _label7
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     qword [_inttmp0],0
        mov     rbx,qword [_inttmp0]
        mov     qword [rbp+16+80], rbx
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rcx,10
_label8:
        push    rcx
        push    rbx
        mov     qword [rbp-8-0], rcx
        push    rdx
        mov     rdx,0
        push    rbp
        push    rdx
        call    _output_
       mov     rdx,10
        mov     rcx,14
        mov     r8,qword [rbp-8-96]
        mov     rsi,qword [rbp-8-0]
        cmp    rdx,rsi
        jg    _indexerrorlabel_
        cmp    rcx,rsi
        jl    _indexerrorlabel_
        mov    rdi, _formatIntOutput
        sub    rsi,rdx
        imul    rsi,rsi,16
        sub     r8,rsi
        mov   rsi,qword [r8]
        mov    rax,0
        call    printf
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rax,qword [rbp+16+80]
        mov     qword [_inttmp1],rax
       mov     rsi,10
        mov     rdi,14
        mov     rax,qword [rbp-8-0]
        cmp     rax,rsi
        jl      _indexerrorlabel_
        cmp     rdi,rax
        jl      _indexerrorlabel_
        sub     rax,rsi
        mov     rsi,qword [rbp-8-96]
        imul    rax,rax,16
        sub     rsi,rax
        mov    rax,qword [rsi]
        mov    qword [_inttmp2],rax
        mov     rax,qword [_inttmp1]
        mov     rbx,qword [_inttmp2]
        add     rax,rbx
        mov     [_inttmp0],rax
        mov     rbx,qword [_inttmp0]
        mov     qword [rbp+16+80], rbx
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rdi, _formatIntOutput
        mov     rsi,qword [rbp+16+80]
        mov    rax,0
        call    printf
        call    _newline_
        pop     rdx
        pop     rbp

        add     rsp,rdx
        pop     rdx
        pop     rbx
        pop     rcx
        inc     rcx
        cmp     rcx,15
        jnz     _label8
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rax,0
       mov     rcx,10
        mov     rbx,14
        mov     r8,qword [rbp-8-96]
_label9:
        push    rbp
        push    rax
        push    rcx
        push    rbx
        push    r8
        mov    rdi, _formatIntOutput
        imul    rax,rax,16
        sub     r8,rax
        mov    rsi,qword [r8]
        mov    rax,0
        call    printf
        pop    r8
        pop    rbx
        pop    rcx
        pop    rax
        pop    rbp
        inc    rax
        inc    rcx
        cmp    rcx,rbx
        jle    _label9
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rdi, _formatIntOutput
        mov     rsi,qword [rbp+16+80]
        mov    rax,0
        call    printf
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rsi,qword [rbp-8-32]
        mov     rdi,qword [rbp-8-48]
        mov     rax,5
        cmp     rax,rsi
        jl      _indexerrorlabel_
        cmp     rdi,rax
        jl      _indexerrorlabel_
        sub     rax,rsi
        mov     rsi,qword [rbp-8-272]
        push    rax
        push    rsi
        mov     rsi,qword [rbp-8-32]
        mov     rdi,qword [rbp-8-48]
        mov     rax,6
        cmp     rax,rsi
        jl      _indexerrorlabel_
        cmp     rdi,rax
        jl      _indexerrorlabel_
        sub     rax,rsi
        mov     rsi,qword [rbp-8-272]
        imul    rax,rax,16
        sub     rsi,rax
        mov    rax,qword [rsi]
        mov    qword [_inttmp1],rax
        mov     rsi,qword [rbp-8-32]
        mov     rdi,qword [rbp-8-48]
        mov     rax,7
        cmp     rax,rsi
        jl      _indexerrorlabel_
        cmp     rdi,rax
        jl      _indexerrorlabel_
        sub     rax,rsi
        mov     rsi,qword [rbp-8-272]
        imul    rax,rax,16
        sub     rsi,rax
        mov    rax,qword [rsi]
        mov    qword [_inttmp2],rax
        mov     rax,qword [_inttmp1]
        mov     rbx,qword [_inttmp2]
        add     rax,rbx
        mov     [_inttmp0],rax
        pop     rsi
        pop     rax
        mov     rbx,qword [_inttmp0]
        imul    rax,rax,16
        sub     rsi,rax
        mov     [rsi],rbx
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rax,0
        mov     rcx,qword [rbp-8-32]
        mov     rbx,qword [rbp-8-48]
        mov     r8,qword [rbp-8-272]
_label10:
        push    rbp
        push    rax
        push    rcx
        push    rbx
        push    r8
        mov    rdi, _formatIntOutput
        imul    rax,rax,16
        sub     r8,rax
        mov    rsi,qword [r8]
        mov    rax,0
        call    printf
        pop    r8
        pop    rbx
        pop    rcx
        pop    rax
        pop    rbp
        inc    rax
        inc    rcx
        cmp    rcx,rbx
        jle    _label10
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     qword [_inttmp0],0
        mov     rbx,qword [_inttmp0]
        mov     qword [rbp+16+96], rbx
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rax,qword [rbp+16+0]
        mov     qword [rbp-8-184], rax
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rax,0
       mov     rcx,10
        mov     rbx,14
        mov     r8,qword [rbp-8-184]
_label11:
        push    rbp
        push    rax
        push    rcx
        push    rbx
        push    r8
        mov    rdi, _formatIntOutput
        imul    rax,rax,16
        sub     r8,rax
        mov    rsi,qword [r8]
        mov    rax,0
        call    printf
        pop    r8
        pop    rbx
        pop    rcx
        pop    rax
        pop    rbp
        inc    rax
        inc    rcx
        cmp    rcx,rbx
        jle    _label11
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rcx,10
_label12:
        push    rcx
        push    rbx
        mov     qword [rbp-8-16], rcx
        push    rdx
        mov     rdx,0
        push    rbp
        push    rdx
        call    _output_
       mov     rdx,10
        mov     rcx,14
        mov     r8,qword [rbp-8-184]
        mov     rsi,qword [rbp-8-16]
        cmp    rdx,rsi
        jg    _indexerrorlabel_
        cmp    rcx,rsi
        jl    _indexerrorlabel_
        mov    rdi, _formatIntOutput
        sub    rsi,rdx
        imul    rsi,rsi,16
        sub     r8,rsi
        mov   rsi,qword [r8]
        mov    rax,0
        call    printf
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rax,qword [rbp+16+96]
        mov     qword [_inttmp1],rax
       mov     rsi,10
        mov     rdi,14
        mov     rax,qword [rbp-8-16]
        cmp     rax,rsi
        jl      _indexerrorlabel_
        cmp     rdi,rax
        jl      _indexerrorlabel_
        sub     rax,rsi
        mov     rsi,qword [rbp-8-184]
        imul    rax,rax,16
        sub     rsi,rax
        mov    rax,qword [rsi]
        mov    qword [_inttmp2],rax
        mov     rax,qword [_inttmp1]
        mov     rbx,qword [_inttmp2]
        add     rax,rbx
        mov     [_inttmp0],rax
        mov     rbx,qword [_inttmp0]
        mov     qword [rbp+16+96], rbx
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rdi, _formatIntOutput
        mov     rsi,qword [rbp+16+96]
        mov    rax,0
        call    printf
        call    _newline_
        pop     rdx
        pop     rbp

        add     rsp,rdx
        pop     rdx
        pop     rbx
        pop     rcx
        inc     rcx
        cmp     rcx,15
        jnz     _label12
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rax,0
       mov     rcx,10
        mov     rbx,14
        mov     r8,qword [rbp-8-184]
_label13:
        push    rbp
        push    rax
        push    rcx
        push    rbx
        push    r8
        mov    rdi, _formatIntOutput
        imul    rax,rax,16
        sub     r8,rax
        mov    rsi,qword [r8]
        mov    rax,0
        call    printf
        pop    r8
        pop    rbx
        pop    rcx
        pop    rax
        pop    rbp
        inc    rax
        inc    rcx
        cmp    rcx,rbx
        jle    _label13
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rdi, _formatIntOutput
        mov     rsi,qword [rbp+16+96]
        mov    rax,0
        call    printf
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     qword [_inttmp0],0
        mov     rbx,qword [_inttmp0]
        mov     qword [rbp+16+112], rbx
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rax,0
       mov     rcx,qword [rbp+16+48]
        mov     rbx,qword [rbp+16+64]
        mov     r8,qword [rbp+16+40]
_label14:
        push    rbp
        push    rax
        push    rcx
        push    rbx
        push    r8
        mov    rdi, _formatIntOutput
        imul    rax,rax,16
        sub     r8,rax
        mov    rsi,qword [r8]
        mov    rax,0
        call    printf
        pop    r8
        pop    rbx
        pop    rcx
        pop    rax
        pop    rbp
        inc    rax
        inc    rcx
        cmp    rcx,rbx
        jle    _label14
        call    _newline_
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rcx,1
_label15:
        push    rcx
        push    rbx
        mov     qword [rbp-8-64], rcx
        push    rdx
        mov     rdx,0
        push    rbp
        push    rdx
        mov     rdi,_formatIntSingle
        mov     rax,0
        call    printf
        mov     rdi,_formatIntInput
        lea     rsi,[rbp-8-80]
        mov     rax,0
        push    rbp
        mov     rbp,rsp
        and     rsp,0xFFFFFFFFFFFFFFF0
        call    scanf
        mov     rsp,rbp
        pop     rbp
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rax,qword [rbp+16+112]
        mov     qword [_inttmp1],rax
       mov     rsi,qword [rbp+16+48]
        mov     rdi,qword [rbp+16+64]
        mov     rax,qword [rbp-8-80]
        cmp     rax,rsi
        jl      _indexerrorlabel_
        cmp     rdi,rax
        jl      _indexerrorlabel_
        sub     rax,rsi
        mov     rsi,qword [rbp+16+40]
        imul    rax,rax,16
        sub     rsi,rax
        mov    rax,qword [rsi]
        mov    qword [_inttmp2],rax
        mov     rax,qword [_inttmp1]
        mov     rbx,qword [_inttmp2]
        add     rax,rbx
        mov     [_inttmp0],rax
        mov     rbx,qword [_inttmp0]
        mov     qword [rbp+16+112], rbx
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rdi, _formatIntOutput
        mov     rsi,qword [rbp+16+112]
        mov    rax,0
        call    printf
        call    _newline_
        pop     rdx
        pop     rbp

        add     rsp,rdx
        pop     rdx
        pop     rbx
        pop     rcx
        inc     rcx
        cmp     rcx,5
        jnz     _label15
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        call    _output_
        mov    rdi, _formatIntOutput
        mov     rsi,qword [rbp+16+112]
        mov    rax,0
        call    printf
        call    _newline_
        pop     rdx
        pop     rbp

        add     rsp,rdx
        pop     rdx
        mov     rsp,rbp
        pop     rbp
	    ret

_output_:
        push    rdi
        push    rax
        push    rbp
        mov     rdi,_output
        mov     rax,0
        mov     rbp,rsp
        and     rsp,0xfffffffffffffff0
        call    printf
        mov     rsp,rbp
        pop     rbp
        pop     rax
        pop     rdi
        ret
_newline_:
        push    rdi
        push    rax
        push    rbp
        mov     rdi,_line
        mov     rax,0
        mov     rbp,rsp
        and     rsp,0xfffffffffffffff0
        call    printf
        mov     rsp,rbp
        pop     rbp
        pop     rax
        pop     rdi
        ret
_indexerrorlabel_:
        and     rsp,0xfffffffffffffff0
        mov     rdi,_errorString
        mov     rax,0
        call    printf
        mov     rax,1
        jmp     _exit

_errorString:
        db  "RUN TIME ERROR:  Array index out of bound ",10,0

_indexordererror_:
        and     rsp,0xfffffffffffffff0
        mov     rdi,_errorStringLowHigh
        mov     rax,0
        call    printf
        mov     rax,1
        jmp     _exit

_errorStringLowHigh:
        db  "RUN TIME ERROR: Lower array index cannot be greater than higher array index. ",10,0

_boundserrorlabel_:
        and     rsp,0xfffffffffffffff0
        mov     rdi,_errorString2
        mov     rax,0
        call    printf
        mov     rax,1
        jmp     _exit
        ret

_errorString2:
        db  "Array Bounds do not match in assignment statement.",10,0

_exit:
        and     rsp,0xfffffffffffffff0
        mov     rdi,_errorString3
        mov     rax,0
        call    printf
        mov     rax,0x02000001
        xor     rdi,rdi
        syscall
        ret

_errorString3:
        db  "Aborting Execution",10,0
_debugger:; use as call _debugger
                push    rsi
                push    rdi
        push    rax
        push    rbx
        push    rcx
        push    rdx
        push    rbp
        
        mov     rdi,_debugOutput1
        mov     rsi,rax
        mov     r8,rdx
        mov     rdx,rbx
        mov     r10,rcx
        mov     rax,0

        mov     rbp,rsp
        and     rsp,0xfffffffffffffff0
        call    printf
        mov     rsp,rbp
        
        pop     rbp
        pop     rdx
        pop     rcx
        pop     rbx
        pop     rax
        pop     rdi
        pop     rsi

        push    rsi
        push    rdi
        push    rax
        push    rdx
        push    rbp
        
        mov     rdx,rdi
        mov     rdi,_debugOutput3
        mov     rax,0

        mov     rbp,rsp
        and     rsp,0xfffffffffffffff0
        call    printf
        mov     rsp,rbp
        
        pop     rbp
        pop     rdx
        pop     rax
        pop     rdi
        pop     rsi

        push    rsi
                push    rdi

        mov     rsi,rsp
        add     rsi,8

        push    rax
        push    rbx
        push    rcx
        push    rdx
        push    rbp
        
        mov     rdx,rbp
        mov     rdi,_debugOutput2
        mov     rax,0


        mov     rbp,rsp
        and     rsp,0xfffffffffffffff0
        call    printf
        mov     rsp,rbp
        
        pop     rbp
        pop     rdx
        pop     rcx
        pop     rbx
        pop     rax

        pop     rdi
        pop     rsi
        ret
_debugOutput1:
        db  10,"rabcdx= %ld : %ld : %ld : %ld ",10,0
_debugOutput2:
        db  "rsp= %ld , rbp = %ld ",10,0
_debugOutput3:
        db  "rsi= %ld , rdi = %ld ",10,0
	SECTION .data
_booltmp0:
        dq 0
_inttmp0:
        dq 0
_flttmp0:
        dq 0.0
_booltmp1:
        dq 0
_inttmp1:
        dq 0
_flttmp1:
        dq 0.0
_booltmp2:
        dq 0
_inttmp2:
        dq 0
_flttmp2:
        dq 0.0
_flt0:
        dq -1.000000
_formatIntArray:
        db  "Input: Enter %ld numbers for integer array from %ld to %ld ", 10, 0

_formatBooleanArray:
        db  "Input: Enter %ld numbers for boolean array from %ld to %ld ", 10, 0

_formatRealArray:
        db  "Input: Enter %ld numbers for real array from %ld to %ld ", 10, 0

_formatIntSingle:
        db  "Input: Enter an integer value", 10, 0

_formatBooleanSingle:
        db  "Input: Enter a boolean value", 10, 0

_formatRealSingle:
        db  "Input: Enter a real value", 10, 0

_formatIntInput:
        db  "%ld",0

_formatIntOutput:
        db  "%ld ",0

_formatBooleanInput:
        db  "%ld",0

_formatBooleanTrue:
        db  "True ",0
_formatBooleanFalse:
        db  "False ",0

_formatRealInput:
        db  "%lf",0

_formatRealOutput:
        db  "%lf ",0
_line:
        db 10,0
_output:
        db "Output: ",0
