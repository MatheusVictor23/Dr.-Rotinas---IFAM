#ifndef MENU_H
#define MENU_H

#include <gtk/gtk.h>

void iniciarMenu();

void on_admin_clicked(GtkButton *button, gpointer user_data);
void on_medico_clicked(GtkButton *button, gpointer user_data);
void on_recep_clicked(GtkButton *button, gpointer user_data);

#endif