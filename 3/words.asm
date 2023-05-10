section .text

export string_equals
export read_word

section .rodata

program_stub: dq xt_interpreter

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

colon "interpreter" interpreter

; スタックの一番上を捨てる
native "drop" drop
    add rsp, 8
    jmp next

; プログラムが実行されたときに最初に呼ばれるルーチン
; レジスタの初期化等を行う
native "init", init
    mov rstack, rstack_start
    mov pc, program_stub
    jmp next

; 全てのコロンワードの開始
native "docol", docol
    sub rstack, 8
    mov [rstack], pc
    add w, 8
    mov pc, w
    jmp next

; 全てのコロンワードの終わり
native "exit" exit
    mov pc, [rstack]
    add rstack, 8
    jmp next

; 標準入力から文字列を読み取る。
; args:
;   1 - 文字列の保存先のアドレス
; return:
;   1 - 文字列の長さ
native "word", word
    ; mov rsi, 1024 ; TODO: なぜ文字列の長さを設定しなくても良いのか
    pop rdi
    call read_word
    push rdx
    jmp next

native "inbuf" inbuf
    push qword input_buf
    jmp next

; 次に配置されている数値だけpcを進める。
native "branch" branch
    add pc, [pc]
    add pc, 8
    jmp next