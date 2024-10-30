#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "telas.h"

GtkBuilder *builder;

// prototipo da funcao que gerencia o click do botao de login
void on_login_button_clicked(GtkButton *button, gpointer user_data);

// Função que lê os usuários do arquivo de usuários e verifica se o login é válido
int verificarLogin(const char *user, const char *senha) {
    FILE *usuarios = fopen("../data/usuarios.txt", "r");
    if (usuarios == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Erro ao abrir o arquivo de usuários.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        exit(1); // Abre a janela dialog e encerra o programa
    }

    char linha[100];
    char *token;
    int indexUsuario = 0;

    while (fgets(linha, sizeof(linha), usuarios) != NULL) {
    
        token = strtok(linha, ",");
        if (token == NULL) {
            continue;
        } 
    
        char usuarioLimpo[50];
        sscanf(token, "%s", usuarioLimpo); // token armazenado na string usuarioLimpo

        token = strtok(NULL, ","); // proxima string apos o delimitador
        if (token == NULL) {
            continue; 
        }

        char senhaLimpa[50]; 
        sscanf(token, "%s", senhaLimpa); // token armazenado na string senhaLimpa

        if (strcmp(usuarioLimpo, user) == 0) {
            if (strcmp(senhaLimpa, senha) == 0) {
                fclose(usuarios);
                return indexUsuario; // Retorna o índice do usuário se encontrado
            }
        }
        indexUsuario++;
    }

    fclose(usuarios);
    return -1; // retorna -1 se não encontrar o usuário ou a senha
}

// Função que inicia a tela de login

void iniciarLogin() {

    builder = gtk_builder_new_from_file("../interface/login.glade");

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
    GtkButton *login_button = GTK_BUTTON(gtk_builder_get_object(builder, "login_button"));

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), window);

    // Exibe a janela de login

    gtk_widget_show_all(window);
    
}


void on_login_button_clicked(GtkButton *button, gpointer user_data) {

    GtkWidget *login_window = GTK_WIDGET(user_data);

    GtkEntry *user = GTK_ENTRY(gtk_builder_get_object(builder, "user"));
    GtkEntry *senha = GTK_ENTRY(gtk_builder_get_object(builder, "senha"));

    const char *user_text = gtk_entry_get_text(user);
    const char *senha_text = gtk_entry_get_text(senha);

    int indexUsuario = verificarLogin(user_text, senha_text);

    switch(indexUsuario) {
        case 0:
            gtk_widget_destroy(login_window);
            telaAdmin(builder);
            break;
        case 1:
            gtk_widget_destroy(login_window);
            telaMedico(builder);
            break;
        case 2:
            gtk_widget_destroy(login_window);
            telaRecepcao(builder);
            break;
        default:
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Falha no login. Verifique se o usuário e senha estão corretos.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            break;
    }
    
}


