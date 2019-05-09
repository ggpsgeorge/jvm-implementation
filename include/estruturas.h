//#################################################################################################
/*! \file estruturas.h
 *
 *  \brief Estruturas da JVM.
 *
 *  Arquivo com a definicao das estruturas, tipos e constantes associadas ao sistema.
 */
//##################################################################################################

#ifndef ESTRUT
#define ESTRUT

#include <stdint.h>

//--------------------------------------------------------------------------------------------------
// Definicoes de sucesso e erro de operacoes

//Erros do leitor .class
#define LinkageSuccess                              0 //!< Sucesso na operacao
#define LinkageError_ClassFormatError               1 //!< Arquivo corrompido
#define LinkageError_UnsupportedClassVersionError   2 //!< Arquivo possui versao nao suportada
#define LinkageError_ClassCirculatityError          3 //!< Classe seria a sua propria superclasse
#define LinkageError_NoClassDefFoundError           4 //!< Erro de abertura do arquivo

//Erros do inicializador de classes
#define InitializerSuccess                          10 //!< Sucesso na operacao
#define InitializerError                            11 //!< Sucesso na inicializacao da classe


//--------------------------------------------------------------------------------------------------
// Definicoes de prerequisitos do sistema

#define LECLASS_MIN_Version        0 //!< Versao minima compativel
#define LECLASS_MAJ_Version        48 //!< Versao maxima compativel


//--------------------------------------------------------------------------------------------------
//Tags do Pool de Constantes

#define CONSTANT_Class              7
#define CONSTANT_Fieldref           9
#define CONSTANT_Methodref          10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String             8
#define CONSTANT_Integer            3
#define CONSTANT_Float              4
#define CONSTANT_Long               5
#define CONSTANT_Double             6
#define CONSTANT_NameAndType        12
#define CONSTANT_Utf8               1


//--------------------------------------------------------------------------------------------------
//Flags de acesso

#define ACC_PUBLIC                  0x0001 //!< Publica: pode ser acessada de fora do pacote.
#define ACC_PRIVATE                 0x0002 //!< Privada: contexto restrito a definição da classe.
#define ACC_PROTECTED               0x0004 //!< Protegida: pode ser usada na classe e nas subclasses.
#define ACC_STATIC                  0x0008 //!< Estatica: variavel de classe e nao de instancia.
#define ACC_FINAL                   0x0010 //!< Final: não pode ter subclasses.
#define ACC_SUPER                   0x0020 //!< Chama metodos de superclasse via a instrucao
//!< invokespecial.
#define ACC_SYNCHRONIZED            0x0020 //!< Sincronizado: requer um monitor antes de ser
//!< executado (Thread).
#define ACC_VOLATILE                0x0040 //!< Nao pode ser colocada em cache.
#define ACC_TRANSIENT               0x0080 //!< não pode ser lida ou gravada por um gerente de objetos
//!< persistente.
#define ACC_NATIVE                  0x0100 //!< Implementado em linguagem nao Java (C, C++, Assembly).
#define ACC_INTERFACE               0x0200 //!< Eh interface, nao uma classe.
#define ACC_ABSTRACT                0x0400 //!< Declarada abstrata: nao pode ser instanciada.
#define ACC_STRICT                  0x0800 //!< utiliza modo de ponto flutuante FP-strict (nao normalizado).


//--------------------------------------------------------------------------------------------------
//Definicoes de tipos de atributos
#define ATT_Code            L"Code"
#define ATT_ConstantValue   L"ConstantValue"
#define ATT_Exceptions      L"Exeptions"


//--------------------------------------------------------------------------------------------------
//Definicoes de constantes para ponto flutuante

#define FLOAT_Positive_infity 0x7f800000 //!< Float infinito positivo
#define FLOAT_Negative_infity 0xff800000 //!< Float infinito negativo
#define FLOAT_MIN_NaN1        0x7f800001 //!< Float valor minimo da primeira faixa de NaN
#define FLOAT_MAX_NaN1        0x7fffffff //!< Float valor maximo da primeira faixa de NaN
#define FLOAT_MIN_NaN2        0xff800001 //!< Float valor minimo da segunda faixa de NaN
#define FLOAT_MAX_NaN2        0xffffffff //!< Float valor maximo da segunda faixa de NaN


//--------------------------------------------------------------------------------------------------
//Definicoes de constantes para double

