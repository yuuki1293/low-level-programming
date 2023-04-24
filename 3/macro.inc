%define head_size 10

%define last_word 0

; %1 ワードの名前
; %2 ワード識別子の一部
; %3 フラグ
%macro native 3
section .data
w_ %+ %2:
    dq last_word
    db %1, 0
    db %3
xt_ %+ %2:
    dq %2 %+ _impl
section .text
%2 %+ _impl:

%define last_word w_ %+ %2
%endmacro

; %1 ワードの名前
; %2 ワード識別子の一部
%macro native 2
native %1, %2, 0
%endmacro

; %1 ワードの名前
; %2 ワード識別子の一部
; %3 フラグ
%macro colon 3
section .data
w_ %+ %2:
    dq w_ %+ last_word
    db %1, 0
    db %3

%define last_word w_ %+ %2
%endmacro

%macro colon 2
colon %1, %2, 0
%endmacro