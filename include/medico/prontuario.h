#ifndef PRONTUARIO_H
#define PRONTUARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>

// Estruturas de Dados

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

typedef struct NoProntuario {
    ProntuarioPaciente prontuario;
    struct NoProntuario *proximo;
    struct NoProntuario2 *exames; // Ponteiro para lista de exames
    struct NoProntuario3 *encaminhamentos; // Ponteiro para lista de encaminhamentos
} NoProntuario;

typedef struct NoProntuario2 {
    SolicitarExame exame;
    struct NoProntuario2 *proximo;
} NoProntuario2;

typedef struct NoProntuario3 {
    Encaminhamento encaminhamento;
    struct NoProntuario3 *proximo;
} NoProntuario3;

typedef struct {
    NoProntuario *inicio;
} Lista;

// Funções
Lista* criarListaMedico();
ProntuarioPaciente capturarDadosProntuario();
SolicitarExame capturarDadosExame(Lista *lista, char *nome);
Encaminhamento capturarDadosEncaminhamento(Lista *lista, char *nome);
void adicionarProntuario(Lista *lista, ProntuarioPaciente prontuario);
void adicionarExame(Lista *lista, SolicitarExame exame, char *nome);
void adicionarEncaminhamento(Lista *lista, Encaminhamento encaminhamento, char *nome);
void listarProntuarios(Lista *lista, char *nome);
void listarExames(Lista *lista, char *nome);
void listarEncaminhamentos(Lista *lista, char *nome);
void salvarProntuarioEmArquivo(Lista *lista);
void lerProntuarioDoArquivo(Lista *lista);
void menuProntuario(Lista *lista);
void menuMedico();
// void setColor(int textColor);
// void imprimirMenu(const char *opcoes[], const char *opcoesDestacadas[], int num_opcoes, int destaque, int espaco_max);
// char controlaEntrada(int *destaque, int num_opcoes);
void liberarListaMedico(Lista *lista);

#endif PRONTUARIO_H