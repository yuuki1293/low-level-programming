%include "lib.inc"
%include "macro.inc"

; 次に実行するワードを指す(アドレスのアドレス)
%define pc r15
; 現在実行中のワードを指す
%define w r14
; コロンワードを呼び出す時にpcを退避させるスタックのアドレス
%define rstack r13

section .rodata

msg_no_such_word: db ": no_such_word"

; .bssに配置されたアドレスは0で初期化される
section .bss

; pcを退避させるためのrstackを確保
; rstack用は下から伸びるため、先に1023確保しておく
resq 1023
rstack_start: resq 1

; 入力された文字列が格納される場所
input_buf: resb 1024

%include "words.asm"

section .data

; ワードリストの最後のアドレスが入る
last_word: dq _lw

section .text

global _start

; pcのワードを実行し、pcを進める
next:
    mov w, [pc]
    add pc, 8
    jmp [w]

_start: jmp init_impl