#include <gtk/gtk.h>
#include "../include/login.h"
#include "../include/telas.h"
#include "../include/gerenciarMedicos.h"
#include "../include/menu.h"

int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    iniciarMenu();

    gtk_main();
    
    return 0;
}

