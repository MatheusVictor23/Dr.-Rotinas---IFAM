#ifndef LOGIN_H
#define LOGIN_H

#define MAX_LENGTH 32

// Declaração das funções de login
int verificarLogin(const char *user, const char *senha);
void on_main_window_destroy(GtkWidget * widget, gpointer data);
void on_login_button_clicked(GtkButton *button, gpointer user_data);
void iniciarLogin();

#endif
