tamp IS $0; taml IS $1; pcnt IS $2; ign IS $3; c IS $4; esp IS $5;
     EXTERN main
main SETW   tamp, #3F3F
a    IS	    $22
j    IS	    $11
res  IS	    $12
     SETW   taml, 0
     SETW   pcnt, 0
     SETW   ign, 0
     SETW   esp, #0707
     SUB    c, rSP, 16
     LDO    c, c, 0
     LDO    c, c, 0
     PUSH   c
     CALL   atoi
     OR	    c, rA, 0
loop SETW   rX, 1
     INT    #80
     CMP    res, rA, 0
     JN	    res, end
     OR	    a, rA, 0
     PUSH   a
     CALL   val
     JZ	    rA, go
     PUSH   a
     CALL   ver
     JMP    loop
go   XOR    ign, ign, ign
     STB    a, tamp, 0
     ADD    tamp, tamp, 1
     JMP    loop
end  SETW   a, 32
     PUSH   a
     CALL   ver
     CMP    res, taml, 0
     JNP    res, fnsh
     XOR    j, j, j
     SETW   rX, 2
w5   CMP    res, j, taml
     JNN    res, ow5
     LDB    rY, j, 0
     INT    #80
     ADD    j, j, 1
     JMP    w5
ow5  SETW   rY, 10
     INT    #80
     JMP    fnsh
fnsh INT    0