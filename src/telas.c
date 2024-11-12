#include <gtk/gtk.h>
#include "../include/login.h"
#include "telas.h"
#include "../include/menu.h"

void on_tela_recepcao_destroy(GtkWidget *widget, gpointer data) {
    iniciarMenu();
}

void telaRecepcao(GtkBuilder *builder) {
    builder = gtk_builder_new_from_file("../interface/tela_recepcao.glade");
    GtkWidget *tela_recepcao = GTK_WIDGET(gtk_builder_get_object(builder, "tela_recepcao"));
    g_signal_connect(tela_recepcao, "destroy", G_CALLBACK(on_tela_recepcao_destroy), NULL);
    gtk_widget_show_all(tela_recepcao);
}

void on_tela_medico_destroy(GtkWidget *widget, gpointer data) {
    iniciarMenu(); 
}

void telaMedico(GtkBuilder *builder) {
    builder = gtk_builder_new_from_file("../interface/tela_medico.glade");
    GtkWidget *tela_medico = GTK_WIDGET(gtk_builder_get_object(builder, "tela_medico"));
    g_signal_connect(tela_medico, "destroy", G_CALLBACK(on_tela_medico_destroy), NULL);
    gtk_widget_show_all(tela_medico);
}

void telaAdmin(GtkBuilder *builder) {
    builder = gtk_builder_new_from_file("../interface/tela_admin.glade");
    GtkWidget *tela_admin = GTK_WIDGET(gtk_builder_get_object(builder, "tela_admin"));
    gtk_widget_show_all(tela_admin);
}
