section .text

; 文字列の長さを返す
; rdi 文字列へのポインタ
string_length:
    xor rax, rax
.loop:
    cmp byte [rdi+rax], 0
    je .end
    inc rax
    jmp .loop
.end:
    ret

; rdi 文字列へのポインタ
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

; rdi 文字列へのポインタ
; rsi 転送先バッファへのポインタ
; rdx バッファの長さ
; returns rax: もし文字列がバッファに入れば転送先アドレス。そうでなければ0
string_copy:
    push rdi
    push rsi
    push rdx
    call string_length
    pop rdx
    pop rsi
    pop rdi

    cmp rax, rdx
    jae .too_long

    push rsi
    
        .loop:
        mov dl, byte[rdi]
        mov byte[rsi], dl
        inc rdi
        inc rsi
        test dl, dl
        jnz .loop

    pop rax
    ret

    .too_long:
    xor rax, rax
    ret