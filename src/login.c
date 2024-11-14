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
    // liberar o builder e sair do loop principal
    g_object_unref(builder_login);
    iniciarMenu();
};

// Função que lê os usuários do arquivo de usuários e verifica se o login é válido
int verificarLogin(const char *user, const char *senha, int cargoAtual) {
    FILE *usuarios = fopen("../data/usuarios.bin", "rb");
    if (usuarios == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Erro ao abrir o arquivo de usuários.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return -1; // retorna -1 se não conseguir abrir o arquivo
    }

    g_message("arquivo binario deu certo\n");

    Usuario usuario;

    while (fread(&usuario, sizeof(Usuario), 1, usuarios)) {
        if (strcmp(usuario.usuario, user) == 0) {
            if (strcmp(usuario.senha, senha) == 0) {
                if(cargoAtual == usuario.cargo) {
                    return 1; // retorna 1 se tudo tiver ok
                    fclose(usuarios);
                }
                
            }
        }    
    }

    fclose(usuarios);
    return -1; // retorna -1 se não encontrar o usuário ou a senha
    g_message("erro na funcao verificarLogin\n");
}

// Função que inicia a tela de login

void iniciarLogin(int cargoSelecionado) {
    cargoAtual = cargoSelecionado;

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

    int index = verificarLogin(user_text, senha_text, cargoAtual);

    if(index == 1) {
        switch(cargoAtual) {
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
         }
    }

    else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,    GTK_BUTTONS_OK, "Falha no login. Verifique se o usuário, senha e cargo selecionado estão corretos.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    
}


