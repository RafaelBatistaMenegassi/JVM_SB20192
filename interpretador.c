#include "interpretador.h"
#include "jvm.h"
#include "leitor_exibidor.h"
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <stdint.h>
#include "classfile.h"

/* Na hora de resolver a classe, executar o init dela */
Classfile* resolverClasse(char* nome_classe)
{
	classesCarregadas *c = BuscarElemento_classes(jvm->classes,nome_classe);
	Classfile *classe = NULL;

	if(c != NULL){
		return c->arquivoClass;
	}
	else{
		char *nomearquivo = (char *) malloc((strlen(nome_classe)+7)*sizeof(char));
		strcpy(nomearquivo, nome_classe);
		strcat(nomearquivo, ".class");
		classe = lerArquivo(nomearquivo);
		jvm->classes = InserirFim_classes(jvm->classes,classe);
	}

	return(classe);
}

/*
	Depois da resolverMetodo, analisar semanticamente. Ver o número de argumentos,
	o tipo deles, ver se está tudo coerente com o descritor do método.
*/
bool resolverMetodo(Cp_info *cp, u2 indice_cp, u1 interface)
{
	Cp_info *methodref = cp - 1 + indice_cp;
	char *nome_classe = NULL;
	if(!interface){
		nome_classe = decodificaNIeNT(cp,methodref->UnionCP.Methodref.class_index,NAME_INDEX);;
	}
	else{
		nome_classe = decodificaNIeNT(cp,methodref->UnionCP.InterfaceMethodref.class_index,NAME_INDEX);
	}

	if(resolverClasse(nome_classe)!=NULL){
		return true;
	}
	else{
		return false;
	}
}

char* obterNomeMetodo(Cp_info *cp, u2 indice_cp, u1 interface)
{
	Cp_info *methodref = cp-1+indice_cp;
	char *descriptor = NULL;
	if(!interface){
		descriptor = decodificaNIeNT(cp,methodref->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);
	}
	else{
		descriptor = decodificaNIeNT(cp,methodref->UnionCP.InterfaceMethodref.name_and_type_index,NAME_AND_TYPE);
	}
	char *pch = strtok(descriptor,":");

	return(pch);
}

char* obterDescriptorMetodo(Cp_info *cp, u2 indice_cp, u1 interface)
{
	Cp_info *methodref = cp-1+indice_cp;
	char *descriptor = NULL;
	if(!interface){
		descriptor = decodificaNIeNT(cp,methodref->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);
	}
	else{
		descriptor = decodificaNIeNT(cp,methodref->UnionCP.InterfaceMethodref.name_and_type_index,NAME_AND_TYPE);
	}

	char *pch = strtok(descriptor,":");
	pch = strtok(NULL,":");

	return(pch);
}

int descriptorFieldValidate (char *descriptor)
{
	if (*descriptor == 'L') {
		return 5;
	}

	if (strcmp(descriptor,"I") == 0) {
		return 0;
	} else if (strcmp(descriptor,"F") == 0) {
		return 1;
	} else if (strcmp(descriptor,"B") == 0) {
		return 2;
	} else if (strcmp(descriptor,"C") == 0) {
		return 3;
	} else if (strcmp(descriptor,"S") == 0) {
		return 4;
	} else if (strcmp(descriptor,"L") == 0) {
		return 5;
	} else if (strcmp(descriptor,"Z") == 0) {
		return 6;
	} else if (strcmp(descriptor,"D") == 0) {
		return 7;
	} else if (strcmp(descriptor,"J") == 0) {
		return 8;
	}
	return 0;
}

char* obterClasseDoMetodo(Cp_info *cp, u2 indice_cp)
{
	Cp_info *methodref = cp-1+indice_cp;
	char *nome_classe = NULL;

	nome_classe = decodificaNIeNT(cp,methodref->UnionCP.Methodref.class_index,NAME_INDEX);
	return(nome_classe);
}

bool isSuper(u2 flag)
{
	int super = false;
	while(flag){

		if(flag>=SYNTHETIC){
			flag-=SYNTHETIC;
		}

		if(flag>=ABSTRACT){
			flag-=ABSTRACT;
		}

		if(flag>=INTERFACE_FLAG){
			flag-=INTERFACE_FLAG;
		}

		if(flag>=TRANSIENT){
			flag-=TRANSIENT;
		}

		if(flag>=VOLATILE){
			flag-=VOLATILE;
		}

		if(flag>=SUPER){
			flag-=SUPER;
			super = true;
			break;
		}

		if(flag>=FINAL){
			flag-=FINAL;
		}

		if(flag>=STATIC){
			flag-=STATIC;
		}

		if(flag>=PROTECTED){
			flag-=PROTECTED;
		}

		if(flag>=PRIVATE){
			flag-=PRIVATE;
		}

		if(flag>=PUBLIC){
			flag-=PUBLIC;
		}
	}

	return(super);
}

Frame* transferePilhaVetor(Frame *anterior, Frame *novo, int *parametros_cont)
{
	Pilha_operandos *aux = CriarPilha_operandos();
	int cont = 0;
	while(anterior->p->topo!=NULL && cont<(*parametros_cont)){
		Pilha_operandos *p = Pop_operandos(anterior->p);
		// Ordem reversa
		aux = Push_operandos(aux,p->topo->operando,(void*)p->topo->referencia,p->topo->tipo_operando);
		cont++;
	}

	for(int i = 0; i < cont; i++) {
		Pilha_operandos *p = Pop_operandos(aux);
		novo->v[i].variavel = malloc(sizeof(u4));
		if(p->topo->tipo_operando<=8){
			*(novo->v[i].variavel) = (u4) p->topo->operando;
		}
		else{
			*(novo->v[i].variavel) = (intptr_t) p->topo->referencia;
		}
		novo->v[i].tipo_variavel = (u1) p->topo->tipo_operando;
	}

	*parametros_cont = cont;

	return(novo);
}

Frame *transferePilhaVetorCount(Frame *f, Frame *novo,int quantidade)
{
	Pilha_operandos *aux = CriarPilha_operandos();
	int cont = 0;
	while(cont<quantidade){
		Pilha_operandos *p = Pop_operandos(f->p);
		// Ordem reversa
		aux = Push_operandos(aux,p->topo->operando,(void*)p->topo->referencia,p->topo->tipo_operando);
		cont++;
	}

	novo->v = malloc(cont*sizeof(Vetor_locais));

	for(int i=0;i<cont;i++){
		Pilha_operandos *p = Pop_operandos(aux);
		novo->v[i].variavel = malloc(sizeof(u4));
		if(p->topo->tipo_operando<=8){
			*(novo->v[i].variavel) = (u4) p->topo->operando;
		}
		else{
			*(novo->v[i].variavel) = (intptr_t) p->topo->referencia;
		}
		novo->v[i].tipo_variavel = (u1) p->topo->tipo_operando;
	}

	return(novo);
}

void nop_impl(Frame *par0, u1 par1, u1 par2)
{
	return;
}

void aconst_null_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,0,NULL,REFERENCE_OP); // 0 do tipo referência quer dizer referência apontando para NULL
}

