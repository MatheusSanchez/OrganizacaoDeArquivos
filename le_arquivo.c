#include "le_arquivo.h"

char * le_tamanho_variavel(FILE * posicao_atual, int * tamanho_campo) {
	char atual, * palavra_lida = NULL;
	int n_letras = 0;
	pa = fopen(posicao_atual, "r");
	
	do {
		// le char a char o que está no arquivo
		atual = fgetc(pa);
		// enquanto não chega no delimitador realoca a string lida e atribui
		// o caractere à última posicao 
		if (atual != ';') {
			palavra_lida = realloc(palavra_lida, n_letras + 1);
			*(palavra_lida + n_letras) = atual;
			n_letras++;
	} while (!(feof(posicao_atual) && atual != ';');
	// delimita a string lida
	palavra_lida = realloc(palavra_lida, n_letras + 1);
	*(palavra_lida + n_letras) = '\0';
	*tamanho_campo = n_letras + 1;
	
	return palavra_lida;
}

Arquivo le_dados(char * nome_arquivo) {
	FILE * pa;
	Arquivo novo;
	int n_registros_lidos = 0;
	pa = fopen(nome_arquivo, "r");
	do {
		novo.registros_lidos = realloc(novo.registros_lidos, n_registros_lidos + 1);
		// acessa o Arquivo no vetor de registros e salva em cada elemento
		// da struct o que lhe é destinado
		fread((novo.registros_lidos + n_registros_lidos)->codigo_escola, 1, 4, pa);
		fgetc(pa);
		(novo.registros_lidos + n_registros_lidos)->nome_escola = 
				le_tamanho_variavel(pa, (novo.registros_lidos + n_registros_lidos)->indicador_tamanho_escola);
		(novo.registros_lidos + n_registros_lidos)->municipio = 
				le_tamanho_variavel(pa, (novo.registros_lidos + n_registros_lidos)->indicador_tamanho_municipio);
		(novo.registros_lidos + n_registros_lidos)->endereco = 
				le_tamanho_variavel(pa, (novo.registros_lidos + n_registros_lidos)->indicador_tamanho_endereco);
		fread((novo.registros_lidos + n_registros_lidos)->data_inicio, 1, 10, pa);
		*((novo.registros_lidos + n_registros_lidos)->(data_inicio + 10)) = '\0'
		fgetc(pa);
		fread((novo.registros_lidos + n_registros_lidos)->data_final, 1, 10, pa);
		*((novo.registros_lidos + n_registros_lidos)->(data_final + 10)) = '\0'
		fgetc(pa);
	} while (!feof(pa));
}


