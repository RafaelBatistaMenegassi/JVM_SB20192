/*Substitui os arquivos: classFileStruct.h + structures.h*/

/*
Universidade de Brasília - 02/2019
Software Básico - Turma A

Alunos:
    Brenda Souza
    Jéssica Oliveira
    Rafael Alencar
    Rafael Batista
    Rodrigo Cardoso

Projeto:
    Leitor/Exibidor de arquivos .class.

Arquivo:
    "ClassFile.h"

Descrição:
    Especificação de estruturas relativas a arquivos de extensão ".class".
*/

#ifndef CLASSFILE_INIT
#define CLASSFILE_INIT

/*
Implementado seguindo as especificações da Java Virtual Machine:
https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html#jvms-4.1

ClassFile {
    u4             magic;
    u2             minor_version;
    u2             major_version;
    u2             constant_pool_count;
    cp_info        constant_pool[constant_pool_count-1];
    u2             access_flags;
    u2             this_class;
    u2             super_class;
    u2             interfaces_count;
    u2             interfaces[interfaces_count];
    u2             fields_count;
    field_info     fields[fields_count];
    u2             methods_count;
    method_info    methods[methods_count];
    u2             attributes_count;
    attribute_info attributes[attributes_count];
}

Definição de tipagem seguindo os seguintes preceitos:

    - u1 -> uint8_t;
    - u2 -> uint16_t;
    - u4 -> uint32_t;
    - cp_info -> struct complexa de tipagem da Constant Pool;
    - field_info -> struct complexa de tipagem de Fields;
    - method_info -> struct complexa de tipagem de Methods;
    - attribute_info -> struct complexa de tipagem de Attributes;

Para maiores detalhes, verificar implementação.
*/

#include <stdint.h> // biblioteca que define o "tamanho das coisas"
#include "pilha_operandos.h"
/* ------------- */
/* TIPAGEM GERAL */
/* ------------- */

typedef uint8_t     u1;
typedef uint16_t    u2;
typedef uint32_t    u4;
typedef uint64_t    u8;

// Valores MAX para debug
#define NAME_INDEX 1
#define NAME_AND_TYPE 2
#define STRING_INDEX 3
#define CLASS_INDEX 4
#define NAME_AND_TYPE_INFO_NAME_INDEX 5
#define NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX 6
#define FIELD_INDEX 7
#define MAXU1 255
#define MAXU2 65535
#define MAXU4 2147483647

// Informaces referentes a  Constant Pool
typedef struct cp_info
{
    // Byte de tag: indica qual o tipo de entrada na CP
    u1 tag;

    // Union com tipos de entrada da CP
    union
    {
        struct
        {
            u2 name;
        } Class;

        struct
        {
            u2 class_name;
            u2 name_and_type;
        } Field;

        struct
        {
            u2 class_name;
            u2 name_and_type;
        } Method;

        struct
        {
            u2 class_name;
            u2 name_and_type;
        } InterfaceMethod;

        struct
        {
            u2 string;
        } String;

        struct
        {
            u4 bytes;
        } Integer;

        struct
        {
            u4 bytes;
        } Float;

        struct
        {
            u4 hi_bytes;
            u4 lo_bytes;
        } Long;

        struct
        {
            u4 hi_bytes;
            u4 lo_bytes;
        } Double;

        struct
        {
            u2 name;
            u2 descriptor;
        } NameAndType;

        struct
        {
            u1 *bytes;
            u2 length;
        } Utf8;

        struct
        {
            u1 reference_kind;
            u2 reference;
        } MethodHandle;

        struct
        {
            u2 descriptor;
        } MethodType;

        struct
        {
            u2 name_and_type;
            u2 bootstrap_method_attr;
        } InvokeDynamic;

    } cp_union;
} Cp_info;

// Enum para os possiveis valores da tag de entrada da CP
enum tag_values
{
	CONSTANT_Utf8                   = 1,
	CONSTANT_Integer                = 3,
	CONSTANT_Float                  = 4,
	CONSTANT_Long                   = 5,
	CONSTANT_Double                 = 6,
	CONSTANT_Class                  = 7,
	CONSTANT_String                 = 8,
	CONSTANT_Field                  = 9,
	CONSTANT_Method                 = 10,
	CONSTANT_InterfaceMethod        = 11,
	CONSTANT_NameAndType            = 12,
	CONSTANT_MethodHandle           = 15,
	CONSTANT_MethodType             = 16,
	CONSTANT_InvokeDynamic          = 18
};

// Enum de possíveis valores de flags de acesso
typedef enum access_flags
{
	PUBLIC      = 1,
	PRIVATE     = 2,
	PROTECTED   = 4,
	STATIC      = 8,
	FINAL       = 16,
	VOLATILE    = 64,
	TRANSIENT   = 128,
	SYNTHETIC   = 4096,
	ENUM        = 16384
} Access_flags;

enum array_type
{
    T_BOOLEAN = 4,
    T_CHAR,
	T_FLOAT,
	T_DOUBLE,
	T_BYTE,
	T_SHORT,
	T_INT,
	T_LONG
};

