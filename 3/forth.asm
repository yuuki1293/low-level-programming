%define pc r15
%define w r14

section .bss

input_data: resb 1024

section .text

global _start

program_stub: dq 0
xt_interpreter: dq .interpreter
.interpreter: dq interpreter_loop

interpreter_loop:
    test input_data, input_data
    call exit

next:
    mov w, [pc]
    add pc, 8
    jmp [w]