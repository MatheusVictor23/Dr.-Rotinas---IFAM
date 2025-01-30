#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../include/admin/medicos.h"
#include "../../include/login/login.h"
#include "../../include/admin/admin.h"

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


typedef struct Medico {
    int id;
    char nome[50];
    char telefone[15];
    char cpf[15];
    char especialidade[30];
    char cargo[20];
    char username[50];
    char senha[50];
    struct Medico *proximo; // Ponteiro para o proximo no da lista
} Medico;

Medico *listaMedicos = NULL; // Cabeça da lista

int gerarID(){
    return rand() % 1000 + 1; // id entre 1 e 1000
}

/* -------- NOVO MENU DINAMICO --------- */




void menuMedicos() {
    int op;
    carregarMedicos();

    do {
        system("cls || clear");

        puts("\t+------------------------------+");
        puts("\t|                              |");
        puts("\t|     GERENCIAMENTO MEDICO     |");
        puts("\t|                              |");
        puts("\t+------------------------------+");
        puts("\t|       Deseja Realizar:       |");
        puts("\t|                              |");
        puts("\t| 1. Cadastrar Medico          |");
        puts("\t| 2. Listar Medicos            |");
        puts("\t| 3. Excluir Medico            |");
        puts("\t| 0. Voltar ao Menu Principal  |");
        puts("\t|                              |");
        puts("\t+------------------------------+");
        printf("Digite a opcao: ");
        scanf("%i", &op);

        switch (op) {
            case 1:
                system("cls || clear");
                cadastrarMedico();
                break;
            case 2:
                system("cls || clear");
                listarMedicosDiretamenteArquivo();
                break;
            case 3:
                system("cls || clear");
                excluirMedico();
                break;
            case 0:
                system("cls || clear");
                menuPrincipal();
                break;

            default:
                getchar();
                printf("Opçao invalida. Tente novamente.\n");
                finalizar();
                break;
        }
    } while (op != 0);
}

