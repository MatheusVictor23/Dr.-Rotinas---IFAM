// #include <gtk/gtk.h>
// #include "../include/login.h"
// #include "../include/telas.h"
// #include "../include/gerenciarMedicos.h"
// #include "../include/menu.h"

// void testeDadosBin() {
//     // Dados a serem escritos no arquivo
//     Usuario usuarios[] = {
//         {"admin", "admin123", ADMIN},
//         {"medico", "medico123", MEDICO},
//         {"recepcionista", "recep123", RECEPCIONISTA}
//     };

//     // Abrindo o arquivo binário para escrita no caminho especificado
//     FILE *arquivo = fopen("../data/usuarios.bin", "wb");
//     if (arquivo == NULL) {
//         g_message("Erro ao abrir o arquivo");
//         return;
//     }

//     // Escrevendo os dados no arquivo
//     size_t numUsuarios = sizeof(usuarios) / sizeof(Usuario);
//     fwrite(usuarios, sizeof(Usuario), numUsuarios, arquivo);

//     // Fechando o arquivo
//     fclose(arquivo);

//     g_message("Dados escritos com sucesso no arquivo binário.\n");
// }

// int main(int argc, char *argv[]) {

//     gtk_init(&argc, &argv);

//     testeDadosBin();

//     iniciarMenu();

//     gtk_main();
    
//     return 0;
// }

