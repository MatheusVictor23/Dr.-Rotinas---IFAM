#include <gtk/gtk.h>
#include "../include/login.h"
#include "telas.h"

void on_button_Cadastrar_paciente_clicked(GtkWidget *widget, gpointer data) {
    g_print("Chamar tela Cadastrar paciente\n");
}

void on_button_Listar_pacientes_clicked(GtkWidget *widget, gpointer data) {
    g_print("Chamar tela Listar pacientes\n");
}

void on_button_Agendar_consulta_clicked(GtkWidget *widget, gpointer data) {
    g_print("Chamar tela Agendar consulta\n");
}

void on_button_Listar_consulta_clicked(GtkWidget *widget, gpointer data) {
    g_print("Chamar tela Listar consulta\n");
}

void on_view_menu_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void telaMenu(GtkBuilder *builder) {
    builder = gtk_builder_new_from_file("../interface/tela_menu.glade");
    GtkWidget *view_menu = GTK_WIDGET(gtk_builder_get_object(builder, "view_menu"));

    g_signal_connect(gtk_builder_get_object(builder, "button_Cadastrar_paciente"), "clicked", G_CALLBACK(on_button_Cadastrar_paciente_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "button_Listar_pacientes"), "clicked", G_CALLBACK(on_button_Listar_pacientes_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "button_Agendar_consulta"), "clicked", G_CALLBACK(on_button_Agendar_consulta_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "button_Listar_consulta"), "clicked", G_CALLBACK(on_button_Listar_consulta_clicked), NULL);

    g_signal_connect(view_menu, "destroy", G_CALLBACK(on_view_menu_destroy), NULL);

    gtk_widget_show_all(view_menu);
    g_object_unref(builder);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new_from_file("../interface/tela_menu.glade");
    telaMenu(builder);

    gtk_main();

    return 0;
}