void iconst_m1_impl(Frame *f, u1 par1, u1 par2)
{
	i4 inteiro_sinal = (i4) -1;
	Push_operandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

void iconst_0_impl(Frame *f, u1 par1, u1 par2)
{
	i4 inteiro_sinal = (i4) 0;
	Push_operandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

void iconst_1_impl(Frame *f, u1 par1, u1 par2)
{
	i4 inteiro_sinal = (i4) 1;
	Push_operandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

void iconst_2_impl(Frame *f, u1 par1, u1 par2)
{
	i4 inteiro_sinal = (i4) 2;
	Push_operandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

void iconst_3_impl(Frame *f, u1 par1, u1 par2)
{
	i4 inteiro_sinal = (i4) 3;
	Push_operandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

void iconst_4_impl(Frame *f, u1 par1, u1 par2)
{
	i4 inteiro_sinal = (i4) 4;
	Push_operandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

void iconst_5_impl(Frame *f, u1 par1, u1 par2)
{
	i4 inteiro_sinal = (i4) 5;
	Push_operandos(f->p,(u4)inteiro_sinal,NULL,INTEGER_OP);
}

void lconst_0_impl(Frame *f, u1 par1, u1 par2)
{
	i4 high_bytes = (i4) 0;
	Push_operandos(f->p,high_bytes,NULL,LONG_OP);

	i4 low_bytes = (i4) 0;
	Push_operandos(f->p,low_bytes,NULL,LONG_OP);
}

void lconst_1_impl(Frame *f, u1 par1, u1 par2)
{

	i4 high_bytes = (i4) 0;
	Push_operandos(f->p,high_bytes,NULL,LONG_OP);

	i4 low_bytes = (i4) 1;
	Push_operandos(f->p,low_bytes,NULL,LONG_OP);
}

void fconst_0_impl(Frame *f, u1 par1, u1 par2)
{

	i4 float_bytes = (i4) 0;
	Push_operandos(f->p,float_bytes,NULL,FLOAT_OP);

}

void fconst_1_impl(Frame *f, u1 par1, u1 par2)
{
	i4 float_bytes = 0x3f800000;
	Push_operandos(f->p,float_bytes,NULL,FLOAT_OP);

}

void fconst_2_impl(Frame *f, u1 par1, u1 par2)
{

	i4 float_bytes = 0x40000000;
	Push_operandos(f->p,float_bytes,NULL,FLOAT_OP);

}

void dconst_0_impl(Frame *f, u1 par1, u1 par2)
{

	i4 high_bytes = (i4) 0;
	Push_operandos(f->p,high_bytes,NULL,DOUBLE_OP);

	i4 low_bytes = (i4) 0;
	Push_operandos(f->p,low_bytes,NULL,DOUBLE_OP);
}

void dconst_1_impl(Frame *f, u1 par1, u1 par2)
{

	i4 high_bytes = 0x3FF00000;
	Push_operandos(f->p,high_bytes,NULL,DOUBLE_OP);

	i4 low_bytes = (i4) 0;
	Push_operandos(f->p,low_bytes,NULL,DOUBLE_OP);
}

void bipush_impl(Frame *f, u1 byte, u1 par1)
{
	i1 aux = (i1)byte;
	i4 byte_int = (i4) aux;
	Push_operandos(f->p,byte_int,NULL,BYTE_OP);
}

void sipush_impl(Frame *f, u1 byte1, u1 byte2)
{
	i2 byte_short = (i2)(byte1<<8) | (i2)byte2;
	i4 byte_int = (i4) byte_short;
	Push_operandos(f->p,byte_int,NULL,SHORT_OP);
}

void ldc_impl(Frame *f, u1 indexbyte1, u1 par2)
{
	Cp_info *item = f->cp-1 + indexbyte1;
	void *valor=NULL;
	u4 num=0;
	void *classe=NULL;

	switch(item->tag){
		case CONSTANT_String:
			valor = (char*) decodificaStringUTF8(f->cp-1+item->UnionCP.String.string_index);
			f->p = Push_operandos(f->p,-INT_MAX,valor,REFERENCE_OP);
		break;
		case CONSTANT_Float:
			num = item->UnionCP.Float.bytes;
			f->p = Push_operandos(f->p,num,NULL,FLOAT_OP);
		break;
		case CONSTANT_Integer:
			num = item->UnionCP.Integer.bytes;
			f->p = Push_operandos(f->p,num,NULL,INTEGER_OP);
		break;
		case CONSTANT_Class:
			valor = (char*) decodificaStringUTF8(f->cp-1+item->UnionCP.Class.name_index);
			classe = resolverClasse(valor);
			f->p = Push_operandos(f->p,-INT_MAX,classe,REFERENCE_OP);
		break;
		default:
			valor = (char *) decodificaNIeNT(f->cp,item->UnionCP.Methodref.class_index,NAME_INDEX);
			valor = (char *) decodificaNIeNT(f->cp,item->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);

		break;
	}

}

void ldc_w_impl(Frame *f, u1 indexbyte1, u1 indexbyte2)
{
	u2 indice_cp = normaliza_indice(indexbyte1,indexbyte2);
	Cp_info *item = f->cp-1 + indice_cp;
	void *valor=NULL;
	u4 num=0;
	void *classe=NULL;

	switch(item->tag){
		case CONSTANT_String:
			valor = (char*) decodificaStringUTF8(f->cp-1+item->UnionCP.String.string_index);
			f->p = Push_operandos(f->p,-INT_MAX,valor,REFERENCE_OP);
		break;
		case CONSTANT_Float:
			num = item->UnionCP.Float.bytes;
			f->p = Push_operandos(f->p,num,NULL,FLOAT_OP);
		break;
		case CONSTANT_Integer:
			num = item->UnionCP.Integer.bytes;
			f->p = Push_operandos(f->p,num,NULL,INTEGER_OP);
		break;
		case CONSTANT_Class:
			valor = (char*) decodificaStringUTF8(f->cp-1+item->UnionCP.Class.name_index);
			classe = resolverClasse(valor);
			f->p = Push_operandos(f->p,-INT_MAX,classe,REFERENCE_OP);
		break;
		default:
			valor = (char *) decodificaNIeNT(f->cp,item->UnionCP.Methodref.class_index,NAME_INDEX);
			valor = (char *) decodificaNIeNT(f->cp,item->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);
		break;
	}

}

void ldc2_w_impl(Frame *f, u1 branchbyte1, u1 branchbyte2)
{
	int8_t v1 = (int8_t)branchbyte1;
	int8_t v2 = (int8_t)branchbyte2;
	int16_t branchoffset = (v1 << 8) | v2;
	Cp_info * doubleValue = f->cp-1+branchoffset;

	u4 high,low;

	if(doubleValue->tag == CONSTANT_Double){
		high = doubleValue->UnionCP.Double.high_bytes;
		low = doubleValue->UnionCP.Double.low_bytes;
	} else {
		high = doubleValue->UnionCP.Long.high_bytes;
		low = doubleValue->UnionCP.Long.low_bytes;
	}

	if(doubleValue->tag == CONSTANT_Double){
		Push_operandos(f->p,high,NULL,DOUBLE_OP);//high
		Push_operandos(f->p,low,NULL,DOUBLE_OP);//low
	} else {
		Push_operandos(f->p,high,NULL,LONG_OP);//high
		Push_operandos(f->p,low,NULL,LONG_OP);//low
	}
}

//Carrega inteiro do Frame para a pilha de operandos
void iload_impl(Frame *f, u1 index, u1 par1)
{
	Push_operandos(f->p,(i4) *(f->v[index].variavel),NULL,INTEGER_OP);
}

//Carrega long do Frame para a pilha de operandos
void lload_impl(Frame *f, u1 index, u1 par1)
{
	//Checar se a ordem ta certa, o topo da pilha deveria ser o LOW
	Push_operandos(f->p,(i4) *(f->v[index].variavel),NULL,LONG_OP);//high
	Push_operandos(f->p,(i4) *(f->v[index+1].variavel),NULL,LONG_OP);//low
}

//Carrega float do Frame para a pilha de operandos
void fload_impl(Frame *f, u1 index, u1 par1)
{
	Push_operandos(f->p, (i4) *(f->v[index].variavel),NULL,FLOAT_OP);
}

//Carrega double do Frame para a pilha de operandos
void dload_impl(Frame *f, u1 index, u1 par2)
{
	//Checar se a ordem ta certa, o topo da pilha deveria ser o LOW
	Push_operandos(f->p,(i4) *(f->v[index].variavel),NULL,DOUBLE_OP);
	Push_operandos(f->p,(i4) *(f->v[index+1].variavel),NULL,DOUBLE_OP);
}

//Carrega referencia de array para a pilha de operandos
void aload_impl(Frame *f, u1 index, u1 par1)
{
	Push_operandos(f->p,-INT_MAX,(void*)(intptr_t)*(f->v[index].variavel),f->v[index].tipo_variavel);
}

//Carrega inteiro na posicao 0 para a pilha
void iload_0_impl(Frame *f, u1 par1, u1 par2)
{
	//Implementar assim ou chamando iload_impl(0)?
	Push_operandos(f->p,(i4) *(f->v[0].variavel),NULL,INTEGER_OP);
}

void iload_1_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[1].variavel),NULL,INTEGER_OP);
}

void iload_2_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[2].variavel),NULL,INTEGER_OP);
}

void iload_3_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[3].variavel),NULL,INTEGER_OP);
}

//Carrega long na posicao 0 para a pilha
void lload_0_impl(Frame *f, u1 par, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[0].variavel),NULL,LONG_OP);//high
	Push_operandos(f->p,(i4) *(f->v[1].variavel),NULL,LONG_OP);//low
}

void lload_1_impl(Frame *f, u1 par, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[1].variavel),NULL,LONG_OP);
	Push_operandos(f->p,(i4) *(f->v[2].variavel),NULL,LONG_OP);
}

void lload_2_impl(Frame *f, u1 par, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[2].variavel),NULL,LONG_OP);
	Push_operandos(f->p,(i4) *(f->v[3].variavel),NULL,LONG_OP);
}

void lload_3_impl(Frame *f, u1 par, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[3].variavel),NULL,LONG_OP);
	Push_operandos(f->p,(i4) *(f->v[4].variavel),NULL,LONG_OP);
}

//Carrega float na posicao 0
void fload_0_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[0].variavel),NULL,FLOAT_OP);
}

void fload_1_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[1].variavel),NULL,FLOAT_OP);
}

void fload_2_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[2].variavel),NULL,FLOAT_OP);
}

void fload_3_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[3].variavel),NULL,FLOAT_OP);
}
//Carrega double na posicao 0 para a pilha
void dload_0_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[0].variavel),NULL,DOUBLE_OP);//high
	Push_operandos(f->p,(i4) *(f->v[1].variavel),NULL,DOUBLE_OP);//low
}
void dload_1_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[1].variavel),NULL,DOUBLE_OP);
	Push_operandos(f->p,(i4) *(f->v[2].variavel),NULL,DOUBLE_OP);
}
void dload_2_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[2].variavel),NULL,DOUBLE_OP);
	Push_operandos(f->p,(i4) *(f->v[3].variavel),NULL,DOUBLE_OP);
}
void dload_3_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,(i4) *(f->v[3].variavel),NULL,DOUBLE_OP);
	Push_operandos(f->p,(i4) *(f->v[4].variavel),NULL,DOUBLE_OP);
}

//Carrega referencia na posicao 0 para a pilha
void aload_0_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,-INT_MAX,(void*)(intptr_t)*(f->v[0].variavel),f->v[0].tipo_variavel);
}

void aload_1_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,-INT_MAX,(void*)(intptr_t)*(f->v[1].variavel),f->v[1].tipo_variavel);
}

void aload_2_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,-INT_MAX,(void*)(intptr_t)*(f->v[2].variavel),f->v[2].tipo_variavel);
}

void aload_3_impl(Frame *f, u1 par1, u1 par2)
{
	Push_operandos(f->p,-INT_MAX,(void*)(intptr_t)*(f->v[3].variavel),f->v[3].tipo_variavel);
}


/* Verificar endereçamento */
/* Vale para iaload a saload */
void iaload_impl(Frame *f, u1 par1, u1 par2)
{
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *referencia = Pop_operandos(f->p);

	u4* endereco = (referencia->topo->referencia) + (indice->topo->operando * sizeof(i4));

    Push_operandos(f->p,*endereco,NULL,INTEGER_OP);
}

void laload_impl(Frame *f, u1 par1, u1 par2)
{
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *referencia = Pop_operandos(f->p);

	u4* endereco = (referencia->topo->referencia) + (indice->topo->operando * 2 * sizeof(i4));
	//Verificar ordem
	Push_operandos(f->p,*endereco,NULL,LONG_OP);
	endereco++;
	Push_operandos(f->p,*(endereco),NULL,LONG_OP);
}

void faload_impl(Frame *f, u1 par1, u1 par2)
{
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *referencia = Pop_operandos(f->p);

	u4* endereco = (referencia->topo->referencia) + (indice->topo->operando * sizeof(i4));

    Push_operandos(f->p,*endereco,NULL,FLOAT_OP);
}

void daload_impl(Frame *f, u1 par1, u1 par2)
{
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *referencia = Pop_operandos(f->p);

	u4* endereco = ( referencia->topo->referencia) + (indice->topo->operando * 2 * sizeof(u4));

	//Verificar ordem
	Push_operandos(f->p,*endereco,NULL,DOUBLE_OP);
	endereco++;
	Push_operandos(f->p,*(endereco),NULL,DOUBLE_OP);
}

void aaload_impl(Frame *f, u1 par1, u1 par2)
{
}

void baload_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *referencia = Pop_operandos(f->p);

	i1* endereco = (referencia->topo->referencia) + (indice->topo->operando * sizeof(i1));
	Push_operandos(f->p,*endereco,NULL,BYTE_OP);
}

void caload_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *referencia = Pop_operandos(f->p);

	u2* endereco = (referencia->topo->referencia) + (indice->topo->operando * sizeof(u2));
	Push_operandos(f->p,*endereco,NULL,CHAR_OP);
}

void saload_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *referencia = Pop_operandos(f->p);

	i2* endereco = (referencia->topo->referencia) + (indice->topo->operando * sizeof(i2));

	Push_operandos(f->p,*endereco,NULL,SHORT_OP);
}

void istore_impl(Frame *f, u1 index,u1 par1){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[index].variavel) = (i4) valor->topo->operando;
}

void lstore_impl(Frame *f, u1 index, u1 par1){
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	*(f->v[index].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[index+1].variavel) = (u4) low_bytes->topo->operando;
}

