//consulta.h 

#ifndef CONSULTA_H
#define CONSULTA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

//Estrutura de dados
typedef struct Paciente {
    int id_paciente;
    char nome_paciente[100];
    char telefone_paciente[20];
    char datanasc_paciente[20];
    char sexo_paciente[20];
    char CPF_paciente[20];
    char RG_paciente[20];
    char email_paciente[100];
    char endereco_paciente[200];
    char estadocivil_paciente[20];
    char profissao_paciente[50];
    float peso_paciente;
    float altura_paciente;
    char planoSaude_paciente[100];
    char convenioMedico_paciente[100];
    char numeroCarteira_paciente[20];
    char validadeCarteira_paciente[20];
    char cartaoCNS_paciente[20];
    char observacao_paciente[300];
} PACIENTE;

typedef struct Consulta {
    int id_consulta;
    int idpaciente_consulta;
    int idmedico_consulta;
    char data_consulta[11];
    int dia_consulta;
    int mes_consulta;
    int ano_consulta;
    char hora_consulta[6];
    char tipoAtendimento[50];
    char status[20];
} CONSULTA;


typedef struct DiaDaAgenda {
    char data_agenda[11];
    int dia_agenda;
    int mes_agenda;
    int ano_agenda;
    int max_consultas_agenda;
    char primeiro_horario[6];
    char intervalo_horario[6];
} DIADAAGENDA;

//Estrutura de nó e lista genérica
typedef struct No {
    void *dado;
    struct No *proximo;
} NO;

typedef struct Lista {
    NO *inicio;
    int tamanho;
} LISTA;

//Criação e manipulação de lista
LISTA *criarLista();
static NO *criarNo(void *dado, size_t tamanhoDado);
bool inserirNoInicio(LISTA *lista, void *dado, size_t tamanhoDado);
bool inserirNoFim(LISTA *lista, void *dado, size_t tamanhoDado);
void *removerNoInicio(LISTA *lista);
void *removerNoFim(LISTA *lista);
void *buscarElemento(LISTA *lista, bool (*comparator)(void *, void *), void *chave);
void listarElementos(LISTA *lista, void (*imprimir)(void *));
int tamanhoLista(LISTA *lista);
void destruirLista(LISTA *lista, void (*liberarDado)(void *));

// Funções do menu de paciente/paciente em geral
void salvarPacientes(LISTA *lista);
void carregarPacientes(LISTA *lista);
void cadastrarPaciente(LISTA *lista);
void listarPacientes(LISTA *lista);
void imprimirPaciente(void *dado);
void editarPaciente(LISTA *lista);
void removerPaciente(LISTA *lista);
void consultarDadosPaciente(LISTA *lista);
void imprimirPaciente2(void *dado);
bool compararPacientePorID(void *dado, void *chave);

// Funções do menu de consulta/consulta em geral
void salvarConsultas(LISTA *lista);
void carregarConsultas(LISTA *lista);
void cadastrarConsulta(LISTA *lista, LISTA *lista2);
void listarConsultas(LISTA *lista);
void imprimirConsulta(void *dado);
void editarConsulta(LISTA *lista);
void removerConsulta(LISTA *lista);
void consultarDadosConsulta(LISTA *lista);
void imprimirConsulta2(void *dado);
bool compararConsultaPorID(void *dado, void *chave);

// Funções do menu de agenda medica/agenda em geral
void salvarAgenda(LISTA *lista);
void carregarAgenda(LISTA *lista);
void cadastrarDia(LISTA *lista);
void listarDias(LISTA *lista);
void imprimirDias(void *dado);
bool editarDia(LISTA *lista);
bool removerDia(LISTA *lista);
bool compararDias(void *dado, void *chave);
void criarDiasDoMes(LISTA *lista);
int diasNoMes(int mes, int ano);
bool diaExistente(LISTA *lista, int dia, int mes, int ano);
void listarMesesCadastrados(LISTA *lista);
void removerDiasDoMes(LISTA *lista);
void AgendaEmDia(LISTA *lista);
void criarMeses(LISTA *lista, int mes, int ano);
void imprimirCalendario(LISTA *listaC, LISTA *listaA, int id_medico);
void definirCor(LISTA *lista, int dia, int mes, int ano, int diaDaSemana, int consultas);
int buscarConsultasPorData(LISTA *lista, int dia, int mes, int ano);
int verificarData(LISTA *lista, int dia, int mes, int ano, int diaDaSemana, int consultas);
int buscarConsultasPorDataEHorario(LISTA *lista, int dia, int mes, int ano, char *horario);

//Funções de menu
void setColor(int textColor);
char controlaEntrada(int *destaque, int num_opcoes);
void menuRecepcionista();
void menuPaciente(LISTA *lista);
void menuListaPacientes(LISTA *lista);
void menuConsulta(LISTA *lista, LISTA *lista2);
void menuAgendaMedica(LISTA *lista, LISTA *lista2); 

#endif // CONSULTA_H
