/**
 * @file leitura.c
 * @brief Arquivo cabeçalho contendo funções do leitor/exibidor de .class
 */

/*Inclusão de estruturas e assinatura de funções de leitura*/
#include "leitor_exibidor.h"
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include "instrucoes.h"

/*Macro para leitura de um tipo u1 do arquivo .class*/
#define u1READ(arq) getc(arq)
/*Macro para leitura de um tipo u2 do arquivo .class*/
#define u2READ(arq) (getc(arq) << 8) | getc(arq)
/*Macro para leitura de um tipo u4 do arquivo .class*/
#define u4READ(arq) (getc(arq) << 24) | (getc(arq) << 16 | getc(arq) << 8 | getc(arq))

/*Função para leitura do arquivo .class*/
Classfile* lerArquivo (char *nomeArquivo)
{
	/*Declaração do ponteiro que irá conter a estrutura do arquivo .class*/
	Classfile *arquivoClass = NULL;
	/*Chamada da função 'fopen' para abrir o arquivo .class*/
	FILE *fp = fopen(nomeArquivo,"rb");
	/*Estrutura condicional que avalia se o ponteiro do arquivo é nulo*/
	if (fp == NULL) {
		printf("Erro na abertura do arquivo. O programa sera encerrado...\n");
		exit(1);
	} else {
		/*Caso contrário, prossegue com a leitura do arquivo .class*/
		/*Alocação da estrutura Classfile em memória*/
		arquivoClass = (Classfile*) malloc(sizeof(struct classfile));

		/*Leitura do valor 'magic', representando a Assinatura
		do arquivo .class*/
		arquivoClass->magic = u4READ(fp);
		/*Leitura do valor 'minor_version', representando a versão
		do arquivo .class*/
		arquivoClass->minor_version = u2READ(fp);
		/*Leitura do valor 'major_version', representando a versão
		do arquivo .class*/
		arquivoClass->major_version = u2READ(fp);
		/*Leitura do valor 'constant_pool_count', representando
		a quantidade de entradas na tabela Constant Pool*/
		arquivoClass->constant_pool_count = u2READ(fp);

		/*Leitura da tabela Constant Pool*/
		arquivoClass->constant_pool = NULL;
		arquivoClass->constant_pool = lerConstantPool(fp, arquivoClass->constant_pool_count);

		/*Leitura do valor 'access_flags', representando a especificação
		de permissão de acesso da classe*/
		arquivoClass->access_flags = u2READ(fp);
		/*Leitura do valor 'this_class' representandoc a classe definida*/
		arquivoClass->this_class = u2READ(fp);
		/*Leitura do valor 'super_class' representando a super classe
		direta da classe definida*/
		arquivoClass->super_class = u2READ(fp);
		/*Leitura do valor 'interfaces_count', representando
		a quantidade de entradas na tabela Interfaces*/
		arquivoClass->interfaces_count = u2READ(fp);

		/*Estrutura condicional que verifica se a quantidade de entradas
		na tabela Interfaces é maior que zero. Se for, prossegue com a leitura
		das entradas, caso contrário prossegue com a leitura dos próximos campos*/
		if (arquivoClass->interfaces_count > 0) {
			// Preencher com leitura de interfaces - CODIFICAR
			arquivoClass->interfaces = lerInterfaces(fp, arquivoClass->interfaces_count);
		}

		/*Leitura do valor 'fields_count', representando
		a quantidade de entradas na tabela Fields*/
		arquivoClass->fields_count = u2READ(fp);

		/*Estrutura condicional que verifica se a quantidade de entradas
		na tabela Fields é maior que zero. Se for, prossegue com a leitura
		das entradas, caso contrário prossegue com a leitura dos próximos campos*/
		if (arquivoClass->fields_count > 0) {
			// Preencher com leitura de fields - CODIFICAR
			arquivoClass->fields = lerField(fp, arquivoClass->fields_count, arquivoClass->constant_pool);
		}

		/*Leitura do valor 'methods_count', representando
		a quantiade de entradas na tabela Method*/
		arquivoClass->methods_count = u2READ(fp);

		/*Estrutura condicional que verifica se a quantidade de entradas
		na tabela Method é maior que zero. Se for, prossegue com a leitura
		das entradas, caso contrário prossegue com a leitura dos próximos campos*/
		if (arquivoClass->methods_count > 0) {
			/*Chamada da função 'lerMethod' para realizar a leitura da tabela Method*/
			arquivoClass->methods = lerMethod(fp, arquivoClass->methods_count,arquivoClass->constant_pool);
		}

		/*Leitura do valor 'attributes_count', representando
		a quantidade de entradas na tabela Attributes*/
		arquivoClass->attributes_count = u2READ(fp);

		/*Estrutura condicional que verifica se a quantidade de entradas
		na tabela Attributes é maior que zero. Se for, prossegue com a leitura
		das entradas, caso contrário prossegue com a leitura dos próximos campos*/
		if(arquivoClass->attributes_count > 0){
			/*Chamada da função 'lerAttributes' para realizar a leitura da tabela Attributes*/
			arquivoClass->attributes = (Attribute_info**) malloc(arquivoClass->attributes_count*sizeof(Attribute_info*));
			for (int posicao = 0; posicao < arquivoClass->attributes_count; posicao++) {
				*(arquivoClass->attributes+posicao) = lerAttributes(fp, arquivoClass->constant_pool);
			}
		}
		/*Fechamento do arquivo .class*/
		fclose(fp);

		/*Retorno do ponteiro da estrutura contendo o conteúdo do arquivo .class*/
		return arquivoClass;
	}
}

/*Função 'lerConstantPool' que realiza a leitua da tabela Constant Pool*/
Cp_info* lerConstantPool (FILE *fp, u2 constant_pool_count)
{
	/*Alocação da estrutura Constant Pool que será retornada para a estrutura
	principal do arquivo .class*/
	Cp_info *constantPool = (Cp_info *) malloc((constant_pool_count-1)*sizeof(struct cp_info));
	/*Ponteiro auxiliar do tipo Cp_info para fazer a varredura de leitura*/
	Cp_info *aux = NULL;
	/*Estrutura de repetição contada que realiza a leitura das informações
	contidas na Constant Pool presente no arquvo .class*/
	for (aux = constantPool; aux < constantPool+constant_pool_count-1; aux++){
		/*Leitura do byte tag de uma entrada da Constant Pool*/
		aux->tag = u1READ(fp);
		/*Estrutura 'switch case' que analisa o byte tag lido e de acordo com o valor
		realiza um procedimento específico de leitura*/
		switch(aux->tag) {
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_Class,
			realiza a leitura do atributo name_index da estrutura Class*/
			case CONSTANT_Class:
				aux->UnionCP.Class.name_index = u2READ(fp);
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_Fieldref,
			realiza a leitura dos atributos class_index e name_and_type_index
			da estrutura Fieldref*/
			case CONSTANT_Fieldref:
				aux->UnionCP.Fieldref.class_index = u2READ(fp);
				aux->UnionCP.Fieldref.name_and_type_index = u2READ(fp);
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_Methodref,
			realiza a leitura dos atributos class_index e name_and_type_index
			da estrutura Methodref*/
			case CONSTANT_Methodref:
				aux->UnionCP.Methodref.class_index = u2READ(fp);
				aux->UnionCP.Methodref.name_and_type_index = u2READ(fp);
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_InterfaceMethodref,
			realiza a leitura dos atributos class_index e name_and_type_index da estrutura
			InterfaceMethodref*/
			case CONSTANT_InterfaceMethodref:
				aux->UnionCP.InterfaceMethodref.class_index = u2READ(fp);
				aux->UnionCP.InterfaceMethodref.name_and_type_index = u2READ(fp);
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_String,
			realiza a leitura do atributo string_index da estrutura String*/
			case CONSTANT_String:
				aux->UnionCP.String.string_index = u2READ(fp);
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_Integer,
			realiza a leitura do atributo bytes da estrutura Integer*/
			case CONSTANT_Integer:
				aux->UnionCP.Integer.bytes = u4READ(fp);
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_Float,
			realiza a leitura do atributo bytes da estrutura Float*/
			case CONSTANT_Float:
				aux->UnionCP.Float.bytes = u4READ(fp);
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_Long,
			realiza a leitura dos atributos high_bytes e low_bytes da estrutura
			Long*/
			case CONSTANT_Long:
				aux->UnionCP.Long.high_bytes = u4READ(fp);
				aux->UnionCP.Long.low_bytes = u4READ(fp);
				aux++;
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_Double,
			realiza a leitura dos atributos high_bytes e low_bytes da estrutura
			Double*/
			case CONSTANT_Double:
				aux->UnionCP.Double.high_bytes = u4READ(fp);
				aux->UnionCP.Double.low_bytes = u4READ(fp);
				aux++;
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_NameAndType,
			realiza a leitura dos atributos name_index e descriptor_index da estrutura
			NameAndType*/
			case CONSTANT_NameAndType:
				aux->UnionCP.NameAndType.name_index = u2READ(fp);
				aux->UnionCP.NameAndType.descriptor_index = u2READ(fp);
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_Long,
			realiza a leitura dos atributos length e bytes da estrutura UTF8*/
			case CONSTANT_Utf8:
				aux->UnionCP.UTF8.length = u2READ(fp);
				aux->UnionCP.UTF8.bytes = malloc(aux->UnionCP.UTF8.length*sizeof(u1));
				for (u1 *i=aux->UnionCP.UTF8.bytes;i<aux->UnionCP.UTF8.bytes+aux->UnionCP.UTF8.length;i++){
					*i = u1READ(fp);
				}
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_MethodHandle,
			realiza a leitura dos atributos reference_kind e reference_index da estrutura
			MethodHandle*/
			case CONSTANT_MethodHandle:
				aux->UnionCP.MethodHandle.reference_kind = u1READ(fp);
				aux->UnionCP.MethodHandle.reference_index = u2READ(fp);
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_MethodType,
			realiza a leitura do atributo descriptor_index da estrutura MethodType*/
			case CONSTANT_MethodType:
				aux->UnionCP.MethodType.descriptor_index = u2READ(fp);
				break;
			/*Caso o byte tag lido represente o valor de tag de CONSTANT_InvokeDynamic,
			realiza a leitura dos atributos bootstrap_method_attr_index e
			name_and_type_index da estrutura InvokeDynamicInfo*/
			case CONSTANT_InvokeDynamic:
				aux->UnionCP.InvokeDynamicInfo.bootstrap_method_attr_index = u2READ(fp);
				aux->UnionCP.InvokeDynamicInfo.name_and_type_index = u2READ(fp);
				break;
			/*Caso um byte tag lido não tenha sido tratado na estrutura switch case,
			realiza a impressão de uma mensagem de debug*/
			default:
				printf("Default\n");
				break;
		}
	}

	/*Retorno da estrutura Constant Pool alocada, com as informações lidas*/
	return constantPool;
}

