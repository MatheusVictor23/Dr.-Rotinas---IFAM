#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/medicos.h"
#include "../include/login.h"

typedef struct Medico {
    int id;
    char nome[50];
    char telefone[15];
    char cpf[15];
    char especialidade[30];
    char cargo[20];
    struct Medico *proximo; // Ponteiro para o proximo no da lista
} Medico;

Medico *listaMedicos = NULL; // Cabeça da lista

int gerarID(){
    return rand() % 1000 + 1; // id entre 1 e 1000
}

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
                system("cls || clear");
                cadastrarMedico();
                break;
            case 2:
                system("cls || clear");
                listarMedicosDiretamenteArquivo();
                break;
            case 3:
                system("cls || clear");
                excluirMedico();
                break;
            case 0:
                system("cls || clear");
                return;

            default:
                getchar();
                printf("Opçao invalida. Tente novamente.\n");
                finalizar();
                break;
        }
    } while (op != 0);
}

void cadastrarMedico() {
    Medico *novo = (Medico *)malloc(sizeof(Medico));
    if (!novo) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    srand(time(NULL)); // inicializa a semente com o tempo atual
    int id = gerarID();

    printf("\n--- Cadastrar Medico ---\n");
    novo->id = id;
    printf("ID: %d\n", novo->id);
    // NOME
    getchar();
    printf("Nome: ");
    fgets(novo->nome, sizeof(novo->nome), stdin);
    strtok(novo->nome, "\n"); // Remove o '\n' que fgets adiciona
    // CPF
    printf("CPF: ");
    fgets(novo->cpf, sizeof(novo->cpf), stdin);
    strtok(novo->cpf, "\n");
    // TELEFONE
    printf("Telefone: ");
    fgets(novo->telefone, sizeof(novo->telefone), stdin);
    strtok(novo->telefone, "\n");
    // ESPECIALIDADE
    printf("Especialidade: ");
    fgets(novo->especialidade, sizeof(novo->especialidade), stdin);
    strtok(novo->especialidade, "\n");

    // Escreve o cargo
    strcpy(novo->cargo, "medico");

    novo->proximo = listaMedicos;
    listaMedicos = novo;

    // Salvar no arquivo binário
    FILE *arquivo = fopen("../data/medicos.bin", "ab");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para salvar.");
        free(novo);
        return;
    }

    fwrite(novo, sizeof(Medico) - sizeof(Medico *), 1, arquivo);
    fclose(arquivo);

    printf("Médico cadastrado com sucesso!\n");
    finalizar();
}


// Lista os médicos a partir da memória

// void listarMedicos() {
//     if (!listaMedicos) {
//         getchar();
//         printf("Nenhum médico encontrado.\n");
//         finalizar();
//         return;
//     }

//     Medico *atual = listaMedicos;
//     printf("\n--- Lista de Medicos ---\n");
//     while (atual) {
//         printf("ID: %d\n", atual->id);
//         printf("Nome: %s\n", atual->nome);
//         printf("CPF: %s\n", atual->cpf);
//         printf("Telefone: %s\n", atual->telefone);
//         printf("Especialidade: %s\n", atual->especialidade);
//         printf("---------------------------\n");
//         atual = atual->proximo;
//     }
//     getchar();
//     finalizar();
// }


// Lista os médicos a partir do arquivo binário
void listarMedicosDiretamenteArquivo() {
    FILE *arquivo = fopen("../data/medicos.bin", "rb");

    if (!arquivo) {
        printf("\nNenhum médico encontrado.\n");
        finalizar();
        return;
    }

    Medico temp;
    printf("\n--- Lista de Medicos ---\n");
    while (fread(&temp, sizeof(Medico) - sizeof(Medico *), 1, arquivo) == 1) {
        printf("ID: %d\n", temp.id);
        printf("Nome: %s\n", temp.nome);
        printf("CPF: %s\n", temp.cpf);
        printf("Telefone: %s\n", temp.telefone);
        printf("Especialidade: %s\n", temp.especialidade);
        printf("---------------------------\n");
    }
    fclose(arquivo);

    getchar();
    finalizar();
}



void excluirMedico() {
    int id;

    printf("Digite o ID do medico a ser excluido: ");
    scanf("%d", &id);

    Medico *atual = listaMedicos;
    Medico *anterior = NULL;

    // Procurar o medico com o ID correspondente
    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Verificar se o medico foi encontrado
    if (!atual) {
        getchar();
        printf("Medico com ID %d nao encontrado.\n", id);
        finalizar();
        return;
    }

    // Remover o medico da lista
    if (anterior == NULL) {
        // O medico a ser removido esta no inicio da lista
        listaMedicos = atual->proximo;
    } else {
        // O medico a ser removido esta no meio ou no final
        anterior->proximo = atual->proximo;
    }

    free(atual);

    // Atualiza o arquivo binário
    FILE *arquivo = fopen("../data/medicos.bin", "wb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para salvar.");
        return;
    }

    Medico *temp = listaMedicos;
    while (temp != NULL) {
        fwrite(temp, sizeof(Medico) - sizeof(Medico *), 1, arquivo);
        temp = temp->proximo;
    }
    fclose(arquivo);

    // recarrega a lista do arquivo
    carregarMedicos();

    getchar();
    printf("Médico excluído com sucesso.\n");
    finalizar();
}

void limparLista() {
    Medico *atual = listaMedicos;
    while (atual) {
        Medico *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    listaMedicos = NULL;
}

// Funçao para carregar a lista de medicos do arquivo binario
void carregarMedicos() {
    limparLista(); // limpa a lista antes de carregar de novo

    FILE *arquivo = fopen("../data/medicos.bin", "rb");
    if (!arquivo) {
        return; // Sem médicos salvos
    }

    Medico temp;
    while (fread(&temp, sizeof(Medico) - sizeof(Medico *), 1, arquivo) == 1) {
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

    fclose(arquivo);
    
}
