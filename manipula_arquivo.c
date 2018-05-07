#include "manipula_arquivo.h"

char * le_tamanho_variavel(FILE * posicao_atual, int * tamanho_campo) {
	char atual, *palavra_lida = NULL;
	int n_letras = 0;
	FILE * pa;
	pa = posicao_atual;
	
	do {
		// le char a char o que está no arquivo
		atual = fgetc(pa);
		printf("ASDDASHSADIJHASD\n");
		// enquanto não chega no delimitador realoca a string lida e atribui
		// o caractere à última posicao 
		if (atual != ';') {
			palavra_lida = realloc(palavra_lida, n_letras + 1);
			*(palavra_lida + n_letras) = atual;
			n_letras++;
		}
	} while (!(feof(posicao_atual) && atual != ';'));
	// delimita a string lida
	if (n_letras == 0) {
		palavra_lida = realloc(palavra_lida, n_letras + 1);
		*(palavra_lida + n_letras) = ';';
		*tamanho_campo = n_letras + 1;
	} else {
		palavra_lida = realloc(palavra_lida, n_letras + 1);
		*(palavra_lida + n_letras) = '\0';
		*tamanho_campo = n_letras + 1;
	}
	
	return palavra_lida;
}

Arquivo le_dados(char * nome_arquivo) {

	FILE * pa = NULL;
	Arquivo novo;
	int n_registros_lidos = 0;
	char auxiliar;
	pa = fopen(nome_arquivo, "r");
	novo.registros_lidos = NULL;
	if(pa == NULL){
		printf("CABOU TUDO AQUI\n");
		exit(0);
	}

	do {
		printf("VAI DESGRACA\n");

		novo.registros_lidos = realloc(novo.registros_lidos, n_registros_lidos + 1);
		printf("MAROTAOTOAOTOAOTAOO\n");
		// acessa o Arquivo no vetor de registros e salva em cada elemento
		// da struct o que lhe é destinado
		fread(&((novo.registros_lidos + n_registros_lidos)->codigo_escola), sizeof(int), 1, pa);
		
		printf("cODIGO %d %c\n",novo.registros_lidos[0].codigo_escola,fgetc(pa));
		exit(0);
		(novo.registros_lidos + n_registros_lidos)->nome_escola = 
				le_tamanho_variavel(pa, &((novo.registros_lidos + n_registros_lidos)->indicador_tamanho_escola));
		(novo.registros_lidos + n_registros_lidos)->municipio = 
				le_tamanho_variavel(pa, &((novo.registros_lidos + n_registros_lidos)->indicador_tamanho_municipio));
		(novo.registros_lidos + n_registros_lidos)->endereco = 
				le_tamanho_variavel(pa, &((novo.registros_lidos + n_registros_lidos)->indicador_tamanho_endereco));
	
		auxiliar = fgetc(pa);

		if (auxiliar == ';') {
			((novo.registros_lidos) + n_registros_lidos)->data_inicio[0] = ';';
		} else {

			(novo.registros_lidos + n_registros_lidos)->data_inicio[0] = auxiliar;
			fread((novo.registros_lidos + n_registros_lidos)->data_inicio+1, 1, 9, pa);	
			(novo.registros_lidos + n_registros_lidos)->data_inicio[10] = '\0';

		}

		auxiliar = fgetc(pa);

		if (auxiliar == ';') {
			(novo.registros_lidos + n_registros_lidos)->data_final[0] = ';';
		} else {
			ungetc(auxiliar, pa);
			fread((novo.registros_lidos + n_registros_lidos)->data_final, 1, 10, pa);	
			((novo.registros_lidos + n_registros_lidos)->data_final[10]) = '\0';
		}
		//atenção
		fgetc(pa);
		n_registros_lidos++;
	} while (!feof(pa));
	
	return novo;
}

FILE * arquivo_saida(Arquivo entrada) {
	FILE * saida;
	int disponiveis, i, j, cabo = TABOM;
	

	saida = fopen("Saida.csv", "w+");
	fprintf(saida, "%c %d;", entrada.status, entrada.topo_pilha);
	
	Registro *caminho = entrada.registros_lidos;

	for (i = 0; i < entrada.n_registros_lidos; i++) {
		disponiveis = TAMANHOREGISTRO - OCUPADOS;
		fprintf(saida, "%d;", (caminho + i)->codigo_escola);
		// verifica se os registros são nulos para imprimir da forma correta
		if ((caminho + i)->data_inicio[0] == ';') {
			fprintf(saida, "%s;", DATANULA);
		} else {
			fprintf(saida, "%s;", (caminho + i)->data_inicio);
		}
		if ((caminho + i)->data_final[0] == ';') {
			fprintf(saida, "%s;", DATANULA);
		} else {
			fprintf(saida, "%s;", (caminho + i)->data_final);
		}
		// verifica se a string lida cabe no espaço que sobrou no registro
		if ((caminho + i)->indicador_tamanho_escola < disponiveis) {
			fprintf(saida, "%d", (caminho + i)->indicador_tamanho_escola);
			fprintf(saida, "%s;", (caminho + i)->nome_escola);
			disponiveis -= (caminho + i)->indicador_tamanho_escola;
		// caso não caiba, trunca a string e seta a flag cabô como DEURUIM	
		} else {
			for (j = 0; j < disponiveis; j++) {
				fprintf(saida, "%d", disponiveis);
				fprintf(saida, "%c", *(((caminho + i)->nome_escola) + j));
			}
			fprintf(saida, ";");
			cabo = DEURUIM;
		}
		// se deu ruim, imprime o tamanho dos próximos campos nulos e 
		// segue para a próxima iteração
		if (cabo == DEURUIM) {
			fprintf(saida, "0;0");
			continue;
		} else {
			// as verificações são análogas ao caso acima
			if ((caminho + i)->indicador_tamanho_municipio < disponiveis) {
				fprintf(saida, "%d", (caminho + i)->indicador_tamanho_municipio);
				fprintf(saida, "%s;", (caminho + i)->municipio);
				disponiveis -= (caminho + i)->indicador_tamanho_municipio;
			} else {
				for (j = 0; j < disponiveis; j++) {
					fprintf(saida, "%d", disponiveis);
					fprintf(saida, "%c", *(((caminho + i)->municipio) + j));
				}
				fprintf(saida, ";");
				cabo = DEURUIM;
			}
		}
		if (cabo == DEURUIM) {
			fprintf(saida, "0");
			continue;
		} else {
			if ((caminho + i)->indicador_tamanho_endereco < disponiveis) {
				fprintf(saida, "%d", (caminho + i)->indicador_tamanho_endereco);
				fprintf(saida, "%s;", (caminho + i)->endereco);
			} else {
				for (j = 0; j < disponiveis; j++) {
					fprintf(saida, "%d", disponiveis);
					fprintf(saida, "%c", *(((caminho + i)->endereco) + j));
				}
			}
		}
	}
	return saida;
}
