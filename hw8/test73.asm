.MODEL SMALL
.586
.STACK 100h
.DATA
	a	DW	?
	b	DW	?
	d	DW	?
	e	DW	?
	t1	DW	?
	t2	DW	?
	t3	DW	?
	t4	DW	?
	t5	DW	?
.CODE
include io.asm

three    PROC
push bp
mov bp , sp
sub sp , 5


mov AX , 5
mov t1 , AX

mov AX , t1
mov a , AX

mov AX , 10
mov t2 , AX

mov AX , t2
mov b , AX

mov AX , 20
mov t3 , AX

mov AX , t3
mov d , AX
mov AX, d
add AX, a
mov t4 , AX
mov AX, t4
mov BX, b
imul BX
mov t5 , AX

mov AX , t5
mov e , AX
mov ax , e
call writeint

add sp , 5
pop bp
ret 0
three    ENDP

main	PROC
mov AX, @data
mov DS, AX

call three

mov ah, 04ch
int 21h
main	ENDP
END	main

