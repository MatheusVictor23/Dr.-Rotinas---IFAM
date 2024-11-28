#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "paciente.h"

static int next_id = 1;

Lista* criarLista() {
    Lista *lista = (Lista*)malloc(sizeof(Lista));
    lista->inicio = NULL;
    return lista;
}

void adicionarPaciente(Lista *lista, Paciente paciente) {
    No *novoNo = (No*)malloc(sizeof(No));
    novoNo->paciente = paciente;
    novoNo->proximo = lista->inicio;
    lista->inicio = novoNo;
}

void removerPaciente(Lista *lista, char *nome) {
    No *atual = lista->inicio;
    No *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->paciente.nome, nome) == 0) {
            if (anterior == NULL) {
                lista->inicio = atual->proximo; 
            } else {
                anterior->proximo = atual->proximo; 
            }
            free(atual);
            printf("Paciente %s removido com sucesso.\n", nome);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("Paciente %s não encontrado.\n", nome);
}

void listarPacientes(Lista *lista) {
    No *atual = lista->inicio;
    if (atual == NULL) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }
    printf("+---------------------------------------------------------------------+\n");
    printf("|                        LISTA DE PACIENTES                          |\n");
    printf("+---------------------------------------------------------------------+\n");

    while (atual != NULL) {
        printf("| ID: %d\n", atual->paciente.id);  // Exibe o ID do paciente
        printf("| Nome: %s\n", atual->paciente.nome);
        printf("| Data de Nascimento: %s\n", atual->paciente.datanasc);
        printf("| Sexo: %s\n", atual->paciente.sexo);
        printf("| CPF: %s\n", atual->paciente.CPF);
        printf("| Telefone: %s\n", atual->paciente.telefone);
        printf("| Email: %s\n", atual->paciente.email);
        printf("| Endereço: %s\n", atual->paciente.endereco);
        printf("| Plano de Saúde: %s\n", atual->paciente.planoSaude);
        printf("| Data de Cadastro: %s\n", atual->paciente.datacadastro);
        printf("| Estado Civil: %s\n", atual->paciente.estadocivil);
        printf("| Cidade de Nascimento: %s\n", atual->paciente.cidadeNascimento);
        printf("| País de Nascimento: %s\n", atual->paciente.paisNascimento);
        printf("| Grau de Instrução: %s\n", atual->paciente.grauInstrucao);
        printf("| Profissão: %s\n", atual->paciente.profissao);
        printf("| RG: %s\n", atual->paciente.rg);
        printf("| Nome da Mãe: %s\n", atual->paciente.nomeMae);
        printf("| Nome do Pai: %s\n", atual->paciente.nomePai);
        printf("| Nome do Responsável: %s\n", atual->paciente.nomeResponsavel);
        printf("| Peso: %.2f kg\n", atual->paciente.peso);
        printf("| Altura: %.2f m\n", atual->paciente.altura);
        printf("| Encaminhado Por: %s\n", atual->paciente.encaminhadoPor);
        printf("| Data da Primeira Consulta: %s\n", atual->paciente.dataPrimeiraConsulta);
        printf("| Convênio: %s\n", atual->paciente.convenio);
        printf("| Nº Carteira Convênio: %s\n", atual->paciente.numeroCarteira);
        printf("| Plano: %s\n", atual->paciente.plano);
        printf("| Validade da Carteira: %s\n", atual->paciente.validadeCarteira);
        printf("| Cartão CNS: %s\n", atual->paciente.cartaoCNS);
        printf("| Observação: %s\n", atual->paciente.observacao);
        printf("| Classificação: %s\n", atual->paciente.classificacao);
        printf("+---------------------------------------------------------------------+\n");
        atual = atual->proximo;
    }
}

