#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "consultas.h"

int i;
static int next_consulta_id = 1;  


const char* buscarNomePaciente(int paciente_id) {
    if (paciente_id == 1) return "João Silva";
    if (paciente_id == 2) return "Maria Oliveira";
    return "Paciente Desconhecido";
}

const char* buscarNomeMedico(int medico_id) {
    if (medico_id == 1) return "Dr. Carlos Mendes";
    if (medico_id == 2) return "Dra. Ana Costa";
    return "Médico Desconhecido";
}

ListaConsultas* criarListaConsultas() {
    ListaConsultas *lista = (ListaConsultas*)malloc(sizeof(ListaConsultas));
    lista->inicio = NULL;
    return lista;
}

void adicionarConsulta(ListaConsultas *lista, Consulta consulta) {
    NoConsulta *novoNo = (NoConsulta*)malloc(sizeof(NoConsulta));
    novoNo->consulta = consulta;
    novoNo->proximo = lista->inicio;
    lista->inicio = novoNo;
}

void listarConsultas(ListaConsultas *lista) {
    NoConsulta *atual = lista->inicio;
    if (atual == NULL) {
        printf("Nenhuma consulta agendada.\n");
        return;
    }
    printf("+---------------------------------------------------------------------+\n");
    printf("|                        LISTA DE CONSULTAS                          |\n");
    printf("+---------------------------------------------------------------------+\n");

    while (atual != NULL) {
        printf("| ID: %d\n", atual->consulta.id);
        printf("| Paciente ID: %d\n", atual->consulta.paciente_id);
        printf("| Médico ID: %d\n", atual->consulta.medico_id);
        printf("| Data: %s\n", atual->consulta.data);
        printf("| Horário: %s\n", atual->consulta.horario);
        printf("| Tipo de Atendimento: %s\n", atual->consulta.tipoAtendimento);
        printf("| Status: %s\n", atual->consulta.status);
        printf("+---------------------------------------------------------------------+\n");
        atual = atual->proximo;
    }
}

void salvarConsultasEmArquivo(ListaConsultas *lista) {
    FILE *arquivo = fopen("consultas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para gravação.\n");
        return;
    }

    NoConsulta *atual = lista->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "ID:%d\nPaciente ID:%d\nMedico ID:%d\nData:%s\nHorario:%s\nTipo de Atendimento:%s\nStatus:%s\n\n",
                atual->consulta.id,
                atual->consulta.paciente_id,
                atual->consulta.medico_id,
                atual->consulta.data,
                atual->consulta.horario,
                atual->consulta.tipoAtendimento,
                atual->consulta.status);
        atual = atual->proximo;
    }

    fclose(arquivo);
    printf("Consultas salvas com sucesso.\n");
}

void mostrarCalendario(int mes, int ano) {
    printf("\nCalendário - %02d/%d\n", mes, ano);
    printf("+-------------------------------+\n");
    printf("| Dom | Seg | Ter | Qua | Qui | Sex | Sab |\n");
    printf("+-------------------------------+\n");

    for (i = 1; i <= 30; i++) {
        if (i == 1) {
            printf("| ");
        }

        if (i % 7 == 0) {
            printf("\033[31m%2d \033[0m| ", i); // Vermelho (Sábado)
        } else if (i % 7 == 1) {
            printf("\033[31m%2d \033[0m| ", i); // Vermelho (Domingo)
        } else if (i == 10 || i == 20) {
            printf("\033[33m%2d \033[0m| ", i); // Laranja (Agenda cheia)
        } else {
            printf("\033[32m%2d \033[0m| ", i); // Verde (Livre)
        }
        if (i % 7 == 0) {
            printf("\n");
        }
    }
    printf("\n+-------------------------------+\n");
}

void agendarConsulta(ListaConsultas *lista, int paciente_id, int medico_id) {
    Consulta c;
    c.id = next_consulta_id++;
    
    printf("Digite o ID do Paciente: ");
    scanf("%d", &c.paciente_id);
    printf("Digite o ID do Médico: ");
    scanf("%d", &c.medico_id);
    

    const char* nomePaciente = buscarNomePaciente(c.paciente_id);
    const char* nomeMedico = buscarNomeMedico(c.medico_id);
    printf("Paciente: %s\n", nomePaciente);
    printf("Médico: %s\n", nomeMedico);

    int mes, ano;
    printf("Digite o mês (1-12): ");
    scanf("%d", &mes);
    printf("Digite o ano: ");
    scanf("%d", &ano);
    getchar(); 

    while (1) {
        mostrarCalendario(mes, ano); 

        int dia;
        printf("Escolha um dia para a consulta: ");
        scanf("%d", &dia);
        getchar(); 

        if (dia < 1 || dia > 30) {
            printf("Dia inválido! Tente novamente.\n");
            continue;
        }

        // Verificação de disponibilidade
        if (dia == 10 || dia == 20) {
            system("cls");
            printf("Agenda cheia para o dia %02d/%02d/%d!\n", dia, mes, ano);
            continue; 
        } else if (dia % 7 == 0 || dia % 7 == 1) {
            system("cls");
            printf("Sem atendimento para o dia %02d/%02d/%d!\n", dia, mes, ano);
            continue;
        } else {
            while (1) {
                printf("Horários disponíveis para o dia %02d/%02d/%d:\n", dia, mes, ano);
                printf("\033[32m09:00 - disponível\033[0m\n");
                printf("\033[32m10:00 - disponível\033[0m\n");
                printf("\033[31m11:00 - ocupado\033[0m\n");
                printf("\033[32m12:00 - disponível\033[0m\n");
                printf("\033[32m14:00 - disponível\033[0m\n");
                printf("\033[31m15:00 - ocupado\033[0m\n");
                printf("\033[32m16:00 - disponível\033[0m\n");
                printf("\033[32m17:00 - disponível\033[0m\n");

                char horario[6];
                printf("Escolha um horário (HH:MM): ");
                fgets(horario, sizeof(horario), stdin);
                horario[strcspn(horario, "\n")] = '\0';
                strcpy(c.horario, horario);
                
                strcpy(c.status, "Agendada");
                adicionarConsulta(lista, c);
                printf("Consulta agendada com sucesso para o dia %02d/%02d/%d às %s.\n",
                       c.tipoAtendimento, dia, mes, ano, horario);
                return;

                if (strcmp(horario, "11:00") == 0 || strcmp(horario, "15:00") == 0) {
                    system("cls");
                    printf("Horário %s ocupado! Por favor, escolha outro.\n", horario);
                }
              
            }
        }
    }
}
