//consulta.c 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "../../include/recepcionista/consulta.h"
#include "../../include/login/login.h"
#include "../../include/admin/admin.h"
#include "../../include/medico/Prontuario.h"



//Variáveis globais/controle

const int YELLOW = 14;
const int RESET = 7;
int ultimo_id_paciente = 0;
const char *arquivo_pacientes = "../../data/pacientes.txt";
int ultimo_id_consulta = 0;
const char *arquivo_consultas = "../../data/consultas.txt";
const char *arquivo_agenda = "../../data/agenda.txt";
int i;

char horarios_agenda[24][6] = {
    "07:00", "07:30", "08:00", "08:30", "09:00", "09:30",
    "10:00", "10:30", "11:00", "11:30", "12:00", "12:30",
    "13:00", "13:30", "14:00", "14:30", "15:00", "15:30",
    "16:00", "16:30", "17:00", "17:30", "18:00", "18:30"
};


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
    getchar();

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
    getchar();

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
        getchar();

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
        printf("\t|   Pacientes cadastrados: %d    |\n", totalPacientes);
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
        puts("\t|     Setas para navegar:       |");
        puts("\t| [^] Consultar Paciente        |");
        puts("\t| [<] Página Anterior           |");
        puts("\t| [>] Próxima Página            |");
        puts("\t| [v] Voltar ao Menu            |");
        puts("\t+-------------------------------+");
        char opcao = getch();

		if (opcao == 72) { // Consultar paciente
            consultarDadosPaciente(lista);
        }
        else if (opcao == 77) { // Próxima página
            if (pagina < totalPaginas - 1) {
                pagina++;
            } else {
            	puts("\t+-------------------------------+");
                puts("\t|  Você está na última página.  |");
                puts("\t+-------------------------------+");
                system("pause");
            }
        } else if (opcao == 75) { // Página anterior
            if (pagina > 0) {
                pagina--;
            } else {
            	puts("\t+-------------------------------+");
                puts("\t| Você está na primeira página. |");
                puts("\t+-------------------------------+");
                system("pause");
            }
        } else if (opcao == 80) {
            return; // Voltar ao menu
        }
    }
}


