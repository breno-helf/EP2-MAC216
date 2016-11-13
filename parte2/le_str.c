int le_str(char *s, char *errptr, SymbolTable table) {
    char *rotulo, *operador, *operand;
    Operator *op;
	Operand **opd;
    int i, j = 0, a;
	opd = malloc(3*sizeof(Operand *));
	for(i = 0; i < 3; i++)
		opd[i] = malloc(sizeof(Operand));

    rotulo = malloc(sizeof(char) * 20);
    for(i = 0; s[i] == ' '; i++);
    while (s[i] != ' ' && s[i] != '\0') {
        rotulo[j] = s[i];
        j++; i++;
    }
    rotulo[j] = '\0';
    if (rotulo(rotulo, errptr) == 0) {
		printf("Rotulo %s invalido\n", rotulo);
        return 0;
    }

	if((op = optable_find(rotulo)) != 0) {
        instr->op = op; /* A funcao optable_find ja faz isso */
		rotulo = NULL;
	}
	if(stable_find(table, rotulo) != NULL) {
		printf("Rotulo %s ja existe\n", rotulo);
		errptr = rotulo;
	}

	instr->label = rotulo;
    operador = malloc(sizeof(char) * 16);
    for(; s[i] == ' '; i++);
    j = 0;
    while (s[i] != ' ' && s[i] != '\0') {
        operador[j] = s[i];
        j++; i++;
    }

	if((op = optable_find(operador)) != 0) {
        instr->op = op;
	}

	else {
		printf("%s Nao eh operador\n", operador);
		errptr = operador;
	}

	for(; s[i] == ' '; i++);
    j = 0;
    while (s[i] != ' ' && s[i] != '\0' && s[i] != ',') {
        operand[j] = s[i];
        j++; i++;
    }
}
