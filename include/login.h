#ifndef LOGIN_H
#define LOGIN_H

typedef enum {
    ADMIN = 0,
    MEDICO = 1,
    RECEPCIONISTA = 2
} Cargo;

typedef struct {
    char usuario[50];
    char senha[50];
    Cargo cargo;
} Usuario;


#endif