// Informações sobre Atributos
typedef struct attribute_info
{
    u2 attribute_name; // index válido na CP indicando o nome do atributo
    u4 attribute_length;
    void *info;
} Attribute_info;

typedef struct static_data
{
  u4 *low;
  u4 *hi;
  u1 *string;
} Static_data;

// Informações sobre Campos (Fields)
typedef struct field_info
{
    u2 name;
    u2 descriptor;
    u2 attributes_count;
    u2 access_flags;
    Attribute_info **attributes;
} Field_info;

// Informações sobre Exceções
typedef struct exception_info
{
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
} Exception_info;

// Informações sobre Número de Linhas
typedef struct line_number_info
{
    u2 line_number;
    u2 start_pc;
} Line_number_info;

// Informações sobre Tabela de Linhas
typedef struct line_number_table
{
    Line_number_info *info;
    u2 line_number_length;
} Line_number_table;

// Informações gerais de Código (Code)
typedef struct code_attribute
{
    u1 *code;

    u2 attributes_count;
    u2 exception_info_length;
    u2 max_locals;
    u2 max_stack;

    u4 code_length;

    Attribute_info **attributes;

    Exception_info *ex_info;
} Code_attribute;

// Informações sobre Métodos (Methods)
typedef struct method_info
{
    u2 name;
    u2 descriptor;
    u2 access_flags;

    u2 attributes_count;
    Attribute_info **attributes;
} Method_info;

// Informações sobre Atributos do Fonte
typedef struct source_file_attribute
{
    u2 source_file;
} Source_file_attribute;

// Informações sobre Atributos de Constantes
typedef struct constant_value_attribute
{
    u2 constant_value;
} Constant_value_attribute;

// Informações sobre Atributos de Exceções
typedef struct exception_attribute
{
    u2 num_exceptions;
    u2 *exception_info;
} Exception_attribute;

// Informações sobre Classes
typedef struct classes
{
    u2 inner_name;

    u2 inner_class_info;
    u2 inner_class_access_flags;

    u2 outer_class_info;
} Classes;

// Informações sobre Atributos de Classes Internas (Inner Classes)
typedef struct inner_classes_attribute
{
    u2 num_classes;
    Classes **classes_array;
} Inner_classes_attribute;

typedef struct enclosingMethod_attribute
{
  u2 class;
  u2 method_index;

} EnclosingMethod_attribute;

// Informações sobre Atributos de Assinatura
typedef struct signature_attribute
{
    u2 signature;
} Signature_attribute;

typedef struct source_debug_extension
{
  u1 *debug_extension; // Aloca com o tamano de attribute_length
} Source_debug;

typedef struct local_variable_table
{
  u2 start_pc;
  u2 length;
	u2 name_index;
	u2 descriptor_index;
	u2 index;

} Local_variable;

typedef struct localVariableTable_attributes
{

	u2 local_variable_table_length;
	Local_variable *local_variables; //Tamanho de local_variable_table_length

} Local_variable_attributes;

/*Adicionados*/
struct element_value
{

	u1 tag;
	union{

		u2 const_value_index;
		u2 outer_class_info_index;

		struct annotation *annotation_value;	// ISSO DAQUI EH MUITO ERRADO! VERIFICAR

		struct {

			u2 type_name_index;
			u2 const_name_index;

		} enum_const_index;

		struct {

			u2 num_values;
			struct element_value *element_values;

		} array_value;

	} value;

};
typedef struct element_value element_value;

struct element_value_pairs
{

	u2 element_name_index;
	element_value value;

};
typedef struct element_value_pairs element_value_pairs;

struct annotation
{

	u2 type_index;
	u2 num_element_value_pairs;
	element_value_pairs *elementes_pairs;

};
typedef struct annotation annotation;


struct runTimeVisibleAnnotations_attribute
{
	u2 num_annotations;
	annotation *annotations;
};
typedef struct runTimeVisibleAnnotations_attribute runTimeVisibleAnnotations_attribute;

struct runTimeInvisibleAnnotations_attribute {

	u2 num_annotations;
	annotation *annotations;	// Alocar com o tamanho de num_annotations;

};
typedef struct runTimeInvisibleAnnotations_attribute runTimeInvisibleAnnotations_attribute;

struct parameter_annotations {		// ESSA ESTRUTURA EH IGUAL A DE CIMA.

	u2 num_annotations;
	annotation *annotations;	// Alocar com o tamanho de num_annotations;

};
typedef struct parameter_annotations parameter_annotations;

struct runtimeVisibleParameterAnnotations_attribute {

    u1 num_parameters;
    parameter_annotations *parameters_annotations;	// Alocar com o tamanho de num_parameters;

};
typedef struct runtimeVisibleParameterAnnotations_attribute runtimeVisibleParameterAnnotations_attribute;

struct runtimeInvisibleParameterAnnotations_attribute {

