#ifndef BST_H
#define BST_H

#include "estruturas.h"

Nota *nota_inserir(Nota *raiz, const char *disciplina, float valor, int de_grupo);
Nota *nota_remover(Nota *raiz, const char *disciplina);
void  nota_imprimir(const Nota *raiz);
float nota_calcular_media(const Nota *raiz);
void  nota_liberar(Nota *raiz);

#endif
