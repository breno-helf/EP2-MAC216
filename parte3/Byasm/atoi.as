	EXTERN	atoi
res	IS	$12
n	IS	$51
pot	IS	$52
char	IS	$53
help	IS	$54
atoi	XOR	rA, rA, rA
	SUB	n, rSP, 16
	ADD	pot, rA, 1
calc1	CMP	res, pot, 8
	JZ	res, end1
	LDB	char, n, pot
	CMP	res, char, 48
	JN	res, end1
	CMP	res, char, 57
	JP	res, end1
	SUB	char, char, 48
	MUL	rA, rA, 10
	ADD	rA, rA, char
	ADD	pot, pot, 1
	JMP	calc1
end1	RET	1