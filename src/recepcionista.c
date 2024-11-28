#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/recepcionista.h"
typedef struct Recepcionista {
    int id;
    char nome[50];
    char telefone[15];
    char cargo[20];
    struct Recepcionista *proximo; // Ponteiro para o proximo no da lista
} Recepcionista;

Recepcionista *listaRecepcionistas = NULL; // Cabeça da lista
int ultimoIdRecepcionista = 0;

void menuRecepcionistas() {
    int op;
    carregarRecepcionistas();
    do {
        system("cls || clear");
        puts("\t+------------------------------+");
        puts("\t|                              |");
        puts("\t| GERENCIAMENTO RECEPCIONISTA  |");
        puts("\t|                              |");
        puts("\t+------------------------------+");
        puts("\t|       Deseja Realizar:       |");
        puts("\t|                              |");
        puts("\t| 1. Cadastrar Recepcionista   |");
        puts("\t| 2. Listar Recepcionista      |");
        puts("\t| 3. Excluir Recepcionista     |");
        puts("\t| 0. Voltar ao Menu Principal  |");
        puts("\t|                              |");
        puts("\t+------------------------------+");
        printf("Digite a opcao: ");
        scanf("%i", &op);

        switch (op) {
            case 1:
                cadastrarRecepcionista();
                break;
            case 2:
                listarRecepcionistas();
                break;
            case 3: 
                excluirRecepcionista();
            case 0:
                salvarRecepcionistas();
                return;
            default:
                printf("Opção invalida. Tente novamente.\n");
        }
    } while (op != 0);
}

void cadastrarRecepcionista() {
    // Cria um novo no para o recepcionista
    Recepcionista *novo = (Recepcionista *)malloc(sizeof(Recepcionista));
    if (!novo) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    printf("\n--- Cadastrar Recepcionista ---\n");
    novo->id = ++ultimoIdRecepcionista; // Incrementa o último ID e atribui ao novo médico
    printf("ID: %d\n", novo->id);
    printf("Nome: ");
    scanf(" %[^\n]s", novo->nome);
    printf("Telefone: ");
    scanf(" %[^\n]s", novo->telefone);
    strcpy(novo->cargo, "recepcionista");

    // Insere o novo recepcionista no início da lista
    novo->proximo = listaRecepcionistas;
    listaRecepcionistas = novo;

    printf("Recepcionista cadastrada com sucesso!\n");
}

void listarRecepcionistas() {
    if (listaRecepcionistas == NULL) {
        printf("\nNenhum recepcionista cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Recepcionistas ---\n");
    Recepcionista *atual = listaRecepcionistas;
    int op = 0;

    while (atual != NULL) {
        printf("ID: %d\n", atual->id);
        printf("Nome: %s\n", atual->nome);
        printf("Telefone: %s\n", atual->telefone);
        printf("Cargo: %s\n", atual->cargo);
        printf("---------------------------\n");
        atual = atual->proximo; // Avança para o proximo no
    }

    do{
        printf("\n\n 1- Retornar\n");
        scanf("%d",&op);
    }while(op != 1);
}


void excluirRecepcionista() {
    int id;
    printf("Digite o ID do recepcionista a ser excluido: ");
    scanf("%d",&id);

    if (!listaRecepcionistas) {
        printf("Nenhum recepcionista cadastrado.\n");
    }

    Recepcionista *atual = listaRecepcionistas;
    Recepcionista *anterior = NULL;

    // Procurar o médico com o ID correspondente
    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Verificar se o médico foi encontrado
    if (!atual) {
        printf("Recepcionista com ID %d não encontrado.\n", id);
    }

    // Remover o médico da lista
    if (anterior == NULL) {
        // O médico a ser removido esta no início da lista
        listaRecepcionistas = atual->proximo;
    } else {
        // O médico a ser removido esta no meio ou no final
        anterior->proximo = atual->proximo;
    }

    free(atual);
    printf("Recepcionista com ID %d excluído com sucesso.\n", id);

    // Salvar a lista atualizada no arquivo binario
    FILE *arquivo = fopen("../data/recepcionista.bin", "wb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para salvar.");
        return;
    }

    Recepcionista *temp = listaRecepcionistas;
    while (temp != NULL) {
        fwrite(temp, sizeof(Recepcionista) - sizeof(Recepcionista *), 1, arquivo);
        temp = temp->proximo;
    }

    fwrite(&ultimoIdRecepcionista, sizeof(int), 1, arquivo); // Salva o último ID
    fclose(arquivo);
    do{
        printf("\n\n 1- Retornar\n");
        scanf("%d",&op);
    }while(op != 1);
    return 0;
}




// Função para salvar recepcionistas no arquivo binario
void salvarRecepcionistas() {
    FILE *arquivo = fopen("../data/recepcionistas.bin", "wb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo para escrita");
        return;
    }

    Recepcionista *atual = listaRecepcionistas;
    while (atual != NULL) {
        fwrite(atual, sizeof(Recepcionista), 1, arquivo);
        atual = atual->proximo;
    }

    fwrite(&ultimoIdRecepcionista, sizeof(int), 1, arquivo);
    fclose(arquivo);
    printf("\nRecepcionistas salvos com sucesso!\n");
}


// Função para carregar recepcionistas do arquivo binario
void carregarRecepcionistas() {
    FILE *arquivo = fopen("../data/recepcionistas.bin", "rb");
    if (!arquivo) {
        printf("Nenhum arquivo de recepcionistas encontrado.\n");
        return;
    }

    Recepcionista temp;
    while (fread(&temp, sizeof(Recepcionista), 1, arquivo) == 1) {
        Recepcionista *novo = (Recepcionista *)malloc(sizeof(Recepcionista));
        if (!novo) {
            printf("Erro ao alocar memoria.\n");
            fclose(arquivo);
            return;
        }

        *novo = temp;
        novo->proximo = listaRecepcionistas;
        listaRecepcionistas = novo;
    }

    fread(&ultimoIdRecepcionista, sizeof(int), 1, arquivo);
    fclose(arquivo);
    printf("Recepcionistas carregados com sucesso!\n");
}
