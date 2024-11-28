#ifndef PACIENTE_H
#define PACIENTE_H

typedef struct {
    int id;
    char nome[100];
    char datanasc[20];
    char sexo[20];
    char CPF[20];
    char email[100];
    char telefone[100];
    char endereco[200];
    char planoSaude[100];
    char datacadastro[20];
    char estadocivil[20];
    char cidadeNascimento[100];
    char paisNascimento[100];
    char grauInstrucao[50];
    char profissao[50];
    char rg[20];
    char nomeMae[100];
    char nomePai[100];
    char nomeResponsavel[100];
    float peso;
    float altura;
    char encaminhadoPor[100];
    char dataPrimeiraConsulta[20];
    char convenio[100];
    char numeroCarteira[20];
    char plano[100];
    char validadeCarteira[20];
    char cartaoCNS[20];
    char observacao[300];
    char classificacao[50];
} Paciente;

typedef struct No {
    Paciente paciente;
    struct No *proximo;
} No;

typedef struct {
    No *inicio;
} Lista;

// Funções para manipulação da lista
Lista* criarLista();
void adicionarPaciente(Lista *lista, Paciente paciente);
void removerPaciente(Lista *lista, char *nome);
void listarPacientes(Lista *lista);
void liberarLista(Lista *lista);
void salvarPacientesEmArquivo(Lista *lista);
Paciente capturarDadosPaciente();

#endif // PACIENTE_H
