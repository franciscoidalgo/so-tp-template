#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <stdint.h>

typedef enum{
    MESSAGE,
    ESTUDIANTE
}op_code;

typedef struct{
    uint32_t size;
    void* stream;
}t_buffer;

typedef struct{
    op_code operation_code;
    t_buffer* buffer;
}t_package;

typedef struct{
    char nombre[25];
    char legajo[25];
}__attribute__((packed)) t_estudiante;

int crear_conexion(char* ip, char* puerto);
int iniciar_servidor (char* ip, char* puerto);
int esperar_cliente(int socket_servidor);
op_code recibir_codigo_operacion (int socket_cliente);
t_buffer* recibir_buffer (int socket_cliente);
void enviar_paquete(int socket_cliente, t_package* paquete);
void enviar_estudiante(int socket_cliente, t_estudiante* estudiante);
t_estudiante* deserializar_estudiante (t_buffer* buffer);

#endif