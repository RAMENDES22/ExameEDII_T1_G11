#include <stdlib.h>
#include "tst.h"

static TSTNode *criar_nodo(char c) {
    TSTNode *n = malloc(sizeof(TSTNode));
    if (n == NULL) return NULL;
    n->caractere = c;
    n->aluno = NULL;
    n->esq = NULL;
    n->meio = NULL;
    n->dir = NULL;
    return n;
}

TSTNode *tst_inserir(TSTNode *raiz, const char *chave, Aluno *aluno) {
    char c;
    if (chave == NULL || chave[0] == '\0') return raiz;

    c = chave[0];
    if (raiz == NULL) {
        raiz = criar_nodo(c);
        if (raiz == NULL) return NULL;
    }

    if (c < raiz->caractere) raiz->esq = tst_inserir(raiz->esq, chave, aluno);
    else if (c > raiz->caractere) raiz->dir = tst_inserir(raiz->dir, chave, aluno);
    else if (chave[1] != '\0') raiz->meio = tst_inserir(raiz->meio, chave + 1, aluno);
    else raiz->aluno = aluno;
    return raiz;
}

Aluno *tst_buscar(TSTNode *raiz, const char *chave) {
    char c;
    if (raiz == NULL || chave == NULL || chave[0] == '\0') return NULL;
    c = chave[0];
    if (c < raiz->caractere) return tst_buscar(raiz->esq, chave);
    if (c > raiz->caractere) return tst_buscar(raiz->dir, chave);
    if (chave[1] != '\0') return tst_buscar(raiz->meio, chave + 1);
    return raiz->aluno;
}

/* remoção lógica: só desliga o ponteiro, não poda nós (chaves podem
   partilhar prefixo com outras) */
TSTNode *tst_remover_chave(TSTNode *raiz, const char *chave) {
    char c;
    if (raiz == NULL || chave == NULL || chave[0] == '\0') return raiz;
    c = chave[0];
    if (c < raiz->caractere) raiz->esq = tst_remover_chave(raiz->esq, chave);
    else if (c > raiz->caractere) raiz->dir = tst_remover_chave(raiz->dir, chave);
    else if (chave[1] != '\0') raiz->meio = tst_remover_chave(raiz->meio, chave + 1);
    else raiz->aluno = NULL;
    return raiz;
}

void tst_liberar(TSTNode *raiz) {
    if (raiz == NULL) return;
    tst_liberar(raiz->esq);
    tst_liberar(raiz->meio);
    tst_liberar(raiz->dir);
    free(raiz);
}
