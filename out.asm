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
        sub 	rsp,1192
        push    rdx
        mov     rdx,0


        push    rbp
        push    rdx
        mov     qword [_inttmp0],12
        mov     rbx,qword [_inttmp0]
        mov     qword [rbp-8-16], rbx
        pop     rdx
        pop     rbp


        push    rbp
        push    rdx
        movsd   xmm0,qword [_flt1]
        movsd   qword [_flttmp0],xmm0
        movsd   xmm0,qword [_flttmp0]
        movsd     qword [rbp-8-48],xmm0
        pop     rdx
        pop     rbp

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

       mov     rsi,2
        mov     rdi,20
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
        lea     rax,[rbp-8-392]
        sub     rax,8
        mov     qword [rbp-8-392], rax

        push    rbp
        push    rdx
        mov     rax,qword [rbp-8-32]
        push    rax
        mov     rax,qword [rbp-8-0]
        push    rax
        push    rax
        mov     rax,qword [rbp-8-48]
        push    rax
        push    rax
        push    rax
        push    rax
        mov     rax,20
        push    rax
        push    rax
       mov     rax,2
        push    rax
        push    rax
        mov     rax,qword [rbp-8-80]
        push    rax
        mov     rax,qword [rbp-8-16]
        push    rax
        push    rax
        call var_demo_array
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
        pop     rax
        mov     qword [rbp-8-0], rax
        pop     rax
        pop     rax
        mov     qword [rbp-8-32], rax
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rax,qword [rbp-8-40]
        push    rax
        mov     rax,qword [rbp-8-0]
        push    rax
        push    rax
        mov     rax,qword [rbp-8-48]
        push    rax
        push    rax
        push    rax
        push    rax
        mov     rax,20
        push    rax
        push    rax
       mov     rax,2
        push    rax
        push    rax
        mov     rax,qword [rbp-8-80]
        push    rax
        mov     rax,qword [rbp-8-16]
        push    rax
        push    rax
        call var_demo_array
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
        pop     rax
        mov     qword [rbp-8-0], rax
        pop     rax
        pop     rax
        mov     qword [rbp-8-40], rax
        pop     rdx
        pop     rbp


        push    rbp
        push    rdx
        movsd   xmm0,qword [_flt2]
        movsd   qword [_flttmp0],xmm0
        mov     rbx,qword [_inttmp0]
        mov     qword [rbp-8-704], rbx
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rdi,_formatIntSingle
        mov     rax,0
        call    printf
        mov     rdi,_formatIntInput
        lea     rsi,[rbp-8-720]
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
        mov     rdi,_formatRealSingle
        mov     rax,0
        call    printf
        mov     rdi,_formatRealInput
        lea     rsi,[rbp-8-768]
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
        movsd   xmm0,qword [_flt3]
        movsd   qword [_flttmp0],xmm0
        movsd   xmm0,qword [_flttmp0]
        movsd     qword [rbp-8-800],xmm0
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        movsd     xmm0,qword [rbp-8-48]
        movsd   qword [_flttmp2],xmm0
        movsd     xmm0,qword [rbp-8-768]
        movsd   qword [_flttmp4],xmm0
        movsd     xmm0,qword [rbp-8-800]
        movsd   qword [_flttmp5],xmm0
        movsd   xmm0,qword [_flttmp4]
        movsd   xmm1,qword [_flttmp5]
        mulsd   xmm0,xmm1
        movsd   qword [_flttmp3],xmm0
        movsd   xmm0,qword [_flttmp2]
        movsd   xmm1,qword [_flttmp3]
        addsd   xmm0,xmm1
        movsd   qword [_flttmp1],xmm0
        mov     rax,qword [rbp-8-704]
        mov     qword [_inttmp3],rax
        movsd     xmm0,qword [rbp-8-736]
        movsd   qword [_flttmp4],xmm0
        mov     rax,qword [_inttmp3]
        mov     rbx,qword [_inttmp4]
        mul     rbx
        mov     [_inttmp2],rax
        movsd   xmm0,qword [_flttmp1]
        movsd   xmm1,qword [_flttmp2]
        subsd   xmm0,xmm1
        movsd   qword [_flttmp0],xmm0
        movsd   xmm0,qword [_flttmp0]
        movsd     qword [rbp-8-736],xmm0
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     qword [_booltmp2],1
        mov     qword [_booltmp3],0
        mov     rax,qword [_booltmp2]
        mov     rbx,qword [_booltmp3]
        and     rax,rbx
        mov     [_booltmp1],rax
        movsd     xmm0,qword [rbp-8-768]
        movsd   qword [_flttmp2],xmm0
        mov     rax,qword [_booltmp1]
        mov     rbx,qword [_booltmp2]
        or      rax,rbx
        mov     [_booltmp0],rax
        mov     rbx,qword [_booltmp0]
        mov     qword [rbp-8-40], rbx
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rax,qword [rbp-8-704]
        mov     qword [_inttmp1],rax
        mov     rax,qword [rbp-8-720]
        mov     qword [_inttmp2],rax
        mov     rax,qword [_inttmp1]
        mov     rbx,qword [_inttmp2]
        mov     rcx,0
        cmp     rax,rbx
        jg      _label1
        mov     rcx,1
_label1:
        mov [_booltmp0],rcx
        mov     rbx,qword [_booltmp0]
        mov     qword [rbp-8-32], rbx
        pop     rdx
        pop     rbp


        push    rbp
        push    rdx
        mov     qword [_inttmp1],23
       mov     rsi,2
        mov     rdi,20
        mov     rax,6
        cmp     rax,rsi
        jl      _indexerrorlabel_
        cmp     rdi,rax
        jl      _indexerrorlabel_
        sub     rax,rsi
        mov     rsi,qword [rbp-8-80]
        imul    rax,rax,16
        sub     rsi,rax
        mov    rax,qword [rsi]
        mov    qword [_inttmp2],rax
        mov     rax,qword [_inttmp1]
        mov     rbx,qword [_inttmp2]
        add     rax,rbx
        mov     [_inttmp0],rax
        mov     rbx,qword [_inttmp0]
        mov     qword [rbp-8-832], rbx
        pop     rdx
        pop     rbp

        push    rbp
        push    rdx
        mov     rax,qword [rbp-8-704]
        mov     qword [_inttmp2],rax
        mov     qword [_inttmp3],25
        mov     rax,qword [_inttmp2]
        mov     rbx,qword [_inttmp3]
        mul     rbx
        mov     [_inttmp1],rax
        mov     qword [