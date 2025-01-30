#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "../../include/login/login.h"
#include "../../include/admin/recepcionista.h"
#include "../../include/admin/admin.h"

typedef struct _user{
    char username[50];
    char senha[50];
    char cargo[20];
    char cpf[15];
}Usuario;

typedef struct Recepcionista {
    int id;
    char nome[50];
    char telefone[15];
    char cpf[15];
    char cargo[20];
    struct Recepcionista *proximo; // Ponteiro para o proximo no da lista
} Recepcionista;

Recepcionista *listaRecepcionistas = NULL; // Cabeça da lista

int static gerarID() {
    return rand() % 9000 + 1001;  // ID entre 1001 e 10000
}

//Declarar cores como variáveis globais
const static int YELLOW = 14;
const static int RESET = 7;

void static setColor(int textColor){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, textColor);
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

void static imprimirMenu(const char *opcoes[], const char *opcoesDestacadas[], int num_opcoes, int destaque, int espaco_max) {
    int i;
    for (i = 0; i < num_opcoes; i++) {
        if (i == destaque) {
            printf("\t\t|");
            setColor(YELLOW);
            printf("       %-*s   ", espaco_max, opcoesDestacadas[i]); // Imprime opção destacada
            setColor(RESET);
            printf(" |\n");
        } else {
            setColor(RESET);
            printf("\t\t|       %-*s    |\n", espaco_max, opcoes[i]); // Imprime opção normal
        }
    }
}

void menuRecepcionistas() {
    int destaque = 0; // Opção destacada
    int escolha = -1;  // Opção escolhida
    int espaco_max = 30; // Aumentei o espaço vazio da caixa para 30

    const char *opcoes[] = { // Vetor de opções
        "Cadastrar Recepcionista",
        "Listar Recepcionistas",
        "Excluir Recepcionista",
        "Menu Admin"
    };

    const char *opcoesDestacadas[] = { // Vetor de opções destacadas
        "> Cadastrar Recepcionista <",
        "> Listar Recepcionistas <",
        "> Excluir Recepcionista <",
        "> Menu Admin <"
    };

    int num_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);

    do {
        system("cls || clear");

        puts("\t\t+-----------------------------------------+"); // Ajustei o número de traços
        puts("\t\t|                                         |"); // Ajustei o número de espaços
        puts("\t\t|   GERENCIAMENTO RECEPCIONISTA           |");
        puts("\t\t|                                         |"); // Ajustei o número de espaços
        puts("\t\t+-----------------------------------------+"); // Ajustei o número de traços
        puts("\t\t|        Deseja Realizar:                 |"); // Ajustei o número de espaços
        puts("\t\t|                                         |"); // Ajustei o número de espaços
        imprimirMenu(opcoes, opcoesDestacadas, num_opcoes, destaque, espaco_max); // Imprime o menu dinamicamente
        puts("\t\t|                                         |"); // Ajustei o número de espaços
        puts("\t\t|                                         |"); // Ajustei o número de espaços
        puts("\t\t+-----------------------------------------+"); // Ajustei o número de traços

        escolha = controlaEntrada(&destaque, num_opcoes); // Controla a entrada do usuário
        switch (escolha) {
            case 0:
                system("cls || clear");
                cadastrarRecepcionista();
                break;
            case 1:
                system("cls || clear");
                listarRecepcionistas();
                break;
            case 2:
                system("cls || clear");
                excluirRecepcionista();
                break;
            case 3:
                system("cls || clear");
                menuPrincipal();
                break;
        }
        escolha = -1; // Ao voltar ao menu principal, a escolha deve ser resetada
    } while (escolha != 3);
}


/* void menuRecepcionistas() {
    int op;
    carregarRecepcionistas();
    do {
        system("cls || clear");
        puts("\t+------------------------------+");
        puts("\t|                              |");
        puts("\t| GERENCIAMENTO RECEPCIONISTA  |");
        puts("\t|                              |");
        puts("\t+------------------------------+");
        puts("\t|       Deseja Realizar:       |");
        puts("\t|                              |");
        puts("\t| 1. Cadastrar Recepcionista   |");
        puts("\t| 2. Listar Recepcionista      |");
        puts("\t| 3. Excluir Recepcionista     |");
        puts("\t| 0. Voltar ao Menu Principal  |");
        puts("\t|                              |");
        puts("\t+------------------------------+");
        printf("Digite a opcao: ");
        scanf("%i", &op);

        switch (op) {
            case 1:
                cadastrarRecepcionista();
                break;
            case 2:
                listarRecepcionistas();
                break;
            case 3: 
                excluirRecepcionista();
            case 0:
                salvarRecepcionistas();
                return;
            default:
                printf("Opção invalida. Tente novamente.\n");
        }
    } while (op != 0);
} */

