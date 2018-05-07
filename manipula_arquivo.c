#include "manipula_arquivo.h"
#include <string.h>

char * le_tamanho_variavel(FILE * posicao_atual, int * tamanho_campo) {
	char atual, *palavra_lida = NULL;
	int n_letras = 0;
	FILE * pa;
	pa = posicao_atual;
	
	do {
		// le char a char o que está no arquivo
		atual = fgetc(pa);

		// enquanto não chega no delimitador realoca a string lida e atribui
		// o caractere à última posicao 

		if (atual != ';') {
			palavra_lida = realloc(palavra_lida, n_letras + 1);
			*(palavra_lida + n_letras) = atual;
			n_letras++;
		}

	} while ((atual != ';'));
	

	// delimita a string lida

	if (n_letras == 0) {
		palavra_lida = realloc(palavra_lida, n_letras + 1);
		*(palavra_lida + n_letras) = ';';
		*tamanho_campo = n_letras ;
	} else {
		palavra_lida = realloc(palavra_lida, n_letras + 1);
		*(palavra_lida + n_letras) = '\0';
		*tamanho_campo = n_letras ;
	}
	
	printf("%s%c \n", palavra_lida,atual);

	return palavra_lida;
}

Arquivo le_dados(char * nome_arquivo) {

	FILE * pa = NULL;


	Arquivo novo;
	int n_registros_lidos = 0;
	char auxiliar;
	pa = fopen(nome_arquivo, "r");
	fseek(pa, 0, SEEK_END);
	int tamanho_arquivo = ftell(pa);
	fseek(pa, 0, SEEK_SET);
	novo.registros_lidos = NULL;
	if(pa == NULL){
		printf("Erro ao abrir o arquivo, lamento !\n");
		exit(0);
	}

	while (ftell(pa) < tamanho_arquivo){

		printf("to aqui meu amor\n");
		novo.registros_lidos = realloc(novo.registros_lidos, (n_registros_lidos + 1) * sizeof(Registro));
		
		// acessa o Arquivo no vetor de registros e salva em cada elemento
		// da struct o que lhe é destinado

		fscanf(pa,"%d",&((novo.registros_lidos + n_registros_lidos)->codigo_escola));
		fgetc(pa);

		// substituido
		// fread(&((novo.registros_lidos + n_registros_lidos)->codigo_escola), 1, sizeof(int), pa);

		(novo.registros_lidos + n_registros_lidos)->nome_escola = 
				le_tamanho_variavel(pa, &((novo.registros_lidos + n_registros_lidos)->indicador_tamanho_escola));
		(novo.registros_lidos + n_registros_lidos)->municipio = 
				le_tamanho_variavel(pa, &((novo.registros_lidos + n_registros_lidos)->indicador_tamanho_municipio));
		(novo.registros_lidos + n_registros_lidos)->endereco = 
				le_tamanho_variavel(pa, &((novo.registros_lidos + n_registros_lidos)->indicador_tamanho_endereco));

		auxiliar = fgetc(pa);

		if (auxiliar == ';') {

			strcpy(((novo.registros_lidos) + n_registros_lidos)->data_inicio, "0000000000\0");
			printf("data ini bugou%s\n", (novo.registros_lidos + n_registros_lidos)->data_inicio);

		} else {

			(novo.registros_lidos + n_registros_lidos)->data_inicio[0] = auxiliar;
			fread((novo.registros_lidos + n_registros_lidos)->data_inicio+1, 1, 9, pa);	
			(novo.registros_lidos + n_registros_lidos)->data_inicio[10] = '\0';
			fgetc(pa);	
			printf("data ini %s\n", (novo.registros_lidos + n_registros_lidos)->data_inicio);
		}


		auxiliar = fgetc(pa);

		if (auxiliar == '\n') {	
			strcpy(((novo.registros_lidos) + n_registros_lidos)->data_final, "0000000000\0");
			printf("data fim bugou %s\n", (novo.registros_lidos + n_registros_lidos)->data_final);	

		} else {

			(novo.registros_lidos + n_registros_lidos)->data_final[0] = auxiliar;
			fread((novo.registros_lidos + n_registros_lidos)->data_final+1, 1, 9, pa);	
			((novo.registros_lidos + n_registros_lidos)->data_final[10]) = '\0';
			auxiliar = fgetc(pa);
			printf("data fim %s\n", (novo.registros_lidos + n_registros_lidos)->data_final);
		}

		//printf("%d\n", 	fgetc(pa));
		//fgetc(pa);


		n_registros_lidos++;
		
	}
	novo.n_registros_lidos = n_registros_lidos;
	return novo;
}

