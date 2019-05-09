//#################################################################################################
/*! \file classloader.h
 *
 *  \brief Interface do Carregador de Classes da JVM.
 *
 *  Interface responsavel por implementar os servicos relacionados ao carregamento de classes da
 *  JVM. Contem submodulos responsaveis por: 
 *  - Implementar os metodos relacionados a exibicao de um arquivo ".class".
 *  - Implementar os metodos de leitura de arquivos ".class"
 *  - Implementar os metodos relacionados a manutencao de uma estrutura ArqClass
 *  - Implementar os metodos responsaveis pela manipulacao de arquivos
 */
//##################################################################################################

#ifndef CLASSLOAD_h
#define CLASSLOAD_h
#ifdef CLASSLOAD
#define EXTC
#else
#define EXTC extern
#endif

#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"

// Funcoes de persistencia do Leitor e Exibidor de ClassFile
// Interfaces responsaveis por prover os metodos de manipulacao de arquivos ".class".

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por abrir e retornar um arquivo em modo de leitura.
 *
 * \param nome_arquivo Nome ou caminho do arquivo a ser aberto.
 */
EXTC FILE* obter_entrada(const char * nome_arquivo);


//--------------------------------------------------------------------------------------------------
/*!
 * Le um tipo u1 (8bits) do arquivo.
 *
 * \param arquivo Arquivo no qual sera realizada a leitura.
 */
EXTC u1 u1Le(FILE *arquivo);



//--------------------------------------------------------------------------------------------------
/*!
 * Le um tipo u2 (16bits) do arquivo.
 *
 * \param arquivo Arquivo no qual sera realizada a leitura.
 */
EXTC u2 u2Le(FILE *arquivo);


//--------------------------------------------------------------------------------------------------
/*!
 * Le um tipo u4 (32bits) do arquivo.
 *
 * \param arquivo Arquivo no qual sera realizada a leitura.
 */
EXTC u4 u4Le(FILE *arquivo);


// Funcoes de Interface do Leitor e exibidor e .class
// Interfaces responsaveis por prover os metodos relacionados a leitura e exibicao de um arquivo ".class".

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por prover o servico de leitura de um arquivo ".class", preenchendo uma 
 * estrutura ArqClass.
 *
 * \param arq_class         Estrutura inicializada do tipo ArqClass a ser preenchida.
 * \param arquivo_entrada   Arquivo ".class" a ser lido.
 * \return Resultado da operacao
 */
EXTC OPresult LECLASS_leitor(ArqClass* arq_class, const char* arquivo_entrada);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por prover o servico de exibicao de uma estrutura ".class"
 *
 * \param arq_class         Estrutura inicializada do tipo ArqClass a ser exibida.
 * \return Resultado da operacao
 */
EXTC OPresult LECLASS_exibidor(ArqClass* arq_class);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por prover o servico de liberacao recursiva de uma estrutura ".class"
 *
 * \param arq_class         Estrutura inicializada do tipo ArqClass a ser liberada.
 */
EXTC void LECLASS_free(ArqClass* arq_class);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por prover mensagens de resuldados de erro de operacoes do leitor ".class"
 *
 * \param resultado         Tipo resultado a ser decodificado
 * \param fileName         Nome do arquivo
 */
EXTC void LECLASS_exibeErroOperacao(OPresult resultado, const char* fileName);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que obtem uma string de caracteres unicode no pool de constantes de uma estrutura
 * CONST_UTF8 referente ao nome da classe apontada pelo indice passado
 *
 * \param cp      ponteiro para o pool de constantes
 * \param index   indice da classe no pool de constantes.
 *  \return        string de char de 16bits (w_char_t).
 */
EXTC char* getClassNameFromConstantPool(cp_info* cp, u2 index);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que, dado um indice valido de field_info ou methdo_info para um pool de constantes, 
 * procura e retorna por referencia o nome da classe do field/metd, o nome do field/metd, e o 
 * descritor do field/metd.
 *
 * \param index Indice valido para o fieldInfo.
 * \param constant_pool Referencia para o pool de constantes a ser utilizado.
 * \param class_name Referencia para um vetor de char a receber o nome da classe do field/metd
 * \param name Referencia para um vetor de char a receber o nome do field/metd
 * \param descriptor Referencia para um vetor de char a receber o descritor do field/metd
 */
EXTC void getFieldOrMethodInfoAttributesFromConstantPool(u2 index,
                                                 cp_info* constant_pool,
                                                 char** class_name,
                                                 char** name,
                                                 char** descriptor);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que obtem uma string de caracteres unicode no pool de constantes de uma estrutura
 * CONST_UTF8 apontada pelo indice passado
 *
 * \param cp      ponteiro para o pool de constantes
 * \param index   indice da estrutura UTF8_Info no pool de constantes.
 *  \return        string de char de 16bits (w_char_t).
 */
EXTC wchar_t * getUnicodeFromConstantPool(cp_info* cp, u2 index);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que obtem uma string de caracteres UTF8 no pool de constantes de uma estrutura
 * CONST_UTF8 apontada pelo indice passado
 *
 * \param cp      ponteiro para o pool de constantes
 * \param index   indice da estrutura UTF8_Info no pool de constantes.
 *  \return        string de char de 8bits (char *).
 */
EXTC char * getUTF8FromConstantPool(cp_info* cp, u2 index);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que, dado uma referencia para um vetor de informacoes de um atributo do tipo code,
 * preenche e retorna uma estrutura
 * CodeAttribute.
 *
 * \param info Referencia para o atributo code
 * \return Estrutura codeAttribute preenchida
 */
EXTC CodeAttribute* parseCode(u1* info);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que, dado uma referencia para um vetor de informacoes de um atributo do tipo constant 
 * value, preenche e retorna uma estrutura ConstantValueAtribute.
 *
 * \param info Referencia para o atributo constant value
 * \return Estrutura ConstantValueAttribute preenchida
 */
EXTC ConstantValueAttribute* parseConstantValue(u1* info);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que, dado uma referencia para um vetor de informacoes de um atributo do tipo exception,
 * preenche e retorna uma estrutura ExceptionAttribute.
 *
 * \param info Referencia para o atributo exception
 * \return Estrutura ExceptionAttribute preenchida
 */
EXTC ExceptionAttribute* parseException(u1* info);


// Interface do Carregador de Classes da JVM
// Interfaces que realizam o carregamento das classes na JVM

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que, dado um nome qualificado de classe, retorna uma estrutura JavaClass preenchida
 *
 * \param qualifiedName Nome qualificado da classe a ser carregada
  * \param environment Ambiente de execucao atual
 * \return Estrutura arqClass preenchida com os dados da classe carregada 
 */
EXTC JavaClass* loadCLass(const char* qualifiedName, Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Método que aloca todos os espaços de memoria necessarios para os campos de uma classe ou objeto e inicializa os inicializa com os valores default.
 *
 * \param javaClass Referencia para a estrutura de classe com os parametros
 * \param flagsAccept Flags para o tipos de campos aceitos (Ex.: STATIC, FINAL)
 * \param flagsRegect Flags para o tipos de campos rejeitados (Ex.: STATIC, FINAL)
 * \return Estrutura de campos preenchida
 */
EXTC Fields* classInitializeFields(JavaClass* javaClass, u2 flagsAccept, u2 flagsRegect);
#endif