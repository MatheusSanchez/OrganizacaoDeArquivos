#include <stdlib.h>
#include <stdio.h>
#include "manipula_arquivo.h"


int main (int argc, char ** argv) {
	int funcionalidade = atoi(*(argv + 1));
	char * nome_arquivo;
	
	nome_arquivo = *(argv + 2);
	
	dados_brutos = le_dados(nome_arquivo);
	
}
