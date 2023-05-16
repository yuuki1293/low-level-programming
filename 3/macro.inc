%define head_size 10

%define _lw 0

; %1 ワードの名前
; %2 ワード識別子の一部
; %3 フラグ
%macro native 3
section .data
w_ %+ %2:
    dq _lw
    db %1, 0
    db %3
xt_ %+ %2:
    dq %2 %+ _impl
section .text
%2 %+ _impl:

%define _lw w_ %+ %2
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
    dq _lw
    db %1, 0
    db %3
xt_ %+ %2 : dq docol_impl

%define _lw w_ %+ %2
%endmacro

%macro colon 2
colon %1, %2, 0
%endmacro

%macro branch 1
dq xt_branch
dq %1 - $ - 8
%endmacro

%macro branch0 1
dq xt_branch0
dq %1 - $ - 8
%endmacro