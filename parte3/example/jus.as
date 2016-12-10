tamp IS $0; taml IS $1; pcnt IS $2; ign IS $3; c IS $4; esp IS $5;
j	IS	$11
res	IS	$12
op	IS	$13
k	IS	$14
w	IS	$15
space 	IS	$16
sz	IS	$17
t	IS	$18
lim	IS 	$19
	EXTERN 	just
just	SUB	space, c, taml
	SETW	j, #0000
	SETW	w, #0707
	SETW	rX, 2	
	CMP 	res, space, 0
	JN	res, zero
	JNN	res, w1	
zero	XOR	space, space, space
w1	CMP	res, j, pcnt
	JP	res, ow1
	SETW	op, 0
	STB	op, w, 0
	ADD	w, w, 1
	ADD	j, j, 1
	JMP	w1
ow1	CMP	res, pcnt, 1
	JZ	res, pline
	SETW	w, #0707
 	SUB 	j, pcnt, 1
	ADD	w, w, j
w2	CMP	res, space, 0
	JNP	res, ow2
	LDB	op, w, 0
	ADD	op, op, 1
	STB	op, w, 0
	SUB	w, w, 1
	SUB	j, j, 1
	CMP	res, j, 0
	JZ	res, initj
	JNZ	res, subspa	
initj 	SETW	w, #0707
	SUB	j, pcnt, 1
	ADD	w, w, j
subspa	SUB	space, space, 1
	JMP	w2
ow2	XOR	j, j, j
	XOR 	k, k, k
	SETW	w, #0707
	ADD	w, w, 1
w3	CMP 	res, j, c
	JNN	res, ext
	LDB	op, k, 0
	PUSH	op
	CALL 	val
	SETW	rX, 2
	JZ	rA, pchar
	LDB	op, k, 0
	OR	rY, op, 0
	INT 	#80
	ADD	j, j, 1
	ADD	k, k, 1
	XOR	t, t, t
	LDB	op, w, 0
w4	CMP	res, t, op
	JZ	res, ow4
	XOR	rY, rY, rY
	OR	rY, rY, 32
	INT 	#80
	ADD	j, j, 1
	ADD 	t, t, 1
	JMP	w4
ow4	ADD	w, w, 1
pchar	LDB	op, k, 0
	OR	rY, op, 0
	INT	#80
	ADD	j, j, 1
	ADD	k, k, 1
	JMP	w3
pline	XOR	j, j, j
	SETW	rX, 2
prin	CMP 	res, j, taml
	JZ	res, ext
	LDB	rY, j, 0
	INT 	#80
	ADD	j, j, 1
	JMP	prin	
ext	XOR	taml, taml, taml
	OR	rY, taml, 10
	INT 	#80
	XOR	pcnt, pcnt, pcnt
	RET 0
	