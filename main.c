#include <stdlib.h>
#include <stdio.h>
#include "manipula_arquivo.h"



int main (int argc, char ** argv) {
	int funcionalidade = atoi(*(argv + 1));
	if(funcionalidade == 1){ //leitura e gravação de registro

		char * nome_arquivo;
		nome_arquivo = *(argv + 2);
		Arquivo dados_brutos = le_dados(nome_arquivo);
		arquivo_saida(&dados_brutos);
		

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
		saida = abreArquivo(arquivoSaida);

		if(saida == NULL){
			printf("Falha no processamento do arquivo.\n");
		}else if(ImprimeRegistro(saida, RRN) == false){
			printf("Registro inexistente.\n");
		}

		fclose(saida);

	}else if(funcionalidade == 5){
		
		int RRN = atoi(*(argv + 2));
		printf("%d\n", RRN);

		FILE * saida;
		saida = fopen(arquivoSaida, "r+"); // para leitura e escrita

		if(saida == NULL){
			printf("Falha no processamento do arquivo.\n");
		}else{
			RemoveRegistro(saida, RRN);	
		}		

		fclose(saida);


	}else if(funcionalidade == 6){
		// Registro p;

		// p.codEscola = atoi(*(argv+2));
		// p.data_inicio = *(argv+3);
		// p.data_final = *(argv+4);
		// p.nome_escola = *(argv+5);
		// p.municipio = *(argv+6);
		// p.endereco = *(argv+7);

		int codEscola = atoi(*(argv+2));

		FILE * saida;
		saida = fopen("saida.bin", "r+"); // para leitura e escrita

		Insercao(saida, codEscola, *(argv+3), *(argv+4), *(argv+5), *(argv+6), *(argv+7));

		fclose(saida);
	}else if(funcionalidade == 7){
		// Registro p;

		// p.codEscola = atoi(*(argv+2));
		// p.data_inicio = *(argv+3);
		// p.data_final = *(argv+4);
		// p.nome_escola = *(argv+5);
		// p.municipio = *(argv+6);
		// p.endereco = *(argv+7);

		int RRN = atoi(*(argv+2));
		int codEscola = atoi(*(argv+3));

		FILE * saida;
		saida = fopen("saida.bin", "r+"); // para leitura e escrita

		updateRegistro(saida, codEscola, *(argv+4), *(argv+5), *(argv+6), *(argv+7), *(argv+8),RRN);

		fclose(saida);
	}else if(funcionalidade == 9){

		func9();

	}else if(funcionalidade == 8){

		func8();

	}


}

