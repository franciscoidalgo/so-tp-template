#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/string.h>

#include "console.h"

#define SUCCESS 0

//Global variables
int done;
command_t* commands;

//Forward declarations
static int execute_line(char* line);
static void intialize_completion();

//Public
void init_console(char* nombre,command_t* _commands){
    
    char* line;

    commands = _commands;
    intialize_completion();

    while(!done){
        line = readline(nombre);
        if(line == NULL)
            break;
        string_trim(&line);
        if(line != NULL){
            if(execute_line(line) != SUCCESS){
                fprintf(stderr, "Error executing: %s\n", line);
            }
            add_history(line);
        }

        free(line); 
    }

}

int console_exit(char* args){
    done = 1;
    return 0;
}

//Static

static char* command_name_generator(const char* text, int state){
    static int list_index, len;
    char* name;

    if(!state){
        list_index = 0;
        len = strlen(text);
    }
    while((name = commands[list_index++].name)){
        if(strncmp(name, text, len) == 0)
            return strdup(name);
    }
    return NULL;
}

static char** completion (const char* text, int start, int end){
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, command_name_generator);
}

static void intialize_completion(){
    rl_attempted_completion_function = completion;
}

static command_t* find_command(char* name, command_t* commands){
    register int i;
    for(i =0; commands[i].name; i++)
        if(string_equals_ignore_case(name, commands[i].name))
            return (&commands[i]);
    return ((command_t*) NULL);
}

static int execute_line (char* line){
    register int i = 0;
    command_t* command;
    char* word;

    while(line[i] && whitespace(line[i]))
        i++;
    word = line + i;
    while(line[i] && !whitespace(line[i]))
        i++;
    if(line[i])
        line[i++] = '\0';
    command = find_command(word, commands);
    if(!command){
        fprintf(stderr, "Command '%s' does not exist.\n", word);
        return 0;
    }
    while(whitespace(line[i]))
        i++;
    word = line + i;
    
    return ((*(command->function)) (word));

}