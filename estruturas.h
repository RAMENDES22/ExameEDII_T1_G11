#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

/* Tipos usados em todo o projeto.
   Ownership: AnoLetivo possui a AVL de Turmas; Turma possui a lista
   de Grupos; Grupo possui a lista de Alunos; Aluno possui a BST de
   Notas. A TST guarda apenas ponteiros para Aluno (nao liberta). */

#include <stddef.h>

#define TAM_NOME       100
#define TAM_EMAIL      100
#define TAM_DISCIPLINA  50
#define TAM_MENSAGEM   256
#define TAM_CAMINHO    256

typedef struct Nota {
    char disciplina[TAM_DISCIPLINA];
    float valor;
    int de_grupo;
    struct Nota *esq;
    struct Nota *dir;
} Nota;

typedef struct Aluno {
    int id;
    char nome[TAM_NOME];
    Nota *notas;
    struct Aluno *anterior;
    struct Aluno *seguinte;
} Aluno;

typedef struct TSTNode {
    char caractere;
    Aluno *aluno;
    struct TSTNode *esq;
    struct TSTNode *meio;
    struct TSTNode *dir;
} TSTNode;

typedef struct Mensagem {
    char remetente[TAM_NOME];
    char conteudo[TAM_MENSAGEM];
    char ficheiro_anexo[TAM_CAMINHO];
    struct Mensagem *seguinte;
} Mensagem;

typedef struct Fila {
    Mensagem *inicio;
    Mensagem *fim;
} Fila;

typedef struct Grupo {
    int id;
    Aluno *alunos_inicio;
    Aluno *alunos_fim;
    int num_alunos;
    Fila *caixa_mensagens;
    struct Grupo *anterior;
    struct Grupo *seguinte;
} Grupo;

typedef struct Turma {
    int id;
    char nome[TAM_NOME];
    Grupo *grupos_inicio;
    Grupo *grupos_fim;
    int altura;
    struct Turma *esq;
    struct Turma *dir;
} Turma;

typedef struct AnoLetivo {
    int ano;
    Turma *turmas;
    int altura;
    struct AnoLetivo *esq;
    struct AnoLetivo *dir;
} AnoLetivo;

typedef struct Professor {
    char nome[TAM_NOME];
    char email[TAM_EMAIL];
    Fila *caixa_mensagens;
    AnoLetivo *anos;
    TSTNode *indice_alunos;
} Professor;

#endif