    u1 num_parameters;
	parameter_annotations *parameters_annotations;	// Alocar com o tamanho de num_parameters;

};
typedef struct runtimeInvisibleParameterAnnotations_attribute runtimeInvisibleParameterAnnotations_attribute;

struct annotationDefault_attribute {

    element_value default_value;

};
typedef struct annotationDefault_attribute annotationDefault_attribute;

struct bootstrap_methods
{

	u2 bootstrap_method_ref;
	u2 num_bootstrap_arguments;
    u2 *bootstrap_arguments;		// Alocar com o tamanho num_bootstrap_arguments;


};
typedef struct bootstrap_methods bootstrap_methods;

struct bootstrapMethods_attribute
{

    u2 num_bootstrap_methods;

    bootstrap_methods *bt_methods; // Alocar com o tamanho de num_bootstrap_methods;

};
typedef struct bootstrapMethods_attribute bootstrapMethods_attribute;
//_________________________________-

// Informações de Verificação de Tipos
typedef struct verification_type_info
{
    // Byte de tag de tipo de entrada
    u1 tag;
    /*
      Tag = 0 -> top variable Info
      Tag = 1 -> integer
      Tag = 2 -> Float
      tag = 3 -> Double
      tag = 4 -> Long
      tag = 5 -> null
      tag = 6 -> uninitializedThis
    */
    // Union de verificação de tipos
    union
    {
        struct
        {
            u2 cp; // index da classe na CP
        } object_variable_info; // tag = 7

        struct
        {
            u2 offset;
        } uninitialized_variable_info; // tag = 8

    } type_info;
} Verification_type_info;

// Mapeamento de Pilha de Frames
typedef struct stack_map_frame
{
    // Tipagem de Frame
    u1 frame_type;

    // Union de tipos de Frames
    union
    {
        struct
        {
            Verification_type_info **stack;
        } same_locals_1_stack_item_frame;

        struct
        {
            u2 offset_delta;
            Verification_type_info **stack;
        } same_locals_1_stack_item_frame_extended;

        struct
        {
            u2 offset_delta;
        } chop_frame;

        struct
        {
            u2 offset_delta;
        } same_frame_extended;

        struct
        {
            u2 offset_delta;
            Verification_type_info **locals;
        } append_frame;

        struct
        {
            u2 offset_delta;
            u2 num_locals;
            u2 num_stack_items;

            Verification_type_info **locals;
            Verification_type_info ** stack;
        } full_frame;
    } map_frame_type;
} Stack_map_frame;

// Informações sobre Atributos de Mapeamento de Pilha
typedef struct stack_map_attribute
{
    u2 num_entries;
    Stack_map_frame **entries;
} Stack_map_attribute;

typedef struct vetor_locais
{
  u4 *variavel;
  u1 tipo_variavel;
} Vetor_locais;

typedef struct frame
{
  u4 return_address; // Verificar se é realmente endereço de retorno
	pilha_operandos *p;
	Vetor_locais *v;
	u2 vetor_length;
	Cp_info *cp;
	char *classeCorrente;
} Frame;
/* ------------- */
/* ARQUIVO .CLASS */
/* ------------- */
// Estrutura referente ao arquivo .class em si
typedef struct classfile
{
    u4              magic;                  // The magic item supplies the magic number identifying the class file format; it has the value 0xCAFEBABE.
    u2              minor_version;          // The values of the minor_version and major_version items are the minor and major version numbers of this class file.
    u2              major_version;          // Together, a major and a minor version number determine the version of the class file format.
    u2              constant_pool_count;    // The value of the constant_pool_count item is equal to the number of entries in the constant_pool table plus one.
    Cp_info         *constant_pool;         // The constant_pool is a table of structures (§4.4) representing various string constants, class and interface names, field names, and other constants that are referred to within the ClassFile structure and its substructures.
    u2              access_flags;           // The value of the access_flags item is a mask of flags used to denote access permissions to and properties of this class or interface.
    u2              this_class;             // The value of the this_class item must be a valid index into the constant_pool table.
    u2              super_class;            // For a class, the value of the super_class item either must be zero or must be a valid index into the constant_pool table.
    u2              interfaces_count;       // The value of the interfaces_count item gives the number of direct superinterfaces of this class or interface type.
    u2              *interfaces;            // Each value in the interfaces array must be a valid index into the constant_pool table.
    u2              fields_count;           // The value of the fields_count item gives the number of field_info structures in the fields table.
    Field_info      *fields;                // Each value in the fields table must be a field_info (§4.5) structure giving a complete description of a field in this class or interface.
    u2              methods_count;           // The value of the methods_count item gives the number of method_info structures in the methods table.
    Method_info     *methods;               // Each value in the methods table must be a method_info (§4.6) structure giving a complete description of a method in this class or interface.
    u2              attributes_count;       // The value of the attributes_count item gives the number of attributes (§4.7) in the attributes table of this class.
    Attribute_info  **attributes;           // Each value of the attributes table must be an attribute_info (§4.7) structure.
} Classfile;

#endif
