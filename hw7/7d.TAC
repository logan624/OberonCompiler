proc	test1

_bp-2 = _bp+6 * _bp+8 
_bp-4 = _bp-2 + 30 
@_bp+4 = _bp-4 

endp	test1

proc	main

_t1 = 10 
a = _t1 

_t2 = 20 
b = _t2 

push a
push b
push c
call test1

endp	main

start proc main

