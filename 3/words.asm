section .rodata

program_stub: dq xt_interpreter

section .text

; ヌルで終わる文字列へのポインタを受け取り、そのワードヘッダの開始アドレスを返す。
; もし、その名前のワードがなければ、ゼロを返す。
; args:
;   文字列へのポインタ
; returns:
;   ワードヘッダの開始アドレス
native "find_word", find_word
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
; args:
;   ワードヘッダの開始アドレス
; returns:
;   実行トークンのアドレス
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

; スタックの一番上を捨てる
native "drop", drop
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
native "exit", exit
    mov pc, [rstack]
    add rstack, 8
    jmp next

; 標準入力から文字列を読み取る。
; args:
;   文字列の保存先のアドレス
; returns:
;   文字列の長さ
native "word", word
    ; mov rsi, 1024 ; TODO: なぜ文字列の長さを設定しなくても良いのか
    pop rdi
    call read_word
    push rdx
    jmp next

; スタックのトップのアドレスの文字列をプリントする。
; args:
;   文字列のアドレス
native "prints", prints
    pop rdi
    call print_string
    jmp next

; プログラムを終了させる
native "bye", bye
    mov rax, 60
    xor rdi, rdi
    syscall

; インプットバッファのアドレスをスタックに積む
; returns:
;   インプットバッファのアドレス
native "inbuf", inbuf
    push qword input_buf
    jmp next

; スタックのトップのアドレスの文字列を符号付き整数へ変換する。
; args:
;   文字列のアドレス
; returns:
;   数値
;   変換された文字列の長さ
native "number", number
    pop rdi
    call parse_int
    push rax
    push rdx
    jmp next

; 次の命令の数値をスタックヘプッシュする
native "lit", lit
    push qword [pc]
    add pc, 8
    jmp next

; スタックのトップのトークンを実行する
; args:
;   xtのアドレス
native "execute", execute
    pop rax
    mov w, rax
    jmp [rax]

; 次に配置されている数値だけpcを進める。
; コンパイル時のみ
native "branch", branch
    add pc, [pc]
    add pc, 8
    jmp next

; スタックのトップが0の場合、次に配置されている数値だけpcを進める。
; コンパイル時のみ
; args:
;   数値
native "0branch", branch0
    pop rax
    test rax, rax
    jnz .skip
    add pc, [pc]
.skip:
    add pc, 8
    jmp next

colon "interpreter", interpreter
.start:
    dq xt_inbuf, xt_word ; 標準入力から文字列を読み取る
    branch0 .exit ; 文字列が空の場合.exitにジャンプ

    dq xt_drop ; 文字列の長さを捨てる
    dq xt_inbuf, xt_find_word ; 文字列のワードヘッダを探す
    branch0 .num ; 文字列のワードヘッダがない場合.numにジャンプ

    dq xt_cfa
    dq xt_execute
    branch .start
.num:
    dq xt_drop ; 0を捨てる
    dq xt_inbuf, xt_number ; 数値への変換を試みる
    branch0 .not_found ; 数値への変換が失敗した場合.not_foundにジャンプ

    dq xt_drop
    branch .start
.not_found:
    dq xt_drop, xt_drop
    dq xt_lit, msg_no_such_word, xt_prints ; エラーメッセージを出力
    branch .start
.exit:
    dq xt_bye
    dq xt_exit