#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../include/login/login.h"
#include "../../include/admin/admin.h"
#include "../../include/medico/prontuario.h"
#include "../../include/recepcionista/consulta.h"

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
    char cpf[15];
}Usuario;


// Função para capturar um caractere sem exibir na tela (não bloqueante)
char get_char_input() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

// Função para aplicar máscara enquanto digita
void ler_cpf_com_mascara(char *cpf) {
    int i = 0; // Índice para a string do CPF
    char input;

    printf("Digite o CPF (apenas números): ");

    while (i < 14) { // 14 porque o CPF formatado tem 14 caracteres (incluindo '.' e '-')
        input = get_char_input();

        // Verifica se é um número
        if (input >= '0' && input <= '9') {
            if (i == 3 || i == 7) {
                cpf[i++] = '.';
                putchar('.');
            }
            if (i == 11) {
                cpf[i++] = '-';
                putchar('-');
            }
            cpf[i++] = input;
            putchar(input);
        } else if (input == '\b' && i > 0) { // Backspace
            if (cpf[i - 1] == '.' || cpf[i - 1] == '-') {
                i--; // Remove o separador
            }
            i--;
            printf("\b \b"); // Remove o último caractere da tela
        } else if (input == '\n' || input == '\r') {
            break; // Enter termina a entrada
        }
    }

    cpf[i] = '\0'; // Finaliza a string
    printf("\n");
}


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
        printf("Erro ao abrir o arquivo de usuários teste\n");
        return;
    }

    // Definindo os usuarios para teste
  
       Usuario admin = {"admin", "admin123","admin", "010.559.452.89",};
       Usuario medico = {"medico", "medico123", "medico", "010.569.452.89"};
       Usuario recepcionista =  {"recepcionista", "recep123", "recepcionista", "010.559.452.81"};

    // gravando os usuarios no arquivo bin
    fwrite(&admin, sizeof(Usuario), 1, arquivo);
    fwrite(&medico, sizeof(Usuario), 1, arquivo);
    fwrite(&recepcionista, sizeof(Usuario), 1, arquivo);

    printf("Usuarios inseridos!\n\n");

    // Fechando o arquivo
    fclose(arquivo);

}

// depois que o usuario escolhe o cargo e insere as credenciais.
bool verificar_login(const char *usuario, const char *senha, const char *cargo_escolhido){
        FILE *fp = fopen("../data/usuarios.bin", "rb");
        if(fp == NULL){
                printf("Falha ao ler os usuários do arquivo usuarios.bin!\n");
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
                                printf("\nVoce nao tem permissao para acessar essa area. Cargo incorreto\n");
                                return false;
                        }
                }
        }

        fclose(fp);
        printf("\nCredenciais invalidas\n");
        return false;
}

void fazer_login(char *cargo_escolhido) {
        char usuario[50], senha[50];

        printf("Digite o nome de usuario: ");
        scanf("%s", usuario);

        getchar();

        printf("Digite a senha: ");
        ler_senha(senha);

        if(verificar_login(usuario, senha, cargo_escolhido)){
            if(strcmp(cargo_escolhido, "admin") == 0) {
                menuPrincipal();
            }
            else if(strcmp(cargo_escolhido, "medico") == 0){
                printf("---------------------------------------\n");
                printf("Você logou como Médico.\n");
                menuMedico();
            }
            else {
                printf("---------------------------------------\n");
                printf("Você logou como recepcionista.\n");
                menuRecepcionista();
            }
        }

        finalizar();
        
}

bool verificar_cpf(char *cpf_informado) {
    FILE *credenciais = fopen("../data/usuarios.bin", "rb");
    if (credenciais == NULL) {
        perror("Erro ao carregar as credenciais.");
        return false;
    }

    Usuario usuario;
    bool cpfEncontrado = false;

    // Lê os dados do arquivo até encontrar o CPF
    while (fread(&usuario, sizeof(Usuario), 1, credenciais) == 1) {
        if (strcmp(cpf_informado, usuario.cpf) == 0) { // Se o CPF for encontrado
            cpfEncontrado = true;
            break;
        }
    }

    // Se encontrado, solicita nova senha
    if (cpfEncontrado) {
        char novaSenha[50];
        printf("CPF encontrado, digite sua nova senha: ");
        ler_senha(novaSenha);  // Aqui você pode usar sua função para ler a senha

        // Substitui a senha no arquivo binário
        FILE *arquivoTemp = fopen("../data/credenciais_temp.bin", "wb");
        if (arquivoTemp == NULL) {
            perror("Erro ao abrir arquivo temporário para salvar credenciais.");
            fclose(credenciais);
            return false;
        }

        // Rewind e reescreve os dados com a nova senha
        rewind(credenciais); // Volta para o início do arquivo
        while (fread(&usuario, sizeof(Usuario), 1, credenciais) == 1) {
            if (strcmp(cpf_informado, usuario.cpf) == 0) {
                // Atualiza a senha do usuário encontrado
                strcpy(usuario.senha, novaSenha);  // Substitui a senha com a nova
            }

            // Escreve o usuário no arquivo temporário
            fwrite(&usuario, sizeof(Usuario), 1, arquivoTemp);
        }

        fclose(credenciais);
        fclose(arquivoTemp);

        // Substitui o arquivo original pelo arquivo temporário
        remove("../data/usuarios.bin");
        rename("../data/credenciais_temp.bin", "../data/usuarios.bin");

        printf("Senha alterada com sucesso!\n");
        return true;

    } else {
        printf("CPF não encontrado.\n");
        fclose(credenciais);
        return false;
    }
}


