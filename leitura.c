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

// Includes necessarios
#include "leitura.h"

/* VERIFICAR necessidade
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include "instrucoes.h"
*/

/* Macro para leitura de um tipo u1 do arquivo .class*/
#define u1READ(arq) getc(arq)

/* Macro para leitura de um tipo u2 do arquivo .class*/
#define u2READ(arq) (getc(arq) << 8) | getc(arq)

/* Macro para leitura de um tipo u4 do arquivo .class*/
#define u4READ(arq) (getc(arq) << 24) | (getc(arq) << 16 | getc(arq) << 8 | getc(arq))

// lerArquivoClass

// lerConstantPool

Field_info* lerFields (FILE *fp, u2 f_count, Cp_info *cp)
{
    Field_info *fieldsPtr = (Field_info*)malloc(f_count * sizeof(Field_info));

    for (Field_info * itPtr = fieldsPtr; itPtr < fieldsPtr + f_count; itPtr++)
    {
        itPtr->access_flags = u2READ(fp);
        itPtr->name = u2READ(fp);
        itPtr->descriptor = u2READ(fp);
        itPtr->attributes_count = u2READ(fp);

        if (itPtr->attributes_count > 0)
        {
            itPtr->attributes = (Attribute_info**)malloc(itPtr->attributes_count*sizeof(Attribute_info*));

            for (int pos = 0; pos < itPtr->attributes_count; pos++)
            {
                *(itPtr->attributes - pos) = lerAtributos(fp, cp);
            }            
        }        
    }

    return fieldsPtr;
    
}

Signature_attribute* lerSignature (FILE *fp)
{
    Signature_attribute *signaturePtr = (Signature_attribute*)malloc(sizeof(Signature_attribute));

    signaturePtr->signature = u2READ(fp);

    return signaturePtr;
}

Source_file_attribute* lerSourceFile (FILE *fp)
{
    Source_file_attribute *sourceFilePtr = (Source_file_attribute*)malloc(sizeof(Source_file_attribute));

    sourceFilePtr->source_file = u2READ(fp);

    return sourceFilePtr;
}