/*
Universidade de Brasília - 02/2019
Software Básico - Turma A
JVM

Alunos:
				Brenda Barbosa de Souza   		 - 12/0111918
				Jéssica da Silva Oliveira 		 - 13/0028983
				Rafael Batista Menegassi  		 - 14/0159355
				Rafael Silva de Alencar   		 - 13/0130834
				Rodrigo Neris Ferreira Cardoso   - 14/0161597
*/

/**
 * Arquivo: main.c
 * Descricao: Arquivo com funcao main para execucao do programa.
 */

#include "classfile.h"  			// Inclusao de estruturas necessarias para leitura do arquivo .class
#include "leitor_exibidor.h"	 	// Inclusao de funcoes para leitura do arquivo .class
#include "jvm.h"        		    // Inclusao de estruturas e funcoes necessarias para execucao da JVM implementada
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * argc -> Número de argumentos do programa
 * argv -> Argumentos da linha de comando
 */

int main(int argc, char const *argv[])
{
	/*Alocação de string para conter o nome do arquivo .class*/
	char *nomearquivo = (char *) malloc(100 * sizeof(char));
	int input = 0;

	printf("\n####################################\n");
	printf("####### JAVA VIRTUAL MACHINE #######\n");
	printf("##### SOFTWARE BASICO - 2019/2 #####\n");
	printf("############# GRUPO 3 ##############\n");
	printf("####################################\n\n");

	jvm = inicializar();

	/*Estrutura condicional que analisa se foi passado o nome do arquivo como parametro*/
	if(argc > 1){
		if(strlen(argv[1]) > 6 && !strcmp(argv[1] + strlen(argv[1]) - 6, ".class")){
			strcpy(nomearquivo, argv[1]);
			input = 1;
		}
	}

	if(!input){
		printf("Digite o nome do arquivo .class:\n");
		scanf("%s",nomearquivo);
	}

	/*Chamada da funcao que realiza a leitura do arquivo .class*/
	Classfile* cf = lerArquivo(nomearquivo);
	jvm->classes = InserirFim_classes(jvm->classes, cf);

	int opcao = opcaoMenu();
	if (opcao == 1) {
		printf("\n####################################\n");
		printf("############# IMPRESSAO ############\n");
		printf("####################################\n");

		if (jvm->classes->arquivoClass != NULL) {
			imprimirClassFile(jvm->classes->arquivoClass, stdout);
		} else {
			printf("Erro ao imprimir arquivo class. O programa será encerrado.\n");
		}

		free(jvm->classes->arquivoClass);
		free(jvm->classes);
		free(jvm);
		free(nomearquivo);

		exit(1);
	} else if (opcao == 2) {
		printf("\n####################################\n");
		printf("############# EXECUCAO #############\n");
		printf("####################################\n");

		executarJVM();

		free(jvm->classes->arquivoClass);
		free(jvm->classes);
		free(jvm);
		free(nomearquivo);

		exit(1);
	} else if (opcao == 3){
		printf("\n####################################\n");
		printf("############# IMPRESSAO ############\n");
		printf("####################################\n");

		imprimirClassFile(jvm->classes->arquivoClass, stdout);

		printf("\n####################################\n");
		printf("############# EXECUCAO #############\n");
		printf("####################################\n");

		executarJVM();

		free(jvm->classes->arquivoClass);
		free(jvm->classes);
		free(jvm);
		free(nomearquivo);
		
		exit(1);
	}

	free(jvm->classes->arquivoClass);
	free(jvm->classes);
	free(jvm);
	free(nomearquivo);

	return 0;
}
