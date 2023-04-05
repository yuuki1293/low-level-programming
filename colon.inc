%define lw 0

%macro colon 2
%%lw: dq lw ; colonマクロが呼び出されるたびに%%lwはユニークな値を持つ
db %1, 0
xt_ %+ %2:  ; xt_ と %2 を連結する?

%define lw %%lw
%endmacro