/* Arquivo de implementação das funções de controle da pilha de operandos */
#include "pilha_operandos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Pilha_operandos* CriarPilha_operandos(){
	Pilha_operandos *p = malloc(sizeof(struct pilha_operandos));
	p->topo = NULL;
	return(p);
}

Pilha_operandos* Push_operandos(Pilha_operandos *p, u4 operando, void *referencia, u1 tipo_operando){
	p->topo = InserirInicio_operandos(p->topo,operando,referencia,tipo_operando);
	return (p);
}

Pilha_operandos* Pop_operandos(Pilha_operandos *p){

	Pilha_operandos *aux = CriarPilha_operandos();
	aux = Push_operandos(aux,p->topo->operando,p->topo->referencia,p->topo->tipo_operando);
	p->topo = RemoverInicio_operandos(p->topo);
	return (aux);
}

bool pilhaVazia (Pilha_operandos *p) {
	if (p->topo == NULL) {
		return true;
	} else {
		return false;
	}
}


bool printVazio (Pilha_operandos *p) {

	int contador = 0;
	u1 tipoOpAux;
	for (lista_operandos *lo = p->topo; lo != NULL; lo = lo->prox) {
		contador++;
		tipoOpAux = lo->tipo_operando;
	}
	if (contador == 1 && tipoOpAux == 10) {
		return true;
	} else {
		return false;
	}
}

Pilha_operandos* Topo_operandos(Pilha_operandos *p){
	return(p);
}

void ImprimirPilha_operandos(Pilha_operandos *p){
	ImprimirLista_operandos(p->topo);
}
