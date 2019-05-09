#############################################################################
# jvm - Java Virtual Machine
# Aluno(s):	Cristoffer Leite 11/0146093
#	     	
#	    
#############################################################################

#----------------------------------------------------------------------------
# Suporte
#----------------------------------------------------------------------------

Suporte somente aos seguintes métodos de bibliotecas java:

	java/io/PrintStream.println
	java/lang/StringBuffer.toString
	java/lang/StringBuffer.append

#----------------------------------------------------------------------------
# Compilação e execucao
#----------------------------------------------------------------------------

Para compilar digite:

	Em sistemas Windows:
		$ make compilar


Para executar digite:
NOTA: É necessário estar em no diretório contento a(s) classe(s)

	Em sistemas Windows:
		$. a <arquivo_entrada> <String de caracteres>


#----------------------------------------------------------------------------
# Documentacao do sistema
#----------------------------------------------------------------------------

A documentacao do sistema eh gerada pelo software doxygen


A documentacao atual se encontra em docs/doxygen (nao gerada ainda)
		
	> Para abrir a documentacao entre em docs/documentacao_html
	  ou docs/doxygen/index.html


Para gerar uma nova documentacao eh necessaria a instalacao dos softwares:
	 
	> doxygen - Software de geracao de documentacao a partir de codigo
		> http://www.doxygen.org
	
	> graphviz - Software para a geracao de diagramas
		> http://www.graphviz.org

Para gerar uma nova documentação doxygen basta executar

	$ make gera_doxygen 


#----------------------------------------------------------------------------
# Descricao dos diretorios
#----------------------------------------------------------------------------

/		- principal contendo os arquivos .c com o codigo fonte

bin/		- diretorio contendo os arquivos binarios executaveis

docs/		- diretorio contendo a documentacao gerada com o doxygen

java/		- diretorio com as funcoes simuladas do java e funcoes de opcodes

include/	- diretorio contendo arquivos de interface

libs/		- classes de exemplo