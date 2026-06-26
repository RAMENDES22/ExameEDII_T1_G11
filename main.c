#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estruturas.h"
#include "avl.h"
#include "lista.h"
#include "bst.h"
#include "tst.h"
#include "mensagem.h"
#include "relatorio.h"
#include "ficheiro.h"

/*

GRUPO 11
Hugo Jeremias - 20241103
Augusto Nangayafina - 20242970
Rafaela Mendes - 20201124

*/

static void ler_linha(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') destino[len - 1] = '\0';
    } else {
        destino[0] = '\0';
    }
}

static int ler_inteiro(const char *prompt) {
    char buffer[64];
    printf("%s", prompt);
    ler_linha(buffer, sizeof(buffer));
    return atoi(buffer);
}

static float ler_float(const char *prompt) {
    char buffer[64];
    printf("%s", prompt);
    ler_linha(buffer, sizeof(buffer));
    return (float)atof(buffer);
}

static Turma *localizar_turma(Professor *prof, int ano, int id_turma) {
    AnoLetivo *al = avl_ano_buscar(prof->anos, ano);
    return al == NULL ? NULL : avl_turma_buscar(al->turmas, id_turma);
}

static void menu_registar_ano(Professor *prof) {
    int ano = ler_inteiro("Ano (ex. 2025): ");
    AnoLetivo *novo = ano_criar(ano);
    if (novo == NULL) return;
    prof->anos = avl_ano_inserir(prof->anos, novo);
    printf("Ano letivo %d/%d registado.\n", ano, ano + 1);
}

static void menu_inserir_turma(Professor *prof) {
    int ano, id;
    char nome[TAM_NOME];
    AnoLetivo *al;
    Turma *nova;

    ano = ler_inteiro("Ano letivo a que pertence: ");
    al = avl_ano_buscar(prof->anos, ano);
    if (al == NULL) { printf("Ano letivo nao encontrado.\n"); return; }

    id = ler_inteiro("Id da turma: ");
    printf("Nome da turma: ");
    ler_linha(nome, sizeof(nome));

    nova = turma_criar(id, nome);
    if (nova == NULL) return;
    al->turmas = avl_turma_inserir(al->turmas, nova);
    printf("Turma inserida.\n");
}

static void menu_inserir_grupo(Professor *prof) {
    int ano, id_turma, id;
    Turma *turma;
    Grupo *novo;

    ano = ler_inteiro("Ano letivo: ");
    id_turma = ler_inteiro("Id da turma: ");
    turma = localizar_turma(prof, ano, id_turma);
    if (turma == NULL) { printf("Turma nao encontrada.\n"); return; }

    id = ler_inteiro("Id do grupo: ");
    novo = grupo_criar(id);
    if (novo == NULL) return;
    turma_adicionar_grupo(turma, novo);
    printf("Grupo inserido.\n");
}

static void menu_inserir_membro(Professor *prof) {
    int ano, id_turma, id_grupo, id_aluno;
    char nome[TAM_NOME];
    Turma *turma;
    Grupo *grupo;
    Aluno *novo;

    ano = ler_inteiro("Ano letivo: ");
    id_turma = ler_inteiro("Id da turma: ");
    turma = localizar_turma(prof, ano, id_turma);
    if (turma == NULL) { printf("Turma nao encontrada.\n"); return; }

    id_grupo = ler_inteiro("Id do grupo: ");
    grupo = turma_buscar_grupo(turma, id_grupo);
    if (grupo == NULL) { printf("Grupo nao encontrado.\n"); return; }

    id_aluno = ler_inteiro("Id do aluno: ");
    printf("Nome do aluno: ");
    ler_linha(nome, sizeof(nome));

    novo = aluno_criar(id_aluno, nome);
    if (novo == NULL) return;
    grupo_adicionar_aluno(grupo, novo, &prof->indice_alunos);
    printf("Aluno inserido no grupo.\n");
}

static void menu_inserir_nota(Professor *prof) {
    int ano, id_turma, id_grupo, tipo;
    char disciplina[TAM_DISCIPLINA];
    float valor;
    Turma *turma;
    Grupo *grupo;

    ano = ler_inteiro("Ano letivo: ");
    id_turma = ler_inteiro("Id da turma: ");
    turma = localizar_turma(prof, ano, id_turma);
    if (turma == NULL) { printf("Turma nao encontrada.\n"); return; }

    id_grupo = ler_inteiro("Id do grupo: ");
    grupo = turma_buscar_grupo(turma, id_grupo);
    if (grupo == NULL) { printf("Grupo nao encontrado.\n"); return; }

    tipo = ler_inteiro("Nota (1) individual ou (2) de grupo? ");
    printf("Disciplina: ");
    ler_linha(disciplina, sizeof(disciplina));
    valor = ler_float("Valor da nota: ");

    if (tipo == 2) {
        grupo_inserir_nota_grupo(grupo, disciplina, valor);
        printf("Nota de grupo lancada para todos os membros.\n");
    } else {
        int id_aluno = ler_inteiro("Id do aluno: ");
        grupo_inserir_nota_individual(grupo, id_aluno, disciplina, valor);
        printf("Nota individual lancada.\n");
    }
}

