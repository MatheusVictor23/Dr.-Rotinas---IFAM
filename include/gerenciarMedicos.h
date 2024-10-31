#ifndef GERENCIAR_MEDICOS_H
#define GERENCIAR_MEDICOS_H

#include <gtk/gtk.h>

extern GtkBuilder *builder;
extern GtkStack *stack;
extern GtkListStore *medico;

void iniciarGerenciamentoMedico(void);
void on_main_window_destroy_medico(GtkWidget * widget, gpointer data);
void on_main_cadastro_cadastrar_clicked(GtkWidget * widget, gpointer data);
void on_main_cadastro_listar_clicked(GtkWidget * widget, gpointer data);
void on_form_cadastro_cadastrar_clicked(GtkWidget * widget, gpointer data);
void on_form_cadastro_cancelar_clicked(GtkWidget * widget, gpointer data);
void on_medico_listagem_back_clicked(GtkWidget * widget, gpointer data);
void on_medico_listagem_list_clicked(GtkWidget * widget, gpointer data);

#endif // GERENCIAR_MEDICOS_H