void fstore_impl(Frame *f, u1 index,u1 par1){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[index].variavel) = (u4) valor->topo->operando;
}

void dstore_impl(Frame *f, u1 index, u1 par1){
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	*(f->v[index].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[index+1].variavel) = (u4) low_bytes->topo->operando;
}

void astore_impl(Frame *f, u1 index,u1 par1){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[index].variavel) = (intptr_t) valor->topo->referencia;
	f->v[index].tipo_variavel = valor->topo->tipo_operando;
}

void istore_0_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[0].variavel) = (i4) valor->topo->operando;
}

void istore_1_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[1].variavel) = (u4) valor->topo->operando;
}

void istore_2_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[2].variavel) = (u4) valor->topo->operando;
}

void istore_3_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[3].variavel) = (u4) valor->topo->operando;
}

void lstore_0_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	*(f->v[0].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[1].variavel) = (u4) low_bytes->topo->operando;
}

void lstore_1_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	*(f->v[1].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[2].variavel) = (u4) low_bytes->topo->operando;
}

void lstore_2_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	*(f->v[2].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[3].variavel) = (u4) low_bytes->topo->operando;
}

void lstore_3_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	*(f->v[3].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[4].variavel) = (u4) low_bytes->topo->operando;
}

void fstore_0_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[0].variavel) = (u4) valor->topo->operando;
}

void fstore_1_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[1].variavel) = (u4) valor->topo->operando;
}

void fstore_2_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[2].variavel) = (u4) valor->topo->operando;
}

void fstore_3_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[3].variavel) = (u4) valor->topo->operando;
}

void dstore_0_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	*(f->v[0].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[1].variavel) = (u4) low_bytes->topo->operando;
}

void dstore_1_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	*(f->v[1].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[2].variavel) = (u4) low_bytes->topo->operando;
}

void dstore_2_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	*(f->v[2].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[3].variavel) = (u4) low_bytes->topo->operando;
}

void dstore_3_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	*(f->v[3].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[4].variavel) = (u4) low_bytes->topo->operando;
}

void astore_0_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[0].variavel) = (intptr_t) valor->topo->referencia;
	f->v[0].tipo_variavel = valor->topo->tipo_operando;
}

void astore_1_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);

	*(f->v[1].variavel) = (intptr_t) valor->topo->referencia;
	f->v[1].tipo_variavel = valor->topo->tipo_operando;

}

void astore_2_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[2].variavel) = (intptr_t) valor->topo->referencia;
	f->v[2].tipo_variavel = valor->topo->tipo_operando;
}

void astore_3_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	*(f->v[3].variavel) = (intptr_t) valor->topo->referencia;
	f->v[3].tipo_variavel = valor->topo->tipo_operando;
}

void iastore_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *array = Pop_operandos(f->p);

	i4 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * sizeof(i4)));
	*endereco_array = valor->topo->operando;
}

void lastore_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *array = Pop_operandos(f->p);

	i4 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * 2 * sizeof(i4)));

	*endereco_array = high_bytes->topo->operando;

	endereco_array++;

	*endereco_array = low_bytes->topo->operando;
}

void fastore_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *valor = Pop_operandos(f->p);
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *array = Pop_operandos(f->p);

	i4 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * sizeof(i4)));
	*endereco_array = valor->topo->operando;
}

void dastore_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *array = Pop_operandos(f->p);

	u4 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * 2 * sizeof(u4)));

	*endereco_array = high_bytes->topo->operando;

	endereco_array++;

	*endereco_array = low_bytes->topo->operando;
}

void bastore_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *array = Pop_operandos(f->p);

	i1 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * sizeof(i1)));
	*endereco_array = valor->topo->operando;
}

void castore_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *array = Pop_operandos(f->p);

	u2 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * sizeof(u2)));
	*endereco_array = valor->topo->operando;
}

void sastore_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Pop_operandos(f->p);
	Pilha_operandos *indice = Pop_operandos(f->p);
	Pilha_operandos *array = Pop_operandos(f->p);

	i2 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * sizeof(i2)));
	*endereco_array = valor->topo->operando;
}

void aastore_impl(Frame *f, u1 par1, u1 par2){
}

Pilha_operandos* pop_impl(Frame *f){
	Pilha_operandos *valor = Pop_operandos(f->p);

	return(valor);
}

Pilha_operandos** pop2_impl(Frame *f){
	Pop_operandos(f->p);
	Pop_operandos(f->p);
	return NULL;
}

void pop_fantasma(Frame *f, u1 par1, u1 par2){
	pop_impl(f);
}

void pop2_fantasma(Frame *par0, u1 par1, u1 par2){

}

void dup_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Topo_operandos(f->p);
	f->p = Push_operandos(f->p,valor->topo->operando,valor->topo->referencia,valor->topo->tipo_operando);
}

void dup_x1_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	f->p = Push_operandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
}

void dup_x2_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);
	Pilha_operandos *valor3 = Pop_operandos(f->p);

	f->p = Push_operandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor3->topo->operando,valor3->topo->referencia,valor3->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
}

void dup2_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	f->p = Push_operandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
}

void dup2_x1_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);
	Pilha_operandos *valor3 = Pop_operandos(f->p);

	f->p = Push_operandos(f->p,valor3->topo->operando,valor3->topo->referencia,valor3->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
}

void dup2_x2_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);
	Pilha_operandos *valor3 = Pop_operandos(f->p);
	Pilha_operandos *valor4 = Pop_operandos(f->p);

	f->p = Push_operandos(f->p,valor4->topo->operando,valor4->topo->referencia,valor4->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor3->topo->operando,valor3->topo->referencia,valor3->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
}

void swap_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	f->p = Push_operandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
}

void iadd_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	Pilha_operandos *valor3 = CriarPilha_operandos();
	valor3 = Push_operandos(valor3,valor1->topo->operando+valor2->topo->operando,NULL,valor1->topo->tipo_operando);
	valor3->topo->tipo_operando = valor1->topo->tipo_operando;
	f->p = Push_operandos(f->p,valor3->topo->operando,NULL,valor3->topo->tipo_operando);

}

void ladd_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	Pilha_operandos *low_bytes2 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes2 = Pop_operandos(f->p);

	i8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	i8 result = long1 + long2;

	f->p = Push_operandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)result,NULL, LONG_OP);

}

void fadd_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);

	float f_sum = op1+op2;
	u4 sum = (u4)(*(u4*)&f_sum);
	u4 result = (sinal(sum)<<31) | (expoente(sum)<<23) | mantissa(sum);
	f->p = Push_operandos(f->p,result,NULL,FLOAT_OP);

}
void dadd_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1_low = Pop_operandos(f->p);
	Pilha_operandos *valor1_high = Pop_operandos(f->p);
	Pilha_operandos *valor2_low = Pop_operandos(f->p);
	Pilha_operandos *valor2_high = Pop_operandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);

	double d_sum = op1+op2;
	u8 sum = (u8)(*(u8*)&d_sum);
	u8 result = (sinal_d(sum)<<63) | (expoente_d(sum)<<52) | mantissa_d(sum);

	f->p = Push_operandos(f->p, (u4)(result>>32),NULL, DOUBLE_OP);
	f->p = Push_operandos(f->p, (u4)result,NULL, DOUBLE_OP);

}

void isub_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	i4 result = valor2->topo->operando-valor1->topo->operando;
	f->p = Push_operandos(f->p,result,NULL,INTEGER_OP);
}

void lsub_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	Pilha_operandos *low_bytes2 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes2 = Pop_operandos(f->p);

	i8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	i8 result = long2 - long1;

	f->p = Push_operandos(f->p, (u4)(result>>32),NULL, LONG_OP);
	f->p = Push_operandos(f->p, (u4)result,NULL, LONG_OP);

}

void fsub_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);
	float f_sub = op2-op1;
	u4 sub = (u4)(*(u4*)&f_sub);
	u4 result = (sinal(sub)<<31) | (expoente(sub)<<23) | mantissa(sub);
	f->p = Push_operandos(f->p,result,NULL,FLOAT_OP);
}

void dsub_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1_low = Pop_operandos(f->p);
	Pilha_operandos *valor1_high = Pop_operandos(f->p);
	Pilha_operandos *valor2_low = Pop_operandos(f->p);
	Pilha_operandos *valor2_high = Pop_operandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);

	double d_sub = op2-op1;
	u8 sub = (u8)(*(u8*)&d_sub);
	u8 result = (sinal_d(sub)<<63) | (expoente_d(sub)<<52) | mantissa_d(sub);

	f->p = Push_operandos(f->p, (u4)(result>>32),NULL, DOUBLE_OP);
	f->p = Push_operandos(f->p, (u4)result,NULL, DOUBLE_OP);

}


void imul_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	i4 result = valor1->topo->operando*valor2->topo->operando;
	f->p = Push_operandos(f->p,result,NULL,INTEGER_OP);
}

void lmul_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	Pilha_operandos *low_bytes2 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes2 = Pop_operandos(f->p);

	i8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	i8 result = long1 * long2;

	f->p = Push_operandos(f->p, (u4)(result>>32),NULL, LONG_OP);
	f->p = Push_operandos(f->p, (u4)result,NULL, LONG_OP);

}

void fmul_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);
	float f_res = op1*op2;
	u4 res = (u4)(*(u4*)&f_res);
	u4 result = (sinal(res)<<31) | (expoente(res)<<23) | mantissa(res);
	f->p = Push_operandos(f->p,result,NULL,FLOAT_OP);
}

void dmul_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1_low = Pop_operandos(f->p);
	Pilha_operandos *valor1_high = Pop_operandos(f->p);
	Pilha_operandos *valor2_low = Pop_operandos(f->p);
	Pilha_operandos *valor2_high = Pop_operandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);

	double d_res = op1*op2;
	u8 res = (u8)(*(u8*)&d_res);
	u8 result = (sinal_d(res)<<63) | (expoente_d(res)<<52) | mantissa_d(res);

	f->p = Push_operandos(f->p, (u4)(result>>32),NULL, DOUBLE_OP);
	f->p = Push_operandos(f->p, (u4)result,NULL, DOUBLE_OP);

}

void idiv_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	i4 result;
	void *messageErrorDiv = NULL;
	char *messageError = "java.lang.ArithmeticException: / by zero";


	if(valor1->topo->operando!=0){
		// Se os tipos dos valores forem iguais, e se esse tipo for inteiro
		result = valor2->topo->operando/valor1->topo->operando;
		f->p = Push_operandos(f->p,result,messageErrorDiv,INTEGER_OP);

	}else{
		jvm->excecao = 1;
		//Caso ocorra uma excecao, temos que result deve ser 1.
		result = valor2->topo->operando/valor2->topo->operando;
		strcpy(jvm->excecao_nome,"java/lang/ArithmeticException");

		messageErrorDiv = messageError;
		f->p = Push_operandos(f->p,result,messageErrorDiv,REFERENCE_OP);
	}
}

