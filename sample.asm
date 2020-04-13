global  main ;nasm -felf64 sample.asm && gcc -no-pie sample.o && ./a.out
        extern  printf
        extern  scanf
        SECTION .text
main:
        mov     rbp,rsp
        mov     rcx,6
_label33:   ;Populating up 48 bytes
        push    rcx    
        dec     rcx
        jnz     _label33

        mov     rax,rbp
        add     rax,12
        mov     [rbp+8],rax
        mov     word [rbp+8+8],2
        mov     word [rbp+8+10],4

        mov     rax,qword [rbp+8]
        movsx   rdx,word [rbp+8+8]
        movsx   rcx,word [rbp+8+10]

        mov rsi,rcx
        sub rsi,rdx
        inc rsi

        push    rax
        push    rbx
        push    rcx
        push    rsi
        push    rbp

        mov     rdi,_formatBooleanArray             
        ;mov     rsi,rsi
        ;mov     rdx,rdx
        ;mov     rcx,rcx
        mov     rax,0

        call    printf                  ; printf(format, current_number)

        pop     rbp
        pop     rsi
        pop     rcx
        pop     rbx
        pop     rax

        mov     rbp,rax
        mov     rbx,0
        mov     rcx,rsi
_label35:
        push    rbp
        push    rbx
        push    rcx

        mov    rdi,_formatBooleanInput

        lea     rsi,[rbp+rbx*2]
        mov     rbx,0
        mov     rax,0

        call    scanf                  ; printf(format, current_number)

        pop     rcx
        pop     rbx     
        pop     rbp

        inc     rbx
        dec     rcx
        jnz     _label35

        mov     rcx,6
_label34:   ;Populating up 48 bytes
        pop    rax    
        dec     rcx
        jnz     _label34
        ret

        SECTION .data
_flttmp0:
        dq  0.1
_flttmp1:
        dq  0.5; 64 bit float

_formatIntArray:
        db  "Enter %hi numbers for integer array from %hi to %hi ", 10, 0

_formatBooleanArray:
        db  "Enter %hi numbers for boolean array from %hi to %hi ", 10, 0

_formatRealArray:
        db  "Enter %hi numbers for real array from %hi to %hi ", 10, 0

_formatIntSingle:
        db  "Enter an integer value", 10, 0

_formatBooleanSingle:
        db  "Enter a boolean value", 10, 0

_formatRealSingle:
        db  "Enter a real value", 10, 0


_formatIntInput:
        db  "%d",0

_formatIntOutput:
        db  "%d",10,0

_formatBooleanInput:
        db  "%d",0

_formatBooleanOutput:
        db  "%s",10,0

_formatRealInput:
        db  "%f",0

_formatRealOutput:
        db  "%f",10,0