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

/* Funções de controle e manipulação da pilha de frames */

#include "pilha_frames.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pilha_frames* CriarPilha_frames()
{
	pilha_frames *p = malloc(sizeof(pilha_frames));
	p->topo = NULL;
	return p;
}

pilha_frames* Push_frames(pilha_frames *p, Frame *f)
{
	p->topo = InserirInicio_frames(p->topo,f);
	return (p);
}

pilha_frames* Pop_frames(pilha_frames *p)
{
	pilha_frames *aux = CriarPilha_frames();
	aux = Push_frames(aux,p->topo->f);
	p->topo = RemoverInicio_frames(p->topo);
	return (aux);
}

pilha_frames* Topo_frames(pilha_frames *p)
{
	return(p);
}

void ImprimirPilha_frames(pilha_frames *p)
{
	ImprimirLista_frames(p->topo);
}