void ldiv_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	Pilha_operandos *low_bytes2 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes2 = Pop_operandos(f->p);

	i8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	i8 result = long2 / long1;

	f->p = Push_operandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)result, NULL,LONG_OP);

}

void fdiv_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);
	float f_res = op2/op1;
	u4 res = (u4)(*(u4*)&f_res);
	u4 result = (sinal(res)<<31) | (expoente(res)<<23) | mantissa(res);
	f->p = Push_operandos(f->p,result,NULL,FLOAT_OP);
}

void ddiv_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1_low = Pop_operandos(f->p);
	Pilha_operandos *valor1_high = Pop_operandos(f->p);
	Pilha_operandos *valor2_low = Pop_operandos(f->p);
	Pilha_operandos *valor2_high = Pop_operandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);

	double d_res = op2/op1;
	u8 res = (u8)(*(u8*)&d_res);
	u8 result = (sinal_d(res)<<63) | (expoente_d(res)<<52) | mantissa_d(res);

	f->p = Push_operandos(f->p, (u4)(result>>32),NULL, DOUBLE_OP);
	f->p = Push_operandos(f->p, (u4)result,NULL, DOUBLE_OP);

}


void irem_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	if(valor1->topo->operando == 0){
		// Lançar Arithmetic Exception
	}

	i4 valor_push = valor2->topo->operando - (valor2->topo->operando/valor1->topo->operando) * valor1->topo->operando;

	f->p = Push_operandos(f->p,valor_push,NULL,INTEGER_OP);
}

void lrem_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	Pilha_operandos *low_bytes2 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes2 = Pop_operandos(f->p);

	i8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	i8 result = long2 - (long2/long1) * long1;

	f->p = Push_operandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)result, NULL,LONG_OP);

}

void frem_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos * valor1 = Pop_operandos(f->p);
	Pilha_operandos * valor2 = Pop_operandos(f->p);

	float v1 = decodificaFloatValor(valor1->topo->operando);
	float v2 = decodificaFloatValor(valor2->topo->operando);

	float resultAux = fmodf(v2,v1);
	u4 result = (u4)(*(u4*)&resultAux);

	f->p = Push_operandos(f->p,result,NULL,FLOAT_OP);

}

void drem_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1_low = Pop_operandos(f->p);
	Pilha_operandos *valor1_high = Pop_operandos(f->p);
	Pilha_operandos *valor2_low = Pop_operandos(f->p);
	Pilha_operandos *valor2_high = Pop_operandos(f->p);

	double v1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double v2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);

	double resultAux = fmod(v2,v1);

	u8 result = (u8)(*(u8*)&resultAux);

	f->p = Push_operandos(f->p,(u4)(result>>32),NULL,DOUBLE_OP);
	f->p = Push_operandos(f->p,(u4)result,NULL,DOUBLE_OP);

}

void ineg_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);

	i4 result = -valor1->topo->operando;
	f->p = Push_operandos(f->p,result,NULL,INTEGER_OP);
}

void lneg_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;

	i8 result = -long1;

	f->p = Push_operandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)result, NULL,LONG_OP);

}

void fneg_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);

	u4 op1 = valor1->topo->operando;

	u4 result = op1 ^ 0X80000000;

	f->p = Push_operandos(f->p,result,NULL,FLOAT_OP);
}

void dneg_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1_low = Pop_operandos(f->p);
	Pilha_operandos *valor1_high = Pop_operandos(f->p);

	u4 high_bytes = valor1_high->topo->operando;
	u4 low_bytes = valor1_low->topo->operando;

	u4 result = high_bytes ^ 0X80000000;

	f->p = Push_operandos(f->p, result,NULL, DOUBLE_OP);
	f->p = Push_operandos(f->p, low_bytes,NULL, DOUBLE_OP);
}


void ishl_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	i4 s = (valor1->topo->operando << 27) >> 27;

	i4 resultado = valor2->topo->operando << s;

	f->p = Push_operandos(f->p,resultado,NULL,valor1->topo->tipo_operando);
}

void lshl_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *valor2 = Pop_operandos(f->p);

	i4 s = (valor2->topo->operando << 27) >> 27;


	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	i8 long1 = ((i8)high_bytes->topo->operando << 32) | low_bytes->topo->operando;

	i8 result = long1 << s;

	f->p = Push_operandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)result, NULL,LONG_OP);

}


void ishr_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	i4 s = (i4) (valor1->topo->operando << 27) >> 27;

	i4 resultado = (i4) valor2->topo->operando >> s;

	f->p = Push_operandos(f->p,resultado,NULL,valor1->topo->tipo_operando);
}

void lshr_impl(Frame *f, u1 par1, u1 par2){


	Pilha_operandos *valor2 = Pop_operandos(f->p);

	i4 s = (valor2->topo->operando << 27) >> 27;


	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	i8 long1 = ((i8)high_bytes->topo->operando << 32) | low_bytes->topo->operando;

	i8 result = long1 >> s;

	f->p = Push_operandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)result, NULL,LONG_OP);

}

void iushr_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	u4 s = (u4) (valor2->topo->operando << 27) >> 27;

	u4 resultado = (u4) valor1->topo->operando >> s;

	f->p = Push_operandos(f->p,(i4) resultado,NULL,valor1->topo->tipo_operando);
}

void lushr_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	u4 s = (valor2->topo->operando << 27) >> 27;


	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	u8 long1 = ((u8)high_bytes->topo->operando << 32) | low_bytes->topo->operando;

	u8 result = long1 >> s;

	f->p = Push_operandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)result, NULL,LONG_OP);

}

void iand_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	i4 resultado = valor2->topo->operando & valor1->topo->operando;

	f->p = Push_operandos(f->p,resultado,NULL,valor1->topo->tipo_operando);
}

void land_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	u8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	Pilha_operandos *low_bytes2 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes2 = Pop_operandos(f->p);

	u8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	u8 result = long2 & long1;

	f->p = Push_operandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)result, NULL,LONG_OP);

}

void ior_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	i4 resultado = valor1->topo->operando | valor2->topo->operando;

	f->p = Push_operandos(f->p,resultado,NULL,valor1->topo->tipo_operando);
}

void lor_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	u8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	Pilha_operandos *low_bytes2 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes2 = Pop_operandos(f->p);

	u8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	u8 result = long2 | long1;

	f->p = Push_operandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)result, NULL,LONG_OP);

}

void ixor_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	i4 resultado = valor1->topo->operando ^ valor2->topo->operando;

	f->p = Push_operandos(f->p,resultado,NULL,INTEGER_OP);
}

void lxor_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	u8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	Pilha_operandos *low_bytes2 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes2 = Pop_operandos(f->p);

	u8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	u8 result = long2 ^ long1;

	f->p = Push_operandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)result, NULL,LONG_OP);

}

void iinc_impl(Frame *f,u1 index, i1 constante){
	i4 inteiro_constante = (i4) constante;
	*(f->v[index].variavel) += inteiro_constante;
}

void iinc_fantasma(Frame *par0, u1 par1, u1 par2){
	i1 valor = (i1)par2;
	iinc_impl(par0,par1,valor);
}

void iinc_wide_fantasma(Frame *f, u1 indexbyte1, u1 indexbyte2, u1 constbyte1, u1 constbyte2){
	u2 indexbyte = normaliza_indice(indexbyte1,indexbyte2);
	i2 constbyte = (i2) ((i1) (constbyte1 << 8) | (i1) constbyte2);

	iinc_wide(f,indexbyte,constbyte);
}

void iinc_wide(Frame *f, u2 indexbyte, i2 constbyte){
	i4 inteiro_constante = (i4) constbyte;
	f->v[indexbyte].variavel += inteiro_constante;
}

void i2l_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	i4 valor = (i4)valor1->topo->operando;

	i8 valor_long = (i8)valor;

	f->p = Push_operandos(f->p, (u4)(valor_long>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)valor_long, NULL,LONG_OP);

}

void i2f_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	float valor = (float)valor1->topo->operando;
	u4 flo = (u4)(*(u4*)&valor);
	f->p = Push_operandos(f->p,flo,NULL,FLOAT_OP);
}

void i2d_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	double valor = (double)valor1->topo->operando;

	u8 doub = (u8)(*(u8*)&valor);

	f->p = Push_operandos(f->p,(u4)(doub>>32),NULL,DOUBLE_OP);
	f->p = Push_operandos(f->p,(u4)doub,NULL,DOUBLE_OP);

}

void l2i_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pop_operandos(f->p);

	f->p = Push_operandos(f->p, low_bytes1->topo->operando, NULL,INTEGER_OP);
}

void l2f_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;
	float flo = (float) long1;
	u4 valor = (u4)(*(u4*)&flo);
	f->p = Push_operandos(f->p, valor, NULL,FLOAT_OP);
}

void l2d_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;
	double valor = (double) long1;
	u8 doub = (u8)(*(u8*)&valor);

	f->p = Push_operandos(f->p,(u4)(doub>>32),NULL,DOUBLE_OP);
	f->p = Push_operandos(f->p,(u4)doub,NULL,DOUBLE_OP);

}

void f2i_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	float valor = decodificaFloatValor(valor1->topo->operando);
	f->p = Push_operandos(f->p,(i4)valor,NULL,INTEGER_OP);
}

void f2l_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	float valor = decodificaFloatValor(valor1->topo->operando);

	i8 valor_long = (i8)valor;

	f->p = Push_operandos(f->p, (u4)(valor_long>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)valor_long, NULL,LONG_OP);
}

void f2d_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	double valor = (double)decodificaFloatValor(valor1->topo->operando);

	u8 doub = (u8)(*(u8*)&valor);

	f->p = Push_operandos(f->p,(u4)(doub>>32),NULL,DOUBLE_OP);
	f->p = Push_operandos(f->p,(u4)doub,NULL,DOUBLE_OP);
}

void d2i_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1_low = Pop_operandos(f->p);
	Pilha_operandos *valor1_high = Pop_operandos(f->p);

	u4 high_bytes = valor1_high->topo->operando;
	u4 low_bytes = valor1_low->topo->operando;

	double valor = decodificaDoubleValor(high_bytes, low_bytes);
	f->p = Push_operandos(f->p,(i4)valor,NULL,INTEGER_OP);
}

void d2l_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *valor1_low = Pop_operandos(f->p);
	Pilha_operandos *valor1_high = Pop_operandos(f->p);

	u4 high_bytes = valor1_high->topo->operando;
	u4 low_bytes = valor1_low->topo->operando;

	double valor = decodificaDoubleValor(high_bytes, low_bytes);

	i8 valor_long = (i8)valor;

	f->p = Push_operandos(f->p, (u4)(valor_long>>32), NULL,LONG_OP);
	f->p = Push_operandos(f->p, (u4)valor_long, NULL,LONG_OP);

}

