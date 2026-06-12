#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct
{
    int id;
    char nome[100];
    char telefone[100];
} Agenda;

int TAM_AGENDA = 0;

void menu(int *opcao);
void menuExclusao(int *opcao);

void novoContato(Agenda agenda[], int *tamAgenda);

void listarContatos(Agenda agenda[], int tamAgenda);

void encontrarPorId(FILE *arquivo, Agenda agenda[], int *tamAgenda);
void encontrarPorNome(FILE *arquivo, Agenda agenda[], int *tamAgenda);
void buscarPorNome(Agenda agenda[], int tamAgenda);

void modoExcluir(FILE *arquivo, Agenda agenda[], int *tamAgenda);
void excluir(FILE *arquivo, Agenda agenda[], int *tamAgenda, int posicaoEncontrado);

void checarDadosExistentesNoArquivo(FILE *arquivo, Agenda agenda[], int *tamAgenda);
void colocarNovoContatoNoArquivo(FILE *arquivo, Agenda agenda[], int tamAgenda);

int main(void)
{
    FILE *arquivo;
    Agenda agenda[10];
    int opcao, tamAgenda = 0;

    checarDadosExistentesNoArquivo(arquivo, agenda, &tamAgenda);
    do
    {
        menu(&opcao);

        switch (opcao)
        {
        case 1:
            novoContato(agenda, &tamAgenda);
            colocarNovoContatoNoArquivo(arquivo, agenda, tamAgenda);
            break;
        case 2:
            listarContatos(agenda, tamAgenda);
            break;
        case 3:
            buscarPorNome(agenda, tamAgenda);
            break;
        case 4:
            modoExcluir(arquivo, agenda, &tamAgenda);
            break;
        case 5:
            printf("\tTchau");
            break;
        default:
            printf("\tOpcao errada, tente novamente!");
        }
    } while (opcao != 5);
}

void checarDadosExistentesNoArquivo(FILE *arquivo, Agenda agenda[], int *tamAgenda)
{
    arquivo = fopen("contatos.txt", "r");
    int id;
    char nome[100], telefone[100];
    while (fscanf(arquivo, "%i%s%s", &id, nome, telefone) == 3)
    {
        Agenda novoContato;
        novoContato.id = id;
        strcpy(novoContato.nome, nome);
        strcpy(novoContato.telefone, telefone);
        agenda[*(tamAgenda)] = novoContato;
        ++*(tamAgenda);
        ++TAM_AGENDA;
    }

    fclose(arquivo);
}

void modoExcluir(FILE *arquivo, Agenda agenda[], int *tamAgenda)
{
    int opcao;
    do
    {
        menuExclusao(&opcao);
        switch (opcao)
        {
        case 1:
            encontrarPorId(arquivo, agenda, tamAgenda);
            break;
        case 2:
            encontrarPorNome(arquivo, agenda, tamAgenda);
            break;
        case 3:
            printf("\tSaindo...");
            break;
        default:
            printf("\tOpcao errada, tente novamente!");
        }
    } while (opcao != 3);
}

void buscarPorNome(Agenda agenda[], int tamAgenda)
{
    getchar();
    Agenda agendaFiltrada[tamAgenda];
    int tamAgendaFiltrada = 0;
    char nome[100];
    printf("\tInforme o nome do contato que deseja: ");
    fgets(nome, 100, stdin);

    for (int i = 0; i < tamAgenda; i++)
    {
        bool nomeExiste = true;
        for (int j = 0; j < strlen(nome) - 1; j++)
        {
            if (tolower(agenda[i].nome[j]) != tolower(nome[j]) && nomeExiste)
                nomeExiste = false;
        }
        if (nomeExiste)
        {
            agendaFiltrada[tamAgendaFiltrada] = agenda[i];
            ++tamAgendaFiltrada;
        }
    }
    listarContatos(agendaFiltrada, tamAgendaFiltrada);
}

void listarContatos(Agenda agenda[], int tamAgenda)
{
    if (tamAgenda == 0)
    {
        printf("\n\tSua lista de contatos esta vazia");
        return;
    }
    char guardarNome[100], guardarTelefone[100];
    int guardarId;
    for (int i = 0; i < tamAgenda; i++)
    {
        for (int j = i + 1; j < tamAgenda; j++)
        {
            if (strcmp(agenda[i].nome, agenda[j].nome) > 0)
            {
                strcpy(guardarNome, agenda[j].nome);
                strcpy(agenda[j].nome, agenda[i].nome);
                strcpy(agenda[i].nome, guardarNome);
                strcpy(guardarTelefone, agenda[j].telefone);
                strcpy(agenda[j].telefone, agenda[i].telefone);
                strcpy(agenda[i].telefone, guardarTelefone);
                guardarId = agenda[j].id;
                agenda[j].id = agenda[i].id;
                agenda[i].id = guardarId;
            }
        }
    }

    printf("\n\n\t=========================LISTA DE CONTATOS========================\n\n");
    printf("\t%5s |%10s |%10s\n", "ID", "NOME", "TELEFONE");
    for (int i = 0; i < tamAgenda; i++)
        printf("\t%5d |%10s |%16s\n", agenda[i].id, agenda[i].nome, agenda[i].telefone);
}

