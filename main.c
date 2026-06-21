#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct
{
    int id;
    char nome[100];
    char telefone[100];
} Agenda;

int TAM_AGENDA = 0;

void limpar();
void mostrarSucesso(char mensagemSucesso[], char mensagemVoltar[]);

void menu(int *opcao);
void menuAcao(int *opcao, char modo[]);
void menuEdicao(int *opcao);

void novoContato(Agenda agenda[], int *tamAgenda);

void listarContatos(Agenda agenda[], int tamAgenda);

int encontrarPorId(Agenda agenda[], int *tamAgenda, char modo[]);
int encontrarPorNome(Agenda agenda[], int *tamAgenda, char modo[]);
void buscarPorNome(Agenda agenda[], int tamAgenda);

void modoExcluir(FILE *arquivo, Agenda agenda[], int *tamAgenda);
void excluir(FILE *arquivo, Agenda agenda[], int *tamAgenda, int posicaoEncontrado);

void checarDadosExistentesNoArquivo(FILE *arquivo, Agenda agenda[], int *tamAgenda);
void colocarNovoContatoNoArquivo(FILE *arquivo, Agenda agenda[], int tamAgenda);

void editarContato(FILE *arquivo, Agenda agenda[], int tamAgenda);
void editar(FILE *arquivo, Agenda agenda[], int tamAgenda, int posicao);

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
            tamAgenda > 0 ? editarContato(arquivo, agenda, tamAgenda) : printf("\n\tA lista de contatos esta vazia!");
            break;
        case 5:
            tamAgenda > 0 ? modoExcluir(arquivo, agenda, &tamAgenda) : printf("\n\tA lista de contatos esta vazia!");
            break;
        case 6:
            limpar();
            printf("\tTchau\n");
            break;
        default:
            limpar();
            printf("\tOpcao errada, tente novamente!");
        }
    } while (opcao != 6);
}

void limpar(){
    #ifdef __WIN32
        system("cls");
    #else
        system("clear");    
    #endif
}

void mostrarSucesso(char mensagemSucesso[], char mensagemVoltar[]){
    printf("\n\t%s", mensagemSucesso);
    for(int i=4; i>=1; i--){
        printf("\n\t%s: %i", mensagemVoltar, i);
        sleep(1);
        limpar();
    }
    limpar();
}

void editar(FILE *arquivo, Agenda agenda[], int tamAgenda, int posicao){
    limpar();
    int opcao;

    do {
        menuEdicao(&opcao);
        switch (opcao)
        {
        case 1:
            getchar();
            char novoNome[100];
            printf("\tInforme o novo nome de %s: ", agenda[posicao].nome);
            fgets(novoNome, 100, stdin);
            novoNome[strlen(novoNome) - 1] = '\0';
            strcpy(agenda[posicao].nome, novoNome);
            colocarNovoContatoNoArquivo(arquivo, agenda, tamAgenda);
            limpar();
            printf("\n\n\tNome editado com sucesso!\n");
            break;
        case 2:
            getchar();
            char novoTelefone[100];
            printf("\tInforme o novo telefone de %s: ", agenda[posicao].nome);
            fgets(novoTelefone, 100, stdin);
            novoTelefone[strlen(novoTelefone) - 1] = '\0';
            strcpy(agenda[posicao].telefone, novoTelefone);
            colocarNovoContatoNoArquivo(arquivo, agenda, tamAgenda);
            limpar();
            printf("\n\n\tTelefone editado com sucesso!\n");
            break;
        case 3:
            mostrarSucesso("Saindo...", "Voltando para as opcoes de edicao em");
            break;
        default:
            printf("\tOpcao errada, tente novamente!");
            break;
        }
    } while(opcao != 3);

    listarContatos(agenda, tamAgenda);
}

void editarContato(FILE *arquivo, Agenda agenda[], int tamAgenda){
    limpar();
    int opcao;
    do {
        menuAcao(&opcao, "EDICAO");
        switch (opcao)
        {
        case 1:
            editar(arquivo, agenda, tamAgenda, encontrarPorId(agenda, &tamAgenda, "editar"));
            break;
        case 2:
            editar(arquivo, agenda, tamAgenda, encontrarPorNome(agenda, &tamAgenda, "editar"));
            break;
        case 3:
            mostrarSucesso("Saindo...", "Voltando para o menu principal em");
            break;
        default:
            break;
        }
    }
    while (opcao != 3);
    
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
        if(TAM_AGENDA < novoContato.id)
            TAM_AGENDA = novoContato.id;
    }

    fclose(arquivo);
}

