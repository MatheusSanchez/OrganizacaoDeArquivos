#include <stdlib.h>
#include <stdio.h>
#include "manipula_arquivo.h"


int main (int argc, char ** argv) {
	int funcionalidade = atoi(*(argv + 1));
	char * nome_arquivo;
	
	nome_arquivo = *(argv + 2);

	printf("Nome do Arquivo %s \n", nome_arquivo);
	printf("Função %d \n", funcionalidade);

	if(funcionalidade == 1){ //leitura e gravação de registro
		Arquivo dados_brutos = le_dados(nome_arquivo);
		arquivo_saida(&dados_brutos);

	}else if(funcionalidade == 2){ // exibição de todos os registros no terminal
		exibe_registros();
	}
	
	


}
