int le_str(char *s, char *errptr, SymbolTable table) {
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
    /* AQUI BRENO */
	if((op = optable_find(rotulo)) != 0) {
		for (a = 0; a < 16; a++)
            op->name[a] = rotulo[a];
        op->opcode = 
        op->opd_types[0] = 
        op->opd_types[1] = 
        op->opd_types[2] = 
        instr->op = op
		rotulo = NULL;
		
	}
	if(stable_find(table, rotulo) != NULL) {
		print_error_msg("Rotulo ja existe");
		errptr = rotulo;
	}
	/*acaba aqui*/
	instr->label = rotulo;
    operador = malloc(sizeof(char) * 16);
    for(; s[i] == ' '; i++);
    j = 0;
    while (s[i] != ' ' && s[i] != '\0') {
        operador[j] = s[i];
        j++; i++;
    }
	if((op = optable_find(rotulo)) != 0) {
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
