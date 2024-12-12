#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Definição do TAD de lista dinâmica encadeada para prontuário de paciente
typedef struct {
    char nome[100];
    char datanasc[20];
    char sexo[20];
    char peso[10];
    char altura[10];
    char observacao[100];
} ProntuarioPaciente;

typedef struct {
    char caraterRequisicao[20];
    char suspeitaClinica[100];
    char tipoExame[30];
    char Observacoes[80];
} SolicitarExame;

typedef struct {
    char motivo[100];
    char destino[100];
    char observacoes[80];
} Encaminhamento;

typedef struct No {
    ProntuarioPaciente prontuario;
    struct No *proximo;
    struct No2 *exames; // Ponteiro para lista de exames
    struct No3 *encaminhamentos; // Ponteiro para lista de encaminhamentos
} No;

typedef struct No2 {
    SolicitarExame exame;
    struct No2 *proximo;
} No2;

typedef struct No3 {
    Encaminhamento encaminhamento;
    struct No3 *proximo;
} No3;

typedef struct {
    No *inicio;
} Lista;

// Funções para manipulação da lista
Lista* criarLista() {
    Lista *lista = (Lista*)malloc(sizeof(Lista));
    lista->inicio = NULL;
    return lista;
}

void adicionarProntuario(Lista *lista, ProntuarioPaciente prontuario) {
    No *novoNo = (No*)malloc(sizeof(No));
    novoNo->prontuario = prontuario;
    novoNo->proximo = lista->inicio;
    novoNo->exames = NULL; // Inicializa a lista de exames como vazia
    novoNo->encaminhamentos = NULL; // Inicializa a lista de encaminhamentos como vazia
    lista->inicio = novoNo;
}

// Função para salvar dados no arquivo
void salvarProntuarioEmArquivo(Lista *lista) {
    FILE *arquivo = fopen("prontuario.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para gravacao.\n");
        return;
    }

    No *atual = lista->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "\nNome:%s\nData de Nascimento:%s\nSexo:%s\nPeso:%s\nAltura:%s\nObservacoes:%s\n",
                atual->prontuario.nome,
                atual->prontuario.datanasc,
                atual->prontuario.sexo,
                atual->prontuario.peso,
                atual->prontuario.altura,
                atual->prontuario.observacao);

        // Salva os exames desse paciente
        No2 *exameAtual = atual->exames;
        while (exameAtual != NULL) {
            fprintf(arquivo, "\nExame:%s\nCarater:%s\nSuspeita Clinica:%s\nObservacoes:%s\n",
                    exameAtual->exame.tipoExame,
                    exameAtual->exame.caraterRequisicao,
                    exameAtual->exame.suspeitaClinica,
                    exameAtual->exame.Observacoes);
            exameAtual = exameAtual->proximo;
        }

        // Salva os encaminhamentos desse paciente
        No3 *encaminhamentoAtual = atual->encaminhamentos;
        while (encaminhamentoAtual != NULL) {
            fprintf(arquivo, "\nEncaminhamento: %s\nDestino: %s\nObservacoes: %s\n",
                    encaminhamentoAtual->encaminhamento.motivo,
                    encaminhamentoAtual->encaminhamento.destino,
                    encaminhamentoAtual->encaminhamento.observacoes);
            encaminhamentoAtual = encaminhamentoAtual->proximo;
        }
        atual = atual->proximo;
    }
    fclose(arquivo);
    printf("Dados salvos com sucesso.\n");
}

void adicionarExame(Lista *lista, SolicitarExame exame, char *nome) {
    No *atual = lista->inicio;

    // Verifica se o paciente existe
    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            No2 *novoNo = (No2*)malloc(sizeof(No2));
            novoNo->exame = exame;
            novoNo->proximo = atual->exames; // Adiciona o exame ao início da lista de exames
            atual->exames = novoNo; // Atualiza a lista de exames do paciente
            printf("Solicitacao de exame feita com sucesso para o paciente: %s\n", nome);
            salvarProntuarioEmArquivo(lista); // Salva os exames
            return;
        }
        atual = atual->proximo;
    }
    printf("Paciente nao encontrado para adicionar exame.\n");
}

