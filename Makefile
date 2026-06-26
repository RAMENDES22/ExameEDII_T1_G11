# Makefile - Sistema de Gestao de Notas (Exame EDII - T1 - G11)
# Compilacao com avisos rigorosos e modo de depuracao.

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
ALVO    = notas
FONTES  = $(wildcard *.c)
OBJ     = $(FONTES:.c=.o)

# Alvo por omissao: compila o programa
all: $(ALVO)

$(ALVO): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(ALVO)

# Regra generica para objectos
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Executa o programa com o ficheiro de entrada de teste
run: $(ALVO)
	./$(ALVO) < entrada_teste.txt

# Verifica fugas de memoria com o valgrind
valgrind: $(ALVO)
	valgrind --leak-check=full --show-leak-kinds=all ./$(ALVO) < entrada_teste.txt

# Remove ficheiros gerados pela compilacao
clean:
	rm -f $(OBJ) $(ALVO)

.PHONY: all run valgrind clean
