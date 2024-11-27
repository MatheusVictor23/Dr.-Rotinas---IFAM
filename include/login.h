#ifndef LOGIN_H
#define LOGIN_H

// typedef enum {
//     ADMIN = 0,
//     MEDICO = 1,
//     RECEPCIONISTA = 2
// } Cargo;

typedef struct _user Usuario;

void menu();

void finalizar();

void inserir_usuarios_de_teste();

int verificar_login(const char *usuario, const char *senha, const char *cargo);

void fazer_login(char *cargo_escolhido);

#endif
