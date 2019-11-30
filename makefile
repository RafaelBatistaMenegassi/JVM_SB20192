all:
	gcc -std=c99 -Wall structures.h lista_operandos.h pilha_operandos.h interpretador.h lista_frames.h pilha_frames.h classFileStruct.h instrucoes.h leitura.h lista_classes.h jvm.h lista_operandos.c pilha_operandos.c interpretador.c lista_frames.c pilha_frames.c instrucoes.c leitura.c lista_classes.c jvm.c main.c -lm

debug:
	gcc -g -std=c99 -Wall structures.h lista_operandos.h pilha_operandos.h interpretador.h lista_frames.h pilha_frames.h classFileStruct.h instrucoes.h leitura.h lista_classes.h jvm.h lista_operandos.c pilha_operandos.c interpretador.c lista_frames.c pilha_frames.c instrucoes.c leitura.c lista_classes.c jvm.c main.c -lm

clean:
	rm a.out
	rm *.gch
