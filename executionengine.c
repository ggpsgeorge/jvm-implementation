//#################################################################################################
/*! \file executionengine.c
 *
 *  \brief Modulo de Inicializacao da JVM.
 *
 *  Modulo responsavel por implementar os metodos relacionados a implementar o ExecutionEngine,
 *  com submodulos responsaveis por:
 *  - Interpreter: Implementa o Interpretador da JVM
 *  - Bootloader: Inicializacao de estruturas e execucao do sistema.
 *  - Decoder: Decodificacao de bytecodes em instrucoes
 *  - Tratamento de excessoes lancadas pela JVM
 *  - Instrucoes:
 *  -- Comparacao e desvio
 *  -- Conversao de tipos
 *  -- Transferencia valores entre o vetor de variaveis locais e a pilha de operandos de um frame de um metodo
 *  -- Instrucoes logico atmeticas
 *  -- Manipulacao de objetos e chamadas de metodos
 *  -- Retorno de funcoes
 */
//##################################################################################################

#define EXECENG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "include/estruturas.h"
#include "include/javastring.h"
#include "include/javaarray.h"
#include "include/opcodes.h"
#include "include/executionengine.h"
#include "include/util.h"
#include "include/memoryunit.h"
#include "include/classloader.h"

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Interpretador.
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void execute(Environment* environment){

    //! Enquanto a pilha de frames nao estiver vazia:
    while (environment->thread->vmStack != NULL) {

        //! 1.Obtem o opcode.
        u1 opcode = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                          environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                          ,environment->thread->PC);
        
        //! 2.Decodifica o opcode
        instruction nextInstruction = decode(opcode);
        
        //! 2.1 Imprime informações de frame em modo de debug
        if(environment->debugFlags & DEBUG_DebugModus){
            JVMPrintFrameInfo(environment->thread->vmStack->top, opcode);
            printf("\n>Pressione Enter para continuar...");
            getchar();
        }
        
        //! 3.Executa o a instrucao referente ao opcode
        if(nextInstruction) nextInstruction(environment);
        
        //! 4.Atualizamos o valor de PC
        environment->thread->PC++;
    }
    
}

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Bootloader - Inicializacao de estruturas e execucao do sistema.
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que configura e empilha um novo frame para o metodo main da classe chamada pela JVM
 *
 * \param environment Ambiente de execucao atual
 * \param argc Numero de argumentos do programa
 * \param argv Vetor de vetores de char passados pelo usuario
 */
void configureClassMain(Environment* environment, int argc, const char* argv[]){
    
    //Empilhamos o metodo main a ser inicializado
    Frame* newFrame = pushFrame(environment, argv[1], "main", "([Ljava/lang/String;)V");
    
    //Criamos o conteudo do array e o JavaArray a receber o conteudo
    void* arrayAddress;
    u4* stringArray = (u4*) malloc((argc-2)*sizeof(u4));
    JavaArray* array = newJavaArray(T_INT, argc-2, arrayAddress);
    array->arrayAddress = stringArray;

    //Preenchemos o array de strings com cada argumento
    for (int i = 2; i < argc; i++) {
        wchar_t* string = (wchar_t*)malloc((strlen(argv[i])+1)*sizeof(wchar_t));
        mbstowcs(string, argv[i], (strlen(argv[i]))+1);
        JavaString* string_info = newJavaString(string);
        stringArray[i-2] = (u4) string_info;
    }
    //Passamos o argumento argv da main java
    newFrame->localVariablesVector[0] = (u4) array;
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo inicial do sistema
 *
 */
int main(int argc, const char * argv[]) {

    char opcoes;
    u1 debugFlags = 0;
    
    //Configuracoes de debug
    printf("Deseja ativar exibidor de .class?[N/s]:");
    scanf("%c", &opcoes);
    getchar();
    if (opcoes == 'S' || opcoes == 's') {
        debugFlags |= DEBUG_ShowClassFiles;
    }
    printf("Modo debug?[N/s]:");
    scanf("%c", &opcoes);
    getchar();
    if (opcoes == 'S' || opcoes == 's') {
        debugFlags |= DEBUG_DebugModus;
    }
    
    //Alocamos espaco para o ambiente de execucao
    Environment* environment = (Environment*) malloc(sizeof(Environment));
    
    //Criamos a area de metodos e a thread e as associamos ao enviroment
    environment->methodArea = newMethodArea();
    environment->thread = newThread();
    environment->debugFlags = debugFlags;
    
    //Empilhamos o metodo main
    configureClassMain(environment, argc, argv);
    
    //Passamos o ambiente de execucao para o interpretador
    execute(environment);

    printf("\n\n");
    return 0;
}


//--------------------------------------------------------------------------------------------------
// SUBMODULO: Decoder: Decodificacao de bytecodes em instrucoes
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
instruction decode(u1 bytecode){

    switch (bytecode) {
        case OP_iadd:
            return iadd;
            break;
        case OP_ladd:
            return ladd;
            break;
        case OP_fadd:
            return fadd;
            break;
        case OP_dadd:
            return dadd;
            break;
        case OP_isub:
            return isub;
            break;
        case OP_lsub:
            return lsub;
            break;
        case OP_fsub:
            return fsub;
            break;
        case OP_dsub:
            return dsub;
            break;
        case OP_imul:
            return imul;
            break;
        case OP_lmul:
            return lmul;
            break;
        case OP_fmul:
            return fmul;
            break;
        case OP_dmul:
            return dmul;
            break;
        case OP_idiv:
            return idiv;
            break;
        case OP_ldiv:
            return Ldiv;
            break;
        case OP_fdiv:
            return fdiv;
            break;
        case OP_ddiv:
            return ddiv;
            break;
        case OP_irem:
            return irem;
            break;
        case OP_lrem:
            return lrem;
            break;
        case OP_frem:
            return frem;
            break;
        case OP_drem:
            return Drem;
            break;
        case OP_ineg:
            return ineg;
            break;
        case OP_lneg:
            return lneg;
            break;
        case OP_fneg:
            return fneg;
            break;
        case OP_dneg:
            return dneg;
            break;
        case OP_ishl:
            return ishl;
            break;
        case OP_lshl:
            return lshl;
            break;
        case OP_ishr:
            return ishr;
            break;
        case OP_lshr:
            return lshr;
            break;
        case OP_iushr:
            return iushr;
            break;
        case OP_lushr:
            return lushr;
            break;
        case OP_iand:
            return iand;
            break;
        case OP_land:
            return land;
            break;
        case OP_ior:
            return ior;
            break;
        case OP_lor:
            return lor;
            break;
        case OP_ixor:
            return ixor;
            break;
        case OP_lxor:
            return lxor;
            break;
        case OP_iinc:
            return iinc;
            break;
        case OP_nop:
            return nop;
            break;
        case OP_aconst_null:
            return aconst_null;
            break;
        case OP_iconst_m1:
            return iconst_m1;
            break;
        case OP_iconst_0:
            return iconst_0;
            break;
        case OP_iconst_1:
            return iconst_1;
            break;
        case OP_iconst_2:
            return iconst_2;
            break;
        case OP_iconst_3:
            return iconst_3;
            break;
        case OP_iconst_4:
            return iconst_4;
            break;
        case OP_iconst_5:
            return iconst_5;
            break;
        case OP_lconst_0:
            return lconst_0;
            break;
        case OP_lconst_1:
            return lconst_1;
            break;
        case OP_fconst_0:
            return fconst_0;
            break;
        case OP_fconst_1:
            return fconst_1;
            break;
        case OP_fconst_2:
            return fconst_2;
            break;
        case OP_dconst_0:
            return dconst_0;
            break;
        case OP_dconst_1:
            return dconst_1;
            break;
        case OP_bipush:
            return bipush;
            break;
        case OP_sipush:
            return sipush;
            break;
        case OP_ldc:
            return ldc;
            break;
        case OP_ldc_w:
            return ldc_w;
            break;
        case OP_ldc2_w:
            return ldc2_w;
            break;
        case OP_iload:
            return iload;
            break;
        case OP_lload:
            return lload;
            break;
        case OP_fload:
            return fload;
            break;
        case OP_dload:
            return dload;
            break;
        case OP_aload:
            return aload;
            break;
        case OP_iload_0:
            return iload_0;
            break;
        case OP_iload_1:
            return iload_1;
            break;
        case OP_iload_2:
            return iload_2;
            break;
        case OP_iload_3:
            return iload_3;
            break;
        case OP_lload_0:
            return lload_0;
            break;
        case OP_lload_1:
            return lload_1;
            break;
        case OP_lload_2:
            return lload_2;
            break;
        case OP_lload_3:
            return lload_3;
            break;
        case OP_fload_0:
            return fload_0;
            break;
        case OP_fload_1:
            return fload_1;
            break;
        case OP_fload_2:
            return fload_2;
            break;
        case OP_fload_3:
            return fload_3;
            break;
        case OP_dload_0:
            return dload_0;
            break;
        case OP_dload_1:
            return dload_1;
            break;
        case OP_dload_2:
            return dload_2;
            break;
        case OP_dload_3:
            return dload_3;
            break;
        case OP_aload_0:
            return aload_0;
            break;
        case OP_aload_1:
            return aload_1;
            break;
        case OP_aload_2:
            return aload_2;
            break;
        case OP_aload_3:
            return aload_3;
            break;
        case OP_iaload:
            return iaload;
            break;
        case OP_laload:
            return laload;
            break;
        case OP_faload:
            return faload;
            break;
        case OP_daload:
            return daload;
            break;
        case OP_aaload:
            return aaload;
            break;
        case OP_baload:
            return baload;
            break;
        case OP_caload:
            return caload;
            break;
        case OP_saload:
            return saload;
            break;
        case OP_istore:
            return istore;
            break;
        case OP_lstore:
            return lstore;
            break;
        case OP_fstore:
            return fstore;
            break;
        case OP_dstore:
            return dstore;
            break;
        case OP_astore:
            return astore;
            break;
        case OP_istore_0:
            return istore_0;
            break;
        case OP_istore_1:
            return istore_1;
            break;
        case OP_istore_2:
            return istore_2;
            break;
        case OP_istore_3:
            return istore_3;
            break;
        case OP_lstore_0:
            return lstore_0;
            break;
        case OP_lstore_1:
            return lstore_1;
            break;
        case OP_lstore_2:
            return lstore_2;
            break;
        case OP_lstore_3:
            return lstore_3;
            break;
        case OP_fstore_0:
            return fstore_0;
            break;
        case OP_fstore_1:
            return fstore_1;
            break;
        case OP_fstore_2:
            return fstore_2;
            break;
        case OP_fstore_3:
            return fstore_3;
            break;
        case OP_dstore_0:
            return dstore_0;
            break;
        case OP_dstore_1:
            return dstore_1;
            break;
        case OP_dstore_2:
            return dstore_2;
            break;
        case OP_dstore_3:
            return dstore_3;
            break;
        case OP_astore_0:
            return astore_0;
            break;
        case OP_astore_1:
            return astore_1;
            break;
        case OP_astore_2:
            return astore_2;
            break;
        case OP_astore_3:
            return astore_3;
            break;
        case OP_iastore:
            return iastore;
            break;
        case OP_lastore:
            return lastore;
            break;
        case OP_fastore:
            return fastore;
            break;
        case OP_dastore:
            return dastore;
            break;
        case OP_aastore:
            return aastore;
            break;
        case OP_bastore:
            return bastore;
            break;
        case OP_castore:
            return castore;
            break;
        case OP_sastore:
            return sastore;
            break;
        case OP_wide:
            return wide;
            break;
        case OP_dup:
            return Dup;
        case OP_pop:
            return pop;
            break;
        case OP_getstatic:
            return getstatic;
            break;
        case OP_putstatic:
            return putstatic;
            break;
        case OP_getfield:
            return getfield;
            break;
        case OP_putfield:
            return putfield;
            break;
        case OP_invokevirtual:
            return invokevirtual;
            break;
        case OP_invokespecial:
            return invokespecial;
            break;
        case OP_invokestatic:
            return invokestatic;
            break;
        case OP_invokeinterface:
            return invokeinterface;
            break;
        case OP_new:
            return New;
            break;
        case OP_anewarray:
            return anewarray;
        case OP_newarray:
            return newarray;
        case OP_arraylength:
            return arraylength;
        case OP_multianewarray:
            return multianewarray;
            break;
        case OP_ireturn:
            return ireturn;
            break;
        case OP_lreturn:
            return lreturn;
            break;
        case OP_freturn:
            return freturn;
            break;
        case OP_dreturn:
            return dreturn;
            break;
        case OP_areturn:
            return areturn;
            break;
        case OP_return:
            return return_;
            break;
        case OP_i2l:
            return i2l;
            break;
        case OP_i2f:
            return i2f;
            break;
        case OP_i2d:
            return i2d;
            break;
        case OP_l2i:
            return l2i;
            break;
        case OP_l2f:
            return l2f;
            break;
        case OP_l2d:
            return l2d;
            break;
        case OP_f2i:
            return f2i;
            break;
        case OP_f2l:
            return f2l;
            break;
        case OP_f2d:
            return f2d;
            break;
        case OP_d2i:
            return d2i;
            break;
        case OP_d2l:
            return d2l;
            break;
        case OP_d2f:
            return d2f;
            break;
        case OP_i2b:
            return i2b;
            break;
        case OP_i2c:
            return i2c;
            break;
        case OP_i2s:
            return i2s;
            break;
        case OP_lcmp:
            return lcmp;
            break;
        case OP_fcmpl:
            return fcmpl;
            break;
        case OP_fcmpg:
            return fcmpg;
            break;
        case OP_dcmpl:
            return dcmpl;
            break;
        case OP_dcmpg:
            return dcmpg;
            break;
        case OP_ifeq:
            return ifeq;
            break;
        case OP_ifne:
            return ifne;
            break;
        case OP_iflt:
            return iflt;
            break;
        case OP_ifge:
            return ifge;
            break;
        case OP_ifgt:
            return ifgt;
            break;
        case OP_ifle:
            return ifle;
            break;
        case OP_if_icmpeq:
            return if_icmpeq;
            break;
        case OP_if_icmpne:
            return if_icmpne;
            break;
        case OP_if_icmplt:
            return if_icmplt;
            break;
        case OP_if_icmpge:
            return if_icmpge;
            break;
        case OP_if_icmpgt:
            return if_icmpgt;
            break;
        case OP_if_icmple:
            return if_icmple;
            break;
        case OP_if_acmpeq:
            return if_acmpeq;
            break;
        case OP_if_acmpne:
            return if_acmpne;
            break;
        case OP_goto:
            return goto_;
            break;
        case OP_jsr:
            return jsr;
            break;
        case OP_ret:
            return ret;
            break;
        case OP_tableswitch:
            return tableswitch;
            break;
        case OP_lookupswitch:
            return lookupswitch;
            break;
        case OP_ifnull:
            return ifnull;
            break;
        case OP_ifnonnull:
            return ifnonnull;
            break;
        case OP_goto_w:
            return goto_w;
            break;
        case OP_jsr_w:
            return jsr_w;
            break;
        default:
            break;
    }
    char buffer[100];
    sprintf(buffer, "Instrucao \"%s\" nao encontrada.", getOpcodeName(bytecode));
    JVMstopAbrupt(buffer);
    return NULL;

}


//--------------------------------------------------------------------------------------------------
// SUBMODULO: Tratamento de excecoes lancadas pela JVM
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void JVMstopAbrupt(const char* msg){
    
    printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    if(msg) printf("\nERRO:\n %s", msg);
    printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    
    printf("\n\nForcando encerramento da JVM...\n\n");

    //TODO Liberar todas as estruturas
    
    exit(-1);
    
}


//--------------------------------------------------------------------------------------------------
void JVMThrow(int exception, Environment* environment){
    
    char mensagem[1000];
    strcpy(mensagem, "Excessao: ");
    
    //Selecionamos a mensagem de erro
    switch (exception) {
        case NullPointerException:
            strcat(mensagem, "NullPointerException");
            break;
        case AbstractMethodError:
            strcat(mensagem, "AbstractMethodError");
            break;
        case IncompatibleClassChangeError:
            strcat(mensagem, "IncompatibleClassChangeError");
            break;
        case IllegalAccessError:
            strcat(mensagem, "IllegalAccessError");
            break;
        case ArrayIndexOutOfBoundsException:
            strcat(mensagem, "ArrayIndexOutOfBoundsException");
            break;
        case NegativeArraySizeException:
            strcat(mensagem, "NegativeArraySizeException");
            break;
        default:
            break;
    }
    strcat(mensagem, "\n------------------------------------");
    strcat(mensagem, "\nClasse: ");
    strcat(mensagem, getClassNameFromConstantPool(environment->thread->vmStack->top->javaClass->arqClass->constant_pool,
                                                  environment->thread->vmStack->top->javaClass->arqClass->this_class));
    
    strcat(mensagem, "\nMetodo: ");
    strcat(mensagem, getUTF8FromConstantPool(environment->thread->vmStack->top->javaClass->arqClass->constant_pool,
                                             environment->thread->vmStack->top->method_info->name_index));
    strcat(mensagem, "\n------------------------------------");
    
    JVMstopAbrupt(mensagem);
}


//--------------------------------------------------------------------------------------------------
void JVMPrintFrameInfo(Frame* frame, u1 opcode){

    printf("\n------------------------------------");
    
    printf("\nClasse: %s\n", getClassNameFromConstantPool(frame->javaClass->arqClass->constant_pool,
                                                          frame->javaClass->arqClass->this_class));
    printf("\nMetodo: %s\n", getUTF8FromConstantPool(frame->javaClass->arqClass->constant_pool,
                                                     frame->method_info->name_index));
    
    if (opcode) printf("\nOpcode: %s\n", getOpcodeName(opcode));
    
    CodeAttribute* code = getCodeFromMethodInfo(frame->method_info, frame->javaClass->arqClass->constant_pool);
    
    printf("\nPilha de Operandos:");
    for (OperandStack* i = frame->opStk; i != NULL; i = i->nextStack) printf("\n| 0x%x", i->top);
    
    printf("\n\nVetor de Variaveis Locais:\n");

    for (int i = 0; i < code->max_locals; i++)
        printf(" 0x%x |", frame->localVariablesVector[i]);
    
    printf("\n------------------------------------");


}

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Instrucoes de comparacao e desvio
//--------------------------------------------------------------------------------------------------

void lcmp(Environment *environment){

    u8 operando1 = 0;
    u8 operando2 = 0;

    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;

    u4 resultado = -1000;

    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);

    operando1 = (u8) operandoPilha1;
    operando1 = (u8) (operando1 << 32) | operandoPilha2;

    long long operando1ComSinal = (long long)operando1;

    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);

    operando2 = (u8) operandoPilha1;
    operando2 = (u8) (operando2 << 32) | operandoPilha2;

    long long operando2ComSinal = (long long) operando2;

    if (operando2ComSinal > operando1ComSinal) {
        resultado = 1;
    }else if (operando2ComSinal == operando1ComSinal){
        resultado = 0;
    }else{
        resultado = -1;
    }
    pushInOperandStack(environment->thread, resultado);
}

