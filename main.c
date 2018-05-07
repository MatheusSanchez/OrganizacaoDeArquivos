#include <stdlib.h>
#include <stdio.h>
#include "manipula_arquivo.h"


int main (int argc, char ** argv) {
	int funcionalidade = atoi(*(argv + 1));
	char * nome_arquivo;
	
	nome_arquivo = *(argv + 2);

	printf("Nome do Arquivo %s \n", nome_arquivo);
	printf("Função desta bilola %d \n", funcionalidade);
	
	Arquivo dados_brutos = le_dados(nome_arquivo);
	arquivo_saida(&dados_brutos);
}