// FILE * arquivo_saida(Arquivo *entrada) {


// 	FILE * saida;
// 	int disponiveis, i, j, cabo = TABOM; // 0

// 	saida = fopen("Saida.csv", "w+");
// 	//cabecalho
// 	//fprintf(saida, "%c %d;", entrada.status, entrada.topo_pilha);
// 	printf("%d\n",entrada->n_registros_lidos);

// 	for (int i = 0; i < entrada->n_registros_lidos; ++i){
// 		printf("VOU GRAVAR ESSA PORRA\n");

// 		fprintf(saida, "%d", entrada->registros_lidos[i].codigo_escola); // gravando codigo da escola	
// 		fprintf(saida, "%s%s", entrada->registros_lidos[i].data_inicio,entrada->registros_lidos[i].data_final); // gravando as datas	
// 		fprintf(saida, "%d", entrada->registros_lidos[i].indicador_tamanho_escola); // gravando indicador de tamanho do nome da escola
// 		fprintf(saida, "%s", entrada->registros_lidos[i].nome_escola); // gravando nome da escola
// 		fprintf(saida, "%d", entrada->registros_lidos[i].indicador_tamanho_municipio); // gravando indicador de tamanho do nome da escola
// 		fprintf(saida, "%s", entrada->registros_lidos[i].municipio); // gravando nome da escola
// 		fprintf(saida, "%d", entrada->registros_lidos[i].indicador_tamanho_endereco); // gravando indicador de tamanho do nome da escola
// 		fprintf(saida, "%s", entrada->registros_lidos[i].endereco); // gravando nome da escola


// 	}		
// 	return saida;
// }

FILE * arquivo_saida(Arquivo *entrada) {


	FILE * saida;
	int disponiveis, i, j, cabo = TABOM; // 0

	saida = fopen("Saida.txt", "w+");
	//cabecalho
	//fprintf(saida, "%c %d;", entrada.status, entrada.topo_pilha);
	//printf("%d\n",entrada->n_registros_lidos);

	for (int i = 0; i < entrada->n_registros_lidos; ++i){
		

		fwrite(&(entrada->registros_lidos[i].codigo_escola), sizeof(int), 1, saida);
		fwrite(&entrada->registros_lidos[i].data_inicio, sizeof(char), 10, saida);
		fwrite(&entrada->registros_lidos[i].data_final, sizeof(char), 10, saida);
		fwrite(&entrada->registros_lidos[i].indicador_tamanho_escola, sizeof(int), 1, saida);
		fwrite(&entrada->registros_lidos[i].nome_escola, sizeof(char), entrada->registros_lidos->indicador_tamanho_escola, saida);
		fwrite(&entrada->registros_lidos[i].indicador_tamanho_municipio, sizeof(int), 1, saida);
		fwrite(&entrada->registros_lidos[i].municipio, sizeof(char), entrada->registros_lidos[i].indicador_tamanho_municipio, saida);
		fwrite(&entrada->registros_lidos[i].indicador_tamanho_endereco, sizeof(int), 1, saida);
		fwrite(&entrada->registros_lidos[i].endereco, sizeof(char), entrada->registros_lidos[i].indicador_tamanho_endereco, saida);
		
	}		
	return saida;
}