void fcmpl(Environment *environment){

    float operando1 = 0;
    float operando2 = 0;

    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    u4 resultado = -1000;

    operandoPilha1 = popFromOperandStack(environment->thread);
    operando1 = u4ToFLoat(operandoPilha1);

    operandoPilha2 = popFromOperandStack(environment->thread);
    operando2 = u4ToFLoat(operandoPilha2);

    if(operando2 > operando1){
        resultado = 1;
    }else if (operando2 == operando1){
        resultado = 0;
    }else if (operando2 < operando1){
        resultado = -1;
    }else{
        //Caso o valor passado seja um NaN
        resultado = -1;
    }
    pushInOperandStack(environment->thread, resultado);
}

void fcmpg(Environment *environment){
    float operando1 = 0;
    float operando2 = 0;

    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    u4 resultado = -1000;

    operandoPilha1 = popFromOperandStack(environment->thread);
    operando1 = u4ToFLoat(operandoPilha1);

    operandoPilha2 = popFromOperandStack(environment->thread);
    operando2 = u4ToFLoat(operandoPilha2);

    if(operando2 > operando1){
        resultado = 1;
    }else if (operando2 == operando1){
        resultado = 0;
    }else if (operando2 < operando1){
        resultado = -1;
    }else{
        //Caso o valor passado seja um NaN
        resultado = 1;
    }
    pushInOperandStack(environment->thread, resultado);
}

void dcmpl(Environment *environment){

    double operando1 = 0;
    double operando2 = 0;

    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    u4 resultado = -1000;

    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment ->thread);

    operando1 = u4ToDouble(operandoPilha1, operandoPilha2);

    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);

    operando2 = u4ToDouble(operandoPilha1, operandoPilha2);

    if(operando2 > operando1){
        resultado = 1;
    }else if (operando2 == operando1){
        resultado = 0;
    }else if (operando2 < operando1){
        resultado = -1;
    }else{
        resultado = -1;
    }
    pushInOperandStack(environment->thread, resultado);
}
void dcmpg(Environment *environment){
    double operando1 = 0;
    double operando2 = 0;

    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    u4 resultado = -1000;

    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment ->thread);

    operando1 = u4ToDouble(operandoPilha1, operandoPilha2);

    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);

    operando2 = u4ToDouble(operandoPilha1, operandoPilha2);

    if(operando2 > operando1){
        resultado = 1;
    }else if (operando2 == operando1){
        resultado = 0;
    }else if (operando2 < operando1){
        resultado = -1;
    }else{
        resultado = 1;
    }
    pushInOperandStack(environment->thread, resultado);

}

void ifeq(Environment *environment){
    short int offset;
    u4 operando = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;
    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando = popFromOperandStack(environment->thread);

    if(operando == 0){
        environment->thread->PC += offset;
    }
}

void ifne(Environment *environment){
    short int offset;
    u4 operando = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;
    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando = popFromOperandStack(environment->thread);

    if(operando != 0){
        environment->thread->PC += offset;
    }
}

void iflt(Environment *environment){
    short int offset;
    u4 operando = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;
    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando = popFromOperandStack(environment->thread);

    //Como u4 eh unsigned fiz um cast para int que contem 4 bytes porem sem unsigned
    int operandoSemUnsigned = (int) operando;

    if(operandoSemUnsigned < 0){
        environment->thread->PC += offset;
    }
}

void ifge(Environment *environment){
    short int offset;
    u4 operando = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;

    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando = popFromOperandStack(environment->thread);

    //Como u4 eh unsigned fiz um cast para int que contem 4 bytes porem sem unsigned
    int operandoSemUnsigned = (int) operando;

    if(operandoSemUnsigned >= 0){
        environment->thread->PC += offset;
    }
}

void ifgt(Environment *environment){
    short int offset;
    u4 operando = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;

    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando = popFromOperandStack(environment->thread);

    //Como u4 eh unsigned fiz um cast para int que contem 4 bytes porem sem unsigned
    int operandoSemUnsigned = (int) operando;

    if(operandoSemUnsigned > 0){
        environment->thread->PC += offset;
    }
}

void ifle(Environment *environment){
    short int offset;
    u4 operando = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;
    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando = popFromOperandStack(environment->thread);

    //Como u4 eh unsigned fiz um cast para int que contem 4 bytes porem sem unsigned
    int operandoSemUnsigned = (int) operando;

    if(operandoSemUnsigned <= 0){
        environment->thread->PC += offset;
    }
}

void if_icmpeq(Environment *environment){
    short int offset;
    int operando1 = 0;
    int operando2 = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;

    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando1 =(int) popFromOperandStack(environment->thread);
    operando2 =(int) popFromOperandStack(environment->thread);

    if(operando2 == operando1){
        environment->thread->PC += offset;
    }
}

void if_icmpne(Environment *environment){

    short int offset;
    int operando1 = 0;
    int operando2 = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;

    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando1 =(int) popFromOperandStack(environment->thread);
    operando2 =(int) popFromOperandStack(environment->thread);

    if(operando2 != operando1){
        environment->thread->PC += offset;
    }
}

void if_icmplt(Environment *environment){
    short int offset;
    int operando1 = 0;
    int operando2 = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;

    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando1 =(int) popFromOperandStack(environment->thread);
    operando2 =(int) popFromOperandStack(environment->thread);

    if(operando2 < operando1){
        environment->thread->PC += offset;
    }
}

void if_icmpge(Environment *environment){
    short int offset;
    int operando1 = 0;
    int operando2 = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;

    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando1 =(int) popFromOperandStack(environment->thread);
    operando2 =(int) popFromOperandStack(environment->thread);

    if(operando2 >= operando1){
        environment->thread->PC += offset;
    }
}

void if_icmpgt(Environment *environment){
    short int offset;
    int operando1 = 0;
    int operando2 = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;

    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando1 =(int) popFromOperandStack(environment->thread);
    operando2 =(int) popFromOperandStack(environment->thread);

    if(operando2 > operando1){
        environment->thread->PC += offset;
    }
}

void if_icmple(Environment *environment){
    short int offset;
    int operando1 = 0;
    int operando2 = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;

    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando1 =(int) popFromOperandStack(environment->thread);
    operando2 =(int) popFromOperandStack(environment->thread);

    if(operando2 <= operando1){
        environment->thread->PC += offset;
    }
}

void if_acmpeq(Environment *environment){
    short int offset;
    u4 operando1 = 0;
    u4 operando2 = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;

    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando1 = popFromOperandStack(environment->thread);
    operando2 = popFromOperandStack(environment->thread);

    if(operando2 == operando1){
        environment->thread->PC += offset;
    }
}

void if_acmpne(Environment *environment){
    short int offset;
    u4 operando1 = 0;
    u4 operando2 = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;

    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    offset = (u2)byte1;
    offset = offset << 8;
    offset |= byte2;
    offset -= 3;

    operando1 = popFromOperandStack(environment->thread);
    operando2 = popFromOperandStack(environment->thread);

    if(operando2 != operando1){
        environment->thread->PC += offset;
    }
}

void goto_(Environment *environment) {

    short int index;
    //u4 pc = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;
    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    index = (short int)byte1;
    index = index << 8;
    index |= (short int)byte2;

    environment->thread->PC += index - 3;
}

void jsr(Environment *environment) {
    short int index;
    //u4 pc = 0;
    u1 byte1, byte2;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->PC++;
    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    index = (short int)byte1;
    index = index << 8;
    index |= (short int)byte2;

    environment->thread->PC++;
    pushInOperandStack(environment->thread,environment->thread->vmStack->top->returnPC);

    environment->thread->vmStack->top->returnPC += index - 3;
}

void ret(Environment *environment) {
  environment->thread->PC++;
  u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                            environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                            ,environment->thread->PC);


  environment->thread->PC = environment->thread->vmStack->top->localVariablesVector[index_argument];
}

void tableswitch(Environment *environment) {
    int32_t i, pad, low, high, def, offset, index, opCode;
    u1 byte;

    low = 0;
    high = 0;
    def = 0;
    offset = 0;

    opCode = environment->thread->PC;
    environment->thread->PC++;
    pad = environment->thread->PC % 4;

    // Conta os 4 bytepads
    if (pad != 0) {
      for (i = 0; i < 4 - pad; i++) {
          environment->thread->PC++;
      }
    }

    // Carrega o default
    for (i = 0; i < 3; i++) {
      byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
      def |= (signed)byte;
      def = def << 8;
      environment->thread->PC++;
    }

    byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    def |= byte;
    environment->thread->PC++;

    // Carrega o low
    for (i = 0; i < 3; i++) {
      byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
      low |= (signed)byte;
      low = low << 8;
      environment->thread->PC++;
    }

    byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    low |= (signed)byte;
    environment->thread->PC++;

    // Carrega o high
    for (i = 0; i < 3; i++) {
      byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
      high |= (signed)byte;
      high = high << 8;
      environment->thread->PC++;
    }

    byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    high |= (signed)byte;
    environment->thread->PC++;

    // Carrega o index da operand stack
    index = (signed) popFromOperandStack(environment->thread);
    if (( (signed)index <  (signed)low) || ( (signed)index >  (signed)high)) {
        environment->thread->PC = def + opCode;
    } else {
        for (i = 0; i < index * 4; i++) {
            environment->thread->PC++;
        }
        for (i = 0; i < 3; i++) {
          byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
          offset |= (signed)byte;
          offset = offset << 8;
          environment->thread->PC++;
        }
        byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
        offset |= (signed)byte + opCode;
        environment->thread->PC = offset;
    }
}

