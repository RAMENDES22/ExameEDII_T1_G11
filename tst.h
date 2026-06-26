#ifndef TST_H
#define TST_H

#include "estruturas.h"

/* Guarda apenas ponteiros para Aluno; nunca os liberta (ver tst_liberar). */
TSTNode *tst_inserir(TSTNode *raiz, const char *chave, Aluno *aluno);
Aluno   *tst_buscar(TSTNode *raiz, const char *chave);
TSTNode *tst_remover_chave(TSTNode *raiz, const char *chave);
void     tst_liberar(TSTNode *raiz);

#endif
