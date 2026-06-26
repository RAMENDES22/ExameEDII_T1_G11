#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mensagem.h"

Fila *fila_criar(void) {
    Fila *f = malloc(sizeof(Fila));
    if (f == NULL) return NULL;
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

int fila_enviar(Fila *fila, const char *remetente, const char *conteudo, const char *ficheiro_anexo) {
    Mensagem *m;
    if (fila == NULL) return 0;

    m = malloc(sizeof(Mensagem));
    if (m == NULL) return 0;
    strncpy(m->remetente, remetente, TAM_NOME - 1); m->remetente[TAM_NOME - 1] = '\0';
    strncpy(m->conteudo, conteudo, TAM_MENSAGEM - 1); m->conteudo[TAM_MENSAGEM - 1] = '\0';
    if (ficheiro_anexo != NULL) {
        strncpy(m->ficheiro_anexo, ficheiro_anexo, TAM_CAMINHO - 1);
        m->ficheiro_anexo[TAM_CAMINHO - 1] = '\0';
    } else {
        m->ficheiro_anexo[0] = '\0';
    }
    m->seguinte = NULL;

    if (fila->fim == NULL) { fila->inicio = m; fila->fim = m; }
    else { fila->fim->seguinte = m; fila->fim = m; }
    return 1;
}

int fila_receber(Fila *fila, Mensagem *destino) {
    Mensagem *m;
    if (fila == NULL || fila->inicio == NULL) return 0;
    m = fila->inicio;
    fila->inicio = m->seguinte;
    if (fila->inicio == NULL) fila->fim = NULL;
    if (destino != NULL) { memcpy(destino, m, sizeof(Mensagem)); destino->seguinte = NULL; }
    free(m);
    return 1;
}

void fila_imprimir(const Fila *fila) {
    const Mensagem *atual = fila == NULL ? NULL : fila->inicio;
    if (atual == NULL) { printf("    (caixa vazia)\n"); return; }
    while (atual != NULL) {
        printf("    [%s]: %s%s\n", atual->remetente, atual->conteudo,
               atual->ficheiro_anexo[0] ? " (com anexo)" : "");
        atual = atual->seguinte;
    }
}

void fila_liberar(Fila *fila) {
    Mensagem *atual, *proxima;
    if (fila == NULL) return;
    atual = fila->inicio;
    while (atual != NULL) { proxima = atual->seguinte; free(atual); atual = proxima; }
    free(fila);
}
