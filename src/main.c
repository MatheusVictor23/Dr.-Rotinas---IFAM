#include <stdio.h>
#include <locale.h>
#include <string.h>
#include "paciente.h"
#include "consultas.h"

int main() {
    setlocale(LC_ALL,"Portuguese");

    Lista *listaPacientes = criarLista();
    ListaConsultas *listaConsultas = criarListaConsultas();
    int opcao;
    Paciente p;

    do {
        printf("\nMenu:\n");
        printf("1. Cadastrar paciente\n");
        printf("2. Listar pacientes\n");
        printf("3. Remover paciente\n");
        printf("4. Salvar pacientes\n");
        printf("5. Agendar consulta\n");
        printf("6. Listar consultas\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();  

        switch (opcao) {
            case 1:
                p = capturarDadosPaciente();
                adicionarPaciente(listaPacientes, p);
                break;
            case 2:
                listarPacientes(listaPacientes);
                break;
            case 3:
                printf("Digite o nome do paciente a ser removido: ");
                char nome[100];
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';  
                removerPaciente(listaPacientes, nome);
                break;
            case 4:
                salvarPacientesEmArquivo(listaPacientes);
                break;
            case 5:
                printf("Agendamento de consulta\n");
                agendarConsulta(listaConsultas, 1, 1);
                break;
            case 6:
                listarConsultas(listaConsultas);
                break;
            case 7:
                liberarLista(listaPacientes);
                liberarLista(listaConsultas);
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao != 7);

    return 0;
}
