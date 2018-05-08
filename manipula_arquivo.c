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

	//SETANDO O REGISTRO DE CABEÇALHO
	novo.status = '1';
	novo.topo_pilha =-1;

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


		n_registros_lidos++;
		
	}


	novo.n_registros_lidos = n_registros_lidos;
	return novo;
}


void  arquivo_saida(Arquivo *entrada) {


	FILE * saida,*saida1;


	saida = fopen("saida.bin", "wb");
	//cabecalho
	fwrite(&entrada->status, sizeof(char), 1, saida);
	fwrite(&entrada->topo_pilha, sizeof(int), 1, saida);

	long int posicao_atual;
	int bytes_faltantes;
	char c = '0';

	for (int i = 0; i < entrada->n_registros_lidos; ++i){
		
		fwrite(&(entrada->registros_lidos[i].codigo_escola), sizeof(int), 1, saida);
		fwrite(&entrada->registros_lidos[i].data_inicio, sizeof(char), 10, saida);
		fwrite(&entrada->registros_lidos[i].data_final, sizeof(char), 10, saida);
		fwrite(&entrada->registros_lidos[i].indicador_tamanho_escola, sizeof(int), 1, saida);
		fwrite(entrada->registros_lidos[i].nome_escola, sizeof(char), entrada->registros_lidos[i].indicador_tamanho_escola, saida);
		fwrite(&entrada->registros_lidos[i].indicador_tamanho_municipio, sizeof(int), 1, saida);
		fwrite(entrada->registros_lidos[i].municipio, sizeof(char), entrada->registros_lidos[i].indicador_tamanho_municipio, saida);
		fwrite(&entrada->registros_lidos[i].indicador_tamanho_endereco, sizeof(int), 1, saida);
		fwrite(entrada->registros_lidos[i].endereco, sizeof(char), entrada->registros_lidos[i].indicador_tamanho_endereco, saida);
		
		posicao_atual = ftell(saida) - T_CABECALHO; // posicao depois do registro inserido (tirando o cabecalho)
		//printf("posicao atual %ld\n", posicao_atual);

		if((posicao_atual % TAMANHOREGISTRO) != 0){ // se não estivermos em um multiplo do tamanho do registro, temos que completar o registro

			bytes_faltantes = (TAMANHOREGISTRO*(i+1)) - (posicao_atual); // qntd de bytes para preencher = tamanho do registro - posicao final do registro atual;

			//printf("bytes faltando %d\n", bytes_faltantes);
			fwrite(&c, sizeof(char), bytes_faltantes, saida); // preenchendo o fim do arquivo
		}

		// fprintf(saida1, "%d", entrada->registros_lidos[i].codigo_escola); // gravando codigo da escola	
 	// 	fprintf(saida1, "%s%s", entrada->registros_lidos[i].data_inicio,entrada->registros_lidos[i].data_final); // gravando as datas	
 	// 	fprintf(saida1, "%d", entrada->registros_lidos[i].indicador_tamanho_escola); // gravando indicador de tamanho do nome da escola
 	// 	fprintf(saida1, "%s", entrada->registros_lidos[i].nome_escola); // gravando nome da escola
 	// 	fprintf(saida1, "%d", entrada->registros_lidos[i].indicador_tamanho_municipio); // gravando indicador de tamanho do nome da escola
 	// 	fprintf(saida1, "%s", entrada->registros_lidos[i].municipio); // gravando nome da escola
 	// 	fprintf(saida1, "%d", entrada->registros_lidos[i].indicador_tamanho_endereco); // gravando indicador de tamanho do nome da escola
 	// 	fprintf(saida1, "%s", entrada->registros_lidos[i].endereco); // gravando nome da escola
 		
	}	
	
	fclose(saida);
	
}

