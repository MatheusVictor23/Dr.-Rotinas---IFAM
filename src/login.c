#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "../include/telas.h"
#include "../include/gerenciarMedicos.h"
#include "../include/login.h"
#include "../include/menu.h"

GtkBuilder *builder_login;
int cargoAtual;

void on_login_window_destroy(GtkWidget * widget, gpointer data){
    gtk_main_quit();
};

// Função que lê os usuários do arquivo de usuários e verifica se o login é válido
int verificarLogin(const char *user, const char *senha) {
    FILE *usuarios = fopen("../data/usuarios.bin", "rb");
    if (usuarios == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Erro ao abrir o arquivo de usuários.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return -1; // retorna -1 se não conseguir abrir o arquivo
    }

    printf("arquivo binario deu certo\n");

    Usuario usuario;
    int indexUsuario = 0;

    while (fread(&usuario, sizeof(Usuario), 1, usuarios)) {
        if (strcmp(usuario.usuario, user) == 0) {
            if (strcmp(usuario.senha, senha) == 0) {
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

void iniciarLogin(int cargo) {
    cargoAtual = cargo;

    printf("cargo atual: %d\n", cargoAtual);

    builder_login = gtk_builder_new_from_file("../interface/login.glade");

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder_login, "login_window"));
    GtkButton *login_button = GTK_BUTTON(gtk_builder_get_object(builder_login, "login_button"));

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), window);
    g_signal_connect(window, "destroy", G_CALLBACK(on_login_window_destroy), NULL);


    // Exibe a janela de login

    gtk_widget_show_all(window);
    
}


void on_login_button_clicked(GtkButton *button, gpointer user_data) {

    GtkWidget *login_window = GTK_WIDGET(user_data);

    GtkEntry *user = GTK_ENTRY(gtk_builder_get_object(builder_login, "user"));
    GtkEntry *senha = GTK_ENTRY(gtk_builder_get_object(builder_login, "senha"));

    const char *user_text = gtk_entry_get_text(user);
    const char *senha_text = gtk_entry_get_text(senha);

    int indexUsuario = verificarLogin(user_text, senha_text);

    switch(indexUsuario) {
        case 0:
            gtk_widget_hide(login_window);
            iniciarGerenciamentoMedico();
            break;
        case 1:
            gtk_widget_hide(login_window);
            telaMedico(builder_login);
            break;
        case 2:
            gtk_widget_hide(login_window);
            telaRecepcao(builder_login);
            break;
        default:
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Falha no login. Verifique se o usuário e senha estão corretos.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            break;
    }
    
}


