#ifndef AVL_H
#define AVL_H

#include "estruturas.h"

/* AVL de Turmas (dentro de um ano letivo) */
Turma *turma_criar(int id, const char *nome);
Turma *avl_turma_inserir(Turma *raiz, Turma *nova);
Turma *avl_turma_buscar(Turma *raiz, int id);
Turma *avl_turma_remover(Turma *raiz, int id, TSTNode *indice_global, int *removido);
void   avl_turma_imprimir(const Turma *raiz);
void   avl_turma_liberar(Turma *raiz, TSTNode *indice_global);

/* AVL de Anos Letivos */
AnoLetivo *ano_criar(int ano);
AnoLetivo *avl_ano_inserir(AnoLetivo *raiz, AnoLetivo *novo);
AnoLetivo *avl_ano_buscar(AnoLetivo *raiz, int ano);
AnoLetivo *avl_ano_remover(AnoLetivo *raiz, int ano, TSTNode *indice_global, int *removido);
void       avl_ano_imprimir(const AnoLetivo *raiz);
void       avl_ano_liberar(AnoLetivo *raiz, TSTNode *indice_global);

#endif