void d2f_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *valor1_low = Pop_operandos(f->p);
	Pilha_operandos *valor1_high = Pop_operandos(f->p);

	u4 high_bytes = valor1_high->topo->operando;
	u4 low_bytes = valor1_low->topo->operando;

	float flo = (float) decodificaDoubleValor(high_bytes, low_bytes);
	u4 valor = (u4)(*(u4*)&flo);
	f->p = Push_operandos(f->p, valor, NULL,FLOAT_OP);

}

void i2b_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);

	i1 truncado = (i1) valor1->topo->operando;

	i4 resultado = (i4) truncado;

	f->p = Push_operandos(f->p,resultado,NULL,BYTE_OP);
}

void i2c_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);

	i1 truncado = (i1) valor1->topo->operando;

	i4 resultado = (i4) truncado;

	f->p = Push_operandos(f->p,resultado,NULL,CHAR_OP);
}

void i2s_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);

	i2 truncado = (i2) valor1->topo->operando;

	i4 resultado = (i4) truncado;

	f->p = Push_operandos(f->p,resultado,NULL,SHORT_OP);
}

void lcmp_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes1 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes1 = Pop_operandos(f->p);

	i8 long1 = ((i8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	Pilha_operandos *low_bytes2 = Pop_operandos(f->p);
	Pilha_operandos *high_bytes2 = Pop_operandos(f->p);

	i8 long2 = ((i8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;
	i4 retorno;
	if(long2 > long1){
		retorno = 1;
	} else if(long2 < long1){
		retorno = -1;
	} else{
		retorno = 0;
	}

	f->p = Push_operandos(f->p, retorno, NULL,INTEGER_OP);
}

void fcmpl_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);
	i4 retorno;
	if(op2 > op1){
		retorno = 1;
	} else if(op2 < op1){
		retorno = -1;
	} else{
		retorno = 0;
	}

	f->p = Push_operandos(f->p, retorno, NULL,INTEGER_OP);
}

void fcmpg_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);
	i4 retorno;
	if(op2 > op1){
		retorno = 1;
	} else if(op2 < op1){
		retorno = -1;
	} else{
		retorno = 0;
	}

	f->p = Push_operandos(f->p, retorno, NULL,INTEGER_OP);

}

void dcmpl_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *valor1_low = Pop_operandos(f->p);
	Pilha_operandos *valor1_high = Pop_operandos(f->p);
	Pilha_operandos *valor2_low = Pop_operandos(f->p);
	Pilha_operandos *valor2_high = Pop_operandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);
	i4 retorno;
	if(op2 > op1){
		retorno = 1;
	} else if(op2 < op1){
		retorno = -1;
	} else{
		retorno = 0;
	}

	f->p = Push_operandos(f->p, retorno, NULL,INTEGER_OP);

}

void dcmpg_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *valor1_low = Pop_operandos(f->p);
	Pilha_operandos *valor1_high = Pop_operandos(f->p);
	Pilha_operandos *valor2_low = Pop_operandos(f->p);
	Pilha_operandos *valor2_high = Pop_operandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);
	i4 retorno;
	if(op2 > op1){
		retorno = 1;
	} else if(op2 < op1){
		retorno = -1;
	} else{
		retorno = 0;
	}

	f->p = Push_operandos(f->p, retorno, NULL,INTEGER_OP);

}

