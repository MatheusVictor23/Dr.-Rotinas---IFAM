// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <locale.h>

// // Definição do TAD de lista dinâmica encadeada
// typedef struct {
//     char nome[100];
//     char datanasc[20];
//     char sexo[20];
//     char peso[10];
//     char altura[10];
//     char observacao[100];
// } ProntuarioPaciente;

// typedef struct {
//     char caraterRequisicao[20];
//     char suspeitaClinica[100];
//     char tipoExame[30];
//     char Observacoes[80];
// } SolicitarExame;

// typedef struct No {
//     ProntuarioPaciente prontuario;
//     struct No *proximo;
//     struct No2 *exames; // Adicionando ponteiro para lista de exames
// } No;

// typedef struct No2 {
//     SolicitarExame exame;
//     struct No2 *proximo;
// } No2;

// typedef struct {
//     No *inicio;
// } Lista;

// // Funções para manipulação da lista
// Lista* criarLista() {
//     Lista *lista = (Lista*)malloc(sizeof(Lista));
//     lista->inicio = NULL;
//     return lista;
// }

// void adicionarProntuario(Lista *lista, ProntuarioPaciente prontuario) {
//     No *novoNo = (No*)malloc(sizeof(No));
//     novoNo->prontuario = prontuario;
//     novoNo->proximo = lista->inicio;
//     novoNo->exames = NULL; // Inicializa a lista de exames como vazia
//     lista->inicio = novoNo;
// }

// void adicionarExame(Lista *lista, SolicitarExame exame, char *nome) {
//     No *atual = lista->inicio;
//     int encontrado = 0;

//     // Primeiro, verifica se o paciente existe
//     while (atual != NULL) {
//         if (strcmp(atual->prontuario.nome, nome) == 0) {
//             // Se o paciente foi encontrado, adiciona o exame
//             No2 *novoNo = (No2*)malloc(sizeof(No2));
//             novoNo->exame = exame;
//             novoNo->proximo = atual->exames; // Adiciona o exame ao início da lista de exames
//             atual->exames = novoNo; // Atualiza a lista de exames do paciente
//             printf("Solicitação de exame feita com sucesso para o paciente: %s\n", nome);
//             salvarProntuarioEmArquivo(lista); // Salva os exames
//             encontrado = 1; // Paciente encontrado
//             break; // Saindo do laço
//         }
//         atual = atual->proximo;
//     }

// }

// void listarProntuarios(Lista *lista, char *nome) {
//     No *atual = lista->inicio;
//     int encontrado = 0;

//     if (atual == NULL) {
//         printf("Nenhum prontuário cadastrado.\n");
//         return;
//     }

//     printf("Lista de Prontuários Médicos:\n");
//     while (atual != NULL) {
//         if (strcmp(atual->prontuario.nome, nome) == 0) {
//             printf("Nome: %s | Data de Nascimento: %s | Sexo: %s | Peso: %s | Altura: %s | Observações: %s\n",
//                    atual->prontuario.nome,
//                    atual->prontuario.datanasc,
//                    atual->prontuario.sexo,
//                    atual->prontuario.peso,
//                    atual->prontuario.altura,
//                    atual->prontuario.observacao);
//             encontrado = 1;
//             break; // Encontrado e exibido
//         }
//         atual = atual->proximo;
//     }

//     if (!encontrado) {
//         printf("Nenhum prontuário encontrado para o paciente: %s\n", nome);
//     }
// }

// void listarExames(Lista *lista, char *nome) {
//     No *atual = lista->inicio;
//     int encontrado = 0;

//     while (atual != NULL) {
//         if (strcmp(atual->prontuario.nome, nome) == 0) {
//             No2 *exameAtual = atual->exames;
//             if (exameAtual == NULL) {
//                 printf("Nenhum exame encontrado para o paciente: %s\n", nome);
//                 return;
//             }
//             printf("Exames do paciente %s:\n", nome);
//             while (exameAtual != NULL) {
//                 printf("Tipo de Exame: %s | Caráter: %s | Suspeita Clínica: %s | Observações: %s\n",
//                        exameAtual->exame.tipoExame,
//                        exameAtual->exame.caraterRequisicao,
//                        exameAtual->exame.suspeitaClinica,
//                        exameAtual->exame.Observacoes);
//                 exameAtual = exameAtual->proximo;
//             }
//             encontrado = 1;
//             break;
//         }
//         atual = atual->proximo;
//     }

//     if (!encontrado) {
//         printf("Nenhum prontuário encontrado para o paciente: %s\n", nome);
//     }
// }

