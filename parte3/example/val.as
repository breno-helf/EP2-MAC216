     EXTERN val
val  SUBU   rX, rSP, 16
     LDOU   rX, rX, 0
aux  IS     $10
     SUB    aux, rX, 9
     JZ	    aux, yes
     SUB    aux, rX, 10
     JZ	    aux, yes
     SUB    aux, rX, 13
     JZ	    aux, yes
     SUB    aux, rX, 32
     JZ     aux, yes
     JMP    no
yes  XOR    rA, rA, rA
     OR	    rA, rA, 1
     RET    1
no   XOR    rA, rA, rA
     RET    1