#define DOUBLE_Positive_infity 0x7ff0000000000000 //!< Double infinito positivo
#define DOUBLE_Negative_infity 0xfff0000000000000 //!< Double infinito negativo
#define DOUBLE_MIN_NaN1        0x7ff0000000000001 //!< Double valor minimo da primeira faixa de NaN
#define DOUBLE_MAX_NaN1        0x7fffffffffffffff //!< Double valor maximo da primeira faixa de NaN
#define DOUBLE_MIN_NaN2        0xfff0000000000001 //!< Double valor minimo da segunda faixa de NaN
#define DOUBLE_MAX_NaN2        0xffffffffffffffff //!< Double valor maximo da segunda faixa de NaN

//--------------------------------------------------------------------------------------------------
//Definicao String

#define STRING_LENGTH 100

//--------------------------------------------------------------------------------------------------
//Tipos de dados

typedef unsigned char u1; //!< Representa 1 byte
typedef unsigned short u2; //!< Representa 2 bytes
typedef unsigned int  u4; //!< Representa 4 bytes
typedef uint64_t u8; //!< Representa 4 bytes
typedef unsigned short OPresult; //!< Representa o resultado de uma operacao
typedef  char String[STRING_LENGTH];


//--------------------------------------------------------------------------------------------------
//! Estrutura do Pool de Constantes
/*!
 * Estrutura utilizada para a representacao de um pool de constantes.
 */

typedef struct constPool{
    u1 tag;
    union{
        struct{
            u2 name_index;
        } Class;
        struct{
            u2 class_index;
            u2 name_and_type_index;
        } Fieldref;
        struct{
            u2 name_index;
            u2 descriptor_index;
        } NameAndType;
        struct{
            u2 lenght;
            u1* bytes; //!< bytes[length]
        } Utf8;
        struct{
            u2 class_index;
            u2 name_and_type_index;
        } Methodref;
        struct{
            u2 class_index;
            u2 name_and_type_index;
        } InterfaceMethodref;
        struct{
            u2 string_index;
        } String;
        struct{
            u4 bytes;
        } Integer;
        struct{
            u4 bytes;
        } Float;
        struct{
            u4 high_bytes;
            u4 low_bytes;
        } Long;
        struct{
            u4 high_bytes;
            u4 low_bytes;
        } Double;
    } u;
} cp_info;


//--------------------------------------------------------------------------------------------------
//! Estrutura de Atributos
/*!
 * Estrutura utilizada para a representacao de um atributo de um arquivo ".class".
 */

typedef struct attribute{
    u2 attribute_name_index;
    u4  attribute_length;
    u1* info;
    
} attribute_info;


//--------------------------------------------------------------------------------------------------
//! Estrutura de uma tabela de excessoes
/*!
 * Estrutura utilizada para a representacao de uma tabela de excessoes de um atributo do tipo code
 */
typedef struct ExceptionTable{
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
}	ExceptionTable;

//--------------------------------------------------------------------------------------------------
//! Estrutura do Atributo code
/*!
 * Estrutura utilizada para a representacao de um atributo do tipo code
 */
typedef struct CodeAttribute{
    u2 attribute_name_index;
    u4 attribute_length;
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1* code;
    u2 exception_table_length;
    ExceptionTable* exception_table;
    u2 attributes_count;
    u1* attributes;
} CodeAttribute;


//--------------------------------------------------------------------------------------------------
//! Estrutura do Atributo Constant Value
/*!
 * Estrutura utilizada para a representacao de um atributo do tipo constant value
 */
typedef struct ConstantValueAttribute{
    u2 attribute_name_index;
    u4 attribute_length;
    u2 constantvalue_index;
} ConstantValueAttribute;


//--------------------------------------------------------------------------------------------------
//! Estrutura do Atributo Exception
/*!
 * Estrutura utilizada para a representacao de um atributo do tipo exception
 */
typedef struct ExceptionAttribute{
    u2 attribute_name_index;
    u4 attribute_length;
    u2 number_of_exceptions;
    u2* exception_index_table;
} ExceptionAttribute;


//--------------------------------------------------------------------------------------------------
//! Estrutura de Campos e Metodos
/*!
 * Estrutura utilizada para a representacao de um campo ou metodo.
 */
typedef struct fieldAndMethod{
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info* attributes;
    
} field_or_method;
typedef struct fieldAndMethod field_info; //!< Estrutura utilizada para a representacao de um campo.
typedef struct fieldAndMethod method_info; //!< Estrutura utilizada para a representacao de
//!um metodo.


//--------------------------------------------------------------------------------------------------
//! Estrutura de Arquivo .class
/*!
 * Estrutura utilizada para a representacao de um arquivo ".class".
 */

