global  main ;nasm -felf64 sample.asm && gcc -no-pie sample.o && ./a.out
        extern  printf
        extern  scanf
        SECTION .text
main:
        push    rbx                     ; we have to save this since we use it

        mov     ecx, 10                 ; ecx will countdown to 0
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

        mov     rdi,_formatIntArray             ; set 1st parameter (format)
        mov     rsi,rcx
        mov     rdx,rax
        mov     rcx,rbx
        mov     rax,1

        call    printf                  ; printf(format, current_number)


        pop     rcx
        pop     rax

        dec     ecx                     ; count down
        jnz     print                   ; if not done counting, do some more

        pop     rbx                     ; restore rbx before returning
        ret

        SECTION .data

_flttmp0:
        dq  0.1
_flttmp1:
        dq  0.5; 64 bit float

_formatIntArray:
        db  "Enter %ld numbers for integer array from %.10lf to %ld ", 10, 0
_formatBoolArray:
        db  "Enter %ld numbers for boolean array from %ld to %ld ", 10, 0
_formatRealArray:
        db  "Enter %ld numbers for real array from %ld to %ld ", 10, 0
