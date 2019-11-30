/* Contém estrutura e cabeçalho da implementação da lista de frames */

/** Definição da macro */
#ifndef LISTA_FRAMES_H
#define LISTA_FRAMES_H

#include <stdio.h>
#include <stdint.h>
#include "classfile.h"

/* Estrutura de dados da lista de frames */
struct lista_frames{
	Frame *f; 						 	            /* Ponteiro para uma variável de frame. */
	struct lista_frames *prox;          /* Ponteiro de acesso ao próximo nó da lista.*/
	struct lista_frames *ant;  	        /* Ponteiro de acesso ao nó anterior da lista.*/
};
typedef struct lista_frames lista_frames;

/* Inicializa uma lista de frames sem valor
 * Retorna uma lista de frames com valor nulo
 */
lista_frames* CriarLista_frames();

/*  Insere um frame no início da lista de frames do programa
 *
 * p1 - lista: Ponteiro da lista de frames
 * p2 - f: Frame a ser inserido na lista
 *
 * Retorna lista de frames com o novo frame inserido no início
 */
lista_frames* InserirInicio_frames(lista_frames *lista, Frame *f);

/* Insere um frame no fim da lista de frames do programa
 *
 * p1 - lista: Ponteiro da lista de frames
 * p2 - f: Frame a ser inserido na lista
 *
 * Retorna lista de frames com o novo frame inserido no final
 */
lista_frames* InserirFim_frames(lista_frames *lista, Frame *f);

/* Remove o frame do início da lista de frames do programa
 *
 * p1 - lista: Ponteiro da lista de frames
 * Retorna lista de frames com o primeiro elemento removido
 */
lista_frames* RemoverInicio_frames(lista_frames *lista);

/* Remove o frame do fim da lista de frames do programa
 *
 * p1 - lista: Ponteiro da lista de frames
 * Retorna lista de frames com o último elemento removido
 */
lista_frames* RemoverFim_frames(lista_frames *lista);

/* Imprime na tela uma representação textual da lista de frames.
 * 			- O campo end_retorno (endereço de retorno) de todos os frames
 * da lista é impresso na tela em formato hexadecimal.
 * p1 - lista: Lista de frames a ser impressa
 */
void ImprimirLista_frames(lista_frames *lista);

/*
 * Libera a memória utilizada por toda a lista de frames
 * 			- A lista de frames é iterada e todos os elementos são
 * liberados com o comando free(p). Evita vazamentos de memória.
 *
 * p1 - lista: Lista de frames a ser liberada
 */
void LiberarLista_frames(lista_frames *lista);

// lista_frames* BuscarPosicao_frames(lista_frames *lis, int posicao);
// lista_frames* InserirPosicao_frames(lista_frames *lis, frame *f, int posicao);
// lista_frames* RemoverPosicao_frames(lista_frames *lis, int posicao);
// lista_frames* BuscarElemento_frames(lista_frames *lis, frame *f);
// lista_frames* RemoverElemento_frames(lista_frames *lis,frame *f);
#endif