static void menu_calcular_media(Professor *prof) {
    char nome[TAM_NOME];
    Aluno *aluno;

    printf("Nome do aluno: ");
    ler_linha(nome, sizeof(nome));
    aluno = tst_buscar(prof->indice_alunos, nome);
    if (aluno == NULL) { printf("Aluno nao encontrado.\n"); return; }
    printf("Media de %s: %.2f -> %s\n", aluno->nome, aluno_calcular_media(aluno),
           aluno_aprovado(aluno, MEDIA_APROVACAO) ? "APROVADO" : "REPROVADO");
}

static void menu_imprimir_relatorio(Professor *prof) {
    int ano = ler_inteiro("Ano letivo: ");
    int id_turma = ler_inteiro("Id da turma: ");
    relatorio_imprimir_turma(localizar_turma(prof, ano, id_turma));
}

static void menu_upload_relatorio(Professor *prof) {
    int ano, id_turma;
    char caminho[TAM_CAMINHO];
    Turma *turma;

    ano = ler_inteiro("Ano letivo: ");
    id_turma = ler_inteiro("Id da turma: ");
    turma = localizar_turma(prof, ano, id_turma);
    if (turma == NULL) { printf("Turma nao encontrada.\n"); return; }

    printf("Caminho do ficheiro de saida: ");
    ler_linha(caminho, sizeof(caminho));
    if (relatorio_upload_turma(turma, caminho)) printf("Relatorio gravado em '%s'.\n", caminho);
}

static void menu_enviar_mensagem(Professor *prof) {
    int ano, id_turma, destino;
    char conteudo[TAM_MENSAGEM];
    Turma *turma;

    ano = ler_inteiro("Ano letivo: ");
    id_turma = ler_inteiro("Id da turma: ");
    turma = localizar_turma(prof, ano, id_turma);
    if (turma == NULL) { printf("Turma nao encontrada.\n"); return; }

    destino = ler_inteiro("Enviar para (1) todos os grupos ou (2) um grupo? ");
    printf("Mensagem: ");
    ler_linha(conteudo, sizeof(conteudo));

    if (destino == 1) {
        turma_enviar_mensagem_para_todos(turma, prof->nome, conteudo);
        printf("Mensagem enviada a todos os grupos.\n");
    } else {
        int id_grupo = ler_inteiro("Id do grupo: ");
        Grupo *grupo = turma_buscar_grupo(turma, id_grupo);
        if (grupo == NULL) { printf("Grupo nao encontrado.\n"); return; }
        grupo_enviar_mensagem(grupo, prof->nome, conteudo);
        printf("Mensagem enviada ao grupo.\n");
    }
}

static void menu_ler_caixa_grupo(Professor *prof) {
    int ano, id_turma, id_grupo;
    Turma *turma;
    Grupo *grupo;

    ano = ler_inteiro("Ano letivo: ");
    id_turma = ler_inteiro("Id da turma: ");
    turma = localizar_turma(prof, ano, id_turma);
    if (turma == NULL) { printf("Turma nao encontrada.\n"); return; }

    id_grupo = ler_inteiro("Id do grupo: ");
    grupo = turma_buscar_grupo(turma, id_grupo);
    if (grupo == NULL) { printf("Grupo nao encontrado.\n"); return; }

    printf("Caixa de mensagens do grupo %d:\n", grupo->id);
    fila_imprimir(grupo->caixa_mensagens);
}

