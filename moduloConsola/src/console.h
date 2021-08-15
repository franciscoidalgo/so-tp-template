#ifndef _CONSOLA_H_
#define _CONSOLA_H_

typedef int function_t (char* arg);

typedef struct{
    char* name;
    function_t *function;
}command_t;

void init_console(char* nombre, command_t* commands);
int console_exit(char* args);

#endif