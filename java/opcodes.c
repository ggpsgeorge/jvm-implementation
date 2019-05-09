//#################################################################################################
/*! \file opcodes.c
 *
 *  \brief Biblioteca de servicos de Opcodes
 *
 *  Bliblioteca responsavel pela implementacoa dos metodos relacionados aos opcodes de uma JVM
 */
//##################################################################################################

#define OPCODES

#include <stdio.h>
#include "../include/opcodes.h"


//--------------------------------------------------------------------------------------------------
char* getOpcodeName(u1 opcode){
    char * op_name = (char* ) malloc(20* sizeof(char));
    switch (opcode) {
        case OP_aaload:
            op_name = "aaload";
            break;
        case OP_aastore:
            op_name = "aastore";
            break;
        case OP_aconst_null:
            op_name = "aconst_null";
            break;
        case OP_aload:
            op_name = "aload";
            break;
        case OP_aload_0:
            op_name = "aload_0";
            break;
        case OP_aload_1:
            op_name = "aload_1";
            break;
        case OP_aload_2:
            op_name = "aload_2";
            break;
        case OP_aload_3:
            op_name = "aload_3";
            break;
        case OP_anewarray:
            op_name = "anewarray";
            break;
        case OP_areturn:
            op_name = "areturn";
            break;
        case OP_arraylength:
            op_name = "arraylength";
            break;
        case OP_astore:
            op_name = "astore";
            break;
        case OP_astore_0:
            op_name = "astore_0";
            break;
        case OP_astore_1:
            op_name = "astore_1";
            break;
        case OP_astore_2:
            op_name = "astore_2";
            break;
        case OP_astore_3:
            op_name = "astore_3";
            break;
        case OP_athrow:
            op_name = "athrow";
            break;
        case OP_baload:
            op_name = "baload";
            break;
        case OP_bastore:
            op_name = "bastore";
            break;
        case OP_bipush:
            op_name = "bipush";
            break;
        case OP_breakpoint:
            op_name = "breakpoint";
            break;
        case OP_caload:
            op_name = "caload";
            break;
        case OP_castore:
            op_name = "castore";
            break;
        case OP_checkcast:
            op_name = "checkcast";
            break;
        case OP_d2f:
            op_name = "d2f";
            break;
        case OP_d2i:
            op_name = "d2i";
            break;
        case OP_d2l:
            op_name = "d2l";
            break;
        case OP_dadd:
            op_name = "dadd";
            break;
        case OP_daload:
            op_name = "daload";
            break;
        case OP_dastore:
            op_name = "dastore";
            break;
        case OP_dcmpg:
            op_name = "dcmpg";
            break;
        case OP_dcmpl:
            op_name = "dcmpl";
            break;
        case OP_dconst_0:
            op_name = "dconst_0";
            break;
        case OP_dconst_1:
            op_name = "dconst_1";
            break;
        case OP_ddiv:
            op_name = "ddiv";
            break;
        case OP_dload:
            op_name = "dload";
            break;
        case OP_dload_0:
            op_name = "dload_0";
            break;
        case OP_dload_1:
            op_name = "dload_1";
            break;
        case OP_dload_2:
            op_name = "dload_2";
            break;
        case OP_dload_3:
            op_name = "dload_3";
            break;
        case OP_dmul:
            op_name = "dmul";
            break;
        case OP_dneg:
            op_name = "dneg";
            break;
        case OP_drem:
            op_name = "Drem";
            break;
        case OP_dreturn:
            op_name = "dreturn";
            break;
        case OP_dstore:
            op_name = "dstore";
            break;
        case OP_dstore_0:
            op_name = "dstore_0";
            break;
        case OP_dstore_1:
            op_name = "dstore_1";
            break;
        case OP_dstore_2:
            op_name = "dstore_2";
            break;
        case OP_dstore_3:
            op_name = "dstore_3";
            break;
        case OP_dsub:
            op_name = "dsub";
            break;
        case OP_dup:
            op_name = "dup";
            break;
        case OP_dup_x1:
            op_name = "dup_x1";
            break;
        case OP_dup_x2:
            op_name = "dup_x2";
            break;
        case OP_dup2:
            op_name = "dup2";
            break;
        case OP_dup2_x1:
            op_name = "dup2_x1";
            break;
        case OP_dup2_x2:
            op_name = "dup2_x2";
            break;
        case OP_f2d:
            op_name = "f2d";
            break;
        case OP_f2i:
            op_name = "f2i";
            break;
        case OP_f2l:
            op_name = "f2l";
            break;
        case OP_fadd:
            op_name = "fadd";
            break;
        case OP_faload:
            op_name = "faload";
            break;
        case OP_fastore:
            op_name = "fastore";
            break;
        case OP_fcmpg:
            op_name = "fcmpg";
            break;
        case OP_fcmpl:
            op_name = "fcmpl";
            break;
        case OP_fconst_0:
            op_name = "fconst_0";
            break;
        case OP_fconst_1:
            op_name = "fconst_1";
            break;
        case OP_fconst_2:
            op_name = "fconst_2";
            break;
        case OP_fdiv:
            op_name = "fdiv";
            break;
        case OP_fload:
            op_name = "fload";
            break;
        case OP_fload_0:
            op_name = "fload_0";
            break;
        case OP_fload_1:
            op_name = "fload_1";
            break;
        case OP_fload_2:
            op_name = "fload_2";
            break;
        case OP_fload_3:
            op_name = "fload_3";
            break;
        case OP_fmul:
            op_name = "fmul";
            break;
        case OP_fneg:
            op_name = "fneg";
            break;
        case OP_frem:
            op_name = "frem";
            break;
        case OP_freturn:
            op_name = "freturn";
            break;
        case OP_fstore:
            op_name = "fstore";
            break;
        case OP_fstore_0:
            op_name = "fstore_0";
            break;
        case OP_fstore_1:
            op_name = "fstore_1";
            break;
        case OP_fstore_2:
            op_name = "fstore_2";
            break;
        case OP_fstore_3:
            op_name = "fstore_3";
            break;
        case OP_fsub:
            op_name = "fsub";
            break;
        case OP_getfield:
            op_name = "getfield";
            break;
        case OP_getstatic:
            op_name = "getstatic";
            break;
        case OP_goto:
            op_name = "goto";
            break;
        case OP_goto_w:
            op_name = "goto_w";
            break;
        case OP_i2b:
            op_name = "i2b";
            break;
        case OP_i2c:
            op_name = "i2c";
            break;
        case OP_i2d:
            op_name = "i2d";
            break;
        case OP_i2f:
            op_name = "i2f";
            break;
        case OP_i2l:
            op_name = "i2l";
            break;
        case OP_i2s:
            op_name = "i2s";
            break;
        case OP_iadd:
            op_name = "iadd";
            break;
        case OP_iaload:
            op_name = "iaload";
            break;
        case OP_iand:
            op_name = "iand";
            break;
        case OP_iastore:
            op_name = "iastore";
            break;
        case OP_iconst_m1:
            op_name = "iconst_m1";
            break;
        case OP_iconst_0:
            op_name = "iconst_0";
            break;
        case OP_iconst_1:
            op_name = "iconst_1";
            break;
        case OP_iconst_2:
            op_name = "iconst_2";
            break;
        case OP_iconst_3:
            op_name = "iconst_3";
            break;
        case OP_iconst_4:
            op_name = "iconst_4";
            break;
        case OP_iconst_5:
            op_name = "iconst_5";
            break;
        case OP_idiv:
            op_name = "idiv";
            break;
        case OP_if_acmpeq:
            op_name = "if_acmpeq";
            break;
        case OP_if_acmpne:
            op_name = "if_acmpne";
            break;
        case OP_if_icmpeq:
            op_name = "if_icmpeq";
            break;
        case OP_if_icmpge:
            op_name = "if_icmpge";
            break;
        case OP_if_icmpgt:
            op_name = "if_icmpgt";
            break;
        case OP_if_icmple:
            op_name = "if_icmple";
            break;
        case OP_if_icmplt:
            op_name = "if_icmplt";
            break;
        case OP_if_icmpne:
            op_name = "if_icmpne";
            break;
        case OP_ifeq:
            op_name = "ifeq";
            break;
        case OP_ifge:
            op_name = "ifge";
            break;
        case OP_ifgt:
            op_name = "ifgt";
            break;
        case OP_ifle:
            op_name = "ifle";
            break;
        case OP_iflt:
            op_name = "iflt";
            break;
        case OP_ifne:
            op_name = "ifne";
            break;
        case OP_ifnonnull:
            op_name = "ifnonnull";
            break;
        case OP_ifnull:
            op_name = "ifnull";
            break;
        case OP_iinc:
            op_name = "iinc";
            break;
        case OP_iload:
            op_name = "iload";
            break;
        case OP_iload_0:
            op_name = "iload_0";
            break;
        case OP_iload_1:
            op_name = "iload_1";
            break;
        case OP_iload_2:
            op_name = "iload_2";
            break;
        case OP_iload_3:
            op_name = "iload_3";
            break;
        case OP_impdep1:
            op_name = "impdep1";
            break;
        case OP_impdep2:
            op_name = "impdep2";
            break;
        case OP_imul:
            op_name = "imul";
            break;
        case OP_ineg:
            op_name = "ineg";
            break;
        case OP_instanceof:
            op_name = "instanceof";
            break;
        case OP_invokedynamic:
            op_name = "invokedynamic";
            break;
        case OP_invokeinterface:
            op_name = "invokeinterface";
            break;
        case OP_invokespecial:
            op_name = "invokespecial";
            break;
        case OP_invokestatic:
            op_name = "invokestatic";
            break;
        case OP_invokevirtual:
            op_name = "invokevirtual";
            break;
        case OP_ior:
            op_name = "ior";
            break;
        case OP_irem:
            op_name = "irem";
            break;
        case OP_ireturn:
            op_name = "ireturn";
            break;
        case OP_ishl:
            op_name = "ishl";
            break;
        case OP_ishr:
            op_name = "ishr";
            break;
        case OP_istore:
            op_name = "istore";
            break;
        case OP_istore_0:
            op_name = "istore_0";
            break;
        case OP_istore_1:
            op_name = "istore_1";
            break;
        case OP_istore_2:
            op_name = "istore_2";
            break;
        case OP_istore_3:
            op_name = "istore_3";
            break;
        case OP_isub:
            op_name = "isub";
            break;
        case OP_iushr:
            op_name = "iushr";
            break;
        case OP_ixor:
            op_name = "ixor";
            break;
        case OP_jsr:
            op_name = "jsr";
            break;
        case OP_jsr_w:
            op_name = "jsr_w";
            break;
        case OP_l2d:
            op_name = "l2d";
            break;
        case OP_l2f:
            op_name = "l2f";
            break;
        case OP_l2i:
            op_name = "l2i";
            break;
        case OP_ladd:
            op_name = "ladd";
            break;
        case OP_laload:
            op_name = "laload";
            break;
        case OP_land:
            op_name = "land";
            break;
        case OP_lastore:
            op_name = "lastore";
            break;
        case OP_lcmp:
            op_name = "lcmp";
            break;
        case OP_lconst_0:
            op_name = "lconst_0";
            break;
        case OP_lconst_1:
            op_name = "lconst_1";
            break;
        case OP_ldc:
            op_name = "ldc";
            break;
        case OP_ldc_w:
            op_name = "ldc_w";
            break;
        case OP_ldc2_w:
            op_name = "ldc2_w";
            break;
        case OP_ldiv:
            op_name = "Ldiv";
            break;
        case OP_lload:
            op_name = "lload";
            break;
        case OP_lload_0:
            op_name = "lload_0";
            break;
        case OP_lload_1:
            op_name = "lload_1";
            break;
        case OP_lload_2:
            op_name = "lload_2";
            break;
        case OP_lload_3:
            op_name = "lload_3";
            break;
        case OP_lmul:
            op_name = "lmul";
            break;
        case OP_lneg:
            op_name = "lneg";
            break;
        case OP_lookupswitch:
            op_name = "lookupswitch";
            break;
        case OP_lor:
            op_name = "lor";
            break;
        case OP_lrem:
            op_name = "lrem";
            break;
        case OP_lreturn:
            op_name = "lreturn";
            break;
        case OP_lshl:
            op_name = "lshl";
            break;
        case OP_lshr:
            op_name = "lshr";
            break;
        case OP_lstore:
            op_name = "lstore";
            break;
        case OP_lstore_0:
            op_name = "lstore_0";
            break;
        case OP_lstore_1:
            op_name = "lstore_1";
            break;
        case OP_lstore_2:
            op_name = "lstore_2";
            break;
        case OP_lstore_3:
            op_name = "lstore_3";
            break;
        case OP_lsub:
            op_name = "lsub";
            break;
        case OP_lushr:
            op_name = "lushr";
            break;
        case OP_lxor:
            op_name = "lxor";
            break;
        case OP_monitorenter:
            op_name = "monitorenter";
            break;
        case OP_monitorexit:
            op_name = "monitorexit";
            break;
        case OP_multianewarray:
            op_name = "multianewarray";
            break;
        case OP_new:
            op_name = "new";
            break;
        case OP_newarray:
            op_name = "newarray";
            break;
        case OP_nop:
            op_name = "nop";
            break;
        case OP_pop:
            op_name = "pop";
            break;
        case OP_pop2:
            op_name = "pop2";
            break;
        case OP_putfield:
            op_name = "putfield";
            break;
        case OP_putstatic:
            op_name = "putstatic";
            break;
        case OP_ret:
            op_name = "ret";
            break;
        case OP_return:
            op_name = "return";
            break;
        case OP_saload:
            op_name = "saload";
            break;
        case OP_sastore:
            op_name = "sastore";
            break;
        case OP_sipush:
            op_name = "sipush";
            break;
        case OP_swap:
            op_name = "swap";
            break;
        case OP_tableswitch:
            op_name = "tableswitch";
            break;
        case OP_wide:
            op_name = "wide";
        default:
            op_name = "";
            break;
    }
    return op_name;
}


