#include <stdio.h>
#include <stdlib.h>

#include "console.h"

//Funciones de los comandos -> int funcion (char*)
int decir_hola(char* args){
    fprintf(stdout, "Hola mundo.\n");
    fflush(stdout);
    return 0;
}

int decir_chau (char* args){
    fprintf(stdout, "Ni nos vimos.\n");
    fflush(stdout);
    return 0;    
}

//Defino los comandos
command_t comandos[] = {
    {"decir_hola", decir_hola},
    {"decir_chau", decir_chau},
    {"exit", console_exit}
};


int main (){
    init_console("Test> ", comandos);
    return EXIT_SUCCESS;
}