#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 3 
#define MAX_LENGTH 32

// Direcionamento das telas, de acordo com o login

void telaAdmin() {
    printf("Bem vindo, admin!\n");
}

void telaMedico() {
    printf("Bem vindo, medico!\n");
}

void telaRecepcao() {
    printf("Bem vindo, recepcionista!\n");
}

/* metodo que verifica se os dados estao corretos */

int login(char *usernames[], char *senhas[], const char *user, const char *senha) {

    for(int i = 0; i < MAX_USERS; i++) {
        if(strcmp(usernames[i], user) == 0 && strcmp(senhas[i], senha) == 0) {
            return 1; // login ok
        }
    }
    
    return 0; // login falhou
}


int main() {

    // Arrays de usuarios e senhas cadastrados

    char *usernames[MAX_USERS] = {"@admin", "@meduser", "@recepcao"};
    char *senhas[MAX_USERS] = {"rotinasdr01", "med01", "recep01"};

    char user[MAX_LENGTH], senha[MAX_LENGTH];

    printf("Usuario: ");
    scanf("%s", user);
    printf("Senha: ");
    scanf("%s", senha);
    
    /* Chama o metodo de verificar o login */

    if(login(usernames, senhas, user, senha)) {
        if(strcmp(user, "@admin") == 0) {
            telaAdmin();
        }
        else if(strcmp(user, "@meduser") == 0) {
            telaMedico();
        }
        else if(strcmp(user, "@recepcao") == 0) {
            telaRecepcao();
        }
        else {
            printf("Usuario nao encontrado.");
        }
    }
    else {
        printf("Falha no login. Verifique se o usuario e senha estao corretos."); // caso nao encontre um user e senha
    }

}
