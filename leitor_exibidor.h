/* Arquivo cabecalho contendo funções do leitor/exibidor de .class */

/** Macro para verificação de inclusão de arquivos de estrutura,
bibliotecas de leitura/escrita de dados e manipulação de arquivos */

#ifndef LEITOR_EXIBIDOR_H
#define LEITOR_EXIBIDOR_H

/** Inclusão de bibliotecas para leitura/escrita de dados,
manipualão de arquivos e definição de estruturas */
#include <stdio.h>
#include <stdlib.h>
#include "classfile.h"
#include "instrucoes.h"

/* Função para ler um arquivo .class .
 * Recebe o nome do arquivo .class como parâmetro.
 * Retorna um ponteiro para uma estrutura do tipo ClassFile
 */
Classfile* lerArquivo(char *nomeArquivo);

/* A função lê uma constant pool de um arquivo .class.
 * Recebe um ponteiro para o arquivo .class sendo lido
 * e o tamanho de constant pool.
 * Retorna um ponteiro para uma estrutura do tipo Cp_info.
 */
Cp_info* lerConstantPool(FILE *fp, u2 constant_pool_count);

/* Função que lê os métodos da classe no .class.
 * Recebe um ponteiro de arquivo .class, o número de métodos
 * que devem ser lidos e um ponteiro para uma estrutura do tipo Cp_info.
 *
 * Retorna um ponteiro para uma estrutura do tipo Method_info.
 */
Method_info* lerMethod (FILE *fp, u2 methods_count, Cp_info *cp);

/* Função para decodificar os argumentos de uma instrução
 *
 * p1 - cp: Constant pool da classe
 * p2 - index: Índice da constant pool que deve ser decodificado
 * p3 - sizeCP: Tamanho da constant pool
 * A função retorna um ponteiro de char, representando os argumentos decodificados
 */
char* decodificarOperandoInstrucao(Cp_info *cp, u2 index, u2 sizeCP);

/* Função para decodificar o atributo Code de um método
 *
 * p1 - cp: Constant pool da classe
 * p2 - sizeCP: Tamanho da constant pool da classe
 * p3 - code: Ponteiro apontando para o início do atributo code
 * p4 - length: Tamanho do atributo code
 * p5 - instrucoes: Vetor contendo todas as instruções
 * A função retorna um ponteiro de char, contendo todo o Code de um método
 */
char* decodificarCode(Cp_info *cp, u2 sizeCP, u1 *code, u4 length, Instrucao *instrucoes);

/* Função para ler os fields do arquivo .class
 *
 * p1 - fp: Ponteiro do arquivo que deve ser lido
 * p2 - fields_count: Contagem de fields da classe.
 * p3 - cp: Constant Pool da classe
 * A função retorna a estrutura contendo os fields lidos
 */
Field_info* lerField (FILE *fp, u2 fields_count, Cp_info *cp);

/* Função para ler as interfaces de um arquivo .class
 *
 * p1 - fp: Ponteiro do arquivo .class que deve ser lido
 * p2 - size: Número de bytes que devem ser lidos
 * A função retorna um ponteiro para o array de interfaces indexadas pela entrada na Constant Pool que referencia a classe da interface
 */
u2* lerInterfaces (FILE *fp, u2 size);

/* Função para ler as inner classes de uma classe
 *
 * p1 - fp: Ponteiro do arquivo que deve ser lido
 * p2 - cp: Ponteiro para a constant pool da classe sendo lida
 * Retorna a estrutura que representa as inner classes de uma classe
 */
InnerClasses_attribute* lerInnerClasses(FILE *fp, Cp_info *cp);

/* Função para ler o exception attribute
 *
 * p1 - fp: Ponteiro do arquivo que deve ser lido
 * A função retorna a estrutura que representa as exceções
 */
Exceptions_attribute* lerExceptionsAttribute (FILE *fp);

/*Função para ler atributos das inner classes da classe sendo lida.
 *
 * p1 - fp: Arquivo .class a ser lido.
 * A função retorna a estrutura representando os atributos lidos.
 */
Classes* lerClasses (FILE *fp);

/* Função para decodificar a major version de um arquivo .class
 *
 * p1- major: A major version lida do .class
 * Retorna um número representando a versão
 */
double decodificaMajorVersion (u2 major);

/* Função para ler o atributo constant value de um field que apresenta modificador constante
 *
 * p1 - fp: O arquivo .class a ser lido
 * Retorna a estrutura que representa o atributo lido.
 */
ConstantValue_attribute* lerConstantValue (FILE *fp);

/* Função para decodificar uma entrada double na constant pool
 *
 * p1 - cp: Entrada da constant pool apontando para o double
 * Retorna número double decodificado.
 */
double decodificaDoubleInfo (Cp_info *cp);

/* Função para decodificar uma entrada long na constant pool
 *
 * p1 - cp: Entrada da constant pool apontando para o long
 * Retonra número long decodificado
 */
long long decodificaLongInfo (Cp_info *cp);

/* Função para decodificar uma entrada float na constant pool
 *
 * p1 - cp: Entrada da constant pool apontando para o float
 * Retorna número float decodificado.
 */
