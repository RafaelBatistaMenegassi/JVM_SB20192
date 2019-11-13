/***********************************
 * Software Basico
 * Turma A
 * Professor Marcelo Ladeira
 * 2019/2
 * 
 * Trabalho Final - JVM
 * 
 ************************************/

/***********************************
 * Main Inicial
 * -> para testes de etapas da JVM
 * 
 ************************************/

/********************************************
 * Etapas
 * 
 * 1) Loading    - ClassLoader
 * 2) Linking    - ClassLoader/Interpreter
 * 3) Initialize - Interpreter/JVM 
 * 
 ********************************************/

// Inclusoes necessarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macros de interesse
#define SEPARADOR "\n------------------------------------------------------"
#define JVM_TITULO "\n- JAVA VIRTUAL MACHINE\n- Seja bem-vindo!\n-\n-"
#define ARQUIVO_SELECIONADO argv[1]

#define ETAPA_UM "\n-> Primeira Etapa: Loading via ClassLoader"
#define ETAPA_DOIS "\n-> Segunda Etapa: Linking com Interpretador"
#define ETAPA_TRES "\n-> Terceira Etapa: Initialize da JVM"

#define ERRO "\n\t- ERRO:"

int main (int argc, char *argv[])
{
    printf(SEPARADOR);
    printf(JVM_TITULO);
    
    if (argc > 1)
    {
        char *nomeArquivoSelecionado = (char *) malloc(strlen(ARQUIVO_SELECIONADO) * sizeof(char));

        strcpy(nomeArquivoSelecionado, ARQUIVO_SELECIONADO);

        printf("\n- Arquivo selecionado: %s", ARQUIVO_SELECIONADO);

        printf(SEPARADOR);
        printf(ETAPA_UM);

        // LEITURA!!!
    }
    else
    {
        printf("%s Nao foi selecionado arquivo na chamada inicial.%s Operacao abortada!\n", ERRO, ERRO);
        printf("%s\n", SEPARADOR);

        exit(1);
    }
    
    return 0;
}