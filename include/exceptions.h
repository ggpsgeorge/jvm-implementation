//#################################################################################################
/*! \file exceptions.h
 *
 *  \brief Lista de Excecoes Trataveis da JVM.
 *
 *  Arquivo com a definicao das excecoes trataveis pela jvm.
 */
//##################################################################################################


//--------------------------------------------------------------------------------------------------
// Definicao de Excessoes e Debugs
//--------------------------------------------------------------------------------------------------

#define NullPointerException            40 //!< Erro de referencia nula
#define AbstractMethodError             41 //!< Erro de chamada de metodo abstrato
#define IncompatibleClassChangeError    42 //!< Erro de chamada de metodo incompativel
#define IllegalAccessError              43 //!< Erro de acesso
#define ArrayIndexOutOfBoundsException  44 //!< Erro acesso index array
#define NegativeArraySizeException      45 //!< Erro de tamanho de array negativo

#define DEBUG_ShowClassFiles            0b001 //!< Ativar exibidor.class
#define DEBUG_DebugModus                0b010 //!< Imprimir frames por instrução