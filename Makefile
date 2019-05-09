#############################################################################
# Makefile for building: JVM
#
# Make em sistemas Windows $. make compilar
#
#############################################################################

CC = gcc
STD = -std=c99
LM = -lm

INCLUDE_HEADER = include/*.h

JAVA_FUNCTIONS_SOURCE = java/*.c

CLASS_LOADER_SOURCE = classloader.c
EXECUTION_ENGINE_SOURCE = executionengine.c
MEMORY_UNIT_SOURCE =  memoryunit.c
UTIL_SOURCE = util.c

HEADER = $(INCLUDE_HEADER)
SOURCES = $(CLASS_LOADER_SOURCE) $(JAVA_FUNCTIONS_SOURCE) $(EXECUTION_ENGINE_SOURCE) $(MEMORY_UNIT_SOURCE) $(UTIL_SOURCE)
OUT_win = jvm
DOXYGEN_CONFIG = docs/doxygen/doxygen_config

compilar:
	$(CC) $(STD) $(HEADER) $(SOURCES) -m32 -o $(OUT_win) $(LM)

gera_doxygen:
	doxygen  $(DOXYGEN_CONFIG)