void adicionarEncaminhamento(Lista *lista, Encaminhamento encaminhamento, char *nome) {
    No *atual = lista->inicio;

    // Verifica se o paciente existe
    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            No3 *novoNo = (No3*)malloc(sizeof(No3));
            novoNo->encaminhamento = encaminhamento;
            novoNo->proximo = atual->encaminhamentos; // Adiciona o encaminhamento ao início da lista
            atual->encaminhamentos = novoNo; // Atualiza a lista de encaminhamentos do paciente
            printf("Encaminhamento feito com sucesso para o paciente: %s\n", nome);
            salvarProntuarioEmArquivo(lista); // Salva os encaminhamentos
            return;
        }
        atual = atual->proximo;
    }
    printf("Paciente nao encontrado para adicionar encaminhamento.\n");
}

void listarProntuarios(Lista *lista, char *nome) {
    No *atual = lista->inicio;

    if (atual == NULL) {
        printf("Nenhum prontuário cadastrado.\n");
        return;
    }

    printf("Lista de Prontuarios Medicos:\n");
    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            printf("Nome: %s | Data de Nascimento: %s | Sexo: %s | Peso: %s | Altura: %s | Observacoes: %s\n",
                   atual->prontuario.nome,
                   atual->prontuario.datanasc,
                   atual->prontuario.sexo,
                   atual->prontuario.peso,
                   atual->prontuario.altura,
                   atual->prontuario.observacao);
            return; // Encontrado e exibido
        }
        atual = atual->proximo;
    }

    printf("Nenhum prontuario encontrado para o paciente: %s\n", nome);
}

void listarExames(Lista *lista, char *nome) {
    No *atual = lista->inicio;

    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            No2 *exameAtual = atual->exames;
            if (exameAtual == NULL) {
                printf("Nenhum exame encontrado para o paciente: %s\n", nome);
                return;
            }
            printf("Exames do paciente %s:\n", nome);
            while (exameAtual != NULL) {
                printf("Tipo de Exame: %s | Carater: %s | Suspeita Clinica: %s | Observacoes: %s\n",
                       exameAtual->exame.tipoExame,
                       exameAtual->exame.caraterRequisicao,
                       exameAtual->exame.suspeitaClinica,
                       exameAtual->exame.Observacoes);
                exameAtual = exameAtual->proximo;
            }
            return;
        }
        atual = atual->proximo;
    }

    printf("Nenhum prontuario encontrado para o paciente: %s\n", nome);
}

void listarEncaminhamentos(Lista *lista, char *nome) {
    No *atual = lista->inicio;

    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            No3 *encaminhamentoAtual = atual->encaminhamentos;
            if (encaminhamentoAtual == NULL) {
                printf("Nenhum encaminhamento encontrado para o paciente: %s\n", nome);
                return;
            }
            printf("Encaminhamentos do paciente %s:\n", nome);
            while (encaminhamentoAtual != NULL) {
                printf("Motivo: %s | Destino: %s | Observacoes: %s\n",
                       encaminhamentoAtual->encaminhamento.motivo,
                       encaminhamentoAtual->encaminhamento.destino,
                       encaminhamentoAtual->encaminhamento.observacoes);
                encaminhamentoAtual = encaminhamentoAtual->proximo;
            }
            return;
        }
        atual = atual->proximo;
    }

    printf("Nenhum prontuario encontrado para o paciente: %s\n", nome);
}