void ifeq_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor = Pop_operandos(f->p);

	if(valor->topo->operando == 0){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void ifne_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){

	Pilha_operandos *valor = Pop_operandos(f->p);

	if(valor->topo->operando != 0){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void iflt_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){

	Pilha_operandos *valor = Pop_operandos(f->p);

	if(valor->topo->operando<0){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void ifge_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor = Pop_operandos(f->p);

	if(valor->topo->operando >= 0){
		uint8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void ifgt_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor = Pop_operandos(f->p);

	if(valor->topo->operando > 0){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void ifle_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor = Pop_operandos(f->p);

	if(valor->topo->operando <= 0){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void if_icmpeq_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	if(valor1->topo->operando == valor2->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void if_icmpne_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	if(valor1->topo->operando != valor2->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void if_icmplt_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	if(valor2->topo->operando < valor1->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void if_icmpge_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	if(valor2->topo->operando >= valor1->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void if_icmpgt_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	if(valor2->topo->operando > valor1->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void if_icmple_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	if(valor2->topo->operando <= valor1->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void acmpeq_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	if(valor1->topo->operando == valor2->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void acmpne_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	if(valor1->topo->operando != valor2->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

void inst_goto_impl(Frame *f,u1 branchbyte1, u1 branchbyte2){
	int8_t bb1 = (int8_t)branchbyte1;
	int8_t bb2 = (int8_t)branchbyte2;
	int16_t branchoffset = (bb1 << 8) | bb2;

	jvm->pc += branchoffset;
}

void jsr_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	int8_t v1 = (int8_t)branchbyte1;
	int8_t v2 = (int8_t)branchbyte2;
	int16_t branchoffset = (v1 << 8) | v2;
	jvm->pc += branchoffset;
}

void ret_impl(Frame *f,u1 index, u1 par){
}

void tableswitch_fantasma(Frame *par0, u1 par1, u1 par2){
}

void lookupswitch_fantasma(Frame *par0, u1 par1, u1 par2){
}

void ireturn_impl(Frame *f, u1 par1, u1 par2){

	Pilha_operandos *valor = Pop_operandos(f->p);

	jvm->frames->topo->prox->f->p = Push_operandos(jvm->frames->topo->prox->f->p,(i4)valor->topo->operando,NULL,valor->topo->tipo_operando);
	Pop_frames(jvm->frames);
}

void lreturn_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	jvm->frames->topo->prox->f->p = Push_operandos(jvm->frames->topo->prox->f->p,(u4)high_bytes->topo->operando,NULL,LONG_OP);
	jvm->frames->topo->prox->f->p = Push_operandos(jvm->frames->topo->prox->f->p,(u4)low_bytes->topo->operando,NULL,LONG_OP);
	Pop_frames(jvm->frames);

}

void freturn_impl(Frame *f, u1 par1, u1 par2){
	// Analisar as condições do método que deve ser retornado
	Pilha_operandos *valor = Pop_operandos(f->p);

	// Empilhar na pilha de operandos do Frame do chamador
	jvm->frames->topo->prox->f->p = Push_operandos(jvm->frames->topo->prox->f->p,(u4)valor->topo->operando,NULL,FLOAT_OP);
	Pop_frames(jvm->frames);
}

void dreturn_impl(Frame *f, u1 par1, u1 par2){
	// Analisar as condições do método que deve ser retornado
	Pilha_operandos *low_bytes = Pop_operandos(f->p);
	Pilha_operandos *high_bytes = Pop_operandos(f->p);

	// Empilhar na pilha de operandos do Frame do chamador
	jvm->frames->topo->prox->f->p = Push_operandos(jvm->frames->topo->prox->f->p,(u4)high_bytes->topo->operando,NULL,DOUBLE_OP);
	jvm->frames->topo->prox->f->p = Push_operandos(jvm->frames->topo->prox->f->p,(u4)low_bytes->topo->operando,NULL,DOUBLE_OP);
	Pop_frames(jvm->frames);
}

void areturn_impl(Frame *f, u1 par1, u1 par2){
	Pop_operandos(f->p);
}

void inst_return_impl(Frame *f, u1 par1, u1 par2){
	Pop_frames(jvm->frames);
}

void getstatic_impl(Frame *f, u1 indexbyte1, u1 indexbyte2){

	u2 indice_cp = (indexbyte1 << 8) | indexbyte2;

	Cp_info *aux = f->cp-1+indice_cp;

	// Descobrir a classe do field
	char *classedoField = decodificaNIeNT(f->cp,aux->UnionCP.Fieldref.class_index,NAME_INDEX);
	if(strcmp(classedoField,"java/lang/System")==0){
		f->p = Push_operandos(f->p,-INT_MAX,"out",REFERENCE_OP);
	} else{
		classesCarregadas * nova = BuscarElemento_classes(jvm->classes,classedoField);
		if (nova == NULL) {
			if (resolverClasse(classedoField) == NULL) {
				printf("Falha ao abrir classe com field estático, encerrando.\n");
				exit(1);
			}
		} else {
			Cp_info * nameTypeField = f->cp-1+aux->UnionCP.Fieldref.name_and_type_index;


			char * nomeField = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.name_index,NAME_AND_TYPE_INFO_NAME_INDEX);


			Field_info * fieldSaida = BuscarFieldClasseCorrente_classes(jvm->classes, classedoField, nomeField);
			if (fieldSaida != NULL) {
				char * accessF = decodificaAccessFlags(fieldSaida->access_flags);

				if (buscaStaticFlags(accessF)) {
					char * descriptorFieldAux = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.descriptor_index,NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
					if (descriptorFieldValidate(descriptorFieldAux) < 7) {
						if (descriptorFieldValidate(descriptorFieldAux) == 1) {
							i4 valorPushed = *fieldSaida->dadosStatics->low;
							f->p = Push_operandos(f->p,valorPushed,NULL,FLOAT_OP);
						} else if (descriptorFieldValidate(descriptorFieldAux) == 5) {
							u4 * valorPushed = fieldSaida->dadosStatics->low;
							f->p = Push_operandos(f->p,-INT_MAX,(void*)valorPushed,REFERENCE_STRING_OP);
						} else {
							i4 valorPushed = *fieldSaida->dadosStatics->low;
							f->p = Push_operandos(f->p,valorPushed,NULL,INTEGER_OP);
						}
					} else if (descriptorFieldValidate(descriptorFieldAux) == 7 || descriptorFieldValidate(descriptorFieldAux) == 8) {
						i4 valorPushedLow = *fieldSaida->dadosStatics->low;
						i4 valorPushedHigh = *fieldSaida->dadosStatics->high;
						if (descriptorFieldValidate(descriptorFieldAux) == 7) {
							f->p = Push_operandos(f->p,valorPushedHigh,NULL,DOUBLE_OP);
							f->p = Push_operandos(f->p,valorPushedLow,NULL,DOUBLE_OP);
						} else {
							f->p = Push_operandos(f->p,valorPushedHigh,NULL,LONG_OP);
							f->p = Push_operandos(f->p,valorPushedLow,NULL,LONG_OP);
						}
					}
				}
			}
		}
	}
}

void putstatic_impl(Frame *f, u1 indexbyte1, u1 indexbyte2){
	u2 indice_cp = (indexbyte1 << 8) | indexbyte2;

	Cp_info * field = f->cp-1+indice_cp;

	// Resolver o field
	char * nomeClasse = decodificaNIeNT(f->cp,field->UnionCP.Fieldref.class_index,NAME_INDEX);
	classesCarregadas * nova = BuscarElemento_classes(jvm->classes,nomeClasse);
	if (nova == NULL) {
		if (resolverClasse(nomeClasse) == NULL) {
			printf("Falha ao abrir classe com field estático, encerrando.\n");
			exit(1);
		}
	} else {
		Cp_info * nameTypeField = f->cp-1+field->UnionCP.Fieldref.name_and_type_index;

		char * nomeField = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.name_index,NAME_AND_TYPE_INFO_NAME_INDEX);


		Field_info * fieldSaida = BuscarFieldClasseCorrente_classes(jvm->classes, nomeClasse, nomeField);
		if (fieldSaida != NULL) {
			char * accessF = decodificaAccessFlags(fieldSaida->access_flags);

			if (buscaStaticFlags(accessF)) {

				char * descriptorFieldAux = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.descriptor_index,NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
				if (descriptorFieldValidate(descriptorFieldAux) < 7) {
					Pilha_operandos *valor = Pop_operandos(f->p);
					fieldSaida->dadosStatics = (Static_data*)malloc(sizeof(Static_data));
					fieldSaida->dadosStatics->low = (u4*) malloc(sizeof(u4));
					fieldSaida->dadosStatics->high = NULL;
					if (valor->topo->tipo_operando == REFERENCE_OP) {
						fieldSaida->dadosStatics->low = (u4*)valor->topo->referencia;
					} else {
						*fieldSaida->dadosStatics->low = (u4)valor->topo->operando;
					}
				} else if (descriptorFieldValidate(descriptorFieldAux) == 7 || descriptorFieldValidate(descriptorFieldAux) == 8) {
					Pilha_operandos *valorLow = Pop_operandos(f->p);
					Pilha_operandos *valorHigh = Pop_operandos(f->p);
					fieldSaida->dadosStatics = (Static_data*) malloc(sizeof(Static_data));
					fieldSaida->dadosStatics->low = (u4*) malloc(sizeof(u4));
					fieldSaida->dadosStatics->high = (u4*) malloc(sizeof(u4));
					*fieldSaida->dadosStatics->low = (u4)valorLow->topo->operando;
					*fieldSaida->dadosStatics->high = (u4)valorHigh->topo->operando;
				}
			}
		}
	}
}

bool buscaStaticFlags (char * accessFlags) {
	char * acc = (char*) malloc(100*sizeof(char));
	strcpy(acc,accessFlags);
	const char s[2] = " ";
  char * token;
  token = strtok(acc, s);
  while(token != NULL) {
  	if (strcmp(token,"STATIC") == 0) {
			return true;
		}
    token = strtok(NULL, s);
  }
	return false;
}

void getfield_impl(Frame *f, u1 indexbyte1, u1 indexbyte2){
    u2 indice_cp = (indexbyte1 << 8) | indexbyte2;
    Cp_info *aux = f->cp-1+indice_cp;
    char *classedoField = decodificaNIeNT(f->cp,aux->UnionCP.Fieldref.class_index,NAME_INDEX);
    if(strcmp(classedoField,"java/lang/System")==0){
        f->p = Push_operandos(f->p,-INT_MAX,"out",REFERENCE_OP);
    } else {
			Cp_info * nameTypeField = f->cp-1+aux->UnionCP.Fieldref.name_and_type_index;
			char * nomeField = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.name_index,NAME_AND_TYPE_INFO_NAME_INDEX);
			char * descriptorRetorno = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.descriptor_index,NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
			int tipoRetorno = getTipoRetorno(descriptorRetorno);
			if (tipoRetorno != -1) {
				Pilha_operandos * objetoRef = Pop_operandos(f->p);
				Classfile * buscado = (Classfile*)objetoRef->topo->referencia;
				Lista_Objetos * obj = buscaObjetoViaReferencia(buscado);
				if (obj != NULL) {
					int posicaoField = getPositionField(obj->obj, nomeField);
					if (posicaoField != -1) {
						if (tipoRetorno == 1) {
							u4 * valorLow = (u4*)malloc(sizeof(u4));
							*valorLow = obj->data[posicaoField];
							int tipoOperandoSaida = getTipoOperandoSaida(descriptorRetorno);
							f->p = Push_operandos(f->p,*valorLow,NULL,tipoOperandoSaida);
						} else {
							u4 * valorHigh = (u4*)malloc(sizeof(u4));
							*valorHigh = obj->data[posicaoField];
							u4 * valorLow = (u4*)malloc(sizeof(u4));
							*valorLow = obj->data[posicaoField+1];
							int tipoOperandoSaida = getTipoOperandoSaida(descriptorRetorno);
							f->p = Push_operandos(f->p,*valorHigh,NULL,tipoOperandoSaida);
							f->p = Push_operandos(f->p,*valorLow,NULL,tipoOperandoSaida);
						}
					} else {
						exit(1);
					}

				}
			}
		}
}

int getTipoOperandoSaida(char * descriptorRetorno) {
	switch (*descriptorRetorno) {
			case 'I':
				return INTEGER_OP;
			case 'F':
				return FLOAT_OP;
			case 'B':
				return BYTE_OP;
			case 'C':
				return CHAR_OP;
			case 'S':
				return SHORT_OP;
			case 'Z':
				return BOOLEAN_OP;
			case 'D':
				return DOUBLE_OP;
			case 'J':
				return LONG_OP;
			default:
				break;
	}
	return -1;
}

int getTipoRetorno (char * descriptor) {
	switch (*descriptor) {
			case 'I':
			case 'F':
			case 'B':
			case 'C':
			case 'S':
			case 'Z':
				return 1;
			case 'D':
			case 'J':
				return 2;
			default:
				break;
	}
	return -1;
}

void putfield_impl(Frame *f, u1 indexbyte1, u1 indexbyte2){
    u2 indice_cp = (indexbyte1 << 8) | indexbyte2;
    Cp_info * field = f->cp-1+indice_cp;
    char * nomeClasse = decodificaNIeNT(f->cp,field->UnionCP.Fieldref.class_index,NAME_INDEX);
    classesCarregadas * nova = BuscarElemento_classes(jvm->classes,nomeClasse);
    if (nova == NULL) {
        if (resolverClasse(nomeClasse) == NULL) {
            printf("Falha ao abrir classe com field estático, encerrando.\n");
            exit(1);
        }
    } else {
        Cp_info * nameTypeField = f->cp-1+field->UnionCP.Fieldref.name_and_type_index;
        char * nomeField = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.name_index,NAME_AND_TYPE_INFO_NAME_INDEX);
        Pilha_operandos * valorPopedLow = Pop_operandos(f->p);
				Pilha_operandos * valorPopedHigh = NULL;
				if (valorPopedLow->topo->tipo_operando == DOUBLE_OP || valorPopedLow->topo->tipo_operando == LONG_OP) {
					valorPopedHigh = Pop_operandos(f->p);
				}
				Pilha_operandos * objetoRef = Pop_operandos(f->p);
				Classfile * buscado = (Classfile*)objetoRef->topo->referencia;
				Lista_Objetos * obj = buscaObjetoViaReferencia(buscado);
				if (obj != NULL) {
					int posicaoField = getPositionField(obj->obj, nomeField);
					if (posicaoField != -1) {
						if (valorPopedHigh == NULL) {
							obj->data[posicaoField] = valorPopedLow->topo->operando;
						} else {
							obj->data[posicaoField] = valorPopedHigh->topo->operando;
							obj->data[posicaoField+1] = valorPopedLow->topo->operando;
						}
					} else {
						exit(1);
					}

				}
    }
}

int getPositionField (Classfile * obj, char * nomeField) {
	Classfile * aux = obj;
	int posicao = 0;
	for (Field_info * auxF = aux->fields; auxF < aux->fields+aux->fields_count; auxF++) {
		char * nomeF = decodificaStringUTF8(aux->constant_pool-1+auxF->name_index);
		if (strcmp(nomeF,nomeField) == 0) {
			return posicao;
		} else {
			posicao++;
		}
	}
	return -1;
}

Lista_Objetos * buscaObjetoViaReferencia (Classfile * p) {
	Lista_Objetos * auxiliarObjetos;
	for (auxiliarObjetos = jvm->objetos; auxiliarObjetos != NULL; auxiliarObjetos = auxiliarObjetos->prox) {
		if (auxiliarObjetos->obj == p) {
			return auxiliarObjetos;
		}
	}
	return NULL;
}

void invokevirtual_impl(Frame *f, u1 indexbyte1, u1 indexbyte2){

	u4 * end;
  u2 indice_cp = (indexbyte1 << 8) | indexbyte2;
	char *classeDoMetodo = NULL;
	int *parametros_cont = malloc(sizeof(int));

    char *nomemetodo = obterNomeMetodo(f->cp,indice_cp,0);
    char *descriptormetodo = obterDescriptorMetodo(f->cp,indice_cp,0);
		char *descriptorcopia = malloc(strlen(descriptormetodo)*sizeof(char));
		strcpy(descriptorcopia,descriptormetodo);

    if(strcmp(nomemetodo,"println")==0){
        double valorSaida_double;
        float valorSaida_float;
        long valorSaida_long;

        if (!pilhaVazia(f->p)) {
            if (!printVazio(f->p)) {
                Pilha_operandos *string = Pop_operandos(f->p);
                Pilha_operandos * v2;
                if (string->topo->tipo_operando == DOUBLE_OP || string->topo->tipo_operando == LONG_OP) {
                    v2 = Pop_operandos(f->p);
                }
                Pop_operandos(f->p);
                switch(string->topo->tipo_operando){

                    case BOOLEAN_OP:
                        printf("%d\n",(i4)string->topo->operando);
                    break;
                    case BYTE_OP:
                        printf("%d\n",(i4)string->topo->operando);
                    break;
                    case CHAR_OP:
                        printf("%c\n",(char)string->topo->operando);
                    break;
                    case SHORT_OP:
                        printf("%d\n",(i4)string->topo->operando);
                    break;
                    case INTEGER_OP:
                        printf("%d\n",(i4)string->topo->operando);
                    break;
                    case FLOAT_OP:
                        valorSaida_float = decodificaFloatValor(string->topo->operando);
                        printf("%g\n",valorSaida_float);
                    break;
                    case LONG_OP:
                    	valorSaida_long = decodificaLongValor(v2->topo->operando,string->topo->operando);
                        printf("%ld\n",valorSaida_long);
                    break;
                    case DOUBLE_OP:
                        valorSaida_double = decodificaDoubleValor(v2->topo->operando, string->topo->operando);
                        printf("%g\n",valorSaida_double);
                    break;
                    case RETURN_ADDRESS_OP:
                        printf("[@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_BOOLEAN_OP:
                        printf("[Z@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_CHAR_OP:
                        printf("%s\n",(char*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_FLOAT_OP:
                        printf("[F@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_DOUBLE_OP:
                        printf("[D@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_BYTE_OP:
                        printf("[B@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_SHORT_OP:
                        printf("[S@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_INT_OP:
                        printf("[I@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_LONG_OP:
                        printf("[J@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_OP:
                        printf("[@%p\n",(u4*) string->topo->referencia);
                    break;
					case REFERENCE_STRING_OP:
						end = (u4*)string->topo->referencia;
						printf("%s\n",(char*)(end));
					break;
                }

            } else {
                Pop_operandos(f->p);
                printf("\n");
            }
        }
    }
    else{
					classeDoMetodo = obterClasseDoMetodo(f->cp,indice_cp);

					Method_info *methodAux = BuscarMethodClasseCorrente_classes(jvm->classes,classeDoMetodo, nomemetodo);
					Attribute_info *aux;
					int posicao;
					*parametros_cont = 1 + getParametrosCount (descriptormetodo);

					for(posicao = 0; posicao < methodAux->attributes_count; posicao++) {
						aux = (*(methodAux->attributes+posicao));
						classesCarregadas *classeAtual = BuscarElemento_classes(jvm->classes,classeDoMetodo);
						char *nameindex = decodificaStringUTF8(classeAtual->arquivoClass->constant_pool-1+aux->attribute_name_index);
						if(strcmp(nameindex,"Code")==0){
							Code_attribute *c = (Code_attribute *) aux->info;
							Frame *f_novo = criarFrame(classeDoMetodo,c->max_locals);
							f_novo = transferePilhaVetor(f,f_novo,parametros_cont);
							jvm->frames = Push_frames(jvm->frames,f_novo);

							classesCarregadas *classe = BuscarElemento_classes(jvm->classes,classeDoMetodo);

							// Achar o método na classe que o contém
							Method_info *metodos = classe->arquivoClass->methods;
							for(Method_info *aux=metodos;aux<metodos+classe->arquivoClass->methods_count;aux++){
								// Verificar se o nome e o descriptor do método que deve ser invocado são iguais ao que está sendo analisado no .class
								char * nomeMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->name_index);
								char * descriptorMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->descriptor_index);

								if(strcmp(nomemetodo,nomeMetodoAux) == 0 && strcmp(descriptorcopia,descriptorMetodoAux) == 0){
									// Executar o code do método invocado
									jvm->pc +=3;
									f->end_retorno = jvm->pc;
									jvm->pc = 0;
									executarMetodo(aux,classeDoMetodo,2);
									jvm->pc = f->end_retorno;

								}
							}
						}
					}


				}

}

void invokespecial_impl(Frame *f, u1 indexbyte1, u1 indexbyte2){
	u2 indice_cp = normaliza_indice(indexbyte1,indexbyte2);

	char *nomemetodo = obterNomeMetodo(f->cp,indice_cp,0);
	char *descriptormetodo = obterDescriptorMetodo(f->cp,indice_cp,0);
	char *descriptorcopia = malloc(strlen(descriptormetodo)*sizeof(char));
	strcpy(descriptorcopia,descriptormetodo);
	char *classeDoMetodo = NULL;
	char *classePaiDaCorrente = NULL;
	classesCarregadas *classeCorrente = NULL;
	int *parametros_cont = malloc(sizeof(int));

	if(resolverMetodo(f->cp,indice_cp,0)){

		classeDoMetodo = obterClasseDoMetodo(f->cp,indice_cp);

		classeCorrente = BuscarElemento_classes(jvm->classes,jvm->frames->topo->f->classeCorrente);

		classePaiDaCorrente = decodificaNIeNT(classeCorrente->arquivoClass->constant_pool,classeCorrente->arquivoClass->super_class,NAME_INDEX);

		// If não executa, else executa
		if(isSuper(classeCorrente->arquivoClass->access_flags) && strcmp(classeDoMetodo,classePaiDaCorrente)==0 && (strcmp(nomemetodo,"init")!=0 || strcmp(nomemetodo,"clinit")!=0)){

		}
		else{
			// Vai invocar o método
			Method_info * methodAux = BuscarMethodClasseCorrente_classes(jvm->classes,classeDoMetodo, nomemetodo);
			Attribute_info *aux;
			int posicao;
			*parametros_cont = 1 + getParametrosCount (descriptormetodo);

			for(posicao = 0; posicao < methodAux->attributes_count; posicao++) {
				aux = (*(methodAux->attributes+posicao));
				classesCarregadas *classeAtual = BuscarElemento_classes(jvm->classes,classeDoMetodo);
				char *nameindex = decodificaStringUTF8(classeAtual->arquivoClass->constant_pool-1+aux->attribute_name_index);
				if(strcmp(nameindex,"Code")==0){
					Code_attribute *c = (Code_attribute *) aux->info;
					Frame *f_novo = criarFrame(classeDoMetodo,c->max_locals);
					f_novo = transferePilhaVetor(f,f_novo,parametros_cont);
					jvm->frames = Push_frames(jvm->frames,f_novo);

					classesCarregadas *classe = BuscarElemento_classes(jvm->classes,classeDoMetodo);

					// Achar o método na classe que o contém
					Method_info *metodos = classe->arquivoClass->methods;
					for(Method_info *aux=metodos;aux<metodos+classe->arquivoClass->methods_count;aux++){
						// Verificar se o nome e o descriptor do método que deve ser invocado são iguais ao que está sendo analisado no .class
						char * nomeMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->name_index);
						char * descriptorMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->descriptor_index);

						if(strcmp(nomemetodo,nomeMetodoAux) == 0 && strcmp(descriptorcopia,descriptorMetodoAux) == 0){
							// Executar o code do método invocado
							jvm->pc +=3;
							f->end_retorno = jvm->pc;
							jvm->pc = 0;
							executarMetodo(aux,classeDoMetodo,2);
							jvm->pc = f->end_retorno;

						}
					}
				}
			}

		}
	}
}

int getParametrosCount (char * descriptor) {
	int sizeChar = strlen(descriptor);
	int contador = 0;
	char * descriptorAuxiliar = descriptor;
	while (sizeChar > 0) {
		switch (*descriptorAuxiliar) {
			case '(':
				break;
			case ')':
				return contador;
				break;
			case 'I':
			case 'F':
			case 'B':
			case 'C':
			case 'S':
			case 'Z':
				contador++;
				break;
			case 'D':
			case 'J':
				contador += 2;
				break;
			default:
				break;
		}
		descriptorAuxiliar++;
		sizeChar--;
	}
	return contador;
}

void invokestatic_impl(Frame *f, u1 indexbyte1, u1 indexbyte2){

	u2 indice_cp = (indexbyte1 << 8) | indexbyte2;
	char *nomemetodo = obterNomeMetodo(f->cp,indice_cp,0);
	char *descriptormetodo = obterDescriptorMetodo(f->cp,indice_cp,0);
	char *descriptorcopia = malloc(strlen(descriptormetodo)*sizeof(char));
	strcpy(descriptorcopia,descriptormetodo);
	int *parametros_cont = malloc(sizeof(int));
	*parametros_cont = 0;


	// Realizar a contagem de parâmetros

	char *pch = strtok(descriptormetodo,"()");
	*parametros_cont += strlen(pch);

	if(resolverMetodo(f->cp,indice_cp,0)){
		char *classeNova = obterClasseDoMetodo(f->cp,indice_cp);
		Method_info * methodAux = BuscarMethodClasseCorrente_classes(jvm->classes, classeNova, nomemetodo);
		Attribute_info *aux;
		int posicao;
		for(posicao = 0; posicao < methodAux->attributes_count; posicao++) {
			aux = (*(methodAux->attributes+posicao));
			classesCarregadas *classeAtual = BuscarElemento_classes(jvm->classes,classeNova);
			char *nameindex = decodificaStringUTF8(classeAtual->arquivoClass->constant_pool-1+aux->attribute_name_index);
			if(strcmp(nameindex,"Code")==0){
				Code_attribute *c = (Code_attribute *) aux->info;
				Frame *f_novo = criarFrame(classeNova,c->max_locals);

				f_novo = transferePilhaVetor(f,f_novo,parametros_cont);
				jvm->frames = Push_frames(jvm->frames,f_novo);

				classesCarregadas *classe = BuscarElemento_classes(jvm->classes,classeNova);

				// Achar o método na classe que o contém
				Method_info *metodos = classe->arquivoClass->methods;
				for(Method_info *aux=metodos;aux<metodos+classe->arquivoClass->methods_count;aux++){
					// Verificar se o nome e o descriptor do método que deve ser invocado são iguais ao que está sendo analisado no .class
					char * nomeMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->name_index);
					char * descriptorMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->descriptor_index);

					if(strcmp(nomemetodo,nomeMetodoAux) == 0 && strcmp(descriptorcopia,descriptorMetodoAux) == 0){
						// Executar o code do método invocado
						jvm->pc +=3;
						f->end_retorno = jvm->pc;
						jvm->pc = 0;
						executarMetodo(aux,classeNova,2);
						jvm->pc = f->end_retorno;

					}
				}
			}
		}
	}
}

void invokeinterface_impl(Frame *f, u1 indexbyte1, u1 indexbyte2, u1 count){
	u2 indice_cp = normaliza_indice(indexbyte1,indexbyte2);

	char *nomemetodo = obterNomeMetodo(f->cp,indice_cp,1);
	char *descriptormetodo = obterDescriptorMetodo(f->cp,indice_cp,1);

	if(resolverMetodo(f->cp,indice_cp,1)){
		char *classeNova = malloc(100*sizeof(char));
		strcpy(classeNova,jvm->frames->topo->f->classeCorrente);
		Method_info *methodAux = BuscarMethodClasseCorrente_classes(jvm->classes, classeNova, nomemetodo);
		Attribute_info *aux;
		int posicao;
		for(posicao = 0; posicao < methodAux->attributes_count; posicao++) {
			aux = (*(methodAux->attributes+posicao));
			classesCarregadas *classeAtual = BuscarElemento_classes(jvm->classes,classeNova);
			char *nameindex = decodificaStringUTF8(classeAtual->arquivoClass->constant_pool-1+aux->attribute_name_index);
			if(strcmp(nameindex,"Code")==0){
				Code_attribute *c = (Code_attribute *) aux->info;
				Frame *f_novo = criarFrame(classeNova,c->max_locals);
				f_novo = transferePilhaVetorCount(f,f_novo,count);
				jvm->frames = Push_frames(jvm->frames,f_novo);

				classesCarregadas *classe = BuscarElemento_classes(jvm->classes,classeNova);

				// Achar o método na classe que o contém
				Method_info *metodos = classe->arquivoClass->methods;
				for(Method_info *aux=metodos;aux<metodos+classe->arquivoClass->methods_count;aux++){
					// Verificar se o nome e o descriptor do método que deve ser invocado são iguais ao que está sendo analisado no .class
					char * nomeMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->name_index);
					char * descriptorMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->descriptor_index);

					if(strcmp(nomemetodo,nomeMetodoAux) == 0 && strcmp(descriptormetodo,descriptorMetodoAux) == 0){
						executarMetodo(aux,classeNova,2);
					}
				}
			}
		}
	}
}

void invokeinterface_fantasma(Frame *par0, u1 par1, u1 par2){

}

void invokedynamic_fantasma(Frame *par0, u1 par1, u1 par2){

}

void inst_new_impl(Frame *f, u1 indexbyte1, u1 indexbyte2){
	u2 indice_cp = (indexbyte1 << 8) | indexbyte2;
	char *nomeClasse = decodificaNIeNT(f->cp,indice_cp,NAME_INDEX);
	Classfile *classe = instanciarClasse(nomeClasse);
	int numFields = getParametrosNaoStatic(classe);
	jvm->objetos = InsereObjeto(jvm->objetos, classe,numFields);
	f->p = Push_operandos(f->p,-INT_MAX,classe,REFERENCE_OP);
}

Classfile * instanciarClasse (char * nomeClasse) {
	char * saida = (char*)malloc((strlen(nomeClasse)+8)*sizeof(char));
	strcpy(saida,nomeClasse);
	strcat(saida,".class");
	Classfile * cf = lerArquivo(saida);
	return cf;
}

Lista_Objetos * InsereObjeto (Lista_Objetos * lis, Classfile * classe, int parametrosNaoStatic) {
	Lista_Objetos * objeto = (Lista_Objetos*)malloc(sizeof(Lista_Objetos));
	objeto->obj = classe;
	objeto->sizeData = parametrosNaoStatic;
	objeto->data = (u4*)malloc(parametrosNaoStatic*sizeof(u4));
	objeto->prox = NULL;
	objeto->ant = NULL;

	Lista_Objetos * aux;
	Lista_Objetos * ultimo;
	if (lis != NULL) {
		aux = lis;
		while (aux != NULL) {
			ultimo = aux;
			aux = aux->prox;
		}
		ultimo->prox = objeto;
		objeto->ant = ultimo;
		return lis;
	} else {
		return objeto;
	}
}

int getParametrosNaoStatic (Classfile * classe) {
	Classfile * classeAux = classe;
	int contador = 0;
	if (classeAux->fields_count > 0) {
		for (Field_info * fieldAux = classeAux->fields; fieldAux < classeAux->fields+classeAux->fields_count; fieldAux++) {
			if (fieldAux->access_flags != 0x0008) {
				contador++;
			}
		}
	}
	return contador;
}

void newarray_impl(Frame *f, u1 atype, u1 par1){
	Pilha_operandos *count = Pop_operandos(f->p);
	i4 countnum = count->topo->operando;
	int ref_size = 0;
	void *endereco = NULL;
	int i = 0;
	if(count<0){
		// Lançar exceção
	}
	else{
		switch(atype){
			case T_BOOLEAN:
				endereco = (u1*) malloc((countnum+1)*sizeof(u1));
				ref_size = sizeof(u1);
				/*// Inicializar com os valores default */
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					// Alocar com -INT_MAX para marcar o fim do array
					if(i==countnum){
						*(u1 *)p = CHAR_MAX;
					}
					else{
						*(u1 *)p=0;
					}
				}
			break;

			case T_CHAR:
				endereco = (i2*) malloc((countnum+1)*sizeof(i2));
				ref_size = sizeof(i2);
				/*// Inicializar com os valores default */
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					// Alocar com -INT_MAX para marcar o fim do array
					if(i==countnum){
						*(i2 *)p = SHRT_MAX;
					}
					else{
						*(i2 *)p=0;
					}
				}
			break;

			case T_FLOAT:
				endereco = (u4*) malloc((countnum+1)*sizeof(u4));
				ref_size = sizeof(u4);
				/*// Inicializar com os valores default */
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					// Alocar com -INT_MAX para marcar o fim do array
					if(i==countnum){
						*(u4 *)p = UINT32_MAX;
					}
					else{
						*(u4 *)p=0;
					}
				}
			break;

			case T_DOUBLE:
				endereco = (u4*) malloc((2*(countnum)+1)*sizeof(u4));
				ref_size = 2*sizeof(u4);
				/*// Inicializar com os valores default */
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					// Alocar com -INT_MAX para marcar o fim do array
					if(i==countnum){
						*(u4 *)p = UINT32_MAX;
					}
					else{
						*(u4 *)p=0;
					}
				}
				break;

			case T_BYTE:
				endereco = (i1*) malloc((countnum+1)*sizeof(i1));
				ref_size = sizeof(i1);
				/* Inicializar com os valores default */
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					// Alocar com -INT_MAX para marcar o fim do array
					if(i==countnum){
						*(i1 *)p = CHAR_MAX;
					}
					else{
						*(i1 *)p=0;
					}
				}
			break;

			case T_SHORT:
				endereco = (i2*) malloc((countnum+1)*sizeof(i2));
				ref_size = sizeof(i2);
				/* Inicializar com os valores default */
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					// Alocar com -INT_MAX para marcar o fim do array
					if(i==countnum){
						*(i2 *)p = SHRT_MAX;
					}
					else{
						*(i2 *)p=0;
					}
				}
			break;

			case T_INT:
				endereco = (i4*) malloc((countnum+1)*sizeof(i4));
				ref_size = sizeof(i4);
				/*// Inicializar com os valores default */
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					// Alocar com -INT_MAX para marcar o fim do array
					if(i==countnum){
						*(i4 *)p = INT_MAX;
					}
					else{
						*(i4 *)p=0;
					}
				}
			break;

			case T_LONG:
				endereco = (i4*) malloc((2*(countnum)+1)*sizeof(i4));
				ref_size = 2*sizeof(i4);
				/*// Inicializar com os valores default */
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					// Alocar com -INT_MAX para marcar o fim do array
					if(i == countnum){
						*(i4 *)p = INT32_MAX;
					}
					else{
						*(i4 *)p=0;
					}
				}
			break;
		}

		// atype + 8 = Transformando tipo de array pra tipo de referencia (ver estrutura)
		f->p = Push_operandos(f->p,-INT_MAX,endereco,atype + 7);
	}
}

void anewarray_impl(Frame *f, u1 par1, u1 par2){
}

void arraylength_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *array_ref = Pop_operandos(f->p);
	void *referencia = array_ref->topo->referencia;
	u1 tipo_referencia = array_ref->topo->tipo_operando;

	int tamanho = 0;
	int ref_size = 0;
	switch(tipo_referencia){
		case REFERENCE_ARRAY_BOOLEAN_OP:
			ref_size = sizeof(u1);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(u1 *)p == CHAR_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_CHAR_OP:
				ref_size = sizeof(i2);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(i2 *)p == SHRT_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_FLOAT_OP:
			ref_size = sizeof(u4);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(u4 *)p == UINT32_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_DOUBLE_OP:
			ref_size = 2*sizeof(u4);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(u4 *)p == UINT32_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_BYTE_OP:
			ref_size = sizeof(i1);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(i1 *)p == CHAR_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_SHORT_OP:
			ref_size = sizeof(i2);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(i2 *)p == SHRT_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_INT_OP:
			ref_size = sizeof(i4);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(i4 *)p == INT_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_LONG_OP:
			ref_size = 2*sizeof(i4);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(i4 *)p == INT32_MAX){
					break;
				}
			}
		break;
	}

	f->p = Push_operandos(f->p,tamanho,NULL,INTEGER_OP);

}

