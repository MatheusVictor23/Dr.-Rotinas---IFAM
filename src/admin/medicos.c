#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
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

//Declarar cores como variáveis globais
const static int YELLOW = 14;
const static int RESET = 7;

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

void menuMedicos() {
    int destaque = 0; // Opção destacada
    int escolha = -1;  // Opção escolhida
    int espaco_max = 23; // Definindo o espaço vázio da caixa, mude se necessário

    const char *opcoes[] = { // Vetor de opções
        "Cadastrar Medico",
        "Listar Medicos",
        "Excluir Medico",
        "Menu Admin"
    };
    
    const char *opcoesDestacadas[] = { // Vetor de opções destacadas
        "> Cadastrar Medico <",
        "> Listar Medicos <",
        "> Excluir Medico <",
        "> Menu Admin <"
    };
    
    int num_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);

    do {
        system("cls || clear");
        
      
        puts("\t\t+--------------------------------+");
        puts("\t\t|                                |");
        puts("\t\t|     GERENCIAMENTO MEDICO       |");
        puts("\t\t|                                |");
        puts("\t\t+--------------------------------+");
        puts("\t\t|        Deseja Realizar:        |");
        puts("\t\t|                                |");
        imprimirMenu(opcoes, opcoesDestacadas, num_opcoes, destaque, espaco_max); // Imprime o menu dinamicamente
        puts("\t\t|                                |");
        puts("\t\t|                                |");
        puts("\t\t+--------------------------------+");

        escolha = controlaEntrada(&destaque, num_opcoes); // Controla a entrada do usuário
        switch (escolha) {
            case 0:
                system("cls || clear");
                cadastrarMedico();
                break;
            case 1:
                system("cls || clear");
                listarMedicosDiretamenteArquivo();
                break;
            case 2:
                system("cls || clear");
                excluirMedico();
                break;
            case 3:
                system("cls || clear");
                menuPrincipal();
                break;
        }
        escolha = -1; // Ao voltar ao menu principal, a escolha deve ser resetada
    } while (escolha != 3);
}


/* void menuMedicos() {
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
} */

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

    printf("Credenciais do medico: \n");
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
    printf("Nome: ");
    fgets(novo->nome, sizeof(novo->nome), stdin);
    strtok(novo->nome, "\n"); // Remove o '\n' que fgets adiciona
    
    // CPF
    ler_cpf_com_mascara_medico(novo->cpf);
    
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

    printf("Medico cadastrado com sucesso!\n");
    finalizar();
}

// Lista os médicos a partir do arquivo binário
void listarMedicosDiretamenteArquivo() {
    FILE *arquivo = fopen("../data/medicos.bin", "rb");

    if (!arquivo) {
        printf("\nNenhum medico encontrado.\n");
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
        perror("Erro ao criar o arquivo temporario.");
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
