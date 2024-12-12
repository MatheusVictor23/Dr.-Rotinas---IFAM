#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTAS 100

typedef struct {
    int id;
    char nome[50];
    float valor;
    char data_vencimento[11];
    char categoria[50];
} Conta;

Conta contas[MAX_CONTAS];
int num_contas = 0;

void cadastrar_conta() {

    contas[num_contas].id = num_contas + 1;

    printf("Digite o nome da conta: ");
    scanf("%s", contas[num_contas].nome);

    printf("Digite o valor da conta: ");
    scanf("%f", &contas[num_contas].valor);

    printf("Digite a data de vencimento (dd/mm/aaaa): ");
    scanf("%s", contas[num_contas].data_vencimento);

    printf("Digite a categoria da conta: ");
    scanf("%s", contas[num_contas].categoria);

    num_contas++;
    printf("Conta cadastrada com sucesso!\n");
}

void listar_contas() {
    if (num_contas == 0) {
        printf("Nenhuma conta cadastrada.\n");
        return;
    }

    printf("ID    \tNome   \t\tValor   \t\tVencimento   \tCategoria\n");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < num_contas; i++) {
        printf("%d\t%s\t\tR$%.2f\t\t%s\t\t%s\n", 
               contas[i].id, contas[i].nome, contas[i].valor, 
               contas[i].data_vencimento, contas[i].categoria);
    }
}

void menuFinanceiro() {
    int opcao;

    do {
        printf("\nMenu:\n");
        printf("1. Cadastrar conta\n");
        printf("2. Listar contas\n");
        printf("3. Sair\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrar_conta();
                break;
            case 2:
                listar_contas();
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 3);

}

int main() {
    menuFinanceiro();
    return 0;
}
