/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: 8991769
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "mactypes.h"
#include "asmtypes.h"
#include "optable.h"
#include "opcodes.h"

/* ./maclk teste.mac abc1.maco abc2.maco */
int main (int argc, char *argv[])
{
	int i, line, hasMain;
	FILE *in, *out;
	char *infile, *execfile;
	execfile = argv[1];
	out = fopen(outfile, "w");

	hasMain = line = 0;
	for (i = 2; i < argc; i++) {
		infile = argv[i];
		in = fopen(infile, "r");

	    while (read_line(in, buffer)) {
	        char *str = buffer->data;
	        line++;

			/* if ( linha exportando main ) hasMain = 1; */

			/*
			Tenta inserir o rotulo exportado na STable
			Se ja estiver la
				die("rotulo %s definido mais de uma vez!\n", rotulo);
			*/

			fprintf(out, "%s\n", str);
	    }
	}

	if (!hasMain) die("Rotulo main nao definido!\n");

	while (read_line(out, buffer)) {
		char *str = buffer->data;
		line++;

		 /* Uma linha com rotulo nao definido comeca com "*"
		 ("* x rotulo", x == numero da linha do arquivo in?)*/
		if (str[0] == '*') {
			/*
			Rotulo nao resolvido
			Procurar rotulo na STable
			Se nao estiver la
				die("rotulo %s nao definido!\n", rotulo);
			Calcular deslocamento
			Substitui o valor na linha
			*/
		}

	}

	return 0;
}

/*
Vinculador
O vinculador é ainda mais simples de escrever do que o montador.  Ele  pode  ser
composto de apenas um arquivo maclk.c. Seu trabalho  consiste  em  ler  diversos
arquivos com código-objeto e montar o  programa  final  em  código  de  máquina.
O vinculador deve verificar se os rótulos não-resolvidos de cada  arquivo  foram
exportados por algum  outro  arquivo  para  calcular  os  deslocamentos  finais.
O vinculador também deve certificar-se de que o rótulo main  foi  exportado  por
algum arquivo, para poder fazer com  que  o  programa  comece  no  lugar  certo.
O vinculador captura três tipos de erro:
    - rótulo main não definido;
	- um mesmo rótulo definido mais de uma vez;
	- algum rótulo não definido.
*/