//Versão resumida
void imprimirPaciente(void *dado) {
    PACIENTE *paciente = (PACIENTE *)dado;
    printf("\tNúmero: %d\n", paciente->id_paciente);
    printf("\tNome: %s\n", paciente->nome_paciente);
    printf("\tTelefone: %s\n", paciente->telefone_paciente);
    // printf("Paciente impresso: ID=%d\n", paciente->id_paciente); // Pra teste
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
    getchar();

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
            getchar();

            printf("\tDigite a nova altura (m): ");
            scanf("%f", &paciente->altura_paciente);
            getchar();

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
    getchar();

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
    getchar();

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
            getchar();

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


bool compararPacientePorID(void *dado, void *chave) {
    PACIENTE *paciente = (PACIENTE *)dado;
    int *idChave = (int *)chave;
    // printf("Comparando ID: Paciente=%d, Chave=%d\n", paciente->id_paciente, *idChave); // Pra teste
    return paciente->id_paciente == *idChave;
}



//Funções do menu de Consulta/Consulta em Geral

void salvarConsultas(LISTA *lista) {
    FILE *arquivo = fopen(arquivo_consultas, "wb");
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

void cadastrarConsulta(LISTA *lista, LISTA *lista2) {
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

    printf("\n\tDigite o ID do paciente: ");
    scanf("%d", &novaConsulta.idpaciente_consulta);
    getchar();
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
	printf("\n\tDigite o ID do médico: ");
    scanf("%d", &novaConsulta.idmedico_consulta);
    getchar();
      switch (novaConsulta.idmedico_consulta) {
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
    printf("\n");
    system("pause\n");
    
    int valido = -1;
    do{
    system("cls || clear");
    imprimirCalendario(lista, lista2, novaConsulta.idmedico_consulta);
    printf("\n\tDigite uma data (dd/mm/aaaa): ");
    fgets(novaConsulta.data_consulta, sizeof(novaConsulta.data_consulta), stdin);
    novaConsulta.data_consulta[strcspn(novaConsulta.data_consulta, "\n")] = 0;

    sscanf(novaConsulta.data_consulta, "%d/%d/%d", &novaConsulta.dia_consulta, &novaConsulta.mes_consulta, &novaConsulta.ano_consulta);
	getchar();
	
	struct tm data = {0};
    data.tm_mday = novaConsulta.dia_consulta;
    data.tm_mon = novaConsulta.mes_consulta - 1; 
    data.tm_year = novaConsulta.ano_consulta - 1900;
    mktime(&data);
    int diaDaSemana = data.tm_wday;
    int consultas = buscarConsultasPorData(lista, novaConsulta.dia_consulta, novaConsulta.mes_consulta, novaConsulta.ano_consulta);
	valido = verificarData(lista2, novaConsulta.dia_consulta, novaConsulta.mes_consulta, novaConsulta.ano_consulta, diaDaSemana, consultas);
	}while(valido!=0);
	
	
	int max_consultas;
	char horario[6];
	char intervalo[6];
	NO *atual = lista2->inicio;
    while (atual != NULL) {
        DIADAAGENDA *agenda = atual->dado;
        if (agenda->dia_agenda == novaConsulta.dia_consulta && 
            agenda->mes_agenda == novaConsulta.mes_consulta && 
            agenda->ano_agenda == novaConsulta.ano_consulta) {
            max_consultas = agenda->max_consultas_agenda;
            strcpy(horario, agenda->primeiro_horario);
            strcpy(intervalo, agenda->intervalo_horario);
            break;
        }
        atual = atual->proximo;
    }
    
    int indice1 = buscarHorario(horarios_agenda, horario);
    int indice2 = buscarHorario(horarios_agenda, intervalo);
	puts("\n\tLista de horários:\n"); 
	for (i = indice1; i < max_consultas+indice1+2; i++) {
        if (i == indice2) {
            i++;
            i++;
        }
        if (i < max_consultas+indice1+2) {
        	int consultas = buscarConsultasPorDataEHorario(lista, novaConsulta.dia_consulta, novaConsulta.mes_consulta, novaConsulta.ano_consulta, horarios_agenda[i]);
            //printf("\t%d", consultas);
            if(consultas>=1){
            	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // Vermelho
            	printf("\t%s - Horário agendado\n", horarios_agenda[i]);
            	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset color to default
			}
			else{
            	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // Verde
            	printf("\t%s - Horario disponível\n", horarios_agenda[i]);
            	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset color to default
			}  
        }
    }
    printf("\n");
    
	
    printf("\tDigite o horário (hh:mm): ");
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
    getchar();

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
        getchar();

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
        cadastrarConsulta(lista, lista2);
    }
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
        puts("\t|     Setas para navegar:       |");
        puts("\t| [^] Consultar Consulta        |");
        puts("\t| [<] Página Anterior           |");
        puts("\t| [>] Próxima Página            |");
        puts("\t| [v] Voltar ao Menu            |");
        puts("\t+-------------------------------+");
        char opcao = getch();

        if (opcao == 72) { // Consultar consulta
            consultarDadosConsulta(lista);
        }
        else if (opcao == 77) { // Próxima página
            if (pagina < totalPaginas - 1) {
                pagina++;
            } else {
                puts("\t+-------------------------------+");
                puts("\t|  Você está na última página.  |");
                puts("\t+-------------------------------+");
                system("pause");
            }
        } else if (opcao == 75) { // Página anterior
            if (pagina > 0) {
                pagina--;
            } else {
                puts("\t+-------------------------------+");
                puts("\t| Você está na primeira página. |");
                puts("\t+-------------------------------+");
                system("pause");
            }
        } else if (opcao == 80) {
            return; // Voltar ao menu
        }
    }
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
    getchar();

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
    getchar();

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

void consultarDadosConsulta(LISTA *lista) {
    int id;
    printf("\tDigite o número da consulta: ");
    scanf("%d", &id);
    getchar();

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
        getchar();

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

bool compararConsultaPorID(void *dado, void *chave) {
    CONSULTA *consulta = (CONSULTA *)dado;
    int *idChave = (int *)chave;
    // printf("Comparando ID: Consulta=%d, Chave=%d\n", consulta->id_consulta, *idChave); // Pra teste
    return consulta->id_consulta == *idChave;
}



// Funções do menu de agenda medica/agenda em geral

void salvarAgenda(LISTA *lista) {
    FILE *arquivo = fopen(arquivo_agenda, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }

    NO *atual = lista->inicio;
    while (atual != NULL) {
        DIADAAGENDA *agenda = (DIADAAGENDA *)atual->dado;
        fprintf(arquivo, "%s,%d,%s,%s\n", agenda->data_agenda, agenda->max_consultas_agenda,
                agenda->primeiro_horario, agenda->intervalo_horario);
        atual = atual->proximo;
    }

    fclose(arquivo);
    printf("Lista salva com sucesso!\n");
}

void carregarAgenda(LISTA *lista) {
    FILE *arquivo = fopen(arquivo_agenda, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para carregar.\n");
        return;
    }

    DIADAAGENDA agenda;
    while (fscanf(arquivo, "%10[^,],%d,%5[^,],%5[^,\n]\n", agenda.data_agenda,
                  &agenda.max_consultas_agenda, agenda.primeiro_horario,
                  agenda.intervalo_horario) == 4) {
        sscanf(agenda.data_agenda, "%d/%d/%d", &agenda.dia_agenda, &agenda.mes_agenda, &agenda.ano_agenda);
        inserirNoFim(lista, &agenda, sizeof(DIADAAGENDA));
    }

    fclose(arquivo);
    printf("Lista carregada com sucesso!\n");
}

void cadastrarDia(LISTA *lista) {
    DIADAAGENDA agenda;
    
    printf("Digite a data (DD/MM/AAAA): ");
    scanf("%10s", agenda.data_agenda);
    getchar();
    sscanf(agenda.data_agenda, "%d/%d/%d", &agenda.dia_agenda, &agenda.mes_agenda, &agenda.ano_agenda);
    
    // Atribuições padrão
    agenda.max_consultas_agenda = 16;
    strcpy(agenda.primeiro_horario, "08:00");
    strcpy(agenda.intervalo_horario, "12:00");

    inserirNoFim(lista, &agenda, sizeof(DIADAAGENDA));
    printf("Dia cadastrado com sucesso!\n");
    system("pause");
    salvarAgenda(lista);
}

void listarDias(LISTA *lista) {
    listarElementos(lista, imprimirDias);
    system("pause");
}

void imprimirDias(void *dado) {
    DIADAAGENDA *agenda = (DIADAAGENDA *)dado;
    printf("Data: %s, Máximo de Consultas Diárias: %d, Primeiro Horário: %s,  Horário do Intervalo: %s\n",
           agenda->data_agenda, agenda->max_consultas_agenda,
           agenda->primeiro_horario, agenda->intervalo_horario);
}

bool editarDia(LISTA *lista) {
    char data[11];
    printf("Digite a data do dia a ser editado (DD/MM/AAAA): ");
    scanf("%10s", data);
    getchar();

    DIADAAGENDA *agenda = (DIADAAGENDA *)buscarElemento(lista, compararDias, data);
    
    if (agenda == NULL) {
        printf("Dia não encontrado!\n");
        return false;
    }

    printf("Editando o dia: %s\n", agenda->data_agenda);
    
    printf("Novo máximo de consultas diárias: ");
    scanf("%d", &agenda->max_consultas_agenda);
    getchar();
    
    printf("Novo primeiro horário (HH:MM): ");
    scanf("%5s", agenda->primeiro_horario);
    getchar();
    
    printf("Novo horário de intervalo (HH:MM): ");
    scanf("%5s", agenda->intervalo_horario);
    getchar();

    printf("Dia editado com sucesso!\n");
    system("pause");
    salvarAgenda(lista);
    return true;
}

bool removerDia(LISTA *lista) {
    char data[11];
    printf("Digite a data do dia a ser removido (DD/MM/AAAA): ");
    scanf("%10s", data);

    DIADAAGENDA *agenda = (DIADAAGENDA *)buscarElemento(lista, compararDias, data);
    
    if (agenda == NULL) {
        printf("Dia não encontrado!\n");
        return false;
    }

    // Implementação da remoção do nó da lista
    NO *atual = lista->inicio;
    NO *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(((DIADAAGENDA *)atual->dado)->data_agenda, data) == 0) {
            if (anterior == NULL) {
                lista->inicio = atual->proximo; // Remover o primeiro nó
            } else {
                anterior->proximo = atual->proximo; // Remover o nó do meio ou do final
            }
            free(atual->dado);
            free(atual);
            lista->tamanho--;
            printf("Dia removido com sucesso!\n");
            system("pause");
            salvarAgenda(lista);
            return true;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return false;
}

bool compararDias(void *dado, void *chave) {
    DIADAAGENDA *agenda = (DIADAAGENDA *)dado;
    return strcmp(agenda->data_agenda, (char *)chave) == 0;
}

void criarDiasDoMes(LISTA *lista) {
    int dia, mes, ano;
    
    printf("Digite o mês (MM): ");
    scanf("%d", &mes);
    printf("Digite o ano (AAAA): ");
    scanf("%d", &ano);

    int dias = diasNoMes(mes, ano);
    bool existe = false;

    for (dia = 1; dia <= dias; dia++) {
        if (diaExistente(lista, dia, mes, ano)) {
            existe = true;
            break;
        }
    }

    if (existe) {
        printf("Os dias do mês %02d/%04d já existem na lista!\n", mes, ano);
        return;
    }

    for (dia = 1; dia <= dias; dia++) {
        DIADAAGENDA agenda;
        snprintf(agenda.data_agenda, sizeof(agenda.data_agenda), "%02d/%02d/%04d", dia, mes, ano);
        agenda.dia_agenda = dia;
        agenda.mes_agenda = mes;
        agenda.ano_agenda = ano;
        agenda.max_consultas_agenda = 16; // Exemplo de valor padrão
        strcpy(agenda.primeiro_horario, "08:00");
        strcpy(agenda.intervalo_horario, "12:00");

        inserirNoFim(lista, &agenda, sizeof(DIADAAGENDA));
    }
    printf("Dias do mês %02d/%04d criados com sucesso!\n", mes, ano);
    system("pause");
    salvarAgenda(lista);
}

int diasNoMes(int mes, int ano) {
    if (mes == 2) { // Fevereiro
        return (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0)) ? 29 : 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return 30;
    }
    return 31; // Meses com 31 dias
}

bool diaExistente(LISTA *lista, int dia, int mes, int ano) {
    char data_agenda[11];
    snprintf(data_agenda, sizeof(data_agenda), "%02d/%02d/%04d", dia, mes, ano);
    return buscarElemento(lista, compararDias, data_agenda) != NULL;
}

void listarMesesCadastrados(LISTA *lista) {
	int mes, ano;
    NO *atual = lista->inicio;
    int contagem[12][2][10] = {{{0}}}; // [mes][quantidade][ano]

    // Contar dias por mês e ano
    while (atual != NULL) {
        DIADAAGENDA *agenda = (DIADAAGENDA *)atual->dado;
        int mes = agenda->mes_agenda - 1; // Ajuste para índice 0
        int ano = agenda->ano_agenda;

        contagem[mes][0][ano % 10]++; // Incrementar contagem para o ano
        atual = atual->proximo;
    }

    printf("Meses cadastrados:\n");
    for (mes = 0; mes < 12; mes++) {
        for (ano = 0; ano < 10; ano++) { // Considerando os últimos 10 anos
            if (contagem[mes][0][ano] > 0) {
                printf("%02d/%d: %d dia(s)\n", mes + 1, ano + 2020, contagem[mes][0][ano]); // Ajuste do ano base
            }
        }
    }
    system("pause");
}

void removerDiasDoMes(LISTA *lista) {
    int mes, ano;
    
    printf("Digite o mês (MM): ");
    scanf("%d", &mes);
    printf("Digite o ano (AAAA): ");
    scanf("%d", &ano);

    NO *atual = lista->inicio;
    NO *anterior = NULL;

    while (atual != NULL) {
        DIADAAGENDA *agenda = (DIADAAGENDA *)atual->dado;
        if (agenda->mes_agenda == mes && agenda->ano_agenda == ano) {
            if (anterior == NULL) {
                lista->inicio = atual->proximo; // Remover o primeiro nó
            } else {
                anterior->proximo = atual->proximo; // Remover o nó do meio ou do final
            }
            free(atual->dado);
            NO *temp = atual;
            atual = atual->proximo; // Avança o ponteiro
            free(temp);
            lista->tamanho--;
        } else {
            anterior = atual;
            atual = atual->proximo;
        }
    }
    printf("Todos os dias do mês %02d/%04d removidos com sucesso!\n", mes, ano);
    system("pause");
    salvarAgenda(lista);
}

void AgendaEmDia(LISTA *lista) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    int mesAtual = tm_info->tm_mon + 1; // Mês atual (0-11, então adicionamos 1)
    int anoAtual = tm_info->tm_year + 1900; // Ano atual (anos desde 1900)

    // Criar dias para o mês atual e os próximos 3 meses
    for (i = 0; i < 4; i++) {
        int mes = mesAtual + i;
        int ano = anoAtual;

        // Ajustar para o próximo ano se necessário
        if (mes > 12) {
            mes -= 12;
            ano++;
        }

        // Verificar se os dias do mês já existem
        if (!diaExistente(lista, 1, mes, ano)) {
            criarMeses(lista, mes, ano);
        }
    }
}

void criarMeses(LISTA *lista, int mes, int ano) {
    int dia;

    int dias = diasNoMes(mes, ano);
    bool existe = false;

    // Verificar se algum dia do mês já existe
    for (dia = 1; dia <= dias; dia++) {
        if (diaExistente(lista, dia, mes, ano)) {
            existe = true;
            break;
        }
    }

    if (existe) {
        printf("Os dias do mês %02d/%04d já existem na lista!\n", mes, ano);
        return;
    }

    // Criar os dias do mês
    for (dia = 1; dia <= dias; dia++) {
        DIADAAGENDA agenda;
        snprintf(agenda.data_agenda, sizeof(agenda.data_agenda), "%02d/%02d/%04d", dia, mes, ano);
        agenda.dia_agenda = dia;
        agenda.mes_agenda = mes;
        agenda.ano_agenda = ano;
        agenda.max_consultas_agenda = 16; // Exemplo de valor padrão
        strcpy(agenda.primeiro_horario, "08:00");
        strcpy(agenda.intervalo_horario, "12:00");

        inserirNoFim(lista, &agenda, sizeof(DIADAAGENDA));
    }
    printf("Dias do mês %02d/%04d criados com sucesso!\n", mes, ano);

    // Chama a função para salvar a agenda
    salvarAgenda(lista);
}

void imprimirCalendario(LISTA *listaC, LISTA *listaA, int id_medico) {
    int dia;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    
    int mes = tm_info->tm_mon + 1; // Mês atual
    int ano = tm_info->tm_year + 1900; // Ano atual

    char tecla;
    
    do {
        struct tm primeiraData = {0};
        primeiraData.tm_year = ano - 1900; 
        primeiraData.tm_mon = mes - 1;      
        primeiraData.tm_mday = 1;           

        mktime(&primeiraData);
        int diaDaSemana = primeiraData.tm_wday; 
        int dias = diasNoMes(mes, ano); 
        
        system("cls || clear");
	
        printf("\n\t   Calendário de %02d/%04d\n", mes, ano);
        printf("\tDom Seg Ter Qua Qui Sex Sab\n");
        printf("\t");
        
        for ( i = 0; i < diaDaSemana; i++) {
            printf("    "); // Imprime espaços em branco antes do primeiro dia do mês
        }
		
        for (dia = 1; dia <= dias; dia++) {
            int consultas = buscarConsultasPorData(listaC, dia, mes, ano); // Busca consultas
            int semana = (diaDaSemana + dia - 1) % 7; // Calcula o dia da semana para o dia atual            
            definirCor(listaA, dia, mes, ano, semana, consultas); // Define a cor antes de imprimir
            
            printf("%2d  ", dia); // Imprime o dia
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset color to default
            
            if ((dia + diaDaSemana) % 7 == 0) {
                printf("\n");
                printf("\t"); // Nova linha a cada semana (7 dias)
            }
        }
        printf("\n");

        printf("\tSetas para mudar os meses.\n");
        printf("\tPressione '0' para continuar.\n");

        tecla = _getch(); // Captura a tecla pressionada
        if (tecla == 75) { // Seta esquerda
            mes--;
            if (mes < 1) {
                mes = 12;
                ano--;
            }
        } else if (tecla == 77) { // Seta direita
            mes++;
            if (mes > 12) {
                mes = 1;
                ano++;
            }
        }
        if(tecla!= '0'){
        	system("cls || clear");
		}
    } while (tecla != '0');
}

void definirCor(LISTA *lista, int dia, int mes, int ano, int diaDaSemana, int consultas) {
    NO *atual = lista->inicio;
    int max_consultas = 0; // Inicializa max_consultas

    // Procura o dia da agenda correspondente
    while (atual != NULL) { // Enquanto houver nós na lista
        DIADAAGENDA *agenda = (DIADAAGENDA *)atual->dado;
        if (agenda->dia_agenda == dia && 
            agenda->mes_agenda == mes && 
            agenda->ano_agenda == ano) {
            max_consultas = agenda->max_consultas_agenda; // Atribui o valor
            break; // Sai do loop, pois já encontrou a agenda
        }
        atual = atual->proximo; // Avança para o próximo nó
    }
    
    // Define a cor do texto com base no dia da semana e no número de consultas
    if (diaDaSemana == 0 || diaDaSemana == 6) { // Sábado e Domingo
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // Amarelo
    } else {
        if (consultas < max_consultas) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // Verde
        } else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // Vermelho
        }
    }
}

