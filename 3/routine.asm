section .text

export string_equals

section .bss

input_buf: resb 1024

section .text

; ヌルで終わる文字列へのポインタを受け取り、そのワードヘッダの開始アドレスを返す。もし、その名前のワードがなければ、ゼロを返す。
native "find_word" find_word
    mov rsi, [last_word]
.loop:
    mov rdi, [rsp]

; ワードヘッダの開始アドレスを受け取り、実行トークン（XT）の値に到達するまでヘッダ全体をスキップする。
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