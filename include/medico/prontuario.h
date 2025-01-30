#ifndef PRONTUARIO_H
#define PRONTUARIO_H

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

typedef struct No2 {
    SolicitarExame exame;
    struct No2 *proximo;
} No2;

typedef struct No3 {
    Encaminhamento encaminhamento;
    struct No3 *proximo;
} No3;

typedef struct NoProntuario {
    ProntuarioPaciente prontuario;
    struct No *proximo;
    No2 *exames; // Ponteiro para lista de exames
    No3 *encaminhamentos; // Ponteiro para lista de encaminhamentos
} No;

typedef struct {
    No *inicio;
} Lista;

// Funções para manipulação da lista
Lista* criarListaProntuario();
void adicionarProntuario(Lista *lista, ProntuarioPaciente prontuario);
void salvarProntuarioEmArquivo(Lista *lista);
void adicionarExame(Lista *lista, SolicitarExame exame, char *nome);
void adicionarEncaminhamento(Lista *lista, Encaminhamento encaminhamento, char *nome);
void listarProntuarios(Lista *lista, char *nome);
void listarExames(Lista *lista, char *nome);
void listarEncaminhamentos(Lista *lista, char *nome);
void liberarLista(Lista *lista);
void lerProntuarioDoArquivo(Lista *lista);
ProntuarioPaciente capturarDadosProntuario();
SolicitarExame capturarDadosExame(Lista *lista, char *nome);
Encaminhamento capturarDadosEncaminhamento(Lista *lista, char *nome);
char* str_repeat(const char *s, int n);
void exibirCabecalho(const char *titulo);
void menuMedico();

#endif // PRONTUARIO_H
