#ifndef __LE_ARQUIVO_H__
#define __LE_ARQUIVO_H__

#include <stdlib.h>
#include <stdio.h>

struct registro {
	int codigo_escola;
	char data_inicio[11];
	char data_final[11];
	int indicador_tamanho_escola;
	char * nome_escola;
	int indicador_tamanho_municipio;
	char * municipio;
	int indicador_tamanho_endereco;
	char * endereco;
};

typedef struct registro Registro;

struct arquivo {
	// pode ser 0 se o arquivo foi corrompido ou 1 se está íntegro
	int n_registros_lidos;
	char status;
	int topo_pilha;
	Registro * registros_lidos = NULL;
};

typedef struct arquivo Arquivo;

char * le_tamanho_variavel(FILE * posicao_atual);

Arquivo le_dados(char * nome_arquivo);




#endif
