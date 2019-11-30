/* Arquivo cabeçalho contendo funções relacionadas à execução do arquivo */

/** Macro para verificar se a biblioteca já foi definida */
#ifndef JVM_H
#define JVM_H

/** Inclusão de bibliotecas necessárias */
#include "lista_classes.h"
#include "pilha_frames.h"
#include "instrucoes.h"

/* Definição de algumas macros relativas aos nomes e descritores de métodos main, init e clinit */
#define MAIN_NOME "main"
#define DESCRIPTOR_MAIN "([Ljava/lang/String;)V"
#define PUBLIC_STATIC 0x0009
#define CLINIT_NOME "<clinit>"
#define DESCRIPTOR_CLINIT "()V"
#define STATICCLINIT 0x0008
#define INIT_NOME "<init>"
#define DESCRIPTOR_INIT "()V"

/* Estrutura de dados para armazenar os objetos instanciados */
struct lista_objetos {
	Classfile * obj;
	u1 sizeData;
	u4 * data;
	struct lista_objetos * prox;
	struct lista_objetos * ant;
};
typedef struct lista_objetos Lista_Objetos;

/* Estrutura da jvm, que será utilizada para controlar a execução do programa */
struct jvm{
	classesCarregadas * classes; /* Ponteiro para a lista de classes carregadas */
	pilha_frames *frames; 			 /* Ponteiro para a pilha de frames */
	u4 pc; 											 /* Contador de programa */
	u2 excecao; 								 /* Flag para indicar se houve alguma exceção */
	Lista_Objetos * objetos; 		 /* Lista de objetos instanciados */
	char *excecao_nome; 				 /* Nome da exceção se for lançada */
};
typedef struct jvm JVM;

/* Declarar as variáveis globais que serão utilizadas no processo de execução */
extern JVM *jvm;
extern Instrucao* instrucoes;

/*
  JVM* CriarJVM(); // inicializa a estrutura da JVM com nulo -> não necessária;
*/

/* Função para inicializar a estrutura da JVM, alocando os campos da estrutura, e setando os valores iniciais
 * A função retorna a estrutura da JVM alocada.
 */
JVM* inicializar();

/* Função para apresentar o menu para o usuário
 * Retorna um inteiro, representando a opção escolhida pelo usuário
 */
int opcaoMenu();

/* Função para criar um frame e inicializar com os valores default
 *
 * p1 - classeCorrente: Nome da classe corrente do frame
 * p2 - max_locals: Tamanho do vetor de variáveis locais do frame
 *
 * A função retorna um ponteiro para uma estrutura frame alocada
 */
Frame* criarFrame(char *classeCorrente, u2 max_locals);

/* A função inicia a interpretação do arquivo .class, chamando outras funções da interpretação */
void executarJVM();

/* Função para atualizar o PC, de acordo com a instrução branch previamente executada, verificando se o branch foi executado ou não
 *
 * p1 - code: Ponteiro para o code do método atual
 * p2 - length: Tamanho do código que deve ser analisado
 *
 * Retorna o valor atualizado de PC
 */
u1* atualizarPCMetodoAtual (u1 * code, u4 length);

/* Função para imprimir um Atributo Code de um método
 *
 * p1 - code: Ponteiro para o código que deve ser impresso
 * p2 - length: Tamanho do código que deve ser percorrido
 */
void ImprimeCode (u1 *code, u4 length);

/* Avalia se a instrução passada como parâmetro é uma instrução do tipo branch
 *
 * p1 - nomeInstrucao: Nome da instrução sendo analisada
 * A função retorna um booleano, indicando se a instrução é uma instrução do tipo branch ou não.
 */
bool instrucaoBranch (char *nomeInstrucao);

/* Função que executa o código de um método
 *
 * p1 - m: Estrutura method_info representando o método que deve ser executado
 * p2 - classeCorrente: Ponteiro para a classe corrente do método
 * p3 - chamador Inteiro que é 1 se o chamador foi o arquivo da JVM, 2 se foi uma instrução do tipo invoke que vai executar um método
 */
void executarMetodo(Method_info *m, char *classeCorrente, int chamador);

/* Função para interpretar as instruções do Code de um método
 *
 * p1 - code: Ponteiro para o código que deve ser interpretado
 * p2 - length: Tamanho do código que deve ser interpretado
 * p3 - m: Estrutura representando o método que deve ter seu code interpretado
 */
void interpretarCode(u1 *code, u4 length, Method_info *m);

/* Função para liberar o vetor de variáveis locais da memória
 *
 * p1 - v: Vetor do tipo \c vetor_locais que deve ser desalocado
 * p2 - vetor_length: Tamanho do vetor que deve ser desalocado
 */
void freeVetorLocais(Vetor_locais *v, u2 vetor_length);

/* Função para encontrar em um método o PC do handler da exceção lançada na JVM
 *
 * p1 - m: A estrutura do método no qual o handler da exceção será buscado
 * A função retorna o PC do handler do método, ou o número máximo short, caso o handler não seja encontrado
 */
u2 verificaHandlerMetodo(Method_info *m);

#endif
