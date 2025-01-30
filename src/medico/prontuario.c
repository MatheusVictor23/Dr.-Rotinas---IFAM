#include "../../include/medico/Prontuario.h"

// Definição de cores para o terminal
const static int YELLOW = 14;
const static int RESET = 7;

void static setColor(int textColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, textColor);
}

void static imprimirMenu(const char *opcoes[], const char *opcoesDestacadas[], int num_opcoes, int destaque, int espaco_max) {
    for (int i = 0; i < num_opcoes; i++) {
        if (i == destaque) {
            printf("\t\t");
            setColor(YELLOW);
            printf("       %-*s  ", espaco_max, opcoesDestacadas[i]); // Imprime opção destacada
            setColor(RESET);
            printf("\n");
        } else {
            setColor(RESET);
            printf("\t\t|       %-*s      |\n", espaco_max, opcoes[i]); // Imprime opção normal
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

Lista* criarListaMedico() {
    Lista *lista = (Lista*)malloc(sizeof(Lista));
    lista->inicio = NULL;
    return lista;
}

ProntuarioPaciente capturarDadosProntuario() {
    ProntuarioPaciente p;
    printf("\nNome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = 0;

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

Encaminhamento capturarDadosEncaminhamento(Lista *lista, char *nome) {
    Encaminhamento e;
    NoProntuario *atual = lista->inicio;

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

void listarExames(Lista *lista, char *nome) {
    NoProntuario *atual = lista->inicio;
    int encontrado = 0;

    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            NoProntuario2 *exameAtual = atual->exames;
            if (exameAtual == NULL) {
                printf("Nenhum exame encontrado para o paciente: %s\n", nome);
                return;
            }
            printf("Exames do paciente %s:\n", nome);
            while (exameAtual != NULL) {
                printf("Tipo de Exame: %s\nCaráter: %s\nSuspeita Clínica: %s\nObservacoes: %s\n",
                       exameAtual->exame.tipoExame,
                       exameAtual->exame.caraterRequisicao,
                       exameAtual->exame.suspeitaClinica,
                       exameAtual->exame.Observacoes);
                exameAtual = exameAtual->proximo;
            }
            encontrado = 1;
            break;
        }
        atual = atual->proximo;
    }

    if (!encontrado) {
        printf("Nenhum exame encontrado para o paciente: %s\n", nome);
    }
}

void adicionarProntuario(Lista *lista, ProntuarioPaciente prontuario) {
    NoProntuario *novoNo = (NoProntuario*)malloc(sizeof(NoProntuario));
    novoNo->prontuario = prontuario;
    novoNo->proximo = lista->inicio;
    novoNo->exames = NULL; // Inicializa a lista de exames como vazia
    novoNo->encaminhamentos = NULL; // Inicializa a lista de encaminhamentos como vazia
    lista->inicio = novoNo;
}

void listarProntuarios(Lista *lista, char *nome) {
    NoProntuario *atual = lista->inicio;
    int encontrado = 0;

    if (atual == NULL) {
        printf("Nenhum prontuario cadastrado.\n");
        return;
    }

    printf("Lista de Prontuarios Medicos:\n");
    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            printf("Nome: %s\nData de Nascimento: %s\nSexo: %s\nPeso: %s\nAltura: %s\nObservacoes: %s\n",
                   atual->prontuario.nome,
                   atual->prontuario.datanasc,
                   atual->prontuario.sexo,
                   atual->prontuario.peso,
                   atual->prontuario.altura,
                   atual->prontuario.observacao);
            encontrado = 1;
            break; // Encontrado e exibido
        }
        atual = atual->proximo;
    }

    if (!encontrado) {
        printf("Nenhum prontuario encontrado para o paciente: %s\n", nome);
    }
}

void salvarProntuarioEmArquivo(Lista *lista) {
    FILE *arquivo = fopen("prontuario.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para gravação.\n");
        return;
    }

    NoProntuario *atual = lista->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "\nNome:%s\nData de Nascimento:%s\nSexo:%s\nPeso:%s\nAltura:%s\nObservacoes:%s\n",
                atual->prontuario.nome,
                atual->prontuario.datanasc,
                atual->prontuario.sexo,
                atual->prontuario.peso,
                atual->prontuario.altura,
                atual->prontuario.observacao);

        // Salva os exames desse paciente
        NoProntuario2 *exameAtual = atual->exames;
        while (exameAtual != NULL) {
            fprintf(arquivo, "\nExame:%s\nCarater:%s\nSuspeita Clinica:%s\nObservacoes:%s\n",
                    exameAtual->exame.tipoExame,
                    exameAtual->exame.caraterRequisicao,
                    exameAtual->exame.suspeitaClinica,
                    exameAtual->exame.Observacoes);
            exameAtual = exameAtual->proximo;
        }

        // Salva os encaminhamentos desse paciente
        NoProntuario3 *encaminhamentoAtual = atual->encaminhamentos;
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

void lerProntuarioDoArquivo(Lista *lista) {
    FILE *arquivo = fopen("../../data/prontuario.txt", "r");
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

SolicitarExame capturarDadosExame(Lista *lista, char *nome) {
    SolicitarExame e;
    NoProntuario *atual = lista->inicio;
    int encontrado = 0;

    // Verifica se o paciente existe
    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            encontrado = 1; // Paciente encontrado

            printf("\nCarater da requisicao (Eletiva, urgencia ou emergencia): ");
            fgets(e.caraterRequisicao, sizeof(e.caraterRequisicao), stdin);
            e.caraterRequisicao[strcspn(e.caraterRequisicao, "\n")] = 0; // Remove a nova linha

            printf("Suspeita clinica: ");
            fgets(e.suspeitaClinica, sizeof(e.suspeitaClinica), stdin);
            e.suspeitaClinica[strcspn(e.suspeitaClinica, "\n")] = 0;

            printf("Tipo de Exame: ");
            fgets(e.tipoExame, sizeof(e.tipoExame), stdin);
            e.tipoExame[strcspn(e.tipoExame, "\n")] = 0;

            printf("Observacoes: ");
            fgets(e.Observacoes, sizeof(e.Observacoes), stdin);
            e.Observacoes[strcspn(e.Observacoes, "\n")] = 0;

            return e; // Retorna os dados do exame
        }
        atual = atual->proximo;
    }

    if (!encontrado) {
        printf("Nenhum paciente com esse nome encontrado: %s\n", nome);
        SolicitarExame vazio; // Retorna um exame vazio se não encontrar
        return vazio;
    }
}