void liberarLista(Lista *lista) {
    No *atual = lista->inicio;
    No *temp;

    while (atual != NULL) {
        No2 *exameAtual = atual->exames;
        while (exameAtual != NULL) {
            No2 *tempExame = exameAtual;
            exameAtual = exameAtual->proximo;
            free(tempExame);
        }
        No3 *encaminhamentoAtual = atual->encaminhamentos;
        while (encaminhamentoAtual != NULL) {
            No3 *tempEncaminhamento = encaminhamentoAtual;
            encaminhamentoAtual = encaminhamentoAtual->proximo;
            free(tempEncaminhamento);
        }
        temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    free(lista);
}

// Função para ler dados do arquivo
void lerProntuarioDoArquivo(Lista *lista) {
    FILE *arquivo = fopen("prontuario.txt", "r");
    if (arquivo == NULL) {
        return; // Se não houver arquivo, não faz nada
    }

    ProntuarioPaciente p;
    while (fscanf(arquivo, "Nome:%99[^\n]\nData de Nascimento:%19[^\n]\nSexo:%19[^\n]\nPeso:%19[^\n]\nAltura:%19[^\n]\nObservacoes:%99[^\n]\n",
                  p.nome, p.datanasc, p.sexo, p.peso, p.altura, p.observacao) == 6) {
        adicionarProntuario(lista, p);
    }
    fclose(arquivo);
}

// Função para capturar dados do paciente
ProntuarioPaciente capturarDadosProntuario() {
    ProntuarioPaciente p;

    printf("\nNome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = 0; // Remove a nova linha

    printf("Data de Nascimento (DD-MM-AAAA): ");
    fgets(p.datanasc, sizeof(p.datanasc), stdin);
    p.datanasc[strcspn(p.datanasc, "\n")] = 0;

    printf("Sexo: ");
    fgets(p.sexo, sizeof(p.sexo), stdin);
    p.sexo[strcspn(p.sexo, "\n")] = 0;

    printf("Peso: ");
    fgets(p.peso, sizeof(p.peso), stdin);
    p.peso[strcspn(p.peso, "\n")] = 0;

    printf("Altura: ");
    fgets(p.altura, sizeof(p.altura), stdin);
    p.altura[strcspn(p.altura, "\n")] = 0;

    printf("Observacoes: ");
    fgets(p.observacao, sizeof(p.observacao), stdin);
    p.observacao[strcspn(p.observacao, "\n")] = 0;

    return p;
}

// Função para capturar dados do exame
SolicitarExame capturarDadosExame(Lista *lista, char *nome) {
    SolicitarExame e;
    No *atual = lista->inicio;

    // Verifica se o paciente existe
    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            printf("\nCarater da requisicao (Eletiva, urgencia ou emergencia): ");
            fgets(e.caraterRequisicao, sizeof(e.caraterRequisicao), stdin);
            e.caraterRequisicao[strcspn(e.caraterRequisicao, "\n")] = 0; // Remove a nova linha

            printf("Suspeita clinica: ");
            fgets(e.suspeitaClinica, sizeof(e.suspeitaClinica), stdin);
            e.suspeitaClinica[strcspn(e.suspeitaClinica, "\n")] = 0;

            printf("Tipo de exame: ");
            fgets(e.tipoExame, sizeof(e.tipoExame), stdin);
            e.tipoExame[strcspn(e.tipoExame, "\n")] = 0;

            printf("Observacoes: ");
            fgets(e.Observacoes, sizeof(e.Observacoes), stdin);
            e.Observacoes[strcspn(e.Observacoes, "\n")] = 0;

            return e; // Retorna os dados do exame
        }
        atual = atual->proximo;
    }

    printf("Nenhum paciente com esse nome encontrado: %s\n", nome);
    SolicitarExame vazio; // Retorna um exame vazio se não encontrar
    return vazio;
}

// Função para capturar dados do encaminhamento
Encaminhamento capturarDadosEncaminhamento(Lista *lista, char *nome) {
    Encaminhamento e;
    No *atual = lista->inicio;

    // Verifica se o paciente existe
    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            printf("\nMotivo do encaminhamento: ");
            fgets(e.motivo, sizeof(e.motivo), stdin);
            e.motivo[strcspn(e.motivo, "\n")] = 0; // Remove a nova linha

            printf("Destino: ");
            fgets(e.destino, sizeof(e.destino), stdin);
            e.destino[strcspn(e.destino, "\n")] = 0;

            printf("Observacoes: ");
            fgets(e.observacoes, sizeof(e.observacoes), stdin);
            e.observacoes[strcspn(e.observacoes, "\n")] = 0;

            return e; // Retorna os dados do encaminhamento
        }
        atual = atual->proximo;
    }

    printf("Nenhum paciente com esse nome encontrado: %s\n", nome);
    Encaminhamento vazio; // Retorna um encaminhamento vazio se não encontrar
    return vazio;
}

