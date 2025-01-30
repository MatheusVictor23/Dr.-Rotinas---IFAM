#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "../../include/admin/admin.h"
#include "../../include/admin/medicos.h"
#include "../../include/admin/recepcionista.h"
#include "../../include/login/login.h"
#include "../../include/financeiro/financeiro.h"


/*  ----- NOVO MENU DINAMICO ---- */

//Declarar cores como variáveis globais
int YELLOW = 14;
int RESET = 7;

void static setColor(int textColor){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, textColor);
}

void static imprimirMenu(const char *opcoes[], const char *opcoesDestacadas[], int num_opcoes, int destaque, int espaco_max) {
    int i;
    for (i = 0; i < num_opcoes; i++) {
        if (i == destaque) {
        	printf("\t\t|");
        	setColor(YELLOW);
            printf("       %-*s  ", espaco_max, opcoesDestacadas[i]); // Imprime opção destacada
            setColor(RESET);
            printf("|\n");
        } else {
        	setColor(RESET);
            printf("\t\t|       %-*s  |\n", espaco_max, opcoes[i]); // Imprime opção normal
        }
    }
}

char static controlaEntrada(int *destaque, int num_opcoes) {
    char ch = getch(); // Captura a tecla pressionada
    switch (ch) {
        case 72: // Seta para cima
            *destaque = (*destaque > 0) ? *destaque - 1 : num_opcoes - 1;
            break;
        case 80: // Seta para baixo
            *destaque = (*destaque < num_opcoes - 1) ? *destaque + 1 : 0;
            break;
        case 13: // Enter
            return *destaque; // Retorna a escolha
    }
    return -1; // Nenhuma escolha
}

void menuPrincipal() {
    int destaque = 0; // Opção destacada
    int escolha = -1;  // Opção escolhida
    int espaco_max = 23; // Definindo o espaço vázio da caixa, mude se necessário

    const char *opcoes[] = { // Vetor de opções
        "Medicos",
        "Recepcionistas",
        "Menu Financeiro",
        "Voltar ao Login"
    };
    
    const char *opcoesDestacadas[] = { // Vetor de opções destacadas
        "> Medico <",
        "> Recepcionistas <",
        "> Menu Financeiro <",
        "> Voltar ao Login <"
    };
    
    int num_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);

    do {
        system("cls || clear");
        
      
        puts("\t\t+--------------------------------+");
        puts("\t\t|                                |");
        puts("\t\t|       BEM VINDO(A) ADMIN       |");
        puts("\t\t|                                |");
        puts("\t\t+--------------------------------+");
        puts("\t\t|       Deseja Gerenciar:        |");
        puts("\t\t|                                |");
        imprimirMenu(opcoes, opcoesDestacadas, num_opcoes, destaque, espaco_max); // Imprime o menu dinamicamente
        puts("\t\t|                                |");
        puts("\t\t|                                |");
        puts("\t\t+--------------------------------+");

        escolha = controlaEntrada(&destaque, num_opcoes); // Controla a entrada do usuário
        switch (escolha) {
            case 0:
                system("cls || clear");
                menuMedicos();
                break;
            case 1:
                system("cls || clear");
                menuRecepcionistas();
                break;
            case 2:
                system("cls || clear");
                menuFinanceiro();
                break;
            case 3:
                puts("Saindo...\n");
                menu();
                break;
        }
        escolha = -1; // Ao voltar ao menu principal, a escolha deve ser resetada
    } while (escolha != 3);
}


/* void menuPrincipal(){
    int op;


    do {

    system("cls || clear");

    puts("\t+------------------------------+");
    puts("\t|                              |");
    puts("\t|      BEM VINDO(A) ADMIN      |");
    puts("\t|                              |");
    puts("\t+------------------------------+");
    puts("\t|       Deseja Gerenciar:      |");
    puts("\t|                              |");
    puts("\t| 1. Medico                    |");
    puts("\t| 2. Recepcionista             |");
    puts("\t| 3. MenuFinanceiro            |");
    puts("\t| 0. Voltar ao login           |");
    puts("\t|                              |");
    puts("\t+------------------------------+");
    printf("Digite a opcao: ");
    scanf("%i", &op);


    switch(op) {
        case 1: system("cls || clear");
                menuMedicos();
                break;

        case 2: system("cls || clear");
                menuRecepcionistas();
                break;

        case 3: system("cls || clear");
                menuFinanceiro();
                break;
        
        case 0: puts("Saindo...\n");
                menu();
                break;

        default: puts("Entrada invalida");
                //  finalizar();
                 break;
                 
    }

    } while (op != 0);

} */

