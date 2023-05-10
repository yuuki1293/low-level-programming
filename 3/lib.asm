section .text
global string_length
global print_error
global print_string
global print_char
global print_newline
global print_uint
global print_int
global string_equals
global read_char
global read_word
global parse_uint
global parse_int
global string_copy
global exit

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
print_error:
    push rdi
    call string_length
    mov rdx, rax
    pop rsi
    mov rax, 1
    mov rdi, 2
    syscall
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

; rdi 文字コード
print_char:
    push rdi
    mov rdi, rsp
    call print_string
    pop rdi
    ret

print_newline:
    mov rax, 1
    mov rdi, 1
    mov rsi, 10
    mov rdx, 1
    syscall
    ret

; rdi 8バイト長符号なし整数
print_uint:
    mov rax, rdi
    mov rdi, rsp
    push 0
    sub rsp, 16

    dec rdi
    mov r8, 10

.loop:
    xor rdx, rdx
    div r8
    or dl, 0x30
    dec rdi
    mov [rdi], dl
    test rax, rax
    jnz .loop

    call print_string

    add rsp, 24
    ret

; rdi 8バイト長符号付き整数
print_int:
    test rdi, rdi
    jns print_uint
    push rdi
    mov rdi, '-'
    call print_char
    pop rdi
    neg rdi
    jmp print_uint

; rdi 文字列へのポインタ1
; rsi 文字列へのポインタ2
; rax 等しければ1、等しくなければ0を返す
string_equals:
    xor rcx, rcx
    mov rax, 1
.loop:
    mov r8b, [rdi + rcx]
    cmp r8b, [rsi + rcx]
    je .eq
    xor rax, rax
.eq:
    cmp byte [rdi + rcx], 0
    je .end
    inc rcx
    jmp .loop
.end:
    ret

read_char:
    push 0
    xor rax, rax
    xor rdi, rdi
    mov rsi, rsp
    mov rdx, 1
    syscall
    pop rax
    ret 

; rdi バッファのアドレス
; rsi バッファのサイズ
read_word:
    push r14
    push r15
    xor r14, r14 
    mov r15, rsi
    dec r15

    .A:
    push rdi
    call read_char
    pop rdi
    cmp al, ' '
    je .A
    cmp al, 10
    je .A
    cmp al, 13
    je .A 
    cmp al, 9 
    je .A
    test al, al
    jz .C

    .B:
    mov byte [rdi + r14], al
    inc r14

    push rdi
    call read_char
    pop rdi
    cmp al, ' '
    je .C
    cmp al, 10
    je .C
    cmp al, 13
    je .C 
    cmp al, 9
    je .C
    test al, al
    jz .C
    cmp r14, r15
    je .D

    jmp .B

    .C:
    mov byte [rdi + r14], 0
    mov rax, rdi 
   
    mov rdx, r14 
    pop r15
    pop r14
    ret

    .D:
    xor rax, rax
    pop r15
    pop r14
    ret

    ; rdi = source
    ; rsi = dest
    ; rdx = dest length 

; 文字列から符号なし整数への構文解析をする
; args:
;   rdi 文字列へのポインタ
; returns:
;   rax 数値
;   rdx 解析した文字列の長さ
parse_uint:
    xor rax, rax
    xor rcx, rcx
    mov r8, 10
.loop:
    movzx r9, byte [rdi + rcx]
    cmp r9b, '0'
    jb .end
    cmp r9b, '9'
    ja .end
    xor rdx, rdx
    mul r8
    add r9, -48
    add rax, r9
    inc rcx
    jmp .loop
.end:
    mov rdx, rcx
    ret

; 文字列から符号付き整数への構文解析をする
; args:
;   rdi 文字列へのポインタ
; returns:
;   rax 数値
;   rdx 解析した文字列の長さ
parse_int:
    movzx r9, byte [rdi]
    cmp r9b, '-'
    jne parse_uint
    inc rdi
    call parse_uint
    neg rax
    inc rdx
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

exit:
    mov rax, 60
    syscall