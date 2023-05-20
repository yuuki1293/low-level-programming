%include "lib.inc"
%include "macro.inc"

%define pc r15      ; 次に実行するワードを指す(アドレスのアドレス)
%define w r14       ; 現在実行中のワードを指す
%define rstack r13  ; コロンワードを呼び出す時にpcを退避させるスタックのアドレス

section .rodata

msg_no_such_word: db ": no_such_word", 10, 0

section .bss

; pcを退避させるためのrstackを確保
; rstack用は下から伸びるため、先に1023確保しておく
resq 1023
rstack_start: resq 1

input_buf: resb 1024    ; 入力された文字列が格納される場所
user_mem: resq 65536    ; Forthマシンのユーザーメモリ
dict_mem: resq 65536    ; 追加の辞書のメモリ

%include "words.asm"

section .data

last_word: dq _lw   ; ワードリストの最後のアドレスが入る
stack_base: dq 0    ; スタックの一番下のアドレスが入る
state: db 0         ; 0:インタープリターモード, 1:コンパイラモード

section .text

global _start

; pcのワードを実行し、pcを進める
next:
    mov w, [pc]
    add pc, 8
    jmp [w]

_start: jmp init_impl