/* Arquivo cabeçalho da implementação das instruções e funções auxiliares.
 */

#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include "instrucoes.h"
#include "classfile.h"
#include "jvm.h"
#include <stdbool.h>

/** Macro para normalizar indice do pool de constantes */
#define normaliza_indice(x,y) (x << 8) | y

/**
 * @brief Função para resolver (analisar e alocar caso nao alocada) uma classe
 *
 * @param nome_classe Nome da classe a ser resolvida
 * @return Ponteiro para a classe resolvida
 */
Classfile* resolverClasse(char *nome_classe);

/**
 * @brief Função para resolver (analisar parâmetros, nome e descriptor) método
 *
 * @param cp Ponteiro para constant pool
 * @param indice_cp Índice a ser acessado na constante pool
 * @param interface Flag para sinalizar método de interface
 * @return Valor booleano representando o sucesso da execucção da resolução
 */
bool resolverMetodo(Cp_info *cp, u2 indice_cp, u1 interface);

/**
 * @brief Função para obter o nome de um método na constant pool
 *
 * @param cp Ponteiro para constant pool
 * @param indice_cp Índice a ser acessado na constante pool
 * @param interface Flag para sinalizar método de interface
 * @return Ponteiro de char representando a string contendo o nome do método
 */
char* obterNomeMetodo(Cp_info *cp, u2 indice_cp, u1 interface);

/**
 * @brief Função para obter o descriptor de um método
 *
 * @param cp Ponteiro para constant pool
 * @param indice_cp Índice a ser acessado na constante pool
 * @param interface Flag para sinalizar método de interface
 * @return Ponteiro de char representando a string contendo o descriptor do método
 */
char* obterDescriptorMetodo(Cp_info *cp, u2 indice_cp, u1 interface);

/**
 * @brief Função para obter o tipo do descriptor do método
 *
 * @param descriptor Ponteiro de char representando a string contendo o descriptor do método
 * @return Valor inteiro representando o tipo do descriptor do método
 */
int descriptorFieldValidate (char * descriptor);

/**
 * @brief Função para obter a classe executante de um método
 *
 * @param cp Ponteiro para constant pool
 * @param indice_cp Índice a ser acessado na constante pool
 *
 * @return Ponteiro de char representando a string contendo o nome da classe de um método
 */
char* obterClasseDoMetodo(Cp_info *cp, u2 indice_cp);

/**
 * @brief Função para transferir os valores da pilha de operandos de um Frame para o vetor de variáveis locais do próximo Frame
 *
 * @param anterior Frame anterior (previamente executado)
 * @param novo Frame novo sendo alocado para execução de método
 * @param parametros_cont Contador de parâmetros a serem passados para o array de variáveis locais do novo Frame
 * @return Retorna um ponteiro para Frame, com o array de variáveis locais atualizado
 */
Frame* transferePilhaVetor(Frame *anterior, Frame *novo, int *parametros_cont);

/**
 * @brief Função para decodificar operandos em um valor double (64 bits)
 *
 * @param high Parte alta do valor double (32 bits)
 * @param low Parte baixa do valor double (32 bits)
 *
 * @return Valor double decodificado
 */
double decodificaDoubleValor(u4 high, u4 low);

/**
 * @brief Função para decodificar operando em um valor float (32 bits)
 *
 * @param valor 4 bytes do tipo unsigned representando o valor float
 * @return Valor float decodificado
 */
float decodificaFloatValor(u4 valor);

/**
 * @brief Função para decodificar operandos em um valor long (64 bits)
 *
 * @param high Parte alta do valor long (32 bits)
 * @param low Parte baixa do valor long (32 bits)
 *
 * @return Valor long decodificado
 */
long decodificaLongValor (u4 high, u4 low);

/**
 * @brief Função para obter a quantidade de campos da classe que não são estáticos (atributos de classe)
 *
 * @param classe Ponteiro para a classe alocada
 * @return Valor inteiro com a quantidade de campos que não são do tipo static
 */
int getParametrosNaoStatic (Classfile *classe);

