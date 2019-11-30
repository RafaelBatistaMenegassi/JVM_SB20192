/* Arquivo fonte que monta o vetor de instruções e seus metadados.
 * 		- A partir de uma enumeração definida no arquivo cabeçalho, este código
 * popula um vetor do tipo "instrucao" atribuindo os valores de nome, opcode, número
 * de argumentos e número de operandos de todas as instruções implementadas neste trabalho.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instrucoes.h"

const int contagem_enum = jsr_w-nop;

Instrucao* construirInstrucoes(void)
{

	Instrucao *instrucoes = (Instrucao *) malloc(contagem_enum * sizeof(struct instrucao));

	for (int i = 0; i < contagem_enum; i++)
	{
		switch(i){
			case nop:
				strcpy(instrucoes[i].inst_nome,"nop");
				instrucoes[i].opcode = nop;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case aconst_null:
				strcpy(instrucoes[i].inst_nome,"aconst_null");
				instrucoes[i].opcode = aconst_null;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case iconst_m1:
				strcpy(instrucoes[i].inst_nome,"iconst_m1");
				instrucoes[i].opcode = iconst_m1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case iconst_0:
				strcpy(instrucoes[i].inst_nome,"iconst_0");
				instrucoes[i].opcode = iconst_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case iconst_1:
				strcpy(instrucoes[i].inst_nome,"iconst_1");
				instrucoes[i].opcode = iconst_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case iconst_2:
				strcpy(instrucoes[i].inst_nome,"iconst_2");
				instrucoes[i].opcode = iconst_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case iconst_3:
				strcpy(instrucoes[i].inst_nome,"iconst_3");
				instrucoes[i].opcode = iconst_3;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case iconst_4:
				strcpy(instrucoes[i].inst_nome,"iconst_4");
				instrucoes[i].opcode = iconst_4;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case iconst_5:
				strcpy(instrucoes[i].inst_nome,"iconst_5");
				instrucoes[i].opcode = iconst_5;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case lconst_0:
				strcpy(instrucoes[i].inst_nome,"lconst_0");
				instrucoes[i].opcode = lconst_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case lconst_1:
				strcpy(instrucoes[i].inst_nome,"lconst_1");
				instrucoes[i].opcode = lconst_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case fconst_0:
				strcpy(instrucoes[i].inst_nome,"fconst_0");
				instrucoes[i].opcode = fconst_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case fconst_1:
				strcpy(instrucoes[i].inst_nome,"fconst_1");
				instrucoes[i].opcode = fconst_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case fconst_2:
				strcpy(instrucoes[i].inst_nome,"fconst_2");
				instrucoes[i].opcode = fconst_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case dconst_0:
				strcpy(instrucoes[i].inst_nome,"dconst_0");
				instrucoes[i].opcode = dconst_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case dconst_1:
				strcpy(instrucoes[i].inst_nome,"dconst_1");
				instrucoes[i].opcode = dconst_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case bipush:
				strcpy(instrucoes[i].inst_nome,"bipush");
				instrucoes[i].opcode = bipush;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 0;
			break;
			case sipush:
				strcpy(instrucoes[i].inst_nome,"sipush");
				instrucoes[i].opcode = sipush;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 0;
			break;
			case ldc:
				strcpy(instrucoes[i].inst_nome,"ldc");
				instrucoes[i].opcode = ldc;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 0;
			break;
			case ldc_w:
				strcpy(instrucoes[i].inst_nome,"ldc_w");
				instrucoes[i].opcode = ldc_w;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 0;
			break;
			case ldc2_w:
				strcpy(instrucoes[i].inst_nome,"ldc2_w");
				instrucoes[i].opcode = ldc2_w;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 0;
			break;
			case iload:
				strcpy(instrucoes[i].inst_nome,"iload");
				instrucoes[i].opcode = iload;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 0;
			break;
			case lload:
				strcpy(instrucoes[i].inst_nome,"lload");
				instrucoes[i].opcode = lload;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 0;
			break;
			case fload:
				strcpy(instrucoes[i].inst_nome,"fload");
				instrucoes[i].opcode = fload;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 0;
			break;
			case dload:
				strcpy(instrucoes[i].inst_nome,"dload");
				instrucoes[i].opcode = dload;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 0;
			break;
			case aload:
				strcpy(instrucoes[i].inst_nome,"aload");
				instrucoes[i].opcode = aload;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 0;
			break;
			case iload_0:
				strcpy(instrucoes[i].inst_nome,"iload_0");
				instrucoes[i].opcode = iload_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case iload_1:
				strcpy(instrucoes[i].inst_nome,"iload_1");
				instrucoes[i].opcode = iload_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case iload_2:
				strcpy(instrucoes[i].inst_nome,"iload_2");
				instrucoes[i].opcode = iload_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case iload_3:
				strcpy(instrucoes[i].inst_nome,"iload_3");
				instrucoes[i].opcode = iload_3;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case lload_0:
				strcpy(instrucoes[i].inst_nome,"lload_0");
				instrucoes[i].opcode = lload_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case lload_1:
				strcpy(instrucoes[i].inst_nome,"lload_1");
				instrucoes[i].opcode = lload_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case lload_2:
				strcpy(instrucoes[i].inst_nome,"lload_2");
				instrucoes[i].opcode = lload_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case lload_3:
				strcpy(instrucoes[i].inst_nome,"lload_3");
				instrucoes[i].opcode = lload_3;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case fload_0:
				strcpy(instrucoes[i].inst_nome,"fload_0");
				instrucoes[i].opcode = fload_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case fload_1:
				strcpy(instrucoes[i].inst_nome,"fload_1");
				instrucoes[i].opcode = fload_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case fload_2:
				strcpy(instrucoes[i].inst_nome,"fload_2");
				instrucoes[i].opcode = fload_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case fload_3:
				strcpy(instrucoes[i].inst_nome,"fload_3");
				instrucoes[i].opcode = fload_3;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case dload_0:
				strcpy(instrucoes[i].inst_nome,"dload_0");
				instrucoes[i].opcode = dload_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case dload_1:
				strcpy(instrucoes[i].inst_nome,"dload_1");
				instrucoes[i].opcode = dload_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case dload_2:
				strcpy(instrucoes[i].inst_nome,"dload_2");
				instrucoes[i].opcode = dload_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case dload_3:
				strcpy(instrucoes[i].inst_nome,"dload_3");
				instrucoes[i].opcode = dload_3;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case aload_0:
				strcpy(instrucoes[i].inst_nome,"aload_0");
				instrucoes[i].opcode = aload_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case aload_1:
				strcpy(instrucoes[i].inst_nome,"aload_1");
				instrucoes[i].opcode = aload_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case aload_2:
				strcpy(instrucoes[i].inst_nome,"aload_2");
				instrucoes[i].opcode = aload_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case aload_3:
				strcpy(instrucoes[i].inst_nome,"aload_3");
				instrucoes[i].opcode = aload_3;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case iaload:
				strcpy(instrucoes[i].inst_nome,"iaload");
				instrucoes[i].opcode = iaload;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case laload:
				strcpy(instrucoes[i].inst_nome,"laload");
				instrucoes[i].opcode = laload;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case faload:
				strcpy(instrucoes[i].inst_nome,"faload");
				instrucoes[i].opcode = faload;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case daload:
				strcpy(instrucoes[i].inst_nome,"daload");
				instrucoes[i].opcode = daload;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case aaload:
				strcpy(instrucoes[i].inst_nome,"aaload");
				instrucoes[i].opcode = aaload;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case baload:
				strcpy(instrucoes[i].inst_nome,"baload");
				instrucoes[i].opcode = baload;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case caload:
				strcpy(instrucoes[i].inst_nome,"caload");
				instrucoes[i].opcode = caload;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case saload:
				strcpy(instrucoes[i].inst_nome,"saload");
				instrucoes[i].opcode = saload;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case istore:
				strcpy(instrucoes[i].inst_nome,"istore");
				instrucoes[i].opcode = istore;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 1;
			break;
			case lstore:
				strcpy(instrucoes[i].inst_nome,"lstore");
				instrucoes[i].opcode = lstore;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 1;
			break;
			case fstore:
				strcpy(instrucoes[i].inst_nome,"fstore");
				instrucoes[i].opcode = fstore;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 1;
			break;
			case dstore:
				strcpy(instrucoes[i].inst_nome,"dstore");
				instrucoes[i].opcode = dstore;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 1;
			break;
			case astore:
				strcpy(instrucoes[i].inst_nome,"astore");
				instrucoes[i].opcode = astore;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 1;
			break;
			case istore_0:
				strcpy(instrucoes[i].inst_nome,"istore_0");
				instrucoes[i].opcode = istore_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case istore_1:
				strcpy(instrucoes[i].inst_nome,"istore_1");
				instrucoes[i].opcode = istore_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case istore_2:
				strcpy(instrucoes[i].inst_nome,"istore_2");
				instrucoes[i].opcode = istore_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case istore_3:
				strcpy(instrucoes[i].inst_nome,"istore_3");
				instrucoes[i].opcode = istore_3;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case lstore_0:
				strcpy(instrucoes[i].inst_nome,"lstore_0");
				instrucoes[i].opcode = lstore_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case lstore_1:
				strcpy(instrucoes[i].inst_nome,"lstore_1");
				instrucoes[i].opcode = lstore_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case lstore_2:
				strcpy(instrucoes[i].inst_nome,"lstore_2");
				instrucoes[i].opcode = lstore_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case lstore_3:
				strcpy(instrucoes[i].inst_nome,"lstore_3");
				instrucoes[i].opcode = lstore_3;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case fstore_0:
				strcpy(instrucoes[i].inst_nome,"fstore_0");
				instrucoes[i].opcode = fstore_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case fstore_1:
				strcpy(instrucoes[i].inst_nome,"fstore_1");
				instrucoes[i].opcode = fstore_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case fstore_2:
				strcpy(instrucoes[i].inst_nome,"fstore_2");
				instrucoes[i].opcode = fstore_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case fstore_3:
				strcpy(instrucoes[i].inst_nome,"fstore_3");
				instrucoes[i].opcode = fstore_3;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case dstore_0:
				strcpy(instrucoes[i].inst_nome,"dstore_0");
				instrucoes[i].opcode = dstore_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case dstore_1:
				strcpy(instrucoes[i].inst_nome,"dstore_1");
				instrucoes[i].opcode = dstore_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case dstore_2:
				strcpy(instrucoes[i].inst_nome,"dstore_2");
				instrucoes[i].opcode = dstore_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case dstore_3:
				strcpy(instrucoes[i].inst_nome,"dstore_3");
				instrucoes[i].opcode = dstore_3;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case astore_0:
				strcpy(instrucoes[i].inst_nome,"astore_0");
				instrucoes[i].opcode = astore_0;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case astore_1:
				strcpy(instrucoes[i].inst_nome,"astore_1");
				instrucoes[i].opcode = astore_1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case astore_2:
				strcpy(instrucoes[i].inst_nome,"astore_2");
				instrucoes[i].opcode = astore_2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case astore_3:
				strcpy(instrucoes[i].inst_nome,"astore_3");
				instrucoes[i].opcode = astore_3;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case iastore:
				strcpy(instrucoes[i].inst_nome,"iastore");
				instrucoes[i].opcode = iastore;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 3;
			break;
			case lastore:
				strcpy(instrucoes[i].inst_nome,"lastore");
				instrucoes[i].opcode = lastore;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 3;
			break;
			case fastore:
				strcpy(instrucoes[i].inst_nome,"fastore");
				instrucoes[i].opcode = fastore;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 3;
			break;
			case dastore:
				strcpy(instrucoes[i].inst_nome,"dastore");
				instrucoes[i].opcode = dastore;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 3;
			break;
			case aastore:
				strcpy(instrucoes[i].inst_nome,"aastore");
				instrucoes[i].opcode = aastore;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 3;
			break;
			case bastore:
				strcpy(instrucoes[i].inst_nome,"bastore");
				instrucoes[i].opcode = bastore;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 3;
			break;
			case castore:
				strcpy(instrucoes[i].inst_nome,"castore");
				instrucoes[i].opcode = castore;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 3;
			break;
			case sastore:
				strcpy(instrucoes[i].inst_nome,"sastore");
				instrucoes[i].opcode = sastore;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 3;
			break;
			case pop:
				strcpy(instrucoes[i].inst_nome,"pop");
				instrucoes[i].opcode = pop;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;

			break;
			case pop2:
				strcpy(instrucoes[i].inst_nome,"pop2");
				instrucoes[i].opcode = pop2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case dup:
				strcpy(instrucoes[i].inst_nome,"dup");
				instrucoes[i].opcode = dup;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case dup_x1:
				strcpy(instrucoes[i].inst_nome,"dup_x1");
				instrucoes[i].opcode = dup_x1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case dup_x2:
				strcpy(instrucoes[i].inst_nome,"dup_x2");
				instrucoes[i].opcode = dup_x2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 3;
			break;
			case dup2:
				strcpy(instrucoes[i].inst_nome,"dup2");
				instrucoes[i].opcode = dup2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case dup2_x1:
				strcpy(instrucoes[i].inst_nome,"dup2_x1");
				instrucoes[i].opcode = dup2_x1;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 3;
			break;
			case dup2_x2:
				strcpy(instrucoes[i].inst_nome,"dup2_x2");
				instrucoes[i].opcode = dup2_x2;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 4;
			break;
			case swap:
				strcpy(instrucoes[i].inst_nome,"swap");
				instrucoes[i].opcode = swap;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case iadd:
				strcpy(instrucoes[i].inst_nome,"iadd");
				instrucoes[i].opcode = iadd;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case ladd:
				strcpy(instrucoes[i].inst_nome,"ladd");
				instrucoes[i].opcode = ladd;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case fadd:
				strcpy(instrucoes[i].inst_nome,"fadd");
				instrucoes[i].opcode = fadd;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case dadd:
				strcpy(instrucoes[i].inst_nome,"dadd");
				instrucoes[i].opcode = dadd;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case isub:
				strcpy(instrucoes[i].inst_nome,"isub");
				instrucoes[i].opcode = isub;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case lsub:
				strcpy(instrucoes[i].inst_nome,"lsub");
				instrucoes[i].opcode = lsub;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case fsub:
				strcpy(instrucoes[i].inst_nome,"fsub");
				instrucoes[i].opcode = fsub;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case dsub:
				strcpy(instrucoes[i].inst_nome,"dsub");
				instrucoes[i].opcode = dsub;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case imul:
				strcpy(instrucoes[i].inst_nome,"imul");
				instrucoes[i].opcode = imul;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case lmul:
				strcpy(instrucoes[i].inst_nome,"lmul");
				instrucoes[i].opcode = lmul;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case fmul:
				strcpy(instrucoes[i].inst_nome,"fmul");
				instrucoes[i].opcode = fmul;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case dmul:
				strcpy(instrucoes[i].inst_nome,"dmul");
				instrucoes[i].opcode = dmul;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case idiv:
				strcpy(instrucoes[i].inst_nome,"idiv");
				instrucoes[i].opcode = idiv;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case inst_ldiv:
				strcpy(instrucoes[i].inst_nome,"ldiv");
				instrucoes[i].opcode = inst_ldiv;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case fdiv:
				strcpy(instrucoes[i].inst_nome,"fdiv");
				instrucoes[i].opcode = fdiv;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case ddiv:
				strcpy(instrucoes[i].inst_nome,"ddiv");
				instrucoes[i].opcode = ddiv;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case irem:
				strcpy(instrucoes[i].inst_nome,"irem");
				instrucoes[i].opcode = irem;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case lrem:
				strcpy(instrucoes[i].inst_nome,"lrem");
				instrucoes[i].opcode = lrem;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case frem:
				strcpy(instrucoes[i].inst_nome,"frem");
				instrucoes[i].opcode = frem;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case inst_drem:
				strcpy(instrucoes[i].inst_nome,"drem");
				instrucoes[i].opcode = inst_drem;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case ineg:
				strcpy(instrucoes[i].inst_nome,"ineg");
				instrucoes[i].opcode = ineg;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case lneg:
				strcpy(instrucoes[i].inst_nome,"lneg");
				instrucoes[i].opcode = lneg;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case fneg:
				strcpy(instrucoes[i].inst_nome,"fneg");
				instrucoes[i].opcode = fneg;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case dneg:
				strcpy(instrucoes[i].inst_nome,"dneg");
				instrucoes[i].opcode = dneg;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case ishl:
				strcpy(instrucoes[i].inst_nome,"ishl");
				instrucoes[i].opcode = ishl;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case lshl:
				strcpy(instrucoes[i].inst_nome,"lshl");
				instrucoes[i].opcode = lshl;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case ishr:
				strcpy(instrucoes[i].inst_nome,"ishr");
				instrucoes[i].opcode = ishr;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case lshr:
				strcpy(instrucoes[i].inst_nome,"lshr");
				instrucoes[i].opcode = lshr;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case iushr:
				strcpy(instrucoes[i].inst_nome,"iushr");
				instrucoes[i].opcode = iushr;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case lushr:
				strcpy(instrucoes[i].inst_nome,"lushr");
				instrucoes[i].opcode = lushr;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case iand:
				strcpy(instrucoes[i].inst_nome,"iand");
				instrucoes[i].opcode = iand;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case land:
				strcpy(instrucoes[i].inst_nome,"land");
				instrucoes[i].opcode = land;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case ior:
				strcpy(instrucoes[i].inst_nome,"ior");
				instrucoes[i].opcode = ior;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case lor:
				strcpy(instrucoes[i].inst_nome,"lor");
				instrucoes[i].opcode = lor;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case ixor:
				strcpy(instrucoes[i].inst_nome,"ixor");
				instrucoes[i].opcode = ixor;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case lxor:
				strcpy(instrucoes[i].inst_nome,"lxor");
				instrucoes[i].opcode = lxor;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case iinc:
				strcpy(instrucoes[i].inst_nome,"iinc");
				instrucoes[i].opcode = iinc;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 0;
			break;
			case i2l:
				strcpy(instrucoes[i].inst_nome,"i2l");
				instrucoes[i].opcode = i2l;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case i2f:
				strcpy(instrucoes[i].inst_nome,"i2f");
				instrucoes[i].opcode = i2f;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case i2d:
				strcpy(instrucoes[i].inst_nome,"i2d");
				instrucoes[i].opcode = i2d;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case l2i:
				strcpy(instrucoes[i].inst_nome,"l2i");
				instrucoes[i].opcode = l2i;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case l2f:
				strcpy(instrucoes[i].inst_nome,"l2f");
				instrucoes[i].opcode = l2f;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case l2d:
				strcpy(instrucoes[i].inst_nome,"l2d");
				instrucoes[i].opcode = l2d;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case f2i:
				strcpy(instrucoes[i].inst_nome,"f2i");
				instrucoes[i].opcode = f2i;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case f2l:
				strcpy(instrucoes[i].inst_nome,"f2l");
				instrucoes[i].opcode = f2l;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case f2d:
				strcpy(instrucoes[i].inst_nome,"f2d");
				instrucoes[i].opcode = f2d;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case d2i:
				strcpy(instrucoes[i].inst_nome,"d2i");
				instrucoes[i].opcode = d2i;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case d2l:
				strcpy(instrucoes[i].inst_nome,"d2l");
				instrucoes[i].opcode = d2l;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case d2f:
				strcpy(instrucoes[i].inst_nome,"d2f");
				instrucoes[i].opcode = d2f;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case i2b:
				strcpy(instrucoes[i].inst_nome,"i2b");
				instrucoes[i].opcode = i2b;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case i2c:
				strcpy(instrucoes[i].inst_nome,"i2c");
				instrucoes[i].opcode = i2c;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case i2s:
				strcpy(instrucoes[i].inst_nome,"i2s");
				instrucoes[i].opcode = i2s;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case lcmp:
				strcpy(instrucoes[i].inst_nome,"lcmp");
				instrucoes[i].opcode = lcmp;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case fcmpl:
				strcpy(instrucoes[i].inst_nome,"fcmpl");
				instrucoes[i].opcode = fcmpl;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case fcmpg:
				strcpy(instrucoes[i].inst_nome,"fcmpg");
				instrucoes[i].opcode = fcmpg;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case dcmpl:
				strcpy(instrucoes[i].inst_nome,"dcmpl");
				instrucoes[i].opcode = dcmpl;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case dcmpg:
				strcpy(instrucoes[i].inst_nome,"dcmpg");
				instrucoes[i].opcode = dcmpg;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 2;
			break;
			case ifeq:
				strcpy(instrucoes[i].inst_nome,"ifeq");
				instrucoes[i].opcode = ifeq;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case ifne:
				strcpy(instrucoes[i].inst_nome,"ifne");
				instrucoes[i].opcode = ifne;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case iflt:
				strcpy(instrucoes[i].inst_nome,"iflt");
				instrucoes[i].opcode = iflt;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case ifge:
				strcpy(instrucoes[i].inst_nome,"ifge");
				instrucoes[i].opcode = ifge;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case ifgt:
				strcpy(instrucoes[i].inst_nome,"ifgt");
				instrucoes[i].opcode = ifgt;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case ifle:
				strcpy(instrucoes[i].inst_nome,"ifle");
				instrucoes[i].opcode = ifle;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case if_icmpeq:
				strcpy(instrucoes[i].inst_nome,"if_icmpeq");
				instrucoes[i].opcode = if_icmpeq;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 2;
			break;
			case if_icmpne:
				strcpy(instrucoes[i].inst_nome,"if_icmpne");
				instrucoes[i].opcode = if_icmpne;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 2;
			break;
			case if_icmplt:
				strcpy(instrucoes[i].inst_nome,"if_icmplt");
				instrucoes[i].opcode = if_icmplt;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 2;
			break;
			case if_icmpge:
				strcpy(instrucoes[i].inst_nome,"if_icmpge");
				instrucoes[i].opcode = if_icmpge;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 2;
			break;
			case if_icmpgt:
				strcpy(instrucoes[i].inst_nome,"if_icmpgt");
				instrucoes[i].opcode = if_icmpgt;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 2;
			break;
			case if_icmple:
				strcpy(instrucoes[i].inst_nome,"if_icmple");
				instrucoes[i].opcode = if_icmple;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 2;
			break;
			case if_acmpeq:
				strcpy(instrucoes[i].inst_nome,"if_acmpeq");
				instrucoes[i].opcode = if_acmpeq;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 2;
			break;
			case if_acmpne:
				strcpy(instrucoes[i].inst_nome,"if_acmpne");
				instrucoes[i].opcode = if_acmpne;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 2;
			break;
			case inst_goto:
				strcpy(instrucoes[i].inst_nome,"goto");
				instrucoes[i].opcode = inst_goto;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 0;
			break;
			case jsr:
				strcpy(instrucoes[i].inst_nome,"jsr");
				instrucoes[i].opcode = jsr;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 0;
			break;
			case ret:
				strcpy(instrucoes[i].inst_nome,"ret");
				instrucoes[i].opcode = ret;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 0;
			break;
			case tableswitch:
				strcpy(instrucoes[i].inst_nome,"tableswitch");
				instrucoes[i].opcode = tableswitch;
				instrucoes[i].numarg = -1;
				instrucoes[i].numop = 1;
			break;
			case lookupswitch:
				strcpy(instrucoes[i].inst_nome,"lookupswitch");
				instrucoes[i].opcode = lookupswitch;
				instrucoes[i].numarg = -1;
				instrucoes[i].numop = 1;
			break;
			case ireturn:
				strcpy(instrucoes[i].inst_nome,"ireturn");
				instrucoes[i].opcode = ireturn;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case lreturn:
				strcpy(instrucoes[i].inst_nome,"lreturn");
				instrucoes[i].opcode = lreturn;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case freturn:
				strcpy(instrucoes[i].inst_nome,"freturn");
				instrucoes[i].opcode = freturn;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case dreturn:
				strcpy(instrucoes[i].inst_nome,"dreturn");
				instrucoes[i].opcode = dreturn;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case areturn:
				strcpy(instrucoes[i].inst_nome,"areturn");
				instrucoes[i].opcode = areturn;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case inst_return:
				strcpy(instrucoes[i].inst_nome,"return");
				instrucoes[i].opcode = inst_return;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case getstatic:
				strcpy(instrucoes[i].inst_nome,"getstatic");
				instrucoes[i].opcode = getstatic;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 0;
			break;
			case putstatic:
				strcpy(instrucoes[i].inst_nome,"putstatic");
				instrucoes[i].opcode = putstatic;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case getfield:
				strcpy(instrucoes[i].inst_nome,"getfield");
				instrucoes[i].opcode = getfield;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case putfield:
				strcpy(instrucoes[i].inst_nome,"putfield");
				instrucoes[i].opcode = putfield;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 2;
			break;
			case invokevirtual:
				strcpy(instrucoes[i].inst_nome,"invokevirtual");
				instrucoes[i].opcode = invokevirtual;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = -1;
			break;
			case invokespecial:
				strcpy(instrucoes[i].inst_nome,"invokespecial");
				instrucoes[i].opcode = invokespecial;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = -1;
			break;
			case invokestatic:
				strcpy(instrucoes[i].inst_nome,"invokestatic");
				instrucoes[i].opcode = invokestatic;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = -1;
			break;
			case invokeinterface:
				strcpy(instrucoes[i].inst_nome,"invokeinterface");
				instrucoes[i].opcode = invokeinterface;
				instrucoes[i].numarg = 4;
				instrucoes[i].numop = -1;
			break;
			case inst_new:
				strcpy(instrucoes[i].inst_nome,"new");
				instrucoes[i].opcode = inst_new;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 0;
			break;
			case newarray:
				strcpy(instrucoes[i].inst_nome,"newarray");
				instrucoes[i].opcode = newarray;
				instrucoes[i].numarg = 1;
				instrucoes[i].numop = 1;
			break;
			case anewarray:
				strcpy(instrucoes[i].inst_nome,"anewarray");
				instrucoes[i].opcode = anewarray;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case arraylength:
				strcpy(instrucoes[i].inst_nome,"arraylength");
				instrucoes[i].opcode = arraylength;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case athrow:
				strcpy(instrucoes[i].inst_nome,"athrow");
				instrucoes[i].opcode = athrow;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case checkcast:
				strcpy(instrucoes[i].inst_nome,"checkcast");
				instrucoes[i].opcode = checkcast;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case instanceof:
				strcpy(instrucoes[i].inst_nome,"instanceof");
				instrucoes[i].opcode = instanceof;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case monitorenter:
				strcpy(instrucoes[i].inst_nome,"monitorenter");
				instrucoes[i].opcode = monitorenter;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case monitorexit:
				strcpy(instrucoes[i].inst_nome,"monitorexit");
				instrucoes[i].opcode = monitorexit;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 1;
			break;
			case wide:
				strcpy(instrucoes[i].inst_nome,"wide");
				instrucoes[i].opcode = wide;
				instrucoes[i].numarg = 0;
				instrucoes[i].numop = 0;
			break;
			case multianewarray:
				strcpy(instrucoes[i].inst_nome,"multianewarray");
				instrucoes[i].opcode = multianewarray;
				instrucoes[i].numarg = 3;
				instrucoes[i].numop = -1;
			break;
			case ifnull:
				strcpy(instrucoes[i].inst_nome,"ifnull");
				instrucoes[i].opcode = ifnull;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case ifnonnull:
				strcpy(instrucoes[i].inst_nome,"ifnonnull");
				instrucoes[i].opcode = ifnonnull;
				instrucoes[i].numarg = 2;
				instrucoes[i].numop = 1;
			break;
			case goto_w:
				strcpy(instrucoes[i].inst_nome,"goto_w");
				instrucoes[i].opcode = goto_w;
				instrucoes[i].numarg = 4;
				instrucoes[i].numop = 0;
			break;
			case jsr_w:
				strcpy(instrucoes[i].inst_nome,"jsr_w");
				instrucoes[i].opcode = jsr_w;
				instrucoes[i].numarg = 4;
				instrucoes[i].numop = 0;
			break;

			default:
				strcpy(instrucoes[i].inst_nome,"undefined");
				instrucoes[i].opcode = 255;
				instrucoes[i].numarg = -1;
				instrucoes[i].numop = -1;
			break;
		}

		instrucoes[i].pc_instrucao = instrucoes[i].numarg + 1;
	}

	return(instrucoes);
}
