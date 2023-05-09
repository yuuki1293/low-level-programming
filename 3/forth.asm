%define pc r15
%define w r14
%define rstack r13

section .bss

; pcを退避させるためのrstackを確保
; rstack用は下から伸びるため、先に1023確保しておく
resq 1023
rstack_start: resq 1

; 入力された文字列が格納される場所
input_data: resb 1024

section .text

extern read_word

global _start

next:
    mov w, [pc]
    add pc, 8
    jmp [w]

_start: jmp init_impl