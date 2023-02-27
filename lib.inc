section .text

string_length:
    xor rax, rax
.loop
    cmp byte [rdi+rax], 0
    je .end
    inc rax
    jmp .loop
.end
    ret

print_string:
    push rdi
    call string_length
    mov rdx, rax
    pop rsi
    mov rax, 1
    mov rdi, 1
    syscall
    ret


print_char:
    mov rax, 1
    mov rsi, rdi
    mov rdi, 1
    mov rdx, 1
    syscall
    ret

print_newline:
    mov rax, 1
    mov rdi, 1
    mov rsi, 10
    mov rdx, 1
    syscall
    ret


print_uint:
    xor rax, rax
    ret


print_int:
    xor rax, rax
    ret

string_equals:
    xor rax, rax
    ret


read_char:
    xor rax, rax
    ret 

read_word:
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rax, rax
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
    ret 


string_copy:
    ret