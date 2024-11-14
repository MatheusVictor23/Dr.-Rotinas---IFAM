#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/gerenciarMedicos.h"
#include "../include/login.h"
#include "../include/menu.h"

GtkBuilder *builder_medico_admin;
GtkStack * stack;
GtkListStore *medico;


typedef struct medico{
    char nome[100];
    char email[100];
    char telefone[100];
    char CRM[20];
    struct medico *proximo;
}user;

const gchar* gtk_entry_get_text (GtkEntry* entry);

user *cabecalho_user;
user *proximo_user;

// void mensagem(char text[100], char secondary_text[100], char icon_name[100]){
//     GtkMessageDialog * mensagem_dialogo = gtk_builder_get_object(_medico_admin, "mensagem");

//     g_object_set(mensagem_dialogo, "text", text, NULL);
//     g_object_set(mensagem_dialogo, "secondary_text", secondary_text, NULL);
//     g_object_set(mensagem_dialogo, "icon_name", icon_name, NULL);


//     gtk_widget_show_all(mensagem_dialogo);
//     gtk_dialog_run(mensagem_dialogo);
//     gtk_widget_hide(mensagem_dialogo);
// }

void on_main_window_destroy_medico(GtkWidget * widget, gpointer data){
    iniciarMenu();
};
void on_main_cadastro_cadastrar_clicked(GtkWidget * widget, gpointer data){
    gtk_stack_set_visible_child_name(stack, "form_cadastro");
};
void on_main_cadastro_listar_clicked(GtkWidget * widget, gpointer data){
    gtk_stack_set_visible_child_name(stack, "medico_listagem");
};
void on_form_cadastro_cadastrar_clicked(GtkWidget * widget, gpointer data){
    
    FILE *medicos;
    medicos = fopen("../data/medicos.txt", "a");
    if (medicos == NULL) {
        g_print("Erro ao abrir o arquivo para gravação.\n");
        return;
    }

    GtkEntry *cad_nome = GTK_ENTRY(gtk_builder_get_object(builder_medico_admin, "form_cadastro_nome"));
    const char *medico_nome = gtk_entry_get_text(cad_nome);

    GtkEntry *cad_email = GTK_ENTRY(gtk_builder_get_object(builder_medico_admin, "form_cadastro_email"));
    const char *medico_email = gtk_entry_get_text(cad_email);

    GtkEntry *cad_telefone = GTK_ENTRY(gtk_builder_get_object(builder_medico_admin, "form_cadastro_telefone"));
    const char *medico_telefone = gtk_entry_get_text(cad_telefone);

    GtkEntry *cad_CRM = GTK_ENTRY(gtk_builder_get_object(builder_medico_admin, "form_cadastro_CRM"));
    const char *medico_CRM = gtk_entry_get_text(cad_CRM);

    fprintf(medicos, "Nome:%s\nEmail:%s\nTelefone:%s\nCRM:%s\n",
            medico_nome, medico_email, medico_telefone, medico_CRM);

    fclose(medicos);
    gtk_stack_set_visible_child_name(stack, "main_cadastro");
};

void on_form_cadastro_cancelar_clicked(GtkWidget * widget, gpointer data){
    gtk_stack_set_visible_child_name(stack, "main_cadastro");
};

void on_medico_listagem_back_clicked(GtkWidget * widget, gpointer data){
    gtk_stack_set_visible_child_name(stack, "main_cadastro");
};

void on_medico_listagem_list_clicked(GtkWidget *widget, gpointer data) {
    FILE *medicos = fopen("../data/medicos.txt", "r");
    if (medicos == NULL) {
        g_print("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    GtkTreeIter iter;
    gtk_list_store_clear(medico);

    char nomeMed[100];
    char emailMed[100];
    char telefoneMed[100];
    char CRMMed[20];

    while (fscanf(medicos, "Nome:%99[^\n]\nEmail:%99[^\n]\nTelefone:%99[^\n]\nCRM:%19[^\n]\n",
                  nomeMed, emailMed, telefoneMed, CRMMed) == 4) {
        gtk_list_store_append(medico, &iter);
        gtk_list_store_set(medico, &iter,
                           0, nomeMed,
                           1, emailMed,
                           2, telefoneMed,
                           3, CRMMed,
                           -1);
    }
    fclose(medicos);

}

void iniciarGerenciamentoMedico(){

    cabecalho_user = (user *)malloc(sizeof(user));
    cabecalho_user->proximo = NULL;
    proximo_user = cabecalho_user;

    builder_medico_admin = gtk_builder_new_from_file("../interface/tela_admin.glade");

    gtk_builder_add_callback_symbols(
        builder_medico_admin,
        "on_main_cadastro_cadastrar_clicked",       G_CALLBACK(on_main_cadastro_cadastrar_clicked),
        "on_main_window_destroy_medico",            G_CALLBACK(on_main_window_destroy_medico),
        "on_main_cadastro_listar_clicked",          G_CALLBACK(on_main_cadastro_listar_clicked),
        "on_form_cadastro_cadastrar_clicked",       G_CALLBACK(on_form_cadastro_cadastrar_clicked),
        "on_form_cadastro_cancelar_clicked",        G_CALLBACK(on_form_cadastro_cancelar_clicked),
        "on_medico_listagem_back_clicked",          G_CALLBACK(on_medico_listagem_back_clicked),
        "on_medico_listagem_list_clicked",          G_CALLBACK(on_medico_listagem_list_clicked),
        NULL
    );

    gtk_builder_connect_signals(builder_medico_admin,NULL);

    stack = GTK_STACK(gtk_builder_get_object(builder_medico_admin, "stack"));
    medico = GTK_LIST_STORE(gtk_builder_get_object(builder_medico_admin, "liststore1"));
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder_medico_admin, "main_window"));

    gtk_widget_show_all(window);

   
}