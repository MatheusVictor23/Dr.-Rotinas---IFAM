#include <gtk/gtk.h>
#include "../include/login.h"
#include "../include/telas.h"
#include "../include/gerenciarMedicos.h"
#include "../include/menu.h"

GtkBuilder *builder_menu;

void carregarCss(){
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    const gchar *css_style_file = "../interface/styles/menu.css";

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GError *error = 0;

    gtk_css_provider_load_from_file(provider, g_file_new_for_path(css_style_file), &error);
    g_object_unref(provider);
}

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

    carregarCss();
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