bool verificar_usuario(const char *nome_usuario){
    char cpf[15];
    FILE *fp = fopen("../data/usuarios.bin", "rb");
    
    if(fp == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return false;
    }

    Usuario user;
    bool usuario_encontrado = false;
    
    // Percorre o arquivo em busca do nome de usuário
    while(fread(&user, sizeof(Usuario), 1, fp)){
        if(strcmp(user.username, nome_usuario) == 0){
            usuario_encontrado = true;
            break;  // Encerra o loop assim que encontrar o usuário
        }
    } 

    if(usuario_encontrado){
        fclose(fp);  // Fecha o arquivo após encontrar o usuário
        ler_cpf_com_mascara(cpf);  // Solicita o CPF
        printf("CPF digitado: %s\n", cpf);
        verificar_cpf(cpf);  // Verifica o CPF
        finalizar();  // Finaliza a execução
        return true;  // Usuário encontrado
    } else {
        fclose(fp);  // Fecha o arquivo após percorrer todo
        puts("### Nome de usuario nao encontrado no sistema ###");
        getchar();  // Para a entrada do usuário
        finalizar();  // Finaliza a execução
        return false;  // Usuário não encontrado
    }
}



void imprimirCredenciais() {
    FILE *arquivo = fopen("../data/usuarios.bin", "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de credenciais.");
        return;
    }

    Usuario usuario;
    
    printf("\n--- Lista de Credenciais ---\n");
    // Lê os dados do arquivo até o final
    // Lê os dados do arquivo binário
    // Lê os dados do arquivo binário até o fim
    while (fread(&usuario, sizeof(Usuario), 1, arquivo) == 1) {
        printf("Usuário: %s\n", usuario.username);
        printf("Senha: %s\n", usuario.senha);
        printf("Cargo: %s\n", usuario.cargo);
        printf("CPF: %s\n", usuario.cpf);
        printf("---------------------------\n");
    }

    fclose(arquivo);
    finalizar();
}


void menu(){
    int op;

    do {

    system("cls || clear");

    puts("\t+------------------------------+");
    puts("\t|  BEM VINDO(A) AO DR. ROTINAS |");
    puts("\t|  Faca login para continuar   |");
    puts("\t+------------------------------+");
    puts("\t|   Deseja fazer login como:   |");
    puts("\t|                              |");
    puts("\t| 1. Admin                     |");
    puts("\t| 2. Medico                    |");
    puts("\t| 3. Recepcionista             |");
    puts("\t| 4. Esqueci a senha           |");
    puts("\t| 0. Sair do Sistema           |");
    puts("\t|                              |");
    puts("\t+------------------------------+");
    printf("Digite a opcao: ");
    scanf("%i", &op);

    char cargo_escolhido[20];
    char nome_usuario[50];

    switch(op) {
        case 1: strcpy(cargo_escolhido, "admin");
                system("cls || clear");
                puts("Voce escolheu a opcao Admin.");
                fazer_login(cargo_escolhido);
                break;

        case 2: strcpy(cargo_escolhido, "medico");
                system("cls || clear");
                puts("Você escolheu a opcao Medico.");
                fazer_login(cargo_escolhido);
                break;

        case 3: strcpy(cargo_escolhido, "recepcionista");
                system("cls || clear");
                puts("Voce escolheu a opcao Recepcionista.");
                fazer_login(cargo_escolhido);
                break;

        case 4: system("cls || clear");
                getchar();
                puts("Informe seu nome de usuário para continuar:");
                printf("-> ");
                fgets(nome_usuario, sizeof(nome_usuario), stdin);
                nome_usuario[strcspn(nome_usuario, "\n")] = '\0'; // Remove o caractere de nova linha se existir
                // scanf("%s", nome_usuario);
                verificar_usuario(nome_usuario);
                break;

        case 5: system("cls || clear");
                getchar();
                imprimirCredenciais();
                break;

        case 0: getchar();
                puts("Saindo...\n");
                exit(0);

        default:
                getchar();
                puts("Entrada inválida");
                finalizar();
                break;
                 
    }

    } while (op != 0);


}