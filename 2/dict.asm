global find_word
extern string_equals

section .text

; rdi nullで終結するキー文字列へのポインタ
; rsi 辞書の最後のワードへのポインタ
; rax 該当レコードが見つからなければ0、見つかったらそのレコードのアドレスを返す
find_word:
    xor rax, rax
.loop:
    test rsi, rsi
    jz .end
    push rdi
    push rsi
    add rsi, 8
    call string_equals
    pop rsi
    pop rdi
    test rax, rax
    jnz .found
    mov rsi, [rsi]
    jmp .loop
.found:
    mov rax, rsi
.end:
    ret