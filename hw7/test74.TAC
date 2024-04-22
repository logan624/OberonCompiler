proc	one

_bp-6 = 5 
a = _bp-6 

_bp-8 = 10 
b = _bp-8 

_bp-10 = 20 
_bp-4 = _bp-10 

_bp-12 = _bp-4 + a 
_bp-14 = _bp-12 * b 
_bp-2 = _bp-14 

endp	one

proc	four

call one

endp	four

start proc four
