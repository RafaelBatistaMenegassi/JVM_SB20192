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
 * Leitura (lib)
 * -> de arquivos .class
 * 
 ************************************/

#ifndef LEITURA_INIT
#define LEITURA_INIT

// Inclusoes necessarias
#include <stdio.h>
#include <stdlib.h>
#include "classfile.h" 
// #include "instrucoes.h"  VERIFICAR

// --- LEITURAS

/* FUNCAO PRINCIPAL */
/* Funcao de leitura do arquivo class em sua totalidade */
Classfile*                  lerArquivoClass         (char *);

/* FUNÇÕES AUXILIARES PARA LEITURA DAS PARTES ESPECÍFICAS DO ARQUIVO .CLASS */
Cp_info*                    lerConstantPool         (FILE *, u2);
u2*                         lerInterfaces           (FILE *fp, u2 i_count);
Field_info*                 lerFields               (FILE *fp, u2 f_count, Cp_info *cp);    // EU
Method_info*                lerMetodos              (FILE *fp, u2 m_count, Cp_info *cp);
Attribute_info*             lerAtributos            (FILE *fp, Cp_info *cp);
Code_attribute*             lerCode                 (FILE *fp, Cp_info *cp);
Line_number_table*          lerLineNumberTable      (FILE *fp, Cp_info * cp);
Exception_info*             lerExceptionTable       (FILE *fp, u2 size);
Exception_attribute*        lerExceptionsAttribute  (FILE *fp);
Constant_value_attribute*   lerConstantValue        (FILE *fp);
Signature_attribute*        lerSignature            (FILE *fp);                             // EU
Inner_classes_attribute*    lerInnerClasses         (FILE *fp, Cp_info *cp);
Classes*                    lerClasses              (FILE *fp);
Stack_map_attribute*        lerStackMapTable        (FILE *fp);
Stack_map_frame*            lerStackMapFrame        (FILE *fp);
Verification_type_info*     lerVerificationTypeInfo (FILE *fp);
Source_file_attribute*      lerSourceFile           (FILE *fp);                             // EU

// --- DECODIFICACOES

/*
/* Funcao de decodificacao de operandos e argumentos de uma instrucao 
char* decodificacaoOperandoInstrucao(cp_info *cp, u2 index, u2 sizeCP);

/* Funcao de decodificacao do atributo Code de um metodo 
char* decodificacaoCode(cp_info *cp, u2 sizeCP, u1 *code, u4 length, instrucao *instrucoes);
*/

#endif