void liberarLista(Lista *lista) {
    No *atual = lista->inicio;
    No *temp;
    while (atual != NULL) {
        temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    free(lista);
}

void salvarPacientesEmArquivo(Lista *lista) {
    FILE *arquivo = fopen("pacientes.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para gravação.\n");
        return;
    }

    No *atual = lista->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "ID:%d\nNome:%s\nData de Nascimento:%s\nSexo:%s\nCPF:%s\nTelefone:%s\nEmail:%s\nEndereco:%s\nPlano de Saude:%s\nData de Cadastro:%s\nEstado Civil:%s\nCidade de Nascimento:%s\nPais de Nascimento:%s\nGrau de Instrução:%s\nProfissao:%s\nRG:%s\nNome da Mãe:%s\nNome do Pai:%s\nNome do Responsável:%s\nPeso:%.2f\nAltura:%.2f\nEncaminhado Por:%s\nData da Primeira Consulta:%s\nConvenio:%s\nNumero da Carteira:%s\nPlano:%s\nValidade da Carteira:%s\nCartao CNS:%s\nObservacao:%s\nClassificacao:%s\n\n",
                atual->paciente.id,
                atual->paciente.nome,
                atual->paciente.datanasc,
                atual->paciente.sexo,
                atual->paciente.CPF,
                atual->paciente.telefone,
                atual->paciente.email,
                atual->paciente.endereco,
                atual->paciente.planoSaude,
                atual->paciente.datacadastro,
                atual->paciente.estadocivil,
                atual->paciente.cidadeNascimento,
                atual->paciente.paisNascimento,
                atual->paciente.grauInstrucao,
                atual->paciente.profissao,
                atual->paciente.rg,
                atual->paciente.nomeMae,
                atual->paciente.nomePai,
                atual->paciente.nomeResponsavel,
                atual->paciente.peso,
                atual->paciente.altura,
                atual->paciente.encaminhadoPor,
                atual->paciente.dataPrimeiraConsulta,
                atual->paciente.convenio,
                atual->paciente.numeroCarteira,
                atual->paciente.plano,
                atual->paciente.validadeCarteira,
                atual->paciente.cartaoCNS,
                atual->paciente.observacao,
                atual->paciente.classificacao);
        atual = atual->proximo;
    }

    fclose(arquivo);
    printf("Dados salvos com sucesso.\n");
}

