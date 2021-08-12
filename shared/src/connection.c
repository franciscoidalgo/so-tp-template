#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "connection.h"

#define SUCCESS 0
#define FAILED -1

struct addrinfo* generar_info(char* ip, char* puerto){
    struct addrinfo hints, *serv_info;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(ip, puerto, &hints, &serv_info) != SUCCESS)
        perror("Error generando informacion para la ip y el puerto deseados");
    
    return serv_info; 
}

int crear_conexion(char* ip, char* puerto){
    int socket_servidor;
    struct addrinfo *serv_info = generar_info(ip, puerto);
    
    if(
        (socket_servidor = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol)) == FAILED
        || connect(socket_servidor, serv_info->ai_addr, serv_info->ai_addrlen) == FAILED
    ) perror("Error creando conexion");
    freeaddrinfo(serv_info);
    
    return socket_servidor;
}

int iniciar_servidor (char* ip, char* puerto){
    int socket_servidor;
    struct addrinfo *serv_info, *current_info;
    serv_info = generar_info(ip, puerto);

    for(current_info = serv_info; current_info != NULL; current_info = current_info->ai_next){
        if((socket_servidor = socket(current_info->ai_family, current_info->ai_socktype, current_info->ai_protocol)) == FAILED)
            continue;
        if(bind(socket_servidor, current_info->ai_addr, current_info->ai_addrlen) == FAILED){
            close(socket_servidor);
            continue;
        }
        break;
    }
    if(listen(socket_servidor, SOMAXCONN) == FAILED)
        perror("Error iniciando el servidor");    
    freeaddrinfo(serv_info);
    
    return socket_servidor;
}

int esperar_cliente(int socket_servidor){
    int socket_cliente;
    struct sockaddr dir_cliente;
    int tam_direccion = sizeof(struct sockaddr_in);

    socket_cliente = accept(socket_servidor, &dir_cliente,(socklen_t*) &tam_direccion);

    return socket_cliente;
}

op_code recibir_codigo_operacion (int socket_cliente){
    op_code operation_code;
    
    if(recv(socket_cliente, &operation_code, sizeof(op_code), MSG_WAITALL) != SUCCESS)
        return operation_code;
    else{
        close(socket_cliente);
        return FAILED;
    }
}

t_buffer* recibir_buffer (int socket_cliente){
    t_buffer* buffer = malloc(sizeof(t_buffer));
    
    recv(socket_cliente, &(buffer->size), sizeof(uint32_t), MSG_WAITALL);
    buffer->stream = malloc(buffer->size);
    recv(socket_cliente, buffer->stream, buffer->size, MSG_WAITALL);

    return buffer;
}

int calcular_tamanio_paquete (t_package* paquete){
    return (sizeof(op_code) + sizeof(uint32_t) + paquete->buffer->size);
}

void enviar_paquete(int socket_cliente, t_package* paquete){
    void* paquete_serializado = malloc(calcular_tamanio_paquete(paquete));
    int offset = 0;

    memcpy(paquete_serializado + offset, &(paquete->operation_code), sizeof(op_code));
    offset += sizeof(op_code);
    memcpy(paquete_serializado + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(paquete_serializado + offset, paquete->buffer->stream, paquete->buffer->size);
    
    send(socket_cliente, paquete_serializado, calcular_tamanio_paquete(paquete), 0);

    free(paquete_serializado);
}

void enviar_estudiante(int socket_cliente, t_estudiante* estudiante){
    t_package* paquete = malloc(sizeof(t_package));

    paquete->operation_code = ESTUDIANTE;
    paquete->buffer = malloc(sizeof(t_buffer));
    paquete->buffer->size = sizeof(t_estudiante);
    paquete->buffer->stream = malloc(paquete->buffer->size);
    memcpy(paquete->buffer->stream, estudiante, paquete->buffer->size);

    enviar_paquete(socket_cliente, paquete);
    
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);
}

t_estudiante* deserializar_estudiante (t_buffer* buffer){
    t_estudiante* estudiante = malloc(buffer->size);
    int offset = 0;

    memcpy(estudiante->nombre, buffer->stream + offset, 25);
    offset += 25;
    memcpy(estudiante->legajo, buffer->stream + offset, 25);
    
    return estudiante;
}