int buscarConsultasPorData(LISTA *lista, int dia, int mes, int ano) {
    int contador = 0;
    NO *atual = lista->inicio; // Começa do início da lista

    while (atual != NULL) { // Enquanto houver nós na lista
        CONSULTA *consulta = (CONSULTA *)atual->dado; // Cast do dado para CONSULTA
        if (consulta->dia_consulta == dia && 
            consulta->mes_consulta == mes && 
            consulta->ano_consulta == ano) {
            contador++; // Incrementa o contador se a data coincidir
        }
        atual = atual->proximo; // Avança para o próximo nó
    }
    
    return contador; // Retorna o número de consultas para a data especificada
}

int buscarConsultasPorDataEHorario(LISTA *lista, int dia, int mes, int ano, char *horario) {
    int contador = 0;
    NO *atual = lista->inicio; // Começa do início da lista

    while (atual != NULL) { // Enquanto houver nós na lista
        CONSULTA *consulta = (CONSULTA *)atual->dado; // Cast do dado para CONSULTA
        // Verifica se a data e o horário coincidem
        if (consulta->dia_consulta == dia && 
            consulta->mes_consulta == mes && 
            consulta->ano_consulta == ano &&
            strcmp(consulta->hora_consulta, horario) == 0) {
            contador++; // Incrementa o contador se a data e o horário coincidirem
        }
        atual = atual->proximo; // Avança para o próximo nó
    }
    
    return contador; // Retorna o número de consultas para a data e horário especificados
}

