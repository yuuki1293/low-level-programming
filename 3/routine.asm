section .text

export string_equals

%define pc r15
%define w r14
%define rstack r13

section .bss
resq 1023
rstack_start: resq 1
input_buf: resb 1024

section .text

; ヌルで終わる文字列へのポインタを受け取り、そのワードヘッダの開始アドレスを返す。もし、その名前のワードがなければ、ゼロを返す。
; rdi 文字列
; rax 開始アドレス
native "find_word" find_word
    xor rax, rax
    mov rsi, last_word
.loop:
    test rsi, rsi
    jz .end
    push rsi
    add rsi, 8
    push rdi
    call string_equals
    pop rdi
    pop rsi
    test rax, rax
    jnz .found
    mov rsi, [rsi]
    jmp .loop
.found:
    mov rax, rsi
.end:
    ret

; ワードヘッダの開始アドレスを受け取り、実行トークン（XT）の値に到達するまでヘッダ全体をスキップする。
; rdi ワードヘッダの開始アドレス
native "cfa", cfa
    pop rsi
    add rsi, 9
    .loop:
    mov al, [rsi]
    test al, al
    jz .end
    inc rsi
    jmp .loop

    .end:
    add rsi, 2
    push rsi
    jmp next