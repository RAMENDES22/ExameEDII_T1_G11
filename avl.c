#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "lista.h"

static int max_int(int a, int b) { return a > b ? a : b; }

/* ================= AVL de Turmas ================= */

static int altura_t(const Turma *n) { return n == NULL ? 0 : n->altura; }
static int fb_t(const Turma *n) { return n == NULL ? 0 : altura_t(n->esq) - altura_t(n->dir); }
static void att_altura_t(Turma *n) { if (n) n->altura = 1 + max_int(altura_t(n->esq), altura_t(n->dir)); }

static Turma *rot_dir_t(Turma *y) {
    Turma *x = y->esq; y->esq = x->dir; x->dir = y;
    att_altura_t(y); att_altura_t(x);
    return x;
}
static Turma *rot_esq_t(Turma *x) {
    Turma *y = x->dir; x->dir = y->esq; y->esq = x;
    att_altura_t(x); att_altura_t(y);
    return y;
}
static Turma *balancear_t(Turma *n) {
    int fb;
    att_altura_t(n);
    fb = fb_t(n);
    if (fb > 1) { if (fb_t(n->esq) < 0) n->esq = rot_esq_t(n->esq); return rot_dir_t(n); }
    if (fb < -1) { if (fb_t(n->dir) > 0) n->dir = rot_dir_t(n->dir); return rot_esq_t(n); }
    return n;
}

Turma *turma_criar(int id, const char *nome) {
    Turma *t = malloc(sizeof(Turma));
    if (t == NULL) return NULL;
    t->id = id;
    strncpy(t->nome, nome, TAM_NOME - 1);
    t->nome[TAM_NOME - 1] = '\0';
    t->grupos_inicio = NULL;
    t->grupos_fim = NULL;
    t->altura = 1;
    t->esq = NULL;
    t->dir = NULL;
    return t;
}

Turma *avl_turma_inserir(Turma *raiz, Turma *nova) {
    if (raiz == NULL) return nova;
    if (nova->id < raiz->id) raiz->esq = avl_turma_inserir(raiz->esq, nova);
    else if (nova->id > raiz->id) raiz->dir = avl_turma_inserir(raiz->dir, nova);
    else { free(nova); return raiz; } /* id duplicado: ignora */
    return balancear_t(raiz);
}

Turma *avl_turma_buscar(Turma *raiz, int id) {
    if (raiz == NULL) return NULL;
    if (id == raiz->id) return raiz;
    return id < raiz->id ? avl_turma_buscar(raiz->esq, id) : avl_turma_buscar(raiz->dir, id);
}

static Turma *minimo_t(Turma *raiz) { while (raiz->esq != NULL) raiz = raiz->esq; return raiz; }

/* Nó com 2 filhos: antes de copiar os dados do sucessor, liberta a
   lista de grupos atual (vai ser substituída) e, ao adotar a lista
   do sucessor, desliga-a dele para a remoção recursiva não a
   libertar de novo (evita leak e double free). */
Turma *avl_turma_remover(Turma *raiz, int id, TSTNode *indice_global, int *removido) {
    if (raiz == NULL) { if (removido) *removido = 0; return NULL; }

    if (id < raiz->id) raiz->esq = avl_turma_remover(raiz->esq, id, indice_global, removido);
    else if (id > raiz->id) raiz->dir = avl_turma_remover(raiz->dir, id, indice_global, removido);
    else {
        if (removido) *removido = 1;
        if (raiz->esq == NULL || raiz->dir == NULL) {
            Turma *sub = raiz->esq != NULL ? raiz->esq : raiz->dir;
            turma_liberar_grupos(raiz, indice_global);
            free(raiz);
            return sub;
        }
        {
            Turma *sucessor = minimo_t(raiz->dir);
            turma_liberar_grupos(raiz, indice_global);
            raiz->id = sucessor->id;
            strncpy(raiz->nome, sucessor->nome, TAM_NOME - 1);
            raiz->nome[TAM_NOME - 1] = '\0';
            raiz->grupos_inicio = sucessor->grupos_inicio;
            raiz->grupos_fim = sucessor->grupos_fim;
            sucessor->grupos_inicio = NULL;
            sucessor->grupos_fim = NULL;
            raiz->dir = avl_turma_remover(raiz->dir, sucessor->id, indice_global, NULL);
        }
    }
    return balancear_t(raiz);
}

void avl_turma_imprimir(const Turma *raiz) {
    if (raiz == NULL) return;
    avl_turma_imprimir(raiz->esq);
    printf("Turma %d: %s\n", raiz->id, raiz->nome);
    turma_imprimir_grupos(raiz);
    avl_turma_imprimir(raiz->dir);
}

