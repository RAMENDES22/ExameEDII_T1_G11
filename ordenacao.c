#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ordenacao.h"
#include "relatorio.h" /* aluno_calcular_media, aluno_aprovado, MEDIA_APROVACAO */

/* ============================================================
   Recolha dos alunos da turma para um vector dinamico
   ============================================================ */

/* Conta quantos alunos existem em todos os grupos da turma. */
static int contar_alunos(const Turma *turma) {
    int total = 0;
    const Grupo *g = turma == NULL ? NULL : turma->grupos_inicio;
    while (g != NULL) {
        total += g->num_alunos;
        g = g->seguinte;
    }
    return total;
}

int ordenacao_recolher_turma(const Turma *turma, RegistoAluno **out_vector) {
    int total, i = 0;
    const Grupo *g;
    RegistoAluno *vetor;

    if (out_vector == NULL) return 0;
    *out_vector = NULL;
    if (turma == NULL) return 0;

    total = contar_alunos(turma);
    if (total == 0) return 0;

    vetor = malloc((size_t)total * sizeof(RegistoAluno));
    if (vetor == NULL) return 0; /* falha de alocacao */

    /* percorre grupos -> alunos, calculando a media uma unica vez */
    for (g = turma->grupos_inicio; g != NULL; g = g->seguinte) {
        const Aluno *a = g->alunos_inicio;
        while (a != NULL) {
            vetor[i].aluno = (Aluno *)a;
            vetor[i].media = aluno_calcular_media(a);
            i++;
            a = a->seguinte;
        }
    }

    *out_vector = vetor;
    return total;
}

/* ============================================================
   Comparador segundo o criterio
   Devolve <0, 0 ou >0 (ordem ja final, sem inversoes posteriores).
   ============================================================ */
static int comparar(const RegistoAluno *a, const RegistoAluno *b,
                    CriterioOrdenacao criterio) {
    switch (criterio) {
        case ORD_MEDIA_DESC:
            if (a->media < b->media) return 1;
            if (a->media > b->media) return -1;
            return strcmp(a->aluno->nome, b->aluno->nome); /* desempate por nome */
        case ORD_MEDIA_ASC:
            if (a->media < b->media) return -1;
            if (a->media > b->media) return 1;
            return strcmp(a->aluno->nome, b->aluno->nome);
        case ORD_NOME_ASC:
            return strcmp(a->aluno->nome, b->aluno->nome);
        case ORD_NOME_DESC:
            return strcmp(b->aluno->nome, a->aluno->nome);
        default:
            return 0;
    }
}

/* ============================================================
   Quicksort (usado nos criterios de media)
   Particao de Lomuto com pivot no ultimo elemento.
   ============================================================ */
static void trocar(RegistoAluno *x, RegistoAluno *y) {
    RegistoAluno tmp = *x;
    *x = *y;
    *y = tmp;
}

static int particionar(RegistoAluno *v, int baixo, int alto, CriterioOrdenacao crit) {
    RegistoAluno pivot = v[alto];
    int i = baixo - 1;
    int j;
    for (j = baixo; j < alto; j++) {
        if (comparar(&v[j], &pivot, crit) < 0) {
            i++;
            trocar(&v[i], &v[j]);
        }
    }
    trocar(&v[i + 1], &v[alto]);
    return i + 1;
}

static void quicksort(RegistoAluno *v, int baixo, int alto, CriterioOrdenacao crit) {
    if (baixo < alto) {
        int p = particionar(v, baixo, alto, crit);
        quicksort(v, baixo, p - 1, crit);
        quicksort(v, p + 1, alto, crit);
    }
}

/* ============================================================
   Merge sort (usado nos criterios de nome)
   Divisao e conquista, estavel.
   ============================================================ */
static void intercalar(RegistoAluno *v, int inicio, int meio, int fim,
                       RegistoAluno *aux, CriterioOrdenacao crit) {
    int i = inicio, j = meio + 1, k = inicio;
    while (i <= meio && j <= fim) {
        if (comparar(&v[i], &v[j], crit) <= 0) aux[k++] = v[i++];
        else aux[k++] = v[j++];
    }
    while (i <= meio) aux[k++] = v[i++];
    while (j <= fim)  aux[k++] = v[j++];
    for (k = inicio; k <= fim; k++) v[k] = aux[k];
}