// // Função para salvar dados no arquivo
// void salvarProntuarioEmArquivo(Lista *lista) {
//     FILE *arquivo = fopen("prontuario.txt", "w");
//     if (arquivo == NULL) {
//         printf("Erro ao abrir o arquivo para gravação.\n");
//         return;
//     }

//     No *atual = lista->inicio;
//     while (atual != NULL) {
//         fprintf(arquivo, "\nNome:%s\nData de Nascimento:%s\nSexo:%s\nPeso:%s\nAltura:%s\nObservações:%s\n",
//                 atual->prontuario.nome,
//                 atual->prontuario.datanasc,
//                 atual->prontuario.sexo,
//                 atual->prontuario.peso,
//                 atual->prontuario.altura,
//                 atual->prontuario.observacao);

//         // Salva também os exames desse paciente
//         No2 *exameAtual = atual->exames;
//         while (exameAtual != NULL) {
//             fprintf(arquivo, "\nExame:%s\nCaráter:%s\nSuspeita Clínica:%s\nObservações:%s\n",
//                     exameAtual->exame.tipoExame,
//                     exameAtual->exame.caraterRequisicao,
//                     exameAtual->exame.suspeitaClinica,
//                     exameAtual->exame.Observacoes);
//             exameAtual = exameAtual->proximo;
//         }
//         atual = atual->proximo;
//     }
//     fclose(arquivo);
//     printf("Dados salvos com sucesso.\n");
// }

// void liberarLista(Lista *lista) {
//     No *atual = lista->inicio;
//     No *temp;

//     while (atual != NULL) {
//         No2 *exameAtual = atual->exames;
//         while (exameAtual != NULL) {
//             No2 *tempExame = exameAtual;
//             exameAtual = exameAtual->proximo;
//             free(tempExame);
//         }
//         temp = atual;
//         atual = atual->proximo;
//         free(temp);
//     }
//     free(lista);
// }

// // Função para ler dados do arquivo
// void lerProntuarioDoArquivo(Lista *lista) {
//     FILE *arquivo = fopen("prontuario.txt", "r");
//     if (arquivo == NULL) {
//         return; // Se não houver arquivo, não faz nada
//     }

//     ProntuarioPaciente p;
//     while (fscanf(arquivo, "Nome:%99[^\n]\nData de Nascimento:%19[^\n]\nSexo:%19[^\n]\nPeso:%19[^\n]\nAltura:%19[^\n]\nObservações:%99[^\n]\n",
//                   p.nome, p.datanasc, p.sexo, p.peso, p.altura, p.observacao) == 6) {
//         adicionarProntuario(lista, p);
//     }
//     fclose(arquivo);
// }

// // Função para capturar dados do paciente
// ProntuarioPaciente capturarDadosProntuario() {
//     ProntuarioPaciente p;

//     printf("\nNome: ");
//     fgets(p.nome, sizeof(p.nome), stdin);
//     p.nome[strcspn(p.nome, "\n")] = 0; // Remove a nova linha

//     printf("Data de Nascimento (DD-MM-AAAA): ");
//     fgets(p.datanasc, sizeof(p.datanasc), stdin);
//     p.datanasc[strcspn(p.datanasc, "\n")] = 0;

//     printf("Sexo: ");
//     fgets(p.sexo, sizeof(p.sexo), stdin);
//     p.sexo[strcspn(p.sexo, "\n")] = 0;

//     printf("Peso: ");
//     fgets(p.peso, sizeof(p.peso), stdin);
//     p.peso[strcspn(p.peso, "\n")] = 0;

//     printf("Altura: ");
//     fgets(p.altura, sizeof(p.altura), stdin);
//     p.altura[strcspn(p.altura, "\n")] = 0;

//     printf("Observação: ");
//     fgets(p.observacao, sizeof(p.observacao), stdin);
//     p.observacao[strcspn(p.observacao, "\n")] = 0;

//     return p;
// }

// // Função para capturar dados do exame
// SolicitarExame capturarDadosExame(Lista *lista, char *nome) {
//     SolicitarExame e;
//     No *atual = lista->inicio;
//     int encontrado = 0;

//     // Primeiro, verifica se o paciente existe
//     while (atual != NULL) {
//         if (strcmp(atual->prontuario.nome, nome) == 0) {
//             encontrado = 1; // Paciente encontrado

//             printf("\nCaráter da requisição (Eletiva, urgência ou emergência): ");
//             fgets(e.caraterRequisicao, sizeof(e.caraterRequisicao), stdin);
//             e.caraterRequisicao[strcspn(e.caraterRequisicao, "\n")] = 0; // Remove a nova linha