// Função para capturar um caractere sem exibir na tela (não bloqueante)
char get_char_input_medico() {
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
void ler_cpf_com_mascara_medico(char *cpf) {
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

void salvarCredenciais(const char *nome, const int id, const char *cpf, char *cargo){
    Usuario usuario;
    
    // Gera o nome de usuário (nome + id)
    snprintf(usuario.username, sizeof(usuario.username), "%s%d", nome, id);

    // Gera a senha (4 primeiros dígitos do CPF)
    int i = 0;
    int cpf_index = 0;
    while (cpf[cpf_index] != '\0' && i < 4) {
        if (cpf[cpf_index] >= '0' && cpf[cpf_index] <= '9') {
            usuario.senha[i++] = cpf[cpf_index];  // armazena e itera
        }
        cpf_index++;
    }
    usuario.senha[i] = '\0';  // Finaliza a senha

    // Copia o CPF e o cargo
    strncpy(usuario.cpf, cpf, sizeof(usuario.cpf));
    strncpy(usuario.cargo, cargo, sizeof(usuario.cargo));

    // Salva as credenciais no arquivo binário
    FILE *arquivo_medicos = fopen("../data/usuarios.bin", "ab");  // Modo binário (ab)
    if (arquivo_medicos == NULL) {
        perror("Erro ao salvar credenciais.");
        return;
    }

    // Salva a estrutura Usuario
    fwrite(&usuario, sizeof(Usuario), 1, arquivo_medicos);
    fclose(arquivo_medicos);

    printf("Credenciais do médico: \n");
    printf("Usuário: %s\n", usuario.username);
    printf("Senha: %s\n", usuario.senha);
}

void cadastrarMedico() {
    Medico *novo = (Medico *)malloc(sizeof(Medico));
    if (!novo) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    srand(time(NULL)); // inicializa a semente com o tempo atual
    int id = gerarID();

    printf("\n--- Cadastrar Medico ---\n");
    novo->id = id;
    printf("ID: %d\n", novo->id);

    // NOME
    getchar();
    printf("Nome: ");
    fgets(novo->nome, sizeof(novo->nome), stdin);
    strtok(novo->nome, "\n"); // Remove o '\n' que fgets adiciona
    
    // CPF
    ler_cpf_com_mascara(novo->cpf);
    
    // TELEFONE
    printf("Telefone: ");
    fgets(novo->telefone, sizeof(novo->telefone), stdin);
    strtok(novo->telefone, "\n");
    // ESPECIALIDADE
    printf("Especialidade: ");
    fgets(novo->especialidade, sizeof(novo->especialidade), stdin);
    strtok(novo->especialidade, "\n");

    // Escreve o cargo
    strcpy(novo->cargo, "medico");

    novo->proximo = listaMedicos;
    listaMedicos = novo;

    // Salvar no arquivo binário
    FILE *arquivo = fopen("../data/medicos.bin", "ab");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para salvar.");
        free(novo);
        return;
    }

    fwrite(novo, sizeof(Medico) - sizeof(Medico *), 1, arquivo);
    fclose(arquivo);

    // cria e salva credenciais
    salvarCredenciais(novo->nome, novo->id, novo->cpf, novo->cargo);

    printf("Médico cadastrado com sucesso!\n");
    finalizar();
}


// Lista os médicos a partir da memória

// void listarMedicos() {
//     if (!listaMedicos) {
//         getchar();
//         printf("Nenhum médico encontrado.\n");
//         finalizar();
//         return;
//     }

//     Medico *atual = listaMedicos;
//     printf("\n--- Lista de Medicos ---\n");
//     while (atual) {
//         printf("ID: %d\n", atual->id);
//         printf("Nome: %s\n", atual->nome);
//         printf("CPF: %s\n", atual->cpf);
//         printf("Telefone: %s\n", atual->telefone);
//         printf("Especialidade: %s\n", atual->especialidade);
//         printf("---------------------------\n");
//         atual = atual->proximo;
//     }
//     getchar();
//     finalizar();
// }


// Lista os médicos a partir do arquivo binário
void listarMedicosDiretamenteArquivo() {
    FILE *arquivo = fopen("../data/medicos.bin", "rb");

    if (!arquivo) {
        printf("\nNenhum médico encontrado.\n");
        getchar();
        finalizar();
        return;
    }

    Medico temp;
    printf("\n--- Lista de Medicos ---\n");
    while (fread(&temp, sizeof(Medico) - sizeof(Medico *), 1, arquivo) == 1) {
        printf("ID: %d\n", temp.id);
        printf("Nome: %s\n", temp.nome);
        printf("CPF: %s\n", temp.cpf);
        printf("Telefone: %s\n", temp.telefone);
        printf("Especialidade: %s\n", temp.especialidade);
        printf("---------------------------\n");
    }
    fclose(arquivo);

    getchar();
    finalizar();
}

void excluirMedico() {
    int id_med;
    printf("Digite o ID do medico a ser excluido: ");
    scanf("%d", &id_med);

    FILE *arquivo = fopen("../data/medicos.bin", "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo.");
        return;
    }

    FILE *arquivoTemp = fopen("../data/temp_medicos.bin", "wb");
    if (!arquivoTemp) {
        perror("Erro ao criar o arquivo temporário.");
        fclose(arquivo);
        return;
    }

    Medico tempMedico;
    int medicoExcluido = 0;

    // Ler e gravar os médicos no arquivo temporário, excluindo o médico com o ID fornecido
    while (fread(&tempMedico, sizeof(Medico) - sizeof(Medico *), 1, arquivo) == 1) {
        if (tempMedico.id != id_med) {
            // Se não for o médico a ser excluído, grava no arquivo temporário
            fwrite(&tempMedico, sizeof(Medico) - sizeof(Medico *), 1, arquivoTemp);
        } else {
            medicoExcluido = 1; // Médico encontrado e excluído
        }
    }

    fclose(arquivo);
    fclose(arquivoTemp);

    if (!medicoExcluido) {
        printf("Médico com ID %d não encontrado.\n", id_med);
        remove("../data/temp_medicos.bin");
        return;
    }

    // Substituir o arquivo original pelo temporário
    if (remove("../data/medicos.bin") != 0) {
        perror("Erro ao remover o arquivo original.");
        return;
    }

    if (rename("../data/temp_medicos.bin", "../data/medicos.bin") != 0) {
        perror("Erro ao renomear o arquivo temporário.");
        return;
    }

    printf("Médico excluído com sucesso.\n");
    getchar();
    finalizar();
}




void limparLista() {
    Medico *atual = listaMedicos;
    while (atual) {
        Medico *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    listaMedicos = NULL;
}

void carregarMedicos() {
    limparLista(); // limpa a lista antes de carregar de novo

    FILE *arquivo = fopen("../data/medicos.bin", "rb");
    if (!arquivo) {
        return; // Sem médicos salvos
    }

    Medico temp;
    while (fread(&temp, sizeof(Medico), 1, arquivo) == 1) {
        Medico *novo = (Medico *)malloc(sizeof(Medico));
        if (!novo) {
            printf("Erro ao alocar memoria.\n");
            fclose(arquivo);
            return;
        }

        *novo = temp;
        novo->proximo = listaMedicos;
        listaMedicos = novo;
    }

    fclose(arquivo);
    
}
