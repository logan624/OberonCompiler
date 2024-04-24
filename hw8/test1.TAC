proc	FirstProcedure

_bp-6 = 10 
_bp-2 = _bp-6 

_bp-8 = _bp-2 + 5 
_bp-4 = _bp-8 

endp	FirstProcedure

proc	SecondProcedure

_bp-6 = 20 
_bp-2 = _bp-6 

_bp-8 = _bp-2 * 2 
_bp-4 = _bp-8 

endp	SecondProcedure

proc	ThirdProcedure

_bp-6 = 30 
_bp-2 = _bp-6 

_bp-8 = _bp-2 - 5 
_bp-4 = _bp-8 

endp	ThirdProcedure

proc	Main

_t1 = 1 
a = _t1 

_t2 = 2 
b = _t2 

_t3 = 3 
c = _t3 

call FirstProcedure

call SecondProcedure

call ThirdProcedure

endp	Main

start proc Main