//--------------------------------------------------------------------------------------------------
short getOpcodeAttributesNumber(u1 opcode, u1* params, u1* codeBegin){
    short result = -1;
    short pad = 0;
    
    switch (opcode) {
            
        case OP_aaload:
            result = 0;
            break;
        case OP_aastore:
            result = 0;
            break;
        case OP_aconst_null:
            result = 0;
            break;
        case OP_aload:
            result = 1;
            break;
        case OP_aload_0:
            result = 0;
            break;
        case OP_aload_1:
            result = 0;
            break;
        case OP_aload_2:
            result = 0;
            break;
        case OP_aload_3:
            result = 0;
            break;
        case OP_anewarray:
            result = 2;
            break;
        case OP_areturn:
            result = 0;
            break;
        case OP_arraylength:
            result = 0;
            break;
        case OP_astore:
            result = 1;
            break;
        case OP_astore_0:
            result = 0;
            break;
        case OP_astore_1:
            result = 0;
            break;
        case OP_astore_2:
            result = 0;
            break;
        case OP_astore_3:
            result = 0;
            break;
        case OP_athrow:
            result = 0;
            break;
        case OP_baload:
            result = 0;
            break;
        case OP_bastore:
            result = 0;
            break;
        case OP_bipush:
            result = 1;
            break;
        case OP_breakpoint:
            result = 0;
            break;
        case OP_caload:
            result = 0;
            break;
        case OP_castore:
            result = 0;
            break;
        case OP_checkcast:
            result = 2;
            break;
        case OP_d2f:
            result = 0;
            break;
        case OP_d2i:
            result = 0;
            break;
        case OP_d2l:
            result = 0;
            break;
        case OP_dadd:
            result = 0;
            break;
        case OP_daload:
            result = 0;
            break;
        case OP_dastore:
            result = 0;
            break;
        case OP_dcmpg:
            result = 0;
            break;
        case OP_dcmpl:
            result = 0;
            break;
        case OP_dconst_0:
            result = 0;
            break;
        case OP_dconst_1:
            result = 0;
            break;
        case OP_ddiv:
            result = 0;
            break;
        case OP_dload:
            result = 1;
            break;
        case OP_dload_0:
            result = 0;
            break;
        case OP_dload_1:
            result = 0;
            break;
        case OP_dload_2:
            result = 0;
            break;
        case OP_dload_3:
            result = 0;
            break;
        case OP_dmul:
            result = 0;
            break;
        case OP_dneg:
            result = 0;
            break;
        case OP_drem:
            result = 0;
            break;
        case OP_dreturn:
            result = 0;
            break;
        case OP_dstore:
            result = 1;
            break;
        case OP_dstore_0:
            result = 0;
            break;
        case OP_dstore_1:
            result = 0;
            break;
        case OP_dstore_2:
            result = 0;
            break;
        case OP_dstore_3:
            result = 0;
            break;
        case OP_dsub:
            result = 0;
            break;
        case OP_dup:
            result = 0;
            break;
        case OP_dup_x1:
            result = 0;
            break;
        case OP_dup_x2:
            result = 0;
            break;
        case OP_dup2:
            result = 0;
            break;
        case OP_dup2_x1:
            result = 0;
            break;
        case OP_dup2_x2:
            result = 0;
            break;
        case OP_f2d:
            result = 0;
            break;
        case OP_f2i:
            result = 0;
            break;
        case OP_f2l:
            result = 0;
            break;
        case OP_fadd:
            result = 0;
            break;
        case OP_faload:
            result = 0;
            break;
        case OP_fastore:
            result = 0;
            break;
        case OP_fcmpg:
            result = 0;
            break;
        case OP_fcmpl:
            result = 0;
            break;
        case OP_fconst_0:
            result = 0;
            break;
        case OP_fconst_1:
            result = 0;
            break;
        case OP_fconst_2:
            result = 0;
            break;
        case OP_fdiv:
            result = 0;
            break;
        case OP_fload:
            result = 1;
            break;
        case OP_fload_0:
            result = 0;
            break;
        case OP_fload_1:
            result = 0;
            break;
        case OP_fload_2:
            result = 0;
            break;
        case OP_fload_3:
            result = 0;
            break;
        case OP_fmul:
            result = 0;
            break;
        case OP_fneg:
            result = 0;
            break;
        case OP_frem:
            result = 0;
            break;
        case OP_freturn:
            result = 0;
            break;
        case OP_fstore:
            result = 1;
            break;
        case OP_fstore_0:
            result = 0;
            break;
        case OP_fstore_1:
            result = 0;
            break;
        case OP_fstore_2:
            result = 0;
            break;
        case OP_fstore_3:
            result = 0;
            break;
        case OP_fsub:
            result = 0;
            break;
        case OP_getfield:
            result = 2;
            break;
        case OP_getstatic:
            result = 2;
            break;
        case OP_goto:
            result = 2;
            break;
        case OP_goto_w:
            result = 4;
            break;
        case OP_i2b:
            result = 0;
            break;
        case OP_i2c:
            result = 0;
            break;
        case OP_i2d:
            result = 0;
            break;
        case OP_i2f:
            result = 0;
            break;
        case OP_i2l:
            result = 0;
            break;
        case OP_i2s:
            result = 0;
            break;
        case OP_iadd:
            result = 0;
            break;
        case OP_iaload:
            result = 0;
            break;
        case OP_iand:
            result = 0;
            break;
        case OP_iastore:
            result = 0;
            break;
        case OP_iconst_m1:
            result = 0;
            break;
        case OP_iconst_0:
            result = 0;
            break;
        case OP_iconst_1:
            result = 0;
            break;
        case OP_iconst_2:
            result = 0;
            break;
        case OP_iconst_3:
            result = 0;
            break;
        case OP_iconst_4:
            result = 0;
            break;
        case OP_iconst_5:
            result = 0;
            break;
        case OP_idiv:
            result = 0;
            break;
        case OP_if_acmpeq:
            result = 2;
            break;
        case OP_if_acmpne:
            result = 2;
            break;
        case OP_if_icmpeq:
            result = 2;
            break;
        case OP_if_icmpge:
            result = 2;
            break;
        case OP_if_icmpgt:
            result = 2;
            break;
        case OP_if_icmple:
            result = 2;
            break;
        case OP_if_icmplt:
            result = 2;
            break;
        case OP_if_icmpne:
            result = 2;
            break;
        case OP_ifeq:
            result = 2;
            break;
        case OP_ifge:
            result = 2;
            break;
        case OP_ifgt:
            result = 2;
            break;
        case OP_ifle:
            result = 2;
            break;
        case OP_iflt:
            result = 2;
            break;
        case OP_ifne:
            result = 2;
            break;
        case OP_ifnonnull:
            result = 2;
            break;
        case OP_ifnull:
            result = 2;
            break;
        case OP_iinc:
            result = 2;
            break;
        case OP_iload:
            result = 1;
            break;
        case OP_iload_0:
            result = 0;
            break;
        case OP_iload_1:
            result = 0;
            break;
        case OP_iload_2:
            result = 0;
            break;
        case OP_iload_3:
            result = 0;
            break;
        case OP_impdep1:
            result = 0;
            break;
        case OP_impdep2:
            result = 0;
            break;
        case OP_imul:
            result = 0;
            break;
        case OP_ineg:
            result = 0;
            break;
        case OP_instanceof:
            result = 2;
            break;
        case OP_invokedynamic:
            result = 4;
            break;
        case OP_invokeinterface:
            result = 4;
            break;
        case OP_invokespecial:
            result = 2;
            break;
        case OP_invokestatic:
            result = 2;
            break;
        case OP_invokevirtual:
            result = 2;
            break;
        case OP_ior:
            result = 0;
            break;
        case OP_irem:
            result = 0;
            break;
        case OP_ireturn:
            result = 0;
            break;
        case OP_ishl:
            result = 0;
            break;
        case OP_ishr:
            result = 0;
            break;
        case OP_istore:
            result = 1;
            break;
        case OP_istore_0:
            result = 0;
            break;
        case OP_istore_1:
            result = 0;
            break;
        case OP_istore_2:
            result = 0;
            break;
        case OP_istore_3:
            result = 0;
            break;
        case OP_isub:
            result = 0;
            break;
        case OP_iushr:
            result = 0;
            break;
        case OP_ixor:
            result = 0;
            break;
        case OP_jsr:
            result = 2;
            break;
        case OP_jsr_w:
            result = 4;
            break;
        case OP_l2d:
            result = 0;
            break;
        case OP_l2f:
            result = 0;
            break;
        case OP_l2i:
            result = 0;
            break;
        case OP_ladd:
            result = 0;
            break;
        case OP_laload:
            result = 0;
            break;
        case OP_land:
            result = 0;
            break;
        case OP_lastore:
            result = 0;
            break;
        case OP_lcmp:
            result = 0;
            break;
        case OP_lconst_0:
            result = 0;
            break;
        case OP_lconst_1:
            result = 0;
            break;
        case OP_ldc:
            result = 1;
            break;
        case OP_ldc_w:
            result = 2;
            break;
        case OP_ldc2_w:
            result = 2;
            break;
        case OP_ldiv:
            result = 0;
            break;
        case OP_lload:
            result = 1;
            break;
        case OP_lload_0:
            result = 0;
            break;
        case OP_lload_1:
            result = 0;
            break;
        case OP_lload_2:
            result = 0;
            break;
        case OP_lload_3:
            result = 0;
            break;
        case OP_lmul:
            result = 0;
            break;
        case OP_lneg:
            result = 0;
            break;
        case OP_lookupswitch:
            //Pad bytes
            while ((params-codeBegin) % 4 != 0) {
                params++;
                pad++;
            }
            //Campos de Default
            params += 4;
            
            int npairs = *params++;
            npairs = npairs << 8 | *params++;
            npairs = npairs << 8 | *params++;
            npairs = npairs << 8 | *params++;
            
            //Npairs de 32bits + outros argumentos
            result = pad + 7 + (npairs)*4*2;
            break;
        case OP_lor:
            result = 0;
            break;
        case OP_lrem:
            result = 0;
            break;
        case OP_lreturn:
            result = 0;
            break;
        case OP_lshl:
            result = 0;
            break;
        case OP_lshr:
            result = 0;
            break;
        case OP_lstore:
            result = 1;
            break;
        case OP_lstore_0:
            result = 0;
            break;
        case OP_lstore_1:
            result = 0;
            break;
        case OP_lstore_2:
            result = 0;
            break;
        case OP_lstore_3:
            result = 0;
            break;
        case OP_lsub:
            result = 0;
            break;
        case OP_lushr:
            result = 0;
            break;
        case OP_lxor:
            result = 0;
            break;
        case OP_monitorenter:
            result = 0;
            break;
        case OP_monitorexit:
            result = 0;
            break;
        case OP_multianewarray:
            result = 3;
            break;
        case OP_new:
            result = 2;
            break;
        case OP_newarray:
            result = 1;
            break;
        case OP_nop:
            result = 0;
            break;
        case OP_pop:
            result = 0;
            break;
        case OP_pop2:
            result = 0;
            break;
        case OP_putfield:
            result = 2;
            break;
        case OP_putstatic:
            result = 2;
            break;
        case OP_ret:
            result = 1;
            break;
        case OP_return:
            result = 0;
            break;
        case OP_saload:
            result = 0;
            break;
        case OP_sastore:
            result = 0;
            break;
        case OP_sipush:
            result = 2;
            break;
        case OP_swap:
            result = 0;
            break;
        case OP_tableswitch:
            //Pad bytes
            while ((params-codeBegin) % 4 != 0) {
                params++;
                pad++;
            }
            //Campos de Default
            params += 4;
            
            int min = *params++;
            min = min << 8 | *params++;
            min = min << 8 | *params++;
            min = min << 8 | *params++;
            
            int max = *params++;
            max = max << 8 | *params++;
            max = max << 8 | *params++;
            max = max << 8 | *params++;
            
            result = pad + 11 + (max-min+1)*4;
            break;
        case OP_wide:
            result = (*params == OP_iinc) ? 5 : 3;
            break;
        default:
            break;
    }
    return result;
}

