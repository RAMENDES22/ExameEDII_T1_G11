#ifndef FICHEIRO_H
#define FICHEIRO_H

#include "estruturas.h"

/* Formato (uma entidade por linha, ';' como separador):
   PROFESSOR;nome;email
   ANO;ano
   TURMA;id;nome
   GRUPO;id
   ALUNO;id;nome
   NOTA;disciplina;valor;de_grupo
   Cada linha pertence à última entidade-pai lida (ALUNO->GRUPO->TURMA->ANO). */

int salvar_professor(const Professor *prof, const char *caminho);
int carregar_professor(Professor *prof, const char *caminho);

#endif
