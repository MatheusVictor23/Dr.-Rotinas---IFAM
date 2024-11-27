#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/medicos.h"

typedef struct {
    int id;
    char nome[50];
    char especialidade[50];
    char CRM[20];
    char cargo[20];
} Medico;

void menuMedicos() {
    int op;
    do {

    system("cls || clear");

    puts("\t+------------------------------+");
    puts("\t|                              |");
    puts("\t|     GERENCIAMENTO MEDICO     |");
    puts("\t|                              |");
    puts("\t+------------------------------+");
    puts("\t|       Deseja Realizar:       |");
    puts("\t|                              |");
    puts("\t| 1. Cadastrar Médico          |");
    puts("\t| 2. Listar Médicos            |");
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
            case 0:
                return;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (op != 0);
}

void cadastrarMedico() {
    FILE *arquivo = fopen("../data/medicos.bin", "ab");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        return;
    }

    Medico novoMedico;
    printf("\n--- Cadastrar Médico ---\n");
    printf("ID: ");
    scanf("%d", &novoMedico.id);
    printf("Nome: ");
    scanf(" %[^\n]s", novoMedico.nome);
    printf("Especialidade: ");
    scanf(" %[^\n]s", novoMedico.especialidade);
    printf("CRM: ");
    scanf(" %[^\n]s", novoMedico.CRM);

    fwrite(&novoMedico, sizeof(Medico), 1, arquivo); // Salvando em binário
    fclose(arquivo);
    printf("Médico cadastrado com sucesso!\n");
}

void listarMedicos() {
    FILE *arquivo = fopen("medicos.bin", "rb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        return;
    }

    Medico medico;
    printf("\n--- Lista de Médicos ---\n");
    while (fread(&medico, sizeof(Medico), 1, arquivo) == 1) {
        printf("ID: %d\n", medico.id);
        printf("Nome: %s\n", medico.nome);
        printf("Especialidade: %s\n", medico.especialidade);
        printf("CRM: %s\n", medico.CRM);
        printf("---------------------------\n");
    }

    fclose(arquivo);
}