void lookupswitch(Environment *environment) {
    u4 i, j, pad, npairs = 0, def = 0, key, found, opCode; //, pc;
    npair *pair, *auxiliar; // criar nova struct npair{ u4 match; u4 offset; }
    u1 byte;


    opCode = environment->thread->PC;
    environment->thread->PC++;
    pad = environment->thread->PC % 4;

    // Conta os 4 bytepads
    if (pad != 0) {
      for (i = 0; i < 4 - pad; i++) {
          environment->thread->PC++;
      }
    }

    // Carrega o default
    for (i = 0; i < 3; i++) {
      byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
      def |= (signed)byte;
      def = def << 8;
      environment->thread->PC++;
    }

    byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    def |= byte;
    environment->thread->PC++;

    for (i = 0; i < 3; i++) {
        byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
        npairs |= (u4)byte;
        npairs = npairs << 8;
        environment->thread->PC++;
    }
    byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    npairs |= (u4)byte;
    environment->thread->PC++;

    // Aloca a area para os pares de chave + offset do case buscado
    pair = calloc(npairs, sizeof(npair));
    auxiliar = pair;
    auxiliar->match = 0;

    // Monta os pares de chave + offset do case buscado
    for (i = 0; i < npairs; i++) {
        pair->match = 0;

        for (j = 0; j < 3; j++) {
            byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
            pair->match |= (u4)byte;
            pair->match = pair->match << 8;
            environment->thread->PC++;
        }

        byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
        pair->match |= (u4)byte;
        environment->thread->PC++;

        pair->offset = 0;

        for (j = 0; j < 3; j++) {
            byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
            pair->offset |= (u4)byte;
            pair->offset = pair->offset << 8;
            environment->thread->PC++;
        }
        byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
        pair->offset |= (u4)byte;
        environment->thread->PC++;

        pair++;
    }

    key = popFromOperandStack(environment->thread);
    found = 0;
    i = 0;
    /*pair = auxiliar;*/

    // Procura a key correspondente ao case do par e encaminha a thread para seu offset
    while ((!found) && (i < npairs)) {
        if (key == auxiliar->match) {
            found = 1;
            environment->thread->PC = auxiliar->offset + opCode;
        } else {
            i++;
            auxiliar++;
        }
    }
    if (!found) {
        environment->thread->PC = def + opCode;
    }
}

void ifnull(Environment *environment) {
    short int index;
    u4 auxiliar1; //pc = 0;
    u1 byte;

    environment->thread->PC++;
    byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);


    auxiliar1 = popFromOperandStack(environment->thread);
    index = (u2)byte;

    index = index << 8;

    environment->thread->PC++;
    byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    index |= (u2)byte;

    //Alteracao feita mas deve ser reportada ao Cristoffer por ser seu codigo
    if (auxiliar1 == -1000) {
        environment->thread->vmStack->top->returnPC += index - 2;
    } else {
        environment->thread->vmStack->top->returnPC++;
    }
}

void ifnonnull(Environment *environment) {
    short int index;
    u4 auxiliar1; // pc = 0;
    u1 byte;

    byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->vmStack->top->returnPC++;

    auxiliar1 = popFromOperandStack(environment->thread);

    index = (u2)byte;
    index = index << 8;
    byte = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);

    environment->thread->vmStack->top->returnPC++;
    index |= (u2)byte;

    //Alteracao feita mas deve ser reportada ao Cristoffer por ser seu codigo
    if (auxiliar1 != -1000) {
        environment->thread->vmStack->top->returnPC += index - 2;
    } else {
        environment->thread->vmStack->top->returnPC++;
    }
}


void jsr_w(Environment *environment) {
    u8 index;
    //u4 pc = 0;
    u1 byte1, byte2, byte3, byte4;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    index = (u8)byte1;
    index = index << 8;

    environment->thread->PC++;
    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    index |= (u8)byte2;
    index = index << 8;

    environment->thread->PC++;
    byte3 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    index |= (u8)byte3;
    index = index << 8;

    environment->thread->PC++;
    byte4 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    index |= (u8)byte4;

    pushInOperandStack(environment->thread,environment->thread->vmStack->top->returnPC);

    environment->thread->vmStack->top->returnPC += index - 5;
}

void goto_w(Environment *environment) {
    u8 index;
    //u4 pc = 0;
    u1 byte1, byte2, byte3, byte4;

    environment->thread->PC++;
    byte1 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    index = (u8)byte1;
    index = index << 8;

    environment->thread->PC++;
    byte2 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    index |= (u8)byte2;
    index = index << 8;

    environment->thread->PC++;
    byte3 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    index |= (u8)byte3;
    index = index << 8;

    environment->thread->PC++;
    byte4 = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    index |= (u8)byte4;

    environment->thread->vmStack->top->returnPC += index - 5;
}

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Conversao de Tipos
//--------------------------------------------------------------------------------------------------

void i2l(Environment* environment) {
    int64_t aux1;
    int32_t aux3;
    u4 aux2;

    aux3 = (signed) popFromOperandStack(environment->thread);
    aux1 = aux3;
    //CORRECAO: PRIMERIO O LOW
  aux2 = aux1 & SHIFT_MASK_UNSI;
    pushInOperandStack(environment->thread,aux2);

    //CORRECAO: DEPOIS O HIGH
  aux2 = aux1 >> 32;
    pushInOperandStack(environment->thread, aux2);
}

void i2f(Environment* environment) {
    u4 *aux1;
    int32_t aux3;
    float aux2;

    aux3 = (signed) popFromOperandStack(environment->thread);
    aux2 = (float) aux3;
    aux1 = malloc(sizeof(u4));


    memcpy(aux1, &aux2, sizeof(u4));

    pushInOperandStack(environment->thread,*aux1);
}


void i2d(Environment* environment) {
    int64_t *aux1;
    int32_t aux4;
    double aux2;
    u4 aux3;

    aux1 = malloc(sizeof(int64_t));
    aux4 = (signed) popFromOperandStack(environment->thread);
    aux2 = (double) aux4;


    memcpy(aux1, &aux2, sizeof(int64_t));

    aux3 = *aux1 & SHIFT_MASK_UNSI;
    pushInOperandStack(environment->thread,aux3);

    aux3 = *aux1 >> 32;
    pushInOperandStack(environment->thread,aux3);

}

void l2i(Environment* environment) {
    u4 aux1;

    // Remove o high do operand stack
    popFromOperandStack(environment->thread);

    // Pega o low do operand stack
    aux1 = (u4) popFromOperandStack(environment->thread);

    // Dá push do valor no operand stack
    pushInOperandStack(environment->thread,aux1);
}


void l2f(Environment* environment) {
    int64_t aux1, aux2;
    float aux3;
    u4 *u4aux;

    // Pega o high do operand stack
    aux1 = popFromOperandStack(environment->thread);
    // Pega o low do operand stack
    aux2 = popFromOperandStack(environment->thread);

    // Move o high com operação de 32 bits à esquerda e adiciona o low
    aux1 = aux1 << 32;
    aux1 = aux2;

    // Converte a junçao do high-low para um float
    aux3 = (float) aux1;
    u4aux = malloc(sizeof(u4));
    memcpy(u4aux, &aux3, sizeof(u4));

    // Dá push do valor no operand stack
    pushInOperandStack(environment->thread,*u4aux);
}


void l2d(Environment* environment) {
    int64_t aux1, *aux3, aux4;
    double aux2;
    u4 u4aux;

    // Pega o high do operand stack
    aux1 = popFromOperandStack(environment->thread);
    // Pega o low do operand stack
    aux4 = popFromOperandStack(environment->thread);

    // Move o high com operação de 32 bits à esquerda e adiciona o low
    aux1 = aux1 << 32;
    aux1 |= aux4;

    // Converte a junçao do high-low para um double
    aux2 = (double) aux1;
    aux3 = malloc(sizeof(int64_t));
    memcpy(aux3, &aux2, sizeof(int64_t));

    // Dá push do low no operand stack
    u4aux = *aux3 & SHIFT_MASK_UNSI;
    pushInOperandStack(environment->thread,u4aux);

    // Dá push do high no operand stack
    u4aux = *aux3 >> 32;
    pushInOperandStack(environment->thread,u4aux);
}

void f2i(Environment* environment) {
    u4 aux2;
    float aux1;
    int32_t aux3;

    aux2 = popFromOperandStack(environment->thread);
    memcpy(&aux1, &aux2, sizeof(float));
    aux3 = (int32_t) aux1;
    pushInOperandStack(environment->thread,aux3);
}

void f2l(Environment* environment) {
    int64_t aux1;
    u4 aux2;
    float aux3;

    aux1 = popFromOperandStack(environment->thread);

    aux2 = aux1 & SHIFT_MASK_UNSI;
    memcpy(&aux3, &aux2, sizeof(float));
    pushInOperandStack(environment->thread,aux3);

    aux2 = aux1 >> 32;
    memcpy(&aux3, &aux2, sizeof(float));
    pushInOperandStack(environment->thread,aux3);
}

void f2d(Environment* environment) {
    double aux4;
    float *aux2;
    u4 aux1, aux3;
    u8 aux5;

    aux1 = popFromOperandStack(environment->thread);
    aux2 = malloc(sizeof(float));

    memcpy(aux2, &aux1, sizeof(float));
    aux4 = *aux2;
    memcpy(&aux5, &aux4, sizeof(u8));

    aux3 = aux5 & 0xffffffff;
    pushInOperandStack(environment->thread,aux3);
    aux3 = aux5 >> 32;
    pushInOperandStack(environment->thread,aux3);
}

void d2i(Environment* environment) {
    int64_t aux1, aux4;
    double *aux2;
    int32_t aux3;

    aux1 = popFromOperandStack(environment->thread);
    aux4 = popFromOperandStack(environment->thread);

    aux1 = aux1 << 32;
    aux1 |= aux4;
    aux2 = malloc(sizeof(double));

    memcpy(aux2, &aux1, sizeof(double));
    aux3 = (int32_t) *aux2;
    pushInOperandStack(environment->thread,aux3);
}

void d2l(Environment* environment) {
    int64_t aux1, aux3;
    double *aux2;
    u4 aux0;

    aux1 = popFromOperandStack(environment->thread);
    aux3 = popFromOperandStack(environment->thread);
    aux1 = aux1 << 32;
    aux1 |= aux3;
    aux2 = malloc(sizeof(double));

    memcpy(aux2, &aux1, sizeof(double));
    aux1 = (int64_t) *aux2;
    aux0 = aux1 & SHIFT_MASK_UNSI;
    pushInOperandStack(environment->thread,aux0);
    aux0 = aux1 >> 32;
    pushInOperandStack(environment->thread,aux0);
}

void d2f(Environment* environment) {
    int64_t aux1, aux4;
    double *aux2;
    float aux3;
    u4 *aux0;

    aux1 = popFromOperandStack(environment->thread);
    aux4 = popFromOperandStack(environment->thread);
    aux1 = aux1 << 32;
    aux1 |= aux4;
    aux2 = malloc(sizeof(double));

    memcpy(aux2, &aux1, sizeof(double));
    aux3 = (float) *aux2;
    aux0 = malloc(sizeof(u4));

    memcpy(aux0, &aux3, sizeof(u4));
    pushInOperandStack(environment->thread,*aux0);

}

void i2b(Environment* environment) {
    char aux1;

    aux1 = (char) popFromOperandStack(environment->thread);
    pushInOperandStack(environment->thread,(int32_t) aux1);

}

void i2c(Environment* environment) {
    char aux1;

    aux1 = (char) popFromOperandStack(environment->thread);
    pushInOperandStack(environment->thread,(int32_t) aux1);

}

void i2s(Environment* environment) {
    short aux1;

    aux1 = (short) popFromOperandStack(environment->thread);
    pushInOperandStack(environment->thread,(int32_t) aux1);

}


//--------------------------------------------------------------------------------------------------
// SUBMODULO: Transferencia de valores entre Vetor de Variaveis Locais e a Pilha de Operandos de um Frame
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void nop(Environment* environment){}


//--------------------------------------------------------------------------------------------------
void aconst_null(Environment* environment){
    
    pushInOperandStack(environment->thread, (u4)NULL);
}


//--------------------------------------------------------------------------------------------------
void iconst_m1(Environment* environment){
    
    pushInOperandStack(environment->thread, -1);
}


//--------------------------------------------------------------------------------------------------
void iconst_0(Environment* environment){
    
    pushInOperandStack(environment->thread, 0);
}


//--------------------------------------------------------------------------------------------------
void iconst_1(Environment* environment){
    
    pushInOperandStack(environment->thread, 1);
}


//--------------------------------------------------------------------------------------------------
void iconst_2(Environment* environment){
    
    pushInOperandStack(environment->thread, 2);
}


//--------------------------------------------------------------------------------------------------
void iconst_3(Environment* environment){
    
    pushInOperandStack(environment->thread, 3);
}


//--------------------------------------------------------------------------------------------------
void iconst_4(Environment* environment){
    
    pushInOperandStack(environment->thread, 4);
}


//--------------------------------------------------------------------------------------------------
void iconst_5(Environment* environment){
    
    pushInOperandStack(environment->thread, 5);
}


//--------------------------------------------------------------------------------------------------
void lconst_0(Environment* environment){
    
    pushInOperandStack(environment->thread, 0); //parte baixa
    pushInOperandStack(environment->thread, 0); //parte alta
}


//--------------------------------------------------------------------------------------------------
void lconst_1(Environment* environment){
    
    pushInOperandStack(environment->thread, 1);
    pushInOperandStack(environment->thread, 0);
}


//--------------------------------------------------------------------------------------------------
void fconst_0(Environment* environment){
    float x = 0;
    u4 Const;
    
    memcpy(&Const, &x, sizeof(u4));
    pushInOperandStack(environment->thread, Const);
}


//--------------------------------------------------------------------------------------------------
void fconst_1(Environment* environment){
    float x = 1;
    u4 Const;
    
    memcpy(&Const, &x, sizeof(u4));
    pushInOperandStack(environment->thread, Const);
}


//--------------------------------------------------------------------------------------------------
void fconst_2(Environment* environment){
    float x = 2;
    u4 Const;
    
    memcpy(&Const, &x, sizeof(u4));
    pushInOperandStack(environment->thread, Const);
}


//--------------------------------------------------------------------------------------------------
void dconst_0(Environment* environment){
    double x = 0;
    long long Const;
    
    memcpy(&Const, &x, sizeof(long long));
    
    u4 operandoPilha1 = (u4) ((Const & 0xFFFFFFFF00000000) >> 32);
    u4 operandoPilha2 = (u4) Const & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
}


//--------------------------------------------------------------------------------------------------
void dconst_1(Environment* environment){
    double x = 1;
    long long Const;
    
    memcpy(&Const, &x, sizeof(long long));
    
    u4 operandoPilha1 = (u4) ((Const & 0xFFFFFFFF00000000) >> 32);
    u4 operandoPilha2 = (u4) Const & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
}


//--------------------------------------------------------------------------------------------------
void bipush(Environment* environment){
    
    environment->thread->PC++;
    u1 byte_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                          environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                          ,environment->thread->PC);
    
    int byte_signal_extend = (signed char) byte_argument;
    
    pushInOperandStack(environment->thread, byte_signal_extend);
}


//--------------------------------------------------------------------------------------------------
void sipush(Environment* environment){
    
    environment->thread->PC++;
    u1 byte1_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                             environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                             ,environment->thread->PC);
    
    environment->thread->PC++;
    u1 byte2_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    short short_result = (byte1_argument << 8) | byte2_argument;
    
    int short_signal_extend = (signed short) short_result;
    
    pushInOperandStack(environment->thread, short_signal_extend);
}