u2* lerInterfaces (FILE *fp, u2 size) {
	u2 *interfaces = malloc(size*sizeof(u2));
	for (u2 *auxInterfaces = interfaces; auxInterfaces < interfaces+size; auxInterfaces++) {
		*auxInterfaces = u2READ(fp);
	}
	return interfaces;
}

Field_info *lerField (FILE *fp, u2 fields_count, Cp_info * cp) {
	Field_info * fields = (Field_info*)malloc(fields_count*sizeof(struct field_info));
	for (Field_info * i = fields; i < fields + fields_count; i++) {
		i->access_flags = u2READ(fp);
		i->name_index = u2READ(fp);
		i->descriptor_index = u2READ(fp);
		i->attributes_count = u2READ(fp);

		if (i->attributes_count > 0) {
			i->attributes = (Attribute_info**)malloc(i->attributes_count*sizeof(Attribute_info*));
			for (int posicao = 0; posicao < i->attributes_count; posicao++) {
				*(i->attributes+posicao) = lerAttributes(fp, cp);
			}
		}
	}

	return fields;
}

/*Função 'lerMethod' para realizar a leitura da tabela Method*/
Method_info * lerMethod (FILE * fp, u2 methods_count, Cp_info *cp) {
	/*Alocação da estrutura Method que será retornada para a estrutura
	principal do arquivo .class*/
	Method_info * methods = (Method_info*) malloc(methods_count*sizeof(struct method_info));

	/*Estrutura de repetição contada que realiza a leitura das informações
	contidas na tabela Method presente no arquvo .class*/
	for (Method_info * i = methods; i < methods + methods_count; i++) {
		/*Leitura do atributo access_flags do respectivo método*/
		i->access_flags = u2READ(fp);
		/*Leitura do atributo name_index do respectivo método*/
		i->name_index = u2READ(fp);
		/*Leitura do atributo descriptor_index do respectivo método*/
		i->descriptor_index = u2READ(fp);
		/*Leitura do atributo attributes_count do respectivo método*/
		i->attributes_count = u2READ(fp);

		/*Estrutura condicional que avalia se a quantidade de atributos
		do método é maior que zero. Se for, prossegue com a leitura dos
		atributos do método*/
		// Attribute_info *attributes = (Attribute_info*) malloc(i->attributes_count*sizeof(Attribute_info));

		if (i->attributes_count > 0) {
			i->attributes = (Attribute_info**)malloc(i->attributes_count*sizeof(Attribute_info*));
			for (int posicao = 0; posicao < i->attributes_count; posicao++) {
				*(i->attributes+posicao) = lerAttributes(fp, cp);
			}
		}
	}

	/*Retorno da estrutura Method alocada, com as informações lidas*/
	return methods;
}

char* decodificarCode(Cp_info *cp, u2 sizeCP, u1 *code, u4 length,Instrucao *instrucoes){
	u1 *aux=NULL,*aux3=NULL,*aux5=NULL,*aux7=NULL, *aux_inc1=NULL, *aux_inc2=NULL;
	char *retorno = (char*)malloc(10000*sizeof(char));
	char *stringaux = (char*)malloc(100*sizeof(char));
	u2 *aux2=NULL,*aux4=NULL;
	u2 *indexbyte=NULL;
	i2 *constbyte = NULL;
	// u2 tag = 0;
	char *stringargs=NULL;
	char *stringdecod=NULL;
	strcpy(retorno,"");
	int pc = 0;

	for(aux=code;aux<code+length;){
		int numarg = instrucoes[*aux].numarg;
		int opcode = instrucoes[*aux].opcode;
		char *nomeinst = malloc(100*sizeof(char));
		sprintf(stringaux,"%d ",pc);
		strcat(retorno,stringaux);
		strcpy(nomeinst,instrucoes[*aux].inst_nome);
		strcat(retorno,instrucoes[*aux].inst_nome);
		pc+=instrucoes[*aux].pc_instrucao;
		switch(numarg){
			case 0:
				if(opcode==wide){
					// Leitura do Wide do iinc aqui
					aux3 = malloc(sizeof(u1));
					*aux3 = *(++aux);

					if(instrucoes[*aux3].opcode==iinc){

						indexbyte = malloc(sizeof(u2));
						*indexbyte = *(++aux) << 8;
						*indexbyte |= *(++aux);

						constbyte = malloc(sizeof(i2));
						*constbyte = (i2) *(++aux) << 8;
						*constbyte |= (i2) *(++aux);

						sprintf(stringaux,"\n%s %d by %d\n",instrucoes[*aux3].inst_nome,(int) *indexbyte,(int) *constbyte);
						strcat(retorno,stringaux);
					}
					else{

						aux2 = (u2*) malloc(sizeof(u2));
						*aux2 = *(++aux) << 8;
						*aux2 |= *(++aux);

						sprintf(stringaux,"\n%s %d\n",instrucoes[*aux3].inst_nome, (int) *aux2);
						strcat(retorno,stringaux);
					}
				}
				else{
					strcat(retorno,"\n");
				}
				aux++;
			break;
			case 1:
				if(opcode==bipush || opcode==istore || opcode == iload || opcode == lstore || opcode == lload || opcode == fstore || opcode == fload || opcode == dstore || opcode == dload){
					sprintf(stringaux," %d\n",*(++aux));
					strcat(retorno,stringaux);
				}
				else if(opcode == newarray){
					aux3 = (u1*) malloc(sizeof(u1));
					*aux3 = *(++aux);
					sprintf(stringaux," %d ",*aux3);
					switch(*aux3){
						case T_BOOLEAN:
							strcat(stringaux,"(boolean)\n");
						break;

						case T_CHAR:
							strcat(stringaux,"(char)\n");
						break;

						case T_FLOAT:
							strcat(stringaux,"(float)\n");
						break;

						case T_DOUBLE:
							strcat(stringaux,"(double)\n");
						break;

						case T_BYTE:
							strcat(stringaux,"(byte)\n");
						break;

						case T_SHORT:
							strcat(stringaux,"(short)\n");
						break;

						case T_INT:
							strcat(stringaux,"(int)\n");
						break;

						case T_LONG:
							strcat(stringaux,"(long)\n");
						break;
					}

					strcat(retorno,stringaux);
				}

				else{
					sprintf(stringaux,"%d ",*(++aux));
					strcat(retorno," #");
					strcat(retorno,stringaux);
					stringdecod = decodificarOperandoInstrucao(cp,*aux,sizeCP);
					strcat(retorno,stringdecod);
					strcat(retorno,"\n");
				}
				aux++;
			break;

			case 2:

				if(opcode==iinc){
					aux_inc1 = (u1 *) malloc(sizeof(u1));
					*aux_inc1 = *(++aux);
					aux_inc2 = (u1 *) malloc(sizeof(u1));
					*aux_inc2 = *(++aux);


					sprintf(stringaux," %d by %d\n",*(aux_inc1),*(aux_inc2));
					strcat(retorno,stringaux);
				}
				else if(opcode==ifeq || opcode==ifne || opcode==iflt || opcode==ifge || opcode==ifgt || opcode==ifle || opcode==if_icmpeq || opcode==if_icmpne || opcode==if_icmplt || opcode==if_icmpge || opcode==if_icmpgt || opcode==if_icmple){
					aux3 = (u1 *) malloc(sizeof(u1));
					aux5 = (u1 *) malloc(sizeof(u1));
					aux7 = (u1 *) malloc(sizeof(u1));
					*aux7 = pc-instrucoes[*aux].pc_instrucao;
					*aux3 = (u1) *(++aux);
					*aux5 = (u1) *(++aux);

					*aux3 = (*aux5)+(*aux7);

					if(*aux3<*aux7){
						sprintf(stringaux," %d (-%d)\n",*aux3,*aux5);
					}
					else{
						sprintf(stringaux," %d (+%d)\n",*aux3,*aux5);
					}
					strcat(retorno,stringaux);
				}
				else if(opcode==sipush){
					aux2 = (u2 *) malloc(sizeof(u2));
					aux3 = (u1 *) malloc(sizeof(u1));
					aux5 = (u1 *) malloc(sizeof(u1));

					*aux3 = *(++aux);
					*aux5 = *(++aux);

					*aux2 = ((*aux3) << 8) | *aux5;

					sprintf(stringaux," %d\n",*aux2);
					strcat(retorno,stringaux);
				}
				else{

					aux2 = (u2*)malloc(sizeof(u2));
					*aux2 = *(++aux) << 8;
					*aux2 |= *(++aux);

					//stringargs = (char*)malloc(100*sizeof(char));
					stringargs = decodificarOperandoInstrucao(cp,*aux2,sizeCP);
					strcat(retorno," #");
					sprintf(stringaux,"%d",(int)*aux2);
					strcat(retorno,stringaux);
					strcat(retorno," ");
					strcat(retorno,stringargs);
					strcat(retorno,"\n");
				}
				aux++;
			break;

			case 4:
				// Fazer leitura do wide quando não é iinc

				aux2 = (u2*) malloc(sizeof(u2));
				*aux2 = *(++aux) << 8;
				*aux2 |= *(++aux);

				stringargs = decodificarOperandoInstrucao(cp,*aux2,sizeCP);
				strcat(retorno," #");
				sprintf(stringaux,"%d",(int)*aux2);
				strcat(retorno,stringaux);
				strcat(retorno," ");
				strcat(retorno,stringargs);
				aux4 = (u2*) malloc(sizeof(u2));
				*aux4 = *(++aux);
				sprintf(stringaux,"%d",(int)*aux4);
				strcat(retorno," count ");
				strcat(retorno,stringaux);
				strcat(retorno,"\n");
				if(strcmp(nomeinst,"invokeinterface")==0){
					aux++; // Incrementar porque é o 0;
				}

				aux++;
			break;

			default:
				strcat(retorno,"undefined");
				aux++;
			break;
		}

	}

	if(stringargs!=NULL){
		free(stringargs);
	}
	if(aux2!=NULL){
		free(aux2);
	}

	if(stringdecod!=NULL){
		free(stringdecod);
	}

	if(stringaux!=NULL){
		free(stringaux);
	}
	return(retorno);
}

