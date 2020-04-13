        global  main ;nasm -felf64 inputTaker.asm && gcc -no-pie inputTaker.o && ./a.out
        extern  printf
        extern  scanf
        SECTION .text
main:
        xor     rax, rax                ; rax will hold the current number
        xor     rbx, rbx                ; rbx will hold the next number
print:  
        movsd   xmm1,[_flttmp0]
        movsd   xmm2,[_flttmp1]
        ;mulsd   xmm2,xmm2
        addsd   xmm1,xmm2
        movsd   xmm0,xmm1
        movsd   [_flttmp0],xmm1
        add     rax,5
        add     rbx,10
        
        push    rax
        push    rcx

        mov     rdi,_formatRealArray             ; set 1st parameter (format)
        mov     rsi,6d
        mov     rdx,rax
        mov     rcx,rbx
        mov     rax,0

        call    printf                  ; printf(format, current_number)


        pop     rcx
        pop     rax
        
        mov     rbp,rsp

        mov     rcx,6
_label33:   ;Populating up 48 bytes
        push    rax    
        ;pop     rax
        dec     rcx
        jnz     _label33
mov     rcx,6

        mov     rax,1
        mov     rcx,0
_label34:
        push    rbp
        push    rax
        push    rcx

        mov    rdi,_formatRealInput

        lea     rsi,[_flttmp0]
        mov     rax,0

        call    scanf                  ; printf(format, current_number)
        movss   xmm0,[_flttmp0]
        movss   dword [rbp+rax*4],xmm0
        pop     rcx
        pop     rax     
        pop     rbp

        inc     rax
        inc     rcx
        cmp     rcx,6
        jnz     _label34
        
        mov     rax,1
        mov     rcx,0
_label36:
        push    rbp
        push    rax
        push    rcx

        mov    rdi,_formatRealOutput

        movss   xmm0,dword [rbp+rax*4]
        mov     rax,1

        call    printf                  ; printf(format, current_number)

        pop     rcx
        pop     rax     
        pop     rbp

        inc rax
        inc rcx
        cmp rcx,6
        jnz     _label36
        




        mov rcx,6
_label35: ;Freeing up 48 bytes 
        ;push    rax    
        pop     rax
        dec     rcx
        jnz     _label35

        ret

        SECTION .data

_flttmp0:
        dw  0.1
_flttmp1:
        dq  0.5; 64 bit float

_formatIntArray:
        db  "Enter %hd numbers for integer array from %hi to %hi ", 10, 0

_formatBooleanArray:
        db  "Enter %hi numbers for boolean array from %hi to %hi ", 10, 0

_formatRealArray:
        db  "Enter %hi numbers for real array from %hi to %hi ", 10, 0

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