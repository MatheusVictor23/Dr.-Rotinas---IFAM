#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <login.h>


void testeDadosBin() {
    // Dados a serem escritos no arquivo
    Usuario usuarios[] = {
        {"admin", "admin123", ADMIN},
        {"medico", "medico123", MEDICO},
        {"recepcionista", "recep123", RECEPCIONISTA}
    };

    // Abrindo o arquivo binário para escrita no caminho especificado
    FILE *arquivo = fopen("../data/usuarios.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo");
        return;
    }

    // Escrevendo os dados no arquivo
    size_t numUsuarios = sizeof(usuarios) / sizeof(Usuario);
    fwrite(usuarios, sizeof(Usuario), numUsuarios, arquivo);

    // Fechando o arquivo
    fclose(arquivo);

}

int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    testeDadosBin();

    iniciarMenu();

    gtk_main();
    
    return 0;
}

