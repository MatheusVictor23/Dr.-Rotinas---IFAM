#ifndef LOGIN_H
#define LOGIN_H

typedef struct {
    char usuario[50];
    char senha[50];
} Usuario;

// Declaração das funções de login
int verificarLogin(const char *user, const char *senha);
void on_login_window_destroy(GtkWidget * widget, gpointer data);
void on_login_button_clicked(GtkButton *button, gpointer user_data);
void iniciarLogin(int cargo);

#endif
