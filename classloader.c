//#################################################################################################
/*! \file classloader.c
 *
 *  \brief Modulo do Carregador de Classes da JVM.
 *
 *  Modulo responsavel por implementar os servicos relacionados ao carregamento de classes da
 *  JVM. Contem submodulos responsaveis por: 
 *  - Implementar os metodos relacionados a exibicao de um arquivo ".class".
 *  - Implementar os metodos de leitura de arquivos ".class"
 *  - Implementar os metodos relacionados a manutencao de uma estrutura ArqClass
 *  - Implementar os metodos responsaveis pela manipulacao de arquivos
 */
//##################################################################################################

#define CLASSLOAD

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "include/memoryunit.h"
#include "include/executionengine.h"
#include "include/classloader.h"
#include "include/opcodes.h"


//--------------------------------------------------------------------------------------------------
/*!
 * Método que executa o processo de verificacao da corretude do codigo e compatibilidade da classe
 *
 * \param arqClass Referencia para uma estrutura de arquvo .class
 * \return Resultado de sucesso ou erros
 */
int classVerifier(ArqClass* arqClass){
    
    //Se a classe estiver em uma versao > MajorVersion.MinorVersion
    if (arqClass->major_version > LECLASS_MAJ_Version) return LinkageError_UnsupportedClassVersionError;
    if (arqClass->major_version == LECLASS_MAJ_Version && arqClass->minor_version > LECLASS_MIN_Version)
        return LinkageError_UnsupportedClassVersionError;;
    
    //Se a classe for a sua propria superclasse
    if (strcmp(getClassNameFromConstantPool(arqClass->constant_pool, arqClass->super_class), getClassNameFromConstantPool(arqClass->constant_pool, arqClass->this_class)) == 0)
        return LinkageError_ClassCirculatityError;
    
    return LinkageSuccess;
    
}



//--------------------------------------------------------------------------------------------------
/*!
 * Método que retorna o numero de bytes ocupados pelo descritor de campo
 *
 * \param descriptor Referencia para descritor de campo
 * \return Numero de bytes ocupados pelo descritor
 */
