#ifndef FINANCEIRO_H
#define FINANCEIRO_H

#define MAX_CONTAS 100

typedef struct {
    int id;
    char nome[50];
    float valor;
    char data_vencimento[11];
    char categoria[50];
} Conta;

// Funções para manipulação das contas
void cadastrar_conta();
void listar_contas();
void menuFinanceiro();

#endif // FINANCEIRO_H
