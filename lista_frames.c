/* Funções de controle e manipulação da lista de frames */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_frames.h"

lista_frames* CriarLista_frames()
{
	return NULL;
}

lista_frames* InserirInicio_frames(lista_frames* lista, Frame *f)
{
	lista_frames *novo = (lista_frames*) malloc(sizeof(lista_frames));
	novo->f = f;
	novo->prox = lista;
	novo->ant = NULL;

	if (lista != NULL){
		lista->ant = novo;
	}

	return novo;
}

lista_frames* InserirFim_frames(lista_frames* lista, Frame *f)
{
	lista_frames *novo;
	lista_frames *ant = NULL;
	lista_frames *p = lista;

	while(p!=NULL){
		ant = p;
		p=p->prox;
	}

	novo = malloc(sizeof(lista_frames));
	novo->f = f;
	novo->prox = NULL;
	novo->ant = ant;
	ant->prox = novo;

	return (lista);
}

lista_frames* RemoverInicio_frames(lista_frames* lista)
{
	lista_frames *p = lista;

	lista = p->prox;
	if(lista != NULL){
		lista->ant = NULL;
	}

	free(p);
	return (lista);
}

lista_frames* RemoverFim_frames(lista_frames* lista)
{
	lista_frames *p = lista, *ant=NULL;

	while (p != NULL){
		ant = p;
		p = p->prox;
	}

	ant->ant->prox = NULL;

	free(ant);
	return lista;
}

/*lista_frames* RemoverElemento_frames(lista_frames* lis,frame *f){
	// lista_frames *p = BuscarElemento_frames(lis, f);

	if(p==NULL){
		return lis;
	}

	if (lis == p){
		lis=p->prox;
	}else{
		p->ant->prox = p->prox;
	}

	if (p->prox!=NULL){
		p->prox->ant = p->ant;
	}

	free(p);

	return lis;
}*/

void ImprimirLista_frames(lista_frames *lista)
{
	lista_frames *f;

	for(f = lista; f != NULL; f = f->prox){
		printf("%04x\n\n",f->f->end_retorno);
		// ImprimirPilha_frames(f->p);
	}
}

void LiberarLista_frames(lista_frames *lista)
{
	lista_frames *p = lista;
	while(p != NULL){
		lista_frames *t = p->prox;
		free(p);
		p=t;
	}
}