int classGetFieldDescriptorSize(char* descriptor){
    
    if (strcmp(descriptor, "B")== 0){
        return 1;
    }
    else if (strcmp(descriptor, "C")== 0){
        return 1;
    }
    else if (strcmp(descriptor, "D")== 0){
        return 8;
    }
    else if (strcmp(descriptor, "F")== 0){
        return 4;
    }
    else if (strcmp(descriptor, "I")== 0){
        return 4;
    }
    else if (strcmp(descriptor, "J")== 0){
        return 8;
    }
    else if (strcmp(descriptor, "S")== 0){
        return 2;
    }
    else if (strcmp(descriptor, "Z")== 0){
        return 1;
    }
    //    //Arrays
    //    else if (descriptor[0] == '['){
    //        int dimension;
    //        //Calculamos a dimensao do array
    //        for (dimension = 1; dimension < strlen(descriptor) ; dimension++) {
    //            if(descriptor[dimension] != '[') break;
    //        }
    //        //verificamos o tamanho do elemento do array e multiplicamos pela dimensao dele
    //        return dimension * classGetFieldDescriptorSize(&descriptor[dimension]);
    //    }
    //    //Referencias
    else return 4;
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que aloca o espaco de memoria necessarios para um determinado campo baseado no descritor
 *
 * \param descriptor Descritor do campo
 * \return Referencia para o ponteiro de memoria a receber o endereco do espaco alocado
 */
void* classInitializeField(char* descriptor){
    
    //Alocamos o espaco de memoria
    void* memoryAddress = (void*) malloc(classGetFieldDescriptorSize(descriptor)*sizeof(void));
    
    //Inicializamos o atributo com o valor default
    if (strcmp(descriptor, "B") == 0){
        u1* byte = memoryAddress;
        *byte = 0;
    }
    else if (strcmp(descriptor, "C")== 0){
        char* c = memoryAddress;
        *c = '0';
    }
    else if (strcmp(descriptor, "D")== 0){
        double* d = memoryAddress;
        *d = 0;
    }
    else if (strcmp(descriptor, "F")== 0){
        float* f = memoryAddress;
        *f = 0;
    }
    else if (strcmp(descriptor, "I")== 0){
        int* i = memoryAddress;
        *i = 0;
    }
    else if (strcmp(descriptor, "J")== 0){
        u8* l = memoryAddress;
        *l = 0;
    }
    else if (strcmp(descriptor, "Z")== 0){
        u1* b = memoryAddress;
        *b = 0;
    }
    else if (strcmp(descriptor, "S")== 0){
        u2* s = memoryAddress;
        *s = 0;
    }
    return memoryAddress;
}


//--------------------------------------------------------------------------------------------------
Fields* classInitializeFields(JavaClass* javaClass, u2 flagsAccept, u2 flagsRegect){
    
    //Numero de campos
    int fieldsCount = javaClass->arqClass->fields_count;
    
    //Resultado
    Fields* fields;
    
    //Alocamos espaco na tabela de campos para o numero de campos
    fields = (Fields*) malloc(sizeof(Fields));
    fields->fieldsCount = 0;
    
    fields->fieldsTable = (FieldsTable*) malloc(sizeof(FieldsTable));
    
    //Inicializamos cada campo estatico da classe
    for (int i = 0; i < fieldsCount; i++) {
        
        //Verificamos as condicoes das flags
        if (javaClass->arqClass->fields[i].access_flags & flagsAccept &&
            !(javaClass->arqClass->fields[i].access_flags & flagsRegect)){
            
            //Redimensionamos a tabela de atributos estaticos da classe
            fields->fieldsTable = (FieldsTable*) realloc(fields->fieldsTable, (fields->fieldsCount+1) * sizeof(FieldsTable));
            
            //Referencia para as informacoes do campo na estrutura de .class
            field_info* fieldInfo = &javaClass->arqClass->fields[i];
            fields->fieldsTable[fields->fieldsCount].fieldInfo = fieldInfo;
            
            
            //Passamos uma referencia para o nome
            fields->fieldsTable[fields->fieldsCount].name =
            getUTF8FromConstantPool(javaClass->arqClass->constant_pool,
                                    fieldInfo->name_index);
            
            //Passamos uma referencia para o descritor do campo
            fields->fieldsTable[fields->fieldsCount].descriptor =
            getUTF8FromConstantPool(javaClass->arqClass->constant_pool,
                                    fieldInfo->descriptor_index);
            
            //Alocamos o espaco de memoria para o campo
            fields->fieldsTable[fields->fieldsCount].memoryAddress = classInitializeField(fields->fieldsTable[fields->fieldsCount].descriptor);
            
            //Atualizamos a quantidade de fields
            fields->fieldsCount++;
        }
    }
    return fields;
}


//--------------------------------------------------------------------------------------------------
/*!
 * Método que aloca todos os espaços de memoria necessarios para a classe e inicializa os campos
 * estaticos com os valores default
 *
 * \param javaClass Referencia para a estrutura de classe a ser inicializada
 * \return Resultado de sucesso ou erros
 */
int classPreparing(JavaClass* javaClass){
    
    javaClass->staticFields = classInitializeFields(javaClass, ACC_STATIC, ACC_FINAL);
    javaClass->objectList = NULL;
    return LinkageSuccess;
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que, dada uma estrutura de arquivo .class, verifica se todas as suas superclasses ou
 * superinterfaces foram carregadas. Caso não, carrega as mesmas de forma recursiva.
 *
 * \param arqClass Nome qualificado da classe ou interface a ser carregada
 * \param environment ambiente atual de execucao
 * \return Estrutura arqClass preenchida com os dados da classe carregada
 */
void classSuperClassChecker(ArqClass* arqClass, Environment* enviroment){
    
    char* superClassName = getClassNameFromConstantPool(arqClass->constant_pool, arqClass->super_class);
    JavaClass* superClass;
    //Verificamos se a superclasse ja fora carregada. Caso nao, carregamos recursivamente
    if(findJavaClassOnMethodArea(superClassName, enviroment->methodArea) == NULL)
        superClass = loadCLass(superClassName, enviroment);
}


//--------------------------------------------------------------------------------------------------
/*!
 * Método que executa o processo de inicializacao de uma classe ou interface.
 *
 * Passos:
 *
 * 1. Verifica e, se necessario, carrega recursivamente todas as suas superclasse ou interfaces
 * 2. Executa o inicializador de campos estáticos (exceto os final static, que são definidos em tempo
 *     de compilacao)
 *
 * \param javaClass Referencia para a estrutura de classe a ser inicializada
 * \param environment ambiente atual de execucao
 * \return Estrutura arqClass preenchida com os dados da classe carregada
 */
int classInitializer(JavaClass* javaClass, Environment* environment){
    
    //Verificacao e carregamento de superclasses
    classSuperClassChecker(javaClass->arqClass, environment);
    
    //Se a classe nao tiver o metodo init
    if (!getMethodInfoFromClass(javaClass,  "<clinit>", "()V")) return InitializerSuccess;

    //Empilhamos o metodo <clinit> da classe
    environment->thread->PC--; //Metodo eh invocado sem ser chamado, precisa decrementar PC
    pushFrame(environment, getClassNameFromConstantPool(javaClass->arqClass->constant_pool, javaClass->arqClass->this_class), "<clinit>", "()V");
    
    return InitializerSuccess;
    
}


//--------------------------------------------------------------------------------------------------
JavaClass* loadCLass(const char* qualifiedName, Environment* environment){
    
    JavaClass* javaClass = (JavaClass*) malloc(sizeof(JavaClass));
    ArqClass* arqClass = (ArqClass*) malloc(sizeof(ArqClass));
    int opResult; //!< Resultado das operacoes (checagem de erros)
    
    //!LOADING - Fazemos a leitura do arquivo .class
    char* classExtension = ".class";
    
    //Nome completo do arquivo
    char* fileName = (char*)malloc((strlen(qualifiedName)+strlen(classExtension)+1) * sizeof(char));
    strcpy(fileName, qualifiedName);
    strcat(fileName, classExtension);
    
    //Lemos o arquivo .class
    opResult = LECLASS_leitor(arqClass, fileName);
    
    //Caso tenha ocorrido algum erro de leitura, apontamos
    if (opResult != LinkageSuccess) {
        LECLASS_exibeErroOperacao(opResult, fileName);
        JVMstopAbrupt(NULL);
        return NULL;
    }
    javaClass->arqClass = arqClass;
    
    //LINKING - VERIFYING - Verificamos a corretude do codigo da classe
    // Parte ja realizada na leitura do arquivo
    opResult = classVerifier(arqClass);
    if (opResult != LinkageSuccess) {
        LECLASS_exibeErroOperacao(opResult, fileName);
        JVMstopAbrupt(NULL);
        return NULL;
    }
    
    //LINKING - PREPARING - Alocamos todos os espacos de memoria necessarios para a classe
    // Campos estaticos sao criados e inicializados com os valores default
    classPreparing(javaClass);
    
    //INITIALIZATION - Executamos o o inicializador estatico <clinit>
    
    //Adicionamos a classe carregada na area de metodos
    addJavaClassToMethodArea(javaClass, environment->methodArea);
    
    classInitializer(javaClass, environment);
    
    if (environment->debugFlags & DEBUG_ShowClassFiles) LECLASS_exibidor(javaClass->arqClass);
    
    //Retornamos a estrutura inicializada
    return javaClass;
}

// Funcoes responsaveis por implementar metodos de exibicao de .class
// SUBMODULO

//--------------------------------------------------------------------------------------------------
wchar_t * getUnicodeFromConstantPool(cp_info* cp, u2 index){
    
    int i;
    wchar_t* string = (wchar_t * ) malloc( (cp[index-1].u.Utf8.lenght+1) * sizeof(wchar_t));
    
    for (i = 0; i < cp[index-1].u.Utf8.lenght; i++) string[i] = cp[index-1].u.Utf8.bytes[i];
    
    string[i] = '\0'; //Ultimo caractere
    
    return string;
}


//--------------------------------------------------------------------------------------------------
char * getUTF8FromConstantPool(cp_info* cp, u2 index){
    
    wchar_t *unicode = getUnicodeFromConstantPool(cp, index);
    char * result = (char*) malloc((wcslen(unicode)+1) * sizeof(char));
    
    wcstombs(result, unicode, wcslen(unicode)+1);
    
    free(unicode);
    
    return result;
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao dos campos nome e descritor de uma estrutura NameAndType do 
 * pool de constantes
 *
 * \param cp    ponteiro para o pool de constantes
 * \param tag   tag do NameAndType no pool de constantes.
 */
void printNameAndTypeInfoFromConstantPool(cp_info* cp, u2 index ){
    printf(" (Name: \"%ls\"", getUnicodeFromConstantPool(cp, cp[index-1].u.NameAndType.name_index));
    printf(" Descr: \"%ls\")", getUnicodeFromConstantPool(cp, cp[index-1].u.NameAndType.descriptor_index));
}


//--------------------------------------------------------------------------------------------------
char* getClassNameFromConstantPool(cp_info* cp, u2 index){
    
    return  getUTF8FromConstantPool(cp, cp[index-1].u.Class.name_index);
}


//--------------------------------------------------------------------------------------------------
void getFieldOrMethodInfoAttributesFromConstantPool(u2 index,
                                            cp_info* constant_pool,
                                            char** class_name,
                                            char** name,
                                            char** descriptor)
{
    
    //Obtemos a referencia para o field_info ou method_info no pool de constantes
    cp_info* field_or_method_info = &constant_pool[index-1];
    
    //Obtemos a referencia para o descritor
    cp_info* nameAndType_info = &constant_pool[field_or_method_info->u.Fieldref.name_and_type_index-1];
    
    //Obtemos os nomes e descritor
    *class_name = getClassNameFromConstantPool(constant_pool,
                                              field_or_method_info->u.Fieldref.class_index);
    *name = getUTF8FromConstantPool(constant_pool,
                                         nameAndType_info->u.NameAndType.name_index);
    *descriptor = getUTF8FromConstantPool(constant_pool,
                                               nameAndType_info->u.NameAndType.descriptor_index);
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que, dado uma referencia para uma lista de tabela de excecoes e seu respectivo tamanho, 
 * cria, preenche e retorna uma referencia para um vetor de estruturas exception table.
 *
 * \param code Ponteiro para o vetor que contem as excetion_tables
 * \param exception_table_length Numero de elementos na tabela de excessoes
 * \param cp ponteiro para o pool de constantes
 * \return Ponteiro para um vetor de tabela de excessoes preenchida
 */
ExceptionTable* parseExceptionTables(u1* exceptionTableList, u2 exception_table_length){
    
    u1* c = exceptionTableList;
    ExceptionTable* exceptionTable = (ExceptionTable*)
                                    malloc(exception_table_length * sizeof(ExceptionTable));
    
    for ( int i = 0; i < exception_table_length; i++) {
        
        exceptionTable[i].start_pc = *c++;
        exceptionTable[i].start_pc = exceptionTable[i].start_pc << 8 | *c++;
        
        exceptionTable[i].end_pc = *c++;
        exceptionTable[i].end_pc = exceptionTable[i].end_pc << 8 | *c++;
        
        exceptionTable[i].handler_pc = *c++;
        exceptionTable[i].handler_pc = exceptionTable[i].handler_pc << 8 | *c++;
        
        exceptionTable[i].catch_type = *c++;
        exceptionTable[i].catch_type = exceptionTable[i].catch_type << 8 | *c++;
        
    }
    
    return exceptionTable;
}


//--------------------------------------------------------------------------------------------------
CodeAttribute* parseCode(u1* info){
    CodeAttribute* code = (CodeAttribute*) malloc(sizeof(CodeAttribute));
    
    int index = 0;
    
    code->max_stack = info[index++];
    code->max_stack = code->max_stack << 8 | info[index++];
    
    code->max_locals = info[index++];
    code->max_locals = code->max_locals << 8 | info[index++];
    
    code->code_length = info[index++];
    code->code_length = code->code_length << 8 | info[index++];
    code->code_length = code->code_length << 8 | info[index++];
    code->code_length = code->code_length << 8 | info[index++];
    
    
    code->code = &info[index];
    
    index += code->code_length;
    
    code->exception_table_length = info[index++];
    code->exception_table_length = code->exception_table_length << 8 | info[index++];
    
    code->exception_table = parseExceptionTables(&info[index], code->exception_table_length);
    
    index += code->exception_table_length * 4 * sizeof(u2); //Tamanho da Tabela de excessao = 4*u2
    
    code->attributes_count = info[index++];
    code->attributes_count = code->attributes_count << 8 | info[index++];
    code->attributes = &info[index];
    
    return code;
}


//--------------------------------------------------------------------------------------------------
ConstantValueAttribute* parseConstantValue(u1* info){
    
    ConstantValueAttribute* constantValue =
    (ConstantValueAttribute*) malloc(sizeof(ConstantValueAttribute));
    
    constantValue->constantvalue_index = info[0];
    constantValue->constantvalue_index = constantValue->constantvalue_index << 8 | info[1];
    
    return constantValue;
  
}

ExceptionAttribute* parseExceptionAttribute(u1* info){
    ExceptionAttribute* exception = (ExceptionAttribute*)malloc(sizeof(ExceptionAttribute));
    int index = 0;
    
    exception->number_of_exceptions = info[index++];
    exception->number_of_exceptions = exception->number_of_exceptions << 8 | info[index++];
   
    exception->exception_index_table = (u2*) &info[index];
    
    return exception;
}

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao de um campo do pool de constantes
 *
 * \param cp               estrutura de pool de constante a ser exibida
 * \param constant_pool    ponteiro para o pool de constantes da classe
 */
void printFromPool(cp_info* cp, cp_info* constant_pool){
    
    switch (cp->tag) {
            
        case CONSTANT_Class:
            printf("CONSTANT_Class_info {");
            printf("\n\tu1 tag: %d", cp->tag);
            printf("\n\tu2 name_index: %d", cp->u.Class.name_index);
            printf(" (\"%ls\")", getUnicodeFromConstantPool(constant_pool, cp->u.Class.name_index));
            printf("\n}\n");
            break;
            
        case CONSTANT_Fieldref:
            printf("CONSTANT_Fieldref_info {");
            printf("\n\tu1 tag: %d", cp->tag);
            printf("\n\tu2 class_index: %d", cp->u.Fieldref.class_index);
            
            //Imprimimos o nome da classe indicada por class_index
            printf(" (\"%s\")", getClassNameFromConstantPool(constant_pool,
                                                         cp->u.Fieldref.class_index));
            
            printf("\n\tu2 name_and_type_index: %d", cp->u.Fieldref.name_and_type_index);
            
            //Imprimimos os atributos de name and type do indice name_and_type_index
            printNameAndTypeInfoFromConstantPool(constant_pool,
                                                 cp->u.Fieldref.name_and_type_index);
            printf("\n}\n");
            break;
            
        case CONSTANT_NameAndType:
            printf("CONSTANT_NameAndType_info {");
            printf("\n\tu1 tag: %d", cp->tag);
            printf("\n\tu2 name_index: %d", cp->u.NameAndType.name_index);
            printf(" (\"%ls\")", getUnicodeFromConstantPool(constant_pool,
                                                         cp->u.NameAndType.name_index));
            printf("\n\tu2 descriptor_index: %d", cp->u.NameAndType.descriptor_index);
            printf(" (\"%ls\")", getUnicodeFromConstantPool(constant_pool,
                                                         cp->u.NameAndType.descriptor_index));
            printf("\n}\n");
            break;
            
        case CONSTANT_Utf8:
            printf("CONSTANT_Utf8_info {");
            printf("\n\tu1 tag: %d", cp->tag);
            printf("\n\tu2 lenght: %d", cp->u.Utf8.lenght);
            printf("\n\tu1 bytes[%d]: ", cp->u.Utf8.lenght);
            printf("\"%ls\"", getUnicodeFromConstantPool(cp,1));
            printf("\n}\n");
            break;
            
        case CONSTANT_Methodref:
            printf("CONSTANT_Methodref_info {");
            printf("\n\tu1 tag: %d", cp->tag);
            printf("\n\tu2 class_index: %d", cp->u.Methodref.class_index);
            
            //Imprimimos o nome da classe indicada por class_index
            printf(" (\"%s\")", getClassNameFromConstantPool(constant_pool,
                                                             cp->u.Methodref.class_index));
            
            printf("\n\tu2 name_and_type_index: %d", cp->u.Methodref.name_and_type_index);
            
            //Imprimimos os atributos de name and type indice name_and_type_index
            printNameAndTypeInfoFromConstantPool(constant_pool,
                                                 cp->u.Methodref.name_and_type_index);
            printf("\n}\n");
            break;
            
        case CONSTANT_InterfaceMethodref:
            printf("CONSTANT_InterfaceMethodref_info {");
            printf("\n\tu1 tag: %d", cp->tag);
            printf("\n\tu2 class_index: %d", cp->u.InterfaceMethodref.class_index);
            
            //Imprimimos o nome da classe indicada por class_index
            printf(" (\"%s\")", getClassNameFromConstantPool(constant_pool,
                                                             cp->u.InterfaceMethodref.class_index));
            
            printf("\n\tu2 name_and_type_index: %d", cp->u.InterfaceMethodref.name_and_type_index);
            
            //Imprimimos os atributos de name and type indice name_and_type_index
            printNameAndTypeInfoFromConstantPool(constant_pool,
                                                 cp->u.InterfaceMethodref.name_and_type_index);
            printf("\n}\n");
            break;
            
        case CONSTANT_String:
            printf("CONSTANT_String_info {");
            printf("\n\tu1 tag: %d", cp->tag);
            printf("\n\tu2 string_index: %d", cp->u.String.string_index);
            printf(" (\"%ls\")", getUnicodeFromConstantPool(constant_pool,
                                                         cp->u.String.string_index));
            printf("\n}\n");
            break;
            
        case CONSTANT_Integer:
            printf("CONSTANT_Integer_info {");
            printf("\n\tu1 tag: %d", cp->tag);
            printf("\n\tu4 bytes: %d", cp->u.Integer.bytes);
            printf("\n}\n");
            break;
            
        case CONSTANT_Float:
            printf("CONSTANT_Float_info {");
            printf("\n\tu1 tag: %d", cp->tag);
            printf("\n\tu4 bytes: %f", u4ToFLoat(cp->u.Float.bytes));
            printf("\n}\n");
            break;
            
        case CONSTANT_Long:
            printf("CONSTANT_Long_info {");
            printf("\n\tu1 tag: %d", cp->tag);
            
            //Unimos os dois u4 em um u8 (u8)
            u8 bytesl = cp->u.Long.high_bytes;
            bytesl = bytesl << 32 | cp->u.Long.low_bytes;
            
            printf("\n\tu4 high_bytes: 0x%x", cp->u.Long.high_bytes);
            printf("\n\tu4 low_bytes: 0x%x", cp->u.Long.low_bytes);
            printf("\n\t(u8 bytes: %lld)", bytesl);
            printf("\n}\n");
            cp++; // Ocupa 2 indices
            break;
            
        case CONSTANT_Double:
            printf("CONSTANT_Double_info {");
            printf("\n\tu1 tag: %d", cp->tag);
            printf("\n\tu4 high_bytes: 0x%x", cp->u.Double.high_bytes);
            printf("\n\tu4 low_bytes: 0x%x", cp->u.Double.low_bytes);
            printf("\n\t(u8 bytes: %lf)", u4ToDouble(cp->u.Double.high_bytes, cp->u.Double.low_bytes));
            printf("\n}\n");
            cp++; // Ocupa 2 indices
            break;
            
        default:
            break;
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao do pool de constantes de uma estrutura ArqClass
 *
 * \param arq_class Estrutura ArqClass contendo o ponteiro para o pool de constantes a ser exibido
 */
void exibeCtePool(ArqClass* arq_class){

    cp_info* cp;
    //Para cada elemento do pool de constantes
    for (cp = arq_class->constant_pool;
         cp < arq_class->constant_pool + arq_class->constant_pool_count - 1;
         cp++)
    {
        printf("%d. ", (cp - arq_class->constant_pool +1));
        //Imprimimos o elemento
        printFromPool(cp, arq_class->constant_pool);
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao das flags de acesso setadas em um u2
 *
 * \param access_flags  u2 contendo as flags de acesso
 * \param is_class      int simulando booleano que verifica se a flag eh ou nao de classe
 */
void exibeAccessFlags(u2 access_flags, int is_class){
    
    if (access_flags & ACC_PUBLIC)      printf("public ");
    if (access_flags & ACC_PRIVATE)     printf("private ");
    if (access_flags & ACC_PROTECTED)   printf("protected ");
    if (access_flags & ACC_STATIC)      printf("static ");
    if (access_flags & ACC_FINAL)       printf("final ");
    if (is_class) {
        if (access_flags & ACC_SUPER)       printf("super ");
    }
    else{
        if (access_flags & ACC_SYNCHRONIZED) printf("synchronized ");
    }
    if (access_flags & ACC_VOLATILE)    printf("volatile ");
    if (access_flags & ACC_TRANSIENT)   printf("transient ");
    if (access_flags & ACC_NATIVE)      printf("native ");
    if (access_flags & ACC_INTERFACE)   printf("interface ");
    if (access_flags & ACC_ABSTRACT)    printf("abstract ");
    if (access_flags & ACC_STRICT)      printf("strict ");
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao das interfaces de uma estrutura ArqClass
 *
 * \param arq_class Estrutura ArqClass contendo o ponteiro as interfaces
 */
void exibeInterfaces(ArqClass* arq_class){
    
    for (int i = 0; i < arq_class->interfaces_count; i++) {
        printf("\n %d (\"%ls\")", arq_class->interfaces[i],
        getUnicodeFromConstantPool(arq_class->constant_pool,
                                arq_class->constant_pool[arq_class->interfaces[i]-1].u.Class.name_index));
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao de um bytecode
 *
 * \param bytecode Bytecode a ser exibido.
 * \param bytecodes Inicio do vetor de bytecodes.
 */
void printByteCode(u1* bytecode, u1* bytecodes){
    
    u1 mnemonic = bytecode[0];
    
    printf("\n\t\t\t%s", getOpcodeName(mnemonic));
    bytecode++;
    
    //Numero de atributos
    int attrNumber = getOpcodeAttributesNumber(mnemonic, bytecode, bytecodes);
    
    //TODO opcode com numero de atributos variavel (-1)
    for (int i = 0; i < attrNumber ; i++) {
        printf(" 0x%.2x", bytecode[0]);
        //Para nao passarmos para o proximo bytecode no ultimo argumento
        if(i != attrNumber-1)
            bytecode++;
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao de um vetor de bytecodes
 *
 * \param bytecodes Vetor de bytecodes
 * \param code_lenght tamanho do vetor de bytecodes
 */
void printByteCodes(u1* bytecodes, u4 code_length){
    
    u1* bc = bytecodes;
    printf("\n\t\t{ ");

    for ( bc = bytecodes; bc < bytecodes + code_length; bc+=getOpcodeAttributesNumber(*bc, bytecodes+1, bytecodes)+1) {
        printByteCode(bc, bytecodes);
    }
    printf("\n\t\t}");
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao de um atributo
 *
 * \param attribute ponteiro para o atributo a ser exibido
 * \param cp    ponteiro para o pool de constantes
 */
void exibeAtributo(attribute_info* attribute, cp_info* cp); //A ser implementada abaixo...


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao dos atributos de um Atributo CODE
 *
 * \param code Ponteiro para o vetor de atributos de code
 * \param attributes_count Numero de atributos
 * \param cp ponteiro para o pool de constantes
 */
void printCodeAttributes(u1* code, u4 attributes_count, cp_info* cp){
    
    u1* c = code;
    
    printf("\n\t\t{");
    
    for ( int i = 0; i < attributes_count; i++) {
        attribute_info* attr = (attribute_info*) malloc(sizeof(attribute_info));
        
        attr->attribute_name_index = *c++;
        attr->attribute_name_index = attr->attribute_name_index << 8 | *c++;

        attr->attribute_length = *c++;
        attr->attribute_length = attr->attribute_length << 8 | *c++;
        attr->attribute_length = attr->attribute_length << 8 | *c++;
        attr->attribute_length = attr->attribute_length << 8 | *c++;
        
        attr->info = (u1*) malloc(attr->attribute_length * sizeof(u1*));
        
        for (int j = 0; j < attr->attribute_length; j++) attr->info[j] = *c++;
        
        exibeAtributo(attr, cp);
        
        free(attr->info);
        free(attr);
    }
    printf("\n\t\t}");
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao das tabelas de excessoes de um Atributo CODE
 *
 * \param code Ponteiro para uma lista de exceptionTables
 * \param exception_table_length Numero de elementos na tabela de excessoes
 * \param cp ponteiro para o pool de constantes
 */
void printCodeExceptions(ExceptionTable* excTable, u2 exception_table_length, cp_info* cp){
    
    printf("\n\t\t{");
    
    for ( int i = 0; i < exception_table_length; i++) {
        printf("\n\t\t\t{");
        printf("\n\t\t\tSTART_PC: %d", excTable[i].start_pc);
        printf("\n\t\t\tEND_PC: %d", excTable[i].end_pc);
        printf("\n\t\t\tHANDLER_PC: %d", excTable[i].handler_pc);
        printf("\n\t\t\tCATCH_TYPE: %d", excTable[i].catch_type);
        
        printf("\n(((((((((((((((\n");
        printFromPool(&cp[excTable[i].catch_type], cp);
        printf(")))))))))))))))\n");
        printf("\n\t\t\t}");
    }
    printf("\n\t\t}");
}


//--------------------------------------------------------------------------------------------------
//
void exibeAtributo(attribute_info* attribute, cp_info* cp){
    
    //Exibimos o nome
    printf("\n\t\tNAME_INDEX:\t\t %d ", attribute->attribute_name_index);
    printf("(\"%ls\")", getUnicodeFromConstantPool(cp, attribute->attribute_name_index));
    
    //tamanho em bytes do restande do atributo
    printf("\n\t\tATTRIBUTES_LENGHT:\t %d ", attribute->attribute_length);
    
    //Verificamos qual o tipo de atributo
    
    //Caso seja ConstantValue
    if (wcscmp(getUnicodeFromConstantPool(cp, attribute->attribute_name_index), ATT_ConstantValue)==0) {
        
        ConstantValueAttribute* constantValue = parseConstantValue(attribute->info);
        
        printf("\n\t\tCONSTANT_VALUE INDEX:\t\t %d ", constantValue->constantvalue_index);
        printf("\n(((((((((((((((\n");
        printFromPool(&cp[constantValue->constantvalue_index-1], cp);
        printf(")))))))))))))))\n");
        
        free(constantValue);
        
    }
    //Caso seja CODE
    else if (wcscmp(getUnicodeFromConstantPool(cp, attribute->attribute_name_index), ATT_Code)==0) {
        
        CodeAttribute* code = parseCode(attribute->info);
        
        printf("\n\t\tMAX_STACK:\t\t %d ", code->max_stack);
        
        printf("\n\t\tMAX_LOCALS:\t\t %d ", code->max_locals);
        
        printf("\n\t\tCODE_LENGHT:\t\t %d ", code->code_length);
     
        printByteCodes(code->code, code->code_length);
        
        printf("\n\t\tEXCEPTION_TABLE_LENGHT:\t %d ", code->exception_table_length);
        printCodeExceptions(code->exception_table, code->exception_table_length, cp);
        
        printf("\n\t\tATTRIBUTES_COUNT:\t %d ", code->attributes_count);
        printCodeAttributes(code->attributes, code->attributes_count, cp);
        
        free(code);
    }
    //Caso seja Exception
    else if (wcscmp(getUnicodeFromConstantPool(cp, attribute->attribute_name_index), ATT_Exceptions)==0) {
        
        ExceptionAttribute* exception = parseExceptionAttribute(attribute->info);
        
        printf("\n\t\tNUMBER_OF_EXECPTIONS:\t\t %d ", exception->number_of_exceptions);
        
        for (int i = 0; i < exception->number_of_exceptions; i ++) {
            printf("\n%d. (\n", i);
            printFromPool(&cp[exception->exception_index_table[i]-1], cp);
            printf(")\n");
        }
    }
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao dos atributos de uma estrutura arqClass
 *
 * \param arqClass ponteiro a estrutura com os atributos a serem exibidos
 */
void exibeAtributos(ArqClass* arq_class){
    
    for (int i = 0; i < arq_class->attributes_count; i++) {
        printf("\n%d.{", i+1);
        exibeAtributo(&arq_class->attributes[i], arq_class->constant_pool);
        printf("\n}");
    }
    
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao de campos ou metodos
 *
 * \param fm    ponteiro para o campo ou metodo a ser exibido
 * \param count quantidade de campos ou metodos a serem exibidos
 * \param cp    ponteiro para o pool de constantes
 */
void exibeCampMetd(field_or_method* fm, u2 count, cp_info* cp){

    for (int i = 0; i < count; i++) {
        
        printf("\n%d.{", i+1);
        //Exibimos as flags de acesso
        printf("\n\tACCESS_FLAGS:\t\t %d ", fm->access_flags);
        printf("( ");
        exibeAccessFlags(fm[i].access_flags, 0);
        printf(")");
        
        //Exibimos o nome
        printf("\n\tNAME_INDEX:\t\t %d ", fm[i].name_index);
        printf("(\"%ls\")", getUnicodeFromConstantPool(cp,
                                                    fm[i].name_index));
        
        //Exibimos o descritor
        printf("\n\tDESCRIPTOR_INDEX:\t %d ", fm[i].descriptor_index);
        printf("(\"%ls\")", getUnicodeFromConstantPool(cp,
                                                    fm[i].descriptor_index));
        
        //Exibimos os atributos
        printf("\n\tATTRIBUTES_COUNT:\t %d ", fm[i].attributes_count);
        for (int j= 0; j< fm[i].attributes_count; j++) {
            printf("\n\t%d.{", j+1);
            exibeAtributo(&fm[i].attributes[j], cp);
            printf("\n\t}");
        }
        
        printf("\n  }");
        
    }

}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao dos campos de uma estrutura arqClass
 *
 * \param arq_class Estrutura ArqClass
 */
void exibeCampos(ArqClass* arq_class){
    exibeCampMetd(arq_class->fields, arq_class->fields_count, arq_class->constant_pool);
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que que realiza a exibicao dos metodos de uma estrutura arqClass
 *
 * \param arq_class Estrutura ArqClass
 */
void exibeMetodos(ArqClass* arq_class){
    exibeCampMetd(arq_class->methods, arq_class->methods_count, arq_class->constant_pool);
}


//--------------------------------------------------------------------------------------------------
OPresult LECLASS_exibidor(ArqClass* arq_class){
    
    printf("\n####################################################\n");
    printf("DETALHES DO ARQUIVO .CLASS");
    printf("\n####################################################\n");
    printf("MAGIC:\t\t\t 0x%x", arq_class->magic);
    printf("\nMINOR_VERSION:\t\t %d", arq_class->minor_version);
    printf("\nMAJOR_VERSION:\t\t %d", arq_class->major_version);
    printf("\nCONSTANT_POOL_COUNT:\t %d", arq_class->constant_pool_count);
    printf("\n----------------------------------------------------\n");
    printf("CONSTANT POOL:");
    printf("\n----------------------------------------------------\n");
    exibeCtePool(arq_class);
    printf("\n----------------------------------------------------\n");
    printf("ACCESS_FLAGS:\t %d ", arq_class->access_flags);
    printf("( ");
    exibeAccessFlags(arq_class->access_flags, 1);
    printf(")");
    
    printf("\nTHIS_CLASS:\t %d ", arq_class->this_class);
    printf("(\"%ls\")",
           getUnicodeFromConstantPool(arq_class->constant_pool,
                                   arq_class->constant_pool[arq_class->this_class-1].u.Class.name_index));
    printf("\nSUPER_CLASS:\t %d ", arq_class->super_class);
    printf("(\"%ls\")",
           getUnicodeFromConstantPool(arq_class->constant_pool,
                                   arq_class->constant_pool[arq_class->super_class-1].u.Class.name_index));
    
    printf("\nINTERFACES_COUNT: %d", arq_class->interfaces_count);
    printf("\n----------------------------------------------------\n");
    printf("INTERFACES: ");
    exibeInterfaces(arq_class);
    printf("\n----------------------------------------------------\n");
    
    printf("FIELDS_COUNT:\t %d", arq_class->fields_count);
    printf("\n----------------------------------------------------\n");
    printf("FIELDS: ");
    exibeCampos(arq_class);
    printf("\n----------------------------------------------------\n");
    
    printf("METHODS_COUNT:\t %d", arq_class->methods_count);
    printf("\n----------------------------------------------------\n");
    printf("METHODS: ");
    exibeMetodos(arq_class);
    printf("\n----------------------------------------------------\n");
    
    printf("ATTRIBUTES_COUNT:\t %d", arq_class->attributes_count);
    printf("\n----------------------------------------------------\n");
    printf("ATTRIBUTES: ");
    exibeAtributos(arq_class);
    printf("\n----------------------------------------------------\n");
    printf("\n####################################################\n");
    
    return LinkageSuccess;

}


//--------------------------------------------------------------------------------------------------
void LECLASS_exibeErroOperacao(OPresult resultado, const char* fileName){
    
    switch (resultado) {
        case LinkageError_NoClassDefFoundError:
            printf("\nErro de abertura do arquivo \"%s\".\n", fileName);
            break;
            
        case LinkageError_ClassFormatError:
            printf("\nArquivo .class invalido. Arquivo:\"%s\".\n", fileName);
            break;
            
        case LinkageError_UnsupportedClassVersionError:
            printf("\nA versao do arquivo .class eh imcompativel. Arquivo: \"%s\".\n", fileName);
            break;
            
        case LinkageError_ClassCirculatityError:
            printf("\nDetectado erro de circularidade de classes. Arquivo: \"%s\".\n", fileName);
            break;
            
        default:
            printf("\nErro de linkagem. Arquivo: \"%s\".\n", fileName);
            break;
    }

}

// SUBMODULO
// Leitura de ".class"

//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que le o pool de constantes de constantes de um arquivo ".class", retornando um ponteiro 
 * para uma estrutura inicializada de pool de constantes.
 *
 * \param constant_pool_count   Contador do pool de constantes.
 * \param arq                   Arquivo a ser lido.
 */
cp_info* leCtePool(u2 constant_pool_count, FILE* arq){

    cp_info* constant_pool = (cp_info *) malloc(constant_pool_count * sizeof(cp_info));
    cp_info* cp;
    
    for (cp = constant_pool; cp < constant_pool + constant_pool_count - 1; cp++) {

        cp->tag = u1Le(arq); //Leitura da tag
        
        //Preenchemos os campos de acordo com o tipo de estrutura apontada pela tag
        switch (cp->tag) {
        
            case CONSTANT_Class:
                cp->u.Class.name_index = u2Le(arq);
                break;
                
            case CONSTANT_Fieldref:
                cp->u.Fieldref.class_index = u2Le(arq);
                cp->u.Fieldref.name_and_type_index = u2Le(arq);
                break;
                
            case CONSTANT_NameAndType:
                cp->u.NameAndType.name_index = u2Le(arq);
                cp->u.NameAndType.descriptor_index = u2Le(arq);
                break;
                
            case CONSTANT_Utf8:
                cp->u.Utf8.lenght = u2Le(arq);
                //Alocamos espaco para o vetor de caracteres
                cp->u.Utf8.bytes = (u1 *) malloc(cp->u.Utf8.lenght * sizeof(u1));
                //Lemos cada um dos caracteres
                for (int i = 0; i < cp->u.Utf8.lenght; i++) cp->u.Utf8.bytes[i] = u1Le(arq);
                break;
                
            case CONSTANT_Methodref:
                cp->u.Methodref.class_index = u2Le(arq);
                cp->u.Methodref.name_and_type_index = u2Le(arq);
                break;
                
            case CONSTANT_InterfaceMethodref:
                cp->u.InterfaceMethodref.class_index = u2Le(arq);
                cp->u.InterfaceMethodref.name_and_type_index = u2Le(arq);
                break;
                
            case CONSTANT_String:
                cp->u.String.string_index = u2Le(arq);
                break;
                
            case CONSTANT_Integer:
                cp->u.Integer.bytes = u4Le(arq);
                break;
                
            case CONSTANT_Float:
                cp->u.Float.bytes = u4Le(arq);
                break;
                
            case CONSTANT_Long:
                cp->u.Long.high_bytes = u4Le(arq);
                cp->u.Long.low_bytes = u4Le(arq);
                cp++; //Long ocupa 2 indices no pool de constantes
                break;
                
            case CONSTANT_Double:
                cp->u.Double.high_bytes = u4Le(arq);
                cp->u.Double.low_bytes = u4Le(arq);
                cp++; //Double ocupa 2 indices no pool de constantes
                break;
                
            default:
                break;
        }
    }
    return constant_pool;
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que le o um atributo de um arquivo ".class" e preenche a estrutura attribute_info
 * passada como parametro
 *
 * \param atributo Atributo a ser preenchido
 * \param arq Arquivo a ser lido.
 */
void leAtributo(attribute_info* atributo, FILE* arq){
    
    atributo->attribute_name_index = u2Le(arq);
    atributo->attribute_length = u4Le(arq);
    atributo->info = (u1 *) malloc(atributo->attribute_length * sizeof(u1));
    for (int i = 0; i < atributo->attribute_length; i++) atributo->info[i] = u1Le(arq);
    
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que le o uma estrutura referente a um campo ou metodo de um arquivo ".class", retornando
 * um ponteiro para uma estrutura inicializada de campo ou metodo.
 *
 * \param count Numero de metodos ou campos a serem lidos.
 * \param arq   Arquivo a ser lido.
 */
field_or_method* leCampMetd(u2 count, FILE* arq){
    
    field_or_method* resultado = (field_or_method * ) malloc(count * sizeof(field_or_method));
    field_or_method* fi;
    
    for (fi = resultado; fi < resultado + count; fi++) {
        fi->access_flags = u2Le(arq);
        fi->name_index = u2Le(arq);
        fi->descriptor_index = u2Le(arq);
        fi->attributes_count = u2Le(arq);

        //Lemos os atributos
        fi->attributes = (attribute_info *) malloc(fi->attributes_count * sizeof(attribute_info));
        for (int i = 0; i < fi->attributes_count; i++) leAtributo(&fi->attributes[i], arq);
        
    }
    
    return resultado;
}


//--------------------------------------------------------------------------------------------------
/*!
 * Metodo que realiza a leitura de um arquivo ".class" preeenchendo uma estrutura ArqClass.
 *
 * \param arq_class Estrutura de arquivo ".class" a ser preenchida
 * \param arq       Arquivo ".class" a ser lido.
 * \return          Resultado da operacao
 */
OPresult arquivoParaArqClass(ArqClass* arq_class, FILE* arq){
    
    //Lemos e verificamos o Magic e as versoes
    arq_class->magic = u4Le(arq);
    if (arq_class->magic != 0xcafebabe) return LinkageError_ClassFormatError;
    arq_class->minor_version = u2Le(arq);
    arq_class->major_version = u2Le(arq);
    
    //Constant pool
    arq_class->constant_pool_count = u2Le(arq);
    arq_class->constant_pool = leCtePool(arq_class->constant_pool_count, arq); //Lemos o constant pool

    arq_class->access_flags = u2Le(arq);
    arq_class->this_class = u2Le(arq);
    arq_class->super_class = u2Le(arq);
    
    
    //Lemos as interfaces
    arq_class->interfaces_count = u2Le(arq);
    arq_class->interfaces = (u2 *) malloc(arq_class->interfaces_count* sizeof(u2));
    for (int i = 0; i < arq_class->interfaces_count; i++) arq_class->interfaces[i]=u2Le(arq);

    //Lemos os campos
    arq_class->fields_count = u2Le(arq);
    arq_class->fields = leCampMetd(arq_class->fields_count, arq);
    
    //Lemos os metodos
    arq_class->methods_count = u2Le(arq);
    arq_class->methods = leCampMetd(arq_class->methods_count, arq);
    
    //Lemos os atributos
    arq_class->attributes_count = u2Le(arq);
    arq_class->attributes = (attribute_info *)
                            malloc(arq_class->attributes_count * sizeof(attribute_info));
    for (int i = 0; i < arq_class->attributes_count; i++) leAtributo(&arq_class->attributes[i], arq);
    
    return LinkageSuccess;
}


//--------------------------------------------------------------------------------------------------
OPresult LECLASS_leitor(ArqClass* arq_class, const char* arq){

    FILE *entrada;

    //Obtemos o arquivo de entrada
    entrada = obter_entrada(arq);
    
    //Erro de abertura de arquivo
    if (!entrada) return LinkageError_NoClassDefFoundError;

    //Lemos os dados e salvamos na estrutura ArqClass
    OPresult result = arquivoParaArqClass(arq_class, entrada);
    
    fclose(entrada);
    
    return result;

}

//--------------------------------------------------------------------------------------------------
// SUBMODULO: Manipulacao de Arquivos
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
FILE* obter_entrada(const char *arq){

   FILE *arquivo; // Arquivo a ser carregado e retornado

   arquivo = fopen(arq, "rb");
    
    if (arquivo) return arquivo;

return NULL;

}


//--------------------------------------------------------------------------------------------------
u1 u1Le(FILE *arquivo){

    return getc(arquivo);
}


//--------------------------------------------------------------------------------------------------
u2 u2Le(FILE *arquivo){
    
    u2 resultado = getc(arquivo);
    
    resultado = (resultado << 8) | (getc(arquivo));
    
    return resultado;
}


//--------------------------------------------------------------------------------------------------
u4 u4Le(FILE *arquivo){
    
    u4 resultado = u2Le(arquivo);

    resultado = (resultado << 16 | u2Le(arquivo));
    
    return resultado;
}