/**
 * @brief Função para inserir um objeto novo na lista de objetos
 *
 * @param lis Ponteiro para o começo da lista de objetos
 * @param classe Referência para a classe (objeto) a ser alocada na lista
 * @param parametrosNaoStatic Quantidade de parâmetros que não são static
 * @return Ponteiro para a lista de objetos atualizado com a inserção do novo objeto
 */
Lista_Objetos *InsereObjeto (Lista_Objetos *lis, Classfile *classe, int parametrosNaoStatic);

/**
 * @brief Função para verificar se um método ou field é do tipo static
 *
 * @param accessFlags String contendo o accessflags do método ou field
 * @return Valor booleano indicando que o método ou field é do tipo static
 */
bool buscaStaticFlags (char *accessFlags);

/**
 * @brief Função para obter a quantidade de parâmetros a serem passados para um método
 *
 * @param descriptor String contendo o descriptor do método (parâmetros e tipo de retorno)
 * @return Valor inteiro representando a quantidade de parâmetros a serem passados para o método
 */
int getParametrosCount (char *descriptor);

/**
 * @brief Função para buscar um objeto na lista de objetos
 *
 * @param p Ponteiro de classe contendo a referência ao objeto
 * @return Retorna um ponteiro para a lista de objetos
 */
Lista_Objetos * buscaObjetoViaReferencia (Classfile *p);

/**
 * @brief Função para obter a posição do field no array de dados de instância (dados não estáticos)
 *
 * @param obj Ponteiro para o objeto alocado
 * @param nomeField Nome do field pertencente ao objeto
 *
 * @return Posição do field no array de dados não estáticos
 */
int getPositionField (Classfile *obj, char *nomeField);

/**
 * @brief Função para instanciar um objeto novo
 *
 * @param nomeClasse Ponteiro para char contendo a string do nome da classe
 * @return Ponteiro para o objeto alocado
 */
Classfile * instanciarClasse (char *nomeClasse);

/**
 * @brief Função para obter o tipo do operando a ser alocado na pilha de operandos
 *
 * @param descriptorRetorno Ponteiro de char contendo a string que representa o descriptor do método
 * @return Valor inteiro representando o tipo do operando a ser alocado na pilha de operandos
 */
int getTipoOperandoSaida(char *descriptorRetorno);

/**
 * @brief Função para obter o modo de empilhamento do valor de retorno de um método
 *
 * @param descriptor Ponteiro de char contendo a string que representa o descriptor do método
 * @return Valor inteiro representando o modo de empilhamento do valor de retorno de um método na pilha de operandos
 */
int getTipoRetorno (char *descriptor);


/**
 * @brief Não executa nada =)
 *
 * @param par0 -
 * @param par1 -
 * @param par2 -
 */
void nop_impl(Frame *par0,u1 par1, u1 par2);
/**
 * @brief Faz o push de um valor nulo para a pilha de operandos
 * @param f Ponteiro para o Frame]
 * @param par1 -
 * @param par2 -
 */
void aconst_null_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief [brief description]
 * @details [long description]
 *
 * @param f [description]
 * @param par1 [description]
 * @param par2 [description]
 */
void iconst_m1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 0 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 1 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 2 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 3 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_3_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 4 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_4_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 5 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_5_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 0 \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lconst_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 1 \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lconst_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 0 \c float
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fconst_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 1 \c float
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fconst_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 2 \c float
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fconst_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 0 \c double
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dconst_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push para a pilha de operando da constante 1 \c double
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dconst_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Faz o push de um byte na pilha de operandos
 *
 * @param f Ponteiro para o Frame
 * @param byte Byte a ser empilhado
 * @param par1 -
 */
void bipush_impl(Frame *f, u1 byte, u1 par1);
/**
 * @brief Faz o push de um short na pilha de operandos
 *
 * @param f Ponteiro para o Frame
 * @param byte Primeiro byte do short
 * @param par1 Segundo byte do short
 */
void sipush_impl(Frame *f,u1 byte1, u1 byte2);
/**
 * @brief Faz o push de um valor presente na constant pool
 * @param f Ponteiro para o Frame
 * @param indexbyte1 Índice da constant pool
 * @param par2 -
 */
