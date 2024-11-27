#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/login.h"

typedef struct _user{
    char username[50];
    char senha[50];
    char cargo[20];
}Usuario;

// pra controle de tela
void finalizar() {
    puts("Pressione <ENTER> para finalizar.");
    getchar();
    getchar();
    
}

// ********** TESTANDO ARQUIVO BIN ******

void inserir_usuarios_de_teste() {


    // abrindo o arquivo binário 
    FILE *arquivo = fopen("../data/usuarios.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    // Definindo os usuarios para teste
  
       Usuario admin = {"admin", "admin123", "admin"};
       Usuario medico = {"medico", "medico123", "medico"};
       Usuario recepcionista =  {"recepcionista", "recep123", "recepcionista"};

    // gravando os usuarios no arquivo bin
    fwrite(&admin, sizeof(Usuario), 1, arquivo);
    fwrite(&medico, sizeof(Usuario), 1, arquivo);
    fwrite(&recepcionista, sizeof(Usuario), 1, arquivo);

    // Fechando o arquivo
    fclose(arquivo);

}

// depois que o usuario escolhe o cargo e insere as credenciais.
int verificar_login(const char *usuario, const char *senha, const char *cargo_escolhido){
        FILE *fp = fopen("../data/usuarios.bin", "rb");
        if(fp == NULL){
                printf("Erro ao abrir o arquivo!\n");
                return false;
        }

        Usuario user;
        while(fread(&user, sizeof(Usuario), 1, fp)) {
                // verifica se o usuario e a senha correspondem
                if(strcmp(user.username, usuario) == 0 && strcmp(user.senha, senha) == 0) {
                        // verifica se os cargos correspondem
                        if(strcmp(user.cargo, cargo_escolhido) == 0) {
                                fclose(fp);
                                return true; // login ok
                        }
                        else {
                                fclose(fp);
                                printf("\nVocê não tem permissão para acessar essa área. Cargo incorreto\n");
                                return false;
                        }
                }
        }

        fclose(fp);
        printf("\nCredenciais inválidas\n");
        return false;
}

void fazer_login(char *cargo_escolhido) {
        char usuario[50], senha[50];

        printf("Digite o nome de usuário: ");
        scanf("%s", usuario);

        printf("Digite a senha: ");
        scanf("%s", senha);

        if(verificar_login(usuario, senha, cargo_escolhido)){
                if(strcmp(cargo_escolhido, "admin") == 0) {
                        printf("---------------------------------------\n");
                        printf("Implementar tela do ADMIN.\n");
                }
                else if(strcmp(cargo_escolhido, "medico") == 0){
                        printf("---------------------------------------\n");
                        printf("Implementar tela do MEDICO.\n");
                }
                else {
                        printf("---------------------------------------\n");
                        printf("Implementar tela do RECEPCIONISTA.\n");
                }
        }

        finalizar();
        
}

void menu(){
    int op;

    do {

    system("cls || clear");

    puts("\t+------------------------------+");
    puts("\t|  BEM VINDO(A) AO DR. ROTINAS |");
    puts("\t|  Faça login para continuar   |");
    puts("\t+------------------------------+");
    puts("\t|   Deseja fazer login como:   |");
    puts("\t|                              |");
    puts("\t| 1. Admin                     |");
    puts("\t| 2. Médico                    |");
    puts("\t| 3. Recepcionista             |");
    puts("\t| 0. Sair do Sistema           |");
    puts("\t|                              |");
    puts("\t+------------------------------+");
    printf("Digite a opção: ");
    scanf("%i", &op);

    char cargo_escolhido[20];

    switch(op) {
        case 1: strcpy(cargo_escolhido, "admin");
                system("cls || clear");
                puts("Você escolheu a opção Admin.");
                fazer_login(cargo_escolhido);
                break;

        case 2: strcpy(cargo_escolhido, "medico");
                system("cls || clear");
                puts("Você escolheu a opção Médico.");
                fazer_login(cargo_escolhido);
                break;

        case 3: strcpy(cargo_escolhido, "recepcionista");
                system("cls || clear");
                puts("Você escolheu a opção Recepcionista.");
                fazer_login(cargo_escolhido);
                break;

        case 0: puts("Saindo...\n");
                break;

        default: puts("Entrada inválida");
                 finalizar();
                 break;
                 
    }

    } while (op != 0);


}