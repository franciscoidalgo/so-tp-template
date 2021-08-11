#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../shared/src/connection.h"

int main (){
    
    int socket_servidor = crear_conexion("127.0.0.1", "4001");
    
    op_code operation_code = recibir_codigo_operacion(socket_servidor);
    t_buffer* buffer = recibir_buffer(socket_servidor);
    t_estudiante* estudiante = deserializar_estudiante(buffer);

    fprintf(stdout, "Nombre: %s\nLegajo: %s\n", estudiante->nombre, estudiante->legajo);
    fflush(stdout);

    free(buffer->stream);
    free(buffer);
    free(estudiante);
    close(socket_servidor);

    return EXIT_SUCCESS;
}