int verificarData(LISTA *lista, int dia, int mes, int ano, int diaDaSemana, int consultas) {
    NO *atual = lista->inicio;
    int max_consultas = 0; // Inicializa max_consultas

    // Procura o dia da agenda correspondente
    while (atual != NULL) { // Enquanto houver nós na lista
        DIADAAGENDA *agenda = (DIADAAGENDA *)atual->dado;
        if (agenda->dia_agenda == dia && 
            agenda->mes_agenda == mes && 
            agenda->ano_agenda == ano) {
            max_consultas = agenda->max_consultas_agenda; // Atribui o valor
            break; // Sai do loop, pois já encontrou a agenda
        }
        atual = atual->proximo; // Avança para o próximo nó
    }
    
    if (diaDaSemana == 0 || diaDaSemana == 6) { // Sábado e Domingo
        printf("\n\tSem expediente no final de semana.\n\t      Escolha outra data.\n");
        system("pause");
        return -1;
    } else {
        if (consultas < max_consultas) {
            printf("\n\tDia com horário disponível.\n");
            printf("\tAgendar um horário? (S/N)? ");
            int op;
            scanf("%c",&op);
            getchar();
            if(op == 'S'||op == 's')
            return 0;
            else
            return -1;
        } else {
            printf("\n\tDia com agenda cheia.\n        Escolha outra data.\n");
            system("pause");
            return -1;
        }
    }
}

