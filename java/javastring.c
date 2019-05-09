//#################################################################################################
/*! \file javastring.c
 *
 *  \brief Biblioteca de strings Java
 *
 *  Biblioteca responsavel por implementar os metodos relacionados as strings constantes da JVM
 */
//##################################################################################################

#define JAVASTRING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/javastring.h"


//--------------------------------------------------------------------------------------------------
JavaString* newJavaString(const wchar_t* string){

    JavaString* string_info = (JavaString*) malloc(sizeof(JavaString));
    string_info->tag = CONSTANT_StringJava;
    string_info->size =  wcslen(string);
    string_info->stringAddress = (wchar_t*) string;
    
    return string_info;
}