Paciente capturarDadosPaciente() {
    Paciente p;
    p.id = next_id++; 
    printf("+---------------------------------------------------------------------+\n");
    printf("|                        FOLHA DE CADASTRO                           |\n");
    printf("+---------------------------------------------------------------------+\n");

    printf("| Nome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';  

    printf("| Data de Nascimento (dd/mm/aaaa): ");
    fgets(p.datanasc, sizeof(p.datanasc), stdin);
    p.datanasc[strcspn(p.datanasc, "\n")] = '\0'; 

    printf("| Sexo (M/F): ");
    fgets(p.sexo, sizeof(p.sexo), stdin);
    p.sexo[strcspn(p.sexo, "\n")] = '\0';  

    printf("| CPF: ");
    fgets(p.CPF, sizeof(p.CPF), stdin);
    p.CPF[strcspn(p.CPF, "\n")] = '\0';

    printf("| Email: ");
    fgets(p.email, sizeof(p.email), stdin);
    p.email[strcspn(p.email, "\n")] = '\0'; 

    printf("| Telefone: ");
    fgets(p.telefone, sizeof(p.telefone), stdin);
    p.telefone[strcspn(p.telefone, "\n")] = '\0'; 

    printf("| Endereço: ");
    fgets(p.endereco, sizeof(p.endereco), stdin);
    p.endereco[strcspn(p.endereco, "\n")] = '\0'; 

    printf("| Plano de Saúde(Sim/Não): ");
    fgets(p.planoSaude, sizeof(p.planoSaude), stdin);
    p.planoSaude[strcspn(p.planoSaude, "\n")] = '\0'; 

    printf("| Data de Cadastro: ");
    fgets(p.datacadastro, sizeof(p.datacadastro), stdin);
    p.datacadastro[strcspn(p.datacadastro, "\n")] = '\0'; 

    printf("| Estado Civil: ");
    fgets(p.estadocivil, sizeof(p.estadocivil), stdin);
    p.estadocivil[strcspn(p.estadocivil, "\n")] = '\0'; 

    printf("| Cidade de Nascimento: ");
    fgets(p.cidadeNascimento, sizeof(p.cidadeNascimento), stdin);
    p.cidadeNascimento[strcspn(p.cidadeNascimento, "\n")] = '\0'; 

    printf("| País de Nascimento: ");
    fgets(p.paisNascimento, sizeof(p.paisNascimento), stdin);
    p.paisNascimento[strcspn(p.paisNascimento, "\n")] = '\0'; 

    printf("| Grau de Instrução: ");
    fgets(p.grauInstrucao, sizeof(p.grauInstrucao), stdin);
    p.grauInstrucao[strcspn(p.grauInstrucao, "\n")] = '\0'; 

    printf("| Profissão: ");
    fgets(p.profissao, sizeof(p.profissao), stdin);
    p.profissao[strcspn(p.profissao, "\n")] = '\0'; 

    printf("| RG: ");
    fgets(p.rg, sizeof(p.rg), stdin);
    p.rg[strcspn(p.rg, "\n")] = '\0'; 

    printf("| Nome da Mãe: ");
    fgets(p.nomeMae, sizeof(p.nomeMae), stdin);
    p.nomeMae[strcspn(p.nomeMae, "\n")] = '\0'; 

    printf("| Nome do Pai: ");
    fgets(p.nomePai, sizeof(p.nomePai), stdin);
    p.nomePai[strcspn(p.nomePai, "\n")] = '\0'; 

    printf("| Nome do Responsável: ");
    fgets(p.nomeResponsavel, sizeof(p.nomeResponsavel), stdin);
    p.nomeResponsavel[strcspn(p.nomeResponsavel, "\n")] = '\0'; 

    printf("| Peso (kg): ");
    scanf("%f", &p.peso);
    getchar(); 

    printf("| Altura (m): ");
    scanf("%f", &p.altura);
    getchar(); 

    printf("| Encaminhado Por: ");
    fgets(p.encaminhadoPor, sizeof(p.encaminhadoPor), stdin);
    p.encaminhadoPor[strcspn(p.encaminhadoPor, "\n")] = '\0'; 

    printf("| Data da Primeira Consulta: ");
    fgets(p.dataPrimeiraConsulta, sizeof(p.dataPrimeiraConsulta), stdin);
    p.dataPrimeiraConsulta[strcspn(p.dataPrimeiraConsulta, "\n")] = '\0'; 

    printf("| Convênio: ");
    fgets(p.convenio, sizeof(p.convenio), stdin);
    p.convenio[strcspn(p.convenio, "\n")] = '\0'; 

    printf("| Número da Carteira Convênio: ");
    fgets(p.numeroCarteira, sizeof(p.numeroCarteira), stdin);
    p.numeroCarteira[strcspn(p.numeroCarteira, "\n")] = '\0'; 

    printf("| Plano: ");
    fgets(p.plano, sizeof(p.plano), stdin);
    p.plano[strcspn(p.plano, "\n")] = '\0'; 

    printf("| Validade da Carteira: ");
    fgets(p.validadeCarteira, sizeof(p.validadeCarteira), stdin);
    p.validadeCarteira[strcspn(p.validadeCarteira, "\n")] = '\0'; 

    printf("| Cartão CNS: ");
    fgets(p.cartaoCNS, sizeof(p.cartaoCNS), stdin);
    p.cartaoCNS[strcspn(p.cartaoCNS, "\n")] = '\0'; 

    printf("| Observação: ");
    fgets(p.observacao, sizeof(p.observacao), stdin);
    p.observacao[strcspn(p.observacao, "\n")] = '\0';  

    return p;
}