// Função para aplicar máscara enquanto digita
void ler_cpf_com_mascara_recepcionista(char *cpf) {
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

void static salvarCredenciais(const char *nome, const int id, const char *cpf, char *cargo){
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
    FILE *arquivo_recepcionistas = fopen("../data/usuarios.bin", "ab");  // Modo binário (ab)
    if (arquivo_recepcionistas == NULL) {
        perror("Erro ao salvar credenciais.");
        return;
    }

    // Salva a estrutura Usuario
    fwrite(&usuario, sizeof(Usuario), 1, arquivo_recepcionistas);
    fclose(arquivo_recepcionistas);

    printf("Credenciais do recepcionista: \n");
    printf("Usuário: %s\n", usuario.username);
    printf("Senha: %s\n", usuario.senha);
}

void cadastrarRecepcionista() {
    // Cria um novo no para o recepcionista
    Recepcionista *novo = (Recepcionista *)malloc(sizeof(Recepcionista));
    if (!novo) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    srand(time(NULL)); // inicializa a semente com o tempo atual
    int id = gerarID();

    printf("\n--- Cadastrar Recepcionista ---\n");
    novo->id = id;
    printf("ID: %d\n", novo->id);

    // NOME
    printf("Nome: ");
    fgets(novo->nome, sizeof(novo->nome), stdin);
    strtok(novo->nome, "\n"); // Remove o '\n' que fgets adiciona

    // TELEFONE
    printf("Telefone: ");
    fgets(novo->telefone, sizeof(novo->telefone), stdin);
    strtok(novo->telefone, "\n");

    // CPF
    ler_cpf_com_mascara_recepcionista(novo->cpf);

    // Escreve o cargo
    strcpy(novo->cargo, "recepcionista");

    // Insere o novo recepcionista no início da lista
    novo->proximo = listaRecepcionistas;
    listaRecepcionistas = novo;

    // Salvar no arquivo binário
    FILE *arquivo = fopen("../data/recepcionistas.bin", "ab");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para salvar.");
        free(novo);
        return;
    }

    fwrite(novo, sizeof(Recepcionista) - sizeof(Recepcionista *), 1, arquivo);
    fclose(arquivo);

    // cria e salva credenciais
    salvarCredenciais(novo->nome, novo->id, novo->cpf, novo->cargo);

    printf("Recepcionista cadastrado com sucesso!\n");
    finalizar();
}

// Lista os Recep. a partir do arquivo binario
void listarRecepcionistas() {
    FILE *arquivo = fopen("../data/recepcionistas.bin", "rb");

    if (!arquivo) {
        printf("\nNenhum recepcionista encontrado.\n");
        finalizar();
        return;
    }

    Recepcionista temp;
    printf("\n--- Lista de Recepcionistas ---\n");
    while (fread(&temp, sizeof(Recepcionista) - sizeof(Recepcionista *), 1, arquivo) == 1) {
        printf("ID: %d\n", temp.id);
        printf("Nome: %s\n", temp.nome);
        printf("Telefone: %s\n", temp.telefone);
        printf("CPF: %s\n", temp.cpf);
        printf("---------------------------\n");
    }
    fclose(arquivo);

    finalizar();
}


// EXCLUSAO
void excluirRecepcionista() {
    int id_recep;
    printf("Digite o ID do recepcionista a ser excluido: ");
    scanf("%d", &id_recep);

    FILE *arquivo = fopen("../data/recepcionistas.bin", "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo.");
        return;
    }

    FILE *arquivoTemp = fopen("../data/temp_recepcionistas.bin", "wb");
    if (!arquivoTemp) {
        perror("Erro ao criar o arquivo temporario.");
        fclose(arquivo);
        return;
    }

    Recepcionista tempRecepcionista;
    int recepcionistaExcluido = 0;

    // Ler e gravar os recepcionistas no arquivo temporário, excluindo o recepcionista com o ID fornecido
    while (fread(&tempRecepcionista, sizeof(Recepcionista) - sizeof(Recepcionista *), 1, arquivo) == 1) {
        if (tempRecepcionista.id != id_recep) {
            // Se não for o recepcionista a ser excluído, grava no arquivo temporário
            fwrite(&tempRecepcionista, sizeof(Recepcionista) - sizeof(Recepcionista *), 1, arquivoTemp);
        } else {
            recepcionistaExcluido = 1; // recep encontrado e excluído
        }
    }

    fclose(arquivo);
    fclose(arquivoTemp);

    if (!recepcionistaExcluido) {
        printf("Recepcionista com ID %d não encontrado.\n", id_recep);
        remove("../data/temp_recepcionistas.bin");
        return;
    }

    // Substituir o arquivo original pelo temporário
    if (remove("../data/recepcionistas.bin") != 0) {
        perror("Erro ao remover o arquivo original.");
        return;
    }

    if (rename("../data/temp_recepcionistas.bin", "../data/recepcionistas.bin") != 0) {
        perror("Erro ao renomear o arquivo temporário.");
        return;
    }

    getchar();
    printf("Recepcionista excluído com sucesso.\n");
    finalizar();
}

void static limparLista() {
    Recepcionista *atual = listaRecepcionistas;
    while (atual) {
        Recepcionista *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    listaRecepcionistas = NULL;
}

void carregarRecepcionistas() {
    limparLista(); // limpa a lista antes de carregar de novo

    FILE *arquivo = fopen("../data/recepcionistas.bin", "rb");
    if (!arquivo) {
        return; // Sem recepcionistas salvos
    }

    Recepcionista temp;
    while (fread(&temp, sizeof(Recepcionista), 1, arquivo) == 1) {
        Recepcionista *novo = (Recepcionista *)malloc(sizeof(Recepcionista));
        if (!novo) {
            printf("Erro ao alocar memoria.\n");
            fclose(arquivo);
            return;
        }

        *novo = temp;
        novo->proximo = listaRecepcionistas;
        listaRecepcionistas = novo;
    }

    fclose(arquivo);
    
}
