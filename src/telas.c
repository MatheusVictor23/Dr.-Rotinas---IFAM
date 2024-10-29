#include <gtk/gtk.h>
#include "telas.h"

void telaRecepcao(GtkBuilder *builder) {
    builder = gtk_builder_new_from_file("../interface/tela_recepcao.glade");
    GtkWidget *tela_recepcao = GTK_WIDGET(gtk_builder_get_object(builder, "tela_recepcao"));
    gtk_widget_show_all(tela_recepcao);
}
