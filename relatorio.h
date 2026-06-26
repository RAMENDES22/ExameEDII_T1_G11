#ifndef RELATORIO_H
#define RELATORIO_H

#include "estruturas.h"

#define MEDIA_APROVACAO 10.0f

float aluno_calcular_media(const Aluno *aluno);
int   aluno_aprovado(const Aluno *aluno, float media_minima);
void  relatorio_imprimir_turma(const Turma *turma);
int   relatorio_upload_turma(const Turma *turma, const char *caminho);

#endif