Code_attribute * lerCode (FILE * fp, Cp_info *cp) {
	Code_attribute * code_attributes = NULL;
	code_attributes = (Code_attribute*) malloc(sizeof(Code_attribute));
	code_attributes->max_stack = u2READ(fp);
	code_attributes->max_locals = u2READ(fp);
	code_attributes->code_length = u4READ(fp);
	if(code_attributes->code_length > 0) {
		code_attributes->code = malloc(code_attributes->code_length*sizeof(u1));
		for(u1 *p = code_attributes->code; p < code_attributes->code + code_attributes->code_length; p++){
			*p = u1READ(fp);
		}
	}

	code_attributes->exception_info_length = u2READ(fp);

	if(code_attributes->exception_info_length > 0){
		code_attributes->ex_info = NULL;//malloc(code_attributes->exception_table_length*sizeof(exception_table));
		code_attributes->ex_info = lerExceptionTable(fp, code_attributes->exception_info_length);
	}

	code_attributes->attributes_count = u2READ(fp);
	// u2 auxCodeAttrCount = code_attributes->attributes_count;
	// char * string_name_index = malloc(100*sizeof(char));

	if (code_attributes->attributes_count > 0){
		code_attributes->attributes = (Attribute_info**)malloc(code_attributes->attributes_count*sizeof(Attribute_info*));
		for (int posicao = 0; posicao < code_attributes->attributes_count; posicao++) {
			*(code_attributes->attributes+posicao) = lerAttributes(fp, cp);
		}
	}

	return code_attributes;
}

Line_number_table * lerLineNumberTable(FILE * fp, Cp_info * cp) {
	Line_number_table * lnt = (Line_number_table*)malloc(sizeof(struct line_number_table));
	lnt->line_number_table_length = u2READ(fp);
	if (lnt->line_number_table_length > 0) {
		lnt->info = (Line_number_info*)malloc(lnt->line_number_table_length*sizeof(struct line_number_info));
		for (Line_number_info * linfo = lnt->info; linfo < lnt->info+lnt->line_number_table_length; linfo++) {
			linfo->start_pc = u2READ(fp);
			linfo->line_number = u2READ(fp);
		}
	}
	return lnt;
}

Exception_table * lerExceptionTable (FILE * fp, u2 size) {
	Exception_table * exception_tableAux = (Exception_table*)malloc(size*sizeof(Exception_table));
	for(Exception_table * e = exception_tableAux; e < exception_tableAux + size; e++){
		e->start_pc = u2READ(fp);
		e->end_pc = u2READ(fp);
		e->handler_pc = u2READ(fp);
		e->catch_type = u2READ(fp);
	}
	return exception_tableAux;
}

/*Função 'lerAttributes' para realizar a leitura da tabela Attribute*/
Attribute_info * lerAttributes (FILE * fp, Cp_info * cp) {

	/*Alocação da estrutura Attribute que será retornada para a estrutura
	principal do arquivo .class*/
	Attribute_info * attributes = (Attribute_info*) malloc(sizeof(struct attribute_info));
	/*Leitura do atributo name_index do respectivo atributo*/
	attributes->attribute_name_index = u2READ(fp);
	/*Leitura do atributo length do respectivo atributo*/
	attributes->attribute_length = u4READ(fp);
	/*Estrutura condicional que avalia se o tamanho do atributo
	é maior que zero. Se for, prossegue com a leitura da informação
	do atributo*/
	if (attributes->attribute_length > 0) {
			char * string_name_index;// = malloc(100*sizeof(char));
			string_name_index = decodificaStringUTF8(cp+attributes->attribute_name_index-1);
			//VERIFICAR SE ELE SO ALOCA NO MAXIMO 1 TIPO, LINENUMBER-CODE-ETC,por chamada
			if(strcmp(string_name_index,"SourceFile") == 0){
				Source_file_attribute *SourceFile = NULL;//(code_attribute*) malloc(i->attributes_count*sizeof(code_attribute));
				SourceFile = lerSourceFile(fp);
				attributes->info = NULL;//malloc(i->attributes_count*sizeof(code_attribute));
				attributes->info = (Source_file_attribute*) SourceFile;
			} else if (strcmp(string_name_index,"Code") == 0) {
				Code_attribute * code_attr = NULL;
				code_attr = lerCode(fp ,cp);
				attributes->info = (Code_attribute*) code_attr;
			} else if (strcmp(string_name_index,"LineNumberTable") == 0) {
				Line_number_table * lnt = NULL;
				lnt = lerLineNumberTable(fp, cp);
				attributes->info = (Line_number_table*)lnt;
			} else if (strcmp(string_name_index,"StackMapTable") == 0) {
				StackMapTable_attribute * stackMapTable = NULL;
				stackMapTable = lerStackMapTable(fp);
				attributes->info = (StackMapTable_attribute*)stackMapTable;
			} else if (strcmp(string_name_index, "InnerClasses") == 0) {
				InnerClasses_attribute * innerClasses = NULL;
				innerClasses = lerInnerClasses(fp, cp);
				attributes->info = (InnerClasses_attribute*)innerClasses;
			} else if (strcmp(string_name_index,"Signature") == 0) {
				Signature_attribute * signatureR = NULL;
				signatureR = lerSignature(fp);
				attributes->info = (Signature_attribute*)signatureR;
			} else if (strcmp(string_name_index,"ConstantValue") == 0) {
				ConstantValue_attribute * constantV = NULL;
				constantV = lerConstantValue(fp);
				attributes->info = (ConstantValue_attribute*)constantV;
			} else if (strcmp(string_name_index,"Exceptions") == 0) {
				Exceptions_attribute * exceptions = NULL;
				exceptions = lerExceptionsAttribute(fp);
				attributes->info = (Exceptions_attribute*)exceptions;
			}
	}
	/*Retorno da estrutura Attribute alocada, com as informações lidas*/
	return attributes;
}

Exceptions_attribute * lerExceptionsAttribute (FILE * fp) {
	Exceptions_attribute * exceptions = (Exceptions_attribute*)malloc(sizeof(struct exceptions_attribute));
	exceptions->number_of_exceptions = u2READ(fp);
	exceptions->exception_index_info = NULL;
	if (exceptions->number_of_exceptions > 0) {
		exceptions->exception_index_info = (u2*)malloc(exceptions->number_of_exceptions*sizeof(u2));
		for (u2 * excpAux = exceptions->exception_index_info; excpAux < exceptions->exception_index_info + exceptions->number_of_exceptions; excpAux++) {
			*excpAux = u2READ(fp);
		}
	}
	return exceptions;
}

ConstantValue_attribute * lerConstantValue (FILE * fp) {
	ConstantValue_attribute * cv = (ConstantValue_attribute*)malloc(sizeof(ConstantValue_attribute));
	cv->constantvalue_index = u2READ(fp);
	return cv;
}

Signature_attribute * lerSignature (FILE * fp) {
	Signature_attribute * signature = (Signature_attribute*)malloc(sizeof(Signature_attribute));
	signature->signature_index = u2READ(fp);
	return signature;
}

InnerClasses_attribute * lerInnerClasses (FILE * fp, Cp_info * cp) {
	InnerClasses_attribute * innerClasses = (InnerClasses_attribute*)malloc(sizeof(InnerClasses_attribute));
	innerClasses->number_of_classes = u2READ(fp);
	if (innerClasses->number_of_classes > 0) {
		innerClasses->classes_array = (Classes**)malloc(innerClasses->number_of_classes*sizeof(Classes*));
		for (int posicao = 0; posicao < innerClasses->number_of_classes; posicao++) {
			*(innerClasses->classes_array+posicao) = lerClasses(fp);
		}
	}
	return innerClasses;
}

