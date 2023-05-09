%define pc r15
%define w r14
%define rstack r13

section .bss

input_data: resb 1024

section .text

extern read_word

global _start

program_stub: dq 0
xt_interpreter: dq .interpreter
.interpreter: dq interpreter_loop

interpreter_loop:
    mov rdi, input_data
    mov rsi, 1024
    call read_word
    test input_data, input_data
    jz exit

next:
    mov w, [pc]
    add pc, 8
    jmp [w]

docol:
    sub rstack, 8
    mov [rstack], pc
    add w, 8
    mov pc, w
    jmp next