void exibe_registros(){ // função 2

	int codigo_escola; 
	
	char data_ini[11]; // OS 10 BYTES DA DATA MAIS O \0
	char data_fim[11];
	
	int tam_nome;
	char* nome_escola = NULL; 
	
	int tam_municipio;
	char* municipio = NULL;
	
	int tam_endereco;
	char* endereco = NULL;
	

	FILE * saida;
	saida = fopen("saida.bin", "rb");
	
	fseek(saida, 0, SEEK_END);
	int tamanho_arquivo = ftell(saida);
	fseek(saida, 0, SEEK_SET);

	fseek(saida, T_CABECALHO, SEEK_SET); // PULA OS 5 BYTES DO REGISTRO DE CABEÇALHO


	for(int i = 0; ftell(saida) < tamanho_arquivo; i++){ // enquanto houver linhas linhas para ler
		


		fread(&codigo_escola, sizeof(int), 1, saida); // 4 bytes
		fread(data_ini, sizeof(char), 10, saida);  // 10*1 byte
		fread(data_fim, sizeof(char), 10, saida); // 10*1 byte
		
		data_ini[10] = data_fim[10] = '\0';
		
		fread(&tam_nome, sizeof(int), 1, saida); // 4 bytes
		nome_escola = realloc(nome_escola, (tam_nome+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
		fread(nome_escola, sizeof(char), tam_nome, saida); // tam_nome*1 byte
		
		fread(&tam_municipio, sizeof(int), 1, saida); // 4 bytes
		municipio = realloc(municipio, (tam_municipio+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
		fread(municipio, sizeof(char), tam_municipio, saida);  // tam_municipio*1 byte

		fread(&tam_endereco, sizeof(int), 1, saida);
		endereco = realloc(endereco, (tam_endereco+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0	
		fread(endereco, sizeof(char), tam_endereco, saida); // tam_endereco*1 byte



		//total 112 bytes, tamanho do registro

		//ACRESCENTA O \0 AO FINAL DAS STRINGS LIDAS
		nome_escola[tam_nome] = '\0';
		municipio[tam_municipio] = '\0';
		endereco[tam_endereco] = '\0';

		fseek(saida, (TAMANHOREGISTRO*(i+1) + T_CABECALHO), SEEK_SET); // Pulando os caracteres "lixo" do registro
		//PRINTA CADA REGISTRO EM 1 LINHA
		printf("%d %s %s %d %s %d %s %d %s\n", codigo_escola,data_ini,data_fim,tam_nome,nome_escola,tam_municipio, municipio, tam_endereco, endereco);
	}


	fclose(saida);
}

void func3(FILE* saida, char* nome_campo, char* val_campo){

	int b_inicial, b_final;
	char FLAG_codEscola = 0; // 0 se não é nome da escola, 1 se é

	int codigo_escola_lido;
	int codEscola_pedido = atoi(val_campo);

	if(strcmp("codEscola",nome_campo) == 0){ // byte 0 + cabecalho
		b_inicial = T_CABECALHO; //byte inicial do codigo escola no primeiro registro
		b_final = T_CABECALHO +4; //byte final do codigo escola no primeiro registro
		FLAG_codEscola = 1;

	}else if(strcmp("dataInicio",nome_campo) == 0){ 
		b_inicial = T_CABECALHO+4; // byte 4 + cabecalho
		b_final = T_CABECALHO +13;

	}else if(strcmp("dataFinal",nome_campo) == 0){ 
		b_inicial = T_CABECALHO+14; // byte 14 + cabecalho
		b_final = T_CABECALHO +23;
		
	}else if(strcmp("nomeEscola",nome_campo) == 0){ 
		printf("Nome Escola\n"); // byte 24 -> tam_nome escola + cabecalho

	}else if(strcmp("municipio",nome_campo) == 0){ 
		printf("Nome Municipio\n");

	}else if(strcmp("endereco",nome_campo) == 0){
		printf("Nome Endereco\n");
	}

	fseek(saida, 0, SEEK_END);
	int tamanho_arquivo = ftell(saida);
	fseek(saida, 0, SEEK_SET);

	fseek(saida, T_CABECALHO, SEEK_SET); // PULA OS 5 BYTES DO REGISTRO DE CABEÇALHO




	for(int i = 0; ftell(saida) < tamanho_arquivo; i++){ // enquanto houver linhas linhas para ler
	

		b_inicial += TAMANHOREGISTRO*i; // byte inicial do campo pedido no registro atual
		b_final += TAMANHOREGISTRO*i; // byte final do campo pedido no registro final
	
		if(FLAG_codEscola == 1){ // se o campo pedido for o codigo da escola
			//leio do arquivo como um int

			fread(&codigo_escola_lido, sizeof(int), 1, saida);
			
			// se os codigos lidos e pedido foram iguais
			if(codigo_escola_lido == codigo_escola_pedido){
				//imprime registro
				fseek(saida, (TAMANHOREGISTRO*i)+T_CABECALHO, SEEK_SET);

				ImprimeRegistro(saida, (TAMANHOREGISTRO*i)+T_CABECALHO);

				fseek(saida, b_inicial, SEEK_SET);
			}

		}else{ // se o campo pedido for outro
			// leio uma string
			char* str = query(saida, b_inicial, b_final, val_campo);
			
			// e comparo com o valor do campo pedido
			if(strcmp(val_campo, str) == 0){
				//imprime registro
				fseek(saida, (TAMANHOREGISTRO*i)+T_CABECALHO, SEEK_SET);

				ImprimeRegistro(saida, (TAMANHOREGISTRO*i)+T_CABECALHO);

				fseek(saida, b_inicial, SEEK_SET);
			}	
		}


		exit(0);
	}	
}

char* query (FILE* fp, int b_inicial, int b_final, char* val_campo){

	// acessa o byte inicial do campo a ser lido
	fseek(fp, b_inicial, SEEK_SET);

	char* str = NULL;
	int tam = (b_final- b_inicial +1); // tamanho total do campo
	str = realloc(str, (tam+1) * sizeof(char)); // tam+1 para suportar o \0
	str[tam+1] ='\0';

	fread(str, sizeof(char),tam, fp);

	printf("%s\n", str);

	return str;

}

void ImprimeRegistro(FILE* saida, int b_inicial)
{

	fseek(saida, b_inicial, SEEK_SET);

	int codigo_escola; 
	
	char data_ini[11]; // OS 10 BYTES DA DATA MAIS O \0
	char data_fim[11];
	
	int tam_nome;
	char* nome_escola = NULL; 
	
	int tam_municipio;
	char* municipio = NULL;
	
	int tam_endereco;
	char* endereco = NULL;

	fread(&codigo_escola, sizeof(int), 1, saida); // 4 bytes
	fread(data_ini, sizeof(char), 10, saida);  // 10*1 byte
	fread(data_fim, sizeof(char), 10, saida); // 10*1 byte
		
	data_ini[10] = data_fim[10] = '\0';
		
	fread(&tam_nome, sizeof(int), 1, saida); // 4 bytes
	nome_escola = realloc(nome_escola, (tam_nome+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
	fread(nome_escola, sizeof(char), tam_nome, saida); // tam_nome*1 byte
		
	fread(&tam_municipio, sizeof(int), 1, saida); // 4 bytes
	municipio = realloc(municipio, (tam_municipio+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
	fread(municipio, sizeof(char), tam_municipio, saida);  // tam_municipio*1 byte

	fread(&tam_endereco, sizeof(int), 1, saida);
	endereco = realloc(endereco, (tam_endereco+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0	
	fread(endereco, sizeof(char), tam_endereco, saida); // tam_endereco*1 byte

	printf("%d %s %s %d %s %d %s %d %s\n", codigo_escola,data_ini,data_fim,tam_nome,nome_escola,tam_municipio, municipio, tam_endereco, endereco);


}