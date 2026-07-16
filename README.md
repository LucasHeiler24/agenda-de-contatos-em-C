# 📇 Projeto de Faculdade: CRUD de Contatos em C

## 👥 Integrantes

- **Lucas Heiler**

---

## 📌 Problema

O projeto tem como objetivo ser uma agenda de contatos para uma empresa que necessita armazenar informações dos clientes de maneira rápida, segura e objetiva.

## 💡 Solução

A solução foi desenvolver um software em **C ANSI** para criar, listar, editar e excluir contatos. Cada contato possui os seguintes campos:

- **Identificador** (`id`)
- **Nome**
- **Telefone**

---

## 🛠️ Protótipos das Funções

Abaixo estão as funções que estruturam o sistema, divididas por categoria de responsabilidade:

### 🎮 Interface e Menus

```c

void menu(int *opcao);
void menuAcao(int *opcao, char modo[]);
void menuEdicao(int *opcao);

```

### 📝 Operações

```c

void novoContato(Agenda agenda[], int *tamAgenda);
void listarContatos(Agenda agenda[], int tamAgenda);
void modoExcluir(FILE *arquivo, Agenda agenda[], int *tamAgenda);
void excluir(FILE *arquivo, Agenda agenda[], int *tamAgenda, int posicaoEncontrado);
void editarContato(FILE *arquivo, Agenda agenda[], int tamAgenda);
void editar(FILE *arquivo, Agenda agenda[], int tamAgenda, int posicao);
void limpar();
void mostrarSucesso(char mensagemSucesso[], char mensagemVoltar[]);

```

### 🔍 Buscas e Filtros

```c

int encontrarPorId(Agenda agenda[], int *tamAgenda, char modo[]);
int encontrarPorNome(Agenda agenda[], int *tamAgenda, char modo[]);
void buscarPorNome(Agenda agenda[], int tamAgenda);

```

### 💾 Persistência de Dados (Arquivos)

```c

void checarDadosExistentesNoArquivo(FILE *arquivo, Agenda agenda[], int *tamAgenda);
void colocarNovoContatoNoArquivo(FILE *arquivo, Agenda agenda[], int tamAgenda);

```
