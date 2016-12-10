tamp IS $0; taml IS $1; pcnt IS $2; ign IS $3; c IS $4; esp IS $5;
     EXTERN ver
j    IS	    $11
res  IS	    $12
op   IS	    $13
b    IS	    $20
szp  IS	    $21
ver  SUB    b, rSP, 16
     LDO    b, b, 0
     SETW   op, #3f3f
     SUB    szp, tamp, op
ini  CMP    res, szp, 0
     JNP    res, par
     CMP    res, taml, 0
     JZ	    res, sC
pC   ADD    op, taml, szp
     ADD    op, op, 1
     CMP    res, op, c
     JP	    res, sCpC
pCpC CALL   addw
     JMP    par
sCpC CALL   just
     CALL   addw
     JMP    par
sC   ADD    op, taml, szp
     CMP    res, op, c
     JP	    res, sCsC
pCsC CALL   addw
     JMP    par
sCsC CALL   just
     CALL   addw
     JMP    par
par  CMP    res, b, 10
     JNZ    res, out
     ADD    ign, ign, 1
     CMP    res, ign, 2
     JNZ    res, out
     XOR    j, j, j
     SETW   rX, 2
w6   CMP    res, j, taml
     JNN    res, ow6
     LDB    rY, j, 0
     INT    #80
     ADD    j, j, 1
     JMP    w6
ow6  XOR    taml, taml, taml
     XOR    pcnt, pcnt, pcnt
     SETW   rY, 10
     INT    #80
     INT    #80
     SUB    ign, ign, 255
     JMP    out
out  RET    1