//#################################################################################################
/*! \file executionengine.h
 *
 *  \brief Interface do Execution Engine da JVM.
 *
 *  Interface responsavel pelos servicos relacionados ao Execution Engine da JVM, 
 *	contendo os submódulos responsáveis por: 
 *	- Decodificacao de bytecodes em instrucoes
 *	- Tratamento de excessoes lancadas pela JVM
 *	- Interpretador
 *	- Instrucoes da JVM: 
 *	-- Comparacao e desvio
 *	-- Conversao de tipos
 *	-- Transferencia valores entre o vetor de variaveis locais e a pilha de operandos de um frame de um metodo
 *	-- Instrucoes logico atmeticas
 *	-- Manipulacao de objetos e chamadas de metodos
 *	-- Retorno de funcoes
 */
//##################################################################################################

#ifndef EXECENG_h
#define EXECENG_h
#ifdef EXECENG
#define EXTE
#else
#define EXTE extern
#endif

#include "exceptions.h"
#include "estruturas.h"
#include "memoryunit.h"
#include "util.h"

//--------------------------------------------------------------------------------------------------
// Define a mascara utilizada nas funcoes de conversao de tipos
#define SHIFT_MASK_UNSI 0xffffffff 
// Define as mascaras utilizadas nas operacoes logico aritmeticas
#define SHIFT_MASK_32 0x1F
#define SHIFT_MASK_64 0x3F

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Decodificacao de bytecodes em instrucoes
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por retornar um ponteiro para uma funcao que realiza as operacoes de um 
 * bytecode
 *
 * \param bytecode  Bytecode a ser decodificado
 * \return Ponteiro para a instrucao referente ao bytecode 
 */
EXTE instruction decode(u1 bytecode);

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Tratamento de excessoes
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 * Método responsavel por encerrar a JVM em caso de excessoes.
 *
 * \param msg Mensagem de erro a ser exibida
 */
EXTE void JVMstopAbrupt(const char* msg);


//--------------------------------------------------------------------------------------------------
/*!
 * Pelo lancamento e tratamento de excessoes
 *
 * \param exception codigo da excessao
 */
