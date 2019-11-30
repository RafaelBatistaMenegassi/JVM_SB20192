/**
 * @file pilha_operandos.h
 * @brief Estrutura e cabeçalho da implementação da pilha de operandos
 */

#ifndef PILHA_OPERANDOS_H
#define PILHA_OPERANDOS_H

#include <stdint.h>
#include <stdbool.h>


/** Definição dos tipos de inteiro sem sinal utilizados */
#ifndef TIPOS_U
#define TIPOS_U
typedef uint8_t u1; 	/**< Tipo \c unsigned \c int de 8 bits */
typedef uint16_t u2; 	/**< Tipo \c unsigned \c int de 16 bits */
typedef uint32_t u4; 	/**< Tipo \c unsigned \c int de 32 bits */
typedef uint64_t u8; 	/**< Tipo \c unsigned \c int de 64 bits */
#endif

#include "lista_operandos.h"

/**
 * @struct pilha_operandos
 * @brief Estrutura de dados da pilha de operandos.
 * @see https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-2.html#jvms-2.6.2
 * @see lista_operandos
 */
typedef struct pilha_operandos{
	struct lista_operandos *topo; /**< Ponteiro para o topo da pilha de operandos */
} Pilha_operandos;

/**
 * @brief Inicializa uma pilha de frames
 * @details Esta função inicializa uma pilha
 * de operandos vazia para utilização posterior
 * @return Pilha de operandos com valor nulo
 */
Pilha_operandos* CriarPilha_operandos();
/**
 * @brief Insere um operando no topo da pilha de operandos
 * @details
 *
 * @param p Ponteiro para a pilha de operandos
 * @param operando Operando de valor direto a ser inserido na pilha
 * @param referencia Operando de referência a ser inserido na lista
 * @param tipo_operando Definição do tipo de operando a ser inserido
 * @return Ponteiro para a pilha de operandos após a inserção do novo operando
 */
Pilha_operandos* Push_operandos(Pilha_operandos *p, u4 operando, void *referencia, u1 tipo_operando);
/**
 * @brief Remove o operando localizado no topo da pilha de operandos
 *
 * @param p Ponteiro para a pilha de operandos
 * @return Pilha de operandos com o elemento do topo removido
 */
Pilha_operandos* Pop_operandos(Pilha_operandos *p);
/**
 * @brief Acessa o topo da pilha de operandos
 * @details
 *
 * @param p Ponteiro para a pilha de operandos
 * @return Ponteiro referente ao topo da pilha
 */
Pilha_operandos* Topo_operandos(Pilha_operandos *p);
/**
 * @brief Verifica se a pilha de operandos está vazia
 * @details A função checa se o topo da pilha contém o valor NULO.
 *
 * @param p Ponteiro para a pilha de operandos
 * @return booleano TRUE (pilha está vazia) ou FALSE (pilha não está vazia)
 */
bool pilhaVazia (Pilha_operandos *p);
/**
 * @brief Imprime na tela uma representação textual da pilha de operandos
 * @details Esta função apenas invoca a função ImprimirLista_operandos do lista_operandos.h
 *
 * @param p Ponteiro para a pilha de operandos a ser impressa
 */
void ImprimirPilha_operandos(Pilha_operandos *p);

/**
 * @brief Verifica se o campo no topo da pilha de operandos é um salto de linha (na impressão)
 * @details Algumas exceções podem imprimir um salto de linha, ou chamadas de \c System.out.println
 * sem argumentos.
 *
 * @param p Ponteiro para a pilha de operandos a ser verificada
 */
bool printVazio (Pilha_operandos *p);

#endif
