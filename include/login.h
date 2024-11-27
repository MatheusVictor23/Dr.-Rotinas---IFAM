#ifndef LOGIN_H
#define LOGIN_H

typedef enum {
    ADMIN = 0,
    MEDICO = 1,
    RECEPCIONISTA = 2
} Cargo;

typedef struct {
    char usuario[50];
    char senha[50];
    Cargo cargo;
} Usuario;

void menu();

int login(int opcao_escolhida){

    // abre o arquivo binário para leitura
    FILE *fp = fopen("../data/usuarios.bin", "rb");

    if(fp == NULL){
        puts("Erro ao abrir o arquivo!");
        exit(EXIT_FAILURE);
    }


}

#endif
