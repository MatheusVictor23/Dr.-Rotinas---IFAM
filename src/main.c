#include <gtk/gtk.h>
#include "../include/login.h"
#include "../include/telas.h"
#include <locale.h>

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "pt_BR.UTF-8");

    gtk_init(&argc, &argv);

    iniciarLogin();

    gtk_main();
    return 0;
}

