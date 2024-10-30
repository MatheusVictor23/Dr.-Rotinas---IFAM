#include <gtk/gtk.h>
#include "telas.h"

void telaRecepcao(GtkBuilder *builder) {
    builder = gtk_builder_new_from_file("../interface/tela_recepcao.glade");
    GtkWidget *tela_recepcao = GTK_WIDGET(gtk_builder_get_object(builder, "tela_recepcao"));
    gtk_widget_show_all(tela_recepcao);
}

void telaMedico(GtkBuilder *builder) {
    builder = gtk_builder_new_from_file("../interface/tela_medico.glade");
    GtkWidget *tela_medico = GTK_WIDGET(gtk_builder_get_object(builder, "tela_medico"));
    gtk_widget_show_all(tela_medico);
}

void telaAdmin(GtkBuilder *builder) {
    builder = gtk_builder_new_from_file("../interface/tela_admin.glade");
    GtkWidget *tela_admin = GTK_WIDGET(gtk_builder_get_object(builder, "tela_admin"));
    gtk_widget_show_all(tela_admin);
}
