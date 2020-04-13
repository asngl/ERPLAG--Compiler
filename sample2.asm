        extern	printf		; the C function, to be called
        SECTION .data		; Data section, initialized variables
	a:	dq	5		; int a=5;
fmt:    db "a=%ld, rax=%ld", 10, 0 ; The printf format, "\n",'0'


        SECTION .text                   ; Code section.

        global main		; the standard gcc entry point
main:				; the program label for the entry point
        PUSH    rbp		; set up stack frame
        mov     rbp,rsp

	mov	rax, [a]	; put a from store into register
	add	rax, 2		; a+2
	PUSH	rax		; value of a+2
        PUSH    qword [a]	; value of variable a
        PUSH    qword fmt	; address of ctrl string
        call    printf		; Call C function
        add     rsp, 12		; POP stack 3 PUSH times 4 bytes

        mov     rsp, rbp	; takedown stack frame
        POP     rbp		; same as "leave" op

	mov	rax,0		;  normal, no error, return value
	ret			; return