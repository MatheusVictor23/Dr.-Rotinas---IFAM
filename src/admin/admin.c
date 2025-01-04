#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "../../include/admin/admin.h"
#include "../../include/admin/medicos.h"
#include "../../include/admin/recepcionista.h"
#include "../../include/login/login.h"
#include "../../include/financeiro/financeiro.h"


// void finalizar() {
//     puts("Pressione <ENTER> para finalizar.");
    
//     getchar();
    
// }

void menuPrincipal(){
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

}
