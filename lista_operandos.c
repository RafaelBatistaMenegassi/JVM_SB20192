/**
 * @file lista_operandos.c
 * @brief Funções de controle da lista de operandos
 */

 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "lista_operandos.h"

lista_operandos* CriarLista_operandos(){
	return NULL;
}

lista_operandos* InserirInicio_operandos(lista_operandos* lis, i4 operando, void* referencia, u1 tipo_operando){
	lista_operandos *novo= malloc(sizeof(lista_operandos));
	if(tipo_operando<=8){
		novo->operando = operando;
		novo->referencia = NULL;
	}
	else{
		/* Na hora de dar o pop, tratar */
		novo->operando = -INT_MAX;
		novo->referencia = referencia;
	}
	novo->tipo_operando = tipo_operando;
	novo->prox = lis;
	novo->ant = NULL;

	if (lis!=NULL){
		lis->ant = novo;
	}

	return novo;
}

lista_operandos* InserirFim_operandos(lista_operandos* lis, i4 operando, void *referencia, u1 tipo_operando){
	lista_operandos *novo; lista_operandos *ant = NULL; lista_operandos *p = lis;

	while(p!=NULL){
		ant = p;
		p=p->prox;
	}

	novo = malloc(sizeof(lista_operandos));

	if(tipo_operando <= 8){
	novo->operando = operando;
	novo->referencia = NULL;
	}
	else {
		novo->operando = -INT_MAX;
		novo->referencia = referencia;
	}
	novo->tipo_operando = tipo_operando;
	novo->prox = NULL;
	novo->ant = ant;
	ant->prox = novo;

	return (lis);
}

lista_operandos* RemoverInicio_operandos(lista_operandos* lis){
	lista_operandos* p=lis;

	lis = p->prox;
	if(lis!=NULL){
		lis->ant = NULL;
	}

	free(p);
	return (lis);
}

lista_operandos* RemoverFim_operandos(lista_operandos* lis){
	lista_operandos *p=lis, *ant=NULL;

	while (p!=NULL){
		ant = p;
		p=p->prox;
	}

	ant->ant->prox = NULL;

	free(ant);

	return lis;
}

lista_operandos* RemoverElemento_operandos(lista_operandos* lis,i4 operando,u1 tipo_operando){
	lista_operandos *p = BuscarElemento_operandos(lis, operando, tipo_operando);

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
}

lista_operandos* BuscarElemento_operandos(lista_operandos* lis, i4 operando, u1 tipo_operando){
	lista_operandos *p;
	for(p=lis;p!=NULL;p=p->prox){
		if(p->operando == operando && p->tipo_operando == tipo_operando){
			return p;
		}
	}

	return(NULL);
}

lista_operandos* BuscarPosicao_operandos(lista_operandos *lis, int posicao){
	lista_operandos *p;
	int cont;

	for(p=lis,cont=0;p!=NULL && cont<posicao;){
		p=p->prox;
		cont++;
	}

	return(p);
}

lista_operandos* InserirPosicao_operandos(lista_operandos *lis, i4 operando, u1 tipo_operando, int posicao){
	lista_operandos *novo; lista_operandos *ant=NULL; lista_operandos *p;
	int cont;

	for(p=lis,cont=0;p!=NULL && cont<posicao;){
		ant=p;
		p=p->prox;
		cont++;
	}

	novo = malloc(sizeof(lista_operandos));
	novo->operando = operando;
	novo->tipo_operando = tipo_operando;

	if (ant==NULL){
		novo->prox = lis;
		novo->ant = NULL;
		lis = novo;
	}else{
		ant->prox = novo;
		novo->ant = ant;
		novo->prox = p;
		if(p!=NULL){
			p->ant = novo;
		}
	}

	return(lis);
}

lista_operandos* RemoverPosicao_operandos(lista_operandos *lis, int posicao){

	lista_operandos *p = BuscarPosicao_operandos(lis, posicao);

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

	return (lis);
}

void ImprimirLista_operandos(lista_operandos *lis){
	lista_operandos *p;

	for(p=lis;p!=NULL;p=p->prox){
		printf("Tipo: %d\t",p->tipo_operando);
		if(p->tipo_operando<=8){
			printf("Operando: 0x%08x\n\n",p->operando);
		}
		else{
			switch(p->tipo_operando){
				case RETURN_ADDRESS_OP:
					printf("Operando: %s\n\n",(char*) p->referencia);
				break;
				case REFERENCE_ARRAY_BOOLEAN_OP:
					printf("[Z@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_CHAR_OP:
					printf("Operando: %s\n\n",(char*) p->referencia);
				break;
				case REFERENCE_ARRAY_FLOAT_OP:
					printf("[F@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_DOUBLE_OP:
					printf("[D@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_BYTE_OP:
					printf("[B@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_SHORT_OP:
					printf("[S@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_INT_OP:
					printf("[I@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_LONG_OP:
					printf("[J@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_STRING_OP:
					printf("String: %s\n", (char *) p->referencia);
				break;
				case REFERENCE_OP:
					printf("Operando: 0x%p\n\n", (u4*) p->referencia);
				break;
			}
		}
	}
}

void LiberarLista_operandos(lista_operandos *lis){
	lista_operandos *p=lis;
	while(p!=NULL){
		lista_operandos *t = p->prox;
		free(p);
		p=t;
	}
}
