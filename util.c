//#################################################################################################
/*! \file util.c
 *
 *  \brief Modulo com funcoes de utilidade para a JVM operar
 *
 *  Modulo responsavel por implementar os servicos uteis para a JVM, 
 *  com submodulos responsaveis por:
 *  - Decoder: Decodificacao de metodos, campos e estuturas que simulam o acesso a bibliotecas Java.
 *  - Conversion: Servicos relacionados a conversao de tipos.
 *  - java.io: Servicos que simulam os metodos e atributos da biblioteca java.io
 *  - java.lang: Servicos que simulam os metodos e atributos da biblioteca java.lang
 */
//##################################################################################################


#define UTIL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "include/util.h"
#include "include/opcodes.h"
#include "include/memoryunit.h"
#include "include/javastring.h"
#include "include/classloader.h"

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Decoder
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
int javaLibIsFrom(const char* name){
    if (strncmp(name, "java/", 5) == 0) return 1;
    if (strncmp(name, "Ljava/", 6) == 0) return 1;
    
    return 0;
}

//--------------------------------------------------------------------------------------------------
void javaLibExecuteMethod(const char* className, const char* methodName, const char* descriptorName,
                          Environment* environment){

    //Metodos de java/lang
    if (strncmp(className, "java/lang/", 10)==0) {
        //TODO: DECODE JAVA:LANG
        javaLangExecuteMethod(&className[10], methodName, descriptorName, environment);
    }
    //Metodos de java/io
    else if (strncmp(className, "java/io/", 8)==0) {
        //TODO: DECODE JAVA:IO
        javaIOExecuteMethod(&className[8], methodName, descriptorName, environment);
    }
    else
        popFromOperandStack(environment->thread);
}


//--------------------------------------------------------------------------------------------------
void javaLibGetStatic(const char* className, const char* fieldName,
                          const char* fieldDescriptor, Environment* environment){
    
    //Campos de java/lang
    if (strncmp(className, "java/lang/", 10)==0) {
        //TODO: CAMPOS JAVA:LANG
        javaLangGetStatic(&className[10], fieldName, fieldDescriptor, environment);
    }
    //Campos de java/io
    else if (strncmp(className, "java/io/", 8)==0) {
        //TODO: CAMPOS JAVA:IO
        javaIOGetStatic(&className[8], fieldName, fieldDescriptor, environment);
    }
    else
    pushInOperandStack(environment->thread, 0);
}



//--------------------------------------------------------------------------------------------------
void javaLibNewObject(const char* className, Environment* environment){
    //Campos de java/lang
    if (strncmp(className, "java/lang/", 10)==0) {
        //TODO: OBJETO JAVA:LANG
        javaLangNewObject(&className[10], environment);
    }
    //Campos de java/io
    else if (strncmp(className, "java/io/", 8)==0) {
        //TODO: OBJETO JAVA:IO
        javaIONewObject(&className[8], environment);
    }
    else
        pushInOperandStack(environment->thread, 0);
}
//--------------------------------------------------------------------------------------------------
// SUBMODULO: Conversion
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
double u4ToDouble(u4 high_bytes, u4 low_bytes){
    double resultado;
    unsigned char b[8]; //vetor de 8 Bytes
    
    //Trocamos a ordem dos bytes salvando no vetor de bytes
    for (int i = 0; i < 4; i++) b[i] = low_bytes >> i * 8;
    for (int i = 4; i < 8; i++) b[i] = high_bytes >> i * 8;
    
    
    //Copiamos os bytes do vetor de bytes para o float
    memcpy(&resultado, &b, sizeof(resultado));
    
    return resultado;
}


//--------------------------------------------------------------------------------------------------
float u4ToFLoat(u4 bytes){
    
    float resultado;
    unsigned char b[4]; //vetor de 4 Bytes
    
    //Trocamos a ordem dos bytes salvando no vetor de bytes
    for (int i = 0; i < 4; i++) b[i] = bytes >> i * 8;
    
    //Copiamos os bytes do vetor de bytes para o float
    memcpy(&resultado, &b, sizeof(resultado));
    
    return resultado;
}


