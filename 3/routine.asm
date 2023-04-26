section .text

export string_equals

section .text

; ヌルで終わる文字列へのポインタを受け取り、そのワードヘッダの開始アドレスを返す。もし、その名前のワードがなければ、ゼロを返す。
native "find_word" find_word
    mov rsi, [last_word]
.loop:
    test rsi, rsi
    jz .not_found
    mov rdi, [rsp]
    push rsi
    add rsi, 9 ; TODO: 8ではなく9
    call string_equals
    pop rsi
    test rax, rax
    jz .found
    mov rsi, [rsi]
    jmp .loop
.not_found:
    mov qword [rsp], 0
    push 0 ; TODO: なぜpushする？
    jmp next
.found:
    mov [rsp], rsi
    jmp next

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