//             printf("Suspeita clínica: ");
//             fgets(e.suspeitaClinica, sizeof(e.suspeitaClinica), stdin);
//             e.suspeitaClinica[strcspn(e.suspeitaClinica, "\n")] = 0;

//             printf("Tipo de exame: ");
//             fgets(e.tipoExame, sizeof(e.tipoExame), stdin);
//             e.tipoExame[strcspn(e.tipoExame, "\n")] = 0;

//             printf("Observações: ");
//             fgets(e.Observacoes, sizeof(e.Observacoes), stdin);
//             e.Observacoes[strcspn(e.Observacoes, "\n")] = 0;

//             return e; // Retorna os dados do exame
//         }
//         atual = atual->proximo;
//     }

//     if (!encontrado) {
//         printf("Nenhum paciente com esse nome encontrado: %s\n", nome);
//         SolicitarExame vazio; // Retorna um exame vazio se não encontrar
//         return vazio;
//     }
// }

// // Função principal
// int main() {
//     setlocale(LC_ALL, "Portuguese");
//     Lista *lista = criarLista();
//     lerProntuarioDoArquivo(lista); // Lê pacientes do arquivo ao iniciar

//     int opcao, opcao2;

//     do {
//         printf("\nMenu:\n");
//         printf("1. Prontuário Médico Eletrônico\n");
//         printf("2. Exames\n");
//         printf("3. Encaminhamentos\n");
//         printf("4. Sair\n");
//         printf("Escolha uma opção: ");
//         scanf("%d", &opcao);
//         getchar(); // Limpa o buffer

//         switch (opcao) {
//             case 1: {
//                 printf("\nProntuário Médico:\n");
//                 printf("1. Cadastrar Prontuário de Pacientes\n");
//                 printf("2. Visualizar Prontuário de Pacientes\n");
//                 printf("3. Voltar\n");
//                 printf("Escolha uma opção: \n");
//                 scanf("%d", &opcao2);
//                 getchar(); // Limpa o buffer

//                 switch (opcao2) {
//                     case 1: {
//                         ProntuarioPaciente p = capturarDadosProntuario();
//                         adicionarProntuario(lista, p);
//                         printf("Prontuário do paciente cadastrado com sucesso.\n");
//                         salvarProntuarioEmArquivo(lista); // Salva após cada cadastro
//                         break;
//                     }
//                     case 2: {
//                         char nome[100];
//                         printf("Digite o nome do paciente para visualizar o prontuário: ");
//                         fgets(nome, sizeof(nome), stdin);
//                         nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
//                         listarProntuarios(lista, nome);
//                         break;
//                     }
//                     case 3: {
//                         // Voltar ao menu principal
//                         break;
//                     }
//                     default: {
//                         printf("Opção inválida. Tente novamente.\n");
//                     }
//                 }
//                 break; // Para sair do case 1
//             }
//             case 2: {
//                 printf("\nExame:\n");
//                 printf("1. Solicitar Exame:\n");
//                 printf("2. Visualizar Solicitação de Exame:\n");
//                 printf("3. Voltar:\n");
//                 printf("Escolha uma opção: \n");
//                 scanf("%d", &opcao2);
//                 getchar(); // Limpa o buffer

//                 switch (opcao2) {
//                     case 1: {
//                         char nome[100];
//                         printf("Digite o nome do paciente para solicitar o exame: ");
//                         fgets(nome, sizeof(nome), stdin);
//                         nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
//                         SolicitarExame e = capturarDadosExame(lista, nome);
//                         if (strlen(e.tipoExame) > 0) { // Verifica se o exame foi capturado corretamente
//                             adicionarExame(lista, e, nome);
//                         }
//                         break;
//                     }
//                     case 2: {
//                         char nome[100];
//                         printf("Digite o nome do paciente para visualizar os exames solicitados: ");
//                         fgets(nome, sizeof(nome), stdin);
//                         nome[strcspn(nome, "\n")] = 0; // Remove a nova linha
//                         listarExames(lista, nome);
//                         break;
//                     }
//                     case 3: {
//                         break;
//                     }
//                     default: {
//                         printf("Opção inválida. Tente novamente.\n");
//                     }
//                 }
//                 break;
//             }
//             case 3:
//                 // Implementação para Encaminhamentos
//                 break;
//             case 4:
//                 printf("Saindo...\n");
//                 salvarProntuarioEmArquivo(lista); // Salva ao sair
//                 break;
//             default:
//                 printf("Opção inválida. Tente novamente.\n");
//         }
//     } while (opcao != 4);

//     liberarLista(lista);
//     return 0;
// }