void colocarNovoContatoNoArquivo(FILE *arquivo, Agenda agenda[], int tamAgenda)
{
    arquivo = fopen("contatos.txt", "w");
    for (int i = 0; i < tamAgenda; i++)
        fprintf(arquivo, "%d %s %s\n", agenda[i].id, agenda[i].nome, agenda[i].telefone);
    fclose(arquivo);
}

void novoContato(Agenda agenda[], int *tamAgenda)
{
    ++TAM_AGENDA;
    Agenda contato;
    getchar();
    printf("\tInforme o nome do novo contato: ");
    fgets(contato.nome, 100, stdin);
    contato.nome[strlen(contato.nome) - 1] = '\0';
    printf("\tInforme o telefone de %s: ", contato.nome);
    fgets(contato.telefone, 100, stdin);
    contato.telefone[strlen(contato.telefone) - 1] = '\0';
    contato.id = TAM_AGENDA;
    agenda[*(tamAgenda)] = contato;
    ++*(tamAgenda);
}

void encontrarPorId(FILE *arquivo, Agenda agenda[], int *tamAgenda)
{
    getchar();
    listarContatos(agenda, *(tamAgenda));
    int posicaoEncontrado = -1;
    int id;
    while (posicaoEncontrado == -1)
    {
        printf("\tInforme o identificador que deseja excluir: ");
        scanf("%i", &id);
        for (int i = 0; i < *(tamAgenda); i++)
        {
            if (agenda[i].id == id)
                posicaoEncontrado = i;
        }

        if (posicaoEncontrado == -1)
            printf("\tIdentificador nao encontrado, tente novamente!\n");
    }
    excluir(arquivo, agenda, tamAgenda, posicaoEncontrado);
}

void encontrarPorNome(FILE *arquivo, Agenda agenda[], int *tamAgenda)
{
    getchar();
    listarContatos(agenda, *(tamAgenda));
    char nome[100];
    int posicaoEncontrado = -1;
    while (posicaoEncontrado == -1)
    {

        printf("\tInforme o nome corretamente que deseja excluir: ");
        fgets(nome, 100, stdin);

        for (int i = 0; i < *(tamAgenda); i++)
        {
            bool nomeCorreto = true;
            for (int j = 0; j < strlen(agenda[i].nome) - 1; j++)
            {
                if (tolower(agenda[i].nome[j]) != tolower(nome[j]))
                {
                    nomeCorreto = false;
                    break;
                }
            }
            if (nomeCorreto)
                posicaoEncontrado = i;
        }

        if (posicaoEncontrado == -1)
            printf("\tNome nao encontrado, tente novamente!\n");
    }

    excluir(arquivo, agenda, tamAgenda, posicaoEncontrado);
}

void excluir(FILE *arquivo, Agenda agenda[], int *tamAgenda, int posicaoEncontrado)
{
    for (int i = posicaoEncontrado + 1; i < *(tamAgenda); i++)
        agenda[i - 1] = agenda[i];
    --*(tamAgenda);

    listarContatos(agenda, *(tamAgenda));
    colocarNovoContatoNoArquivo(arquivo, agenda, *(tamAgenda));
    printf("\n\tContato excluido com sucesso!");
}

void menu(int *opcao)
{
    printf("\n\n\t=============PROJETO AGENDA DE CONTATOS==============\n\n");
    printf("\t1 - Incluir contato\n");
    printf("\t2 - Listar contatos\n");
    printf("\t3 - Consultar contato pelo nome\n");
    printf("\t4 - Excluir\n");
    printf("\t5 - Sair\n");
    printf("\tInforme a sua opcao: ");
    scanf("\t%i", opcao);
}

void menuExclusao(int *opcao)
{
    printf("\n\n\t====================OPCOES DE EXCLUSAO==================\n");
    printf("\t1 - Identificador\n");
    printf("\t2 - Nome\n");
    printf("\t3 - Cancelar\n");
    printf("\tInforme sua opcao: ");
    scanf("%i", opcao);
}