EXTE void JVMThrow(int exception, Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Funcao que imprime informacoes relativas ao frame atual
 *
 * \param frame Frame a ser exibido
 * \param opcode Opcode a ser impresso (NULL caso nao precise)
 */
EXTE void JVMPrintFrameInfo(Frame* frame, u1 opcode);

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Instrucoes de Comparacao e desvio da JVM
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao responsavel por comparar dois longs vindos da pilha de operando
 *  comparando se sao iguais e lanca uma flag na propria
 *  pilha de operando
 *
 * \param Environment
 * \return void
 */
EXTE void lcmp(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao responsavel por comparar dois floats vindos da pilha de operando
 *  compara vendo se segundo eh menor que o primeiro lanca uma flag na propria
 *  pilha de operando
 *
 * \param Environment
 * \return void
 */

EXTE void fcmpl(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao responsavel por comparar dois floats vindos da pilha de operando
 *  compara vendo se o segundo eh maior que o primeiro lanca uma flag na propria
 *  pilha de operando
 *
 * \param Environment
 * \return void
 */

EXTE void fcmpg(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao responsavel por comparar dois doubles vindos da pilha de operando
 *  compara vendo se o segundo eh menor que o primeiro lanca uma flag na propria
 *  pilha de operando
 *
 * \param Environment
 * \return void
 */

EXTE void dcmpl(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao responsavel por comparar dois doubles vindos da pilha de operando
 *  compara vendo se o segundo eh maior que o primeiro lanca uma flag na propria
 *  pilha de operando
 *
 * \param Environment
 * \return void
 */

EXTE void dcmpg(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao responsavel por comparar o topo da pilha de operando
 *  comparar com zero e lancar a flag na pilha de operando
 *
 * \param Environment
 * \return void
 */

EXTE void ifeq(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao responsavel por comparar o topo da pilha de operando
 *  comparar com zero e lancar a flag na pilha de operando
 *
 * \param Environment
 * \return void
 */
EXTE void ifne(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao responsavel por comparar o topo da pilha de operando
 *  comparar com zero e lancar a flag na pilha de operando
 *
 * \param Environment
 * \return void
 */
EXTE void iflt(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao responsavel por comparar o topo da pilha de operando
 *  comparar com zero e lancar a flag na pilha de operando
 *
 * \param Environment
 * \return void
 */
EXTE void ifge(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao responsavel por comparar o topo da pilha de operando
 *  comparar com zero e lancar a flag na pilha de operando
 *
 * \param Environment
 * \return void
 */
EXTE void ifgt(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao responsavel por comparar o topo da pilha de operando
 *  comparar com zero e lancar a flag na pilha de operando
 *
 * \param Environment
 * \return void
 */
EXTE void ifle(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao que compara se dois numeros sao iguais e salta para a posicao
 *  determinada pelo indice
 *
 * \param Environment
 * \return void
 */
EXTE void if_icmpeq(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao que compara se dois numeros sao diferentes e salta para a posicao
 *  determinada pelo indice
 *
 * \param Environment
 * \return void
 */
EXTE void if_icmpne(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao que compara se o segundo numero eh menor que o primeiro e salta para a posicao
 *  determinada pelo indice
 *
 * \param Environment
 * \return void
 */
EXTE void if_icmplt(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao que compara se o segundo numero eh maior ou igual ao primeiro
 *  e salta para a posicao
 *  determinada pelo indice
 *
 * \param Environment
 * \return void
 */
EXTE void if_icmpge(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao que compara se o segundo numero eh maior que o primeiro e salta para a posicao
 *  determinada pelo indice
 *
 * \param Environment
 * \return void
 */
EXTE void if_icmpgt(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao que compara se o segundo numero eh menor ou igual ao primeiro
 *  e salta para a posicao
 *  determinada pelo indice
 *
 * \param Environment
 * \return void
 */

EXTE void if_icmple(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao que compara se duas referencias sao iguais e salta para a posicao
 *  determinada pelo indice
 *
 * \param Environment
 * \return void
 */
EXTE void if_acmpeq(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 *  Instrucao que compara se duas referencias sao iguais e salta para a posicao
 *  determinada pelo indice
 *
 * \param Environment
 * \return void
 */
EXTE void if_acmpne(Environment *environment);

//--------------------------------------------------------------------------------------------------
/*!
 *  Faz com que a execucao pule para a instrucao no endereco (pc + branchoffset),
 *  onde pc eh o endereco do opcode no bytecode e branchoffset eh um inteiro 16-bit (u2)
 *  que vem imediatamente após o opcode de goto no bytecode.
 *
 * \param Environment
 * \return void
 */
EXTE void goto_(Environment *environment);

//--------------------------------------------------------------------------------------------------
/*!
 *  Faz com que a execucao pule para a instrucao no endereco (pc + branchoffset),
 *  onde pc eh o endereco do opcode no bytecode e branchoffset eh um inteiro 16-bit (u2)
 *  que vem imediatamente após o opcode de jsr no bytecode. Após isso, salva o endereco atual no operand stack
 *
 * \param Environment
 * \return void
 */
EXTE void jsr(Environment *environment);

//--------------------------------------------------------------------------------------------------
/*!
 *  Pela o bytecode seguinte, esse bytecode contem um indice.
 *  Salva o indice no registrador do pc, fazendo com que a execucao continue dali
 *
 * \param Environment
 * \return void
 */
EXTE void ret(Environment *environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Pega o primeiro opcode entre 0 e 3 bytes após a instrução e salva como seu default,
 * após isso salva os 2 bytecodes seguintes como low e high respectivamente, depois pega
 * um indice salvo na pilha de operandos e compara com o high e o low. Se o indice não estiver
 * entre o low e o high, então o endereço de destino eh calculado como default+opcode inicial,
 * senao adiciona o primeiro offset em 'indice - low' ao opcode inicial e salva no pc
 *
 * \param Environment
 * \return void
 */
EXTE void tableswitch(Environment *environment);
//--------------------------------------------------------------------------------------------------
/*!
 * Pega o primeiro opcode entre 0 e 3 bytes após a instrução e salva como seu default,
 * após isso salva os 2 bytecodes seguintes como low e high respectivamente, depois pega
 * um indice salvo na pilha de operandos e compara com o high e o low. Se o indice não estiver
 * entre o low e o high, então o endereço de destino eh calculado como default+opcode inicial,
 * senao adiciona o primeiro offset em 'indice - low' ao opcode inicial e salva no pc
 *
 * \param Environment
 * \return void
 */
EXTE void lookupswitch(Environment *environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Se o valor for nulo, os bytes seguintes são construidos para criar um offset, continuando
 * a execucao no offset do opcode dessa instrucao, senao continua no proximo endereco apos essa instrucao
 *
 * \param Environment
 * \return void
 */
EXTE void ifnull(Environment *environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Se o valor nao for nulo, os bytes seguintes são construidos para criar um offset, continuando
 * a execucao no offset do opcode dessa instrucao, senao continua no proximo endereco apos essa instrucao
 *
 * \param Environment
 * \return void
 */
EXTE void ifnonnull(Environment *environment);

//--------------------------------------------------------------------------------------------------
/*!
 *  Faz com que a execucao pule para a instrucao no endereco (pc + branchoffset),
 *  onde pc eh o endereco do opcode no bytecode e branchoffset eh um inteiro 64-bit (u8)
 *  que vem imediatamente após o opcode de goto no bytecode. Mesma operacao de goto, so que 'wide' (offset maior)
 *
 * \param Environment
 * \return void
 */
EXTE void goto_w(Environment *environment);

//--------------------------------------------------------------------------------------------------
/*!
 *  Faz com que a execucao pule para a instrucao no endereco (pc + branchoffset),
 *  onde pc eh o endereco do opcode no bytecode e branchoffset eh um inteiro 64-bit (u8)
 *  que vem imediatamente após o opcode de jsr no bytecode. Após isso, salva o endereco atual no operand stack
 *  Mesma operacao de jsr, so que 'wide' (com um offset maior)
 * \param Environment
 * \return void
 */
EXTE void jsr_w(Environment *environment);

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Conversao de tipos
//--------------------------------------------------------------------------------------------------

/*!
 *  Instrucao para conversao de um int em um long
 *  \param Environment
 *  \return void
 */
EXTE void i2l(Environment* environment);

/*!
 *  Instrucao para conversao de um int em um float
 *  \param Environment
 *  \return void
 */
EXTE void i2f(Environment* environment);

/*!
 *  Instrucao para conversao de um int em um double
 *  \param Environment
 *  \return void
 */
EXTE void i2d(Environment* environment);

/*!
 *  Instrucao para conversao de um long em um int
 *  \param Environment
 *  \return void
 */
EXTE void l2i(Environment* environment);

/*!
 *  Instrucao para conversao de um long em um float
 *  \param Environment
 *  \return void
 */
EXTE void l2f(Environment* environment);

/*!
 *  Instrucao para conversao de um long em um double
 *  \param Environment
 *  \return void
 */
EXTE void l2d(Environment* environment);

/*!
 *  Instrucao para conversao de um float em um int
 *  \param Environment
 *  \return void
 */
EXTE void f2i(Environment* environment);

/*!
 *  Instrucao para conversao de um float em um long
 *  \param Environment
 *  \return void
 */
EXTE void f2l(Environment* environment);

/*!
 *  Instrucao para conversao de um float em um double
 *  \param Environment
 *  \return void
 */
EXTE void f2d(Environment* environment);

/*!
 *  Instrucao para conversao de um double em um int
 *  \param Environment
 *  \return void
 */
EXTE void d2i(Environment* environment);

/*!
 *  Instrucao para conversao de um double em um long
 *  \param Environment
 *  \return void
 */
EXTE void d2l(Environment* environment);

/*!
 *  Instrucao para conversao de um double em um float
 *  \param Environment
 *  \return void
 */
EXTE void d2f(Environment* environment);
/*!
 *  Instrucao para conversao de um int em um byte
 *  \param Environment
 *  \return void
 */
EXTE void i2b(Environment* environment);

/*!
 *  Instrucao para conversao de um int em um char
 *  \param Environment
 *  \return void
 */
EXTE void i2c(Environment* environment);

/*!
 *  Instrucao para conversao de um int em um short
 *  \param Environment
 *  \return void
 */
EXTE void i2s(Environment* environment);

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Transferencia valores entre o vetor de variaveis locais e a pilha de operandos de um frame de um metodo
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que faz nada
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void nop(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos uma referencia ao
 * objeto null
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void aconst_null(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor inteiro -1
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iconst_m1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor inteiro 0
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iconst_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor inteiro 1
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iconst_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor inteiro 2
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iconst_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor inteiro 3
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iconst_3(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor inteiro 4
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iconst_4(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor inteiro 5
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iconst_5(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor long 0
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lconst_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor long 1
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lconst_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor float 0
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fconst_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor float 1
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fconst_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor float 2
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fconst_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor double 0
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dconst_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que empilha na pilha de operandos o valor double 1
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dconst_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que estende com sinal o byte imediato para um valor
 * int e, então, o empilha na pilha de operandos
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void bipush(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que estende com sinal o valor imediato short
 * (byte1 << 8) | byte2 para um valor int e, então, o empilha na pilha de operandos
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void sipush(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento o unsigned byte index, o
 * qual eh um indice valido para o pool de constantes da classe corrente. O valor no pool de
 * constantes pode ser uma constante do tipo int ou float ou uma referencia simbolica para um
 * literal string. Caso for do tipo int ou float, deve emplilhar na pilha de operandos o valor
 * numerico da constante e, caso seja uma referencia para uma instancia da classe String, esta
 * referencia deve ser empilhada.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void ldc(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento os unsigned byte
 * indexbyte1 e indexbyte2 que, quando montados em um unsigned de 16 bits, viram um indice valido
 * para o pool de constantes da classe corrente. O valor no pool de
 * constantes pode ser uma constante do tipo int ou float ou uma referencia simbolica para um
 * literal string. Caso for do tipo int ou float, deve emplilhar na pilha de operandos o valor
 * numerico da constante e, caso seja uma referencia para uma instancia da classe String, esta
 * referencia deve ser empilhada.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void ldc_w(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento os unsigned byte
 * indexbyte1 e indexbyte2 que, quando montados em um unsigned de 16 bits, viram um indice valido
 * para o pool de constantes da classe corrente, onde o valor do indice eh
 * (indexbyte1 << 8) | indexbyte2. O valor no pool de constantes pode ser uma constante do tipo long
 * ou double. O valor numerico da constante eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void ldc2_w(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento o unsigned byte index, o
 * qual eh um indice valido para o vetor de variaveis locais do frame corrente. A variavel local no
 * index deve conter um inteiro. O valor da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento o unsigned byte index,
 * onde ambos os index e index+1 sao indices validos para o vetor de variaveis locais do frame
 * corrente. A variavel local no index deve conter um long. O valor da variavel local no index eh
 * empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento o unsigned byte index, o
 * qual eh um indice valido para o vetor de variaveis locais do frame corrente. A variavel local no
 * index deve conter um float. O valor da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento o unsigned byte index,
 * onde ambos os index e index+1 sao indices validos para o vetor de variaveis locais do frame
 * corrente. A variavel local no index deve conter um double. O valor da variavel local no index eh
 * empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento o unsigned byte index, o
 * qual eh um indice valido para o vetor de variaveis locais do frame corrente. A variavel local no
 * index deve conter uma referencia. O objectref da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void aload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 0. A variavel local no index deve conter um inteiro. O valor da
 * variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iload_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 1. A variavel local no index deve conter um inteiro. O valor da
 * variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iload_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 2. A variavel local no index deve conter um inteiro. O valor da
 * variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iload_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 3. A variavel local no index deve conter um inteiro. O valor da
 * variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iload_3(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 0 e 1. A variavel local no index deve conter um long.
 * O valor da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lload_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 1 e 2. A variavel local no index deve conter um long.
 * O valor da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lload_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 2 e 3. A variavel local no index deve conter um long.
 * O valor da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lload_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 3 e 4. A variavel local no index deve conter um long.
 * O valor da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lload_3(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 0. A variavel local no index deve conter um float. O valor da
 * variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fload_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 1. A variavel local no index deve conter um float. O valor da
 * variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fload_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 2. A variavel local no index deve conter um float. O valor da
 * variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fload_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 3. A variavel local no index deve conter um float. O valor da
 * variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fload_3(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 0 e 1. A variavel local no index deve conter um double.
 * O valor da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dload_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 1 e 2. A variavel local no index deve conter um double.
 * O valor da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dload_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 2 e 3. A variavel local no index deve conter um double.
 * O valor da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dload_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 3 e 4. A variavel local no index deve conter um double.
 * O valor da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dload_3(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 0. A variavel local no index deve conter uma referencia. O objectref
 * da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void aload_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 1. A variavel local no index deve conter uma referencia. O objectref
 * da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void aload_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 2. A variavel local no index deve conter uma referencia. O objectref
 * da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void aload_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh o 3. A variavel local no index deve conter uma referencia. O objectref
 * da variavel local eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void aload_3(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo int.
 * O index tambem eh desempilhado e deve ser do tipo int. O valor int no componente do vetor eh
 * recuperado no index e empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iaload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo long.
 * O index tambem eh desempilhado e deve ser do tipo int. O valor long no componente do vetor eh
 * recuperado no index e empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void laload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo float.
 * O index tambem eh desempilhado e deve ser do tipo int. O valor float no componente do vetor eh
 * recuperado no index e empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void faload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo double.
 * O index tambem eh desempilhado e deve ser do tipo int. O valor double no componente do vetor eh
 * recuperado no index e empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void daload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo reference.
 * O index tambem eh desempilhado e deve ser do tipo int. O valor reference no componente do vetor eh
 * recuperado no index e empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void aaload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo byte
 * ou do tipo boolean. O index tambem eh desempilhado e deve ser do tipo int. O valor no componente
 * do vetor eh recuperado no index e, caso seja do tipo byte, ele eh estendido com sinal ou, caso o
 * valor seja do tipo booleano, ele eh estendido sem sinal. Nos dois casos ele eh empilhado na pilha
 * de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void baload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo char e fazer referencia a um vetor cujo os componentes sao do tipo char.
 * O index tambem eh desempilhado e deve ser do tipo int. O valor char no componente do vetor eh
 * recuperado no index e estendido sem sinal. Ele eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void caload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo char e fazer referencia a um vetor cujo os componentes sao do tipo short.
 * O index tambem eh desempilhado e deve ser do tipo int. O valor short no componente do vetor eh
 * recuperado no index e estendido com sinal para um valor int. Ele eh empilhado na pilha de operandos.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void saload(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento o unsigned byte index, o
 * qual eh um indice valido para o vetor de variaveis locais do frame corrente. O valor no topo da
 * pilha de operandos deve ser do tipo int e eh desempilhado. O valor da variavel local no index eh
 * setado para este valor desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void istore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento o unsigned byte index,
 * onde ambos os index e index+1 sao indices validos para o vetor de variaveis locais do frame
 * corrente. O valor no topo da pilha de operandos deve ser do tipo long e eh desempilhado. Os
 * valores das variavel locais no index e index+1 sao setados para este valor desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lstore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento o unsigned byte index, o
 * qual eh um indice valido para o vetor de variaveis locais do frame corrente. O valor no topo da
 * pilha de operandos deve ser do tipo float e eh desempilhado e passa pela "value set conversion",
 * resultando em valor'. O valor da variavel local no index eh setado para este valor'.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fstore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento o unsigned byte index,
 * onde ambos os index e index+1 sao indices validos para o vetor de variaveis locais do frame
 * corrente. O valor no topo da pilha de operandos deve ser do tipo double e eh desempilhado e passa
 * pela "value set conversion", resultando em valor'. Os valores das variavel locais no index e
 * index+1 sao setados para este valor'.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dstore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao que recebe como argumento o unsigned byte index, o
 * qual eh um indice valido para o vetor de variaveis locais do frame corrente. O objectref no topo
 * da pilha de operandos deve ser do tipo returnAddress ou reference e eh desempilhado. O valor da
 * variavel local no index eh setado para este objectref desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void astore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 0. O valor no topo da pilha de operandos deve ser do tipo int e eh
 * desempilhado. O valor da variavel local no index eh setado para este valor desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void istore_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 1. O valor no topo da pilha de operandos deve ser do tipo int e eh
 * desempilhado. O valor da variavel local no index eh setado para este valor desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void istore_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 2. O valor no topo da pilha de operandos deve ser do tipo int e eh
 * desempilhado. O valor da variavel local no index eh setado para este valor desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void istore_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 3. O valor no topo da pilha de operandos deve ser do tipo int e eh
 * desempilhado. O valor da variavel local no index eh setado para este valor desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void istore_3(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 0 e 1. O valor no topo da pilha de operandos deve ser
 * do tipo long e eh desempilhado. Os valores das variavel locais no index e index+1 sao setados
 * para este valor desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lstore_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 1 e 2. O valor no topo da pilha de operandos deve ser
 * do tipo long e eh desempilhado. Os valores das variavel locais no index e index+1 sao setados
 * para este valor desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lstore_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 2 e 3. O valor no topo da pilha de operandos deve ser
 * do tipo long e eh desempilhado. Os valores das variavel locais no index e index+1 sao setados
 * para este valor desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lstore_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 3 e 4. O valor no topo da pilha de operandos deve ser
 * do tipo long e eh desempilhado. Os valores das variavel locais no index e index+1 sao setados
 * para este valor desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lstore_3(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 0. O valor no topo da pilha de operandos deve ser do tipo float e eh
 * desempilhado e passa pela "value set conversion", resultando em valor'. O valor da variavel local
 * no index eh setado para este valor'.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fstore_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 1. O valor no topo da pilha de operandos deve ser do tipo float e eh
 * desempilhado e passa pela "value set conversion", resultando em valor'. O valor da variavel local
 * no index eh setado para este valor'.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fstore_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 2. O valor no topo da pilha de operandos deve ser do tipo float e eh
 * desempilhado e passa pela "value set conversion", resultando em valor'. O valor da variavel local
 * no index eh setado para este valor'.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fstore_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 3. O valor no topo da pilha de operandos deve ser do tipo float e eh
 * desempilhado e passa pela "value set conversion", resultando em valor'. O valor da variavel local
 * no index eh setado para este valor'.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fstore_3(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 0 e 1. O valor no topo da pilha de operandos deve ser
 * do tipo double e eh desempilhado e passa pela "value set conversion", resultando em valor'. Os
 * valores das variavel locais no index e index+1 sao setados para este valor'.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dstore_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 1 e 2. O valor no topo da pilha de operandos deve ser
 * do tipo double e eh desempilhado e passa pela "value set conversion", resultando em valor'. Os
 * valores das variavel locais no index e index+1 sao setados para este valor'.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dstore_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 2 e 3. O valor no topo da pilha de operandos deve ser
 * do tipo double e eh desempilhado e passa pela "value set conversion", resultando em valor'. Os
 * valores das variavel locais no index e index+1 sao setados para este valor'.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dstore_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde os indices validos index e index+1 para o vetor
 * de variaveis locais do frame corrente sao o 3 e 4. O valor no topo da pilha de operandos deve ser
 * do tipo double e eh desempilhado e passa pela "value set conversion", resultando em valor'. Os
 * valores das variavel locais no index e index+1 sao setados para este valor'.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dstore_3(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 0. O objectref no topo da pilha de operandos deve ser do tipo
 * returnAddress ou reference e eh desempilhado. O valor da variavel local no index eh setado para
 * este objectref desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void astore_0(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 1. O objectref no topo da pilha de operandos deve ser do tipo
 * returnAddress ou reference e eh desempilhado. O valor da variavel local no index eh setado para
 * este objectref desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void astore_1(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 2. O objectref no topo da pilha de operandos deve ser do tipo
 * returnAddress ou reference e eh desempilhado. O valor da variavel local no index eh setado para
 * este objectref desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void astore_2(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por relizar uma instrucao onde o indice valido index para o vetor de variaveis
 * locais do frame corrente eh 3. O objectref no topo da pilha de operandos deve ser do tipo
 * returnAddress ou reference e eh desempilhado. O valor da variavel local no index eh setado para
 * este objectref desempilhado.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void astore_3(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo int.
 * O index e value tambem sao desempilhados e devem ser do tipo int. O int value eh armazenado como
 * o componente do vetor indexado pelo index.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void iastore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo
 * long. O index deve ser do tipo int e value deve ser do tipo long e tambem sao desempilhados. O
 * long value eh armazenado como o componente do vetor indexado pelo index.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void lastore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo
 * float. O index deve ser do tipo int e value deve ser do tipo float e tambem sao desempilhados. O
 * float value passa pela "value set conversion", resultando em value', e value' eh armazenado como
 * o componente do vetor indexado pelo index.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void fastore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo
 * double. O index deve ser do tipo int e value deve ser do tipo double e tambem sao desempilhados.
 * O double value passa pela "value set conversion", resultando em value', e value' eh armazenado
 * como o componente do vetor indexado pelo index.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void dastore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo
 * reference. O index deve ser do tipo int e value deve ser do tipo reference e tambem sao
 * desempilhados. O reference value eh armazenado como o componente do vetor indexado pelo index.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void aastore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo byte
 * ou do tipo boolean. O index e value tambem sao desempilhados e devem ser do tipo int. Caso os
 * componentes do vetor forem do tipo byte, o int value eh truncado para um byte ou, caso os
 * componentes do vetor forem do tipo boolean, o int value eh truncado para seu bit de ordem baixa
 * e, entao, estendido sem sinal para o tamanho de armazenamento para componentes de vetores boolen
 * usados pela implementacao. O resultado eh armazenado como o componente do vetor indexado pelo
 * index.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void bastore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo
 * char. O index e value tambem sao desempilhados e devem ser do tipo int. O int value eh truncado
 * para um char e armazenado como o componente do vetor indexado pelo index.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void castore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que desempilha da pilha de operandos o arrayref,
 * que deve ser do tipo reference e fazer referencia a um vetor cujo os componentes sao do tipo
 * short. O index e value tambem sao desempilhados e devem ser do tipo int. O int value eh truncado
 * para um short e armazenado como o componente do vetor indexado pelo index.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void sastore(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por realizar uma instrucao que estende o indice de variavel local por bytes
 * adicionais. Ela modifica o comportamento de outras instrucoes e assume um de dois formatos,
 * dependendo da instrucao a ser modificada. O primeiro formato modifica uma das instrucoes iload,
 * fload, aload, lload, dload, istore, fstore, astore, lstore, dstore, ou ret. O segundo formato se
 * aplica apenas para a instrucao iinc. Em qualquer caso a instrucao wide eh seguida pelo opcode da
 * instrucao a ser modificada e, logo apos ele, segue-se os unsigned bytes indexbyte1 e indexbyte2
 * que, quando montados em um unsigned de 16 bits, viram um indice valido para um vetor de variaveis
 * locais no frame corrente, onde o valor do indice eh (indexbyte1 << 8) | indexbyte2. Para as
 * instrucoes lload, dload, lstore, or dstore, o indice index+1 seguinte ao calculo tambem eh um
 * indice valido para o vetor de variaveis locais. No segundo formato, dois imediatos unsigned bytes
 * constbyte1 e constbyte2 seguem indexbyte1 e indexbyte2. Eles tambem sao montados em uma constante
 * signed 16-bit, onde o valor da constante eh (constbyte1 << 8) | constbyte2.
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void wide(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Instrucao que duplica o topo da pilha
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void Dup(Environment* environment);

//--------------------------------------------------------------------------------------------------
/*!
 * Instrucao que desempilha o topo da pilha
 *
 * \param environment Ambiente com a thread que contem a pilha JVM com o frame atual
 */
EXTE void pop(Environment* environment);

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Instrucoes logico atmeticas
//--------------------------------------------------------------------------------------------------

/*!
 *  Instrucao para soma de inteiros de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void iadd(Environment*);

/*!
 *  Instrucao para soma de inteiros de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void ladd(Environment*);
/*!
 *  Instrucao para soma de float de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void fadd(Environment*);
/*!
 *  Instrucao para soma de double de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void dadd(Environment*);
/*!
 *  Instrucao para subtracao de inteiros de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void isub(Environment*);
/*!
 *  Instrucao para subtracao de inteiros de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void lsub(Environment*);
/*!
 *  Instrucao para subtracao de float de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void fsub(Environment*);
/*!
 *  Instrucao para subtracao de double de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void dsub(Environment*);
/*!
 *  Instrucao para multiplicacao de inteiros de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void imul(Environment*);
/*!
 *  Instrucao para multiplicacao de inteiros de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void lmul(Environment*);
/*!
 *  Instrucao para multiplicacao de float de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void fmul(Environment*);
/*!
 *  Instrucao para multiplicacao de double de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void dmul(Environment*);
/*!
 *  Instrucao para divisao de inteiros de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void idiv(Environment*);
/*!
 *  Instrucao para divisao de inteiros de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void Ldiv(Environment*);
/*!
 *  Instrucao para divisao de float de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void fdiv(Environment*);
/*!
 *  Instrucao para divisao de double de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void ddiv(Environment*);
/*!
 *  Instrucao para resto de inteiros de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void irem(Environment*);
/*!
 *  Instrucao para resto de inteiros de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void lrem(Environment*);
/*!
 *  Instrucao para resto de float de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void frem(Environment*);
/*!
 *  Instrucao para resto de double de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void Drem(Environment*);
/*!
 *  Instrucao para nega de inteiro de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void ineg(Environment*);
/*!
 *  Instrucao para nega de inteiro de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void lneg(Environment*);
/*!
 *  Instrucao para nega de float de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void fneg(Environment*);
/*!
 *  Instrucao para nega de double de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void dneg(Environment*);
/*!
 *  Instrucao para shift left de inteiros de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void ishl(Environment*);
/*!
 *  Instrucao para shift left de inteiros de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void lshl(Environment*);
/*!
 *  Instrucao para shift right de inteiros de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void ishr(Environment*);
/*!
 *  Instrucao para shift right de inteiros de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void lshr(Environment*);
/*!
 *  Instrucao para shift right sem sinal de inteiros de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void iushr(Environment*);
/*!
 *  Instrucao para shift right sem sinal de inteiros de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void lushr(Environment*);
/*!
 *  Instrucao para and de inteiros de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void iand(Environment*);
/*!
 *  Instrucao para and de inteiros de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void land(Environment*);
/*!
 *  Instrucao para or de inteiros de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void ior(Environment*);
/*!
 *  Instrucao para or de inteiros de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void lor(Environment*);
/*!
 *  Instrucao para xor de inteiros de 32 bits
 *  \param Environment
 *  \return void
 */
EXTE void ixor(Environment*);
/*!
 *  Instrucao para xor de inteiros de 64 bits
 *  \param Environment
 *  \return void
 */
EXTE void lxor(Environment*);
/*!
 *  Instrucao para incremento de inteiros de 32 bits local
 *  \param Environment
 *  \param int será a constante que será somada
 *  \param u4 será o indice que será modificado
 *  \return void
 */
EXTE void iinc(Environment*);

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Manipulacao de objetos e chamadas de metodos
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que, dada uma referencia de 2bytes para um atributo de classe no pool de constantes,
 * empilha o valor deste na pilha de operandos
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void getstatic(Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que, dada uma referencia de 2bytes para um atributo de classe no pool de constantes,
 * desempilha um valor e seta o atributo da classe para o valor desempilhado.
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void putstatic(Environment* environment);



//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que, dada uma referencia de 2bytes para um atributo de objeto no pool de constantes,
 * desempilha uma referencia para um objeto e empilha o valor do atributo deste objeto.
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void getfield(Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que, dada uma referencia de 2bytes para um atributo de objeto no pool de constantes,
 * desempilha um valor e uma referencia para um objeto, setando o atributo do objeto para o valor
 * desempilhado.
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void putfield(Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que invoca um metodo de instancia. Executa baseado na classe.
 * Recebe 2bytes para montagem de referencia para um metodo no pool de constantes.
 *
 *
 * 1. Resolvemos o a classe do metodo, obtendo uma referencia para javaClass.
 *
 * 2. Resolvemos o nome do metodo, obtendo a referencia do method_info.
 *
 * 3. Baseado no descritor do metodo, desempilhamos os parametros.
 *
 * 4. Desempilhamos uma referencia para o objeto (Objectref).
 *
 * 5. Se o metodo for protected da classe ou superclasse, verificamos se a classe de Objectref
 *    eh a propria classe ou subclasse do metodo:
 *
 *      1. Se a classe C do objeto for a classe do metodo, buscamos o metodo em C e invocamos.
 *
 *      2. Se a C tiver uma superclasse, procuramos recursivamente e invocamos.
 *
 *      3. Se nao, lancamos a excessao AbstractMethodError.
 *
 *  6. Criamos um novo frame e empilhamos.
 *
 *      6.1. Passamos os argumentos para o vetor de variaveis locais:
 *              1. var_local_0 := Objectref.
 *              2. var_local_1 := argumento_1.
 *              K. var_local_N := argumento_N.
 *
 *
 *  Excessoes:
 *      1. Se Objectref eh nulo, lancamos NullPointerException.
 *      2. Se o metodo nao for encontrado, lancamos AbstractMethodError.
 *      3. Se o metodo for abstrato, lancamos AbstractMethodError.
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void invokevirtual(Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que invoca um metodo de instancia. Tratamento especial para superclasse, private e
 * invocacao de metodos de inicializacao de instancia.
 * Recebe 2bytes para montagem de referencia para um metodo no pool de constantes.
 *
 *
 * 1. Resolvemos o a classe do metodo, obtendo uma referencia para javaClass.
 *
 * 2. Resolvemos o nome do metodo, obtendo a referencia do method_info.
 *
 * 4. Baseado no descritor do metodo, desempilhamos os parametros.
 *
 * 5. Desempilhamos uma referencia para o objeto (Objectref).
 *
 * 6. Se o metodo for protected da classe ou superclasse, verificamos se a classe de Objectref
 *    eh a propria classe ou subclasse do metodo.
 *
 * 7. O metodo eh selecionado para invocacao a nao ser que todas as seguintes condicoes sejam
 *    verdadeiras:
 *
 *      1. A flag ACC_SUPER esta setada para a classe atual.
 *      2. A classe do metodo eh superclasse da classe atual.
 *      3. O metodo nao eh um metodo <init>.
 *      4. A classe do metodo eh superclasse da classe atual.
 *
 *  8. Se as condicoes acima forem verdadeiras, o metodo a ser invocado eh selecionado pelo seguinte
 *      procedimento de busca. Consideramos C uma superclasse direta da classe atual:
 *
 *      1. Se C contiver um metodo de instancia com o mesmo nome e descritor do metodo a ser
 *              invocado. Invocamos o metodo e terminamos.
 *      2. Se nao, se C tiver uma superclasse, o procedimento de busca eh realizado recursivamente
 *              na superclasse. O metodo a ser invocado eh o resultado do processo de busca recursiva.
 *      3. Se nao, AbstractMethodError.
 *
 *  9. Criamos um novo frame e empilhamos.
 *
 *      9.1. Passamos os argumentos para o vetor de variaveis locais:
 *              1. var_local_0 := Objectref.
 *              2. var_local_1 := argumento_1.
 *              3. var_local_N := argumento_N.
 *
 *
 *  Excessoes:
 *      1. Se o metodo eh <init> e a classe na qual ele eh declarado nao eh a classe referida pela
 *          instrucao, lancamos NoSuchMethodError.
 *      2. Se o metodo for estatico, lancamos IncompatibleClassChangeError.
 *      3. Se nao encontrarmos o metodo, lacamos AbstractMethodError.
 *      4. Se o metodo for abstrato, lancamos AbstractMethodError.
 *      5. Se Objectref eh nulo, lancamos NullPointerException.
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void invokespecial(Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que invoca um metodo de classe.
 * Recebe 2bytes para montagem de referencia para um metodo no pool de constantes.
 *
 *
 * 1. Resolvemos o a classe do metodo, obtendo uma referencia para javaClass.
 *
 * 2. Resolvemos o nome do metodo, obtendo a referencia do method_info.
 *
 * 3. Baseado no descritor do metodo, desempilhamos os parametros.
 *
 * 4. Criamos um novo frame e empilhamos.
 *
 *      1. Passamos os argumentos para o vetor de variaveis locais:
 *              1. var_local_0 := argumento_1.
 *              2. var_local_[N-1] := argumento_N.
 *
 *
 *  Excessoes:
 *      1. Se o metodo nao for estatico, lancamos IncompatibleClassChangeError.
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void invokestatic(Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que invoca um metodo de interface.
 * Recebe 2bytes para montagem de referencia para um metodo no pool de constantes.
 * Recebe um unsigned byte count != 0.
 * Recebe um unsigned byte 0.
 *
 *
 * 1. Resolvemos o a interface do metodo, obtendo uma referencia para javaClass.
 *
 * 2. Resolvemos o nome do metodo, obtendo a referencia do method_info.
 *
 * 3. Baseado no descritor do metodo, desempilhamos os parametros.
 *
 * 4. Desempilhamos uma referencia para o objeto (Objectref).
 *
 * 5. Se C eh a classe de Objectref, selecionamos o metodo com o seguinte procedimento:
 *
 *      1. Se C contem um metodo de instancia com o mesmo nome e descritor, invocamos e terminamos.
 *      2. Se nao, se C tiver uma superclasse, o procedimento de busca eh realizado recursivamente
 *              na superclasse. O metodo a ser invocado eh o resultado do processo de busca recursiva.
 *      3. Se nao, AbstractMethodError.
 *
 *
 *  6. Criamos um novo frame e empilhamos.
 *
 *      6.1. Passamos os argumentos para o vetor de variaveis locais:
 *              1. var_local_0 := Objectref.
 *              2. var_local_1 := argumento_1.
 *              3. var_local_N := argumento_N.
 *
 *
 *  Excessoes:
 *      1. Se Objectref eh nulo, lancamos NullPointerException
 *      2. Se a classe de Objectref nao implementa a interface, IncompatibleClassChangeError
 *      3. Se nao encontrarmos o metodo, lacamos AbstractMethodError
 *      4. Se o metodo nao for publico, IllegalAccessError
 *      5. Se o metodo for abstrato, lancamos AbstractMethodError
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void invokeinterface(Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que cria uma nova instancia de classe e empilha a referencia na pilha de operandos.
 * Recebe 2bytes para montagem de referencia para uma classe no pool de constantes.
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void New(Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que cria um novo array. Ele recebe como parametro o tipo do array a ser criado atype e
 * desempilha o numero de elementos no array a ser criado count. O novo array, cujos componentes sao
 * do tipo atype e de tamanho count, eh alocado e uma reference arrayref a ele eh empilhada na pilha
 * de operandos. Cada um dos elementos do novo array sao inicializados para o valor inicial padrao
 * para o tipo do array.
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void newarray(Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que cria um novo array de reference. O numero de componentes do array a ser criado count
 * deve ser do tipo int e eh desempilhado da pilha de operandos. O metodo recebe como parametro os
 * unsigned indexbyte1 e indexbyte2 que, quando montados, viram um indice para a constante pool de
 * tempo de execucao da classe corrente, onde o valor do indice eh (indexbyte1 << 8) | indexbyte2. O
 * item da constante pool de tempo de execucao no indice deve ser uma referencia simbolica para um
 * tipo class, array ou interface. O tipo nomeado da classe, array ou interface eh resolvido.
 * Um novo array com componentes daquele tipo, de tamanho count, eh alocado e uma reference arrayref
 * a ele eh empilhada na pilha de operandos. Cada um dos elementos do novo array sao inicializados
 * para null, o valor padrao para tipos reference.
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void anewarray(Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que obtem o tamanho do array. O arrayref deve ser do tipo reference e deve se referir a um
 * array. Ele eh desempilhado da pilha de operandos. O tamanho do array que ele referencia eh
 * determinado e eh empilhado na pilha de operandos como um int.
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void arraylength(Environment* environment);


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que cria um novo array multidimensional. O metodo recebe como argumento um numero de
 * dimensoes do array a ser criado dimensions. A pilha de operandos deve conter os valores das
 * dimensoes count1, count2... Cada valor representa o numero de componentes em uma dimensao do
 * array a ser criado e deve ser do tipo int e nao negativo.
 *
 * Todos os valores count sao desempilhados da pilha de operandos. O metodo tambem recebe como
 * parametro os unsigned indexbyte1 e indexbyte2 que, quando montados, viram um indice para a
 * constante pool de tempo de execucao da classe corrente, onde o valor do indice eh
 * (indexbyte1 << 8) | indexbyte2. O item da constante pool de tempo de execucao no indice deve ser
 * uma referencia simbolica para um tipo class, array ou interface. O tipo nomeado da classe, array
 * ou interface esta resolvido. A entrada resultante deve ser um array de tipo class de
 * dimensionalidade maior ou igual a dimensions.
 *
 * Um novo array multidimensional de tipo do array eh alocado. Se qualquer valor count for zero,
 * nenhuma dimensao subsequente eh alocada. Os componentes do array na primeira dimensao sao
 * inicializados para os subarrays do tipo da segunda dimensao, e assim por diante. Os componentes
 * da ultima dimensao alocada do array sao inicializados para o valor inicial padrao pelo tipo dos
 * componentes. Uma reference arrayref para o novo array eh empilhada na pilha de operandos.
 *
 * \param environment Ambiente de execucao atual.
 */
EXTE void multianewarray(Environment* environment);


//--------------------------------------------------------------------------------------------------
// SUBMODULO: 
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------

/*!
 *  Instrucao para tratar retorno de um int
 *  \param Environment
 *  \return void
 */
EXTE void ireturn(Environment* environment);

/*!
 *  Instrucao para tratar retorno de um long
 *  \param Environment
 *  \return void
 */
EXTE void lreturn(Environment* environment);

/*!
 *  Instrucao para tratar retorno de um float
 *  \param Environment
 *  \return void
 */
EXTE void freturn(Environment* environment);

/*!
 *  Instrucao para tratar retorno de um double
 *  \param Environment
 *  \return void
 */
EXTE void dreturn(Environment* environment);

/*!
 *  Instrucao para tratar retorno de um caracter
 *  \param Environment
 *  \return void
 */
EXTE void areturn(Environment* environment);

/*!
 *  Instrucao para tratar retorno em geral
 *  \param Environment
 *  \return void
 */
EXTE void return_(Environment* environment);


//--------------------------------------------------------------------------------------------------
// SUBMODULO: Interpretador
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo responsavel por executar um ambiente de execucao.
 *
 * \param enviroment  Estrutura contendo a thread e a area de metodos a ser executada
 */
EXTE void execute(Environment* environment);
#endif