int buscarHorario(const char horarios_agenda[24][6], const char* horario_buscado) {
    for ( i = 0; i < 24; i++) {
        if (strcmp(horarios_agenda[i], horario_buscado) == 0) {
            return i; // Retorna o índice se o horário for encontrado
        }
    }
    return -1; // Retorna -1 se o horário não for encontrado
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void imprimirCalendario2(LISTA *listaC, LISTA *listaA, int id_medico) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    
    int dia = tm_info->tm_mday; // Dia atual
    int mes = tm_info->tm_mon + 1; // Mês atual
    int ano = tm_info->tm_year + 1900; // Ano atual

    char tecla;
    
    do {
        struct tm primeiraData = {0};
        primeiraData.tm_year = ano - 1900; 
        primeiraData.tm_mon = mes - 1;      
        primeiraData.tm_mday = dia; // Usa o dia atual

        mktime(&primeiraData);
        int diaDaSemana = primeiraData.tm_wday; // Dia da semana do primeiro dia do mês

        // Ajuste para a semana (de domingo a sábado)
        int inicio_semana = dia - diaDaSemana; // Dia de início da semana
        if (inicio_semana < 1) {
            inicio_semana = 1; // Não pode começar antes do primeiro dia do mês
        }

        system("cls || clear");
	
        printf("\n\t   Calendário Semanal de %02d/%04d\n", mes, ano);
        printf("\tDom Seg Ter Qua Qui Sex Sab\n");

        // Imprime os dias da semana
        for (i = 0; i < 7; i++) {
            int dia_atual = inicio_semana + i;
            if (dia_atual > diasNoMes(mes, ano)) {
                printf("      "); // Espaço em branco se o dia ultrapassar o mês
            } else {
                int consultas = buscarConsultasPorData(listaC, dia_atual, mes, ano); // Busca consultas

                int max_consultas = 0;
                NO *atual = listaA->inicio;
                while (atual != NULL) {
                    DIADAAGENDA *agenda = atual->dado;
                    if (agenda->dia_agenda == dia_atual && 
                        agenda->mes_agenda == mes && 
                        agenda->ano_agenda == ano) {
                        max_consultas = agenda->max_consultas_agenda;
                        break;
                    }
                    atual = atual->proximo;
                }
                
                // Imprime o dia
                printf("%2d     ", dia_atual); 
            }
        }

        printf("\n"); // Nova linha antes de imprimir as relações

        // Imprime a relação de consultas
        for ( i = 0; i < 7; i++) {
            int dia_atual = inicio_semana + i;
            if (dia_atual > diasNoMes(mes, ano)) {
                printf("      "); // Espaço em branco se o dia ultrapassar o mês
            } else {
                int consultas = buscarConsultasPorData(listaC, dia_atual, mes, ano); // Busca consultas

                int max_consultas = 0;
                NO *atual = listaA->inicio;
                while (atual != NULL) {
                    DIADAAGENDA *agenda = atual->dado;
                    if (agenda->dia_agenda == dia_atual && 
                        agenda->mes_agenda == mes && 
                        agenda->ano_agenda == ano) {
                        max_consultas = agenda->max_consultas_agenda;
                        break;
                    }
                    atual = atual->proximo;
                }
                
                // Imprime a relação de consultas
                printf("%d/%d  ", consultas, max_consultas); 
            }
        }
        
        printf("\n");
        printf("\tSetas para mudar a semana.\n");
        printf("\tPressione '0' para continuar.\n");

        tecla = _getch(); // Captura a tecla pressionada
        if (tecla == 75) { // Seta esquerda
            inicio_semana -= 7; // Muda para a semana anterior
        } else if (tecla == 77) { // Seta direita
            inicio_semana += 7; // Muda para a próxima semana
        }
        if(tecla != '0'){
        	system("cls || clear");
		}
    } while (tecla != '0');
}