static void merge_rec(RegistoAluno *v, int inicio, int fim,
                      RegistoAluno *aux, CriterioOrdenacao crit) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        merge_rec(v, inicio, meio, aux, crit);
        merge_rec(v, meio + 1, fim, aux, crit);
        intercalar(v, inicio, meio, fim, aux, crit);
    }
}

static void mergesort(RegistoAluno *v, int n, CriterioOrdenacao crit) {
    RegistoAluno *aux;
    if (n < 2) return;
    aux = malloc((size_t)n * sizeof(RegistoAluno));
    if (aux == NULL) { /* sem memoria auxiliar: recorre ao quicksort */
        quicksort(v, 0, n - 1, crit);
        return;
    }
    merge_rec(v, 0, n - 1, aux, crit);
    free(aux);
}

/* ============================================================
   Selector de algoritmo
   ============================================================ */
void ordenacao_ordenar(RegistoAluno *vetor, int n, CriterioOrdenacao criterio) {
    if (vetor == NULL || n < 2) return;
    if (criterio == ORD_MEDIA_DESC || criterio == ORD_MEDIA_ASC) {
        quicksort(vetor, 0, n - 1, criterio); /* media -> Quicksort */
    } else {
        mergesort(vetor, n, criterio);        /* nome  -> Merge sort */
    }
}

/* ============================================================
   Apresentacao do ranking (ecra e ficheiro)
   ============================================================ */
static const char *texto_criterio(CriterioOrdenacao c) {
    switch (c) {
        case ORD_MEDIA_DESC: return "media (maior -> menor), Quicksort";
        case ORD_MEDIA_ASC:  return "media (menor -> maior), Quicksort";
        case ORD_NOME_ASC:   return "nome (A -> Z), Merge sort";
        case ORD_NOME_DESC:  return "nome (Z -> A), Merge sort";
        default:             return "desconhecido";
    }
}

void ordenacao_imprimir_ranking(const Turma *turma, CriterioOrdenacao criterio) {
    RegistoAluno *vetor = NULL;
    int n, i;

    if (turma == NULL) { printf("Turma nao encontrada.\n"); return; }

    n = ordenacao_recolher_turma(turma, &vetor);
    if (n == 0) { printf("A turma nao tem alunos para ordenar.\n"); return; }

    ordenacao_ordenar(vetor, n, criterio);

    printf("===== Ranking da turma %d: %s =====\n", turma->id, turma->nome);
    printf(" Criterio: %s\n", texto_criterio(criterio));
    printf(" %-4s %-30s %-7s %s\n", "Pos", "Aluno", "Media", "Estado");
    for (i = 0; i < n; i++) {
        printf(" %-4d %-30s %-7.2f %s\n",
               i + 1,
               vetor[i].aluno->nome,
               vetor[i].media,
               vetor[i].media >= MEDIA_APROVACAO ? "APROVADO" : "REPROVADO");
    }

    free(vetor);
}

int ordenacao_gravar_ranking(const Turma *turma, CriterioOrdenacao criterio,
                             const char *caminho) {
    RegistoAluno *vetor = NULL;
    FILE *f;
    int n, i;

    if (turma == NULL || caminho == NULL) return 0;

    n = ordenacao_recolher_turma(turma, &vetor);
    if (n == 0) return 0;

    ordenacao_ordenar(vetor, n, criterio);

    f = fopen(caminho, "w");
    if (f == NULL) { free(vetor); return 0; }

    fprintf(f, "===== Ranking da turma %d: %s =====\n", turma->id, turma->nome);
    fprintf(f, "Criterio: %s\n", texto_criterio(criterio));
    fprintf(f, "%-4s %-30s %-7s %s\n", "Pos", "Aluno", "Media", "Estado");
    for (i = 0; i < n; i++) {
        fprintf(f, "%-4d %-30s %-7.2f %s\n",
                i + 1,
                vetor[i].aluno->nome,
                vetor[i].media,
                vetor[i].media >= MEDIA_APROVACAO ? "APROVADO" : "REPROVADO");
    }

    fclose(f);
    free(vetor);
    return 1;
}
