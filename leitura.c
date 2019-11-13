/***********************************
 * Software Basico
 * Turma A
 * Professor Marcelo Ladeira
 * 2019/2
 * 
 * Trabalho Final - JVM
 * 
 ************************************/

/***********************************
 * Leitura
 * -> de arquivos .class
 * 
 ************************************/

/* Macro para leitura de um tipo u1 do arquivo .class*/
#define u1READ(arq) getc(arq)

/* Macro para leitura de um tipo u2 do arquivo .class*/
#define u2READ(arq) (getc(arq) << 8) | getc(arq)

/* Macro para leitura de um tipo u4 do arquivo .class*/
#define u4READ(arq) (getc(arq) << 24) | (getc(arq) << 16 | getc(arq) << 8 | getc(arq))

