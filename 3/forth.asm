%define pc r15
%define w r14
%define rstack r13

section .bss

input_data: resb 1024

section .text

extern read_word

global _start

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

_start: jmp init_impl