void adicionarExame(Lista *lista, SolicitarExame exame, char *nome) {
    NoProntuario *atual = lista->inicio;
    int encontrado = 0;

    // Verifica se o paciente existe
    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            // Se o paciente foi encontrado, adiciona o exame
            NoProntuario2 *novoNo = (NoProntuario2*)malloc(sizeof(NoProntuario2));
            novoNo->exame = exame;
            novoNo->proximo = atual->exames; // Adiciona o exame ao início da lista de exames
            atual->exames = novoNo; // Atualiza a lista de exames do paciente
            printf("Solicitacao de exame feita com sucesso para o paciente: %s\n", nome);
            salvarProntuarioEmArquivo(lista); // Salva os exames
            encontrado = 1; // Paciente encontrado
            break; // Saindo do laço
        }
        atual = atual->proximo;
    }
    if (!encontrado) {
        printf("Paciente nao encontrado para adicionar exame.\n");
    }
}

void adicionarEncaminhamento(Lista *lista, Encaminhamento encaminhamento, char *nome) {
    NoProntuario *atual = lista->inicio;

    // Verifica se o paciente existe
    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            NoProntuario3 *novoNo = (NoProntuario3*)malloc(sizeof(NoProntuario3));
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

void listarEncaminhamentos(Lista *lista, char *nome) {
    NoProntuario *atual = lista->inicio;

    while (atual != NULL) {
        if (strcmp(atual->prontuario.nome, nome) == 0) {
            NoProntuario3 *encaminhamentoAtual = atual->encaminhamentos;
            if (encaminhamentoAtual == NULL) {
                printf("Nenhum encaminhamento encontrado para o paciente: %s\n", nome);
                return;
            }
            printf("Encaminhamentos do paciente %s:\n", nome);
            while (encaminhamentoAtual != NULL) {
                printf("Motivo: %s\nDestino: %s\nObservacoes: %s\n",
                       encaminhamentoAtual->encaminhamento.motivo,
                       encaminhamentoAtual->encaminhamento.destino,
                       encaminhamentoAtual->encaminhamento.observacoes);
                encaminhamentoAtual = encaminhamentoAtual->proximo;
            }
            return;
        }
        atual = atual->proximo;
    }
    printf("Nenhum encaminhamento encontrado para o paciente: %s\n", nome);
}

void liberarListaMedico(Lista *lista) {
    NoProntuario *atual = lista->inicio;
    while (atual != NULL) {
        NoProntuario *temp = atual;
        atual = atual->proximo;

        // Liberar lista de exames
        NoProntuario2 *exameAtual = temp->exames;
        while (exameAtual != NULL) {
            NoProntuario2 *tempExame = exameAtual;
            exameAtual = exameAtual->proximo;
            free(tempExame);
        }

        // Liberar lista de encaminhamentos
        NoProntuario3 *encaminhamentoAtual = temp->encaminhamentos;
        while (encaminhamentoAtual != NULL) {
            NoProntuario3 *tempEncaminhamento = encaminhamentoAtual;
            encaminhamentoAtual = encaminhamentoAtual->proximo;
            free(tempEncaminhamento);
        }
        free(temp);
    }
    free(lista);
}

void menuProntuario(Lista *lista) {
    int destaque = 0;
    int escolha = -1;
    const char *opcoes[] = { 
        "Cadastrar Prontuario", 
        "Visualizar Prontuario", 
        "Cadastrar Exame",
        "Visualizar Exame",
        "Cadastrar Encaminhamento",
        "Visualizar Encaminhamento",
        "Voltar ao Menu Principal" 
    };
    const char *opcoesDestacadas[] = { 
        "> Cadastrar Prontuario <", 
        "> Visualizar Prontuario <", 
        "> Cadastrar Exame <",
        "> Visualizar Exame <",
        "> Cadastrar Encaminhamento <",
        "> Visualizar Encaminhamento <",
        "> Voltar ao Menu Principal <" 
    };
    int num_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);

    do {
        system("cls || clear");
        puts("\t\t+-------------------------------------------+");
        puts("\t\t|     BEM VINDO(A) AO MENU                  |");
        puts("\t\t|    DE PRONTUARIO MEDICO                   |");
        puts("\t\t+-------------------------------------------+");
        puts("\t\t|       Deseja Gerenciar:                   |");
        imprimirMenu(opcoes, opcoesDestacadas, num_opcoes, destaque, 30);
        puts("\t\t+--------------------------------------------+");

        escolha = controlaEntrada(&destaque, num_opcoes); 
        switch (escolha) {
            case 0: { // Cadastrar prontuário
                printf("\nCadastro de paciente:\n");
                ProntuarioPaciente p = capturarDadosProntuario();
                adicionarProntuario(lista, p);
                printf("Prontuario cadastrado com sucesso.\n");
                salvarProntuarioEmArquivo(lista); // Salva após cada cadastro
                system("pause");
                break;
            }
            case 1: { // Visualizar prontuário
                char nome[100];
                printf("Digite o nome do paciente para visualizar o prontuario: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
                listarProntuarios(lista, nome);
                system("pause");
                break;
            }
            case 2: { // Cadastrar exame
                char nome[100];
                printf("Digite o nome do paciente para solicitar o exame: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
                SolicitarExame e = capturarDadosExame(lista, nome);
                adicionarExame(lista, e, nome);
                system("pause");
                break;
            }
            case 3: { // Visualizar exames
                char nome[100];
                printf("Digite o nome do paciente para visualizar os exames: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
                listarExames(lista, nome);
                system("pause");
                break;
            }
            case 4: { // Cadastrar encaminhamento
                char nome[100];
                printf("Digite o nome do paciente para solicitar o encaminhamento: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
                Encaminhamento e = capturarDadosEncaminhamento(lista, nome);
                adicionarEncaminhamento(lista, e, nome);
                system("pause");
                break;
            }
            case 5: { // Visualizar encaminhamentos
                char nome[100];
                printf("Digite o nome do paciente para visualizar os encaminhamentos: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
                listarEncaminhamentos(lista, nome);
                system("pause");
                break;
            }
            case 6: { // Voltar ao menu principal
                break;
            }
            default:
                break;
        }
    } while (escolha != 6);
}

void menuMedico() {
    setlocale(LC_ALL, "Portuguese");
    Lista *lista = criarListaMedico();
    lerProntuarioDoArquivo(lista);

    int destaque = 0;
    int opcao = -1;

    const char *opcoes[] = {
        "Prontuario Medico",
        "Sair"
    };

    const char *opcoesDestacadas[] = {
        "> Prontuario Medico <",
        "> Sair <"
    };

    int num_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);

    do {
        system("cls || clear");
       
        puts("\t\t+---------------------------------------+");
        puts("\t\t|      MENU PRINCIPAL                   |");
        puts("\t\t+---------------------------------------+");
        imprimirMenu(opcoes, opcoesDestacadas, num_opcoes, destaque, 26);
        puts("\t\t+---------------------------------------+");

        opcao = controlaEntrada(&destaque, num_opcoes);

        switch (opcao) {
            case 0:
                menuProntuario(lista); // Chama o menu de prontuário
                break;
            case 1:
                puts("Saindo...\n");
                break;
        }
        
    } while (opcao != 1);

    liberarListaMedico(lista); // Libera a memória alocada
}