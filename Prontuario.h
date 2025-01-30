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

// Funções
Lista* criarLista();
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
void setColor(int textColor);
void imprimirMenu(const char *opcoes[], const char *opcoesDestacadas[], int num_opcoes, int destaque, int espaco_max);
char controlaEntrada(int *destaque, int num_opcoes);
void liberarLista(Lista *lista);

#endif // PRONTUARIO_H