//Funções de menu

void setColor(int textColor){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, textColor);
}

char controlaEntrada(int *destaque, int num_opcoes) {
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

void menuRecepcionista() {
	int destaque = 0;
    int escolha = -1;
    int espaco_max = 22;
    int i;
    
        const char *opcoes[] = {
        "Pacientes",
        "Consultas",
        "Agenda Medica",
        "Voltar ao Login"
    };
    
    const char *opcoesDestacadas[] = {
        "> Pacientes <",
        "> Consultas <",
        "> Agenda Medica <",
        "> Voltar ao Login <"
    };
    int num_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);
    
    LISTA *lista_pacientes = criarLista();
    LISTA *lista_consultas = criarLista();
    LISTA *lista_agenda = criarLista();
    carregarPacientes(lista_pacientes);
    carregarConsultas(lista_consultas);
    carregarAgenda(lista_agenda);
    AgendaEmDia(lista_agenda);
    
    do {
        system("cls || clear");


        puts("\t\t+------------------------------+");
    	puts("\t\t|                              |");
    	puts("\t\t|  BEM VINDO(A) RECEPCIONISTA  |");
    	puts("\t\t|                              |");
    	puts("\t\t+------------------------------+");
    	puts("\t\t|       Deseja Gerenciar:      |");
    	puts("\t\t|                              |");
    	//Inicio do laço para imprimir opções dinamicamente
			for (i = 0; i < num_opcoes; i++) {
		        if (i == destaque) {
		        	printf("\t\t|");
		        	setColor(YELLOW);
		            printf("      %-*s  ", espaco_max, opcoesDestacadas[i]); // Imprime opção destacada
		            setColor(RESET);
		            printf("|\n");
		        } else {
		        	setColor(RESET);
		            printf("\t\t|      %-*s  |\n", espaco_max, opcoes[i]); // Imprime opção normal
		        }
		    }
		//Fim do laço para imprimir opções dinamicamente
        puts("\t\t|                              |");
        puts("\t\t+------------------------------+");
        puts("\t\t      Setas para navegar.\n\t\t     Enter para selecionar.");

		escolha = controlaEntrada(&destaque, num_opcoes);
        switch(escolha) {
            case 0:
                menuPaciente(lista_pacientes);
                break;
            case 1:
                menuConsulta(lista_consultas, lista_agenda);
                break;
            case 2:
            	menuAgendaMedica(lista_agenda, lista_consultas);
                break;
            case 3:
                puts("Voltando ao menu de login...\n");
                //Salvar coisas no futuro
                system("pause");
                //return;
                break;
            default:
                break;
        }
        escolha = -1;
    } while (escolha != 0);
    
    destruirLista(lista_pacientes, free);
    destruirLista(lista_consultas, free);
    destruirLista(lista_agenda, free);
}

