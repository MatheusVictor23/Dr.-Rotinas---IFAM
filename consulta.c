//consulta.c

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include "consulta.h"
#include "login.h"
#include "admin.h"
#include "prontuario.h"

//Variáveis globais/controle
int ultimo_id_paciente = 0;
const char *arquivo_pacientes = "pacientes.txt";
int ultimo_id_consulta = 0;
const char *arquivo_consultas = "consultas.txt";
int i;

//Criação e manipulação de lista
LISTA *criarLista() {
    LISTA *lista = malloc(sizeof(LISTA));
    if (lista != NULL) {
        lista->inicio = NULL;
        lista->tamanho = 0;
    }
    return lista;
}

static NO *criarNo(void *dado, size_t tamanhoDado) {
    NO *novoNo = malloc(sizeof(NO));
    if (novoNo != NULL) {
        novoNo->dado = malloc(tamanhoDado);
        if (novoNo->dado != NULL) {
            memcpy(novoNo->dado, dado, tamanhoDado);
            novoNo->proximo = NULL;
        } else {
            free(novoNo);
            return NULL;
        }
    }
    return novoNo;
}

bool inserirNoInicio(LISTA *lista, void *dado, size_t tamanhoDado) {
    NO *novoNo = criarNo(dado, tamanhoDado);
    if (novoNo == NULL) return false;

    novoNo->proximo = lista->inicio;
    lista->inicio = novoNo;
    lista->tamanho++;
    return true;
}

bool inserirNoFim(LISTA *lista, void *dado, size_t tamanhoDado) {
    NO *novoNo = criarNo(dado, tamanhoDado);
    if (novoNo == NULL) return false;

    if (lista->inicio == NULL) {
        lista->inicio = novoNo;
    } else {
        NO *atual = lista->inicio;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novoNo;
    }
    lista->tamanho++;
    return true;
}

void *removerNoInicio(LISTA *lista) {
    if (lista->inicio == NULL) return NULL;

    NO *temp = lista->inicio;
    void *dado = temp->dado;
    lista->inicio = temp->proximo;
    free(temp);
    lista->tamanho--;
    return dado;
}

void *removerNoFim(LISTA *lista) {
    if (lista->inicio == NULL) return NULL;

    NO *atual = lista->inicio;
    NO *anterior = NULL;

    while (atual->proximo != NULL) {
        anterior = atual;
        atual = atual->proximo;
    }

    void *dado = atual->dado;
    if (anterior == NULL) {
        lista->inicio = NULL;
    } else {
        anterior->proximo = NULL;
    }
    free(atual);
    lista->tamanho--;
    return dado;
}

