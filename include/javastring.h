//#################################################################################################
/*! \file javastring.h
 *
 *  \brief Biblioteca de strings Java
 *
 *  Biblioteca responsavel por prover as definicoes e metodos relacionados as strings constantes
 * da JVM
 */
//##################################################################################################

#ifndef JAVASTRING_h
#define JAVASTRING_h

#ifdef JAVASTRING
#define EXTJS
#else
#define EXTJS extern
#endif

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "estruturas.h"


//--------------------------------------------------------------------------------------------------
//Para referencia de string
#define CONSTANT_StringJava          20


//--------------------------------------------------------------------------------------------------
//! Estrutura de Constante String Java
/*!
 * Estrutura utilizada para a representacao de uma String Java (constante)
 */
typedef struct StringRef{
    u1 tag;
    u2 size;
    wchar_t* stringAddress;
    
} JavaString;

    

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por inicializar e retornar uma estrutura de string java preenchida a partir
 * de um vetor de char
 *
 * \param string String c a ser carregada na estrutura java string
 * \return estrutura JavaString preenchida
 */
EXTJS JavaString* newJavaString(const wchar_t* string);


#endif /* JAVASTRING_h */
