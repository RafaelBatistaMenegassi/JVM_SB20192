all:
	gcc -std=c99 -Wall lista_operandos.h pilha_operandos.h interpretador.h lista_frames.h pilha_frames.h classfile.h instrucoes.h leitor_exibidor.h lista_classes.h jvm.h lista_operandos.c pilha_operandos.c interpretador.c lista_frames.c pilha_frames.c instrucoes.c leitor_exibidor.c lista_classes.c jvm.c main.c -lm -o jvm

debug:
	gcc -g -std=c99 -Wall lista_operandos.h pilha_operandos.h interpretador.h lista_frames.h pilha_frames.h classfile.h instrucoes.h leitor_exibidor.h lista_classes.h jvm.h lista_operandos.c pilha_operandos.c interpretador.c lista_frames.c pilha_frames.c instrucoes.c leitor_exibidor.c lista_classes.c jvm.c main.c -lm -o jvm

clean_jvm:
	rm jvm
	
clean:
	rm a.out
	rm *.gch
