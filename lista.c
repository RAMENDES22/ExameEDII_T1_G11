#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "tst.h"
#include "bst.h"
#include "mensagem.h"

/* ---------- Alunos ---------- */

Aluno *aluno_criar(int id, const char *nome) {
    Aluno *a = malloc(sizeof(Aluno));
    if (a == NULL) return NULL;
    a->id = id;
    strncpy(a->nome, nome, TAM_NOME - 1);
    a->nome[TAM_NOME - 1] = '\0';
    a->notas = NULL;
    a->anterior = NULL;
    a->seguinte = NULL;
    return a;
}

int grupo_adicionar_aluno(Grupo *grupo, Aluno *aluno, TSTNode **indice_global) {
    if (grupo == NULL || aluno == NULL) return 0;

    if (grupo->alunos_inicio == NULL) {
        grupo->alunos_inicio = aluno;
        grupo->alunos_fim = aluno;
    } else {
        aluno->anterior = grupo->alunos_fim;
        grupo->alunos_fim->seguinte = aluno;
        grupo->alunos_fim = aluno;
    }
    grupo->num_alunos++;

    if (indice_global != NULL) *indice_global = tst_inserir(*indice_global, aluno->nome, aluno);
    return 1;
}

Aluno *grupo_buscar_aluno(Grupo *grupo, int id) {
    Aluno *atual = grupo == NULL ? NULL : grupo->alunos_inicio;
    while (atual != NULL) {
        if (atual->id == id) return atual;
        atual = atual->seguinte;
    }
    return NULL;
}

int grupo_remover_aluno(Grupo *grupo, int id, TSTNode *indice_global) {
    Aluno *aluno = grupo_buscar_aluno(grupo, id);
    if (aluno == NULL) return 0;

    if (aluno->anterior != NULL) aluno->anterior->seguinte = aluno->seguinte;
    else grupo->alunos_inicio = aluno->seguinte;
    if (aluno->seguinte != NULL) aluno->seguinte->anterior = aluno->anterior;
    else grupo->alunos_fim = aluno->anterior;
    grupo->num_alunos--;

    if (indice_global != NULL) tst_remover_chave(indice_global, aluno->nome);
    nota_liberar(aluno->notas);
    free(aluno);
    return 1;
}

void grupo_imprimir_alunos(const Grupo *grupo) {
    const Aluno *atual = grupo == NULL ? NULL : grupo->alunos_inicio;
    while (atual != NULL) {
        printf("  - Aluno %d: %s\n", atual->id, atual->nome);
        nota_imprimir(atual->notas);
        atual = atual->seguinte;
    }
}

void grupo_liberar_alunos(Grupo *grupo) {
    Aluno *atual, *proximo;
    if (grupo == NULL) return;
    atual = grupo->alunos_inicio;
    while (atual != NULL) {
        proximo = atual->seguinte;
        nota_liberar(atual->notas);
        free(atual);
        atual = proximo;
    }
    grupo->alunos_inicio = NULL;
    grupo->alunos_fim = NULL;
    grupo->num_alunos = 0;
}

void grupo_remover_indice_tst_todos(Grupo *grupo, TSTNode *indice_global) {
    Aluno *atual;
    if (grupo == NULL || indice_global == NULL) return;
    atual = grupo->alunos_inicio;
    while (atual != NULL) {
        tst_remover_chave(indice_global, atual->nome);
        atual = atual->seguinte;
    }
}

void grupo_inserir_nota_individual(Grupo *grupo, int id_aluno, const char *disciplina, float valor) {
    Aluno *aluno = grupo_buscar_aluno(grupo, id_aluno);
    if (aluno == NULL) { fprintf(stderr, "Aviso: aluno %d nao encontrado.\n", id_aluno); return; }
    aluno->notas = nota_inserir(aluno->notas, disciplina, valor, 0);
}

/* propaga a mesma nota a todos os membros do grupo */
void grupo_inserir_nota_grupo(Grupo *grupo, const char *disciplina, float valor) {
    Aluno *atual = grupo == NULL ? NULL : grupo->alunos_inicio;
    while (atual != NULL) {
        atual->notas = nota_inserir(atual->notas, disciplina, valor, 1);
        atual = atual->seguinte;
    }
}

/* ---------- Grupos ---------- */

Grupo *grupo_criar(int id) {
    Grupo *g = malloc(sizeof(Grupo));
    if (g == NULL) return NULL;
    g->id = id;
    g->alunos_inicio = NULL;
    g->alunos_fim = NULL;
    g->num_alunos = 0;
    g->caixa_mensagens = fila_criar();
    if (g->caixa_mensagens == NULL) { free(g); return NULL; }
    g->anterior = NULL;
    g->seguinte = NULL;
    return g;
}

int turma_adicionar_grupo(Turma *turma, Grupo *grupo) {
    if (turma == NULL || grupo == NULL) return 0;
    if (turma->grupos_inicio == NULL) {
        turma->grupos_inicio = grupo;
        turma->grupos_fim = grupo;
    } else {
        grupo->anterior = turma->grupos_fim;
        turma->grupos_fim->seguinte = grupo;
        turma->grupos_fim = grupo;
    }
    return 1;
}

Grupo *turma_buscar_grupo(Turma *turma, int id) {
    Grupo *atual = turma == NULL ? NULL : turma->grupos_inicio;
    while (atual != NULL) {
        if (atual->id == id) return atual;
        atual = atual->seguinte;
    }
    return NULL;
}

int turma_remover_grupo(Turma *turma, int id, TSTNode *indice_global) {
    Grupo *grupo = turma_buscar_grupo(turma, id);
    if (grupo == NULL) return 0;

    if (grupo->anterior != NULL) grupo->anterior->seguinte = grupo->seguinte;
    else turma->grupos_inicio = grupo->seguinte;
    if (grupo->seguinte != NULL) grupo->seguinte->anterior = grupo->anterior;
    else turma->grupos_fim = grupo->anterior;

    grupo_remover_indice_tst_todos(grupo, indice_global);
    grupo_liberar_alunos(grupo);
    fila_liberar(grupo->caixa_mensagens);
    free(grupo);
    return 1;
}

void turma_imprimir_grupos(const Turma *turma) {
    const Grupo *atual = turma == NULL ? NULL : turma->grupos_inicio;
    while (atual != NULL) {
        printf(" Grupo %d (%d aluno(s))\n", atual->id, atual->num_alunos);
        grupo_imprimir_alunos(atual);
        atual = atual->seguinte;
    }
}

void turma_liberar_grupos(Turma *turma, TSTNode *indice_global) {
    Grupo *atual, *proximo;
    if (turma == NULL) return;
    atual = turma->grupos_inicio;
    while (atual != NULL) {
        proximo = atual->seguinte;
        if (indice_global != NULL) grupo_remover_indice_tst_todos(atual, indice_global);
        grupo_liberar_alunos(atual);
        fila_liberar(atual->caixa_mensagens);
        free(atual);
        atual = proximo;
    }
    turma->grupos_inicio = NULL;
    turma->grupos_fim = NULL;
}

void grupo_enviar_mensagem(Grupo *grupo, const char *remetente, const char *conteudo) {
    if (grupo != NULL) fila_enviar(grupo->caixa_mensagens, remetente, conteudo, NULL);
}

void turma_enviar_mensagem_para_todos(Turma *turma, const char *remetente, const char *conteudo) {
    Grupo *atual = turma == NULL ? NULL : turma->grupos_inicio;
    while (atual != NULL) {
        fila_enviar(atual->caixa_mensagens, remetente, conteudo, NULL);
        atual = atual->seguinte;
    }
}