void ldc_impl(Frame *f,u1 indexbyte1,u1 par2);
/**
 * @brief Faz o push de um valor presente na constant pool
 * @param f Ponteiro para o Frame
 * @param indexbyte1 Índice da constant pool (primeira parte)
 * @param indexbyte2 Índice da constant pool (segunda parte)
 */
void ldc_w_impl(Frame *f, u1 indexbyte1, u1 indexbyte2);
/**
 * @brief Faz o push de um valor long ou double presente na constant pool
 * @param f Ponteiro para o Frame
 * @param branchbyte1 Índice da constant pool (primeira parte)
 * @param branchbyte2 Índice da constant pool (segunda parte)
 */
void ldc2_w_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);
/**
 * @brief Carrega um valor \c int do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 -
 */
void iload_impl(Frame *f, u1 index, u1 par1);
/**
 * @brief Carrega um valor \c long do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 -
 */
void lload_impl(Frame *f, u1 index, u1 par1);
/**
 * @brief Carrega um valor \c float do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 -
 */
void fload_impl(Frame *f, u1 index, u1 par1);
/**
 * @brief Carrega um valor \c double do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 -
 */
void dload_impl(Frame *par0, u1 par1,u1 par2);
/**
 * @brief Carrega uma referência do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 -
 */
void aload_impl(Frame *f, u1 index, u1 par1);
/**
 * @brief Carrega um valor \c int da posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void iload_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c int da posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void iload_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c int da posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void iload_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c int da posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void iload_3_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c long da posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void lload_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c long da posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void lload_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c long da posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void lload_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c long da posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void lload_3_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c float da posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void fload_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c float da posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void fload_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c float da posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void fload_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c float da posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void fload_3_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c double da posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void dload_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c double da posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void dload_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c double da posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void dload_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c double da posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void dload_3_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega uma referência da posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void aload_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega uma referência da posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void aload_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega uma referência da posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void aload_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega uma referência da posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par2 -
 * @param par1 -
 */
void aload_3_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c int de um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iaload_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c long de um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void laload_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c float de um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void faload_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c double de um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void daload_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega uma referência de um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void aaload_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c byte de um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void baload_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c char de um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void caload_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c short de um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void saload_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c int no vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param index Índice do vetor de variáveis locais
 */
void istore_impl(Frame *f, u1 index, u1 par1);
/**
 * @brief Armazena um \c long no vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param index Índice do vetor de variáveis locais
 */
void lstore_impl(Frame *f, u1 index, u1 par1);
/**
 * @brief Armazena um \c float no vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param index Índice do vetor de variáveis locais
 */
void fstore_impl(Frame *f, u1 index, u1 par1);
/**
 * @brief Armazena um \c double no vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param index Índice do vetor de variáveis locais
 */
void dstore_impl(Frame *f, u1 index, u1 par1);
/**
 * @brief Armazena uma referência no vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param index Índice do vetor de variáveis locais
 */
void astore_impl(Frame *f,u1 index, u1 par1);
/**
 * @brief Armazena um \c int na posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void istore_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c int na posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void istore_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c int na posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void istore_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c int na posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void istore_3_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c long na posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lstore_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c long na posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lstore_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c long na posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lstore_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c long na posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lstore_3_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c float na posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fstore_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c float na posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fstore_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c float na posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fstore_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c float na posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fstore_3_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c double na posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dstore_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c double na posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dstore_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c double na posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dstore_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c double na posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dstore_3_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena uma referência na posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void astore_0_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena uma referência na posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void astore_1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena uma referência na posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void astore_2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena uma referência na posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void astore_3_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c int em um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iastore_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c long em um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lastore_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c float em um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fastore_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c double em um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dastore_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena uma referência em um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void aastore_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c byte em um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void bastore_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c char em um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void castore_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c short em um array
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void sastore_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Remove o valor de topo da pilha de operandos
 *
 * @param f Ponteiro para o Frame
 */
