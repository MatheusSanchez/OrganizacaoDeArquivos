#include <stdlib.h>
#include <stdio.h>
#include "manipula_arquivo.h"


int main (int argc, char ** argv) {
	int funcionalidade = atoi(*(argv + 1));
	

	
	printf("Função %d \n", funcionalidade);

	if(funcionalidade == 1){ //leitura e gravação de registro

		char * nome_arquivo;
		nome_arquivo = *(argv + 2);
		Arquivo dados_brutos = le_dados(nome_arquivo);
		arquivo_saida(&dados_brutos);
		//printf("Nome do Arquivo %s \n", nome_arquivo);

	}else if(funcionalidade == 2){ // exibição de todos os registros no terminal
		exibe_registros();
	}else if(funcionalidade == 3) { // EXIBIÇÃO DE TODOS OS REGISTROS COM DETERMINADO VALOR EM DETERMINANDO CAMPO
		char * nome_campo;
		nome_campo = *(argv + 2);

		FILE * saida;
		saida = fopen("saida.bin", "rb");

		func3(saida, nome_campo, *(argv+3));
		fclose(saida);

	}else if(funcionalidade == 4) {

		int RRN = atoi(*(argv + 2));
		printf("%d\n", RRN);

		FILE * saida;
		saida = fopen("saida.bin", "rb");

		ImprimeRegistro(saida,(RRN*TAMANHOREGISTRO)+T_CABECALHO);
		fclose(saida);

	}else if(funcionalidade == 5){
		int RRN = atoi(*(argv + 2));
		printf("%d\n", RRN);

		FILE * saida;
		saida = fopen("saida.bin", "r+"); // para leitura e escrita

		//printf("ola\n");
		RemoveRegistro(saida, RRN);

		fclose(saida);
	}

}
