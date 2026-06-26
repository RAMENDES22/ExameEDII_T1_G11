# Sistema de Gestão de Notas — Exame EDII (T1 — G11)

Trabalho final da cadeira de **Estrutura de Dados II** (2025/2) — Instituto
Superior Politécnico de Tecnologias e Ciências. Aplicação de consola, escrita
integralmente em **C**, para a gestão académica de notas por um professor,
demonstrando algoritmos de ordenação, busca e estruturas de dados.

## Estruturas de dados utilizadas

| Entidade | Estrutura | Critério |
|----------|-----------|----------|
| Anos lectivos | Árvore **AVL** | busca/ordem por ano |
| Turmas | Árvore **AVL** | busca/ordem por id |
| Grupos | Lista **duplamente ligada** | ordem de inserção |
| Alunos | Lista **duplamente ligada** | ordem de inserção |
| Notas | Árvore **BST** | busca/ordem por disciplina |
| Índice de alunos | **TST** (árvore de pesquisa ternária) | busca por nome |
| Mensagens | **Fila** (FIFO) | ordem de chegada |
| Ranking | **Quicksort** / **Merge sort** | média ou nome |

A organização segue um modelo de *ownership* em cascata: o professor possui os
anos, cada ano possui as turmas, cada turma possui os grupos, cada grupo possui
os alunos e cada aluno possui as suas notas. A libertação de memória é feita em
cascata a partir da raiz.

## Tipos Abstractos de Dados (TADs)

Cada estrutura está encapsulada num par de ficheiros `.h` / `.c`:

- `estruturas.h` — definição dos tipos
- `avl.{h,c}` — AVL de anos e de turmas
- `bst.{h,c}` — BST de notas
- `tst.{h,c}` — índice ternário de alunos
- `lista.{h,c}` — listas de grupos e alunos
- `mensagem.{h,c}` — fila de mensagens
- `relatorio.{h,c}` — médias, aprovação e relatórios
- `ordenacao.{h,c}` — ranking por ordenação (Quicksort / Merge sort)
- `ficheiro.{h,c}` — persistência (gravar/carregar)
- `main.c` — interface e menu

## Como compilar e executar

```bash
make            # compila -> binário "notas"
make run        # executa com o ficheiro entrada_teste.txt
make valgrind   # verifica fugas de memória
make clean      # remove binários e objectos
```

Em alternativa, sem `make`:

```bash
gcc -Wall -Wextra -std=c11 *.c -o notas
./notas
```

## Funcionalidades do menu

1. Registar ano lectivo
2. Inserir turma
3. Inserir grupo
4. Inserir membro (aluno) num grupo
5. Inserir nota (individual ou de grupo, validada entre 0 e 20)
6. Calcular média de um aluno
7. Imprimir relatório de uma turma
8. Gravar relatório de uma turma em ficheiro
9. Enviar mensagem (professor → turma/grupo)
10. Ler caixa de mensagens de um grupo
11. Remover (ano / turma / grupo / aluno / nota)
12. Salvar todos os dados em ficheiro
13. Carregar todos os dados de ficheiro
14. Imprimir tudo
15. **Ranking de alunos de uma turma (ordenação)**
16. **Gravar ranking de uma turma em ficheiro**

## Ficheiros de dados

- `entrada_teste.txt` — sequência de comandos de demonstração (entrada)
- `ranking_turma1.txt`, `relatorio_turma1.txt`, `sistema.txt` — saídas geradas em execução

## Qualidade

- Compila sem avisos com `-Wall -Wextra`.
- Sem fugas nem acessos inválidos de memória (verificado com `valgrind`).
- Código comentado e indentado.
