section .rodata

program_stub: dq xt_interpreter

section .text

; スタックの一番上を捨てる
; ( a -- )
native "drop", drop
    add rsp, 8
    jmp next

; ( x2 x1 -- x1 x2 )
native "swap", swap
    pop rax
    pop rcx
    push rax
    push rcx
    jmp next

; スタックの一番上を複製する
; ( a -- a a )
native "dup", dup
    pop rax
    push rax
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

; スタックの1番目と2番目を足す
; ( nu2 nu1 -- [ nu1 + nu2 ] )
native "+", plus
    pop rax
    add [rsp], rax
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

; スタックの1番目から2番目を引く
; ( nu2 nu1 -- [ nu1 - nu2 ] )
native "-", minus
    pop rax
    sub [rsp], rax
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

; nullで終わる文字列を受け取り、その長さを返す
; ( str -- len )
native "count", count
    pop rdi
    call string_length
    push rax
    jmp next

; 数をスタックからポップして出力する
; ( nu -- )
native ".", dot
    pop rdi
    call print_int
    call print_newline
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

; リターンスタックにデータスタックからプッシュ
; ( x -- )
native ">r", push_r
    add rstack, 8
    pop qword [rstack]
    jmp next

; リターンスタックからデータスタックにポップ
; ( -- x )
native "r>", pop_r
    push qword [rstack]
    sub rstack, 8
    jmp next

; リターンスタックのトップからデータスタックのトップへの、非破壊的なコピー
; ( -- x )
native "r@", fetch_r
    push qword [rstack]
    jmp next

; ヌルで終わる文字列へのポインタを受け取り、そのワードヘッダの開始アドレスを返す。
; もし、その名前のワードがなければ、ゼロを返す。
; ( str -- addr)
native "find", find
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

; スタックの1番目の文字を出力する
; ( c -- )
native "emit", emit
    pop rdi
    call print_char
    call print_newline
    jmp next

; 標準入力から文字列を読み取り、addrで始まるアドレスに格納する
; ワードの長さをスタックにプッシュする
; ( addr -- length )
native "word", word
    mov rsi, 1024
    pop rdi
    call read_word
    push rdx
    jmp next

; スタックのトップのアドレスの文字列を符号付き整数へ変換する。
; ( str -- unum length)
native "number", number
    pop rdi
    call parse_int
    push rax
    push rdx
    jmp next

; nullで終わる文字列をプリントとする
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

; システムコール
; ( call num a1 a2 a3 a4 a5 a6 -- new rax )
native "syscall", syscall
    pop r9
    pop r8
    pop r10
    pop rdx
    pop rsi
    pop rdi
    pop rax
    syscall
    push rax
    jmp next

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

; 次の命令の数値をスタックヘプッシュする
; ( -- a )
native "lit", lit
    push qword [pc]
    add pc, 8
    jmp next

; インプットバッファのアドレスをスタックに積む
; ( -- addr )
native "inbuf", inbuf
    push qword input_buf
    jmp next

; Forthマシンのメモリの開始アドレスをスタックに積む
; ( -- addr )
native "mem", mem
    push user_mem
    jmp next

; 最後のワードのアドレスをスタックにプッシュする
; ( -- addr )
native "last_word", last_word
    push qword [last_word]
    jmp next

; モードを示すstateセルのアドレスをスタックにプッシュする
; ( -- addr )
native "state", state
    push qword [state]
    jmp next

; 現在定義中のワードの最後のセルを指すポインタをスタックにプッシュする
; ( -- addr )
native "here", here
    push qword [here]
    jmp next

; スタックのトップのトークンを実行する
; ( addr -- )
native "execute", execute
    pop rax
    mov w, rax
    jmp [rax]

; addrから始まる1個のセルを読む
; ( addr -- data )
native "@", fetch
    pop rax
    push qword[rax]
    jmp next

; スタックの1番目を2番目メモリに格納する
; ( addr data -- )
native "!", write
    pop rax
    pop rcx
    mov [rcx], rax
    jmp next

; addrから始まる1個のbyteを読む
; ( addr -- c )
native "@c", fetch_c
    pop rax
    movzx rax, byte[rax]
    push rax
    jmp next

; 定義中のワードにxを追加
; ( x -- )
native ",", comma
    mov rcx, [here]
    pop qword [rcx]
    add qword [here], 8
    jmp next

; 定義中のワードに1バイトを追加
; ( c -- )
native "c,", comma_c
    mov rcx, [here]
    pop rax
    mov [here], al
    add qword [here], 1
    jmp next

; 辞書に新しい名前を持つエントリを作成する。
; ( flags name -- )
native "create", create
    mov rax, [last_word]
    mov rsi, [here]
    mov [rsi], rax
    mov [last_word], rsi
    add rsi, 8

    pop rdi
    mov rdx, 31
    push rsi
    call string_copy
    mov rdi, rax
    call string_length
    pop rsi
    add rsi, rax

    pop rax
    mov [rsi], rax

    jmp next

colon ":", colon
    dq xt_inbuf, xt_word, xt_drop
    dq xt_lit, 0, xt_inbuf
    dq xt_create
    dq xt_lit, xt_docol, xt_comma
    dq xt_state, xt_lit, 1, xt_write
    dq xt_exit

; TODO: ;

colon "interpreter", interpreter
.start:
    dq xt_inbuf, xt_word ; 標準入力から文字列を読み取る
    branch0 .exit ; 文字列が空の場合.exitにジャンプ

    dq xt_inbuf, xt_find ; 文字列のワードヘッダを探す
    dq xt_dup
    branch0 .num ; 文字列のワードヘッダがない場合.numにジャンプ

    dq xt_cfa
    dq xt_execute
    branch .start
.num:
    dq xt_drop ; 0を捨てる*2
    dq xt_drop
    dq xt_inbuf, xt_number; 数値への変換を試みる
    branch0 .not_found ; 数値への変換が失敗した場合.not_foundにジャンプ
    
    branch .start
.not_found:
    dq xt_drop
    dq xt_lit, msg_no_such_word, xt_prints ; エラーメッセージを出力
    branch .start
.exit:
    dq xt_bye
    dq xt_exit