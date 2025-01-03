//consulta.h

#ifndef CONSULTA_H
#define CONSULTA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>

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

//ESTRUTURAS DA AGENDA MÉDICA REMOVIDAS.

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
bool inserirNoInicio(LISTA *lista, void *dado, size_t tamanhoDado);
bool inserirNoFim(LISTA *lista, void *dado, size_t tamanhoDado);
void *removerNoInicio(LISTA *lista);
void *removerNoFim(LISTA *lista);
void *buscarElemento(LISTA *lista, bool (*comparator)(void *, void *), void *chave);
void listarElementos(LISTA *lista, void (*imprimir)(void *));
int tamanhoLista(LISTA *lista);
void destruirLista(LISTA *lista, void (*liberarDado)(void *));

// Funções do menu de paciente/paciente em geral
void carregarPacientes(LISTA *lista);
void salvarPacientes(LISTA *lista);
void imprimirPaciente(void *dado);
void imprimirPaciente2(void *dado);
void cadastrarPaciente(LISTA *lista);
void listarPacientes(LISTA *lista);
bool compararPacientePorID(void *dado, void *chave);
void editarPaciente(LISTA *lista);
void removerPaciente(LISTA *lista);

// Funções do menu de consulta/consulta em geral
void carregarConsultas(LISTA *lista);
void salvarConsultas(LISTA *lista);
void imprimirConsulta(void *dado);
void imprimirConsulta2(void *dado);
void cadastrarConsulta(LISTA *lista);
void listarConsultas(LISTA *lista);
bool compararConsultaPorID(void *dado, void *chave);
void editarConsulta(LISTA *lista);
void removerConsulta(LISTA *lista);

//Funções de menu
void menuRecepcionista();
void menuPaciente(LISTA *lista);
void menuListaPacientes(LISTA *lista);
void menuConsulta(LISTA *lista);
void menuAgendaMedica(); 

//FUNÇÕES DA AGENDA MÉDICA REMOVIDAS.

#endif // CONSULTA_H
