#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ficheiro.h"
#include "avl.h"
#include "lista.h"
#include "bst.h"

/* ---------- Gravar ---------- */

static void escrever_notas(FILE *f, const Nota *n) {
    if (n == NULL) return;
    escrever_notas(f, n->esq);
    fprintf(f, "NOTA;%s;%.2f;%d\n", n->disciplina, n->valor, n->de_grupo);
    escrever_notas(f, n->dir);
}

static void escrever_alunos(FILE *f, const Aluno *a) {
    while (a != NULL) {
        fprintf(f, "ALUNO;%d;%s\n", a->id, a->nome);
        escrever_notas(f, a->notas);
        a = a->seguinte;
    }
}

static void escrever_grupos(FILE *f, const Grupo *g) {
    while (g != NULL) {
        fprintf(f, "GRUPO;%d\n", g->id);
        escrever_alunos(f, g->alunos_inicio);
        g = g->seguinte;
    }
}

static void escrever_turmas(FILE *f, const Turma *t) {
    if (t == NULL) return;
    escrever_turmas(f, t->esq);
    fprintf(f, "TURMA;%d;%s\n", t->id, t->nome);
    escrever_grupos(f, t->grupos_inicio);
    escrever_turmas(f, t->dir);
}

static void escrever_anos(FILE *f, const AnoLetivo *a) {
    if (a == NULL) return;
    escrever_anos(f, a->esq);
    fprintf(f, "ANO;%d\n", a->ano);
    escrever_turmas(f, a->turmas);
    escrever_anos(f, a->dir);
}

int salvar_professor(const Professor *prof, const char *caminho) {
    FILE *f;
    if (prof == NULL || caminho == NULL) return 0;
    f = fopen(caminho, "w");
    if (f == NULL) return 0;
    fprintf(f, "PROFESSOR;%s;%s\n", prof->nome, prof->email);
    escrever_anos(f, prof->anos);
    fclose(f);
    return 1;
}

/* ---------- Carregar ---------- */

static int dividir_campos(char *linha, char *campos[4]) {
    int n = 0;
    char *inicio = linha;
    char *p;
    char *fim_linha;
    for (p = linha; n < 3; p++) {
        if (*p == ';' || *p == '\0' || *p == '\n' || *p == '\r') {
            int fim = (*p == '\0');
            *p = '\0';
            campos[n++] = inicio;
            if (fim) return n;
            inicio = p + 1;
        }
    }
    fim_linha = inicio + strlen(inicio);
    while (fim_linha > inicio && (*(fim_linha - 1) == '\n' || *(fim_linha - 1) == '\r')) *(--fim_linha) = '\0';
    campos[n++] = inicio;
    return n;
}

int carregar_professor(Professor *prof, const char *caminho) {
    FILE *f;
    char linha[512];
    AnoLetivo *ano_atual = NULL;
    Turma *turma_atual = NULL;
    Grupo *grupo_atual = NULL;
    Aluno *aluno_atual = NULL;

    if (prof == NULL || caminho == NULL) return 0;
    f = fopen(caminho, "r");
    if (f == NULL) return 0;

    while (fgets(linha, sizeof(linha), f) != NULL) {
        char *campos[4] = {0};
        char copia[512];
        int n;
        strncpy(copia, linha, sizeof(copia) - 1);
        copia[sizeof(copia) - 1] = '\0';
        n = dividir_campos(copia, campos);
        if (n < 2) continue;

        if (strcmp(campos[0], "PROFESSOR") == 0 && n >= 3) {
            strncpy(prof->nome, campos[1], TAM_NOME - 1); prof->nome[TAM_NOME - 1] = '\0';
            strncpy(prof->email, campos[2], TAM_EMAIL - 1); prof->email[TAM_EMAIL - 1] = '\0';

        } else if (strcmp(campos[0], "ANO") == 0) {
            int ano_num = atoi(campos[1]);
            AnoLetivo *novo = ano_criar(ano_num);
            if (novo == NULL) continue;
            prof->anos = avl_ano_inserir(prof->anos, novo);
            ano_atual = avl_ano_buscar(prof->anos, ano_num);
            turma_atual = NULL; grupo_atual = NULL; aluno_atual = NULL;

        } else if (strcmp(campos[0], "TURMA") == 0 && n >= 3 && ano_atual != NULL) {
            int id = atoi(campos[1]);
            Turma *novo = turma_criar(id, campos[2]);
            if (novo == NULL) continue;
            ano_atual->turmas = avl_turma_inserir(ano_atual->turmas, novo);
            turma_atual = avl_turma_buscar(ano_atual->turmas, id);
            grupo_atual = NULL; aluno_atual = NULL;

        } else if (strcmp(campos[0], "GRUPO") == 0 && n >= 2 && turma_atual != NULL) {
            int id = atoi(campos[1]);
            Grupo *novo = grupo_criar(id);
            if (novo == NULL) continue;
            turma_adicionar_grupo(turma_atual, novo);
            grupo_atual = novo;
            aluno_atual = NULL;

        } else if (strcmp(campos[0], "ALUNO") == 0 && n >= 3 && grupo_atual != NULL) {
            int id = atoi(campos[1]);
            Aluno *novo = aluno_criar(id, campos[2]);
            if (novo == NULL) continue;
            grupo_adicionar_aluno(grupo_atual, novo, &prof->indice_alunos);
            aluno_atual = novo;

        } else if (strcmp(campos[0], "NOTA") == 0 && n >= 4 && aluno_atual != NULL) {
            float valor = (float)atof(campos[2]);
            int de_grupo = atoi(campos[3]);
            aluno_atual->notas = nota_inserir(aluno_atual->notas, campos[1], valor, de_grupo);
        }
    }

    fclose(f);
    return 1;
}