void menuPaciente(LISTA *lista) {
    int destaque = 0;
    int escolha = -1;
    int espaco_max = 27;
    int i;
    
    const char *opcoes[] = {
        "Cadastrar Paciente",
        "Listar Pacientes",
        "Editar Paciente",
        "Remover Paciente",
        "Voltar ao menu anterior"
    };
    
    const char *opcoesDestacadas[] = {
        "> Cadastrar Paciente <",
        "> Listar Pacientes <",
        "> Editar Paciente <",
        "> Remover Paciente <",
        "> Voltar ao menu anterior <"
    };
    int num_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);

    do {
        system("cls || clear");

        puts("\t\t+-------------------------------+");
        puts("\t\t|                               |");
        puts("\t\t|       MENU DE PACIENTES       |");
        puts("\t\t|                               |");
        puts("\t\t+-------------------------------+");
        puts("\t\t|       Deseja Realizar:        |");
        puts("\t\t|                               |");
        //Inicio do laço para imprimir opções dinamicamente
			for (i = 0; i < num_opcoes; i++) {
		        if (i == destaque) {
		        	printf("\t\t|");
		        	setColor(YELLOW);
		            printf("  %-*s  ", espaco_max, opcoesDestacadas[i]); // Imprime opção destacada
		            setColor(RESET);
		            printf("|\n");
		        } else {
		        	setColor(RESET);
		            printf("\t\t|  %-*s  |\n", espaco_max, opcoes[i]); // Imprime opção normal
		        }
		    }
		//Fim do laço para imprimir opções dinamicamente
        puts("\t\t|                               |");
        puts("\t\t+-------------------------------+");
        puts("\t\t      Setas para navegar.\n\t\t     Enter para selecionar.");

		escolha = controlaEntrada(&destaque, num_opcoes);
        switch(escolha) {
            case 0:
                cadastrarPaciente(lista);
                break;
            case 1:
                menuListaPacientes(lista);
                break;
            case 2:
                editarPaciente(lista);
                break;
            case 3:
                removerPaciente(lista);
                break;
            case 4:
                system("cls || clear");
                puts("Voltando ao menu anterior...\n");
                system("pause");
                return;
                break;
            default:
                break;
        }
        escolha = -1;
    } while (escolha != 0);
}

void menuListaPacientes(LISTA *lista) {
    int destaque = 0;
    int escolha = -1;
    int espaco_max = 27;
    int i;
    
    const char *opcoes[] = {
        "Pesquisar Paciente",
        "Exibir Todos Pacientes",
        "Voltar ao Menu Anterior"
    };
    
    const char *opcoesDestacadas[] = {
        "> Pesquisar Paciente <",
        "> Exibir Todos Pacientes <",
        "> Voltar ao Menu Anterior <"
    };
    int num_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);

   	do {
        system("cls || clear"); 
        
        puts("\t\t+--------------------------------+");
        puts("\t\t|                                |");
        puts("\t\t|   MENU DE LISTA DE PACIENTES   |");
        puts("\t\t|                                |");
        puts("\t\t+--------------------------------+");
        puts("\t\t|       Deseja Realizar:         |");
        puts("\t\t|                                |");
        //Inicio do laço para imprimir opções dinamicamente
			for (i = 0; i < num_opcoes; i++) {
		        if (i == destaque) {
		        	printf("\t\t|");
		        	setColor(YELLOW);
		            printf("   %-*s  ", espaco_max, opcoesDestacadas[i]); // Imprime opção destacada
		            setColor(RESET);
		            printf("|\n");
		        } else {
		        	setColor(RESET);
		            printf("\t\t|   %-*s  |\n", espaco_max, opcoes[i]); // Imprime opção normal
		        }
		    }
		//Fim do laço para imprimir opções dinamicamente	   
        puts("\t\t|                                |");
        puts("\t\t+--------------------------------+");
        puts("\t\t      Setas para navegar.\n\t\t     Enter para selecionar.");

		escolha = controlaEntrada(&destaque, num_opcoes);
        switch (escolha) {
            case 0:
                //pesquisarPacientes(lista);
                break;
            case 1:
                listarPacientes(lista);
                break;
            case 2:
                system("cls || clear");
                puts("Voltando ao menu anterior...\n");
                system("pause");
                return;
            default:
                break;
        }
        escolha = -1;
    }while (escolha != 0);
}

