#ifndef MENSAGEM_H
#define MENSAGEM_H

#include "estruturas.h"

Fila *fila_criar(void);
int   fila_enviar(Fila *fila, const char *remetente, const char *conteudo, const char *ficheiro_anexo);
int   fila_receber(Fila *fila, Mensagem *destino);
void  fila_imprimir(const Fila *fila);
void  fila_liberar(Fila *fila);

#endif