void modoExcluir(FILE *arquivo, Agenda agenda[], int *tamAgenda)
{
    limpar();
    int opcao;
    do
    {
        menuAcao(&opcao, "EXCLUSAO");
        switch (opcao)
        {
        case 1:
            excluir(arquivo, agenda, tamAgenda, encontrarPorId(agenda, tamAgenda, "excluir"));
            break;
        case 2:
            excluir(arquivo, agenda, tamAgenda, encontrarPorNome(agenda, tamAgenda, "excluir"));
            break;
        case 3:
            mostrarSucesso("Saindo...", "Voltando para o menu principal em");
            break;
        default:
            printf("\tOpcao errada, tente novamente!");
        }
    } while (opcao != 3);
}

void buscarPorNome(Agenda agenda[], int tamAgenda)
{
    limpar();
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
    limpar();
    if (tamAgenda == 0)
    {
        printf("\n\tSua lista de contatos esta vazia\n");
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
    limpar();
    ++TAM_AGENDA;
    Agenda contato;
    getchar();
    printf("\n\tInforme o nome do novo contato: ");
    fgets(contato.nome, 100, stdin);
    contato.nome[strlen(contato.nome) - 1] = '\0';
    printf("\n\tInforme o telefone de %s: ", contato.nome);
    fgets(contato.telefone, 100, stdin);
    contato.telefone[strlen(contato.telefone) - 1] = '\0';
    contato.id = TAM_AGENDA;
    agenda[*(tamAgenda)] = contato;
    ++*(tamAgenda);
    limpar();
    mostrarSucesso("Contato criado com sucesso!", "Voltando para o menu principal em");
    limpar();
}

int encontrarPorId(Agenda agenda[], int *tamAgenda, char modo[])
{
    getchar();
    listarContatos(agenda, *(tamAgenda));
    int posicaoEncontrado = -1;
    int id;
    while (posicaoEncontrado == -1)
    {
        printf("\n\tInforme o identificador que deseja %s: ", modo);
        scanf("%i", &id);
        for (int i = 0; i < *(tamAgenda); i++)
        {
            if (agenda[i].id == id)
                return i;
        }

        if (posicaoEncontrado == -1)
            printf("\n\tIdentificador nao encontrado, tente novamente!\n");
    }
}

int encontrarPorNome(Agenda agenda[], int *tamAgenda, char modo[])
{
    getchar();
    listarContatos(agenda, *(tamAgenda));
    char nome[100];
    int posicaoEncontrado = -1;
    while (posicaoEncontrado == -1)
    {

        printf("\n\tInforme o nome corretamente que deseja %s: ", modo);
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
                return i;
        }

        if (posicaoEncontrado == -1)
            printf("\n\tNome nao encontrado, tente novamente!\n");
    }
}

void excluir(FILE *arquivo, Agenda agenda[], int *tamAgenda, int posicaoEncontrado)
{
    for (int i = posicaoEncontrado + 1; i < *(tamAgenda); i++)
        agenda[i - 1] = agenda[i];
    --*(tamAgenda);

    limpar();
    colocarNovoContatoNoArquivo(arquivo, agenda, *(tamAgenda));
    mostrarSucesso("Contato excluido com sucesso!", "Voltando para o menu de exclusao em");
}

void menu(int *opcao)
{
    printf("\n\n\t=============PROJETO AGENDA DE CONTATOS==============\n\n");
    printf("\t1 - Incluir contato\n");
    printf("\t2 - Listar contatos\n");
    printf("\t3 - Consultar contato pelo nome\n");
    printf("\t4 - Editar\n");
    printf("\t5 - Excluir\n");
    printf("\t6 - Sair\n");
    printf("\tInforme a sua opcao: ");
    scanf("\t%i", opcao);
}

void menuAcao(int *opcao, char modo[])
{
    printf("\n\n\t====================OPCOES DE %s==================\n", modo);
    printf("\t1 - Identificador\n");
    printf("\t2 - Nome\n");
    printf("\t3 - Cancelar\n");
    printf("\tInforme sua opcao: ");
    scanf("%i", opcao);
}

void menuEdicao(int *opcao){
    printf("\n\n\t====================CAMPOS PARA EDITAR==================\n");
    printf("\t1 - Nome\n");
    printf("\t2 - Telefone\n");
    printf("\t3 - Sair\n");
    printf("\tInforme sua opcao: ");
    scanf("%i", opcao);
}