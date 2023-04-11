%define lw 0

; %1 ワードの名前
; %2 ワード識別子の一部
; %3 フラグ
%macro native 3
section .data
w_ %+ %1:
    dq lw
    db %2, 0
    db %3
xt_ %+ %1:
    dq %1 %+ _impl
section .text
%1 %+ _impl:

%define lw w_ %+ %1
%endmacro

; %1 ワードの名前
; %2 ワード識別子の一部
%macro native 2
native %1, %2, 0
%endmacro