//--------------------------------------------------------------------------------------------------
u2 concat2Bytes(u1 high_bytes, u1 low_bytes){

    u2 result = high_bytes;
    
    return (result << 8) | low_bytes;

}

//--------------------------------------------------------------------------------------------------
// SUBMODULO: java.io
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que decodifica e simula a execucao do metodo de java.io.java.io.PrintStream.println
 *
 * \param descriptor Descritor do metodo a ser executado.
 * \param environment Ambiente de execucao atual.
 */
void printStreamPrintln(const char* descriptorName, Environment* environment){
    //Strings
    if (strncmp(descriptorName, "(Ljava/lang/String;)V", 21)==0) {
        
        JavaString* string = (JavaString*) popFromOperandStack(environment->thread);
        
        //Se for JavaString
        if(string->tag == CONSTANT_StringJava){
            printf("\n%ls", string->stringAddress);
        }
        //Se for referencia de string para o pool de constantes
        else{
            cp_info* stringCP = (cp_info*) string;
            
            wchar_t* unicodeString = getUnicodeFromConstantPool(environment->thread->vmStack->top->javaClass->arqClass->constant_pool, stringCP->u.String.string_index);
            
            printf("\n%ls", unicodeString);
        }
    }
    //Decodificamos pelos tipos numericos, char e bool
    else if (strncmp(descriptorName, "(I)V", 4)==0 ||
             strncmp(descriptorName, "(S)V", 4)==0 ||
             strncmp(descriptorName, "(B)V", 4)==0)
        printf("\n%d", (int) popFromOperandStack(environment->thread));
    
    else if (strncmp(descriptorName, "(C)V", 4)==0)
        printf("\n%c", (char) popFromOperandStack(environment->thread));
    
    else if (strncmp(descriptorName, "(F)V", 4)==0)
        printf("\n%.2f",  u4ToFLoat(popFromOperandStack(environment->thread)));
    
    else if (strncmp(descriptorName, "(J)V", 4)==0){
        u8 bytesl = popFromOperandStack(environment->thread);
        u8 low_Bytes = popFromOperandStack(environment->thread);
        bytesl = bytesl << 32 | low_Bytes;
        printf("\n%lld", bytesl);
    }
    
    else if (strncmp(descriptorName, "(D)V", 4)==0){
        u4 high = popFromOperandStack(environment->thread);
        u4 low = popFromOperandStack(environment->thread);
        printf("\n%.2lf", u4ToDouble(high, low));
    }
    
    else if (strncmp(descriptorName, "(Z)V", 4)==0)
        printf("\n%s", ((int) popFromOperandStack(environment->thread)) == 0 ? "false" : "true");
    
    else
        popFromOperandStack(environment->thread);
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que decodifica e simula os metodos de java.io.PrintStream
 *
 * \param descriptor Descritor do metodo a ser executado.
 * \param environment Ambiente de execucao atual.
 */
void printStreamExecuteMethod(const char* className, const char* methodName, const char* descriptorName,
                         Environment* environment){
    
    //Metodos de java.io.PrintStream.prinln
    if (strncmp(methodName, "println", 7)==0) {
        printStreamPrintln(descriptorName, environment);
        popFromOperandStack(environment->thread); //PrintStream nao utilizado
    }
    else
        popFromOperandStack(environment->thread);
}


//--------------------------------------------------------------------------------------------------
void javaIOExecuteMethod(const char* className, const char* methodName, const char* descriptorName,
                           Environment* environment){
    
    //Metodos de java.io.PrintStream
    if (strncmp(className, "PrintStream", 11)==0) {
        printStreamExecuteMethod(&className[11],methodName,descriptorName,environment);
    }
    else
        popFromOperandStack(environment->thread);
}


//--------------------------------------------------------------------------------------------------
void javaIOGetStatic(const char* className, const char* fieldName,
                       const char* fieldDescriptor, Environment* environment){
    
    pushInOperandStack(environment->thread, 0);
}


//--------------------------------------------------------------------------------------------------
void javaIONewObject(const char* className, Environment* environment){
    pushInOperandStack(environment->thread, 0);
}


//--------------------------------------------------------------------------------------------------
// SUBMODULO: java.lang
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que decodifica e simula a execucao de um metodo de java.io.System
 *
 * \param className Nome da classe do metodo
 * \param methodName Nome do metodo a ser executado.
 * \param descriptor Descritor do metodo a ser executado.
 * \param environment Ambiente de execucao atual.
 */
void systemExecuteMethod(const char* className, const char* methodName,
                         const char* descriptorName, Environment* environment){
        popFromOperandStack(environment->thread);
}



//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que decodifica e simula a execucao do metodo de java.lang.StringBuffer.append
 *
 * \param descriptor Descritor do metodo a ser executado.
 * \param environment Ambiente de execucao atual.
 */
void stringBufferAppend(const char* descriptorName, Environment* environment){
    //Se a entrada for uma string
    if (strncmp(descriptorName, "(Ljava/lang/String;)Ljava/lang/StringBuffer;", 44)==0 ||
        strncmp(descriptorName, "(Ljava/lang/Object;)Ljava/lang/StringBuffer;", 44)==0) {
        
        JavaString* string = (JavaString*) popFromOperandStack(environment->thread);
        wchar_t* stringBuffer = (wchar_t*) popFromOperandStack(environment->thread);
        
        //Se for JavaString
        if(string->tag == CONSTANT_StringJava){
            //Realocacao do buffer
            stringBuffer = (wchar_t*) realloc(stringBuffer,
                                              (wcslen(stringBuffer)+string->size+1)*sizeof(wchar_t));
            //Concatenamos
            wcsncat(stringBuffer, string->stringAddress, string->size);
        }
        //Se for referencia de string para o pool de constantes
        else{
            cp_info* stringCP = (cp_info*) string;
            
            wchar_t* unicodeString = getUnicodeFromConstantPool(environment->thread->vmStack->top->javaClass->arqClass->constant_pool, stringCP->u.String.string_index);
            
            int stringBuferLenght = wcslen(stringBuffer);
            //Realocacao do buffer
            stringBuffer = (wchar_t*) realloc(stringBuffer,
                                              (stringBuferLenght+wcslen(unicodeString)+1)*sizeof(wchar_t));
            //Concatenamos
            wcscat(stringBuffer, unicodeString);
        }
        //Empilhamos o string buffer
        pushInOperandStack(environment->thread, (u4) stringBuffer);
    }
    
    //Para cada tipo numerico, bool ou char
    else if(strncmp(descriptorName, "(I)Ljava/lang/StringBuffer;", 44)==0 ||
            strncmp(descriptorName, "(S)Ljava/lang/StringBuffer;", 44)==0 ||
            strncmp(descriptorName, "(B)Ljava/lang/StringBuffer;", 44)==0){
        int valor = (int) popFromOperandStack(environment->thread);
        wchar_t* stringBuffer = (wchar_t*) popFromOperandStack(environment->thread);
        
//        int stringBufferLen = wcslen(stringBuffer);
        
        stringBuffer = (wchar_t*) realloc(stringBuffer,
                                          (wcslen(stringBuffer)+15)*sizeof(wchar_t));
        
        //Procedimento para substiuicao de swprintf
        char buffer[15] = "";
        sprintf(buffer, "%d", valor);
        wchar_t* wbuffer = (wchar_t*) malloc((sizeof(buffer)+1)*sizeof(wchar_t));
        mbstowcs(wbuffer, buffer, (strlen(buffer))+1);
        wcscat(stringBuffer, wbuffer);
        pushInOperandStack(environment->thread, (u4) stringBuffer);
    }
    
    else if(strncmp(descriptorName, "(C)Ljava/lang/StringBuffer;", 44)==0){
        char valor = (char) popFromOperandStack(environment->thread);
        wchar_t* stringBuffer = (wchar_t*) popFromOperandStack(environment->thread);
        
        stringBuffer = (wchar_t*) realloc(stringBuffer,
                                          (wcslen(stringBuffer)+2)*sizeof(wchar_t));
        
        //Procedimento para substiuicao de swprintf
        char buffer[3] = "";
        sprintf(buffer, "%c", valor);
        wchar_t* wbuffer = (wchar_t*) malloc((sizeof(buffer)+1)*sizeof(wchar_t));
        mbstowcs(wbuffer, buffer, (strlen(buffer))+1);
        wcscat(stringBuffer, wbuffer);
        pushInOperandStack(environment->thread, (u4) stringBuffer);
    }
    
    else if(strncmp(descriptorName, "(Z)Ljava/lang/StringBuffer;", 44)==0){
        int valor = (char) popFromOperandStack(environment->thread);
        wchar_t* stringBuffer = (wchar_t*) popFromOperandStack(environment->thread);
        
        stringBuffer = (wchar_t*) realloc(stringBuffer,
                                          (wcslen(stringBuffer)+6)*sizeof(wchar_t));
        
        //Procedimento para substiuicao de swprintf
        char buffer[6] = "";
        sprintf(buffer, "%s", valor == 0 ? "false" : "true");
        wchar_t* wbuffer = (wchar_t*) malloc((sizeof(buffer)+1)*sizeof(wchar_t));
        mbstowcs(wbuffer, buffer, (strlen(buffer))+1);
        wcscat(stringBuffer, wbuffer);
        pushInOperandStack(environment->thread, (u4) stringBuffer);
    }

    else if(strncmp(descriptorName, "(F)Ljava/lang/StringBuffer;", 44)==0){
        float valor = u4ToFLoat(popFromOperandStack(environment->thread));
        wchar_t* stringBuffer = (wchar_t*) popFromOperandStack(environment->thread);
        
        stringBuffer = (wchar_t*) realloc(stringBuffer,
                                          (wcslen(stringBuffer)+20)*sizeof(wchar_t));
        
        //Procedimento para substiuicao de swprintf
        char buffer[30] = "";
        sprintf(buffer, "%.2f", valor);
        wchar_t* wbuffer = (wchar_t*) malloc((sizeof(buffer)+1)*sizeof(wchar_t));
        mbstowcs(wbuffer, buffer, (strlen(buffer))+1);
        wcscat(stringBuffer, wbuffer);
        pushInOperandStack(environment->thread, (u4) stringBuffer);
    }
    
    else if(strncmp(descriptorName, "(J)Ljava/lang/StringBuffer;", 44)==0){
        u8 valor = popFromOperandStack(environment->thread);
        u8 low_Bytes = popFromOperandStack(environment->thread);
        valor = valor << 32 | low_Bytes;
        wchar_t* stringBuffer = (wchar_t*) popFromOperandStack(environment->thread);
        
        stringBuffer = (wchar_t*) realloc(stringBuffer,
                                          (wcslen(stringBuffer)+30)*sizeof(wchar_t));
        
        //Procedimento para substiuicao de swprintf
        char buffer[30] = "";
        sprintf(buffer, "%lld", valor);
        wchar_t* wbuffer = (wchar_t*) malloc((sizeof(buffer)+1)*sizeof(wchar_t));
        mbstowcs(wbuffer, buffer, (strlen(buffer))+1);
        wcscat(stringBuffer, wbuffer);
        pushInOperandStack(environment->thread, (u4) stringBuffer);
    }
    
    else if(strncmp(descriptorName, "(D)Ljava/lang/StringBuffer;", 44)==0){
        u4 high = popFromOperandStack(environment->thread);
        u4 low = popFromOperandStack(environment->thread);
        double valor = u4ToDouble(high, low);
        
        wchar_t* stringBuffer = (wchar_t*) popFromOperandStack(environment->thread);
        
        
        stringBuffer = (wchar_t*) realloc(stringBuffer,
                                          (wcslen(stringBuffer)+15)*sizeof(wchar_t));
        
        //Procedimento para substiuicao de swprintf
        char buffer[30] = "";
        sprintf(buffer, "%.2f", valor);
        wchar_t* wbuffer = (wchar_t*) malloc((sizeof(buffer)+1)*sizeof(wchar_t));
        mbstowcs(wbuffer, buffer, (strlen(buffer))+1);
        wcscat(stringBuffer, wbuffer);
        pushInOperandStack(environment->thread, (u4) stringBuffer);
    }
}

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que decodifica e simula a execucao de um metodo de java.lang.StringBuffer
 *
 * \param className Nome da classe do metodo
 * \param methodName Nome do metodo a ser executado.
 * \param descriptor Descritor do metodo a ser executado.
 * \param environment Ambiente de execucao atual.
 */
void stringBufferExecuteMethod(const char* className, const char* methodName,
                         const char* descriptorName, Environment* environment){

    if (strncmp(methodName, "<init>", 6)==0) {
        popFromOperandStack(environment->thread);
    }

    else if (strncmp(methodName, "toString", 6)==0){
        wchar_t* stringBuffer = (wchar_t*)(popFromOperandStack(environment->thread));
        JavaString* string = newJavaString(stringBuffer);
        pushInOperandStack(environment->thread, (u4) string);
    }

    else if (strncmp(methodName, "append", 6)==0) {
        stringBufferAppend(descriptorName,environment);
    }
    
    else
        popFromOperandStack(environment->thread);

}


//--------------------------------------------------------------------------------------------------
void javaLangExecuteMethod(const char* className, const char* methodName, const char* descriptorName,
                          Environment* environment){
    
    //Metodos de java/lang/System
    if (strncmp(className, "System", 6)==0) {
        systemExecuteMethod(&className[6], methodName, descriptorName, environment);
    }
    //Metodos de java/lang/StringBuffer
    else if (strncmp(className, "StringBuffer", 12)==0) {
        stringBufferExecuteMethod(&className[12], methodName, descriptorName, environment);
    }
    //Metodos de java/lang/Object
    else if ((strncmp(className, "Object", 6)==0)){
        if(strncmp(methodName, "getClass", 8)==0)
            return;
        else
            popFromOperandStack(environment->thread);
    }
    else
        popFromOperandStack(environment->thread);
}


//--------------------------------------------------------------------------------------------------
void javaLangGetStatic(const char* className, const char* fieldName,
                      const char* fieldDescriptor, Environment* environment){
    
    //Metodos de java/lang/System
    if (strncmp(className, "System", 6)==0) {
        pushInOperandStack(environment->thread, 0);
    }
    //Metodos de java/lang/StringBuffer
    else if (strncmp(className, "StringBuffer", 12)==0) {
        pushInOperandStack(environment->thread, 0);
    }
    else
        pushInOperandStack(environment->thread, 0);
}


//--------------------------------------------------------------------------------------------------
void javaLangNewObject(const char* className, Environment* environment){
    //Metodos de java/lang/System
    if (strncmp(className, "System", 6)==0) {
        pushInOperandStack(environment->thread, 0);
    }
    //Metodos de java/lang/StringBuffer
    else if (strncmp(className, "StringBuffer", 12)==0) {
        wchar_t* stringBuffer = (wchar_t*)malloc(sizeof(wchar_t));
        wcscpy(stringBuffer, L"");
        pushInOperandStack(environment->thread, (u4) stringBuffer);
    }
    else
        pushInOperandStack(environment->thread, 0);
}