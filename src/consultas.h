#ifndef CONSULTAS_H
#define CONSULTAS_H

typedef struct {
    int id;  // ID da consulta
    int paciente_id;  // ID do paciente
    int medico_id;    // ID do médico
    char data[11];    // Data da consulta (dd/mm/aaaa)
    char horario[6];  // Horário da consulta (HH:MM)
    char tipoAtendimento[50]; // Tipo de atendimento
    char status[20];  // Status da consulta (livre, agendada, etc.)
} Consulta;

typedef struct NoConsulta {
    Consulta consulta;
    struct NoConsulta *proximo;
} NoConsulta;

typedef struct {
    NoConsulta *inicio;
} ListaConsultas;

// Funções para manipulação das consultas
ListaConsultas* criarListaConsultas();
void adicionarConsulta(ListaConsultas *lista, Consulta consulta);
void listarConsultas(ListaConsultas *lista);
void salvarConsultasEmArquivo(ListaConsultas *lista);
void agendarConsulta(ListaConsultas *lista, int paciente_id, int medico_id);
void mostrarCalendario(int mes, int ano);

// Prototipação das funções para buscar nomes
const char* buscarNomePaciente(int paciente_id);
const char* buscarNomeMedico(int medico_id);

#endif // CONSULTAS_H
