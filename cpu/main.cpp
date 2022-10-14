#include <stdio.h>
#include "cpu.h"

int main (int argc, char *argv[])
{
        code_t code = {};
        stack stk = {};
        if (oper_stack_ctor(&stk, DEFAULT_CAPACITY, (char*) "stk", (char*) __PRETTY_FUNCTION__, (char*) __FILE__,__LINE__))
                return CTOR_ERROR;
        FILE *source_code = nullptr;
        char input_file_name[MAX_NAME_LENGTH] = {};
        strcpy(input_file_name, argv[1]);
        append_txt(input_file_name);
        source_code = fopen(input_file_name, "r");
        get_code(source_code, &code, input_file_name);
        if (!source_code)
                return NO_SOURCE;

        switch (divide_cmds(&code)) {
        case 1:
                break;
        case 0:
                return 0;
        default:
                return EXECUTE_ERR;
        }

        switch (execute_code(&code, &stk)) {
        case 1:
                break;
        case 0:
                return 0;
        default:
                return EXECUTE_ERR;
        }

        free(code.cmds);
        return 0;
}
