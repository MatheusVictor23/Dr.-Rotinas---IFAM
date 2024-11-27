#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/login.h"



void finalizar() {
    puts("Pressione <ENTER> para finalizar.");
    getchar();
    getchar();
    
}

void menu(){
    int op;

    do {

    system("cls || clear");

    puts("\t+------------------------------+");
    puts("\t|  BEM VINDO(A) AO DR. ROTINAS |");
    puts("\t|  Faça login para continuar   |");
    puts("\t+------------------------------+");
    puts("\t|    Deseja fazer login como:  |");
    puts("\t|                              |");
    puts("\t| 1. Admin                     |");
    puts("\t| 2. Médico                    |");
    puts("\t| 3. Recepcionista             |");
    puts("\t| 0. Sair do Sistema           |");
    puts("\t|                              |");
    puts("\t+------------------------------+");
    printf("Digite a opção: ");
    scanf("%i", &op);

    switch(op) {
        case 1: system("cls || clear");
                puts("Tela de admin");
                finalizar();
                break;

        case 2: system("cls || clear");
                puts("Tela de médico");
                finalizar();
                break;

        case 3: system("cls || clear");
                puts("Tela de recepcionista");
                finalizar();
                break;

        case 0: puts("Saindo...\n");
                break;

        default: puts("Entrada inválida");
                 finalizar();
                 break;
                 
    }

    } while (op != 0);


}