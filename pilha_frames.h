/* Estrutura e cabeçalho da implementação da pilha de frames */

/* Definição da macro */
#ifndef PILHA_FRAMES_H
#define PILHA_FRAMES_H

#include <stdint.h>
#include "lista_frames.h"

/*
 * Estrutura de dados da pilha de frames
 */
struct pilha_frames{
	struct lista_frames *topo; /**< Ponteiro para o topo da pilha de frames */
};
typedef struct pilha_frames pilha_frames;

/* Inicializa pilha de frames */
pilha_frames* CriarPilha_frames();

/* Insere um frame no topo da pilha de frames
 * p1 - p: Ponteiro da pilha de frames
 * p2 - f: Frame a ser inserido
 *
 * Retorna a pilha de frames com o novo frame inserido no topo
 */
pilha_frames* Push_frames(pilha_frames *p, Frame *f);

/* Remove o frame localizado no topo da pilha de frames
 *
 * p1: p Ponteiro da pilha de frames
 * Retorna a pilha de frames com o topo removido
 */
pilha_frames* Pop_frames(pilha_frames *p);

/* Acessa o topo da pilha de frames
 *
 * p1: p Ponteiro da pilha de frames
 * 		- Não deveria retornar lista_frames?
 * Retorna ponteiro para o nó de topo da pilha de frames
 */
pilha_frames* Topo_frames(pilha_frames *p);
/*
 * Imprime na tela uma representação da pilha de frames
 *
 * p1: p Ponteiro da pilha de frames
 */
void ImprimirPilha_frames(pilha_frames *p);
#endif
