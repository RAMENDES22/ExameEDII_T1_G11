#include <stdio.h>
#include "relatorio.h"
#include "bst.h"

float aluno_calcular_media(const Aluno *aluno) {
    return aluno == NULL ? 0.0f : nota_calcular_media(aluno->notas);
}

int aluno_aprovado(const Aluno *aluno, float media_minima) {
    return aluno != NULL && aluno_calcular_media(aluno) >= media_minima;
}

static void imprimir_aluno(const Aluno *aluno) {
    float media = aluno_calcular_media(aluno);
    printf("  Aluno %d - %s\n", aluno->id, aluno->nome);
    nota_imprimir(aluno->notas);
    printf("    Media: %.2f -> %s\n", media, aluno_aprovado(aluno, MEDIA_APROVACAO) ? "APROVADO" : "REPROVADO");
}

void relatorio_imprimir_turma(const Turma *turma) {
    const Grupo *g;
    if (turma == NULL) { printf("(turma nao encontrada)\n"); return; }
    printf("===== Relatorio da turma %d: %s =====\n", turma->id, turma->nome);
    g = turma->grupos_inicio;
    if (g == NULL) printf(" (sem grupos registados)\n");
    while (g != NULL) {
        const Aluno *a = g->alunos_inicio;
        printf(" Grupo %d\n", g->id);
        while (a != NULL) { imprimir_aluno(a); a = a->seguinte; }
        g = g->seguinte;
    }
}

int relatorio_upload_turma(const Turma *turma, const char *caminho) {
    FILE *f;
    const Grupo *g;
    if (turma == NULL || caminho == NULL) return 0;

    f = fopen(caminho, "w");
    if (f == NULL) return 0;

    fprintf(f, "===== Relatorio da turma %d: %s =====\n", turma->id, turma->nome);
    g = turma->grupos_inicio;
    while (g != NULL) {
        const Aluno *a = g->alunos_inicio;
        fprintf(f, " Grupo %d\n", g->id);
        while (a != NULL) {
            fprintf(f, "  Aluno %d - %s | Media: %.2f | %s\n", a->id, a->nome,
                    aluno_calcular_media(a), aluno_aprovado(a, MEDIA_APROVACAO) ? "APROVADO" : "REPROVADO");
            a = a->seguinte;
        }
        g = g->seguinte;
    }
    fclose(f);
    return 1;
}
