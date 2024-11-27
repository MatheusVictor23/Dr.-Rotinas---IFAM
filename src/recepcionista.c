#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/recepcionista.h"
#include "../include/medicos.h"

typedef struct {
    int id;
    char nome[50];
    char telefone[15];
    char cargo[20];
} Recepcionista;

void menuRecepcionistas() {
    int op;
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
            case 0:
                return;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (op != 0);
}

void cadastrarRecepcionista() {
    FILE *arquivo = fopen("../data/recepcionistas.bin", "ab");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        return;
    }

    Recepcionista novaRecepcionista;
    printf("\n--- Cadastrar Recepcionista ---\n");
    printf("ID: ");
    scanf("%d", &novaRecepcionista.id);
    printf("Nome: ");
    scanf(" %[^\n]s", novaRecepcionista.nome);
    printf("Telefone: ");
    scanf(" %[^\n]s", novaRecepcionista.telefone);

    fwrite(&novaRecepcionista, sizeof(Recepcionista), 1, arquivo); // Salvando em binário
    fclose(arquivo);
    printf("Recepcionista cadastrada com sucesso!\n");
}

void listarRecepcionistas() {
    FILE *arquivo = fopen("recepcionistas.bin", "rb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        return;
    }

    Recepcionista recepcionista;
    printf("\n--- Lista de Recepcionistas ---\n");
    while (fread(&recepcionista, sizeof(Recepcionista), 1, arquivo) == 1) {
        printf("ID: %d\n", recepcionista.id);
        printf("Nome: %s\n", recepcionista.nome);
        printf("Telefone: %s\n", recepcionista.telefone);
        printf("---------------------------\n");
    }

    fclose(arquivo);
}