//--------------------------------------------------------------------------------------------------
void ldc(Environment* environment){
    
    environment->thread->PC++;
    u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    u1 tag_valor_constant_pool = environment->thread->vmStack->top->javaClass->arqClass->constant_pool[index_argument-1].tag;
    
    if (tag_valor_constant_pool == CONSTANT_Integer || tag_valor_constant_pool == CONSTANT_Float){
        
        // Estrutra Integer e Float possuem o mesmo formato
        u4 valor_numerico = environment->thread->vmStack->top->javaClass->arqClass->constant_pool[index_argument-1].u.Integer.bytes;
        
        pushInOperandStack(environment->thread, valor_numerico);
    }
    else if (tag_valor_constant_pool == CONSTANT_String){
        
        //CORRECAO: REFERENCIA PARA STRING INFO, NAO PARA O INDICE DA STRING INFO
        u4 referencia = (u4) &environment->thread->vmStack->top->javaClass->arqClass->constant_pool[index_argument-1];
        
        pushInOperandStack(environment->thread, referencia);
    }
}


//--------------------------------------------------------------------------------------------------
void ldc_w(Environment* environment){
    
    environment->thread->PC++;
    u1 index1byte_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    environment->thread->PC++;
    u1 index2byte_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                                   environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                                   ,environment->thread->PC);
    
    u2 index_result = (index1byte_argument << 8) | index2byte_argument;
    
    u1 tag_valor_constant_pool = environment->thread->vmStack->top->javaClass->arqClass->constant_pool[index_result-1].tag;
    
    if (tag_valor_constant_pool == CONSTANT_Integer || tag_valor_constant_pool == CONSTANT_Float){
        
        // Estrutra Integer e Float possuem o mesmo formato
        u4 valor_numerico = environment->thread->vmStack->top->javaClass->arqClass->constant_pool[index_result-1].u.Integer.bytes;
        
        pushInOperandStack(environment->thread, valor_numerico);
    }
    else if (tag_valor_constant_pool == CONSTANT_String){
        
        u4 referencia = (u4) &environment->thread->vmStack->top->javaClass->arqClass->constant_pool[index_result-1];

        
        pushInOperandStack(environment->thread, referencia);
    }
}


//--------------------------------------------------------------------------------------------------
void ldc2_w(Environment* environment){
    
    environment->thread->PC++;
    u1 index1byte_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                                   environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                                   ,environment->thread->PC);
    
    environment->thread->PC++;
    u1 index2byte_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                                   environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                                   ,environment->thread->PC);
    
    u2 index_result = (index1byte_argument << 8) | index2byte_argument;
    
    u1 tag_valor_constant_pool = environment->thread->vmStack->top->javaClass->arqClass->constant_pool[index_result-1].tag;
    
    if (tag_valor_constant_pool == CONSTANT_Long || tag_valor_constant_pool == CONSTANT_Double){
        
        //Estrutura Long e Double possuem o mesmo formato
        u4 valor_numerico_high = environment->thread->vmStack->top->javaClass->arqClass->constant_pool[index_result-1].u.Long.high_bytes;
        
        u4 valor_numerico_low = environment->thread->vmStack->top->javaClass->arqClass->constant_pool[index_result-1].u.Long.low_bytes;
        
        pushInOperandStack(environment->thread, valor_numerico_low);
        pushInOperandStack(environment->thread, valor_numerico_high);
    }
}


