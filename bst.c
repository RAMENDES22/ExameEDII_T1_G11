#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

static Nota *nota_criar(const char *disciplina, float valor, int de_grupo) {
    Nota *n = malloc(sizeof(Nota));
    if (n == NULL) return NULL;
    strncpy(n->disciplina, disciplina, TAM_DISCIPLINA - 1);
    n->disciplina[TAM_DISCIPLINA - 1] = '\0';
    n->valor = valor;
    n->de_grupo = de_grupo;
    n->esq = NULL;
    n->dir = NULL;
    return n;
}

Nota *nota_inserir(Nota *raiz, const char *disciplina, float valor, int de_grupo) {
    int cmp;
    if (raiz == NULL) return nota_criar(disciplina, valor, de_grupo);

    cmp = strcmp(disciplina, raiz->disciplina);
    if (cmp < 0) raiz->esq = nota_inserir(raiz->esq, disciplina, valor, de_grupo);
    else if (cmp > 0) raiz->dir = nota_inserir(raiz->dir, disciplina, valor, de_grupo);
    else { raiz->valor = valor; raiz->de_grupo = de_grupo; }
    return raiz;
}

static Nota *nota_minimo(Nota *raiz) {
    while (raiz->esq != NULL) raiz = raiz->esq;
    return raiz;
}

/* remoção clássica de BST: nó com 2 filhos é substituído pelo sucessor in-order */
Nota *nota_remover(Nota *raiz, const char *disciplina) {
    int cmp;
    if (raiz == NULL) return NULL;

    cmp = strcmp(disciplina, raiz->disciplina);
    if (cmp < 0) { raiz->esq = nota_remover(raiz->esq, disciplina); return raiz; }
    if (cmp > 0) { raiz->dir = nota_remover(raiz->dir, disciplina); return raiz; }

    if (raiz->esq == NULL) { Nota *t = raiz->dir; free(raiz); return t; }
    if (raiz->dir == NULL) { Nota *t = raiz->esq; free(raiz); return t; }

    {
        Nota *sucessor = nota_minimo(raiz->dir);
        strncpy(raiz->disciplina, sucessor->disciplina, TAM_DISCIPLINA - 1);
        raiz->disciplina[TAM_DISCIPLINA - 1] = '\0';
        raiz->valor = sucessor->valor;
        raiz->de_grupo = sucessor->de_grupo;
        raiz->dir = nota_remover(raiz->dir, sucessor->disciplina);
    }
    return raiz;
}

void nota_imprimir(const Nota *raiz) {
    if (raiz == NULL) return;
    nota_imprimir(raiz->esq);
    printf("      - %-20s %.2f%s\n", raiz->disciplina, raiz->valor,
           raiz->de_grupo ? " (grupo)" : "");
    nota_imprimir(raiz->dir);
}

static void soma_contagem(const Nota *raiz, float *soma, int *contagem) {
    if (raiz == NULL) return;
    *soma += raiz->valor;
    (*contagem)++;
    soma_contagem(raiz->esq, soma, contagem);
    soma_contagem(raiz->dir, soma, contagem);
}

float nota_calcular_media(const Nota *raiz) {
    float soma = 0.0f;
    int contagem = 0;
    soma_contagem(raiz, &soma, &contagem);
    return contagem == 0 ? 0.0f : soma / (float)contagem;
}

void nota_liberar(Nota *raiz) {
    if (raiz == NULL) return;
    nota_liberar(raiz->esq);
    nota_liberar(raiz->dir);
    free(raiz);
}
