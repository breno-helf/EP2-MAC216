int le_str(char *s, char *errptr) {
    char *rotulo, *operador;
    Operator *op;
    int i, j = 0, a;
    rotulo = malloc(sizeof(char) * 20);
    for(i = 0; s[i] == ' '; i++);
    while (s[i] != ' ' && s[i] != '\0') {
        rotulo[j] = s[i];
        j++; i++;
    }
    rotulo[j] = '\0';
    if (rotulo(rotulo, errptr) == 0) {      
		print_error_msg("Rotulo invalido");
        return 0;
    }
	
    /*se é operador:
        rotulo = NULL
        preenche operador
    se rótulo já existe:
        mensagem de erro
        errprt*/

	if((op = optable_find(rotulo)) != 0) {
		rotulo = NULL;
		/*
		  preenche operador (?)
		*/
	} else {
		print_error_msg("Rotulo ja existe");
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
    /*se é operador:*/
        for (a = 0; a < 16; a++)
            op->name[a] = operador[a];
        op->opcode = 
        op->opd_types[0] = 
        op->opd_types[1] = 
        op->opd_types[2] = 
        instr->op = op
    /*se não é operador:
        mensagem de erro
        errprt*/
}
