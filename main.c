#include <stdlib.h>
#include <stdio.h>
#include "le_arquivo.h"


int main (int argc, char ** argv) {
	int funcionalidade = atoi(*(argv + 1));
	char * nome_arquivo;
	char *** dados_brutos;
	
	nome_arquivo = *(argv + 2);
	
	dados_brutos = le_dados(nome_arquivo);
	
}
