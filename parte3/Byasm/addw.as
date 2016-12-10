tamp IS $0; taml IS $1; pcnt IS $2; ign IS $3; c IS $4;
        EXTERN	addw
j	IS	$11
res	IS	$12
op	IS	$13
addw	SETW	j, #3F3F
	CMP	res, taml, 0
	JP 	res, addtl
	JMP	w
addtl	ADD	taml, taml, 1
w	CMP 	res, j, tamp
	JNN	res, ow
	LDB	op, j, 0
	STB	op, taml, 0
	ADD 	taml, taml, 1
	ADD 	j, j, 1
	JMP	w
ow	SETW	tamp, #3F3F
	SETW	op, 32
	STB	op, taml, 0	
	ADD	pcnt, pcnt, 1
	RET	0