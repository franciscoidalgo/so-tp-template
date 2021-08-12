#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../shared/src/connection.h"

int main (){  
    int socket_servidor = iniciar_servidor("127.0.0.1", "4444");
    int socket_cliente = esperar_cliente(socket_servidor);

    t_estudiante estudiante = {.legajo="1234567\0", .nombre="Nombre generico\0"};
    enviar_estudiante(socket_cliente, &estudiante);
    
    close(socket_servidor);

    return EXIT_SUCCESS;
}