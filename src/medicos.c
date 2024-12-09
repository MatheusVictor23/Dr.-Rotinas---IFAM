#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/medicos.h"

typedef struct Medico {
    int id;
    char nome[50];
    char telefone[15];
    char especialidade[30];
    char cargo[20];
    struct Medico *proximo; // Ponteiro para o proximo no da lista
} Medico;

Medico *listaMedicos = NULL; // Cabeça da lista
int ultimoIdMedico = 0;


void menuMedicos() {
    int op;
    carregarMedicos();

    do {
        system("cls || clear");

        puts("\t+------------------------------+");
        puts("\t|                              |");
        puts("\t|     GERENCIAMENTO MEDICO     |");
        puts("\t|                              |");
        puts("\t+------------------------------+");
        puts("\t|       Deseja Realizar:       |");
        puts("\t|                              |");
        puts("\t| 1. Cadastrar Medico          |");
        puts("\t| 2. Listar Medicos            |");
        puts("\t| 3. Excluir Medico            |");
        puts("\t| 0. Voltar ao Menu Principal  |");
        puts("\t|                              |");
        puts("\t+------------------------------+");
        printf("Digite a opcao: ");
        scanf("%i", &op);

        switch (op) {
            case 1:
                cadastrarMedico();
                break;
            case 2:
                listarMedicos();
                break;
            case 3: 
                excluirMedico();
            case 0:
                salvarMedicos();
                return;
            default:
                printf("Opçao invalida. Tente novamente.\n");
        }
    } while (op != 0);
}

void cadastrarMedico() {
    Medico *novo = (Medico *)malloc(sizeof(Medico));
    if (!novo) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    printf("\n--- Cadastrar Medico ---\n");
    novo->id = ++ultimoIdMedico;
    printf("ID: %d\n", novo->id);
    printf("Nome: ");
    scanf(" %[^\n]s", novo->nome);
    printf("Telefone: ");
    scanf(" %[^\n]s", novo->telefone);
    printf("Especialidade: ");
    scanf(" %[^\n]s", novo->especialidade);
    strcpy(novo->cargo, "medico");

    novo->proximo = listaMedicos;
    listaMedicos = novo;

    printf("Medico cadastrado com sucesso!\n");
}




void listarMedicos() {
    if (listaMedicos == NULL) {
        printf("\nNenhum medico cadastrado.\n");
        
    }

    printf("\n--- Lista de Medicos ---\n");
    Medico *atual = listaMedicos;
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
    return;
}


void excluirMedico() {
    int id;
    int op = 0;

    if (!listaMedicos) {
        printf("Nenhum medico cadastrado.\n");
        do{
        printf("\n\n 1- Retornar: ");
        scanf("%d",&op);
    }while(op != 1);
    return ;
        
    }

    printf("Digite o ID do medico a ser excluido: ");
    scanf("%d",&id);

    Medico *atual = listaMedicos;
    Medico *anterior = NULL;

    // Procurar o médico com o ID correspondente
    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Verificar se o médico foi encontrado
    if (!atual) {
        printf("Medico com ID %d não encontrado.\n", id);
        do{
        printf("\n\n 1- Retornar: ");
        scanf("%d",&op);
    }while(op != 1);
    return ;
    }

    // Remover o médico da lista
    if (anterior == NULL) {
        // O médico a ser removido esta no início da lista
        listaMedicos = atual->proximo;
    } else {
        // O médico a ser removido esta no meio ou no final
        anterior->proximo = atual->proximo;
    }

    free(atual);
    printf("Medico com ID %d excluído com sucesso.\n", id);

    // Salvar a lista atualizada no arquivo binario
    FILE *arquivo = fopen("../data/medico.bin", "wb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para salvar.");
        return;
    }

    Medico *temp = listaMedicos;
    while (temp != NULL) {
        fwrite(temp, sizeof(Medico) - sizeof(Medico *), 1, arquivo);
        temp = temp->proximo;
    }

    fwrite(&ultimoIdMedico, sizeof(int), 1, arquivo); // Salva o último ID
    fclose(arquivo);
    do{
        printf("\n\n 1- Retornar: ");
        scanf("%d",&op);
    }while(op != 1);
    return ;
}


void salvarMedicos() {
    FILE *arquivo = fopen("../data/medico.bin", "wb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo para escrita");
        return;
    }

    Medico *atual = listaMedicos;
    while (atual != NULL) {
        fwrite(atual, sizeof(Medico), 1, arquivo);
        atual = atual->proximo;
    }

    fwrite(&ultimoIdMedico, sizeof(int), 1, arquivo);
    fclose(arquivo);
    printf("\nMedicos salvos com sucesso!\n");
}




void carregarMedicos() {
    FILE *arquivo = fopen("../data/medico.bin", "rb");
    if (!arquivo) {
        printf("Nenhum arquivo de medicos encontrado.\n");
        return;
    }

    Medico temp;
    while (fread(&temp, sizeof(Medico), 1, arquivo) == 1) {
        Medico *novo = (Medico *)malloc(sizeof(Medico));
        if (!novo) {
            printf("Erro ao alocar memoria.\n");
            fclose(arquivo);
            return;
        }

        *novo = temp;
        novo->proximo = listaMedicos;
        listaMedicos = novo;
    }

    fread(&ultimoIdMedico, sizeof(int), 1, arquivo);
    fclose(arquivo);
    printf("Medicos carregados com sucesso!\n");
}