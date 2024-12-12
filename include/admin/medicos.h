#ifndef MEDICOS_H
#define MEDICOS_H

void ler_cpf_com_mascara_medico(char *cpf);
char get_char_input_medico();

void menuMedicos();
void cadastrarMedico();
void listarMedicos();
void listarMedicosDiretamenteArquivo();
void limparLista();
void carregarMedicos();
void excluirMedico();
int gerarID();

#endif