Classes *lerClasses (FILE *fp) {
	Classes *classeRetorno = (Classes*) malloc(sizeof(Classes));
	classeRetorno->inner_class_info_index = u2READ(fp);
	classeRetorno->outer_class_info_index = u2READ(fp);
	classeRetorno->inner_name_index = u2READ(fp);
	classeRetorno->inner_class_access_flags = u2READ(fp);

	return classeRetorno;
}

StackMapTable_attribute * lerStackMapTable (FILE * fp) {
	StackMapTable_attribute * stackMapTable = (StackMapTable_attribute*)malloc(sizeof(StackMapTable_attribute));
	stackMapTable->number_of_entries = u2READ(fp);
	if (stackMapTable->number_of_entries > 0) {
		stackMapTable->entries = (Stack_map_frame**)malloc(stackMapTable->number_of_entries*sizeof(Stack_map_frame*));
		for (int posicao = 0; posicao < stackMapTable->number_of_entries; posicao++) {
			*(stackMapTable->entries+posicao) = lerStackMapFrame(fp);
		}
	}
	return stackMapTable;
}

Stack_map_frame * lerStackMapFrame (FILE * fp) {
	Stack_map_frame * StackMapFrame = (Stack_map_frame*)malloc(sizeof(Stack_map_frame));
	StackMapFrame->frame_type = u1READ(fp);
	if (StackMapFrame->frame_type >= 0 && StackMapFrame->frame_type <= 63) {
	} else if (StackMapFrame->frame_type >= 64 && StackMapFrame->frame_type <= 127) {
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame.stack = (Verification_type_info**)malloc(sizeof(Verification_type_info*));
		*(StackMapFrame->map_frame_type.same_locals_1_stack_item_frame.stack) = lerVerificationTypeInfo(fp);
	} else if (StackMapFrame->frame_type == 247) {
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta = u2READ(fp);
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.stack = (Verification_type_info**)malloc(sizeof(Verification_type_info*));
		*(StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.stack) = lerVerificationTypeInfo(fp);
	} else if (StackMapFrame->frame_type >= 248 && StackMapFrame->frame_type <= 250) {
		StackMapFrame->map_frame_type.chop_frame.offset_delta = u2READ(fp);
	} else if (StackMapFrame->frame_type == 251) {
		StackMapFrame->map_frame_type.same_frame_extended.offset_delta = u2READ(fp);
	} else if (StackMapFrame->frame_type >= 252 && StackMapFrame->frame_type <= 254) {
		StackMapFrame->map_frame_type.append_frame.offset_delta = u2READ(fp);
		u2 sizeMalloc = (StackMapFrame->frame_type-251);
		StackMapFrame->map_frame_type.append_frame.locals = (Verification_type_info**)malloc(sizeMalloc*sizeof(Verification_type_info*));
		for (int posicao = 0; posicao < sizeMalloc; posicao++) {
			*(StackMapFrame->map_frame_type.append_frame.locals+posicao) = lerVerificationTypeInfo(fp);
		}
	} else if (StackMapFrame->frame_type == 255) {
		StackMapFrame->map_frame_type.full_frame.offset_delta = u2READ(fp);
		StackMapFrame->map_frame_type.full_frame.number_of_locals = u2READ(fp);
		if (StackMapFrame->map_frame_type.full_frame.number_of_locals > 0) {
			StackMapFrame->map_frame_type.full_frame.locals = (Verification_type_info**)malloc(StackMapFrame->map_frame_type.full_frame.number_of_locals*sizeof(Verification_type_info*));
			for (int posicao = 0; posicao < StackMapFrame->map_frame_type.full_frame.number_of_locals; posicao++) {
				*(StackMapFrame->map_frame_type.full_frame.locals+posicao) = lerVerificationTypeInfo(fp);
				if ((*(StackMapFrame->map_frame_type.full_frame.locals+posicao))->tag == 7) {
				}
			}
		}
		StackMapFrame->map_frame_type.full_frame.number_of_stack_items = u2READ(fp);
		if (StackMapFrame->map_frame_type.full_frame.number_of_stack_items > 0) {
			StackMapFrame->map_frame_type.full_frame.stack = (Verification_type_info**)malloc(StackMapFrame->map_frame_type.full_frame.number_of_stack_items*sizeof(Verification_type_info*));
			for (int posicao = 0; posicao < StackMapFrame->map_frame_type.full_frame.number_of_stack_items; posicao++) {
				*(StackMapFrame->map_frame_type.full_frame.stack+posicao) = lerVerificationTypeInfo(fp);
			}
		}
	}
	return StackMapFrame;
}

Verification_type_info * lerVerificationTypeInfo (FILE * fp) {
	Verification_type_info * VTI = (Verification_type_info*)malloc(sizeof(Verification_type_info));
	VTI->tag = u1READ(fp);
	switch (VTI->tag) {
		case 7:
			VTI->type_info.object_variable_info.cpool_index = u2READ(fp);
			break;
		case 8:
			VTI->type_info.uninitialized_variable_info.offset = u2READ(fp);
			break;
		default:
			break;
	}

	return VTI;
}

Source_file_attribute * lerSourceFile (FILE * fp) {
	Source_file_attribute * SourceFile = NULL;
	SourceFile = (Source_file_attribute*)malloc(sizeof(Source_file_attribute));
	SourceFile->source_file_index = u2READ(fp);
	return SourceFile;
}

char* buscaNomeTag(u1 tag){
	char *nometag = malloc(40*sizeof(char));
	switch(tag){
		case CONSTANT_Class:
			strcpy(nometag,"CONSTANT_Class_Info");
			break;
		case CONSTANT_Fieldref:
			strcpy(nometag,"CONSTANT_Fieldref_Info");
			break;
		case CONSTANT_Methodref:
			strcpy(nometag,"CONSTANT_Methodref_Info");
			break;
		case CONSTANT_InterfaceMethodref:
			strcpy(nometag,"CONSTANT_InterfaceMethodref_Info");
			break;
		case CONSTANT_String:
			strcpy(nometag,"CONSTANT_String_Info");
			break;
		case CONSTANT_Integer:
			strcpy(nometag,"CONSTANT_Integer_Info");
			break;
		case CONSTANT_Float:
			strcpy(nometag,"CONSTANT_Float_Info");
			break;
		case CONSTANT_Long:
			strcpy(nometag,"CONSTANT_Long_Info");
			break;
		case CONSTANT_Double:
			strcpy(nometag,"CONSTANT_Double_Info");
			break;
		case CONSTANT_NameAndType:
			strcpy(nometag,"CONSTANT_NameAndType_Info");
			break;
		case CONSTANT_Utf8:
			strcpy(nometag,"CONSTANT_Utf8_Info");
			break;
		case CONSTANT_MethodHandle:
			strcpy(nometag,"CONSTANT_MethodHandle_Info");
			break;
		case CONSTANT_MethodType:
			strcpy(nometag,"CONSTANT_MethodType_Info");
			break;
		case CONSTANT_InvokeDynamic:
			strcpy(nometag,"CONSTANT_InvokeDynamic_Info");
			break;
		default:
			return NULL;
			break;
	}

	return(nometag);
}

char* decodificaStringUTF8(Cp_info *cp){
	char* retorno = malloc((cp->UnionCP.UTF8.length+1)*sizeof(char));
	char *auxretorno = retorno;

	for (u1 *aux=cp->UnionCP.UTF8.bytes;aux<cp->UnionCP.UTF8.bytes+cp->UnionCP.UTF8.length;aux++){
		*auxretorno = (char) *aux;
		auxretorno++;
	}

	*auxretorno = '\0';

	return(retorno);
}