float decodificaFloatInfo (Cp_info *cp);

/* Função para decodificar uma entrada Integer na constant pool
 *
 * p1 - cp: Entrada na constant pool apontando para uma entrada do tipo Integer
 * A função retorna o inteiro decodificado
 */
int decodificaIntegerInfo (Cp_info *cp);

/* Função para ler o atributo signature do arquivo .class sendo lido
 *
 * p1 - fp: Ponteiro de arquivo que deve ser lido
 * Retorna a estrutura representando o atributo signature lido
 */
Signature_attribute* lerSignature (FILE *fp);

/* Função para colocar as flags de acesso na ordem correta
 *
 * p1 - flagsOrdemInversa: Ponteiro de char com as flags na ordem inversa
 * Retorna um ponteiro para string na ordem correta
 */
char* organizandoFlags(char *flagsOrdemInversa);

/* Função para ler o atributo stack map table dentro de Code
 *
 * p1 - fp: Arquivo que deve ser lido
 * Retorna a estrutura que representa o atributo stack map table
 */
StackMapTable_attribute* lerStackMapTable (FILE *fp);

/* Função para ler o atributo stack map frame
 *
 * p1 - fp: Ponteiro de arquivo que deve ser lido
 * Retorna a estrutura representando o atributo stack map frame
 */
Stack_map_frame* lerStackMapFrame (FILE *fp);

/* Função para ajustar um offset do atributo interno da stack map frame
 *
 * p1 - posicao: Posição do atributo interno ao stack frame
 * p2 - offset: Valor de ajuste presente no atributo do stack frame a ser avaliado para correta impressao
 * A função retorna a posição correta de impressão do atributo
 */
int setaOffsetImpressao (int posicao, u1 offset);

/* Função para ler o atributo verification_type_info
 *
 * p1 - fp: Arquivo que deve ser lido
 * Retorna a estrutura representando o atributo verification_type_info lido
 */
Verification_type_info* lerVerificationTypeInfo (FILE *fp);

/* Função para ler o atributo Code de um método
 *
 * p1 - fp: Ponteiro do arquivo que deve ser lido
 * p2 - cp: Constant Pool do arquivo lido
 * A função retorna a estrutura representando o atributo code lido
 */
Code_attribute* lerCode (FILE *fp, Cp_info *cp);

/* Função para ler o atributo Line Number Table de um atributo Code de um método
 *
 * p1 - fp: Ponteiro do arquivo que deve ser lido
 * p2 - cp: Constant Pool do arquivo lido
 *
 * A função retorna a estrutura representando o atributo Line Number Table lido
 */
Line_number_table* lerLineNumberTable(FILE *fp, Cp_info *cp);

/* Função para ler a exception table de Code
 *
 * p1 - fp: Ponteiro do arquivo que deve ser lido
 * p2 - size: Tamanho da exception table que deve ser lida
 *  A função retorna a estrutura representando a tabela de exceções do Code
 */
Exception_table* lerExceptionTable (FILE *fp, u2 size);

/* Função para ler a estrutura attribute info de um arquivo
 *
 * p1 - fp: Ponteiro de arquivo que deve ser lido
 * p2 - cp: Ponteiro para a constant pool do arquivo lido
 * A função retorna a estrutura representando o attribute info lido
 */
Attribute_info *lerAttributes (FILE *fp, Cp_info *cp);

/* Função para ler o atributo source file
 *
 * p1 - fp: Ponteiro do arquivo que deve ser lido
 * A função retorna a estrutura que representa o atributo source file
 */
Source_file_attribute* lerSourceFile (FILE *fp);

/* Função para retornar a string representando uma tag
 *
 * p1 - tag: A tag que deve ser convertida para string
 * A função retorna a string equivalente à tag enviada como parâmetro
 */
char* buscaNomeTag(u1 tag);

/* Função que retorna a string UTF8 da entrada da constant pool recebida como parâmetro
 *
 * p1 - cp: Entrada da constant pool que aponta para uma estrutura do tipo UTF8
 * A função retorna a string UTF8 decodificada
 */
char* decodificaStringUTF8(Cp_info *cp);

/* Função para decodificar uma entrada da Constant Pool
 * p1 - cp: Ponteiro para a constant pool de uma classe
 * p2 - index: Índice da constant pool que deve ser decodificado
 * p3 - tipo: Tipo da entrada que deve ser decodificada
 * A função retorna a string, que é a entrada decodificada
 */
char* decodificaNIeNT(Cp_info *cp, u2 index, u1 tipo);

/* Função para decodificar o access flag enviado como parâmetro
 *
 * p1 - flag: Inteiro representando as flags que devem ser decodificadas
 * A função retorna a string que representa os access flags decodificados
 */
char* decodificaAccessFlags(u2 flag);

/* Função para imprimir um arquivo .class lido
 *
 * p1 - arquivoClass: Estrutura de um arquivo .class que já foi lido
 * p2 - fp: Ponteiro de arquivo em que o arquivo .class deve ser impresso
 */
void imprimirClassFile (Classfile *arquivoClass, FILE *fp);

#endif
