//#################################################################################################
/*! \file javaarray.h
 *
 *  \brief Biblioteca de arrays Java
 *
 *  Biblioteca responsavel por prover as definicoes e metodos relacionados aos arrays da JVM
 */
//##################################################################################################

#ifndef JAVAARRAY_h
#define JAVAARRAY_h

#ifdef JAVAARRAY
#define EXTJA
#else
#define EXTJA extern
#endif

#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"



//--------------------------------------------------------------------------------------------------
//Tags do Pool de Constantes

#define T_BOOLEAN   4
#define T_CHAR      5
#define T_FLOAT     6
#define T_DOUBLE	7
#define T_BYTE      8
#define T_SHORT     9
#define T_INT       10
#define T_LONG      11



//--------------------------------------------------------------------------------------------------
//! Estrutura de Array Java
/*!
 * Estrutura utilizada para a representacao de um Array Java
 */
typedef struct ArrayRef{
    u4 atype;
    u2 count;
    void* arrayAddress;
} JavaArray;



//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por inicializar e retornar uma estrutura de array java preenchida a partir
 * de um vetor de char
 *
 * \param value Value de tipo int, long, float, double, reference, byte, boolean, char ou short a
 * ser carregado na estrutura java array
 * \return estrutura JavaArray preenchida
 */
EXTJA JavaArray* newJavaArray(u4 n_type, u2 n_length, void* n_arrayAddress);


#endif /* JAVAARRAY_h */
