proc	FirstProcedure

_bp-6 = 10 
_bp-2 = _bp-6 

_bp-8 = _bp-2 + 5 
_bp-4 = _bp-8 

endp	FirstProcedure

proc	SecondProcedure

endp	SecondProcedure
proc	Main

_t1 = 1 
a = _t1 

_t2 = 2 
b = _t2 

_t3 = 3 
c = _t3 

push 1
push 2
call FirstProcedure

push a
push b
call FirstProcedure

push a
push @c
call SecondProcedure

endp	Main

start proc Main