char* decodificarOperandoInstrucao(Cp_info *cp,u2 index, u2 sizeCP){

	char *retorno = malloc(100*sizeof(char));
	char *stringNomeClasse;// = malloc(100*sizeof(char));
	char *stringNomeMetodo;// = malloc(100*sizeof(char));
	char *stringGeral;// = malloc(100*sizeof(char));
	char *ponteiro2pontos;// = malloc(100*sizeof(char));
	Cp_info *cp_aux = cp+index-1;
	long long saidaLong;
	double valorSaida;
	float saidaFloat;


	if (index < sizeCP) {
		switch(cp_aux->tag){
			case CONSTANT_Methodref:

				// String do Class Name do Methodref
				// Concatenar com .
				// String do name do Name_and_type do Methodref

				stringNomeClasse = decodificaNIeNT(cp,cp_aux->UnionCP.Methodref.class_index,NAME_INDEX);

				stringNomeMetodo = decodificaNIeNT(cp,cp_aux->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);

				ponteiro2pontos = strchr(stringNomeMetodo,':');
				*ponteiro2pontos = '\0';


				strcpy(retorno,"<");
				strcat(retorno,stringNomeClasse);
				strcat(retorno,".");
				strcat(retorno,stringNomeMetodo);
				strcat(retorno,">");
			break;

			case CONSTANT_InterfaceMethodref:
				stringNomeClasse = decodificaNIeNT(cp,cp_aux->UnionCP.InterfaceMethodref.class_index,NAME_INDEX);
				stringNomeMetodo = decodificaNIeNT(cp,cp_aux->UnionCP.InterfaceMethodref.name_and_type_index,NAME_AND_TYPE);

				ponteiro2pontos = strchr(stringNomeMetodo,':');
				*ponteiro2pontos = '\0';

				strcpy(retorno,"<");
				strcat(retorno,stringNomeClasse);
				strcat(retorno,".");
				strcat(retorno,stringNomeMetodo);
				strcat(retorno,">");
			break;

			case CONSTANT_Fieldref:

				stringNomeClasse = decodificaNIeNT(cp,cp_aux->UnionCP.Fieldref.class_index,NAME_INDEX);
				stringGeral = decodificaNIeNT(cp,cp_aux->UnionCP.Fieldref.name_and_type_index,NAME_AND_TYPE);
				// printf("String Geral: %s\n",stringGeral);


				ponteiro2pontos = strchr(stringGeral,':');
				*ponteiro2pontos = '\0';

				// stringGeral = strncpy(strstr(stringGeral,&stringGeral[1]),stringGeral,strlen(stringGeral));

				strcpy(retorno,"<");
				strcat(retorno,stringNomeClasse);
				strcat(retorno,".");
				strcat(retorno,stringGeral);
				strcat(retorno,">");
			break;

			case CONSTANT_String:

				stringGeral = decodificaNIeNT(cp,cp_aux->UnionCP.String.string_index,STRING_INDEX);

				strcpy(retorno,"<");
				strcat(retorno,stringGeral);
				strcat(retorno,">");
			break;

			case CONSTANT_Class:
				stringGeral = decodificaNIeNT(cp,cp_aux->UnionCP.Class.name_index,CLASS_INDEX);
				strcpy(retorno,"<");
				strcat(retorno,stringGeral);
				strcat(retorno,">");
			break;

			case CONSTANT_Double:
				valorSaida = decodificaDoubleInfo(cp_aux);
				stringGeral = (char*)malloc(100*sizeof(char));
				sprintf(stringGeral,"%lf",valorSaida);
				strcpy(retorno,"<");
				strcat(retorno,stringGeral);
				strcat(retorno,">");
			break;

			case CONSTANT_Long:
				saidaLong = decodificaLongInfo(cp_aux);
				stringGeral = (char*)malloc(100*sizeof(char));
				sprintf(stringGeral,"%lld",saidaLong);
				strcpy(retorno,"<");
				strcat(retorno,stringGeral);
				strcat(retorno,">");
			break;

			case CONSTANT_Float:
				saidaFloat = decodificaFloatInfo(cp_aux);
				stringGeral = (char *) malloc(100*sizeof(char));
				sprintf(stringGeral,"%f",saidaFloat);
				strcpy(retorno,"<");
				strcat(retorno,stringGeral);
				strcat(retorno,">");
			break;

			default:
				strcpy(retorno,"undefined");
			break;
		}
	} else {
		sprintf(retorno,"%d",index);
	}

	// printf("RETORNO: %s\n",retorno);

	return(retorno);
}

// LEMBRAR QUE CP INFO COMEÇA DE 1, POR ISSO QUE SUBTRAI 1 NA SOMA
// Decodifica Name Index e Name Type
char* decodificaNIeNT(Cp_info *cp, u2 index, u1 tipo){

	char *retorno = malloc(100*sizeof(char));

	Cp_info *aux;
	Cp_info *aux2;
	Cp_info *aux3;

	aux = cp+index-1;

	switch(tipo){
		case NAME_INDEX:

			aux2 = cp+(aux->UnionCP.Class.name_index-1);

			retorno = decodificaStringUTF8(aux2);
		break;

		case NAME_AND_TYPE:

			aux2 = cp+(aux->UnionCP.NameAndType.name_index-1);
			aux3 = cp+(aux->UnionCP.NameAndType.descriptor_index-1);

			strcpy(retorno,"");
			strcat(retorno,decodificaStringUTF8(aux2));
			strcat(retorno,":");
			strcat(retorno,decodificaStringUTF8(aux3));
		break;

		case STRING_INDEX:
		case CLASS_INDEX:
			retorno = decodificaStringUTF8(aux);
		break;

		case NAME_AND_TYPE_INFO_NAME_INDEX:
			retorno = decodificaStringUTF8(aux);
		break;

		case NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX:
			retorno = decodificaStringUTF8(aux);
		break;
	}

	return(retorno);
}

char* decodificaAccessFlags(u2 flag){

	char *retorno = malloc(100*sizeof(char));
	strcpy(retorno,"");

	while(flag){

		if(flag>=SYNTHETIC){
			flag-=SYNTHETIC;
			strcat(retorno,"SYNTHETIC;");
		}

		if(flag>=ABSTRACT){
			flag-=ABSTRACT;
			strcat(retorno,"ABSTRACT;");
		}

		if(flag>=INTERFACE_FLAG){
			flag-=INTERFACE_FLAG;
			strcat(retorno,"INTERFACE;");
		}

		if(flag>=TRANSIENT){
			flag-=TRANSIENT;
			strcat(retorno,"TRANSIENT;");
		}

		if(flag>=VOLATILE){
			flag-=VOLATILE;
			strcat(retorno,"VOLATILE;");
		}

		if(flag>=FINAL){
			flag-=FINAL;
			strcat(retorno,"FINAL;");
		}

		if(flag>=STATIC){
			flag-=STATIC;
			strcat(retorno,"STATIC;");
		}

		if(flag>=PROTECTED){
			flag-=PROTECTED;
			strcat(retorno,"PROTECTED;");
		}

		if(flag>=PRIVATE){
			flag-=PRIVATE;
			strcat(retorno,"PRIVATE;");
		}

		if(flag>=PUBLIC){
			flag-=PUBLIC;
			strcat(retorno,"PUBLIC;");
		}
	}

	retorno = organizandoFlags(retorno);
	return(retorno);

}

char* organizandoFlags(char* flagsOrdemInversa){

	const char s[2] = ";"; // Delimitador que vai dividir a string;

	char* flags = strtok(flagsOrdemInversa,s);
	char* velho = (char*)malloc(100*sizeof(char));
	char* novo = (char*)malloc(100*sizeof(char));

	int contador = 0;

	strcpy(novo, "");
	strcpy(velho, "");

	printf("--------------------------------------\n");
	while(flags != NULL){

		if(contador == 0)
			strcpy(velho, flags);

		if(contador > 0){

			strcpy(novo, flags);
			strcat(novo," ");
			strcat(novo, velho);
			strcpy(velho, novo);
			contador++;
		}
		else
			contador++;

		flags = strtok(NULL, s);
	}

	if(contador == 1){
		free(novo);
		return velho;
	}
	else{
		free(velho);
		return novo;
	}
}

double decodificaDoubleInfo (Cp_info * cp) {
	long long valor = ((long long)(cp->UnionCP.Double.high_bytes)<<32) | (long long)cp->UnionCP.Double.low_bytes;
	int8_t sinal = ((valor>>63) == 0) ? 1 : -1;
	int16_t expon = ((valor>>52) & 0x7ffL);
	long long mant = (expon == 0) ? ((valor & 0xfffffffffffffL) << 1) : ((valor & 0xfffffffffffffL) | 0x10000000000000L);
	double retorno = sinal*mant*(pow(2,expon-1075));
	return retorno;
}

long long decodificaLongInfo (Cp_info * cp) {
	return ((((long long)cp->UnionCP.Long.high_bytes)<<32) | ((long long)cp->UnionCP.Long.low_bytes));
}

int decodificaIntegerInfo (Cp_info * cp) {

	u4 valor = cp->UnionCP.Integer.bytes;
	return valor;
}

float decodificaFloatInfo (Cp_info * cp) {
	u4 valor = cp->UnionCP.Float.bytes;
	float retorno;
	int sinal = ((valor>>31) == 0) ? 1 : -1;
	int expon = ((valor>>23) & 0xff);
	int mant = (expon == 0) ? (valor & 0x7fffff)<<1 : (valor & 0x7fffff) | 0x800000;
	retorno = (sinal)*(mant)*(pow(2,expon-150));
	return retorno;
}

double decodificaMajorVersion (u2 major) {

	double retorno;

	switch (major) {
		case 53:
			retorno = 1.9;
			break;
		case 52:
			retorno = 1.8;
			break;
		case 51:
			retorno = 1.7;
			break;
		case 50:
			retorno = 1.6;
			break;
		case 49:
			retorno = 1.5;
			break;
		case 48:
			retorno = 1.4;
			break;
		case 47:
			retorno = 1.3;
			break;
		case 46:
			retorno = 1.2;
			break;
		case 45:
			retorno = 1.1;
			break;
		default:
			retorno = 0;
			break;
	}

	return retorno;
}

