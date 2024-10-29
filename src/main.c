#include <gtk/gtk.h>
#include "../include/login.h"
#include "../include/telas.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    iniciarLogin();

    gtk_main();
    return 0;
}