void *buscarElemento(LISTA *lista, bool (*comparator)(void *, void *), void *chave) {
    NO *atual = lista->inicio;
    while (atual != NULL) {
        if (comparator(atual->dado, chave)) {
            return atual->dado;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void listarElementos(LISTA *lista, void (*imprimir)(void *)) {
    NO *atual = lista->inicio;
    while (atual != NULL) {
        imprimir(atual->dado);
        atual = atual->proximo;
    }
}

int tamanhoLista(LISTA *lista) {
    return lista->tamanho;
}

void destruirLista(LISTA *lista, void (*liberarDado)(void *)) {
    NO *atual = lista->inicio;
    while (atual != NULL) {
        NO *temp = atual;
        atual = atual->proximo;
        liberarDado(temp->dado);
        free(temp);
    }
    free(lista);
}


// Funções para Menu de Paciente/Paciente em geral
void carregarPacientes(LISTA *lista) {
    FILE *arquivo = fopen(arquivo_pacientes, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        
        return;
    }

    PACIENTE novoPaciente;
    while (fscanf(arquivo, "%d;%99[^;];%19[^;];%19[^;];%19[^;];%19[^;];%19[^;];%99[^;];%199[^;];%19[^;];%49[^;];%f;%f;%99[^;];%99[^;];%19[^;];%19[^;];%19[^;];%299[^\n]\n",
                  &novoPaciente.id_paciente,
                  novoPaciente.nome_paciente,
                  novoPaciente.telefone_paciente,
                  novoPaciente.datanasc_paciente,
                  novoPaciente.sexo_paciente,
                  novoPaciente.CPF_paciente,
                  novoPaciente.RG_paciente,
                  novoPaciente.email_paciente,
                  novoPaciente.endereco_paciente,
                  novoPaciente.estadocivil_paciente,
                  novoPaciente.profissao_paciente,
                  &novoPaciente.peso_paciente,
                  &novoPaciente.altura_paciente,
                  novoPaciente.planoSaude_paciente,
                  novoPaciente.convenioMedico_paciente,
                  novoPaciente.numeroCarteira_paciente,
                  novoPaciente.validadeCarteira_paciente,
                  novoPaciente.cartaoCNS_paciente,
                  novoPaciente.observacao_paciente) == 18) {
        inserirNoFim(lista, &novoPaciente, sizeof(PACIENTE));
        if (novoPaciente.id_paciente > ultimo_id_paciente) {
            ultimo_id_paciente = novoPaciente.id_paciente;
        }
    }

    fclose(arquivo);
    // printf("Pacientes carregados com sucesso!\n"); // Pra teste
}

void salvarPacientes(LISTA *lista) {
    FILE *arquivo = fopen(arquivo_pacientes, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    NO *atual = lista->inicio;
    while (atual != NULL) {
        PACIENTE *paciente = (PACIENTE *)atual->dado;
        fprintf(arquivo, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%.2f;%.2f;%s;%s;%s;%s;%s;%s\n",
                paciente->id_paciente,
                paciente->nome_paciente,
                paciente->telefone_paciente,
                paciente->datanasc_paciente,
                paciente->sexo_paciente,
                paciente->CPF_paciente,
                paciente->RG_paciente,
                paciente->email_paciente,
                paciente->endereco_paciente,
                paciente->estadocivil_paciente,
                paciente->profissao_paciente,
                paciente->peso_paciente,
                paciente->altura_paciente,
                paciente->planoSaude_paciente,
                paciente->convenioMedico_paciente,
                paciente->numeroCarteira_paciente,
                paciente->validadeCarteira_paciente,
                paciente->cartaoCNS_paciente,
                paciente->observacao_paciente);
        atual = atual->proximo;
    }

    fclose(arquivo);
    // printf("Pacientes salvos com sucesso!\n"); // Pra teste
}

//Versão resumida
void imprimirPaciente(void *dado) {
    PACIENTE *paciente = (PACIENTE *)dado;
    printf("\tNúmero: %d\n", paciente->id_paciente);
    printf("\tNome: %s\n", paciente->nome_paciente);
    printf("\tTelefone: %s\n", paciente->telefone_paciente);
    // printf("Paciente impresso: ID=%d\n", paciente->id_paciente); // Pra teste
}

//Versão completa
void imprimirPaciente2(void *dado) {
	PACIENTE *paciente = (PACIENTE *)dado;
    printf("\tNúmero: %d\n", paciente->id_paciente);
    printf("\tNome: %s\n", paciente->nome_paciente);
    printf("\tTelefone: %s\n", paciente->telefone_paciente);
    printf("\tData de Nascimento: %s\n", paciente->datanasc_paciente);
    printf("\tSexo: %s\n", paciente->sexo_paciente);
    printf("\tCPF: %s\n", paciente->CPF_paciente);
    printf("\tRG: %s\n", paciente->RG_paciente);
    printf("\tEmail: %s\n", paciente->email_paciente);
    printf("\tEndereço: %s\n", paciente->endereco_paciente);
    printf("\tEstado Civil: %s\n", paciente->estadocivil_paciente);
    printf("\tProfissão: %s\n", paciente->profissao_paciente);
    printf("\tPeso: %.2f\n", paciente->peso_paciente);
    printf("\tAltura: %.2f\n", paciente->altura_paciente);
    printf("\tPlano de Saúde: %s\n", paciente->planoSaude_paciente);
    printf("\tConvênio Médico: %s\n", paciente->convenioMedico_paciente);
    printf("\tNúmero da Carteira: %s\n", paciente->numeroCarteira_paciente);
    printf("\tValidade da Carteira: %s\n", paciente->validadeCarteira_paciente);
    printf("\tCartão CNS: %s\n", paciente->cartaoCNS_paciente);
    printf("\tObservação: %s\n", paciente->observacao_paciente);
}

void cadastrarPaciente(LISTA *lista) {
    PACIENTE novoPaciente;
   	ultimo_id_paciente++;
   	novoPaciente.id_paciente = ultimo_id_paciente;
    
    //Formulário de preenchimento do cadastro
    system("cls || clear");
	puts("\t+-------------------------------+");
    puts("\t|                               |");
    puts("\t|      CADASTRO DE PACIENTE     |");
    puts("\t|                               |");
    puts("\t+-------------------------------+");
    printf("\tNúmero do novo paciente: %d\n", novoPaciente.id_paciente);
    printf("\tDigite o nome: ");
    getchar();
    fgets(novoPaciente.nome_paciente, sizeof(novoPaciente.nome_paciente), stdin);
    novoPaciente.nome_paciente[strcspn(novoPaciente.nome_paciente, "\n")] = 0;

    printf("\tDigite o telefone: ");
    fgets(novoPaciente.telefone_paciente, sizeof(novoPaciente.telefone_paciente), stdin);
    novoPaciente.telefone_paciente[strcspn(novoPaciente.telefone_paciente, "\n")] = 0;

    printf("\tDigite a data de nascimento (dd/mm/aaaa): ");
    fgets(novoPaciente.datanasc_paciente, sizeof(novoPaciente.datanasc_paciente), stdin);
    novoPaciente.datanasc_paciente[strcspn(novoPaciente.datanasc_paciente, "\n")] = 0;

    printf("\tDigite o sexo: ");
    fgets(novoPaciente.sexo_paciente, sizeof(novoPaciente.sexo_paciente), stdin);
    novoPaciente.sexo_paciente[strcspn(novoPaciente.sexo_paciente, "\n")] = 0;

    printf("\tDigite o CPF: ");
    fgets(novoPaciente.CPF_paciente, sizeof(novoPaciente.CPF_paciente), stdin);
    novoPaciente.CPF_paciente[strcspn(novoPaciente.CPF_paciente, "\n")] = 0;

    printf("\tDigite o RG: ");
    fgets(novoPaciente.RG_paciente, sizeof(novoPaciente.RG_paciente), stdin);
    novoPaciente.RG_paciente[strcspn(novoPaciente.RG_paciente, "\n")] = 0;

    printf("\tDigite o email: ");
    fgets(novoPaciente.email_paciente, sizeof(novoPaciente.email_paciente), stdin);
    novoPaciente.email_paciente[strcspn(novoPaciente.email_paciente, "\n")] = 0;

    printf("\tDigite o endereço: ");
    fgets(novoPaciente.endereco_paciente, sizeof(novoPaciente.endereco_paciente), stdin);
    novoPaciente.endereco_paciente[strcspn(novoPaciente.endereco_paciente, "\n")] = 0;

    printf("\tDigite o estado civil: ");
    fgets(novoPaciente.estadocivil_paciente, sizeof(novoPaciente.estadocivil_paciente), stdin);
    novoPaciente.estadocivil_paciente[strcspn(novoPaciente.estadocivil_paciente, "\n")] = 0;

    printf("\tDigite a profissão: ");
    fgets(novoPaciente.profissao_paciente, sizeof(novoPaciente.profissao_paciente), stdin);
    novoPaciente.profissao_paciente[strcspn(novoPaciente.profissao_paciente, "\n")] = 0;

    printf("\tDigite o peso (kg): ");
    scanf("%f", &novoPaciente.peso_paciente);

    printf("\tDigite a altura (m): ");
    scanf("%f", &novoPaciente.altura_paciente);
    getchar(); 

    printf("\tDigite o plano de saúde: ");
    fgets(novoPaciente.planoSaude_paciente, sizeof(novoPaciente.planoSaude_paciente), stdin);
    novoPaciente.planoSaude_paciente[strcspn(novoPaciente.planoSaude_paciente, "\n")] = 0;

    printf("\tDigite o convênio médico: ");
    fgets(novoPaciente.convenioMedico_paciente, sizeof(novoPaciente.convenioMedico_paciente), stdin);
    novoPaciente.convenioMedico_paciente[strcspn(novoPaciente.convenioMedico_paciente, "\n")] = 0;

    printf("\tDigite o número da carteira: ");
    fgets(novoPaciente.numeroCarteira_paciente, sizeof(novoPaciente.numeroCarteira_paciente), stdin);
    novoPaciente.numeroCarteira_paciente[strcspn(novoPaciente.numeroCarteira_paciente, "\n")] = 0;

    printf("\tDigite a validade da carteira: ");
    fgets(novoPaciente.validadeCarteira_paciente, sizeof(novoPaciente.validadeCarteira_paciente), stdin);
    novoPaciente.validadeCarteira_paciente[strcspn(novoPaciente.validadeCarteira_paciente, "\n")] = 0;

    printf("\tDigite o cartão CNS: ");
    fgets(novoPaciente.cartaoCNS_paciente, sizeof(novoPaciente.cartaoCNS_paciente), stdin);
    novoPaciente.cartaoCNS_paciente[strcspn(novoPaciente.cartaoCNS_paciente, "\n")] = 0;

    printf("\tDigite uma observação: ");
    fgets(novoPaciente.observacao_paciente, sizeof(novoPaciente.observacao_paciente), stdin);
    novoPaciente.observacao_paciente[strcspn(novoPaciente.observacao_paciente, "\n")] = 0;

	//Finalização do cadastro
	int finalizar;
    puts("\t+-------------------------------+");
    puts("\t|      Você quer finalizar      |\n        |          o cadastro?          |");
    puts("\t| 1. Sim                        |");
    puts("\t| 0. Não                        |");
    puts("\t+-------------------------------+");
    printf("\tEscolha: ");
    scanf("%d", &finalizar);

	if (finalizar == 1) {
		puts("\t+-------------------------------+");
    	if (inserirNoFim(lista, &novoPaciente, sizeof(PACIENTE))) {
        	puts("\t|      Paciente cadastrado      |\n        |          com sucesso!         |");

        	salvarPacientes(lista);
        	// printf("Paciente cadastrado: ID=%d, Nome=%s, Telefone=%s\n", novoPaciente.id_paciente, novoPaciente.nome_paciente, novoPaciente.telefone_paciente); // Pra teste
    	} else {
       		puts("\t|  Erro ao cadastrar paciente.  |");
       		ultimo_id_paciente--;
   		}
   		puts("\t+-------------------------------+");
    	system("pause");
	}else {
        int descartar;
        puts("\t+-------------------------------+");
        puts("\t|    Você quer descartar ou     |\n        |   continuar esse cadastro?    |");
        puts("\t| 1. Continuar                  |");
        puts("\t| 0. Descartar                  |");
        puts("\t+-------------------------------+");
        printf("\tEscolha: ");
        scanf("%d", &descartar);

        if (descartar == 0) {
        	ultimo_id_paciente--;
        	puts("\t+-------------------------------+");
            puts("\t|      Cadastro descartado.     |");
            puts("\t+-------------------------------+");
            system("pause");
            return;
        }
        ultimo_id_paciente--;
        puts("\t+-------------------------------+");
        puts("\t|      Insira novamente os      |\n        |       dados do cadastro.      |");
        puts("\t+-------------------------------+");
        system("pause");
        cadastrarPaciente(lista);
    }
}

void editarPaciente(LISTA *lista) {
    int id;
    
    system("cls || clear");
    puts("\t+-------------------------------+");
    puts("\t|                               |");
    puts("\t|       EDIÇÃO DE PACIENTE      |");
    puts("\t|                               |");
    puts("\t+-------------------------------+");
    printf("\tDigite o número do paciente: ");
    scanf("%d", &id);

    NO *atual = lista->inicio;
    while (atual != NULL) {
        PACIENTE *paciente = (PACIENTE *)atual->dado;
        if (paciente->id_paciente == id) {
            printf("\tDigite o novo nome: ");
            getchar();
            fgets(paciente->nome_paciente, sizeof(paciente->nome_paciente), stdin);
            paciente->nome_paciente[strcspn(paciente->nome_paciente, "\n")] = 0;

            printf("\tDigite o novo telefone: ");
            fgets(paciente->telefone_paciente, sizeof(paciente->telefone_paciente), stdin);
            paciente->telefone_paciente[strcspn(paciente->telefone_paciente, "\n")] = 0;

            printf("\tDigite a nova data de nascimento (dd/mm/aaaa): ");
            fgets(paciente->datanasc_paciente, sizeof(paciente->datanasc_paciente), stdin);
            paciente->datanasc_paciente[strcspn(paciente->datanasc_paciente, "\n")] = 0;

            printf("\tDigite o novo sexo: ");
            fgets(paciente->sexo_paciente, sizeof(paciente->sexo_paciente), stdin);
            paciente->sexo_paciente[strcspn(paciente->sexo_paciente, "\n")] = 0;

            printf("\tDigite o novo CPF: ");
            fgets(paciente->CPF_paciente, sizeof(paciente->CPF_paciente), stdin);
            paciente->CPF_paciente[strcspn(paciente->CPF_paciente, "\n")] = 0;

            printf("\tDigite o novo RG: ");
            fgets(paciente->RG_paciente, sizeof(paciente->RG_paciente), stdin);
            paciente->RG_paciente[strcspn(paciente->RG_paciente, "\n")] = 0;

            printf("\tDigite o novo email: ");
            fgets(paciente->email_paciente, sizeof(paciente->email_paciente), stdin);
            paciente->email_paciente[strcspn(paciente->email_paciente, "\n")] = 0;

            printf("\tDigite o novo endereço: ");
            fgets(paciente->endereco_paciente, sizeof(paciente->endereco_paciente), stdin);
            paciente->endereco_paciente[strcspn(paciente->endereco_paciente, "\n")] = 0;

            printf("\tDigite o novo estado civil: ");
            fgets(paciente->estadocivil_paciente, sizeof(paciente->estadocivil_paciente), stdin);
            paciente->estadocivil_paciente[strcspn(paciente->estadocivil_paciente, "\n")] = 0;

            printf("\tDigite a nova profissão: ");
            fgets(paciente->profissao_paciente, sizeof(paciente->profissao_paciente), stdin);
            paciente->profissao_paciente[strcspn(paciente->profissao_paciente, "\n")] = 0;

            printf("\tDigite o novo peso (kg): ");
            scanf("%f", &paciente->peso_paciente);

            printf("\tDigite a nova altura (m): ");
            scanf("%f", &paciente->altura_paciente);

            getchar(); // Limpa o buffer após a leitura de float

            printf("\tDigite o novo plano de saúde: ");
            fgets(paciente->planoSaude_paciente, sizeof(paciente->planoSaude_paciente), stdin);
            paciente->planoSaude_paciente[strcspn(paciente->planoSaude_paciente, "\n")] = 0;

            printf("\tDigite o novo convênio médico: ");
            fgets(paciente->convenioMedico_paciente, sizeof(paciente->convenioMedico_paciente), stdin);
            paciente->convenioMedico_paciente[strcspn(paciente->convenioMedico_paciente, "\n")] = 0;

            printf("\tDigite o novo número da carteira: ");
            fgets(paciente->numeroCarteira_paciente, sizeof(paciente->numeroCarteira_paciente), stdin);
            paciente->numeroCarteira_paciente[strcspn(paciente->numeroCarteira_paciente, "\n")] = 0;

            printf("\tDigite a nova validade da carteira: ");
            fgets(paciente->validadeCarteira_paciente, sizeof(paciente->validadeCarteira_paciente), stdin);
            paciente->validadeCarteira_paciente[strcspn(paciente->validadeCarteira_paciente, "\n")] = 0;

            printf("\tDigite o novo cartão CNS: ");
            fgets(paciente->cartaoCNS_paciente, sizeof(paciente->cartaoCNS_paciente), stdin);
            paciente->cartaoCNS_paciente[strcspn(paciente->cartaoCNS_paciente, "\n")] = 0;

            printf("\tDigite uma nova observação: ");
            fgets(paciente->observacao_paciente, sizeof(paciente->observacao_paciente), stdin);
            paciente->observacao_paciente[strcspn(paciente->observacao_paciente, "\n")] = 0;

            
            puts("\t+-------------------------------+");
            puts("\t|        Paciente editado       |\n        |          com sucesso!         |");
            puts("\t+-------------------------------+");
            system("pause");
            salvarPacientes(lista);
            return;
        }
        atual = atual->proximo;
    }
    puts("\t+-------------------------------+");
    puts("\t|   Paciente não encontrado!    |");
    puts("\t+-------------------------------+");
    system("pause");
}

void removerPaciente(LISTA *lista) {
    int id;
    
    system("cls || clear");
    puts("\t+-------------------------------+");
    puts("\t|                               |");
    puts("\t|      EXCLUSÃO DE PACIENTE     |");
    puts("\t|                               |");
    puts("\t+-------------------------------+");
    printf("\tDigite o número do paciente: ");
    scanf("%d", &id);

    NO *anterior = NULL;
    NO *atual = lista->inicio;
    while (atual != NULL) {
        PACIENTE *paciente = (PACIENTE *)atual->dado;
        if (paciente->id_paciente == id) {
            if (anterior == NULL) {
                lista->inicio = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual->dado);
            free(atual);
            puts("\t+-------------------------------+");
            puts("\t|       Paciente removido       |\n        |          com sucesso!         |");
            puts("\t+-------------------------------+");
            system("pause");
            salvarPacientes(lista);
            // printf("Paciente removido: ID=%d\n", id); // Pra teste
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    puts("\t+-------------------------------+");
    puts("\t|   Paciente não encontrado!    |");
    puts("\t+-------------------------------+");
    system("pause");
}

void consultarDadosPaciente(LISTA *lista) {
    int id;
    printf("\tDigite o número do paciente: ");
    scanf("%d", &id);

    PACIENTE *pacienteEncontrado = (PACIENTE *)buscarElemento(lista, compararPacientePorID, &id);

    if (pacienteEncontrado != NULL) {
        system("cls || clear");
        printf("\t+-------------------------------+\n");
        printf("\t|       DADOS DO PACIENTE       |\n");
        printf("\t+-------------------------------+\n");
        imprimirPaciente2(pacienteEncontrado);
         puts("\t+-------------------------------+");
        puts("\t| 1. Alterar Paciente           |");
        puts("\t| 2. Remover Paciente           |");
        puts("\t| 3. Voltar                     |");
        puts("\t+-------------------------------+");
        printf("\tEscolha: ");

            int opcao;
            scanf("%d", &opcao);

            if (opcao == 1) {
                editarPaciente(lista);
            } else if (opcao == 2) {
                removerPaciente(lista);
                return;
            } else if (opcao == 3) {
                return;
            } else {
                puts("\t| Opção inválida! Tente novamente. |");
       		    system("pause");}
    }else {
        puts("Paciente não encontrado.");
        system("pause");
    }
}

void listarPacientes(LISTA *lista) {
    if (tamanhoLista(lista) == 0) {
        puts("Nenhum paciente cadastrado. Cadastre um paciente primeiro.");
        system("pause");
        return; // Retorna para evitar listar pacientes
    }

    int totalPacientes = tamanhoLista(lista);
    int pagina = 0;
    int pacientesPorPagina = 5;
    int totalPaginas = (totalPacientes + pacientesPorPagina - 1) / pacientesPorPagina;

    while (1) {
        system("cls || clear"); // Limpa a tela
        puts("\t+-------------------------------+");
        puts("\t|       LISTA DE PACIENTES      |");
        printf("\t|   Pacientes cadastrados: %d   |\n", totalPacientes);
        puts("\t+-------------------------------+");
        
        NO *atual = lista->inicio;
        // Avança para o início da página atual
        for (i = 0; i < pagina * pacientesPorPagina && atual != NULL; i++) {
            atual = atual->proximo;
        }

        // Exibe os pacientes da página atual
        for (i = 0; i < pacientesPorPagina && atual != NULL; i++) {
            imprimirPaciente(atual->dado);
            if(i+1 != pacientesPorPagina){
            	if(i+1 != totalPacientes){
				printf("\t---------------------------------\n");}
			}
            atual = atual->proximo;
        }

        puts("\t+-------------------------------+");
        printf("\t| Página %02d de %02d               |\n", pagina + 1, totalPaginas);
        puts("\t+-------------------------------+");
        puts("\t| 1. Consultar Paciente         |");
        puts("\t| 2. Página Anterior            |");
        puts("\t| 3. Próxima Página             |");
        puts("\t| 0. Voltar ao Menu             |");
        puts("\t+-------------------------------+");
        printf("\tEscolha: ");

        int opcao;
        scanf("%d", &opcao);

		if (opcao == 1) { // Consultar paciente
            consultarDadosPaciente(lista);
        }
        else if (opcao == 3) { // Próxima página
            if (pagina < totalPaginas - 1) {
                pagina++;
            } else {
            	puts("\t+-------------------------------+");
                puts("\t|  Você está na última página.  |");
                puts("\t+-------------------------------+");
                system("pause");
            }
        } else if (opcao == 2) { // Página anterior
            if (pagina > 0) {
                pagina--;
            } else {
            	puts("\t+-------------------------------+");
                puts("\t| Você está na primeira página. |");
                puts("\t+-------------------------------+");
                system("pause");
            }
        } else if (opcao == 0) {
            return; // Voltar ao menu
        } else {
            puts("\t| Opção inválida! Tente novamente. |");
            system("pause");
        }
    }
}

bool compararPacientePorID(void *dado, void *chave) {
    PACIENTE *paciente = (PACIENTE *)dado;
    int *idChave = (int *)chave;
    // printf("Comparando ID: Paciente=%d, Chave=%d\n", paciente->id_paciente, *idChave); // Pra teste
    return paciente->id_paciente == *idChave;
}


//Funções do menu de Consulta/Consulta em Geral
void carregarConsultas(LISTA *lista) {
    FILE *arquivo = fopen(arquivo_consultas, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        
        return;
    }

    CONSULTA novaConsulta;
    while (fscanf(arquivo, "%d;%d;%d;%10[^;];%d;%d;%d;%5[^;];%49[^;];%19[^\n]\n",
                  &novaConsulta.id_consulta,
                  &novaConsulta.idpaciente_consulta,
                  &novaConsulta.idmedico_consulta,
                  novaConsulta.data_consulta,
                  &novaConsulta.dia_consulta,
                  &novaConsulta.mes_consulta,
                  &novaConsulta.ano_consulta,
                  novaConsulta.hora_consulta,
                  novaConsulta.tipoAtendimento,
                  novaConsulta.status) == 10) {
        inserirNoFim(lista, &novaConsulta, sizeof(CONSULTA));
        if (novaConsulta.id_consulta > ultimo_id_consulta) {
            ultimo_id_consulta = novaConsulta.id_consulta;
        }
    }

    fclose(arquivo);
   	//printf("Consultas carregadas com sucesso!\n");
}

void salvarConsultas(LISTA *lista) {
    FILE *arquivo = fopen(arquivo_consultas, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    NO *atual = lista->inicio;
    while (atual != NULL) {
        CONSULTA *consulta = (CONSULTA *)atual->dado;
       fprintf(arquivo, "%d;%d;%d;%s;%d;%d;%d;%s;%s;%s\n",
                consulta->id_consulta,
                consulta->idpaciente_consulta,
                consulta->idmedico_consulta,
                consulta->data_consulta,
                consulta->dia_consulta,
                consulta->mes_consulta,
                consulta->ano_consulta,
                consulta->hora_consulta,
                consulta->tipoAtendimento,
                consulta->status);
        atual = atual->proximo;
    }

    fclose(arquivo);
    //printf("Consultas salvas com sucesso!\n");
}

void imprimirConsulta(void *dado) {
    CONSULTA *consulta = (CONSULTA *)dado;
    printf("\tNúmero da Consulta: %d\n", consulta->id_consulta);
    //Para teste
	    switch (consulta->idpaciente_consulta) {
	    case 1:
	        printf("\tNome do Paciente: Yasmim Frota\n");
	        break;
	    case 2:
	        printf("\tNome do Paciente: Carlos Eduardo\n");
	        break;
	    case 3:
	        printf("\tNome do Paciente: Gabriel Fernandes\n");
	        break;
	    case 4:
	        printf("\tNome do Paciente: Kayo Serrão\n");
	        break;
	    case 5:
	        printf("\tNome do Paciente: Adriano Melo\n");
	        break;
	    case 6:
	        printf("\tNome do Paciente: Larissa Souza\n");
	        break;
	    case 7:
	        printf("\tNome do Paciente: Juliane Costa\n");
	        break;
	    case 8:
	        printf("\tNome do Paciente: Miguel Viana\n");
	        break;
	    case 9:
	        printf("\tNome do Paciente: Silas Salan\n");
	        break;
	    case 10:
	        printf("\tNome do Paciente: Matheus Victor\n");
	        break;
	    default:
	        printf("\tPaciente não encontrado.\n");
	        break;
		}
    //Para teste
      switch (consulta->idmedico_consulta) {
        case 1:
            printf("\tNome do Médico: Dr. João Silva\n");
            printf("\tEspecialização: Cardiologia\n");
            break;
        case 2:
            printf("\tNome do Médico: Dra. Maria Oliveira\n");
            printf("\tEspecialização: Pediatria\n");
            break;
        case 3:
            printf("\tNome do Médico: Dr. Carlos Pereira\n");
            printf("\tEspecialização: Neurologia\n");
            break;
        case 4:
            printf("\tNome do Médico: Dra. Ana Souza\n");
            printf("\tEspecialização: Dermatologia\n");
            break;
        case 5:
            printf("\tNome do Médico: Dr. Pedro Santos\n");
            printf("\tEspecialização: Ortopedia\n");
            break;
        case 6:
            printf("\tNome do Médico: Dra. Fernanda Lima\n");
            printf("\tEspecialização: Ginecologia\n");
            break;
        case 7:
            printf("\tNome do Médico: Dr. Lucas Almeida\n");
            printf("\tEspecialização: Oftalmologia\n");
            break;
        case 8:
            printf("\tNome do Médico: Dra. Juliana Costa\n");
            printf("\tEspecialização: Psiquiatria\n");
            break;
        case 9:
            printf("\tNome do Médico: Dr. Rafael Mendes\n");
            printf("\tEspecialização: Clínico Geral\n");
            break;
        case 10:
            printf("\tNome do Médico: Dra. Beatriz Rocha\n");
            printf("\tEspecialização: Endocrinologia\n");
            break;
        default:
            printf("\tMédico não encontrado.\n");
            break;
    }
    printf("\tData: %s\n", consulta->data_consulta);
}
void imprimirConsulta2(void *dado) {
    CONSULTA *consulta = (CONSULTA *)dado;
    printf("\tNúmero da Consulta: %d\n", consulta->id_consulta);
    printf("\tID do Paciente: %d\n", consulta->idpaciente_consulta);
	    switch (consulta->idpaciente_consulta) {
	    case 1:
	        printf("\tNome do Paciente: Yasmim Frota\n");
	        break;
	    case 2:
	        printf("\tNome do Paciente: Carlos Eduardo\n");
	        break;
	    case 3:
	        printf("\tNome do Paciente: Gabriel Fernandes\n");
	        break;
	    case 4:
	        printf("\tNome do Paciente: Kayo Serrão\n");
	        break;
	    case 5:
	        printf("\tNome do Paciente: Adriano Melo\n");
	        break;
	    case 6:
	        printf("\tNome do Paciente: Larissa Souza\n");
	        break;
	    case 7:
	        printf("\tNome do Paciente: Juliane Costa\n");
	        break;
	    case 8:
	        printf("\tNome do Paciente: Miguel Viana\n");
	        break;
	    case 9:
	        printf("\tNome do Paciente: Silas Salan\n");
	        break;
	    case 10:
	        printf("\tNome do Paciente: Matheus Victor\n");
	        break;
	    default:
	        printf("\tPaciente não encontrado.\n");
	        break;
		}
    printf("\tID do Médico: %d\n", consulta->idmedico_consulta);
      switch (consulta->idmedico_consulta) {
        case 1:
            printf("\tNome do Médico: Dr. João Silva\n");
            printf("\tEspecialização: Cardiologia\n");
            break;
        case 2:
            printf("\tNome do Médico: Dra. Maria Oliveira\n");
            printf("\tEspecialização: Pediatria\n");
            break;
        case 3:
            printf("\tNome do Médico: Dr. Carlos Pereira\n");
            printf("\tEspecialização: Neurologia\n");
            break;
        case 4:
            printf("\tNome do Médico: Dra. Ana Souza\n");
            printf("\tEspecialização: Dermatologia\n");
            break;
        case 5:
            printf("\tNome do Médico: Dr. Pedro Santos\n");
            printf("\tEspecialização: Ortopedia\n");
            break;
        case 6:
            printf("\tNome do Médico: Dra. Fernanda Lima\n");
            printf("\tEspecialização: Ginecologia\n");
            break;
        case 7:
            printf("\tNome do Médico: Dr. Lucas Almeida\n");
            printf("\tEspecialização: Oftalmologia\n");
            break;
        case 8:
            printf("\tNome do Médico: Dra. Juliana Costa\n");
            printf("\tEspecialização: Psiquiatria\n");
            break;
        case 9:
            printf("\tNome do Médico: Dr. Rafael Mendes\n");
            printf("\tEspecialização: Clínico Geral\n");
            break;
        case 10:
            printf("\tNome do Médico: Dra. Beatriz Rocha\n");
            printf("\tEspecialização: Endocrinologia\n");
            break;
        default:
            printf("\tMédico não encontrado.\n");
            break;
    }
    printf("\tData: %s\n", consulta->data_consulta);
    printf("\tDia: %d\n", consulta->dia_consulta);
    printf("\tMês: %d\n", consulta->mes_consulta);
    printf("\tAno: %d\n", consulta->ano_consulta);
    printf("\tHora: %s\n", consulta->hora_consulta);
    printf("\tTipo de Atendimento: %s\n", consulta->tipoAtendimento);
    printf("\tStatus: %s\n", consulta->status);
}

void cadastrarConsulta(LISTA *lista) {
    CONSULTA novaConsulta;
    ultimo_id_consulta++;
    novaConsulta.id_consulta = ultimo_id_consulta;

    // Formulário de preenchimento do cadastro
    system("cls || clear");
    puts("\t+-------------------------------+");
    puts("\t|                               |");
    puts("\t|      CADASTRO DE CONSULTA     |");
    puts("\t|                               |");
    puts("\t+-------------------------------+");
    printf("\tNúmero da nova consulta: %d\n", novaConsulta.id_consulta);

    printf("\tDigite o ID do paciente: ");
    scanf("%d", &novaConsulta.idpaciente_consulta);
	    switch (novaConsulta.idpaciente_consulta) {
	    case 1:
	        printf("\tNome do Paciente: Yasmim Frota\n");
	        break;
	    case 2:
	        printf("\tNome do Paciente: Carlos Eduardo\n");
	        break;
	    case 3:
	        printf("\tNome do Paciente: Gabriel Fernandes\n");
	        break;
	    case 4:
	        printf("\tNome do Paciente: Kayo Serrão\n");
	        break;
	    case 5:
	        printf("\tNome do Paciente: Adriano Melo\n");
	        break;
	    case 6:
	        printf("\tNome do Paciente: Larissa Souza\n");
	        break;
	    case 7:
	        printf("\tNome do Paciente: Juliane Costa\n");
	        break;
	    case 8:
	        printf("\tNome do Paciente: Miguel Viana\n");
	        break;
	    case 9:
	        printf("\tNome do Paciente: Silas Salan\n");
	        break;
	    case 10:
	        printf("\tNome do Paciente: Matheus Victor\n");
	        break;
	    default:
	        printf("\tPaciente não encontrado.\n");
	        break;
		}
	printf("\tDigite o ID do médico: ");
    scanf("%d", &novaConsulta.idmedico_consulta);
      switch (novaConsulta.idpaciente_consulta) {
        case 1:
            printf("\tNome do Médico: Dr. João Silva\n");
            printf("\tEspecialização: Cardiologia\n");
            break;
        case 2:
            printf("\tNome do Médico: Dra. Maria Oliveira\n");
            printf("\tEspecialização: Pediatria\n");
            break;
        case 3:
            printf("\tNome do Médico: Dr. Carlos Pereira\n");
            printf("\tEspecialização: Neurologia\n");
            break;
        case 4:
            printf("\tNome do Médico: Dra. Ana Souza\n");
            printf("\tEspecialização: Dermatologia\n");
            break;
        case 5:
            printf("\tNome do Médico: Dr. Pedro Santos\n");
            printf("\tEspecialização: Ortopedia\n");
            break;
        case 6:
            printf("\tNome do Médico: Dra. Fernanda Lima\n");
            printf("\tEspecialização: Ginecologia\n");
            break;
        case 7:
            printf("\tNome do Médico: Dr. Lucas Almeida\n");
            printf("\tEspecialização: Oftalmologia\n");
            break;
        case 8:
            printf("\tNome do Médico: Dra. Juliana Costa\n");
            printf("\tEspecialização: Psiquiatria\n");
            break;
        case 9:
            printf("\tNome do Médico: Dr. Rafael Mendes\n");
            printf("\tEspecialização: Clínico Geral\n");
            break;
        case 10:
            printf("\tNome do Médico: Dra. Beatriz Rocha\n");
            printf("\tEspecialização: Endocrinologia\n");
            break;
        default:
            printf("\tMédico não encontrado.\n");
            break;
    }

    printf("\tDigite a data da consulta (dd/mm/aaaa): ");
    getchar();
    fgets(novaConsulta.data_consulta, sizeof(novaConsulta.data_consulta), stdin);
    novaConsulta.data_consulta[strcspn(novaConsulta.data_consulta, "\n")] = 0;

    sscanf(novaConsulta.data_consulta, "%d/%d/%d", &novaConsulta.dia_consulta, &novaConsulta.mes_consulta, &novaConsulta.ano_consulta);
	getchar();
	
    printf("\tDigite a hora da consulta (hh:mm): ");
    fgets(novaConsulta.hora_consulta, sizeof(novaConsulta.hora_consulta), stdin);
    novaConsulta.hora_consulta[strcspn(novaConsulta.hora_consulta, "\n")] = 0;
	getchar();
	
    printf("\tDigite o tipo de atendimento: ");
    fgets(novaConsulta.tipoAtendimento, sizeof(novaConsulta.tipoAtendimento), stdin);
    novaConsulta.tipoAtendimento[strcspn(novaConsulta.tipoAtendimento, "\n")] = 0;
	
    printf("\tDigite o status da consulta: ");
    fgets(novaConsulta.status, sizeof(novaConsulta.status), stdin);
    novaConsulta.status[strcspn(novaConsulta.status, "\n")] = 0;

    // Finalização do cadastro
    int finalizar;
    puts("\t+-------------------------------+");
    puts("\t|      Você quer finalizar      |\n        |          o cadastro?          |");
    puts("\t| 1. Sim                        |");
    puts("\t| 0. Não                        |");
    puts("\t+-------------------------------+");
    printf("\tEscolha: ");
    scanf("%d", &finalizar);

    if (finalizar == 1) {
        puts("\t+-------------------------------+");
        if (inserirNoFim(lista, &novaConsulta, sizeof(CONSULTA))) {
            puts("\t|      Consulta cadastrada      |\n        |          com sucesso!         |");
            salvarConsultas(lista);
        } else {
            puts("\t|  Erro ao cadastrar consulta.  |");
            ultimo_id_consulta--;
        }
        puts("\t+-------------------------------+");
        system("pause");
    } else {
        int descartar;
        puts("\t+-------------------------------+");
        puts("\t|    Você quer descartar ou     |\n        |   continuar esse cadastro?    |");
        puts("\t| 1. Continuar                  |");
        puts("\t| 0. Descartar                  |");
        puts("\t+-------------------------------+");
        printf("\tEscolha: ");
        scanf("%d", &descartar);

        if (descartar == 0) {
            ultimo_id_consulta--;
            puts("\t+-------------------------------+");
            puts("\t|      Cadastro descartado.     |");
            puts("\t+-------------------------------+");
            system("pause");
            return;
        }
        ultimo_id_consulta--;
        puts("\t+-------------------------------+");
        puts("\t|      Insira novamente os      |\n        |       dados do cadastro.      |");
        puts("\t+-------------------------------+");
        system("pause");
        cadastrarConsulta(lista);
    }
}

bool compararConsultaPorID(void *dado, void *chave) {
    CONSULTA *consulta = (CONSULTA *)dado;
    int *idChave = (int *)chave;
    // printf("Comparando ID: Consulta=%d, Chave=%d\n", consulta->id_consulta, *idChave); // Pra teste
    return consulta->id_consulta == *idChave;
}

void consultarDadosConsulta(LISTA *lista) {
    int id;
    printf("\tDigite o número da consulta: ");
    scanf("%d", &id);

    CONSULTA *consultaEncontrada = (CONSULTA *)buscarElemento(lista, compararConsultaPorID, &id);

    if (consultaEncontrada != NULL) {
        system("cls || clear");
        printf("\t+-------------------------------+\n");
        printf("\t|       DADOS DA CONSULTA       |\n");
        printf("\t+-------------------------------+\n");
        imprimirConsulta2(consultaEncontrada);
        puts("\t+-------------------------------+");
        puts("\t| 1. Alterar Consulta           |");
        puts("\t| 2. Remover Consulta           |");
        puts("\t| 3. Voltar                     |");
        puts("\t+-------------------------------+");
        printf("\tEscolha: ");

        int opcao;
        scanf("%d", &opcao);

        if (opcao == 1) {
            editarConsulta(lista);
        } else if (opcao == 2) {
            removerConsulta(lista);
            return;
        } else if (opcao == 3) {
            return;
        } else {
            puts("\t| Opção inválida! Tente novamente. |");
            system("pause");
        }
    } else {
        puts("Consulta não encontrada.");
        system("pause");
    }
}

void editarConsulta(LISTA *lista) {
    int id;
    
    system("cls || clear");
    puts("\t+-------------------------------+");
    puts("\t|                               |");
    puts("\t|      EDIÇÃO DE CONSULTA       |");
    puts("\t|                               |");
    puts("\t+-------------------------------+");
    printf("\tDigite o número da consulta: ");
    scanf("%d", &id);

    NO *atual = lista->inicio;
    while (atual != NULL) {
        CONSULTA *consulta = (CONSULTA *)atual->dado;
        if (consulta->id_consulta == id) {
            printf("\tDigite a nova data (dd/mm/aaaa): ");
            getchar(); // Limpa o buffer	
            fgets(consulta->data_consulta, sizeof(consulta->data_consulta), stdin);
            consulta->data_consulta[strcspn(consulta->data_consulta, "\n")] = 0;

		
            printf("\tDigite a nova hora (hh:mm): ");
            getchar;
            fgets(consulta->hora_consulta, sizeof(consulta->hora_consulta), stdin);
            consulta->hora_consulta[strcspn(consulta->hora_consulta, "\n")] = 0;

            printf("\tDigite o novo tipo de atendimento: ");
            getchar;
            fgets(consulta->tipoAtendimento, sizeof(consulta->tipoAtendimento), stdin);
            consulta->tipoAtendimento[strcspn(consulta->tipoAtendimento, "\n")] = 0;

            printf("\tDigite o novo status: ");
            getchar;
            fgets(consulta->status, sizeof(consulta->status), stdin);
            consulta->status[strcspn(consulta->status, "\n")] = 0;

            puts("\t+-------------------------------+");
            puts("\t|        Consulta editada       |\n        |          com sucesso!         |");
            puts("\t+-------------------------------+");
            system("pause");
            salvarConsultas(lista); // Salva as alterações
            return;
        }
        atual = atual->proximo;
    }
    puts("\t+-------------------------------+");
    puts("\t|   Consulta não encontrada!    |");
    puts("\t+-------------------------------+");
    system("pause");
}

void removerConsulta(LISTA *lista) {
    int id;
    
    system("cls || clear");
    puts("\t+-------------------------------+");
    puts("\t|                               |");
    puts("\t|      EXCLUSÃO DE CONSULTA     |");
    puts("\t|                               |");
    puts("\t+-------------------------------+");
    printf("\tDigite o número da consulta: ");
    scanf("%d", &id);

    NO *anterior = NULL;
    NO *atual = lista->inicio;
    while (atual != NULL) {
        CONSULTA *consulta = (CONSULTA *)atual->dado;
        if (consulta->id_consulta == id) {
            if (anterior == NULL) {
                lista->inicio = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual->dado);
            free(atual);
            puts("\t+-------------------------------+");
            puts("\t|       Consulta removida       |\n        |          com sucesso!         |");
            puts("\t+-------------------------------+");
            system("pause");
            salvarConsultas(lista); // Salva as alterações
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    puts("\t+-------------------------------+");
    puts("\t|   Consulta não encontrada!    |");
    puts("\t+-------------------------------+");
    system("pause");
}

void listarConsultas(LISTA *lista) {
    if (tamanhoLista(lista) == 0) {
        puts("Nenhuma consulta cadastrada. Cadastre uma consulta primeiro.");
        system("pause");
        return; // Retorna para evitar listar consultas
    }

    int totalConsultas = tamanhoLista(lista);
    int pagina = 0;
    int consultasPorPagina = 5;
    int totalPaginas = (totalConsultas + consultasPorPagina - 1) / consultasPorPagina;

    while (1) {
        system("cls || clear"); // Limpa a tela
        puts("\t+-------------------------------+");
          puts("\t|      LISTA DE CONSULTAS       |");
        printf("\t|   Consultas cadastradas: %d    |\n", totalConsultas);
          puts("\t+-------------------------------+");
        
        NO *atual = lista->inicio;
        // Avança para o início da página atual
        for (i = 0; i < pagina * consultasPorPagina && atual != NULL; i++) {
            atual = atual->proximo;
        }

        // Exibe as consultas da página atual
        for (i = 0; i < consultasPorPagina && atual != NULL; i++) {
            imprimirConsulta(atual->dado);
            if (i + 1 != consultasPorPagina) {
                if (i + 1 != totalConsultas) {
                    printf("\t---------------------------------\n");
                }
            }
            atual = atual->proximo;
        }

        puts("\t+-------------------------------+");
        printf("\t| Página %02d de %02d               |\n", pagina + 1, totalPaginas);
        puts("\t+-------------------------------+");
        puts("\t| 1. Consultar Consulta         |");
        puts("\t| 2. Página Anterior            |");
        puts("\t| 3. Próxima Página             |");
        puts("\t| 0. Voltar ao Menu             |");
        puts("\t+-------------------------------+");
        printf("\tEscolha: ");

        int opcao;
        scanf("%d", &opcao);

        if (opcao == 1) { // Consultar consulta
            consultarDadosConsulta(lista);
        }
        else if (opcao == 3) { // Próxima página
            if (pagina < totalPaginas - 1) {
                pagina++;
            } else {
                puts("\t+-------------------------------+");
                puts("\t|  Você está na última página.  |");
                puts("\t+-------------------------------+");
                system("pause");
            }
        } else if (opcao == 2) { // Página anterior
            if (pagina > 0) {
                pagina--;
            } else {
                puts("\t+-------------------------------+");
                puts("\t| Você está na primeira página. |");
                puts("\t+-------------------------------+");
                system("pause");
            }
        } else if (opcao == 0) {
            return; // Voltar ao menu
        } else {
            puts("\t| Opção inválida! Tente novamente. |");
            system("pause");
        }
    }
}

//Funções de menu
void menuListaPacientes(LISTA *lista) {
    int op;

   	do {
        system("cls || clear"); 
        
        puts("\t+-------------------------------+");
        puts("\t|                               |");
        puts("\t|   MENU DE LISTA DE PACIENTES  |");
        puts("\t|                               |");
        puts("\t+-------------------------------+");
        puts("\t|       Deseja Realizar:        |");
        puts("\t|                               |");
        puts("\t| 1. Pesquisar Paciente         |");
        puts("\t| 2. Exibir Todos os Pacientes  |");
        puts("\t| 0. Voltar ao Menu Anterior    |");
        puts("\t|                               |");
        puts("\t+-------------------------------+");
        printf("\tDigite a opcão: ");

        int opcao;
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                //pesquisarPacientes(lista);
                break;
            case 2:
                listarPacientes(lista);
                break;
            case 0:
                system("cls || clear");
                puts("Voltando ao menu anterior...\n");
                system("pause");
                return;
            default:
                system("cls || clear");
                puts("Entrada inválida, tente novamente");
                system("pause");
                break;
        }
    }while (op != 0);
}

void menuPaciente(LISTA *lista) {
    int op;

    do {
        system("cls || clear");

        puts("\t+-------------------------------+");
        puts("\t|                               |");
        puts("\t|       MENU DE PACIENTES       |");
        puts("\t|                               |");
        puts("\t+-------------------------------+");
        puts("\t|       Deseja Realizar:        |");
        puts("\t|                               |");
        puts("\t| 1. Cadastrar Paciente         |");
        puts("\t| 2. Listar Pacientes           |");
        puts("\t| 3. Editar Paciente            |");
        puts("\t| 4. Remover Paciente           |");
        puts("\t| 0. Voltar ao Menu Anterior    |");
        puts("\t|                               |");
        puts("\t+-------------------------------+");
        printf("\tDigite a opcão: ");
        scanf("%i", &op);

        switch(op) {
            case 1:
                cadastrarPaciente(lista);
                break;
            case 2:
                menuListaPacientes(lista);
                break;
            case 3:
                editarPaciente(lista);
                break;
            case 4:
                removerPaciente(lista);
                break;
            case 0:
                system("cls || clear");
                puts("Voltando ao menu anterior...\n");
                system("pause");
                return;
                break;
            default:
                system("cls || clear");
                puts("Entrada inválida, tente novamente");
                system("pause");
                break;
        }
    } while (op != 0);
}

void menuConsulta(LISTA *lista) {
    int op;

    do {
        system("cls || clear");

        puts("\t+-------------------------------+");
        puts("\t|                               |");
        puts("\t|       MENU DE CONSULTAS       |");
        puts("\t|                               |");
        puts("\t+-------------------------------+");
        puts("\t|       Deseja Realizar:        |");
        puts("\t|                               |");
        puts("\t| 1. Cadastrar Consulta         |");
        puts("\t| 2. Listar Consultas           |");
        puts("\t| 3. Editar Consulta            |");
        puts("\t| 4. Remover Consulta           |");
        puts("\t| 0. Voltar ao Menu Anterior    |");
        puts("\t|                               |");
        puts("\t+-------------------------------+");
        printf("\tDigite a opcão: ");
        scanf("%i", &op);

        switch(op) {
            case 1:
                cadastrarConsulta(lista);
                break;
            case 2:
                listarConsultas(lista);
                break;
            case 3:
                editarConsulta(lista);
                break;
            case 4:
                removerConsulta(lista);
                break;
            case 0:
               	system("cls || clear");
                puts("Voltando ao menu anterior...\n");
                system("pause");
                return;
                break;
            default:
                system("cls || clear");
                puts("Entrada inválida, tente novamente");
                system("pause");
                break;
        }
    } while (op != 0);
}

void menuAgendaMedica() {
    puts("Funcionalidade de agenda médica removida para refatoração. Em andamento...");
}

void menuRecepcionista() {
    int op;
    LISTA *lista_pacientes = criarLista();
    LISTA *lista_consultas = criarLista();
    carregarPacientes(lista_pacientes);
    carregarConsultas(lista_consultas);
    
    do {
        system("cls || clear");

        puts("\t+------------------------------+");
    	puts("\t|                              |");
    	puts("\t|  BEM VINDO(A) RECEPCIONISTA  |");
    	puts("\t|                              |");
    	puts("\t+------------------------------+");
    	puts("\t|       Deseja Gerenciar:      |");
    	puts("\t|                              |");
        puts("\t| 1. Pacientes                 |");
        puts("\t| 2. Consultas                 |");
        puts("\t| 3. Agenda Médica             |");
        puts("\t| 0. Voltar ao Login           |");
        puts("\t|                              |");
        puts("\t+------------------------------+");
        printf("\tDigite a opcão: ");
        scanf("%i", &op);

        switch(op) {
            case 1:
                menuPaciente(lista_pacientes);
                break;
            case 2:
                menuConsulta(lista_consultas);
                break;
            case 3:
            	puts("Funcionalidade de agenda médica removida para refatoração. Em andamento...\n");
            	system("pause");
                break;
            case 0:
                puts("Voltando ao menu de login...\n");
                //Salvar coisas no futuro
                system("pause");
                //return;
                break;
            default:
                puts("Entrada inválida, tente novamente");
                system("pause");
                break;
        }
    } while (op != 0);
    
    destruirLista(lista_pacientes, free);
    destruirLista(lista_consultas, free);
}