void menuConsulta(LISTA *lista, LISTA *lista2) {
    int destaque = 0;
    int escolha = -1;
    int espaco_max = 27;
    int i;
    
    const char *opcoes[] = {
        "Cadastrar Consulta",
        "Listar Consultas",
        "Editar Consulta",
        "Remover Consulta",
        "Voltar ao menu anterior"
    };
    
    const char *opcoesDestacadas[] = {
        "> Cadastrar Consulta <",
        "> Listar Consultas <",
        "> Editar Consulta <",
        "> Remover Consulta <",
        "> Voltar ao menu anterior <"
    };
    int num_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);

    do {
        system("cls || clear");

        puts("\t\t+-------------------------------+");
        puts("\t\t|                               |");
        puts("\t\t|       MENU DE CONSULTAS       |");
        puts("\t\t|                               |");
        puts("\t\t+-------------------------------+");
        puts("\t\t|       Deseja Realizar:        |");
        puts("\t\t|                               |");
        //Inicio do laço para imprimir opções dinamicamente
			for (i = 0; i < num_opcoes; i++) {
		        if (i == destaque) {
		        	printf("\t\t|");
		        	setColor(YELLOW);
		            printf("  %-*s  ", espaco_max, opcoesDestacadas[i]); // Imprime opção destacada
		            setColor(RESET);
		            printf("|\n");
		        } else {
		        	setColor(RESET);
		            printf("\t\t|  %-*s  |\n", espaco_max, opcoes[i]); // Imprime opção normal
		        }
		    }
		//Fim do laço para imprimir opções dinamicamente        
        puts("\t\t|                               |");
        puts("\t\t+-------------------------------+");
        puts("\t\t      Setas para navegar.\n\t\t     Enter para selecionar.");

		escolha = controlaEntrada(&destaque, num_opcoes);
        switch(escolha) {
            case 0:
                cadastrarConsulta(lista, lista2);
                break;
            case 1:
                listarConsultas(lista);
                break;
            case 2:
                editarConsulta(lista);
                break;
            case 3:
                removerConsulta(lista);
                break;
            case 4:
               	system("cls || clear");
                puts("Voltando ao menu anterior...\n");
                system("pause");
                return;
                break;
            default:
                break;
        }
        escolha = -1;
    } while (escolha != 0);
}

void menuAgendaMedica(LISTA *lista, LISTA *lista2) {
    int destaque = 0;
    int escolha = -1;
    int espaco_max = 27;
    int i;
    int id_medico = 1;
    
    const char *opcoes[] = {
    	"Calendario Semanal",
        "Cadastrar Dia",
        "Listar Dias",
        "Editar Dia",
        "Remover Dia",
        "Criar mês",
    	"Listar meses",
    	"Remover mês",
        "Voltar ao menu anterior"
    };
    
    const char *opcoesDestacadas[] = {
    	"> Calendario Semanal <",
        "> Cadastrar Dia <",
        "> Listar Dias <",
        "> Editar Dia <",
        "> Remover Dia <",
        "> Criar mês <",
    	"> Listar meses <",
    	"> Remover mês <",
        "> Voltar ao menu anterior <"
    };
    int num_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);

    do {
        system("cls || clear");

        puts("\t\t+-------------------------------+");
        puts("\t\t|                               |");
        puts("\t\t|     MENU DE AGENDA MEDICA     |");
        puts("\t\t|                               |");
        puts("\t\t+-------------------------------+");
        puts("\t\t|       Deseja Realizar:        |");
        puts("\t\t|                               |");
        //Inicio do laço para imprimir opções dinamicamente
			for (i = 0; i < num_opcoes; i++) {
		        if (i == destaque) {
		        	printf("\t\t|");
		        	setColor(YELLOW);
		            printf("  %-*s  ", espaco_max, opcoesDestacadas[i]); // Imprime opção destacada
		            setColor(RESET);
		            printf("|\n");
		        } else {
		        	setColor(RESET);
		            printf("\t\t|  %-*s  |\n", espaco_max, opcoes[i]); // Imprime opção normal
		        }
		    }
		//Fim do laço para imprimir opções dinamicamente        
        puts("\t\t|                               |");
        puts("\t\t+-------------------------------+");
        puts("\t\t      Setas para navegar.\n\t\t     Enter para selecionar.");

		escolha = controlaEntrada(&destaque, num_opcoes);
        switch(escolha) {
            case 1:
                cadastrarDia(lista);
                break;
            case 2:
                listarDias(lista);
                break;
            case 3:
                editarDia(lista);
                break;
            case 4:
                removerDia(lista);
                break;
            case 5:
                criarDiasDoMes(lista);
                break;
            case 6:
                listarMesesCadastrados(lista);
                break;
            case 7:
                removerDiasDoMes(lista);
                break;
            case 0:
                imprimirCalendario2(lista, lista2, id_medico);
                break;
            case 8:
               	system("cls || clear");
                puts("Voltando ao menu anterior...\n");
                system("pause");
                return;
                break;
            default:
                break;
        }
        escolha = -1;
    } while (escolha != 0);;
}



