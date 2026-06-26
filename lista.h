#ifndef LISTA_H
#define LISTA_H

#include "estruturas.h"

/* Alunos */
Aluno *aluno_criar(int id, const char *nome);
int    grupo_adicionar_aluno(Grupo *grupo, Aluno *aluno, TSTNode **indice_global);
Aluno *grupo_buscar_aluno(Grupo *grupo, int id);
int    grupo_remover_aluno(Grupo *grupo, int id, TSTNode *indice_global);
void   grupo_imprimir_alunos(const Grupo *grupo);
void   grupo_liberar_alunos(Grupo *grupo);
void   grupo_remover_indice_tst_todos(Grupo *grupo, TSTNode *indice_global);
void   grupo_inserir_nota_individual(Grupo *grupo, int id_aluno, const char *disciplina, float valor);
void   grupo_inserir_nota_grupo(Grupo *grupo, const char *disciplina, float valor);

/* Grupos */
Grupo *grupo_criar(int id);
int    turma_adicionar_grupo(Turma *turma, Grupo *grupo);
Grupo *turma_buscar_grupo(Turma *turma, int id);
int    turma_remover_grupo(Turma *turma, int id, TSTNode *indice_global);
void   turma_imprimir_grupos(const Turma *turma);
void   turma_liberar_grupos(Turma *turma, TSTNode *indice_global);
void   turma_enviar_mensagem_para_todos(Turma *turma, const char *remetente, const char *conteudo);
void   grupo_enviar_mensagem(Grupo *grupo, const char *remetente, const char *conteudo);

#endif