typedef struct arqClass {
    u4                  magic;
    u2                  minor_version;
    u2                  major_version;
    u2                  constant_pool_count;
    cp_info*            constant_pool;
    u2                  access_flags;
    u2                  this_class;
    u2                  super_class;
    u2                  interfaces_count;
    u2*                 interfaces;
    u2                  fields_count;
    field_info*         fields;
    u2                  methods_count;
    method_info*        methods;
    u2                  attributes_count;
    attribute_info*     attributes;
    
    
} ArqClass;




//--------------------------------------------------------------------------------------------------
//! Estrutura da FieldsTable
/*!
 * 
 */
typedef struct FieldsTable{
    char* name; //!< Aponta para o nome do campo
    char* descriptor; //!< Aponta para o descritor do campo
    void* memoryAddress; //!< Endereco de memoria contendo o valor do campo
    field_info* fieldInfo; //!< Aponta para a estrutura field_info referente ao campo
} FieldsTable;


//--------------------------------------------------------------------------------------------------
//! Estrutura da FieldsTable
/*!
 *
 */
typedef struct Fields{
    int fieldsCount; //!< Tamanho da tabela
    FieldsTable* fieldsTable; //!< Ponteiro para a tabela
} Fields;


struct JavaClass;

//--------------------------------------------------------------------------------------------------
//! Estrutura da Handler
/*!
 *
 */
typedef struct Handler{
    struct JavaClass *javaClass;
    Fields* fields;
} Handler;


//--------------------------------------------------------------------------------------------------
//! Estrutura do Object
/*!
 * 
 */
typedef struct Object{
    Handler *handler;
} Object;


//--------------------------------------------------------------------------------------------------
//! Estrutura da ObjectList
/*!
 * 
 */
typedef struct ObjectList{
    struct ObjectList *next;
    Object *object;
} ObjectList;


//--------------------------------------------------------------------------------------------------
//! Estrutura da JavaClass
/*!
 * A StaticFieldsTable do JavaClass foi generalizada como uma FieldsTable normal
 */
typedef struct JavaClass{
    ObjectList *objectList;
    ArqClass *arqClass;
    Fields* staticFields;
}JavaClass;


//--------------------------------------------------------------------------------------------------
//! Estrutura da ClassTable
/*!
 * ClassTable mesmo sem a criacao do JavaClass
 */
typedef struct ClassTable{
    char* name;
    JavaClass *javaClass;
}ClassTable;


//--------------------------------------------------------------------------------------------------
//! Estrutura da MethodArea
/*!
 * 
 */
typedef struct MethodArea{
    int classCount;
    ClassTable *classTable;
} MethodArea;


//--------------------------------------------------------------------------------------------------
//! Estrutura da Stack
/*!
 * Abstracao da pilha para que ficasse mais simples a estrutura das outra
 * Caso seja uma abstracao desnecessaria basta modificar as outras pilhas
 * Criando os vetores com seus respectivos tipos
 */
typedef struct Stack{
    void *stk;
    void *top;
}Stack;

typedef struct OperandStack{
    struct OperandStack* nextStack;
    u4 top;
}OperandStack;


//--------------------------------------------------------------------------------------------------
//! Estrutura da Frame
/*!
 * Construcao da estrutura de Frame
 */
typedef struct Frame{
    JavaClass *javaClass;
    method_info *method_info;
    int returnPC;
    u4 *localVariablesVector;
    OperandStack *opStk;
}Frame;

typedef struct VMStack{
    Frame *top;
    struct VMStack *next;
}VMStack;


//--------------------------------------------------------------------------------------------------
//! Estrutura da Thread
/*!
 * Construcao da estrutura de Thread
 */
typedef struct Thread{
    int PC;
    VMStack *vmStack;
}Thread;


//--------------------------------------------------------------------------------------------------
//! Estrutura do ambiente de execucao da JVM
/*!
 * Estrura que apresenta um ambiente de execucao, a qual contem uma thread e uma area de metodos 
 */
typedef struct Environment{
    Thread *thread; //!< Referencia para a thread em execucao
    MethodArea* methodArea; //!< Referencia para a area de metodos
    u1 debugFlags; //!< Flags de debug
}Environment;


//--------------------------------------------------------------------------------------------------
//! Estrutura do auxiliar de comparação de pares para a TableSwitch
/*!
 * Estrura que apresenta um indicador de igualdade e o offset 
 */
typedef struct npair{
    u4 match; //!< Indicador de igualdade entre o pc e o offset
    u4 offset; //!< Offset
}npair;

//--------------------------------------------------------------------------------------------------
//! Ponteiro para uma instrucao
/*!
 * Tipo que define um ponteiro para uma instrucao. Todas as instrucoes devem receber uma referencia
 * para a o ambiente de execucao.
 */
typedef void (*instruction)(Environment*);
#endif