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

// Declaração das funções de login
int verificarLogin(const char *user, const char *senha, int cargoAtual);
void on_login_window_destroy(GtkWidget * widget, gpointer data);
void on_login_button_clicked(GtkButton *button, gpointer user_data);
void iniciarLogin(int cargo);

#endif
