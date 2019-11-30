/*
 * lista_classes.c
 * Funcoes de manipulacao da lista de classes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_classes.h"
#include "leitor_exibidor.h"

classesCarregadas* CriarLista_classes()
{
	return NULL;
}

classesCarregadas* InserirFim_classes(classesCarregadas* lista, Classfile *arquivoClass)
{
	classesCarregadas *novo;
	classesCarregadas *ant = NULL;
	classesCarregadas *p = lista;

	while(p != NULL){
		ant = p;
		p = p->prox;
	}

	novo = (classesCarregadas*) malloc(sizeof(classesCarregadas));
	novo->arquivoClass = arquivoClass;
	novo->prox = NULL;
	novo->ant = ant;
	if (ant != NULL) {
		ant->prox = novo;
		return (lista);
	} else {
		return novo;
	}
}

classesCarregadas* RemoverFim_classes(classesCarregadas* lista)
{
	classesCarregadas *p = lista, *ant = NULL;

	while(p != NULL){
		ant = p;
		p=p->prox;
	}

	if (ant != NULL) {
		if (ant->ant != NULL) {
			ant->ant->prox = NULL;
		} else {
			free(ant);
			return NULL;
		}
	}

	free(ant);
	return lista;
}

classesCarregadas* RemoverElemento_classes(classesCarregadas* lista, char *string)
{
	classesCarregadas *p = BuscarElemento_classes(lista, string);

	if(p == NULL){
		return lista;
	}

	if (lista == p){
		lista = p->prox;
	}else{
		p->ant->prox = p->prox;
	}

	if (p->prox!=NULL){
		p->prox->ant = p->ant;
	}

	free(p);

	return lista;
}

classesCarregadas* BuscarElemento_classes(classesCarregadas* lista, char *string)
{
	classesCarregadas *p;
	for(p = lista; p != NULL; p = p->prox){
		if(strcmp(decodificaNIeNT(p->arquivoClass->constant_pool,p->arquivoClass->this_class,NAME_INDEX),string)==0){
			return p;
		}
	}

	return(NULL);
}

Cp_info* BuscarCPClasseCorrente_classes(classesCarregadas *lista, char *string)
{
	classesCarregadas *classeCorrente = BuscarElemento_classes(lista,string);
	return(classeCorrente->arquivoClass->constant_pool);
}

Method_info* BuscarMethodClasseCorrente_classes(classesCarregadas *lista, char * nomeClasse, char * nomeMetodo)
{
	classesCarregadas *classeCorrente = BuscarElemento_classes(lista, nomeClasse);
	Method_info *auxMethod = classeCorrente->arquivoClass->methods;

	for(Method_info *aux = auxMethod; aux < auxMethod+classeCorrente->arquivoClass->methods_count; aux++){
		// Verificar se o nome e o descriptor do método que deve ser invocado são iguais ao que está sendo analisado no .class
		char *nomeMetodoAux = decodificaStringUTF8(classeCorrente->arquivoClass->constant_pool-1+aux->name_index);
		if(strcmp(nomeMetodo,nomeMetodoAux) == 0){
			return aux;
		}
	}

	return NULL;
}

void ImprimirLista_classes(classesCarregadas *lista)
{
	classesCarregadas *p;

	for(p = lista; p != NULL; p = p->prox){
		printf("%s",decodificaNIeNT(p->arquivoClass->constant_pool,p->arquivoClass->this_class,NAME_INDEX));
		printf("\n");
	}
}

void LiberarLista_classes(classesCarregadas *lista)
{
	classesCarregadas *p = lista;
	while(p != NULL){
		classesCarregadas *t = p->prox;
		free(p);
		p=t;
	}
}

Field_info* BuscarFieldClasseCorrente_classes(classesCarregadas *lista, char * nomeClasse, char * nomeField)
{
	classesCarregadas *classeCorrente = BuscarElemento_classes(lista, nomeClasse);
	Field_info *auxField = classeCorrente->arquivoClass->fields;

	for(Field_info *aux = auxField;aux < auxField+classeCorrente->arquivoClass->fields_count;aux++){
		// Verificar se o nome e o descritor do método que deve ser invocado são iguais
		// ao que está sendo analisado no .class
		char *nomeFieldAux = decodificaStringUTF8(classeCorrente->arquivoClass->constant_pool-1+aux->name_index);
		if(strcmp(nomeField,nomeFieldAux) == 0){
			return aux;
		}
	}

	return NULL;
}
