section .rodata

program_stub: dq xt_interpreter

section .text

; ヌルで終わる文字列へのポインタを受け取り、そのワードヘッダの開始アドレスを返す。
; もし、その名前のワードがなければ、ゼロを返す。
; ( str -- addr)
native "find_word", find_word
    mov rsi, [last_word]
.loop:
    test rsi, rsi
    jz .not_found
    mov rdi, [rsp]
    push rsi
    add rsi, 8
    call string_equals
    pop rsi
    test rax, rax
    jnz .found
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
; ( addr -- addr )
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
; ( a -- )
native "drop", drop
    add rsp, 8
    jmp next

; プログラムが実行されたときに最初に呼ばれるルーチン
; レジスタの初期化等を行う
native "init", init
    mov rstack, rstack_start
    mov pc, program_stub
    mov [stack_base], rsp
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
; ( addr -- length )
native "word", word
    ; mov rsi, 1024 ; TODO: なぜ文字列の長さを設定しなくても良いのか
    pop rdi
    call read_word
    push rdx
    jmp next

; スタックのトップのアドレスの文字列をプリントする。
; ( str -- )
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
; ( -- addr )
native "inbuf", inbuf
    push qword input_buf
    jmp next

; スタックのトップのアドレスの文字列を符号付き整数へ変換する。
; ( str -- unum length)
native "parse_nu", parse_nu
    pop rdi
    call parse_int
    push rax
    push rdx
    jmp next

; 次の命令の数値をスタックヘプッシュする
; ( -- a )
native "lit", lit
    push qword [pc]
    add pc, 8
    jmp next

; スタックのトップのトークンを実行する
; ( addr -- )
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
; ( a -- )
native "0branch", branch0
    pop rax
    test rax, rax
    jnz .skip
    add pc, [pc]
.skip:
    add pc, 8
    jmp next

; スタックの一番上を複製する
; ( a -- a a )
native "dup", dup
    pop rax
    push rax
    push rax
    jmp next

; Forthマシンのメモリの開始アドレスをスタックに積む
; ( -- addr )
native "mem", mem
    push forth_mem
    jmp next

; スタックの数を全てプリントする
native ".S", show_stack
    mov rcx, rsp
.loop:
    cmp rcx, [stack_base]
    jae next
    mov rdi, [rcx]
    push rcx
    call print_int
    call print_newline
    pop rcx
    add rcx, 8
    jmp .loop

; スタックの1番目と2番目を足す
; ( nu2 nu1 -- [ nu1 + nu2 ] )
native "+", plus
    pop rax
    add [rsp], rax
    jmp next

; スタックの1番目から2番目を引く
; ( nu2 nu1 -- [ nu1 - nu2 ] )
native "-", minus
    pop rax
    sub [rsp], rax
    jmp next

; スタックの1番目に2番目を掛ける
; ( nu2 nu1 -- [ nu1 * nu2 ] )
native "*", mul
    pop rax
    imul qword [rsp]
    mov [rsp], rax
    jmp next

; スタックの1番目を2番目で割る
; ( nu2 nu1 -- [ nu1 / nu2 ] )
native "/", div
    pop rcx
    pop rax
    xor rdx, rdx
    idiv rcx
    push rax
    jmp next

; スタックの1番目と2番目が等しいか比較する
; ( nu2 nu1 -- [ nu1 = nu2 ] )
native "=", eq
    pop rax
    pop rcx
    cmp rax, rcx
    je .e
    push 0
    jmp next
.e:
    push 1
    jmp next

; スタックの1番目が2番目より小さいか調べる
; ( nu2 nu1 -- [ nu1 < nu2 ] )
native "<", less
    pop rcx
    pop rax
    cmp rax, rcx
    jl .l
    push 0
    jmp next
.l:
    push 1
    jmp next

; スタックの1番目と2番目の論理積を取る
; ( x2 x1 -- [ x1 & x2 ] )
native "and", and
    pop rcx
    pop rax
    test rcx, rcx
    setnz cl
    test rax, rax
    setnz al
    and al, cl
    movzx rax, al
    push rax
    jmp next

; スタックの1番目の論理否定を取る
; ( x -- !x )
native "not", not
    pop rax
    test rax, rax
    setz al
    movzx rax, al
    push rax
    jmp next

; ( x3 x2 x1 -- x2 x1 x3 )
native "rot", rot
    pop rax
    pop rcx
    pop rdx
    push rcx
    push rax
    push rdx
    jmp next

; ( x2 x1 -- x1 x2 )
native "swap", swap
    pop rax
    pop rcx
    push rax
    push rcx
    jmp next

; 数をスタックからポップして出力する
; ( nu -- )
native ".", dot
    pop rdi
    call print_int
    call print_newline
    jmp next

; stdinから1文字を読む
; ( -- c )
native "key", key
    call read_char
    push rax
    jmp next

; スタックの1番目の文字を出力する
; ( c -- )
native "emit", emit
    pop rdi
    call print_char
    call print_newline
    jmp next

; stdinから符号付き整数を読む
; ( -- nu )
colon "number", number
    dq xt_inbuf
    dq xt_word
    dq xt_drop
    dq xt_inbuf
    dq xt_parse_nu
    dq xt_drop
    dq xt_exit

; スタックの1番目を2番目メモリに格納する
; ( addr data -- )
native "!", write
    pop rax
    pop rcx
    mov [rcx], rax
    jmp next

colon "interpreter", interpreter
.start:
    dq xt_inbuf, xt_word ; 標準入力から文字列を読み取る
    branch0 .exit ; 文字列が空の場合.exitにジャンプ

    dq xt_inbuf, xt_find_word ; 文字列のワードヘッダを探す
    dq xt_dup
    branch0 .num ; 文字列のワードヘッダがない場合.numにジャンプ

    dq xt_cfa
    dq xt_execute
    branch .start
.num:
    dq xt_drop ; 0を捨てる*2
    dq xt_drop
    dq xt_inbuf, xt_parse_nu; 数値への変換を試みる
    branch0 .not_found ; 数値への変換が失敗した場合.not_foundにジャンプ
    
    branch .start
.not_found:
    dq xt_drop
    dq xt_lit, msg_no_such_word, xt_prints ; エラーメッセージを出力
    branch .start
.exit:
    dq xt_bye
    dq xt_exit