//#################################################################################################
/*! \file util.h
 *
 *  \brief Interface com funcoes de utilidade para a JVM operar
 *
 *  Interface responsavel por implementar os servicos uteis para a JVM, 
 *  com submodulos responsaveis por:
 *  - Decoder: Decodificacao de metodos, campos e estuturas que simulam o acesso a bibliotecas Java.
 *  - Conversion: Servicos relacionados a conversao de tipos.
 *  - java.io: Servicos que simulam os metodos e atributos da biblioteca java.io
 *  - java.lang: Servicos que simulam os metodos e atributos da biblioteca java.lang
 */
//##################################################################################################

#ifndef UTIL_h
#define UTIL_h
#ifdef UTIL
#define EXTU
#else
#define EXTU extern
#endif

#include "estruturas.h"


//--------------------------------------------------------------------------------------------------
// SUBMODULO: Decoder
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que verifica se a classe, instancia ou campo pertencem a java/<pacote>
 *
 * \param name Nome a ser verificado.
 * \return 1 caso sim, 0 caso nao.
 */
EXTU int javaLibIsFrom(const char* name);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que decodifica e simula a execucao de um metodo de uma biblioteca java
 *
 * \param className Nome da classe do metodo
 * \param methodName Nome do metodo a ser executado.
 * \param descriptor Descritor do metodo a ser executado.
 * \param environment Ambiente de execucao atual.
 */
EXTU void javaLibExecuteMethod(const char* className, const char* methodName,
                                const char* descriptorName, Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que obtem o valor de um campo estatico de uma biblioteca java e empilha na pilha de 
 * operandos.
 *
 * \param className Nome da classe do campo
 * \param fieldName Nome do campo a ser obtido.
 * \param fieldDescriptor Descritor do campo a ser obtido.
 * \param environment Ambiente de execucao atual.
 */
EXTU void javaLibGetStatic(const char* className, const char* fieldName,
                                const char* fieldDescriptor, Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que cria um objeto de uma biblioteca java e empilha uma referencia na pilha de operandos.
 *
 * \param className Nome da classe do objeto a ser criado.
 * \param environment Ambiente de execucao atual.
 */
EXTU void javaLibNewObject(const char* className, Environment* environment);


//--------------------------------------------------------------------------------------------------
// SUBMODULO: Conversion
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a conversao de um u4 (little-endian) para um double em big-endian order.
 *
 * \param high_bytes    Bytes de mais alta ordem.
 * \param low_bytes     Bytes de mais baixa ordem.
 */
EXTU double u4ToDouble(u4 high_bytes, u4 low_bytes);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a conversao de um u4 (little-endian) para um float em big-endian order.
 *
 * \param bytes    Bytes em u4.
 */
EXTU float u4ToFLoat(u4 bytes);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que concatena 2bytes em um u2
 *
 * \param high_bytes    Bytes mais siginificativos.
 * \param low_bytes    Bytes menos siginificativos.
 * \return high_bytes << 8 | low_bytes
 */
EXTU u2 concat2Bytes(u1 high_bytes, u1 low_bytes);

//--------------------------------------------------------------------------------------------------
// SUBMODULO: java.io
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que decodifica e simula a execucao de um metodo da biblioteca java.io
 *
 * \param className Nome da classe do metodo
 * \param methodName Nome do metodo a ser executado.
 * \param descriptor Descritor do metodo a ser executado.
 * \param environment Ambiente de execucao atual.
 */
EXTU void javaIOExecuteMethod(const char* className, const char* methodName,
                               const char* descriptorName, Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que obtem o valor de um campo estatico da biblioteca java.io e empilha na pilha de
 * operandos.
 *
 * \param className Nome da classe do campo
 * \param fieldName Nome do campo a ser obtido.
 * \param fieldDescriptor Descritor do campo a ser obtido.
 * \param environment Ambiente de execucao atual.
 */
EXTU void javaIOGetStatic(const char* className, const char* fieldName,
                           const char* fieldDescriptor, Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que cria um objeto da biblioteca java.io e empilha uma referencia na pilha de operandos.
 *
 * \param className Nome da classe do objeto a ser criado.
 * \param environment Ambiente de execucao atual.
 */
EXTU void javaIONewObject(const char* className, Environment* environment);

//--------------------------------------------------------------------------------------------------
// SUBMODULO: java.lang
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que decodifica e simula a execucao de um metodo da biblioteca java.lang
 *
 * \param className Nome da classe do metodo
 * \param methodName Nome do metodo a ser executado.
 * \param descriptor Descritor do metodo a ser executado.
 * \param environment Ambiente de execucao atual.
 */
EXTU void javaLangExecuteMethod(const char* className, const char* methodName,
                                 const char* descriptorName, Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que obtem o valor de um campo estatico da biblioteca java.lang e empilha na pilha de
 * operandos.
 *
 * \param className Nome da classe do campo
 * \param fieldName Nome do campo a ser obtido.
 * \param fieldDescriptor Descritor do campo a ser obtido.
 * \param environment Ambiente de execucao atual.
 */
EXTU void javaLangGetStatic(const char* className, const char* fieldName,
                             const char* fieldDescriptor, Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que cria um objeto da biblioteca java.lang e empilha uma referencia na pilha de operandos.
 *
 * \param className Nome da classe do objeto a ser criado.
 * \param environment Ambiente de execucao atual.
 */
EXTU void javaLangNewObject(const char* className, Environment* environment);

#endif