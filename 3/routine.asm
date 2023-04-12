section .text

export string_equals

; ヌルで終わる文字列へのポインタを受け取り、そのワードヘッダの開始アドレスを返す。もし、その名前のワードがなければ、ゼロを返す。
; rdi 文字列
; rax 開始アドレス
find_word:
    xor rax, rax
    mov rsi, lw
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
cfa:
