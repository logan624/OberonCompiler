Proc	test1

_bp6 := _bp+6 * _bp+4 
@_bp+8 := _bp6 + 30 

Endp	test1

Proc	main

a := 10 

b := 20 

push a
push b
push c
Call test1

Endp	test1

Start proc main