Pilha_operandos* pop_impl(Frame *f);
/**
 * @brief Remove os dois valores de topo da pilha de operandos
 *
 * @param f Ponteiro para o Frame
 */
Pilha_operandos** pop2_impl(Frame *f);
/**
 * @brief PlaceHolder para ocupar posicao no vetor de posicoes da instrucao pop
 * @details [long description]
 *
 * @param par0 Frame.
 * @param par1 -
 * @param par2 -
 */
void pop_fantasma(Frame *par0, u1 par1, u1 par2);
/**
 * @brief PlaceHolder para ocupar posicao no vetor de posicoes da instrucao pop2
 * @details [long description]
 *
 * @param par0 Frame.
 * @param par1 -
 * @param par2 -
 */
void pop2_fantasma(Frame *par0, u1 par1, u1 par2);
/**
 * @brief Duplica o valor no topo da pilha de operandos.
 * @details [long description]
 *
 * @param f Frame.
 * @param par1 -
 * @param par2 -
 */
void dup_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Duplica o valor no topo da pilha de operandos e insere dois valores a baixo.
 * @details [long description]
 *
 * @param f Frame.
 * @param par1 -
 * @param par2 -
 */
void dup_x1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Duplica o valor no topo da pilha de operandos e insere dois ou três valores a baixo.
 * @details [long description]
 *
 * @param f Frame.
 * @param par1 -
 * @param par2 -
 */
void dup_x2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere logo abaixo.
 * @details [long description]
 *
 * @param f Frame.
 * @param par1 -
 * @param par2 -
 */
void dup2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere dois valores a baixo.
 * @details [long description]
 *
 * @param f Frame.
 * @param par1 -
 * @param par2 -
 */
void dup2_x1_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere dois ou três valores a baixo.
 * @details [long description]
 *
 * @param f Frame.
 * @param par1 -
 * @param par2 -
 */