static void menu_remover(Professor *prof) {
    int opcao, removido = 0;
    printf("Remover: (1) ano, (2) turma, (3) grupo, (4) aluno, (5) nota\n");
    opcao = ler_inteiro("Escolha: ");

    switch (opcao) {
        case 1: {
            int ano = ler_inteiro("Ano a remover: ");
            prof->anos = avl_ano_remover(prof->anos, ano, prof->indice_alunos, &removido);
            break;
        }
        case 2: {
            int ano = ler_inteiro("Ano letivo: ");
            int id_turma = ler_inteiro("Id da turma a remover: ");
            AnoLetivo *al = avl_ano_buscar(prof->anos, ano);
            if (al != NULL) al->turmas = avl_turma_remover(al->turmas, id_turma, prof->indice_alunos, &removido);
            break;
        }
        case 3: {
            int ano = ler_inteiro("Ano letivo: ");
            int id_turma = ler_inteiro("Id da turma: ");
            int id_grupo = ler_inteiro("Id do grupo a remover: ");
            Turma *turma = localizar_turma(prof, ano, id_turma);
            if (turma != NULL) removido = turma_remover_grupo(turma, id_grupo, prof->indice_alunos);
            break;
        }
        case 4: {
            int ano = ler_inteiro("Ano letivo: ");
            int id_turma = ler_inteiro("Id da turma: ");
            int id_grupo = ler_inteiro("Id do grupo: ");
            int id_aluno = ler_inteiro("Id do aluno a remover: ");
            Turma *turma = localizar_turma(prof, ano, id_turma);
            Grupo *grupo = turma != NULL ? turma_buscar_grupo(turma, id_grupo) : NULL;
            if (grupo != NULL) removido = grupo_remover_aluno(grupo, id_aluno, prof->indice_alunos);
            break;
        }
        case 5: {
            int ano = ler_inteiro("Ano letivo: ");
            int id_turma = ler_inteiro("Id da turma: ");
            int id_grupo = ler_inteiro("Id do grupo: ");
            int id_aluno = ler_inteiro("Id do aluno: ");
            char disciplina[TAM_DISCIPLINA];
            Turma *turma;
            Grupo *grupo;
            Aluno *aluno;

            printf("Disciplina da nota a remover: ");
            ler_linha(disciplina, sizeof(disciplina));

            turma = localizar_turma(prof, ano, id_turma);
            grupo = turma != NULL ? turma_buscar_grupo(turma, id_grupo) : NULL;
            aluno = grupo != NULL ? grupo_buscar_aluno(grupo, id_aluno) : NULL;
            if (aluno != NULL) { aluno->notas = nota_remover(aluno->notas, disciplina); removido = 1; }
            break;
        }
        default:
            printf("Opcao invalida.\n");
            return;
    }
    printf(removido ? "Removido com sucesso.\n" : "Nao encontrado.\n");
}

static void menu_salvar(const Professor *prof) {
    char caminho[TAM_CAMINHO];
    printf("Caminho para gravar: ");
    ler_linha(caminho, sizeof(caminho));
    if (salvar_professor(prof, caminho)) printf("Dados gravados em '%s'.\n", caminho);
}

static void menu_carregar(Professor *prof) {
    char caminho[TAM_CAMINHO];
    printf("Caminho a carregar: ");
    ler_linha(caminho, sizeof(caminho));
    if (carregar_professor(prof, caminho)) printf("Dados carregados de '%s'.\n", caminho);
}

static void imprimir_menu(void) {
    printf("\n===== Sistema de gestao de notas =====\n");
    printf(" 1. Registar ano letivo\n");
    printf(" 2. Inserir turma\n");
    printf(" 3. Inserir grupo\n");
    printf(" 4. Inserir membro (aluno) num grupo\n");
    printf(" 5. Inserir nota (individual ou de grupo)\n");
    printf(" 6. Calcular media de um aluno\n");
    printf(" 7. Imprimir relatorio de uma turma\n");
    printf(" 8. Upload (gravar) do relatorio de uma turma\n");
    printf(" 9. Enviar mensagem (professor -> turma/grupo)\n");
    printf("10. Ler caixa de mensagens de um grupo\n");
    printf("11. Remover (ano/turma/grupo/aluno/nota)\n");
    printf("12. Salvar todos os dados em ficheiro\n");
    printf("13. Carregar todos os dados de ficheiro\n");
    printf("14. Imprimir tudo\n");
    printf(" 0. Sair\n");
}

int main(void) {
    Professor prof;
    int opcao;

    prof.anos = NULL;
    prof.indice_alunos = NULL;
    prof.caixa_mensagens = fila_criar();
    if (prof.caixa_mensagens == NULL) return EXIT_FAILURE;

    printf("Nome do professor: ");
    ler_linha(prof.nome, sizeof(prof.nome));
    printf("Email institucional: ");
    ler_linha(prof.email, sizeof(prof.email));

    do {
        imprimir_menu();
        opcao = ler_inteiro("Opcao: ");
        switch (opcao) {
            case 1:  menu_registar_ano(&prof); break;
            case 2:  menu_inserir_turma(&prof); break;
            case 3:  menu_inserir_grupo(&prof); break;
            case 4:  menu_inserir_membro(&prof); break;
            case 5:  menu_inserir_nota(&prof); break;
            case 6:  menu_calcular_media(&prof); break;
            case 7:  menu_imprimir_relatorio(&prof); break;
            case 8:  menu_upload_relatorio(&prof); break;
            case 9:  menu_enviar_mensagem(&prof); break;
            case 10: menu_ler_caixa_grupo(&prof); break;
            case 11: menu_remover(&prof); break;
            case 12: menu_salvar(&prof); break;
            case 13: menu_carregar(&prof); break;
            case 14: avl_ano_imprimir(prof.anos); break;
            case 0:  printf("A encerrar...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    /* liberta tudo em cascata: anos -> turmas -> grupos -> alunos -> notas */
    avl_ano_liberar(prof.anos, NULL);
    tst_liberar(prof.indice_alunos);
    fila_liberar(prof.caixa_mensagens);
    return EXIT_SUCCESS;
}
