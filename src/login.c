#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "telas.h"

#define MAX_USERS 3 
#define MAX_LENGTH 32

// Arrays de usuários e senhas cadastrados

static char *usernames[MAX_USERS] = {"@admin", "@meduser", "@recepcao"};
static char *senhas[MAX_USERS] = {"rotinasdr01", "med01", "recep01"};

GtkBuilder *builder;

// Função que verifica se o login é válido

int verificarLogin(const char *user, const char *senha) {

    for(int i = 0; i < MAX_USERS; i++) {
        if(strcmp(usernames[i], user) == 0 && strcmp(senhas[i], senha) == 0) {
            return i; // login ok, e retorna o indice do usuario encontrado
        }
    }
    
    return -1; // login falhou
}

void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *user = GTK_ENTRY(gtk_builder_get_object(builder, "user"));
    GtkEntry *senha = GTK_ENTRY(gtk_builder_get_object(builder, "senha"));

    const char *user_text = gtk_entry_get_text(user);
    const char *senha_text = gtk_entry_get_text(senha);

    int indexUsuario = verificarLogin(user_text, senha_text);

    switch(indexUsuario) {
/*         case 0:
            telaAdmin(builder);
            break;
        case 1:
            telaMedico(builder);
            break; */
        case 2:
            telaRecepcao(builder);
            break;
        default:
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Falha no login. Verifique se o usuário e senha estão corretos.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            break;
    }
}

// Função que inicia a tela de login

void iniciarLogin() {

    builder = gtk_builder_new_from_file("../interface/login.glade");

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
    GtkButton *login_button = GTK_BUTTON(gtk_builder_get_object(builder, "login_button"));

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);

    // Exibe a janela de login

    gtk_widget_show_all(window);
    
}