void imprimirClassFile (Classfile *arquivoClass, FILE* fp)
{
	if(fp == NULL) { return; }

	Cp_info *aux;
	double valor;
	long long longValue;
	double valorMajor;
	float valorRetFloat;
	Method_info *auxMethod;
	Field_info *auxField;
	Attribute_info **auxAttributeClasse;
	Attribute_info **fieldAttrAux;
	Exception_table *exceptionTableAux;
	u4 contador = 1;
	// u1 * auxBytesCode;
	char *ponteiroprint;

	Instrucao *instrucoes = construirInstrucoes();

	fprintf(fp, "\n-----GENERAL INFORMATION-----\n\n");
	fprintf(fp, "Magic: %08x\n",arquivoClass->magic);
	fprintf(fp, "Minor Version: %d\n",arquivoClass->minor_version);
	valorMajor = decodificaMajorVersion(arquivoClass->major_version);
	fprintf(fp, "Major Version: %d [%.1lf]\n",arquivoClass->major_version,valorMajor);
	fprintf(fp, "Constant Pool Count: %d\n",arquivoClass->constant_pool_count);
	fprintf(fp, "Access Flags: 0x%04x\n",arquivoClass->access_flags);
	ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,arquivoClass->this_class,NAME_INDEX);
	fprintf(fp, "This Class: cp_info#%d <%s>\n",arquivoClass->this_class, ponteiroprint);
	ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,arquivoClass->super_class,NAME_INDEX);
	fprintf(fp, "Super Class: cp_info#%d <%s>\n",arquivoClass->super_class, ponteiroprint);
	fprintf(fp, "Interfaces Count: %d\n",arquivoClass->interfaces_count);
	fprintf(fp, "Fields Count: %d\n",arquivoClass->fields_count);
	fprintf(fp, "Methods Count: %d\n",arquivoClass->methods_count);
	fprintf(fp, "Atributes Count: %d\n",arquivoClass->attributes_count);

	fprintf(fp, "\n\n-----CONSTANT POOL-----\n\n");

	for (aux = arquivoClass->constant_pool; aux < arquivoClass->constant_pool+arquivoClass->constant_pool_count-1; aux++) {
		fprintf(fp, "--------------Entrada [%d]--------------\n",contador);
		contador++;
		fprintf(fp, "TAG: %02d: %s\n",aux->tag,buscaNomeTag(aux->tag));
		switch(aux->tag) {
			case CONSTANT_Class:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.Class.name_index,CLASS_INDEX);
				fprintf(fp, "Class Name: cp_info#%02d <%s>\n",aux->UnionCP.Class.name_index,ponteiroprint);
				break;
			case CONSTANT_Fieldref:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.Fieldref.class_index,NAME_INDEX);
				fprintf(fp, "Class Name: cp_info#%02d <%s>\n",aux->UnionCP.Fieldref.class_index,ponteiroprint);
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.Fieldref.name_and_type_index,NAME_AND_TYPE);
				fprintf(fp, "Name and Type: cp_info#%02d <%s>\n",aux->UnionCP.Fieldref.name_and_type_index,ponteiroprint);
				break;
			case CONSTANT_Methodref:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.Methodref.class_index,NAME_INDEX);
				fprintf(fp, "Class Name: cp_info#%02d <%s>\n",aux->UnionCP.Methodref.class_index,ponteiroprint);
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);
				fprintf(fp, "Name and Type: cp_info#%02d <%s>\n",aux->UnionCP.Methodref.name_and_type_index,ponteiroprint);
				break;
			case CONSTANT_InterfaceMethodref:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.InterfaceMethodref.class_index,NAME_INDEX);
				fprintf(fp, "Class Name: cp_info#%02d <%s>\n",aux->UnionCP.InterfaceMethodref.class_index, ponteiroprint);
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.InterfaceMethodref.name_and_type_index,NAME_AND_TYPE);
				fprintf(fp, "Name and Type Index: cp_info#%02d <%s>\n",aux->UnionCP.InterfaceMethodref.name_and_type_index, ponteiroprint);
				break;
			case CONSTANT_String:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.String.string_index,STRING_INDEX);
				fprintf(fp, "String: cp_info#%02d <%s>\n",aux->UnionCP.String.string_index,ponteiroprint);
				break;
			case CONSTANT_Integer:
				fprintf(fp, "Bytes: %04x\n",aux->UnionCP.Integer.bytes);
				fprintf(fp,"Integer: %d\n",aux->UnionCP.Integer.bytes);
				break;
			case CONSTANT_Float:
				valorRetFloat = decodificaFloatInfo(aux);
				fprintf(fp, "Bytes: %04x\n",aux->UnionCP.Float.bytes);
				fprintf(fp, "Float: %f\n",valorRetFloat);
				break;
			case CONSTANT_Long:
				longValue = decodificaLongInfo(aux);
				fprintf(fp, "Long High Bytes: 0x%08x\n",aux->UnionCP.Long.high_bytes);
				fprintf(fp, "Long Low Bytes: 0x%08x\n",aux->UnionCP.Long.low_bytes);
				fprintf(fp, "Long: %lld\n",longValue);
				fprintf(fp, "--------------Entrada [%d]--------------\n",contador);
				fprintf(fp, "(large numeric continued)\n");
				aux++;
				contador++;
				break;
			case CONSTANT_Double:
				valor = decodificaDoubleInfo(aux);
				fprintf(fp, "Double High Bytes: 0x%08x\n",aux->UnionCP.Double.high_bytes);
				fprintf(fp, "Double Low Bytes: 0x%08x\n",aux->UnionCP.Double.low_bytes);
				fprintf(fp, "Double: %lf\n",valor);
				fprintf(fp, "--------------Entrada [%d]--------------\n",contador);
				fprintf(fp, "(large numeric continued)\n");
				aux++;
				contador++;
				continue;
				break;
			case CONSTANT_NameAndType:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.NameAndType.name_index,NAME_AND_TYPE_INFO_NAME_INDEX);
				fprintf(fp, "Name: cp_info#%02d <%s>\n",aux->UnionCP.NameAndType.name_index,ponteiroprint);
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.NameAndType.descriptor_index,NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
				fprintf(fp, "Descriptor: cp_info#%02d <%s>\n",aux->UnionCP.NameAndType.descriptor_index,ponteiroprint);
				break;
			case CONSTANT_Utf8:
				fprintf(fp, "Length of byte array: %d\n",(int)aux->UnionCP.UTF8.length);
				fprintf(fp, "Length of string: %d\n",(int)aux->UnionCP.UTF8.length);
				fprintf(fp, "String: ");
				for (u1 * i = aux->UnionCP.UTF8.bytes; i < aux->UnionCP.UTF8.bytes + aux->UnionCP.UTF8.length; i++) {
					fprintf(fp, "%c",(char) (*i));
				}
				fprintf(fp, "\n");
				break;
			case CONSTANT_MethodHandle:
				fprintf(fp, "MethodHandle Reference Kind: %02x\n",aux->UnionCP.MethodHandle.reference_kind);
				fprintf(fp, "MethodHandle Reference Index: %04x\n",aux->UnionCP.MethodHandle.reference_index);
				break;
			case CONSTANT_MethodType:
				fprintf(fp, "MethodType Descriptor Index: %04x\n",aux->UnionCP.MethodType.descriptor_index);
				break;
			case CONSTANT_InvokeDynamic:
				fprintf(fp, "InvokeDynamic - Bootstrap Method Attr Index: %04x\n",aux->UnionCP.InvokeDynamicInfo.bootstrap_method_attr_index);
				fprintf(fp, "InvokeDynamic - Name and Type Index: %04x\n",aux->UnionCP.InvokeDynamicInfo.name_and_type_index);
				break;
			default:
				fprintf(fp, "DefaultImpressao\n");
				break;
		}
	}

	fprintf(fp, "\n\n-----INTERFACES-----\n\n");
	contador = 0;
	for (u2 * auxInterfaces = arquivoClass->interfaces; auxInterfaces < arquivoClass->interfaces+arquivoClass->interfaces_count; auxInterfaces++) {
		ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,*auxInterfaces,NAME_INDEX);
		fprintf(fp, "Interface: cp_info#%d <%s>\n",*auxInterfaces, ponteiroprint);
	}

	fprintf(fp, "\n\n-----FIELDS-----\n\n");
	contador = 0;
	for (auxField = arquivoClass->fields; auxField < arquivoClass->fields + arquivoClass->fields_count; auxField++,contador++) {
		fprintf(fp, "--------------Field [%d]--------------\n\n",contador);
		ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+auxField->name_index);
		fprintf(fp, "Name: cp_info#%d <%s>\n",auxField->name_index,ponteiroprint);
		ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+auxField->descriptor_index);
		fprintf(fp, "Descriptor: cp_info#%d <%s>\n",auxField->descriptor_index,ponteiroprint);
		ponteiroprint = decodificaAccessFlags(auxField->access_flags);
		fprintf(fp, "Access Flags: 0x%04x [%s]\n",auxField->access_flags,ponteiroprint);
		fprintf(fp, "Attributes Count: %d\n",auxField->attributes_count);
		if (auxField->attributes_count > 0) {
			fieldAttrAux = auxField->attributes;
			for (int posicaoFields = 0; posicaoFields < auxField->attributes_count; posicaoFields++) {
				ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+(*(fieldAttrAux+posicaoFields))->attribute_name_index);
				fprintf(fp, "Attribute Name Index: cp_info#%d <%s>\n",(*(fieldAttrAux+posicaoFields))->attribute_name_index,ponteiroprint);
				fprintf(fp, "Attribute Length: %d\n",(*(fieldAttrAux+posicaoFields))->attribute_length);
				if (strcmp(ponteiroprint, "ConstantValue") == 0) {
					ConstantValue_attribute * cvAux = (ConstantValue_attribute*)(*(fieldAttrAux+posicaoFields))->info;
					Cp_info * cpInfoAux = arquivoClass->constant_pool-1+cvAux->constantvalue_index;
					//FLOAT
					if (cpInfoAux->tag == 4) {
						float valorCV = decodificaFloatInfo(arquivoClass->constant_pool-1+cvAux->constantvalue_index);
						fprintf(fp, "Constant Value Index: cp_info#%d <%f>\n",cvAux->constantvalue_index,valorCV);
					//Integer-Byte-Boolean-Short-Char
					} else if (cpInfoAux->tag == 3) {
						int valorRetorno = decodificaIntegerInfo (arquivoClass->constant_pool-1+cvAux->constantvalue_index);
						fprintf(fp, "Constant Value Index: cp_info#%d <%d>\n",cvAux->constantvalue_index,valorRetorno);
					//STRING
					} else if (cpInfoAux->tag == 8) {
						char * stringEntrada = decodificaNIeNT(arquivoClass->constant_pool,cvAux->constantvalue_index,NAME_INDEX);
						fprintf(fp, "Constant Value Index: cp_info#%d <%s>\n",cvAux->constantvalue_index,stringEntrada);
					//DOUBLE
					} else if (cpInfoAux->tag == 6) {
						double valorDB = decodificaDoubleInfo(arquivoClass->constant_pool-1+cvAux->constantvalue_index);
						fprintf(fp, "Constant Value Index: cp_info#%d <%lf>\n",cvAux->constantvalue_index,valorDB);
					} else if (cpInfoAux->tag == 5) {
						long long valorL = decodificaLongInfo(arquivoClass->constant_pool-1+cvAux->constantvalue_index);
						fprintf(fp, "Constant Value Index: cp_info#%d <%lld>\n",cvAux->constantvalue_index,valorL);
					}
				} else if (strcmp(ponteiroprint,"Signature") == 0) {
					Signature_attribute * sig = (Signature_attribute*)(*(fieldAttrAux+posicaoFields))->info;
					char * Signature_Index = decodificaStringUTF8(arquivoClass->constant_pool-1+sig->signature_index);
					fprintf(fp, "Signature index: cp_info#%d <%s>\n",sig->signature_index,Signature_Index);
				}
			}
		}
		//Incluir impressao de atributos dos fields
	}

	fprintf(fp, "\n\n-----MÉTODOS-----\n\n");

	contador = 0;

	for (auxMethod = arquivoClass->methods; auxMethod < arquivoClass->methods + arquivoClass->methods_count; auxMethod++,contador++) {
		fprintf(fp, "--------------Método [%d]--------------\n\n",contador);
		ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+auxMethod->name_index);
		fprintf(fp, "Name: cp_info#%d <%s>\n",auxMethod->name_index,ponteiroprint);
		ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+auxMethod->descriptor_index);
		fprintf(fp, "Descriptor: cp_info#%d <%s>\n",auxMethod->descriptor_index,ponteiroprint);
		ponteiroprint = decodificaAccessFlags(auxMethod->access_flags);
		fprintf(fp, "Access Flags: 0x%04x [%s]\n",auxMethod->access_flags,ponteiroprint);
		fprintf(fp, "Attributes Count: %d\n",auxMethod->attributes_count);

		fprintf(fp, "Atributos:\n");
		Attribute_info ** auxAttrCompleto = auxMethod->attributes;
		for (int posicao = 0; posicao < auxMethod->attributes_count; posicao++) {
			ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+(*(auxAttrCompleto+posicao))->attribute_name_index);
			fprintf(fp, "Attribute Name Index: cp_info#%d <%s>\n",(*(auxAttrCompleto+posicao))->attribute_name_index,ponteiroprint);
			fprintf(fp, "Attribute Length: %d\n",(*(auxAttrCompleto+posicao))->attribute_length);

			if (strcmp(ponteiroprint,"Code") == 0) {
				Code_attribute * auxCodePontual = (Code_attribute*)(*(auxAttrCompleto+posicao))->info;
				fprintf(fp, "Minor Version: %d\n",auxCodePontual->max_stack);
				fprintf(fp, "Maximum local variables: %d\n",auxCodePontual->max_locals);
				fprintf(fp, "Code length: %d\n",auxCodePontual->code_length);
				fprintf(fp, "\n\n----Code----\n\n");
				if(auxCodePontual->code_length > 0) {
					ponteiroprint = decodificarCode(arquivoClass->constant_pool,arquivoClass->constant_pool_count,auxCodePontual->code,auxCodePontual->code_length,instrucoes);
					fprintf(fp, "%s\n",ponteiroprint);
				}
				if(auxCodePontual->exception_info_length > 0) {
					fprintf(fp, "Exception Table:\n");
					fprintf(fp, "Nr.\t\tStart PC\tEnd PC\tHandler PC\tCatch Type\n");
					int contadorExceptionTable = 0;
					for(exceptionTableAux = auxCodePontual->ex_info; exceptionTableAux < auxCodePontual->ex_info + auxCodePontual->exception_info_length; exceptionTableAux++){
						fprintf(fp, "%d\t\t%02x\t\t%02x\t\t%02x\t%02x\n",contadorExceptionTable,exceptionTableAux->start_pc,exceptionTableAux->end_pc,exceptionTableAux->handler_pc,exceptionTableAux->catch_type);
						contadorExceptionTable++;
					}
					fprintf(fp, "\n\n");
				}
				fprintf(fp, "Attributes Count: %d\n",auxCodePontual->attributes_count);
				if (auxCodePontual->attributes_count > 0) {
					int lntContador = 0;
					Attribute_info ** auxAttributesFromCode = auxCodePontual->attributes;
					for (int posicaoDois = 0; posicaoDois < auxCodePontual->attributes_count; posicaoDois++) {
						ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+(*(auxAttributesFromCode+posicaoDois))->attribute_name_index);
						fprintf(fp, "Attribute Name Index: cp_info#%d <%s>\n",(*(auxAttributesFromCode+posicaoDois))->attribute_name_index,ponteiroprint);
						fprintf(fp, "Attribute Length: %d\n",(*(auxAttributesFromCode+posicaoDois))->attribute_length);
						if (strcmp(ponteiroprint,"LineNumberTable") == 0) {
							Line_number_table * lntAux = (Line_number_table*)(*(auxAttributesFromCode+posicaoDois))->info;
							fprintf(fp, "Line Number Table Length: %d\n",(int)lntAux->line_number_table_length);
							fprintf(fp, "Attribute Info: \n");
							fprintf(fp, "Nr.\t\tStartPC\t\tLineNumber\n");
							for (Line_number_info * linfo = lntAux->info; linfo < lntAux->info + lntAux->line_number_table_length; linfo++) {
								fprintf(fp, "%d\t\t%d\t\t%d\n",lntContador,linfo->start_pc,linfo->line_number);
								lntContador++;
							}
							fprintf(fp, "\n");
						} else if (strcmp(ponteiroprint,"StackMapTable") == 0) {
							int offsetImpressao = 0;
							StackMapTable_attribute * smt = (StackMapTable_attribute*)(*(auxAttributesFromCode+posicaoDois))->info;
							Stack_map_frame ** smf = smt->entries;
							fprintf(fp, "Nr.\t\tStack Map Frame\n");
							for (int posicaoSMF = 0; posicaoSMF < smt->number_of_entries; posicaoSMF++) {
								if ((*(smf+posicaoSMF))->frame_type >= 0 && (*(smf+posicaoSMF))->frame_type <= 63) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->frame_type);
									fprintf(fp, "%d\t\tSAME(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type,offsetImpressao,(*(smf+posicaoSMF))->frame_type);
								} else if ((*(smf+posicaoSMF))->frame_type >= 64 && (*(smf+posicaoSMF))->frame_type <= 127) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,((*(smf+posicaoSMF))->frame_type)-64);
									fprintf(fp, "%d\t\tSAME_LOCALS_1_STACK_ITEM(%d), Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type,offsetImpressao,((*(smf+posicaoSMF))->frame_type-64));
									fprintf(fp, "\t\tStack verifications:\n");
									Verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame.stack;
									switch ((*(VTIAux))->tag) {
										case 0:
											fprintf(fp, "\t\t\tTOP\n");
											break;
										case 1:
											fprintf(fp, "\t\t\tINTEGER\n");
											break;
										case 2:
											fprintf(fp, "\t\t\tFLOAT\n");
											break;
										case 3:
											fprintf(fp, "\t\t\tLONG\n");
											break;
										case 4:
											fprintf(fp, "\t\t\tDOUBLE\n");
											break;
										case 5:
											fprintf(fp, "\t\t\tNULL\n");
											break;
										case 6:
											fprintf(fp, "\t\t\tUNINITIALIZED THIS\n");
											break;
										case 7:
											ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,(*(VTIAux))->type_info.object_variable_info.cpool_index,NAME_INDEX);
											fprintf(fp, "\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux))->type_info.object_variable_info.cpool_index, ponteiroprint);
											break;
										case 8:
											fprintf(fp, "\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux))->type_info.uninitialized_variable_info.offset);
											break;
									}
								} else if ((*(smf+posicaoSMF))->frame_type == 247) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta);
									fprintf(fp, "%d\t\tSAME_LOCALS_1_STACK_ITEM_EXTENDED(%d), Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type,offsetImpressao,(*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta);
									fprintf(fp, "\t\tStack verifications:\n");
									Verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame_extended.stack;
									switch ((*(VTIAux))->tag) {
										case 0:
											fprintf(fp, "\t\t\tTOP\n");
											break;
										case 1:
											fprintf(fp, "\t\t\tINTEGER\n");
											break;
										case 2:
											fprintf(fp, "\t\t\tFLOAT\n");
											break;
										case 3:
											fprintf(fp, "\t\t\tLONG\n");
											break;
										case 4:
											fprintf(fp, "\t\t\tDOUBLE\n");
											break;
										case 5:
											fprintf(fp, "\t\t\tNULL\n");
											break;
										case 6:
											fprintf(fp, "\t\t\tUNINITIALIZED THIS\n");
											break;
										case 7:
											ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,(*(VTIAux))->type_info.object_variable_info.cpool_index,NAME_INDEX);
											fprintf(fp, "\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux))->type_info.object_variable_info.cpool_index, ponteiroprint);
											break;
										case 8:
											fprintf(fp, "\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux))->type_info.uninitialized_variable_info.offset);
											break;
									}
								} else if ((*(smf+posicaoSMF))->frame_type >= 248 && (*(smf+posicaoSMF))->frame_type <= 250) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.chop_frame.offset_delta);
									fprintf(fp, "%d\t\tCHOP(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao,(*(smf+posicaoSMF))->map_frame_type.chop_frame.offset_delta);
								} else if ((*(smf+posicaoSMF))->frame_type == 251) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.same_frame_extended.offset_delta);
									fprintf(fp, "%d\t\tSAME_FRAME_EXTENDED(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao,(*(smf+posicaoSMF))->map_frame_type.same_frame_extended.offset_delta);
								} else if ((*(smf+posicaoSMF))->frame_type >= 252 && (*(smf+posicaoSMF))->frame_type <= 254) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.append_frame.offset_delta);
									fprintf(fp, "%d\t\tAPPEND(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao, (*(smf+posicaoSMF))->map_frame_type.append_frame.offset_delta);
									Verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.append_frame.locals;
									fprintf(fp, "\t\t  Local verifications:\n");
									for (int posicaoVTI = 0; posicaoVTI < ((*(smf+posicaoSMF))->frame_type-251); posicaoVTI++) {
										switch ((*(VTIAux+posicaoVTI))->tag) {
											case 0:
												fprintf(fp, "\t\t\tTOP\n");
												break;
											case 1:
												fprintf(fp, "\t\t\tINTEGER\n");
												break;
											case 2:
												fprintf(fp, "\t\t\tFLOAT\n");
												break;
											case 3:
												fprintf(fp, "\t\t\tLONG\n");
												break;
											case 4:
												fprintf(fp, "\t\t\tDOUBLE\n");
												break;
											case 5:
												fprintf(fp, "\t\t\tNULL\n");
												break;
											case 6:
												fprintf(fp, "\t\t\tUNINITIALIZED THIS\n");
												break;
											case 7:
												ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index,NAME_INDEX);
												fprintf(fp, "\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index, ponteiroprint);
												break;
											case 8:
												fprintf(fp, "\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux+posicaoVTI))->type_info.uninitialized_variable_info.offset);
												break;
										}
									}
								} else if ((*(smf+posicaoSMF))->frame_type == 255) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.full_frame.offset_delta);
									fprintf(fp, "%d\t\tFULL_FRAME(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao, (*(smf+posicaoSMF))->map_frame_type.full_frame.offset_delta);
									Verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.full_frame.locals;
									fprintf(fp, "\t\t  Local verifications:\n");
									for (int posicaoVTI = 0; posicaoVTI < (*(smf+posicaoSMF))->map_frame_type.full_frame.number_of_locals; posicaoVTI++) {
										switch ((*(VTIAux+posicaoVTI))->tag) {
											case 0:
												fprintf(fp, "\t\t\tTOP\n");
												break;
											case 1:
												fprintf(fp, "\t\t\tINTEGER\n");
												break;
											case 2:
												fprintf(fp, "\t\t\tFLOAT\n");
												break;
											case 3:
												fprintf(fp, "\t\t\tLONG\n");
												break;
											case 4:
												fprintf(fp, "\t\t\tDOUBLE\n");
												break;
											case 5:
												fprintf(fp, "\t\t\tNULL\n");
												break;
											case 6:
												fprintf(fp, "\t\t\tUNINITIALIZED THIS\n");
												break;
											case 7:
												ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index,NAME_INDEX);
												fprintf(fp, "\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index, ponteiroprint);
												break;
											case 8:
												fprintf(fp, "\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux+posicaoVTI))->type_info.uninitialized_variable_info.offset);
												break;
										}
									}
									VTIAux = (*(smf+posicaoSMF))->map_frame_type.full_frame.stack;
									fprintf(fp, "\t\t  Stack verifications:\n");
									for (int posicaoVTI = 0; posicaoVTI < (*(smf+posicaoSMF))->map_frame_type.full_frame.number_of_stack_items; posicaoVTI++) {
										switch ((*(VTIAux+posicaoVTI))->tag) {
											case 0:
												fprintf(fp, "\t\t\tTOP\n");
												break;
											case 1:
												fprintf(fp, "\t\t\tINTEGER\n");
												break;
											case 2:
												fprintf(fp, "\t\t\tFLOAT\n");
												break;
											case 3:
												fprintf(fp, "\t\t\tLONG\n");
												break;
											case 4:
												fprintf(fp, "\t\t\tDOUBLE\n");
												break;
											case 5:
												fprintf(fp, "\t\t\tNULL\n");
												break;
											case 6:
												fprintf(fp, "\t\t\tUNINITIALIZED THIS\n");
												break;
											case 7:
												ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index,NAME_INDEX);
												fprintf(fp, "\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index, ponteiroprint);
												break;
											case 8:
												fprintf(fp, "\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux+posicaoVTI))->type_info.uninitialized_variable_info.offset);
												break;
										}
									}
								}
							}
						}
					}
				}
			} else if (strcmp(ponteiroprint,"Exceptions") == 0) {
				Exceptions_attribute * excpAux = (Exceptions_attribute*)(*(auxAttrCompleto+posicao))->info;
				int contadorExcp = 0;
				char * exceptionIndexString;
				fprintf(fp, "Nr.\t\tException\t\t\tVerbose\n");
				for (u2 * indexExcp = excpAux->exception_index_info; indexExcp < excpAux->exception_index_info + excpAux->number_of_exceptions; indexExcp++) {
					exceptionIndexString = decodificaNIeNT(arquivoClass->constant_pool,*indexExcp,NAME_INDEX);
					fprintf(fp, "%d\t\tcp_info#%d\t\t\t%s\n",contadorExcp,*indexExcp,exceptionIndexString);
					contadorExcp++;
				}
			} else if (strcmp(ponteiroprint,"Signature") == 0) {
				Signature_attribute *sig = (Signature_attribute*)(*(auxAttrCompleto+posicao))->info;
				char * Signature_Index = decodificaStringUTF8(arquivoClass->constant_pool-1+sig->signature_index);
				fprintf(fp, "Signature index: cp_info#%d <%s>\n",sig->signature_index,Signature_Index);
			}
		}
	}

	fprintf(fp, "\n\n-----ATRIBUTOS DA CLASSE-----\n\n");
	auxAttributeClasse = arquivoClass->attributes;
	for (int posicao = 0; posicao < arquivoClass->attributes_count; posicao++) {
		ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool+(*(auxAttributeClasse+posicao))->attribute_name_index-1);
		fprintf(fp, "Attribute Name Index: cp_info#%d <%s>\n",(*(auxAttributeClasse+posicao))->attribute_name_index,ponteiroprint);
		fprintf(fp, "Attribute Length: %d\n",(int) (*(auxAttributeClasse+posicao))->attribute_length);
		if (strcmp(ponteiroprint,"SourceFile") == 0) {
		 	Source_file_attribute * SourceFile = ((Source_file_attribute*)((*(auxAttributeClasse+posicao))->info));
			fprintf(fp, "Source File Name Index: cp_info#%d <%s>\n",SourceFile->source_file_index,decodificaStringUTF8(arquivoClass->constant_pool+SourceFile->source_file_index-1));
		} else if (strcmp(ponteiroprint, "InnerClasses") == 0) {
			InnerClasses_attribute * innerC = ((InnerClasses_attribute*)((*(auxAttributeClasse+posicao))->info));
			fprintf(fp, "Nr.\t\tInner Class\t\t\tOuter Class\t\tInner Name\t\tAccess Flags\n");
			char * innerClassString, * outerClassString, * innerNameIndex, * accessFlagsInner;
			Classes ** vetorClasses = innerC->classes_array;
			for (int posicaoInncerC = 0; posicaoInncerC < innerC->number_of_classes; posicaoInncerC++) {
				innerClassString = decodificaNIeNT(arquivoClass->constant_pool,(*(vetorClasses+posicaoInncerC))->inner_class_info_index,NAME_INDEX);
				outerClassString = decodificaNIeNT(arquivoClass->constant_pool,(*(vetorClasses+posicaoInncerC))->outer_class_info_index,NAME_INDEX);
				innerNameIndex = decodificaStringUTF8(arquivoClass->constant_pool-1+(*(vetorClasses+posicaoInncerC))->inner_name_index);
				accessFlagsInner = decodificaAccessFlags((*(vetorClasses+posicaoInncerC))->inner_class_access_flags);
				fprintf(fp, "%d\t\tcp_info#%d\t\t\tcp_info#%d\t\tcp_info#%d\t\t0x%04x\n",posicaoInncerC,(*(vetorClasses+posicaoInncerC))->inner_class_info_index,(*(vetorClasses+posicaoInncerC))->outer_class_info_index,(*(vetorClasses+posicaoInncerC))->inner_name_index,(*(vetorClasses+posicaoInncerC))->inner_class_access_flags);
				fprintf(fp, "  \t\t%s\t\t%s\t\t%s\t\t\t%s\n",innerClassString,outerClassString,innerNameIndex,accessFlagsInner);
			}
		} else if (strcmp(ponteiroprint,"Signature") == 0) {
			Signature_attribute * sig = (Signature_attribute*)((*(auxAttributeClasse+posicao))->info);
			char * Signature_Index = decodificaStringUTF8(arquivoClass->constant_pool-1+sig->signature_index);
			fprintf(fp, "Signature index: cp_info#%d <%s>\n",sig->signature_index,Signature_Index);
		}
	}
}

int setaOffsetImpressao (int posicao, u1 offset)
{
	if (posicao == 0) {
		return offset;
	} else {
		return (offset + 1);
	}
}