void avl_turma_liberar(Turma *raiz, TSTNode *indice_global) {
    if (raiz == NULL) return;
    avl_turma_liberar(raiz->esq, indice_global);
    avl_turma_liberar(raiz->dir, indice_global);
    turma_liberar_grupos(raiz, indice_global);
    free(raiz);
}

/* ================= AVL de Anos Letivos ================= */

static int altura_a(const AnoLetivo *n) { return n == NULL ? 0 : n->altura; }
static int fb_a(const AnoLetivo *n) { return n == NULL ? 0 : altura_a(n->esq) - altura_a(n->dir); }
static void att_altura_a(AnoLetivo *n) { if (n) n->altura = 1 + max_int(altura_a(n->esq), altura_a(n->dir)); }

static AnoLetivo *rot_dir_a(AnoLetivo *y) {
    AnoLetivo *x = y->esq; y->esq = x->dir; x->dir = y;
    att_altura_a(y); att_altura_a(x);
    return x;
}
static AnoLetivo *rot_esq_a(AnoLetivo *x) {
    AnoLetivo *y = x->dir; x->dir = y->esq; y->esq = x;
    att_altura_a(x); att_altura_a(y);
    return y;
}
static AnoLetivo *balancear_a(AnoLetivo *n) {
    int fb;
    att_altura_a(n);
    fb = fb_a(n);
    if (fb > 1) { if (fb_a(n->esq) < 0) n->esq = rot_esq_a(n->esq); return rot_dir_a(n); }
    if (fb < -1) { if (fb_a(n->dir) > 0) n->dir = rot_dir_a(n->dir); return rot_esq_a(n); }
    return n;
}

AnoLetivo *ano_criar(int ano) {
    AnoLetivo *a = malloc(sizeof(AnoLetivo));
    if (a == NULL) return NULL;
    a->ano = ano;
    a->turmas = NULL;
    a->altura = 1;
    a->esq = NULL;
    a->dir = NULL;
    return a;
}

AnoLetivo *avl_ano_inserir(AnoLetivo *raiz, AnoLetivo *novo) {
    if (raiz == NULL) return novo;
    if (novo->ano < raiz->ano) raiz->esq = avl_ano_inserir(raiz->esq, novo);
    else if (novo->ano > raiz->ano) raiz->dir = avl_ano_inserir(raiz->dir, novo);
    else { free(novo); return raiz; }
    return balancear_a(raiz);
}

AnoLetivo *avl_ano_buscar(AnoLetivo *raiz, int ano) {
    if (raiz == NULL) return NULL;
    if (ano == raiz->ano) return raiz;
    return ano < raiz->ano ? avl_ano_buscar(raiz->esq, ano) : avl_ano_buscar(raiz->dir, ano);
}

static AnoLetivo *minimo_a(AnoLetivo *raiz) { while (raiz->esq != NULL) raiz = raiz->esq; return raiz; }

/* mesma lógica de ownership da AVL de turmas, um nível acima */
AnoLetivo *avl_ano_remover(AnoLetivo *raiz, int ano, TSTNode *indice_global, int *removido) {
    if (raiz == NULL) { if (removido) *removido = 0; return NULL; }

    if (ano < raiz->ano) raiz->esq = avl_ano_remover(raiz->esq, ano, indice_global, removido);
    else if (ano > raiz->ano) raiz->dir = avl_ano_remover(raiz->dir, ano, indice_global, removido);
    else {
        if (removido) *removido = 1;
        if (raiz->esq == NULL || raiz->dir == NULL) {
            AnoLetivo *sub = raiz->esq != NULL ? raiz->esq : raiz->dir;
            avl_turma_liberar(raiz->turmas, indice_global);
            free(raiz);
            return sub;
        }
        {
            AnoLetivo *sucessor = minimo_a(raiz->dir);
            avl_turma_liberar(raiz->turmas, indice_global);
            raiz->ano = sucessor->ano;
            raiz->turmas = sucessor->turmas;
            sucessor->turmas = NULL;
            raiz->dir = avl_ano_remover(raiz->dir, sucessor->ano, indice_global, NULL);
        }
    }
    return balancear_a(raiz);
}

void avl_ano_imprimir(const AnoLetivo *raiz) {
    if (raiz == NULL) return;
    avl_ano_imprimir(raiz->esq);
    printf("=== Ano letivo %d/%d ===\n", raiz->ano, raiz->ano + 1);
    avl_turma_imprimir(raiz->turmas);
    avl_ano_imprimir(raiz->dir);
}

void avl_ano_liberar(AnoLetivo *raiz, TSTNode *indice_global) {
    if (raiz == NULL) return;
    avl_ano_liberar(raiz->esq, indice_global);
    avl_ano_liberar(raiz->dir, indice_global);
    avl_turma_liberar(raiz->turmas, indice_global);
    free(raiz);
}
