#include <stdio.h>
#include "cpu.h"

int main (int argc, char *argv[])
{
        code_t code = {};
        stack stk = {};
        if (oper_stack_ctor(&stk, DEFAULT_CAPACITY, (char*) "stk",
                            (char*) __PRETTY_FUNCTION__, (char*) __FILE__, __LINE__))
                return CTOR_ERROR;

        cpu_t cpu = {};
// func
        FILE *source_code = nullptr;
        char input_file_name[MAX_NAME_LENGTH] = {};    // make ctor
        strcpy(input_file_name, argv[1]);
        append_txt(input_file_name);

        source_code = fopen(input_file_name, "r");
        if (!source_code)
                return NO_SOURCE;
        get_code(source_code, &code, input_file_name);

// Address sanitizer
        int error = 0;

        if (error = divide_cmds(&code)) {
                free(code.cmds);
                return error;
        }

        error = execute_code(&code, &stk, &cpu);
        free(code.cmds);

        return error;
}
