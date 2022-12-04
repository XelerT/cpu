#include <stdio.h>
#include "cpu.h"

#define $ fprintf(stderr, "I'm here. File %s Line %d\n", __FILE__, __LINE__);
#define $

int main (int argc, char *argv[])
{
        if (argc != 2)
                return fprintf(stderr, "Nuzna 2 argumenta\n"), EXIT_FAILURE;

        code_t code = {};
        stack stk = {};
        if (oper_stack_ctor(&stk, DEFAULT_CAPACITY, (char*) "stk",
                            (char*) __PRETTY_FUNCTION__, (char*) __FILE__, __LINE__))
                return CTOR_ERROR;
        $
        cpu_t cpu = {};

        FILE *source_code = nullptr;
        char input_file_name[MAX_NAME_LENGTH] = {};
        if (source_file_ctor(source_code, input_file_name, argv[1])) {
                $
                return NO_SOURCE;
        }

        $
        source_code = fopen(input_file_name, "r");
        get_code(source_code, &code, input_file_name);
        $

        int error = 0;

        if (error = divide_cmds(&code)) {
                free(code.cmds);
                return error;
        }

        $
        error = execute_code(&code, &stk, &cpu);
        free(code.cmds);
        return error;
}
