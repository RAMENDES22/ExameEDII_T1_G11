#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "estruturas.h"

/* TAD de ordenacao.
   Demonstra os algoritmos de ordenacao da cadeira aplicados aos dados
   de uma turma. Os alunos estao distribuidos por varios grupos (listas
   ligadas); este modulo "achata" essa estrutura para um vector dinamico
   e ordena-o segundo o criterio escolhido:

     - Por media  -> Quicksort  (particionamento, O(n log n) medio)
     - Por nome   -> Merge sort (divisao e conquista, O(n log n) estavel)

   A escolha de dois algoritmos distintos e propositada: serve para
   exemplificar duas estrategias classicas de ordenacao estudadas na
   cadeira de Estrutura de Dados II. */

typedef enum {
    ORD_MEDIA_DESC = 1, /* ranking de merito: maior media primeiro */
    ORD_MEDIA_ASC  = 2,
    ORD_NOME_ASC   = 3, /* ordem alfabetica A-Z */
    ORD_NOME_DESC  = 4
} CriterioOrdenacao;

/* Par (aluno, media) usado durante a ordenacao para evitar recalcular
   a media a cada comparacao. */
typedef struct {
    Aluno *aluno;
    float  media;
} RegistoAluno;

/* Recolhe todos os alunos de todos os grupos da turma para um vector
   dinamico de RegistoAluno (com a media ja calculada). Devolve o numero
   de alunos e escreve o vector em *out_vector. O chamador deve libertar
   o vector com free(). Em caso de turma vazia, *out_vector fica NULL. */
int ordenacao_recolher_turma(const Turma *turma, RegistoAluno **out_vector);

/* Ordena o vector de RegistoAluno segundo o criterio indicado.
   Aplica Quicksort para criterios de media e Merge sort para nome. */
void ordenacao_ordenar(RegistoAluno *vetor, int n, CriterioOrdenacao criterio);

/* Imprime no ecra o ranking ordenado da turma. */
void ordenacao_imprimir_ranking(const Turma *turma, CriterioOrdenacao criterio);

/* Grava o ranking ordenado da turma num ficheiro de saida.
   Devolve 1 em caso de sucesso, 0 em caso de falha. */
int ordenacao_gravar_ranking(const Turma *turma, CriterioOrdenacao criterio,
                             const char *caminho);

#endif