//--------------------------------------------------------------------------------------------------
void iload(Environment* environment){
    
    environment->thread->PC++;
    u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[index_argument];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void lload(Environment* environment){
    
    environment->thread->PC++;
    u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    
    u4 valor_numerico_high = environment->thread->vmStack->top->localVariablesVector[index_argument];
    u4 valor_numerico_low = environment->thread->vmStack->top->localVariablesVector[index_argument+1];
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void fload(Environment* environment){
    
    environment->thread->PC++;
    u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[index_argument];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void dload(Environment* environment){
    
    environment->thread->PC++;
    u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    
    u4 valor_numerico_high = environment->thread->vmStack->top->localVariablesVector[index_argument];
    u4 valor_numerico_low = environment->thread->vmStack->top->localVariablesVector[index_argument+1];
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void aload(Environment* environment){
    
    environment->thread->PC++;
    u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[index_argument];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void iload_0(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[0];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void iload_1(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[1];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void iload_2(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[2];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void iload_3(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[3];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void lload_0(Environment* environment){
    
    u4 valor_numerico_high = environment->thread->vmStack->top->localVariablesVector[0];
    u4 valor_numerico_low = environment->thread->vmStack->top->localVariablesVector[1];
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void lload_1(Environment* environment){
    
    u4 valor_numerico_high = environment->thread->vmStack->top->localVariablesVector[1];
    u4 valor_numerico_low = environment->thread->vmStack->top->localVariablesVector[2];
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void lload_2(Environment* environment){
    
    u4 valor_numerico_high = environment->thread->vmStack->top->localVariablesVector[2];
    u4 valor_numerico_low = environment->thread->vmStack->top->localVariablesVector[3];
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void lload_3(Environment* environment){
    
    u4 valor_numerico_high = environment->thread->vmStack->top->localVariablesVector[3];
    u4 valor_numerico_low = environment->thread->vmStack->top->localVariablesVector[4];
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void fload_0(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[0];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void fload_1(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[1];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void fload_2(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[2];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void fload_3(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[3];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void dload_0(Environment* environment){
    
    u4 valor_numerico_high = environment->thread->vmStack->top->localVariablesVector[0];
    u4 valor_numerico_low = environment->thread->vmStack->top->localVariablesVector[1];
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void dload_1(Environment* environment){
    
    u4 valor_numerico_high = environment->thread->vmStack->top->localVariablesVector[1];
    u4 valor_numerico_low = environment->thread->vmStack->top->localVariablesVector[2];
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void dload_2(Environment* environment){
    
    u4 valor_numerico_high = environment->thread->vmStack->top->localVariablesVector[2];
    u4 valor_numerico_low = environment->thread->vmStack->top->localVariablesVector[3];
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void dload_3(Environment* environment){
    
    u4 valor_numerico_high = environment->thread->vmStack->top->localVariablesVector[3];
    u4 valor_numerico_low = environment->thread->vmStack->top->localVariablesVector[4];
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void aload_0(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[0];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void aload_1(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[1];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void aload_2(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[2];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void aload_3(Environment* environment){
    
    u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[3];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void iaload(Environment* environment){
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    u4 valor_numerico = *((u4*)((array_info->arrayAddress)+ index*sizeof(u4)));
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void laload(Environment* environment){
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    u8* array = array_info->arrayAddress;
    
    u8 value = array[index];
    
    u4 valor_numerico_high = value >> 32;
    u4 valor_numerico_low = (u4) value;
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void faload(Environment* environment){
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    u4 valor_numerico = *((u4*)((array_info->arrayAddress)+index*sizeof(u4)));
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void daload(Environment* environment){
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    u8* array = array_info->arrayAddress;
    
    u8 value = array[index];
    
    u4 valor_numerico_high = value >> 32;
    u4 valor_numerico_low = (u4) value;
    
    pushInOperandStack(environment->thread, valor_numerico_low);
    pushInOperandStack(environment->thread, valor_numerico_high);
}


//--------------------------------------------------------------------------------------------------
void aaload(Environment* environment){
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    //Verificacao de erro de acesso de indice
    if (index >= array_info->count) JVMThrow(ArrayIndexOutOfBoundsException, environment);
    
    u4* array = array_info->arrayAddress;
    
    u4 valor_numerico = array[index];
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void baload(Environment* environment){
    
    int valor_numerico;
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    // Se o valor no componente do vetor for do tipo byte
    if (array_info->atype == T_BYTE) {
        
        // O valor eh estendido com sinal
        valor_numerico = *((char*)((array_info->arrayAddress)+index));
        
    }
    // Se o valor no componente do vetor for do tipo boolean
    else if (array_info->atype == T_BOOLEAN) {
        
        // O valor eh estendido sem sinal
        valor_numerico = *((u1*)((array_info->arrayAddress)+index));
        
    }
    
    pushInOperandStack(environment->thread, valor_numerico);
}


//--------------------------------------------------------------------------------------------------
void caload(Environment* environment){
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    u4 valor_numerico_zero_extend = *((u1*)((array_info->arrayAddress)+index));
    
    pushInOperandStack(environment->thread, valor_numerico_zero_extend);
}


//--------------------------------------------------------------------------------------------------
void saload(Environment* environment){
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    int valor_numerico_signal_extend = *((signed short*)((array_info->arrayAddress)+index*sizeof(u2)));
    
    pushInOperandStack(environment->thread, valor_numerico_signal_extend);
}


//--------------------------------------------------------------------------------------------------
void istore(Environment* environment){
    
    environment->thread->PC++;
    u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[index_argument] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void lstore(Environment* environment){
    
    environment->thread->PC++;
    u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[index_argument] = valor_numerico_high;
    environment->thread->vmStack->top->localVariablesVector[index_argument+1] = valor_numerico_low;
}


//--------------------------------------------------------------------------------------------------
void fstore(Environment* environment){
    
    environment->thread->PC++;
    u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[index_argument] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void dstore(Environment* environment){
    
    environment->thread->PC++;
    u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[index_argument] = valor_numerico_high;
    environment->thread->vmStack->top->localVariablesVector[index_argument+1] = valor_numerico_low;
}


//--------------------------------------------------------------------------------------------------
void astore(Environment* environment){
    
    environment->thread->PC++;
    u1 index_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[index_argument] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void istore_0(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[0] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void istore_1(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[1] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void istore_2(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[2] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void istore_3(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[3] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void lstore_0(Environment* environment){
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[0] = valor_numerico_high;
    environment->thread->vmStack->top->localVariablesVector[1] = valor_numerico_low;
}


//--------------------------------------------------------------------------------------------------
void lstore_1(Environment* environment){
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[1] = valor_numerico_high;
    environment->thread->vmStack->top->localVariablesVector[2] = valor_numerico_low;
}


//--------------------------------------------------------------------------------------------------
void lstore_2(Environment* environment){
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[2] = valor_numerico_high;
    environment->thread->vmStack->top->localVariablesVector[3] = valor_numerico_low;
}


//--------------------------------------------------------------------------------------------------
void lstore_3(Environment* environment){
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[3] = valor_numerico_high;
    environment->thread->vmStack->top->localVariablesVector[4] = valor_numerico_low;
}


//--------------------------------------------------------------------------------------------------
void fstore_0(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[0] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void fstore_1(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[1] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void fstore_2(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[2] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void fstore_3(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[3] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void dstore_0(Environment* environment){
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[0] = valor_numerico_high;
    environment->thread->vmStack->top->localVariablesVector[1] = valor_numerico_low;
}


//--------------------------------------------------------------------------------------------------
void dstore_1(Environment* environment){
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[1] = valor_numerico_high;
    environment->thread->vmStack->top->localVariablesVector[2] = valor_numerico_low;
}


//--------------------------------------------------------------------------------------------------
void dstore_2(Environment* environment){
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[2] = valor_numerico_high;
    environment->thread->vmStack->top->localVariablesVector[3] = valor_numerico_low;
}


//--------------------------------------------------------------------------------------------------
void dstore_3(Environment* environment){
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[3] = valor_numerico_high;
    environment->thread->vmStack->top->localVariablesVector[4] = valor_numerico_low;
}


//--------------------------------------------------------------------------------------------------
void astore_0(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[0] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void astore_1(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[1] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void astore_2(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[2] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void astore_3(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    environment->thread->vmStack->top->localVariablesVector[3] = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void iastore(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    *((u4*)((array_info->arrayAddress)+index * sizeof(u4))) = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void lastore(Environment* environment){
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    u8 value = valor_numerico_high;
    value = value<<32 | valor_numerico_low;
    
    u8* array = array_info->arrayAddress;
    
    array[index] = value;
}


//--------------------------------------------------------------------------------------------------
void fastore(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    *((u4*)((array_info->arrayAddress) + index * sizeof(u4))) = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void dastore(Environment* environment){
    
    u4 valor_numerico_high = popFromOperandStack(environment->thread);
    u4 valor_numerico_low = popFromOperandStack(environment->thread);
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    u8 value = valor_numerico_high;
    value = value<<32 | valor_numerico_low;
    
    u8* array = array_info->arrayAddress;
    
    array[index] = value;
}


//--------------------------------------------------------------------------------------------------
void aastore(Environment* environment){
    
    u4 valor_numerico = popFromOperandStack(environment->thread);
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    //TODO: Otherwise, if arrayref is not null and the actual type of value is not assignment compatible (§2.6.7) with the actual type of the components of the array, aastore throws an ArrayStoreException.
    
    *((u4*)((array_info->arrayAddress)+ index * sizeof(u4))) = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void bastore(Environment* environment){
    
    u1 valor_numerico;
    
    u4 valor_numerico_int = popFromOperandStack(environment->thread);
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    // Se componentes do vetor forem do tipo byte
    if (array_info->atype == T_BYTE) {
        
        // O int value eh truncado para byte
        valor_numerico = valor_numerico_int;
        
    }
    // Se componentes do vetor forem do tipo boolean
    else if (array_info->atype == T_BOOLEAN) {
        
        // O int value eh truncado para o seu bit de ordem mais baixa e estendido sem sinal
        valor_numerico = (u1)(valor_numerico_int & 0x1);
        
    }
    
    *((u1*)((array_info->arrayAddress)+index)) = valor_numerico;
}


//--------------------------------------------------------------------------------------------------
void castore(Environment* environment){
    
    u4 valor_numerico_int = popFromOperandStack(environment->thread);
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    u1 valor_numerico_char = valor_numerico_int;
    
    *((u1*)((array_info->arrayAddress)+index)) = valor_numerico_char;
}


//--------------------------------------------------------------------------------------------------
void sastore(Environment* environment){
    
    u4 valor_numerico_int = popFromOperandStack(environment->thread);
    
    u4 index = popFromOperandStack(environment->thread);
    
    JavaArray* array_info = (JavaArray*) popFromOperandStack(environment->thread);
    
    if (array_info->arrayAddress == NULL) {
        //TODO: throw NullPointerException;
    }
    if (index > array_info->count) {
        //TODO: Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
    }
    
    u2 valor_numerico_short = valor_numerico_int;
    
    *((u2*)((array_info->arrayAddress)+ index * sizeof(u2))) = valor_numerico_short;
}


//--------------------------------------------------------------------------------------------------
void wide(Environment* environment){
    
    environment->thread->PC++;
    u1 opcode_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                              environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                              ,environment->thread->PC);
    
    environment->thread->PC++;
    u1 indexbyte1_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                               environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                               ,environment->thread->PC);
    
    environment->thread->PC++;
    u1 indexbyte2_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                               environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                               ,environment->thread->PC);
    
    u2 index_result = (indexbyte1_argument << 8) | indexbyte2_argument;
    
    
    // PRIMEIRO FORMATO
    if (opcode_argument == OP_iload || opcode_argument == OP_fload || opcode_argument == OP_aload) {
        
        u4 valor_numerico = environment->thread->vmStack->top->localVariablesVector[index_result];
        
        pushInOperandStack(environment->thread, valor_numerico);
    }
    else if (opcode_argument == OP_istore || opcode_argument == OP_fstore || opcode_argument == OP_astore){
        
        u4 valor_numerico = popFromOperandStack(environment->thread);
        
        environment->thread->vmStack->top->localVariablesVector[index_result] = valor_numerico;
    }
    else if (opcode_argument == OP_lload || opcode_argument == OP_dload) {
        
        u4 valor_numerico_high = environment->thread->vmStack->top->localVariablesVector[index_result];
        u4 valor_numerico_low = environment->thread->vmStack->top->localVariablesVector[index_result+1];
        
        pushInOperandStack(environment->thread, valor_numerico_low);
        pushInOperandStack(environment->thread, valor_numerico_high);
    }
    else if (opcode_argument == OP_lstore || opcode_argument == OP_dstore) {
        
        u4 valor_numerico_high = popFromOperandStack(environment->thread);
        u4 valor_numerico_low = popFromOperandStack(environment->thread);
        
        environment->thread->vmStack->top->localVariablesVector[index_result] = valor_numerico_high;
        environment->thread->vmStack->top->localVariablesVector[index_result+1] = valor_numerico_low;
    }
    else if (opcode_argument == OP_ret) {
        //TODO:
    }
    
    // SEGUNDO FORMATO
    else if (opcode_argument == OP_iinc) {
        
        environment->thread->PC++;
        u1 constbyte1_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                                       environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                                       ,environment->thread->PC);
        
        environment->thread->PC++;
        u1 constbyte2_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                                       environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                                       ,environment->thread->PC);
        
        int constbyte_result = (int)((constbyte1_argument << 8) | constbyte2_argument);
        
        u4 index_forma2 = (u4)(index_result + constbyte_result);
        
        index_forma2 = index_forma2;
        
        //TODO:
    }
}


//--------------------------------------------------------------------------------------------------
void Dup(Environment* environment){

    u4 value = popFromOperandStack(environment->thread);
    
    pushInOperandStack(environment->thread, value);
    pushInOperandStack(environment->thread, value);
}


//--------------------------------------------------------------------------------------------------
void pop(Environment* environment){
    popFromOperandStack(environment->thread);
}

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Instrucoes logico atmeticas
//--------------------------------------------------------------------------------------------------

void iadd(Environment* environment){
    
    u4 operando1 = 0;
    u4 operando2 = 0;
    u4 soma = 0;
    
    operando1 = popFromOperandStack(environment->thread);
    
    operando2 = popFromOperandStack(environment->thread);
    
    soma = operando1 + operando2;
    
    pushInOperandStack(environment->thread, soma);
    
}


void ladd(Environment* environment){
    
    u8 operando1_64 = 0;
    u8 operando2_64 = 0;
    u8 soma64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando1_64 = (u8) operandoPilha1;
    operando1_64 = (u8) (operando1_64 << 32) | operandoPilha2;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando2_64 = (u8) operandoPilha1;
    operando2_64 = (u8) (operando2_64 << 32) | operandoPilha2;
    
    soma64 = operando1_64 + operando2_64;
    
    operandoPilha1 = (u4) ((soma64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) soma64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void fadd(Environment* environment){
    
    float operando1 = 0;
    float operando2 = 0;
    float soma = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    u4 somaPilha = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    memcpy(&operando1, &operandoPilha1, sizeof(float));
    memcpy(&operando2, &operandoPilha2, sizeof(float));
    
    soma = operando1 + operando2;
    
    memcpy(&somaPilha, &soma, sizeof(u4));
    
    pushInOperandStack(environment->thread, somaPilha);
    
}

void dadd(Environment* environment){
    
    double operando1 = 0;
    double operando2 = 0;
    double soma = 0;
    
    u4 operandoPilha1;
    u4 operandoPilha2;
    
    u8 operandoSoma1;
    u8 operandoSoma2;
    u8 somaLong;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operandoSoma1 = (u8) operandoPilha1;
    operandoSoma1 = (u8) (operandoSoma1 << 32) | operandoPilha2;
    
    memcpy(&operando1, &operandoSoma1, sizeof(double));
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operandoSoma2 = (u8) operandoPilha1;
    operandoSoma2 = (u8) (operandoSoma2 << 32) | operandoPilha2;
    
    memcpy(&operando2, &operandoSoma2, sizeof(double));
    
    soma = operando1 + operando2;
    
    memcpy(&somaLong, &soma, sizeof(u8));
    
    operandoPilha1 = (u4) ((somaLong & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) somaLong & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
}

void isub(Environment* environment){
    
    u4 operando1 = 0;
    u4 operando2 = 0;
    u4 sub = 0;
    
    operando1 = popFromOperandStack(environment->thread);
    
    operando2 = popFromOperandStack(environment->thread);
    
    sub = operando2 - operando1;
    
    pushInOperandStack(environment->thread, sub);
    
}

void lsub(Environment* environment){
    
    u8 operando1_64 = 0;
    u8 operando2_64 = 0;
    u8 sub64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando1_64 = (u8) operandoPilha1;
    operando1_64 = (u8) (operando1_64 << 32) | operandoPilha2;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando2_64 = (u8) operandoPilha1;
    operando2_64 = (u8) (operando2_64 << 32) | operandoPilha2;
    
    sub64 = operando2_64 - operando1_64;
    
    operandoPilha1 = (u4) ((sub64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) sub64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void fsub(Environment* environment){
    
    float operando1 = 0;
    float operando2 = 0;
    float sub = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    u4 subPilha = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    memcpy(&operando1, &operandoPilha1, sizeof(float));
    memcpy(&operando2, &operandoPilha2, sizeof(float));
    
    sub = operando2 - operando1;
    
    memcpy(&subPilha, &sub, sizeof(u4));
    
    pushInOperandStack(environment->thread, subPilha);
    
}

void dsub(Environment* environment){
    
    double operando1 = 0;
    double operando2 = 0;
    double sub = 0;
    
    u4 operandoPilha1;
    u4 operandoPilha2;
    
    u8 operandoSub1;
    u8 operandoSub2;
    u8 subLong;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operandoSub1 = (u8) operandoPilha1;
    operandoSub1 = (u8) (operandoSub1 << 32) | operandoPilha2;
    
    memcpy(&operando1, &operandoSub1, sizeof(double));
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operandoSub2 = (u8) operandoPilha1;
    operandoSub2 = (u8) (operandoSub2 << 32) | operandoPilha2;
    
    memcpy(&operando2, &operandoSub2, sizeof(double));
    
    sub = operando2 - operando1;
    
    memcpy(&subLong, &sub, sizeof(u8));
    
    operandoPilha1 = (u4) ((subLong & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) subLong & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
}

void imul(Environment* environment){
    
    u4 operando1 = 0;
    u4 operando2 = 0;
    u4 mul = 0;
    
    operando1 = popFromOperandStack(environment->thread);
    
    operando2 = popFromOperandStack(environment->thread);
    
    mul = operando2 * operando1;
    
    pushInOperandStack(environment->thread, mul);
    
}

void lmul(Environment* environment){
    
    u8 operando1_64 = 0;
    u8 operando2_64 = 0;
    u8 mul64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando1_64 = (u8) operandoPilha1;
    operando1_64 = (u8) (operando1_64 << 32) | operandoPilha2;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando2_64 = (u8) operandoPilha1;
    operando2_64 = (u8) (operando2_64 << 32) | operandoPilha2;
    
    mul64 = operando2_64 * operando1_64;
    
    operandoPilha1 = (u4) ((mul64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) mul64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void fmul(Environment* environment){
    
    float operando1 = 0;
    float operando2 = 0;
    float mul = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    u4 mulPilha = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    memcpy(&operando1, &operandoPilha1, sizeof(float));
    memcpy(&operando2, &operandoPilha2, sizeof(float));
    
    mul = operando2 * operando1;
    
    memcpy(&mulPilha, &mul, sizeof(u4));
    
    pushInOperandStack(environment->thread, mulPilha);
    
}

void dmul(Environment* environment){
    
    double operando1 = 0;
    double operando2 = 0;
    double mul = 0;
    
    u4 operandoPilha1;
    u4 operandoPilha2;
    
    u8 operandoMul1;
    u8 operandoMul2;
    u8 mulLong;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operandoMul1 = (u8) operandoPilha1;
    operandoMul1 = (u8) (operandoMul1 << 32) | operandoPilha2;
    
    memcpy(&operando1, &operandoMul1, sizeof(double));
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operandoMul2 = (u8) operandoPilha1;
    operandoMul2 = (u8) (operandoMul2 << 32) | operandoPilha2;
    
    memcpy(&operando2, &operandoMul2, sizeof(double));
    
    mul = operando2 * operando1;
    
    memcpy(&mulLong, &mul, sizeof(u8));
    
    operandoPilha1 = (u4) ((mulLong & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) mulLong & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
}

void idiv(Environment* environment){
    
    u4 operando1 = 0;
    u4 operando2 = 0;
    u4 div = 0;
    
    operando1 = popFromOperandStack(environment->thread);
    
    operando2 = popFromOperandStack(environment->thread);
    
    if (operando1 == 0) {
        printf("ERRO nao pode ser feita divisao por zero\n");
    }else{
        div = operando2 / operando1;
    }
    
    pushInOperandStack(environment->thread, div);
    
}

void Ldiv(Environment* environment){
    
    u8 operando1_64 = 0;
    u8 operando2_64 = 0;
    u8 div64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando1_64 = (u8) operandoPilha1;
    operando1_64 = (u8) (operando1_64 << 32) | operandoPilha2;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando2_64 = (u8) operandoPilha1;
    operando2_64 = (u8) (operando2_64 << 32) | operandoPilha2;
    
    if (operando1_64 == 0) {
        printf("ERRO nao pode ser feita divisao por zero/n");
    }else{
        div64 = operando2_64 / operando1_64;
    }
    operandoPilha1 = (u4) ((div64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) div64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void fdiv(Environment* environment){
    
    float operando1 = 0;
    float operando2 = 0;
    float div = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    u4 divPilha = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    memcpy(&operando1, &operandoPilha1, sizeof(float));
    memcpy(&operando2, &operandoPilha2, sizeof(float));
    
    if (operando1 == 0) {
        printf("ERRO nao pode ser feita divisao por zero/n");
    }else{
        div = operando2 / operando1;
    }
    memcpy(&divPilha, &div, sizeof(u4));
    
    pushInOperandStack(environment->thread, divPilha);
    
}

void ddiv(Environment* environment){
    
    double operando1 = 0;
    double operando2 = 0;
    double div = 0;
    
    u4 operandoPilha1;
    u4 operandoPilha2;
    
    u8 operandoDiv1;
    u8 operandoDiv2;
    u8 divLong;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operandoDiv1 = (u8) operandoPilha1;
    operandoDiv1 = (u8) (operandoDiv1 << 32) | operandoPilha2;
    
    memcpy(&operando1, &operandoDiv1, sizeof(double));
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operandoDiv2 = (u8) operandoPilha1;
    operandoDiv2 = (u8) (operandoDiv2 << 32) | operandoPilha2;
    
    memcpy(&operando2, &operandoDiv2, sizeof(double));
    
    if(operando1 == 0){
        printf("ERRO nao pode ser feita divisao por zero/n");
    }else{
        div = operando2 / operando1;
    }
    memcpy(&divLong, &div, sizeof(u8));
    
    operandoPilha1 = (u4) ((divLong & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) divLong & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
}

void irem(Environment* environment){
    
    u4 operando1 = 0;
    u4 operando2 = 0;
    u4 rem = 0;
    
    operando1 = popFromOperandStack(environment->thread);
    
    operando2 = popFromOperandStack(environment->thread);
    
    rem = operando2 % operando1;
    
    pushInOperandStack(environment->thread, rem);
    
}

void lrem(Environment* environment){
    
    u8 operando1_64 = 0;
    u8 operando2_64 = 0;
    u8 rem64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando1_64 = (u8) operandoPilha1;
    operando1_64 = (u8) (operando1_64 << 32) | operandoPilha2;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando2_64 = (u8) operandoPilha1;
    operando2_64 = (u8) (operando2_64 << 32) | operandoPilha2;
    
    rem64 = operando2_64 % operando1_64;
    
    operandoPilha1 = (u4) ((rem64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) rem64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void frem(Environment* environment){
    
    float operando1 = 0;
    float operando2 = 0;
    float rem = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    u4 remPilha = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    memcpy(&operando1, &operandoPilha1, sizeof(float));
    memcpy(&operando2, &operandoPilha2, sizeof(float));
    
    rem = fmodf(operando2, operando1);
    
    memcpy(&remPilha, &rem, sizeof(u4));
    
    pushInOperandStack(environment->thread, remPilha);
    
}

void Drem(Environment* environment){
    
    double operando1 = 0;
    double operando2 = 0;
    double rem = 0;
    
    u4 operandoPilha1;
    u4 operandoPilha2;
    
    u8 operandoRem1;
    u8 operandoRem2;
    u8 remLong;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operandoRem1 = (u8) operandoPilha1;
    operandoRem1 = (u8) (operandoRem1 << 32) | operandoPilha2;
    
    memcpy(&operando1, &operandoRem1, sizeof(double));
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operandoRem2 = (u8) operandoPilha1;
    operandoRem2 = (u8) (operandoRem2 << 32) | operandoPilha2;

    memcpy(&operando2, &operandoRem2, sizeof(double));
    
    rem = fmod(operando2, operando1);
    
    memcpy(&remLong, &rem, sizeof(u8));
    
    operandoPilha1 = (u4) ((remLong & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) remLong & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
}

void ineg(Environment* environment){
    
    u4 operando = 0;
    u4 neg = 0;
    
    operando = popFromOperandStack(environment->thread);
    
    neg = ~operando + 1;
    
    pushInOperandStack(environment->thread, neg);
    
}

void lneg(Environment* environment){
    
    u8 operando_64 = 0;
    u8 neg64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando_64 = (u8) operandoPilha1;
    operando_64 = (u8) (operando_64 << 32) | operandoPilha2;
    
    neg64 = ~operando_64 + 1;
    
    operandoPilha1 = (u4) ((neg64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) neg64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void fneg(Environment* environment){
    
    float operando = 0;
    float neg = 0;
    
    u4 operandoPilha1 = 0;
    u4 negPilha = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    
    memcpy(&operando, &operandoPilha1, sizeof(float));
    
    neg = -(operando);
    
    memcpy(&negPilha, &neg, sizeof(u4));
    
    pushInOperandStack(environment->thread, negPilha);
    
}

void dneg(Environment* environment){
    
    double operando = 0;
    double neg = 0;
    
    u4 operandoPilha1;
    u4 operandoPilha2;
    
    u8 operandoNeg;
    u8 negLong;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operandoNeg = (u8) operandoPilha1;
    operandoNeg = (u8) (operandoNeg << 32) | operandoPilha2;
    
    memcpy(&operando, &operandoNeg, sizeof(double));
    
    neg = -(operando);
    
    memcpy(&negLong, &neg, sizeof(u8));
    
    operandoPilha1 = (u4) ((negLong & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) negLong & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
}

void ishl(Environment* environment){
    
    u4 operando1 = 0;
    u4 operando2 = 0;
    u4 shl = 0;
    
    operando1 = popFromOperandStack(environment->thread);
    
    operando2 = popFromOperandStack(environment->thread);
    
    operando1 = operando1 & SHIFT_MASK_32;
    shl = operando2 << operando1;
    
    pushInOperandStack(environment->thread, shl);
    
}

void lshl(Environment* environment){
    
    u8 operando1_64 = 0;
    u8 operando2_64 = 0;
    u8 shl64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    //operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando1_64 = (u8) operandoPilha1;
    //operando1_64 = (u8) (operando1_64 << 32) | operandoPilha2;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando2_64 = (u8) operandoPilha1;
    operando2_64 = (u8) (operando2_64 << 32) | operandoPilha2;
    
    operando1_64 = operando1_64 & SHIFT_MASK_32;
    shl64 = operando2_64 << operando1_64;
    
    operandoPilha1 = (u4) ((shl64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) shl64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void ishr(Environment* environment){
    
    u4 operando1 = 0;
    u4 operando2 = 0;
    u4 shr = 0;
    
    operando1 = popFromOperandStack(environment->thread);
    
    operando2 = popFromOperandStack(environment->thread);
    
    operando1 = operando1 & SHIFT_MASK_32;
    shr = operando2 >> operando1;
    
    pushInOperandStack(environment->thread, shr);
    
}

void lshr(Environment* environment){
    
    u8 operando1_64 = 0;
    u8 operando2_64 = 0;
    u8 shr64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    //operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando1_64 = (u8) operandoPilha1;
    //operando1_64 = (u8) (operando1_64 << 32) | operandoPilha2;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando2_64 = (u8) operandoPilha1;
    operando2_64 = (u8) (operando2_64 << 32) | operandoPilha2;
    
    operando1_64 = operando1_64 & SHIFT_MASK_32;
    shr64 = operando2_64 >> operando1_64;
    
    operandoPilha1 = (u4) ((shr64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) shr64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void iushr(Environment* environment){
    
    u4 operando1 = 0;
    u4 operando2 = 0;
    u4 shr = 0;
    
    operando1 = popFromOperandStack(environment->thread);
    
    operando2 = popFromOperandStack(environment->thread);
    
    operando1 = operando1 & SHIFT_MASK_32;
    
    if(operando2 > 0){
        shr = operando2 >> operando1;
    }else{
        //copiado da especificacao
        shr = ((operando2 >> operando1) - (2 << ~operando1));
    }
    
    pushInOperandStack(environment->thread, shr);
    
}

void lushr(Environment* environment){
    
    u8 operando1_64 = 0;
    u8 operando2_64 = 0;
    u8 shr64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    //operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando1_64 = (u8) operandoPilha1;
    //operando1_64 = (u8) (operando1_64 << 32) | operandoPilha2;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando2_64 = (u8) operandoPilha1;
    operando2_64 = (u8) (operando2_64 << 32) | operandoPilha2;
    
    operando1_64 = operando1_64 & SHIFT_MASK_32;
    
    if (operando1_64 > 0) {
        shr64 = operando2_64 >> operando1_64;
    }else{
        shr64 = ((operando2_64 >> operando1_64) - (2 << ~operando1_64));
    }
    operandoPilha1 = (u4) ((shr64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) shr64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void iand(Environment* environment){
    
    u4 operando1 = 0;
    u4 operando2 = 0;
    u4 and = 0;
    
    operando1 = popFromOperandStack(environment->thread);
    
    operando2 = popFromOperandStack(environment->thread);
    
    and = operando2 & operando1;
    
    pushInOperandStack(environment->thread, and);
    
}

void land(Environment* environment){
    
    u8 operando1_64 = 0;
    u8 operando2_64 = 0;
    u8 and64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando1_64 = (u8) operandoPilha1;
    operando1_64 = (u8) (operando1_64 << 32) | operandoPilha2;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando2_64 = (u8) operandoPilha1;
    operando2_64 = (u8) (operando2_64 << 32) | operandoPilha2;
    
    and64 = operando2_64 & operando1_64;
    
    operandoPilha1 = (u4) ((and64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) and64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void ior(Environment* environment){
    
    u4 operando1 = 0;
    u4 operando2 = 0;
    u4 or = 0;
    
    operando1 = popFromOperandStack(environment->thread);
    
    operando2 = popFromOperandStack(environment->thread);
    
    or = operando2 | operando1;
    
    pushInOperandStack(environment->thread, or);
    
}

void lor(Environment* environment){
    
    u8 operando1_64 = 0;
    u8 operando2_64 = 0;
    u8 or64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando1_64 = (u8) operandoPilha1;
    operando1_64 = (u8) (operando1_64 << 32) | operandoPilha2;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando2_64 = (u8) operandoPilha1;
    operando2_64 = (u8) (operando2_64 << 32) | operandoPilha2;
    
    or64 = operando2_64 | operando1_64;
    
    operandoPilha1 = (u4) ((or64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) or64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void ixor(Environment* environment){
    
    u4 operando1 = 0;
    u4 operando2 = 0;
    u4 xor = 0;
    
    operando1 = popFromOperandStack(environment->thread);
    
    operando2 = popFromOperandStack(environment->thread);
    
    xor = operando2 ^ operando1;
    
    pushInOperandStack(environment->thread, xor);
    
}

void lxor(Environment* environment){
    
    u8 operando1_64 = 0;
    u8 operando2_64 = 0;
    u8 xor64 = 0;
    
    u4 operandoPilha1 = 0;
    u4 operandoPilha2 = 0;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando1_64 = (u8) operandoPilha1;
    operando1_64 = (u8) (operando1_64 << 32) | operandoPilha2;
    
    operandoPilha1 = popFromOperandStack(environment->thread);
    operandoPilha2 = popFromOperandStack(environment->thread);
    
    operando2_64 = (u8) operandoPilha1;
    operando2_64 = (u8) (operando2_64 << 32) | operandoPilha2;
    
    xor64 = operando2_64 ^ operando1_64;
    
    operandoPilha1 = (u4) ((xor64 & 0xFFFFFFFF00000000) >> 32);
    operandoPilha2 = (u4) xor64 & 0x00000000FFFFFFFF;
    
    pushInOperandStack(environment->thread, operandoPilha2);
    pushInOperandStack(environment->thread, operandoPilha1);
    
}

void iinc(Environment* environment){
    u1 index;
    u1 Const;
    
    environment->thread->PC++;
    
    index = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    
    environment->thread->PC++;
    
    Const = getByteCodeFromMethod(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread->PC);
    
    //environment->thread->PC++;
    
    char constSemSinal = (char) Const;
    
    environment->thread->vmStack->top->localVariablesVector[index] += constSemSinal;
}

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Manipulacao de objetos e chamadas de metodos
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que calcula un indice para o pool de constantes baseado em dois bytes obtidos do codigo,
 * atualizando o valor de PC.
 *
 * \param method Referencia para o metodo contendo o codigo
 * \param method Referencia para o pool de constantes do metodo
 * \param method Referencia para o PC da thread atual
 * \return Indice calculado como byte1 << 8 | byte2
 */
u2 calculatePoolIndexFromCode(method_info* method, cp_info* constant_pool, Thread* thread){
    // Obtemos o primeiro byte argumento para o indice no pool de cte
    thread->PC++;
    u2 index = getByteCodeFromMethod(method, constant_pool, thread->PC);
    // Obtemos o segundo byte argumento
    thread->PC++;
    index = index << 8 | getByteCodeFromMethod(method,
                                              constant_pool,
                                               thread->PC);
    return index;
}


//--------------------------------------------------------------------------------------------------
void getstatic(Environment* environment){
    
    char* attribute_name;
    char* class_name;
    char* attribute_descriptor;
    
    JavaClass* actual_class = environment->thread->vmStack->top->javaClass;
    method_info* actual_method = environment->thread->vmStack->top->method_info;
   
    //Calculamos o indice do fieldRef no pool
    u2 index = calculatePoolIndexFromCode(actual_method,
                                          actual_class->arqClass->constant_pool,
                                          environment->thread);
    
    //Obtemos os nomes da classe, o nome e descritor do field
    getFieldOrMethodInfoAttributesFromConstantPool(index,
                                           actual_class->arqClass->constant_pool,
                                           &class_name, &attribute_name, &attribute_descriptor);
    
    //VERIFICACAO DE CAMPO DE BIBLIOTECA JAVA
    if (javaLibIsFrom(class_name)) {
        javaLibGetStatic(class_name, attribute_name, attribute_descriptor, environment);
        return;
    }
    
    //TODO: Verifica se campo eh estatico
    
    //Verificamos se eh de 32 ou 64 bits
    if (strcmp(attribute_descriptor, "J") == 0 || strcmp(attribute_descriptor, "D") == 0) {
        u8* value_reference = getClassAttributeReference(class_name, attribute_name, environment);
        u8 value = *value_reference;
        
        u4 high_bytes = value >> 32;
        u8 low_bytes = value & 0xFFFFFFFF;
        pushInOperandStack(environment->thread, (u4) low_bytes);
        pushInOperandStack(environment->thread, (u4) high_bytes);
    }
    else{
        //Se eh de 8bits
        if (strcmp(attribute_descriptor, "B") == 0 || strcmp(attribute_descriptor, "C") == 0 ||
            strcmp(attribute_descriptor, "Z") == 0){
            
            u1* value_reference = getClassAttributeReference(class_name, attribute_name, environment);
            u1 value = *value_reference;
            pushInOperandStack(environment->thread, (u4) value);
        }
        
        //Se eh de 16bits
        else if (strcmp(attribute_descriptor, "S") == 0){
            u2* value_reference = getClassAttributeReference(class_name, attribute_name, environment);
            u2 value = *value_reference;
            pushInOperandStack(environment->thread, (u4) value);
        }
        //Se eh de 32bits
        else {
            u4* value_reference = getClassAttributeReference(class_name, attribute_name, environment);
            u4 value = *value_reference;
            pushInOperandStack(environment->thread, value);
        }
    }
}


//--------------------------------------------------------------------------------------------------
void putstatic(Environment* environment){
    
    char* attribute_name;
    char* class_name;
    char* attribute_descriptor;
    
    JavaClass* actual_class = environment->thread->vmStack->top->javaClass;
    method_info* actual_method = environment->thread->vmStack->top->method_info;
    
    //Calculamos o indice do fieldRef no pool
    u2 index = calculatePoolIndexFromCode(actual_method,
                                          actual_class->arqClass->constant_pool,
                                          environment->thread);
    
    //Obtemos os nomes da classe, o nome e descritor do field
    getFieldOrMethodInfoAttributesFromConstantPool(index,
                                           actual_class->arqClass->constant_pool,
                                           &class_name, &attribute_name, &attribute_descriptor);
    
    
    //VERIFICACAO DE CAMPO DE BIBLIOTECA JAVA
    if (javaLibIsFrom(class_name)) return;
    
    //TODO: Verifica se campo eh estatico
    
    //Verificamos se eh de 64 bits
    if (strcmp(attribute_descriptor, "J") == 0 || strcmp(attribute_descriptor, "D") == 0) {
        u8* value_reference = getClassAttributeReference(class_name, attribute_name, environment);
        if (value_reference == NULL) JVMThrow(NullPointerException, environment);
        u8 value;
        
        //Obtemos e concatenamos os bytes
        value = popFromOperandStack(environment->thread);
        u8 lowBytes = popFromOperandStack(environment->thread);
        value = value << 32 | lowBytes;
        
        //Atualizamos o campo
        *value_reference = value;
    }

    else{
        
        //Obtemos o valor
        u4 value = popFromOperandStack(environment->thread);
        
        
        //Se eh de 8bits
        if (strcmp(attribute_descriptor, "B") == 0 || strcmp(attribute_descriptor, "C") == 0 ||
                 strcmp(attribute_descriptor, "Z") == 0){
            
            u1* value_reference = getClassAttributeReference(class_name, attribute_name, environment);
            if (value_reference == NULL) JVMThrow(NullPointerException, environment);
            *value_reference = (u1) value;
        }

        //Se eh de 16bits
        else if (strcmp(attribute_descriptor, "S") == 0){
            u2* value_reference = getClassAttributeReference(class_name, attribute_name, environment);
            if (value_reference == NULL) JVMThrow(NullPointerException, environment);
            *value_reference = (u2) value;
        }
        //Se eh de 32bits
        else {
         u4* value_reference = getClassAttributeReference(class_name, attribute_name, environment);
            if (value_reference == NULL) JVMThrow(NullPointerException, environment);
            *value_reference = value;
        }
    }

}



//--------------------------------------------------------------------------------------------------
void getfield(Environment* environment){
    
    char* attribute_name;
    char* class_name;
    char* attribute_descriptor;
    
    JavaClass* actual_class = environment->thread->vmStack->top->javaClass;
    method_info* actual_method = environment->thread->vmStack->top->method_info;
    
    //Obtemos a referencia para o objeto
    Object* objectRef = (Object*) popFromOperandStack(environment->thread);
    
    //Calculamos o indice do fieldRef no pool
    u2 index = calculatePoolIndexFromCode(actual_method,
                                          actual_class->arqClass->constant_pool,
                                          environment->thread);
    
    //Obtemos os nomes da classe, o nome e descritor do field
    getFieldOrMethodInfoAttributesFromConstantPool(index,
                                           actual_class->arqClass->constant_pool,
                                           &class_name, &attribute_name, &attribute_descriptor);
    
    //VERIFICACAO DE CAMPO DE BIBLIOTECA JAVA
    if (javaLibIsFrom(class_name)) return;
    
    //Verificamos se eh de 32 ou 64 bits
    if (strcmp(attribute_descriptor, "J") == 0 || strcmp(attribute_descriptor, "D") == 0) {
        u8* value_reference = getObjectAttributeReference(objectRef, attribute_name);
        u8 value = *value_reference;
        
        u4 high_bytes = value >> 32;
        u4 low_bytes = value & 0xFFFFFFFF;
        pushInOperandStack(environment->thread, (u4)low_bytes);
        pushInOperandStack(environment->thread, (u4)high_bytes);
    }
    else{
        //Se eh de 8bits
        if (strcmp(attribute_descriptor, "B") == 0 || strcmp(attribute_descriptor, "C") == 0 ||
            strcmp(attribute_descriptor, "Z") == 0){
            
            u1* value_reference = getObjectAttributeReference(objectRef, attribute_name);
            u1 value = *value_reference;
            pushInOperandStack(environment->thread, (u4) value);
        }
        
        //Se eh de 16bits
        else if (strcmp(attribute_descriptor, "S") == 0){
            u2* value_reference = getObjectAttributeReference(objectRef, attribute_name);
            u2 value = *value_reference;
            pushInOperandStack(environment->thread, (u4) value);
        }
        //Se eh de 32bits
        else {
            u4* value_reference = getObjectAttributeReference(objectRef, attribute_name);   
            u4 value = *value_reference;
            pushInOperandStack(environment->thread, value);
        }
    }
}


//--------------------------------------------------------------------------------------------------
void putfield(Environment* environment){
    
    char* attribute_name;
    char* class_name;
    char* attribute_descriptor;
    
    JavaClass* actual_class = environment->thread->vmStack->top->javaClass;
    method_info* actual_method = environment->thread->vmStack->top->method_info;
    
    //Calculamos o indice do fieldRef no pool
    u2 index = calculatePoolIndexFromCode(actual_method,
                                          actual_class->arqClass->constant_pool,
                                          environment->thread);
    
    //Obtemos os nomes da classe, o nome e descritor do field
    getFieldOrMethodInfoAttributesFromConstantPool(index,
                                           actual_class->arqClass->constant_pool,
                                           &class_name, &attribute_name, &attribute_descriptor);
    
    
    //VERIFICACAO DE CAMPO DE BIBLIOTECA JAVA
    if (javaLibIsFrom(class_name)) return;
    
    //TODO: Verifica se campo eh estatico
    
    //Verificamos se eh de 32 ou 64 bits
    if (strcmp(attribute_descriptor, "J") == 0 || strcmp(attribute_descriptor, "D") == 0) {
        //Obtemos e concatenamos os bytes
        u8 value;
        value = popFromOperandStack(environment->thread);
        value = value << 32 | popFromOperandStack(environment->thread);
        //Obtemos a referencia para o objeto
        Object* objectRef = (Object*) popFromOperandStack(environment->thread);
        
        u8* value_reference = getObjectAttributeReference(objectRef, attribute_name);
        if (value_reference == NULL) JVMThrow(NullPointerException, environment);
        //Atualizamos o campo
        *value_reference = value;
    }
    else{
        //Obtemos o valor
        u4 value = popFromOperandStack(environment->thread);
        Object* objectRef = (Object*) popFromOperandStack(environment->thread);
        
        //Se eh de 8bits
        if (strcmp(attribute_descriptor, "B") == 0 || strcmp(attribute_descriptor, "C") == 0 ||
            strcmp(attribute_descriptor, "Z") == 0){
            
            u1* value_reference = getObjectAttributeReference(objectRef, attribute_name);
            if (value_reference == NULL) JVMThrow(NullPointerException, environment);
            *value_reference = (u1) value;
        }
        
        //Se eh de 16bits
        else if (strcmp(attribute_descriptor, "S") == 0){
            u2* value_reference = getObjectAttributeReference(objectRef, attribute_name);
            if (value_reference == NULL) JVMThrow(NullPointerException, environment);
            *value_reference = (u2) value;
        }
        //Se eh de 32bits
        else {
            u4* value_reference = getObjectAttributeReference(objectRef, attribute_name);
            if (value_reference == NULL) JVMThrow(NullPointerException, environment);
            *value_reference = value;
        }
    }
}

//--------------------------------------------------------------------------------------------------
/*! Metodo que retorna a quanitdade de parametros (de U4s) sao passados como parametro de um 
 *  desctritor de metodo.
 * 
 * \param descriptor Descritor do metodo
 *
 */
int getParameterNumberFromMethodDescriptor(char* descriptor){
    
    int nParams = 0;
    
    if (descriptor[0] != '(') return -1;
    
    //Pulamos o '(' e procuramos o numero de parametros
    for (int i = 1; i < strlen(descriptor); i++) {
        //Fim de parametros
        if (descriptor[i] == ')') break;
        
        //Se for L<Name>;
        if (descriptor[i] == 'L') {
            while (descriptor[i]!=';'){
                i++; //Procuramos o ';'
                if (i > strlen(descriptor)) return -1;
            }
            nParams++;
        }
        //Se for long ou double
        else if (descriptor[i] == 'J' || descriptor[i] == 'D') nParams += 2;
        //Se forem de 32bits
        else nParams ++;
    }
    return nParams;
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que verifica se uma classe eh subclasse de outra
 *
 * \param className Nome da classe a ser verificada
 * \param supClassName Nome da possivel superclasse
 * \param environment Ambiente de execucao
 * \return 1 se sim, 0 se nao
 */
int isClassSubClassFromClass(char* className, char* supClassName, Environment* environment){
    
    if(strcmp(className, supClassName) == 0) return 1;
    
    JavaClass* class = getClass(className, environment);
    char* superName = getClassNameFromConstantPool(class->arqClass->constant_pool, class->arqClass->super_class);
    
    if (javaLibIsFrom(superName)) return 0;
    else isClassSubClassFromClass(superName, supClassName, environment);
    
    return 0;
}

//--------------------------------------------------------------------------------------------------
/*!
 *  Metodo que verifica se existe o metodo existe na classe ou superclasse de uma classe passada e
 * retorna uma referencia para o method_info e o nome da classe do metodo eh copiada para o 
 * parametro method_class_name
 *
 * \param objectClass Referencia para a classe do objeto
 * \param method_name nome do metodo atual
 * \param method_descriptor descritor do metodo atual
 * \param environment Ambiente de execucao
 * \param method_class_name Nome da classe do metodo encontrado
 * \return method_info do metodo, se nao lanca excessao
 */
method_info* isMethodInClassOrSuperClass(JavaClass* objectClass, char* method_name, char* method_descriptor, Environment* environment, char** method_class_name){
    
    method_info* method = getMethodInfoFromClass(objectClass, method_name, method_descriptor);
    if (method){
        *method_class_name = getClassNameFromConstantPool(objectClass->arqClass->constant_pool,
                                     objectClass->arqClass->this_class);
        return method;
    }
    
    char* superClassName = getClassNameFromConstantPool(objectClass->arqClass->constant_pool,
                                                        objectClass->arqClass->super_class);
    
    if (javaLibIsFrom(superClassName)) {
        JVMThrow(AbstractMethodError, environment);
        return NULL;
    }
    //Verificamos recursivamente
    else return isMethodInClassOrSuperClass(getClass(superClassName, environment), method_name, method_descriptor, environment, method_class_name);
}


//--------------------------------------------------------------------------------------------------
void invokevirtual(Environment* environment){
    
    char* method_name;
    char* class_name;
    char* method_descriptor;
    
    JavaClass* actual_class = environment->thread->vmStack->top->javaClass;
    method_info* actual_method = environment->thread->vmStack->top->method_info;
    
    //Calculamos o indice do method_info no pool
    u2 index = calculatePoolIndexFromCode(actual_method,
                                          actual_class->arqClass->constant_pool,
                                          environment->thread);
    
    //2. Resolvemos o nome do metodo, obtendo a referencia do method_info
    getFieldOrMethodInfoAttributesFromConstantPool(index,
                                                   actual_class->arqClass->constant_pool,
                                                   &class_name, &method_name, &method_descriptor);
    
    //VERIFICACAO DE METODO DE BIBLIOTECA JAVA
    if (javaLibIsFrom(class_name)){
        javaLibExecuteMethod(class_name, method_name, method_descriptor, environment);
        return;
    }
    
    //4. Baseado no descritor do metodo, desempilhamos os parametros
    //Obtemos a quantidade de parametros a serem desempilhados
    int nParams = getParameterNumberFromMethodDescriptor(method_descriptor);
    //Criamos um vetor de u4 para os parametros
    u4* params = (u4*) malloc(nParams*sizeof(u4));
    //Desempilhamos os parametros e salvamos no vetor
    for (int i = 0; i < nParams; i++) params[i] = popFromOperandStack(environment->thread);
    
    //5. Desempilhamos uma referencia para o objeto (Objectref)
    Object* objectRef = (Object*) popFromOperandStack(environment->thread);
    if (objectRef == NULL) JVMThrow(NullPointerException, environment);
    

    //6. Buscamos o metodo recursivamente na classe e superclasses do objeto
    method_info* method = isMethodInClassOrSuperClass(objectRef->handler->javaClass, method_name, method_descriptor, environment, &class_name);
    
    if ((method->access_flags & ACC_ABSTRACT)) JVMThrow(AbstractMethodError, environment);
    
    //7. Criamos um novo frame e empilhamos
    Frame* newFrame = pushFrame(environment, class_name, method_name, method_descriptor);
    environment->thread->PC--; //Pc é colocado para -1 devido ao incremento do interpretador
    
    //7.1. Passamos os argumentos para o vetor de variaveis locais
    newFrame->localVariablesVector[0] = (u4) objectRef;
    for (int i = 0; i < nParams; i++) newFrame->localVariablesVector[i+1] = params[i];
}


//--------------------------------------------------------------------------------------------------
/*!
 * O metodo eh selecionado para invocacao a nao ser que todas as seguintes condicoes sejam
 *    verdadeiras:
 *
 *      7.1. A flag ACC_SUPER esta setada para a classe atual
 *      7.2. A classe do metodo eh superclasse da classe atual
 *      7.3. O metodo nao eh um metodo <init>
 *
 * \param object Referencia para o objeto
 * \param method_class Referencia para a classe do metodo invocado
 * \param method_name nome do metodo atual
 * \param method_descriptor descritor do metodo atual
 * \param environment Ambiente de execucao
 */
void verifyInvokeSpecial(Object* objectRef, JavaClass* method_class, char* method_name,
                         char*method_descriptor, Environment* environment){
    
    JavaClass* actual_class = environment->thread->vmStack->top->javaClass;
    char* actual_class_name = getClassNameFromConstantPool(actual_class->arqClass->constant_pool,
                                                           actual_class->arqClass->this_class);
    char* method_class_name = getClassNameFromConstantPool(method_class->arqClass->constant_pool,
                                                           method_class->arqClass->this_class);
    
    int isSuper = actual_class->arqClass->access_flags & ACC_SUPER;
    
    int isSub = isClassSubClassFromClass(actual_class_name, method_class_name, environment);
    
    int isInit = strcmp(method_name, "<init>") == 0;
    
    if (!(isSuper && isSub && isInit)) return;
}


//--------------------------------------------------------------------------------------------------
void invokespecial(Environment* environment){
    
    char* method_name;
    char* class_name;
    char* method_descriptor;
    
    JavaClass* actual_class = environment->thread->vmStack->top->javaClass;
    method_info* actual_method = environment->thread->vmStack->top->method_info;
    
    //Calculamos o indice do method_info no pool
    u2 index = calculatePoolIndexFromCode(actual_method,
                                          actual_class->arqClass->constant_pool,
                                          environment->thread);
    
    //2. Resolvemos o nome do metodo, obtendo a referencia do method_info
    getFieldOrMethodInfoAttributesFromConstantPool(index,
                                                   actual_class->arqClass->constant_pool,
                                                   &class_name, &method_name, &method_descriptor);
    
    //VERIFICACAO DE METODO DE BIBLIOTECA JAVA
    if (javaLibIsFrom(class_name)){
        javaLibExecuteMethod(class_name, method_name, method_descriptor, environment);
        return;
    }
    
    //1. Resolvemos o a classe do metodo, obtendo uma referencia para javaClass
    JavaClass* method_class = getClass(class_name, environment);
    
    
    //4. Baseado no descritor do metodo, desempilhamos os parametros
    
    //Obtemos a quantidade de parametros a serem desempilhados
    int nParams = getParameterNumberFromMethodDescriptor(method_descriptor);
    
    //Criamos um vetor de u4 para os parametros
    u4* params = (u4*) malloc(nParams*sizeof(u4));
    
    //Desempilhamos os parametros e salvamos no vetor
    for (int i = 0; i < nParams; i++) params[i] = popFromOperandStack(environment->thread);
    
    
    //5. Desempilhamos uma referencia para o objeto (Objectref)
    Object* objectRef = (Object*) popFromOperandStack(environment->thread);
    if (objectRef == NULL) JVMThrow(NullPointerException, environment);
    
    
    //6. Verificacoes de invokespecial
    verifyInvokeSpecial(objectRef, method_class, method_name, method_descriptor, environment);
    
    
    //Buscamos o metodo recursivamente na classe e superclasses do objeto
    method_info* method = isMethodInClassOrSuperClass(objectRef->handler->javaClass, method_name, method_descriptor, environment, &class_name);
    
    
    if ((method->access_flags & ACC_STATIC)) JVMThrow(IncompatibleClassChangeError, environment);
    if ((method->access_flags & ACC_ABSTRACT)) JVMThrow(AbstractMethodError, environment);
    
    
    //7. Criamos um novo frame e empilhamos
    Frame* newFrame = pushFrame(environment, class_name, method_name, method_descriptor);
    environment->thread->PC--; //Pc é colocado para -1 devido ao incremento do interpretador
    
    //7.1. Passamos os argumentos para o vetor de variaveis locais
    newFrame->localVariablesVector[0] = (u4) objectRef;
    for (int i = 0; i < nParams; i++) newFrame->localVariablesVector[i+1] = params[i];
}


//--------------------------------------------------------------------------------------------------
void invokestatic(Environment* environment){
    
    char* method_name;
    char* class_name;
    char* method_descriptor;
    
    JavaClass* actual_class = environment->thread->vmStack->top->javaClass;
    method_info* actual_method = environment->thread->vmStack->top->method_info;
    
    //Calculamos o indice do method_info no pool
    u2 index = calculatePoolIndexFromCode(actual_method,
                                          actual_class->arqClass->constant_pool,
                                          environment->thread);
    
    //2. Resolvemos o nome do metodo, obtendo a referencia do method_info
    getFieldOrMethodInfoAttributesFromConstantPool(index,
                                                   actual_class->arqClass->constant_pool,
                                                   &class_name, &method_name, &method_descriptor);
    
    //VERIFICACAO DE METODO DE BIBLIOTECA JAVA
    if (javaLibIsFrom(class_name)){
        javaLibExecuteMethod(class_name, method_name, method_descriptor, environment);
        return;
    }
    
    //1. Resolvemos o a classe do metodo, obtendo uma referencia para javaClass
    JavaClass* method_class = getClass(class_name, environment);
    
    
    //4. Baseado no descritor do metodo, desempilhamos os parametros
    //Obtemos a quantidade de parametros a serem desempilhados
    int nParams = getParameterNumberFromMethodDescriptor(method_descriptor);
    //Criamos um vetor de u4 para os parametros
    u4* params = (u4*) malloc(nParams*sizeof(u4));
    //Desempilhamos os parametros e salvamos no vetor
    for (int i = 0; i < nParams; i++) params[i] = popFromOperandStack(environment->thread);
    
    
    //Verificamos se o metodo eh estatico
    method_info* method = getMethodInfoFromClass(method_class, method_name, method_descriptor);
    if ((method->access_flags & ACC_STATIC) == 0) JVMThrow(IncompatibleClassChangeError, environment);
    
    
    //7. Criamos um novo frame e empilhamos
    Frame* newFrame = pushFrame(environment, class_name, method_name, method_descriptor);
    environment->thread->PC--; //Pc é colocado para -1 devido ao incremento do interpretador
    
    //7.1. Passamos os argumentos para o vetor de variaveis locais
    for (int i = 0; i < nParams; i++) newFrame->localVariablesVector[i] = params[i];
}


//--------------------------------------------------------------------------------------------------
void invokeinterface(Environment* environment){

    char* method_name;
    char* class_name;
    char* method_descriptor;
    
    JavaClass* actual_class = environment->thread->vmStack->top->javaClass;
    method_info* actual_method = environment->thread->vmStack->top->method_info;
    
    //Calculamos o indice do method_info no pool
    u2 index = calculatePoolIndexFromCode(actual_method,
                                          actual_class->arqClass->constant_pool,
                                          environment->thread);
    
    //2. Resolvemos o nome do metodo, obtendo a referencia do method_info
    getFieldOrMethodInfoAttributesFromConstantPool(index,
                                                   actual_class->arqClass->constant_pool,
                                                   &class_name, &method_name, &method_descriptor);
    
    //VERIFICACAO DE METODO DE BIBLIOTECA JAVA
    if (javaLibIsFrom(class_name)){
        javaLibExecuteMethod(class_name, method_name, method_descriptor, environment);
        return;
    }
    
    
    //4. Baseado no descritor do metodo, desempilhamos os parametros
    //Obtemos a quantidade de parametros a serem desempilhados
    int nParams = getParameterNumberFromMethodDescriptor(method_descriptor);
    //Criamos um vetor de u4 para os parametros
    u4* params = (u4*) malloc(nParams*sizeof(u4));
    //Desempilhamos os parametros e salvamos no vetor
    for (int i = 0; i < nParams; i++) params[i] = popFromOperandStack(environment->thread);
    
    
    //5. Desempilhamos uma referencia para o objeto (Objectref)
    Object* objectRef = (Object*) popFromOperandStack(environment->thread);
    
    environment->thread->PC++; //COUNT
    environment->thread->PC++; //Byte 0
    
    if (objectRef == NULL) JVMThrow(NullPointerException, environment);
    //TODO: Se a classe de Objectref nao implementa a interface, IncompatibleClassChangeError
    
    
    //6. Verificacoes de invokeinterface
    
    // 6.2. Se nao, se C tiver uma superclasse, o procedimento de busca eh realizado recursivamente
    //   na superclasse. O metodo a ser invocado eh o resultado do processo de busca recursiva.
    //   6.3. Se nao, AbstractMethodError.
    method_info* method = isMethodInClassOrSuperClass(objectRef->handler->javaClass, method_name, method_descriptor, environment, &class_name);
    
    if ((method->access_flags & ACC_ABSTRACT)) JVMThrow(AbstractMethodError, environment);
    if (!(method->access_flags & ACC_PUBLIC)) JVMThrow(IllegalAccessError, environment);
    
    
    //7. Criamos um novo frame e empilhamos
    //VERIFICAR METODO EM SUPERCLASSES DE FORMA RECURSIVA
    class_name = getClassNameFromConstantPool(objectRef->handler->javaClass->arqClass->constant_pool, objectRef->handler->javaClass->arqClass->this_class);
    Frame* newFrame = pushFrame(environment, class_name, method_name, method_descriptor);
    environment->thread->PC--; //Pc é colocado para -1 devido ao incremento do interpretador
    
    //7.1. Passamos os argumentos para o vetor de variaveis locais
    newFrame->localVariablesVector[0] = (u4) objectRef;
    for (int i = 0; i < nParams; i++) newFrame->localVariablesVector[i+1] = params[i];
}


//--------------------------------------------------------------------------------------------------
void New(Environment* environment){
    
    JavaClass* actual_class = environment->thread->vmStack->top->javaClass;
    method_info* actual_method = environment->thread->vmStack->top->method_info;
    
    //Calculamos o indice para a classe no pool
    u2 index = calculatePoolIndexFromCode(actual_method,
                                          actual_class->arqClass->constant_pool,
                                          environment->thread);
    
    //Obtemos o nome da classe
    char* className = getClassNameFromConstantPool(actual_class->arqClass->constant_pool, index);
    
    //VERIFICACAO DE METODO DE BIBLIOTECA JAVA
    if (javaLibIsFrom(className)){
        javaLibNewObject(className, environment);
        return;
    }
    
    //Criamos uma nova instancia de classe
    Object* instance = newObjectFromClass(className, environment);

    //Empilhamos na pilha de operandos
    pushInOperandStack(environment->thread, (u4) instance);
}


//--------------------------------------------------------------------------------------------------
void newarray(Environment* environment){
    
    void* array;
    int i;
    
    environment->thread->PC++;
    u1 atype_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                                   environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                                   ,environment->thread->PC);
    
    int count = (u4)popFromOperandStack(environment->thread);
    
    if (count < 0) JVMThrow(NegativeArraySizeException, environment);
    
    if (atype_argument == T_BYTE || atype_argument == T_BOOLEAN) {
        array = (u1*) malloc(sizeof(u1) * count);
        for(i = 0; i < count; i++){
            u1* b = (u1*) array+i;
            *b = 0;
        }
    }
    else if (atype_argument == T_CHAR) {
        array = (u1*) malloc(sizeof(u1) * count);
        for(i = 0; i < count; i++){
            u1* b = (u1*) array+i;
            *b = '\0';
        }
    }
    else if (atype_argument == T_SHORT) {
        array = (u2*) malloc(sizeof(u2) * count);
        for(i = 0; i < count; i++){
            u2* s = (u2*) array+i;
            *s = 0;
        }
    }
    else if (atype_argument == T_INT) {
        array = (u4*) malloc(sizeof(u4) * count);
        for(i = 0; i < count; i++){
            u4* i_f = (u4*) array+i;
            *i_f = 0;
        }
    }
    else if (atype_argument == T_FLOAT) {
        array = (u4*) malloc(sizeof(u4) * count);
        for(i = 0; i < count; i++){
            u4* i_f = (u4*) array+i;
            *i_f = 0;
        }
    }
    else if (atype_argument == T_LONG) {
        array = (u8*) malloc(sizeof(u8) * count);
        for(i = 0; i < count; i++){
            u8* l_d  = (u8*) array+i;
            *l_d = 0;
        }
    }
    else if (atype_argument == T_DOUBLE) {
        array = (u8*) malloc(sizeof(u8) * count);
        for(i = 0; i < count; i++){
            double* l_d  = (double*) array+i;
            *l_d = 0.0;
        }
    }
    
    JavaArray* arrayref = newJavaArray(atype_argument, count, array);
    
    pushInOperandStack(environment->thread, (u4) arrayref);

}


//--------------------------------------------------------------------------------------------------
void anewarray(Environment* environment){
    
    int i;
    
    u2 index_result = calculatePoolIndexFromCode(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread);
    
    u4 atype = (u4) getClassNameFromConstantPool(environment->thread->vmStack->top->javaClass->arqClass->constant_pool, index_result);
    
    int count = (u4)popFromOperandStack(environment->thread);
    
    if (count < 0) JVMThrow(NegativeArraySizeException, environment);
    
    u4* array = (u4*) malloc(sizeof(u4) * count);
    
    for(i = 0; i < count; i++){
        array[i] = (u4) NULL;
    }
    
    JavaArray* arrayref = newJavaArray(atype, count, array);
    
    pushInOperandStack(environment->thread, (u4) arrayref);
}


//--------------------------------------------------------------------------------------------------
void arraylength(Environment* environment){
    
    JavaArray* arrayref = (JavaArray*)popFromOperandStack(environment->thread);
    
    if (arrayref->arrayAddress == NULL) JVMThrow(NullPointerException, environment);
    
    pushInOperandStack(environment->thread, arrayref->count);
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que inicializa um array multidimensional.
 *
 * \param total_dimensions Total de dimensoes do array a ser inicializado.
 * \param current_dimension A dimensao atual do array a ser inicializado.
 * \param count O vetor count que possui o quantidade de componentes em cada dimensao do array a ser inicializado.
 * \param posicao A posicao do array em que sera inicializada com o valor inicial padrao do tipo dos componentes.
 * \param type_array O tipo dos componentes do array a ser inicializado.
 * \param array O array a ser inicializado.
 */
void initializeNDArray(int total_dimensions, int current_dimension, int* count ,int posicao,
                     char type_array, void* array){
    int i;
    current_dimension++;
    // Enquanto ainda nao chegar na ultima dimensao, continua na recusividade
    for (i = 0; i < count[current_dimension]; i++) {
        posicao += i*count[current_dimension+1];
        if (current_dimension != total_dimensions-1) {
            initializeNDArray(total_dimensions, current_dimension, count, posicao, type_array, array);
        }
        else {
            if (type_array == 'B' || type_array == 'Z') {
                u1* b = (u1*) array+posicao+i;
                *b = 0;
            }
            else if (type_array == 'C') {
                u1* b = (u1*) array+posicao+i;
                *b = '\0';
            }
            else if (type_array == 'S') {
                u2* s = (u2*) array+posicao+i;
                *s = 0;
            }
            else if (type_array == 'I') {
                u4* i_f = (u4*) array+posicao+i;
                *i_f = 0;
            }
            else if (type_array == 'F') {
                u4* i_f = (u4*) array+posicao+i;
                *i_f = 0.0f;
            }
            else if (type_array == 'J') {
                u8* l_d  = (u8*) array+posicao+i;
                *l_d = 0L;
            }
            else if (type_array == 'D') {
                u8* l_d  = (u8*) array+posicao+i;
                *l_d = 0.0;
            }
            return;
        }
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void multianewarray(Environment* environment){
    
    u2 index_result = calculatePoolIndexFromCode(environment->thread->vmStack->top->method_info, environment->thread->vmStack->top->javaClass->arqClass->constant_pool, environment->thread);
    
    u4 atype = (u4) getClassNameFromConstantPool(environment->thread->vmStack->top->javaClass->arqClass->constant_pool, index_result);
    
    
    
    //TODO: RESOLVER A REFERENCIA SIMBOLICA DOS TIPO CLASS, ARRAY E INTERFACE
    
    //TODO: During resolution of the symbolic reference to the class, array, or interface type, any of the exceptions documented in Section 5.4.3.1 can be thrown.
    
    //TODO: Otherwise, if the current class does not have permission to access the element type of the resolved array class, multianewarray throws an IllegalAccessError.
    
    //TODO: Otherwise, if any of the dimensions values on the operand stack are less than zero, the multianewarray instruction throws a NegativeArraySizeException.
    
    environment->thread->PC++;
    u1 dimensions_argument = getByteCodeFromMethod(environment->thread->vmStack->top->method_info,
                                                   environment->thread->vmStack->top->javaClass->arqClass->constant_pool
                                                   ,environment->thread->PC);
    
    //Vetor que armazena o tamanho de cada dimensao do array
    int *count = (int*) malloc(sizeof(int) * dimensions_argument);
    
    //Desloca-se de todos os caracteres '[' que representam uma dimensao de array, para obter o caracter seguinte, que representara o tipo dos componentes do array.
    char type_components = atype+dimensions_argument;
    
    //O primeiro count a ser desempilhado eh quantidade de componentes na ultima dimensao do array
    for (int i = ((u1)dimensions_argument)-1; i >= 0; i--) {
        
        count[i] = popFromOperandStack(environment->thread);
    }
    
    //Variavel auxiliar para se obter o tamanho total do array
    int total = 1;
    
    //Saira do loop quando for a ultima dimensao a ser alocada, pois n eh o numero de dimensoes menos uma.
    for (int i = 1; i < ((u1)dimensions_argument); i++) {
        //Se count igual a zero, nenhuma dimensao subsequente sera alocada.
        if (count[i] != 0) {
            //Cada dimensao sera multiplicada pela proxima dimensao, obtendo o tamanho total em um componente na primeira dimensao
            total *= count[i];
        }
        else break;
    }
    
    //O array multidimensional
    void* array;
    
    if (type_components == 'B' || type_components == 'Z' || type_components == 'C') {
        array = (u1*) malloc(count[0] * total * sizeof(u1));
        initializeNDArray(dimensions_argument, -1, count, 0, type_components, array);
    }
    else if (type_components == 'S') {
        array = (u2*) malloc(count[0] * total * sizeof(u2));
        initializeNDArray(dimensions_argument, -1, count, 0, type_components, array);
    }
    else if (type_components == 'I' || type_components == 'F') {
        array = (u4*) malloc(count[0] * total * sizeof(u4));
        initializeNDArray(dimensions_argument, -1, count, 0, type_components, array);
    }
    else if (type_components == 'J' || type_components == 'D') {
        array = (u8*) malloc(count[0] * total * sizeof(u8));
        initializeNDArray(dimensions_argument, -1, count, 0, type_components, array);
    }
}

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Retorno de funcoes
//--------------------------------------------------------------------------------------------------

void ireturn(Environment* environment){
    u4 conteudo;
    Frame *tmp;

    tmp = getCurrentFrame(environment->thread);
    conteudo = popFromOperandStack(environment->thread);
    popFrame(environment->thread);

    pushInOperandStack(environment->thread, conteudo);
}

void lreturn(Environment* environment){
    u4 conteudo1, conteudo2;
    Frame *tmp;

    tmp = getCurrentFrame(environment->thread);
    conteudo1 = popFromOperandStack(environment->thread);
    conteudo2 = popFromOperandStack(environment->thread);
    popFrame(environment->thread);

    pushInOperandStack(environment->thread, conteudo2);
    pushInOperandStack(environment->thread, conteudo1);
}

void freturn(Environment* environment){
    u4 conteudo;
    Frame *tmp;

    tmp = getCurrentFrame(environment->thread);
    conteudo = popFromOperandStack(environment->thread);
    popFrame(environment->thread);

    pushInOperandStack(environment->thread, conteudo);
}

void dreturn(Environment* environment){
    u4 conteudo1, conteudo2;
    Frame *tmp;

    tmp = getCurrentFrame(environment->thread);
    conteudo1 = popFromOperandStack(environment->thread);
    conteudo2 = popFromOperandStack(environment->thread);
    popFrame(environment->thread);

    pushInOperandStack(environment->thread, conteudo2);
    pushInOperandStack(environment->thread, conteudo1);
}

void areturn(Environment* environment){
    u4 conteudo;
    Frame *tmp;

    tmp = getCurrentFrame(environment->thread);
    conteudo = popFromOperandStack(environment->thread);
    popFrame(environment->thread);

    pushInOperandStack(environment->thread, conteudo);
}

void return_(Environment* environment){
    Frame *tmp;
    tmp = getCurrentFrame(environment->thread);
    popFrame(environment->thread);
}