void athrow_impl(Frame *f, u1 par1, u1 par2){

}

void checkcast_impl(Frame *f, u1 par1, u1 par2){

}

void instanceof_impl(Frame *f, u1 par1, u1 par2){

}

void monitorenter_impl(Frame *f, u1 par1, u1 par2){

}

void monitorexit_impl(Frame *f, u1 par1, u1 par2){

}

void wide_impl(Frame *f, u1 indexbyte1, u1 indexbyte2){
	return;
}

void multianewarray_fantasma(Frame *f, u1 par1, u1 par2){

}
void* inicializa_multiarray_recursivo(void* endereco, i4* counts, char* tipos){
	switch(tipos[1]){
		case '[':{
			endereco = (void**)malloc(counts[0]*sizeof(void*));
			void ** endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux = inicializa_multiarray_recursivo(endereco_aux, &counts[1], &tipos[1]);
				endereco_aux++;
			}
			break;
		}
		case 'I':{
			endereco = (i4*)malloc(counts[0]*sizeof(i4));
			i4* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		case 'B':{
			endereco = (i1*)malloc(counts[0]*sizeof(i1));
			i1* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		case 'S':{
			endereco = (i2*)malloc(counts[0]*sizeof(i2));
			i2* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		case 'J':{//LONG
			endereco = (i8*)malloc(counts[0]*sizeof(i8));
			i8* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		case 'F':{
			endereco = (float*)malloc(counts[0]*sizeof(float));
			float* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		case 'D':{
			endereco = (double*)malloc(counts[0]*sizeof(double));
			double* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		default:
		break;
	}
	return endereco;
}
void multianewarray_impl(Frame *f, u1 indexbyte1, u1 indexbyte2, u1 dimensions){

	u2 indice_cp = normaliza_indice(indexbyte1,indexbyte2);
	Cp_info *item = f->cp-1 + indice_cp;
	char* tipos = decodificaStringUTF8(f->cp-1+item->UnionCP.Class.name_index);
	i4* counts = (i4*)malloc(dimensions*sizeof(i4));
	void *endereco = NULL;
	for(u1 i = dimensions; i > 0; i--){
		Pilha_operandos *valor = Pop_operandos(f->p);
		counts[i-1] = valor->topo->operando;
		if(counts[i] < 0){
			//Lancar NegativeArraySizeException
		}
	}

	endereco = inicializa_multiarray_recursivo(endereco, counts, tipos);

	f->p = Push_operandos(f->p,-INT_MAX,endereco,REFERENCE_OP);
	free(counts);

}

void ifnull_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor = Pop_operandos(f->p);

	i2 offset = 0;

	if (valor->topo->referencia == NULL) {
		i1 v1 = (i1)branchbyte1;
		i1 v2 = (i1)branchbyte2;
		offset = (v1 << 8) | v2;
		jvm->pc += offset;
	}
}

void ifnonnull_impl(Frame *f, u1 branchbyte1, u1 branchbyte2){
	Pilha_operandos *valor = Pop_operandos(f->p);

	i2 offset = 0;

	if(valor->topo->referencia != NULL) {
		i1 v1 = (i1)branchbyte1;
		i1 v2 = (i1)branchbyte2;
		offset = (v1 << 8) | v2;
		jvm->pc += offset;
	}
}

void goto_w_impl(Frame *f, u1 par1, u1 par2){

}

void jsr_w_impl(Frame *f, u1 par1, u1 par2){

}

double decodificaDoubleValor (u4 high, u4 low) {
	u8 valor = ((u8)(high)<<32) | (u8)low;

	double retorno = (double)(*(double*)&valor);
	return retorno;
}

long decodificaLongValor (u4 high, u4 low) {
	u8 valor = ((u8)(high)<<32) | (u8)low;
	long retorno = (long)(*(long*)&valor);
	return retorno;
}

float decodificaFloatValor (u4 valor) {
	float retorno = (float)(*(float*)&valor);
	return retorno;
}
