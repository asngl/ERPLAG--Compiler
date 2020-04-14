_debugger:; use as call _debugger
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
        and     rsp,0xFFFFFFF0
        call    printf
        mov     rsp,rbp
        
        pop     rbp
        pop     rdx
        pop     rcx
        pop     rbx
        pop     rax

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
        and     rsp,0xfffffff0
        call    printf
        mov     rsp,rbp
        
        pop     rbp
        pop     rdx
        pop     rcx
        pop     rbx
        pop     rax

        ret
_debugOutput1:
        db  10,"rabcdx= %ld : %ld : %ld : %ld ",10,0
_debugOutput2:
        db  "rsp= %ld , rbp = %ld ",10,0