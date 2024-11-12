#include <gtk/gtk.h>
#include "../include/login.h"
#include "../include/telas.h"
#include "../include/gerenciarMedicos.h"
#include "../include/menu.h"

GtkBuilder *builder_menu;

void on_main_window_destroy(GtkWidget *widget, gpointer data) {
    // liberar o builder e sair do loop principal
    g_object_unref(builder_menu);
    gtk_main_quit();
}

void iniciarLoginComCargo(int cargo) {
    iniciarLogin(cargo);
}

void on_recep_clicked(GtkButton *button, gpointer user_data) {
    g_print("botao recepcionista clicado\n");
    iniciarLoginComCargo(2); // 2 para recepcionista

    // esconder a janela de menu
    GtkWidget *menu_window = GTK_WIDGET(gtk_builder_get_object(builder_menu, "menu_inicial"));
    gtk_widget_hide(menu_window);
}

void on_medico_clicked(GtkButton *button, gpointer user_data) {
    g_print("botao medico clicado\n");
    iniciarLoginComCargo(1); // 1 para medico

    // esconder a janela de menu
    GtkWidget *menu_window = GTK_WIDGET(gtk_builder_get_object(builder_menu, "menu_inicial"));
    gtk_widget_hide(menu_window);
}

void on_admin_clicked(GtkButton *button, gpointer user_data) {
    g_print("botao administrador clicado\n");
    iniciarLoginComCargo(0); // 0 para admin

    // esconder a janela de menu
    GtkWidget *menu_window = GTK_WIDGET(gtk_builder_get_object(builder_menu, "menu_inicial"));
    gtk_widget_hide(menu_window);
}

void iniciarMenu() {
    builder_menu = gtk_builder_new_from_file("../interface/menu.glade");
    if (!builder_menu) {
        g_critical("erro ao carregar o builder.");
        return;
    }

    GtkWidget *menu_window = GTK_WIDGET(gtk_builder_get_object(builder_menu, "menu_inicial"));
    if (!menu_window) {
        g_critical("erro ao carregar a janela.");
        g_object_unref(builder_menu);
        return;
    }

    g_signal_connect(menu_window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);

    // Conectar os sinais
    GtkButton *admin_button = GTK_BUTTON(gtk_builder_get_object(builder_menu, "admin_button"));
    g_signal_connect(admin_button, "clicked", G_CALLBACK(on_admin_clicked), NULL);

    GtkButton *medico_button = GTK_BUTTON(gtk_builder_get_object(builder_menu, "med_button"));
    g_signal_connect(medico_button, "clicked", G_CALLBACK(on_medico_clicked), NULL);

    GtkButton *recep_button = GTK_BUTTON(gtk_builder_get_object(builder_menu, "recep_button"));
    g_signal_connect(recep_button, "clicked", G_CALLBACK(on_recep_clicked), NULL);

    // Exibir a janela
    gtk_widget_show_all(menu_window);

}