void dup2_x2_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Inverte os dois operandos do topo da pilha
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void swap_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma soma de \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iadd_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma soma de \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void ladd_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma soma de \c float
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fadd_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma soma de \c double
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dadd_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma subtração de \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void isub_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma subtração de \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lsub_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma subtração de \c float
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fsub_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma subtração de \c double
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dsub_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma multiplicação de \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void imul_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma multiplicação de \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lmul_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma multiplicação de \c float
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fmul_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma multiplicação de \c double
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dmul_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma divisão de \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void idiv_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma divisão de \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void ldiv_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma divisão de \c float
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fdiv_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma divisão de \c double
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void ddiv_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Calcula o resto \c int da divisão entre dois operandos
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void irem_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Calcula o resto \c long da divisão entre dois operandos
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lrem_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Calcula o resto \c float da divisão entre dois operandos
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void frem_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Calcula o resto \c double da divisão entre dois operandos
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void drem_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Inverte o sinal de um \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void ineg_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Inverte o sinal de um \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lneg_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Inverte o sinal de um \c float
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fneg_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Inverte o sinal de um \c double
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dneg_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift left em um \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void ishl_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift left em um \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lshl_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift right em um \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void ishr_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift right em um \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lshr_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift right lógico em um \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iushr_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift right lógico em um \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lushr_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "AND" de dois \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iand_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "AND" de dois \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void land_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "OR" de dois \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void ior_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "OR" de dois \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lor_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "XOR" de dois \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void ixor_impl(Frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "XOR" de dois \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lxor_impl(Frame *f, u1 par1, u1 par2);
void iinc_fantasma(Frame *par0, u1 par1, u1 par2);
void iinc_impl(Frame *f, u1 index, i1 constante);
void iinc_wide_fantasma(Frame *f, u1 indexbyte1, u1 indexbyte2, u1 constbyte1, u1 constbyte2);
void iinc_wide(Frame *f, u2 indexbyte, i2 constbyte);
void i2l_impl(Frame *f, u1 par1, u1 par2);
void i2f_impl(Frame *f, u1 par1, u1 par2);
void i2d_impl(Frame *f, u1 par1, u1 par2);
void l2i_impl(Frame *f, u1 par1, u1 par2);
void l2f_impl(Frame *f, u1 par1, u1 par2);
void l2d_impl(Frame *f, u1 par1, u1 par2);
void f2i_impl(Frame *f, u1 par1, u1 par2);
void f2l_impl(Frame *f, u1 par1, u1 par2);
void f2d_impl(Frame *f, u1 par1, u1 par2);
void d2i_impl(Frame *f, u1 par1, u1 par2);
void d2l_impl(Frame *f, u1 par1, u1 par2);
void d2f_impl(Frame *f, u1 par1, u1 par2);
void i2b_impl(Frame *f, u1 par1, u1 par2);
void i2c_impl(Frame *f, u1 par1, u1 par2);
void i2s_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores long oriundos da pilha de operandos
 *
 * @param f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lcmp_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores float oriundos da pilha de operandos
 *
 * @param f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fcmpl_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores float oriundos da pilha de operandos
 *
 * @param f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fcmpg_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores double oriundos da pilha de operandos
 *
 * @param f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dcmpl_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores double oriundos da pilha de operandos
 *
 * @param f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dcmpg_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valor da pilha de operandos com zero, verificando se são iguais
 *
 * @param f Frame do método corrente que está executando a instrução atual
 * @param branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 * @param branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void ifeq_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução que compara valor da pilha de operandos com zero, verificando se são diferentes
 *
 * @param f Frame do método corrente que está executando a instrução atual
 * @param branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 * @param branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void ifne_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução que compara valor da pilha de operandos com zero, verificando se o valor da pilha de operandos é menor que zero
 *
 * @param f Frame do método corrente que está executando a instrução atual
 * @param branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 * @param branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void iflt_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior ou igual ao valor zero
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifge_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior do que o valor zero
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifgt_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for menor do que o valor zero
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifle_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for igual ao próximo inteiro na pilha
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpeq_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for diferente do próximo inteiro na pilha
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpne_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior do que o próximo inteiro na pilha
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmplt_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for menor do que o próximo inteiro na pilha
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpge_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for menor do que o próximo inteiro na pilha
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpgt_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior do que o próximo inteiro na pilha
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmple_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se as referências na pilha forem iguais
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void acmpeq_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se as referências na pilha não forem iguais
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void acmpne_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief A instrução sempre efetuará um branch
 *
 * @param f Frame corrente
 * @param branchbyte1 Parâmetro que será concatenado para montar o índice do branch
 * @param branchbyte2 Parâmetro que será concatenado para montar o índice do branch
 */
void inst_goto_impl(Frame *f,u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução para efetuar um branch para um offset, dentro do código do mesmo método sendo executado
 *
 * @param f Frame corrente
 * @param branchbyte1 Parâmetro que será concatenado para montar o índice do branch
 * @param branchbyte2 Parâmetro que será concatenado para montar o índice do branch
 */
void jsr_impl(Frame *f,u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução para retornar de uma subrotina
 *
 * @param f Frame corrente
 * @param index Índice no vetor de variáveis locais do Frame corrente
 * @param par1 Esse parâmetro não será utilizado na prática
 */
void ret_impl(Frame *f, u1 index, u1 par1);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *
 * @param par0 Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void tableswitch_fantasma(Frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *
 * @param par0 Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void lookupswitch_fantasma(Frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução para acessar uma jump table por index e efetuar um jump
 *
 * @param f Frame corrente
 */
void tableswitch_impl(Frame *f);

/**
 * @brief Instrução para acessar uma chave em um jump table e efetuar um jump
 *
 * @param f Frame corrente
 */
void lookupswitch_impl(Frame *f);

/**
 * @brief Instrução para retornar um inteiro de um método
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void ireturn_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar um long de um método
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void lreturn_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar um float de um método
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void freturn_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar um double de um método
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void dreturn_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar uma referência de um método
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void areturn_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar void de um método
 *
 * @param f Frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void inst_return_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para obter um field estático de uma classe
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void getstatic_impl(Frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para setar um field estático em uma classe
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void putstatic_impl(Frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para obter o field de um objeto
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void getfield_impl(Frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para setar um field em um objeto
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void putfield_impl(Frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Função para invocar um método de instância, geralmente utilizada para impressão em tela
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokevirtual_impl(Frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para invocar um método de instância
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokespecial_impl(Frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para invocar um método estático (método de classe)
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokestatic_impl(Frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *
 * @param f Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void invokeinterface_fantasma(Frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução para invocar um método de interface
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 * @param count Um byte que não pode ser zero
 */
void invokeinterface_impl(Frame *f, u1 indexbyte1, u1 indexbyte2, u1 count);

/**
 * @brief Instrução para invocar um método dinâmico. Essa instrução só existe para criar uma posição "falsa" no vetor de ponteiros de função
 * @details [long description]
 *
 * @param par0 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void invokedynamic_fantasma(Frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução para criar um novo objeto
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void inst_new_impl(Frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Função para criar um novo array de um dado tipo
 *
 * @param f Frame corrente
 * @param atype Tipo do array que deve ser criado
 * @param par1 Parâmetro não utilizado
 */
void newarray_impl(Frame *f ,u1 atype, u1 par1);

/**
 * @brief Criar um novo array do tipo referência
 * @details [long description]
 *
 * @param f Frame corrente
 * @param par1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param par2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void anewarray_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para obter o tamanho de um array
 *
 * @param f Frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void arraylength_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para lançar uma exceção ou erro
 *
 * @param f Frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void athrow_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para verificar se um objeto é de um determinado tipo
 *
 * @param f Frame corrente
 * @param par1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param par2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void checkcast_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para determinar se um objeto é de um eterminado tipo
 *
 * @param f Frame corrente
 * @param par1 Byte que será concatenado para acessar uma entrada na constant pool
 *
 * @param par2 Byte que será concatenado para acessar uma entrada na constant pool
 *
 */
void instanceof_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para entrar no monitor de um objeto
 *
 * @param f Frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void monitorenter_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para sair de um monitor de um objeto
 *
 * @param f Frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void monitorexit_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para estender a próxima instrução para ter o dobro de bytes de índice.
 *
 * @param f Parâmetro não utilizado
 * @param indexbyte1 Parâmetro não utilizado
 * @param indexbyte2 Parâmetro não utilizado
 */
void wide_impl(Frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução que irá criar um array multidimensional
 *
 * @param f Frame corrente
 * @param indexbyte1 Byte que será concatenado para montar um índice para a constant pool da classe corrente
 * @param indexbyte2 Byte que será concatenado para montar um índice para a constant pool da classe corrente
 * @param dimensions Número de dimensões do array
 */
void multianewarray_impl(Frame *f, u1 indexbyte1, u1 indexbyte2, u1 dimensions);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *
 * @param f Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void multianewarray_fantasma(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que irá efetuar um branch caso a referência no topo da pilha seja nula
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o offset do branch
 * @param branchbyte2 Byte que será concatenado para montar o offset do branch
 */
void ifnull_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução que irá efetuar um branch caso a referência no topo da pilha não seja nula
 *
 * @param f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o offset do branch
 * @param branchbyte2 Byte que será concatenado para montar o offset do branch
 */
void ifnonnull_impl(Frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief A instrução sempre efetuará um branch, para um offset wide
 *
 * @param f Frame corrente
 * @param par1 Parâmetro que será concatenado para montar o índice do branch
 * @param par2 Parâmetro que será concatenado para montar o índice do branch
 */
void goto_w_impl(Frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para efetuar um branch para um offset wide, de 32 bits, dentro do código do mesmo método sendo executado
 *
 * @param f Frame corrente
 * @param par1 Parâmetro que será concatenado para montar o índice do branch
 * @param par2 Parâmetro que será concatenado para montar o índice do branch
 */
void jsr_w_impl(Frame *f, u1 par1, u1 par2);

extern void (*func_ptr[202])(Frame *,u1,u1);

#endif