char* str_repeat(const char *s, int n) {
    static char buffer[256];
    buffer[0] = '\0';
    for (int i = 0; i < n; i++) {
        strcat(buffer, s);
    }
    return buffer;
}

// Função para exibir o cabeçalho
void exibirCabecalho(const char *titulo) {
    int largura = 50;
    printf("\n");
    printf("%s\n", str_repeat("-", largura));
    printf("%-*s\n", largura - 2, titulo);
    printf("%s\n", str_repeat("-", largura));
}

// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese");
    Lista *lista = criarLista();
    lerProntuarioDoArquivo(lista); // Lê pacientes do arquivo ao iniciar

    int opcao, opcao2;

    do {
    	exibirCabecalho("\nMenu:\n");
        printf("1. Prontuario Medico Eletronico\n");
        printf("2. Exames\n");
        printf("3. Encaminhamentos\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1: {
            	exibirCabecalho("Prontuario Medico:\n");
                printf("1. Cadastrar Prontuario de Pacientes\n");
                printf("2. Visualizar Prontuario de Pacientes\n");
                printf("3. Voltar\n");
                printf("Escolha uma opcao: \n");
                scanf("%d", &opcao2);
                getchar(); // Limpa o buffer

                switch (opcao2) {
                    case 1: {
                        ProntuarioPaciente p = capturarDadosProntuario();
                        adicionarProntuario(lista, p);
                        printf("Prontuario do paciente cadastrado com sucesso.\n");
                        salvarProntuarioEmArquivo(lista); // Salva após cada cadastro
                        break;
                    }
                    case 2: {
                        char nome[100];
                        printf("Digite o nome do paciente para visualizar o prontuario: ");
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
                        listarProntuarios(lista, nome);
                        break;
                    }
                    case 3: {
                        // Voltar ao menu principal
                        break;
                    }
                    default: {
                        printf("Opcao invalida. Tente novamente.\n");
                    }
                }
                break; // Para sair do case 1
            }
            case 2: {
            	exibirCabecalho("Exame:\n");
                printf("1. Solicitar Exame:\n");
                printf("2. Visualizar Solicitacao de Exame:\n");
                printf("3. Voltar:\n");
                printf("Escolha uma opcao: \n");
                scanf("%d", &opcao2);
                getchar(); // Limpa o buffer

                switch (opcao2) {
                    case 1: {
                        char nome[100];
                        printf("Digite o nome do paciente para solicitar o exame: ");
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
                        SolicitarExame e = capturarDadosExame(lista, nome);
                        if (strlen(e.tipoExame) > 0) { // Verifica se o exame foi capturado corretamente
                            adicionarExame(lista, e, nome);
                        }
                        break;
                    }
                    case 2: {
                        char nome[100];
                        printf("Digite o nome do paciente para visualizar os exames solicitados: ");
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
                        listarExames(lista, nome);
                        break;
                    }
                    case 3: {
                        break;
                    }
                    default: {
                        printf("Opcso invalida. Tente novamente.\n");
                    }
                }
                break;
            }
           case 3: {
    exibirCabecalho("Encaminhamentos:\n");
    printf("1. Solicitar Encaminhamento:\n");
    printf("2. Visualizar Encaminhamento:\n");
    printf("3. Voltar:\n");
    printf("Escolha uma opcao: \n");
    scanf("%d", &opcao2);
    getchar(); // Limpa o buffer

    switch (opcao2) {
        case 1: {
            char nome[100];
            printf("Digite o nome do paciente para solicitar o encaminhamento: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
            Encaminhamento e = capturarDadosEncaminhamento(lista, nome);
            if (strlen(e.motivo) > 0) { // Verifica se o encaminhamento foi capturado corretamente
                adicionarEncaminhamento(lista, e, nome);
            }
            break;
        }
        case 2: {
            char nome[100];
            printf("Digite o nome do paciente para visualizar os encaminhamentos: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
            listarEncaminhamentos(lista, nome);
            break;
        }
        case 3: {
            break;
        }
        default: {
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    }
    
}
case 4:
                printf("Saindo...\n");
                salvarProntuarioEmArquivo(lista); // Salva ao sair
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 4);

    liberarLista(lista);
    return 0;
}