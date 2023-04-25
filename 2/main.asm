section .text
%include "colon.inc"
%include "words.inc"

extern find_word
extern read_word
extern print_error
extern print_string
extern string_length
extern exit

global _start

section .rodata
msg_not_found: db "Not found", 10, 0

section .text
_start:
    mov rdi, rsp
    sub rsp, 256
    mov rsi, 255
.loop:
    push rsi
    call read_word
    mov rdi, rax
    call string_length

    mov rsi, lw
    call find_word
    test rax, rax
    jz .error

    add rax, 8  ; lwがdq(8byte)なので8進める
    mov rdi, rax
    push rdi
    call string_length
    pop rdi
    add rdi, rax
    inc rdi     ; null文字の分進める
    call print_string

    add rsp, 256
    mov rdi, 0
    call exit
.error:
    mov rdi, msg_not_found
    call print_error

    add rsp, 256
    mov rdi, 0
    call exit