#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/login.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

typedef struct _user{
    char username[50];
    char senha[50];
    char cargo[20];
}Usuario;

// Função para capturar senha com asteriscos
void ler_senha(char *senha) {
    int i = 0;
#ifdef _WIN32
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' é o Enter no Windows
        if (ch == 8) {  // Backspace
            if (i > 0) {
                i--;
                printf("\b \b"); // Apaga o asterisco
            }
        } else {
            senha[i++] = ch;
            printf("*");
        }
    }
    senha[i] = '\0'; // Finaliza a string
#else
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((ch = getchar()) != '\n' && i < 49) { // Limita a 49 caracteres
        senha[i++] = ch;
        putchar('*');
    }
        senha[i] = '\0'; // Finaliza a string
        printf("\n");

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    #endif
}



// pra controle de tela
void finalizar() {
    puts("Pressione <ENTER> para finalizar.");
    
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
bool verificar_login(const char *usuario, const char *senha, const char *cargo_escolhido){
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

        getchar();

        printf("Digite a senha: ");
        ler_senha(senha);

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