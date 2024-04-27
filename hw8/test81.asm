.MODEL SMALL
.586
.STACK 100h
.DATA
	a	DW	?
	b	DW	?
	d	DW	?
	t1	DW	?
	t2	DW	?
	t3	DW	?
.CODE
include io.asm

one    PROC
push bp
mov bp , sp
sub sp , 3


mov AX , 5
mov t1 , AX

mov AX , t1
mov a , AX

mov AX , 10
mov t2 , AX

mov AX , t2
mov b , AX
mov AX, a
mov BX, b
imul BX
mov t3 , AX

mov AX , t3
mov d , AX
mov ax , d
call writeint
call writeln

add sp , 3
pop bp
ret 0
one    ENDP

main	PROC
mov AX, @data
mov DS, AX

call one

mov ah, 04ch
int 21h
main	ENDP
END	main

