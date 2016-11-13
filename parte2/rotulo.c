int check_carac (char c) {
/*checa se o caracter é letra, número ou _, devolve 1 se sim, 0 caso contrário*/
    if ((c > 64 && c < 91) || (c == 95) || (c > 96 && c < 123) || (c > 47 && c < 58))
        return 1;
    return 0;
}

int rotulo(char *stg, char *errptr) {
    int i = 1, j;
    if (!(check_carac(stg[0])) {
        errptr = &stg[0];
		printf("Econtrado char %c que nao eh numero, letra ou underscore\n", stg[0]);
		return 0;
    }
    while (stg[i] != '\0') {
        j = check_carac(stg[i]);
        if (!j) {
			errptr = &stg[i];
			printf("Encontrado char %c que nao eh numero, letra ou underscore", stg[i]);
			return 0;
        }
        i++;
    }
    return 1;
}
